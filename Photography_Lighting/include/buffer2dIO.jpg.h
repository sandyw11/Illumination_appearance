///////////////////////////////
// JPG importer and exporter //
///////////////////////////////

#ifndef _BUFFER2DIO_JPG_H_
#define _BUFFER2DIO_JPG_H_

#include "buffer2dIO.util.h"
#include "exceptions.h"
#include "tempArray.h"

#ifdef INCLUDE_JPEG

extern "C" {
  #include "jpeglib.h"
}

#endif /* INCLUDE_JPEG */

using namespace std;

namespace io {
  namespace jpg {

#ifndef INCLUDE_JPEG

    template<typename Buffer, typename C>
      void _export(const string& filename, const Buffer& buf, const typename C::value_type& pad=0, float compressionQuality=0.95f) { throw unsupportedFormat(); }

    template<typename Buffer, typename C>
      void _import(const string& filename, Buffer& buf, const typename C::value_type& pad=0) { throw unsupportedFormat(); }

#else /* INCLUDE_JPEG */

    ////////////////
    // Export PNG //
    ////////////////
    template<typename Buffer, typename C>
      void _export(const string& filename, const Buffer& buf, const typename C::value_type& pad=0, float compressionQuality=0.95f)
    {
      // open file
      FILE *fp = fopen(filename.c_str(), "wb");
      if(!fp) throw fileNotFound(filename, "writing");

      // setup data stuctures
      struct jpeg_compress_struct cinfo;
      struct jpeg_error_mgr jerr;
      
      // allocate and init
      cinfo.err = jpeg_std_error(&jerr);
      jpeg_create_compress(&cinfo);
      
      // use fp for output
      jpeg_stdio_dest(&cinfo, fp);

      // set compression parameters
      cinfo.image_width = buf.width();
      cinfo.image_height = buf.height();
      cinfo.input_components = 3;
      cinfo.in_color_space = JCS_RGB;

      jpeg_set_defaults(&cinfo);
      jpeg_set_quality (&cinfo, compressionQuality*100, true);
  
      // start compression
      jpeg_start_compress(&cinfo, TRUE);

      // allocate buffer
      JSAMPLE *buffer = new JSAMPLE[buf.width() * 3];
      JSAMPROW rowPointer[1] = {buffer};

      // write image per scanline and convert
      typename Buffer::const_iterator itr = buf.begin();
      for(unsigned int y=0; y < buf.height(); y++, itr+=buf.width())
      {
        // convert
	JSAMPLE* bufferItr = &(buffer[0]);
        ::io::util::convertPixelToFlat<typename Buffer::const_iterator, JSAMPLE*, C>(itr, itr+buf.width(), bufferItr, 3, pad);

	// feed to libjpeg
	jpeg_write_scanlines(&cinfo, rowPointer, 1);
      }

      // compress
      jpeg_finish_compress(&cinfo);

      // clean up
      jpeg_destroy_compress(&cinfo);
      delete[] buffer;

      // Done.
      fclose(fp);
    }

    ////////////////
    // Import PNG //
    ////////////////
    template<typename Buffer, typename C>
      void _import(const string& filename, Buffer& result, const typename C::value_type& pad=0)
    {
      // read temp buffer
      FILE *fp = fopen(filename.c_str(), "rb");
      if(!fp) throw fileNotFound(filename, "reading");

      // setup data structures
      struct jpeg_decompress_struct cinfo;
      struct jpeg_error_mgr jerr;  

      // allocate and init
      cinfo.err = jpeg_std_error(&jerr);
      jpeg_create_decompress(&cinfo);

      // load from fp
      jpeg_stdio_src(&cinfo, fp);

      // read parameters & start decompress
      jpeg_read_header(&cinfo, TRUE);
      jpeg_start_decompress(&cinfo);   

      // allocate buffer
      JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, 3 * cinfo.output_width, 1);
      result.resize(cinfo.output_width, cinfo.output_height);

      // read scanline and convert
      typename Buffer::iterator itr = result.begin();
  for(unsigned int y=0; y < result.height(); y++, itr += result.width())
      {
	// read
	jpeg_read_scanlines(&cinfo, buffer, 1);

        // convert
        ::io::util::convertFlatToPixel<JSAMPLE*, typename Buffer::iterator, C>(&((*buffer)[0]), 3, itr, itr+result.width(), pad);

	// next scanline
      }
      
      // finish decompression
      jpeg_finish_decompress(&cinfo);

      // clean up
      jpeg_destroy_decompress(&cinfo);
      fclose(fp);
    }

#endif /* INCLUDE_JPEG */

  } // end png namespace
} // end jpg namespace

#endif /* _BUFFER2DIO_JPG_H_ */
