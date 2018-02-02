///////////////////////////////
// PPM importer and exporter //
///////////////////////////////

#ifndef _BUFFER2DIO_PPM_H_
#define _BUFFER2DIO_PPM_H_

#include "Endian.h"
#include "tempArray.h"
#include "exceptions.h"
#include "buffer2dIO.util.h"

#include <cstdio>
#include <stdint.h>
#include <boost/limits.hpp>

using namespace std;

namespace io {
  namespace ppm {
    
    namespace detail {
      
      /////////////////////////////////
      // Skip Comments in the Header //
      /////////////////////////////////
      void skipComment(FILE *fp, char token='#')
      {
	int b = fgetc(fp);

	while(b != EOF && (b == token))
	{
	  while (b != EOF && b != '\n' && b != '\r')
	    b = fgetc(fp);
	  
	  b = fgetc(fp);
	}
	ungetc(b, fp);
      }

    } // namespace detail


    enum bit_depth {
      PPM8BIT = 8,
      PPM16BIT = 16,
      PPM32BIT = 32          // WARNING: unofficial extension!
    };

  ////////////////
  // Export PPM //
  ////////////////
  template<typename Buffer, typename C>
    void _export(const string& filename, const Buffer& buf, bit_depth bitDepth=PPM8BIT, const typename C::value_type& pad=0)
    {
      // sanity check
      if(buf.width() == 0 || buf.height() == 0) throw buffer2dIllegalSize();

      // determine number of channels
      C wrapper(*buf.begin());
      unsigned int numChannels = std::distance(wrapper.begin(), wrapper.end());
      if(numChannels > 3) throw customException("PPM exporter: source buffer contains more than 3 channels.");

      std::string magicIdentifier = (numChannels == 1) ? "P5" : "P6";

      // allocate & convert temporary buffer
      unsigned long maxValue = 0;
      unsigned long tempBufferSize = buf.width() * buf.height() * numChannels;
      tempArray(uint8_t, tempBuffer, tempBufferSize * bitDepth / 8);

      if(bitDepth == PPM8BIT)
      {
        maxValue = std::numeric_limits<uint8_t>::max();
        ::io::util::convertPixelToFlat<typename Buffer::const_iterator, uint8_t*, C>(buf.begin(), buf.end(), tempArrayBegin(uint8_t, tempBuffer, tempBufferSize), numChannels, pad);
      }
      else if(bitDepth == PPM16BIT)
      {
        maxValue = std::numeric_limits<uint16_t>::max();
        ::io::util::convertPixelToFlat<typename Buffer::const_iterator, uint16_t*, C>(buf.begin(), buf.end(), tempArrayBegin(uint16_t, tempBuffer, tempBufferSize), numChannels, pad);
        endian::big(tempArrayBegin(uint16_t, tempBuffer, tempBufferSize), tempArrayEnd(uint16_t, tempBuffer, tempBufferSize));
      }
      else if(bitDepth == PPM32BIT)
      {
        maxValue = std::numeric_limits<uint32_t>::max();
        ::io::util::convertPixelToFlat<typename Buffer::const_iterator, uint32_t*, C>(buf.begin(), buf.end(), tempArrayBegin(uint32_t, tempBuffer, tempBufferSize), numChannels, pad);
        endian::big(tempArrayBegin(uint32_t, tempBuffer, tempBufferSize), tempArrayEnd(uint32_t, tempBuffer, tempBufferSize));
      }

      // write temp buffer to file
      FILE *fp = fopen(filename.c_str(), "wb");
      char dummy='\n';
      fprintf(fp,"%s%c%lu %lu%c%lu%c", magicIdentifier.c_str(), dummy, (unsigned long)(buf.width()), (unsigned long)(buf.height()), dummy, maxValue, dummy);
      fwrite(tempBuffer, bitDepth/8, tempBufferSize, fp);
      fclose(fp);      

      // done.
    }


  ////////////////
  // Import PPM //
  ////////////////
  template<typename Buffer, typename C>
    void _import(const string& filename, Buffer& result, const typename C::value_type& pad=0)
    {
      // read temp buffer
      char dummy, type;
      unsigned long width, height, colors;
      FILE *fp = fopen(filename.c_str(), "rb");

      if(!fp) throw fileNotFound(filename);

      // scan header
      int res = fscanf(fp, "P%c%c", &type, &dummy);
      detail::skipComment(fp);
      res += fscanf(fp, "%lu%c%lu%c", &width, &dummy, &height, &dummy);
      detail::skipComment(fp);
      res += fscanf(fp, "%lu%c", &colors, &dummy);

      if (res != 8) throw customException("PPM importer: failed to read header."); 

      // Determine number of color channels
      unsigned numChannels;
      if(type == '6') numChannels = 3;
      else if(type == '5') numChannels = 1;
      else throw unsupportedFormat();

      // get bit depth
      bit_depth bitDepth;
      if(colors == std::numeric_limits<uint8_t>::max()) bitDepth = PPM8BIT;
      else if(colors == std::numeric_limits<uint16_t>::max()) bitDepth = PPM16BIT;
      else if(colors == std::numeric_limits<uint32_t>::max()) bitDepth = PPM32BIT;
      else unsupportedFormat();

      // read buffer
      unsigned long tempBufferSize = width * height * numChannels;
      tempArray(uint8_t, tempBuffer, tempBufferSize * (bitDepth / 8));
      fread(tempArrayBegin(uint8_t, tempBuffer, tempBufferSize), bitDepth / 8,  tempBufferSize, fp);
      fclose(fp);


      // allocate
      result.resize(width, height);
      
      if(bitDepth == PPM8BIT)
	::io::util::convertFlatToPixel<const uint8_t*, typename Buffer::iterator, C>(tempArrayBegin(uint8_t, tempBuffer, tempBufferSize), numChannels, result.begin(), result.end(), pad);
      else if(bitDepth == PPM16BIT)
      {
        endian::big(tempArrayBegin(uint16_t, tempBuffer, tempBufferSize), tempArrayEnd(uint16_t, tempBuffer, tempBufferSize));
	::io::util::convertFlatToPixel<const uint16_t*, typename Buffer::iterator, C>(tempArrayBegin(uint16_t, tempBuffer, tempBufferSize), numChannels, result.begin(), result.end(), pad);
      }
      else if(bitDepth == PPM32BIT)
      {
        endian::big(tempArrayBegin(uint32_t, tempBuffer, tempBufferSize), tempArrayEnd(uint32_t, tempBuffer, tempBufferSize));
	::io::util::convertFlatToPixel<const uint32_t*, typename Buffer::iterator, C>(tempArrayBegin(uint32_t, tempBuffer, tempBufferSize), numChannels, result.begin(), result.end(), pad);
      }

      // done.
    }

  } // namespace ppm
}   // namespace io

#endif /* _BUFFER2DIO_PPM_H_ */

