#ifndef _IMAGEIO_INLINE_H_
#define _IMAGEIO_INLINE_H_

#include "imageIO.h"

namespace io {

/////////////////
// importImage //
/////////////////
inline void importImage(const std::string& filename, image& result)
{
  // determine type based on extension
  std::string ext = filename.substr(filename.size() - 3, 3);
  
  if(ext == "PFM" || ext == "pfm") importPFM(filename, result);
  else if(ext == "PPM" || ext == "ppm" || ext == "PNM" || ext == "pnm") importPPM(filename, result);
  else if(ext == "EXR" || ext == "exr") importEXR(filename, result);
  else if(ext == "PNG" || ext == "png") importPNG(filename, result);
  else if(ext == "JPG" || ext == "jpg" || ext == "JPEG" || ext == "jpeg") importJPG(filename, result);
  else if(ext == "TIF" || ext == "tif" || ext == "TIFF" || ext == "tiff") importTIF(filename, result);
  else throw unsupportedFormat();
  
  // done.
}


/////////////////
// exportImage //
/////////////////
inline void exportImage(const std::string& filename, const image& source)
{
  // determine type based on extension
  std::string ext = filename.substr(filename.size() - 3, 3);
  
  if(ext == "PFM" || ext == "pfm") exportPFM(filename, source);
  else if(ext == "PPM" || ext == "ppm" || ext == "PNM" || ext == "pnm") exportPPM(filename, source);
  else if(ext == "EXR" || ext == "exr") exportEXR(filename, source);
  else if(ext == "PNG" || ext == "png") exportPNG(filename, source);
  else if(ext == "JPG" || ext == "jpg" || ext == "JPEG" || ext == "jpeg") exportJPG(filename, source);
  else if(ext == "TIF" || ext == "tif" || ext == "TIFF" || ext == "tiff") exportTIF(filename, source);
  else throw unsupportedFormat();
  
  // done.
}


} // end io namespace

#endif /* _IMAGEIO_INLINE_H_ */
