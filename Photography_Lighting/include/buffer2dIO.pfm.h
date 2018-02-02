///////////////////////////////
// PFM importer and exporter //
///////////////////////////////

#ifndef _BUFFER2DIO_PFM_H_
#define _BUFFER2DIO_PFM_H_

#include "Endian.h"
#include "exceptions.h"
#include "buffer2dIO.util.h"
#include <cstdio>

using namespace std;

namespace io {
  namespace pfm {

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

  ////////////////
  // Export PFM //
  ////////////////
  template<typename Buffer, typename C>
    void _export(const string& filename, const Buffer& buf, const typename C::value_type& pad=0)
    {
      // sanity check
      if(buf.width() == 0 || buf.height() == 0) throw buffer2dIllegalSize();

      // create temp buffer
      typedef float   dest_type;
      buffer2d<dest_type> temp(buf.width() * 3, buf.height());
      ::io::util::convertPixelToFlat<typename Buffer::const_iterator, typename buffer2d<dest_type>::iterator, C>(buf.begin(), buf.end(), temp.begin(), 3, pad);
      endian::little(temp.begin(), temp.end());

      // write temp buffer to file
      FILE *fp = fopen(filename.c_str(), "wb");
      char dummy='\n';
      fprintf(fp,"PF%c%lu %lu%c-1.000000%c", dummy, (unsigned long)(buf.width()), (unsigned long)(buf.height()), dummy, dummy);
      fwrite(temp.begin(), sizeof(dest_type), temp.size(), fp);
      fclose(fp);      

      // done.
    }


  ////////////////
  // Import PFM //
  ////////////////
  template<typename Buffer, typename C>
    void _import(const string& filename, Buffer& result, const typename C::value_type& pad=0)
    {
      // read temp buffer
      char dummy, type;         // type is 'f' or 'F'
      float endianess;          // endianess is -1 (LE) or 1 (BE)
      int width, height;
      FILE *fp = fopen(filename.c_str(), "rb");

      if(!fp) throw fileNotFound(filename);

      // scan header
      int res = 0;
      res += fscanf(fp, "P%c%c", &type, &dummy);
      detail::skipComment(fp);
      res += fscanf(fp, "%d%c%d%c", &width, &dummy, &height, &dummy);
      detail::skipComment(fp);
      res += fscanf(fp, "%g%c", &endianess, &dummy);

      // Check if PFM file format
      if (res != 8) throw unsupportedFormat(); 
      if (type != 'f' && type != 'F') throw unsupportedFormat();

      // Determine number of channels
      unsigned int channels = (type == 'f') ? 1 : 3;
      bool bigEndian = (endianess > 0);

      // pixel convertor
      typedef float   src_type;
      //      ::io::util::pixelTypeConvertor<src_type, typename C::value_type> convertor;
      //typename C::value_type cpad = convertor(pad);

      // read buffer
      buffer2d<src_type> temp(width*channels, height);
      fread(temp.begin(), sizeof(src_type),  temp.size(), fp);
      fclose(fp);

      // endian
      if(bigEndian) endian::big(temp.begin(), temp.end());
      else endian::little(temp.begin(), temp.end());

      // allocate & convert
      result.resize(width, height);
      ::io::util::convertFlatToPixel<typename buffer2d<src_type>::const_iterator, typename Buffer::iterator, C>(temp.begin(), 3, result.begin(), result.end(), pad);

      // done.
    }

  } // namespace pfm
}   // namespace io

#endif /* _BUFFER2DIO_PFM_H_ */
