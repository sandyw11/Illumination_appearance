#ifndef _IMAGEIO_H_
#define _IMAGEIO_H_

#include <string>

#include "image.h"
#include "buffer2dIO.h"

namespace io {

  void importImage(const std::string& filename, image& result);
  void exportImage(const std::string& filename, const image& source);

}  // end io namespace


////////////////////
// Inline Methods //
////////////////////
#include "imageIO.inline.h"

#endif /* _IMAGEIO_H_ */
