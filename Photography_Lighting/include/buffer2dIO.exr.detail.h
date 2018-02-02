//////////////////////////////////////////////////////
// EXR Importer and exporter implementation details //
//////////////////////////////////////////////////////

#if !defined(_BUFFER2DIO_EXR_DETAIL_H_) && defined(_BUFFER2DIO_EXR_H_)
#define _BUFFER2D_EXR_DETAIL_H_

#include <stdint.h>
#include "half.h"
#include "exceptions.h"
#include "buffer2dIO.exr.options.h"

namespace io {
  namespace exr {
    namespace detail {  
      
  unsigned int bytesPerPixel(const ::io::exr::pixel& pixelType)
  {
    if(pixelType == ::io::exr::UINT) return sizeof(uint32_t);
    else if(pixelType == ::io::exr::HALF) return sizeof(half);
    else if(pixelType == ::io::exr::FLOAT) return sizeof(float);
    else throw customException("Unknown pixel type in EXR options.");
  }


  template<typename S, typename Swrapper>
    void convertPixelToFlat(const S& src_begin, const S& src_end,
			    const void* dst_begin, unsigned int channels,
			    const ::io::exr::pixel& pixelType, 
			    const typename Swrapper::value_type& pad=0)
  {
    if(pixelType == ::io::exr::UINT) ::io::util::convertPixelToFlat<S, uint32_t*, Swrapper>(src_begin, src_end, (uint32_t*)(dst_begin), channels, pad);
    else if(pixelType == ::io::exr::HALF) ::io::util::convertPixelToFlat<S, half*, Swrapper>(src_begin, src_end, (half*)(dst_begin), channels, pad);
    else if(pixelType == ::io::exr::FLOAT) ::io::util::convertPixelToFlat<S, float*, Swrapper>(src_begin, src_end, (float*)(dst_begin), channels, pad);
    else throw customException("Uknown EXR pixel type.");
  }


  template<typename D, typename Dwrapper>
    void convertFlatToPixel(const void* src_begin, unsigned int channels,
			    const D& dst_begin, const D& dst_end,
			    const ::io::exr::pixel& pixelType,
			    const typename Dwrapper::value_type& pad=0)
  {
    if(pixelType == ::io::exr::UINT) ::io::util::convertFlatToPixel<const uint32_t*, D, Dwrapper>((const uint32_t*)(src_begin), channels, dst_begin, dst_end, pad);
    else if(pixelType == ::io::exr::HALF) ::io::util::convertFlatToPixel<const half*, D, Dwrapper>((const half*)(src_begin), channels, dst_begin, dst_end, pad);
    else if(pixelType == ::io::exr::FLOAT) ::io::util::convertFlatToPixel<const float*, D, Dwrapper>((const float*)(src_begin), channels, dst_begin, dst_end, pad);
    else throw customException("Unknown EXR pixel type.");
  }

    }  // end detail namespace
  }    // end exr namespace
}      // end io namespace

#endif /* _BUFFER2D_EXR_DETAIL_H_ */

