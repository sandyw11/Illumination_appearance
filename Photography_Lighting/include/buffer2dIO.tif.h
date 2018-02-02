///////////////////////////////////////////////////
// TIFF Importer/Exporter.  Requires libTIFF.    //
//  Supports a limited set of available options  //
//  Does not support any color space conversions //
///////////////////////////////////////////////////

#ifndef _BUFFER2DIO_TIF_H_
#define _BUFFER2DIO_TIF_H_

#include "exceptions.h"
#include "buffer2dIO.tif.options.h"

#ifdef INCLUDE_TIFF

extern "C" {
  #include "tiff.h"
  #include "tiffio.h"
}

#include "offset_iterator.h"
#include "buffer2dIO.tif.detail.h"

#endif /* INCLUDE_TIFF */

namespace io {
  namespace tif {

#ifndef INCLUDE_TIFF

    template<typename Buffer, typename C>
      void _export(const string& filename, const Buffer& buf, float pad=0.0f, const options& opt=options()) { throw unsupportedFormat(); }

    template<typename Buffer, typename C>
      void _import(const string& filename, Buffer& buf, float pad=0.0f) { throw unsupportedFormat(); }


#else /* INCLUDE_TIFF */
#include "buffer2dIO.tif.detail.h"


    /////////////////
    // Export TIFF //
    /////////////////
    template<typename Buffer, typename C>
      void _export(const string& filename, const Buffer& buf, float pad=0.0f, const options& opt=options())
    {
      // sanity check
      if(buf.width() == 0 || buf.height() == 0) throw buffer2dIllegalSize();

      // determine number of channels
      const C wrapper(*(buf.begin()));
      unsigned int numChannels = std::distance( wrapper.begin(), wrapper.end() );

      // open the tiff image
      TIFF *fp = TIFFOpen(filename.c_str(), "w");
      if(!fp) throw fileNotFound(filename, "writing");

      // create header
      int headerSuccess = 1;
      headerSuccess &= TIFFSetField(fp, TIFFTAG_SAMPLESPERPIXEL, (uint16)(numChannels));
      headerSuccess &= TIFFSetField(fp, TIFFTAG_IMAGEWIDTH, (uint32)(buf.width()));
      headerSuccess &= TIFFSetField(fp, TIFFTAG_IMAGELENGTH, (uint32)(buf.height()));
      headerSuccess &= TIFFSetField(fp, TIFFTAG_SAMPLEFORMAT, (uint16)(opt.sampleFormat()));
      headerSuccess &= TIFFSetField(fp, TIFFTAG_BITSPERSAMPLE, (uint16)(opt.bitsPerSample()));
      headerSuccess &= TIFFSetField(fp, TIFFTAG_COMPRESSION, (uint16)(opt.compressionType()));
      headerSuccess &= TIFFSetField(fp, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
      headerSuccess &= TIFFSetField(fp, TIFFTAG_ROWSPERSTRIP, 1);
      headerSuccess &= TIFFSetField(fp, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
      
      if(headerSuccess != 0x01) throw customException("TIFF exporter failed to write header.");

      // Allocate memory
      tdata_t buffer = _TIFFmalloc(buf.width() * numChannels * opt.bitsPerSample() / 8);
      if(buffer == NULL) 
	throw customException("TIFF exporter failed to allocate necessary write buffers.");

      // Pass data to libTIFF
      typename Buffer::const_iterator scan_itr = buf.begin();
      for(unsigned int y=0; y < buf.height(); y++, scan_itr+=buf.width())
      {
	// convert
	detail::convertPixelToFlat<typename Buffer::const_iterator, C>(scan_itr, scan_itr+buf.width(), buffer, numChannels, opt.sampleFormat(), opt.bitsPerSample(), pad);

	// write
	if(TIFFWriteScanline(fp, buffer, (uint32)(y)) != 1)
	  throw customException("TIFF exporter failed to write to file.");
      }

      // free memory
      _TIFFfree(buffer);

      // close the tiff image
      TIFFClose(fp);
    }


    /////////////////
    // Import TIFF //
    /////////////////
    template<typename Buffer, typename C>
      void _import(const string& filename, Buffer& buf, const typename C::value_type& pad=0.0f)
    {
      // open the tiff image
      TIFF *fp = TIFFOpen(filename.c_str(), "r");
      if(!fp) throw fileNotFound(filename, "reading");

      // read the header
      int headerSuccess = 1;
      uint32_t header_width, header_height;
      uint16_t header_channels, header_bitsPerSample, header_config, header_sampleFormat;

      headerSuccess &= TIFFGetField(fp, TIFFTAG_IMAGEWIDTH, &header_width);
      headerSuccess &= TIFFGetField(fp, TIFFTAG_IMAGELENGTH, &header_height);
      headerSuccess &= TIFFGetField(fp, TIFFTAG_BITSPERSAMPLE, &header_bitsPerSample);
      if(headerSuccess != 0x01) throw customException("TIFF importer failed to read header.");

      if(TIFFGetField(fp, TIFFTAG_SAMPLESPERPIXEL, &header_channels) != 1) header_channels = 1;
      if(TIFFGetField(fp, TIFFTAG_SAMPLEFORMAT, &header_sampleFormat) != 1) header_sampleFormat = 1;
      if(TIFFGetField(fp, TIFFTAG_PLANARCONFIG, &header_config) != 1) header_config = PLANARCONFIG_CONTIG;

      // allocate memory
      buf.resize(header_width, header_height);

      tdata_t buffer = _TIFFmalloc(TIFFScanlineSize(fp));
      if(buffer == NULL) 
	throw customException("TIFF importer failed to allocate necessary read buffers.");


      // decode scaneline per scanline based on header_config
      //   => Chunky config
      if(header_config == PLANARCONFIG_CONTIG)
      {
	// scanline is stored with mixed channels
	typename Buffer::iterator scan_itr = buf.begin();
	for(uint32 y=0; y < buf.height(); y++, scan_itr+=buf.width())
	{
	  // read
	  if(TIFFReadScanline(fp, buffer, y) != 1) throw customException("TIFF importer failed to read scanline.");
	  
	  // convert
	  detail::convertFlatToPixel<typename Buffer::iterator, C>(buffer, header_channels, scan_itr, scan_itr+buf.width(), static_cast<sample_format>(header_sampleFormat), header_bitsPerSample, pad);
	}
      }

      //   => Separate config (uncommon)
      else if(header_config == PLANARCONFIG_SEPARATE)
      {
	// determine number of channels
	const C wrapper(*(buf.begin()));
	unsigned int availableChannels = std::distance( wrapper.begin(), wrapper.end() );
      
	// scanline is stored with seperated channels
	for(uint16 c=0; c < availableChannels; c++)
	{
	  offset_iterator<typename Buffer::iterator> scan_itr(buf.begin(), c);

	  // if channel exists in TIFF, copy
	  if(c < header_channels)
	    for(uint32 y=0; y < buf.height(); y++, scan_itr+=buf.width())
	    {
	      // read scanline
	      if(TIFFReadScanline(fp, buffer, y, c) != 1) throw customException("TIFF importer failed to read scanline.");

	      // convert
	      detail::convertFlatToPixel<offset_iterator<typename Buffer::iterator>, iteratorWrapper<typename C::value_type> >(buffer, 1, scan_itr, scan_itr+buf.width(), static_cast<sample_format>(header_sampleFormat), header_bitsPerSample, pad);	      
	    }

	  // else pad
	  else std::fill(scan_itr, offset_iterator<typename Buffer::iterator>(buf.end(),c), pad);
	}
      }

      //   => Unknown config
      else throw customException("TIFF importer: unsupported planar config.");

      // close the tiff image
      _TIFFfree(buffer);
      TIFFClose(fp);
    }


#endif /* INCLUDE_TIFF */

  } // end tif namespace
} // end io namespace


#endif /* _BUFFER2DIO_TIF_H_ */
