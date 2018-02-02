#include <iostream>
#include <string>

#include "image.h"
#include "imageIO.h"

int main(int argc, char** argv)
{
  // create a list of supported formats
  std::string support_list = "ppm, pfm";
#ifdef INCLUDE_PNG                   
  support_list = support_list + ", png";
#endif /* INCLUDE_PNG */             
#ifdef INCLUDE_OPENEXR                   
  support_list = support_list + ", exr";
#endif /* INCLUDE_OPENEXR */             
#ifdef INCLUDE_JPEG                   
  support_list = support_list + ", jpg";
#endif /* INCLUDE_JPEG */             
#ifdef INCLUDE_TIFF                   
  support_list = support_list + ", tif";
#endif /* INCLUDE_TIFF */             

  // parse commend line
  if(argc != 3)
  {
    std::cerr << "Usage: " << argv[0] << " <input image name> <output image name>" << std::endl;
    std::cerr << "  * Supported formats: " << support_list << std::endl;
    return -1;
  }

  // load and save images
  image img;
  ::io::importImage(argv[1], img);
  float width = img.width();
  float height = img.height();
  float size = img.size();
  float red0 = img(0,0).r;
  float green0 = img(0,0).g;
  float blue0 = img(0,0).b;  
  float red1 = img(width-1,height-1).r;
  float green1 = img(width-1,height-1).g;
  float blue1 = img(width-1,height-1).b;  
  std::cerr <<width << std::endl;
  std::cerr <<height << std::endl;
  std::cerr <<size << std::endl;
  std::cerr <<red0 << std::endl;
  std::cerr <<green0 << std::endl;
  std::cerr <<blue0 << std::endl;
  std::cerr <<red1 << std::endl;
  std::cerr <<green1 << std::endl;
  std::cerr <<blue1 << std::endl;  
  
  ::io::exportImage(argv[2], img);

  // Done.
  return 0;
}
