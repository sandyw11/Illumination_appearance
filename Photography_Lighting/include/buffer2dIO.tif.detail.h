///////////////////////////////////////////////////////
// TIFF Importer and Exporter implementation details //
///////////////////////////////////////////////////////

#if !defined(_BUFFER2DIO_TIF_DETAIL_H_) && defined(_BUFFER2DIO_TIF_H_)
#define _BUFFER2D_TIF_DETAIL_H_

#include <stdint.h>
#include "exceptions.h"
#include "buffer2dIO.tif.options.h"

namespace detail {  // io::tif::detail

      template<typename S, typename Swrapper>
	void convertPixelToFlat(const S& src_begin, const S& src_end,
				const void* dst_begin, unsigned int channels, 
				const ::io::tif::sample_format& sampleFormat, unsigned int bitsPerSample,
				const typename Swrapper::value_type& pad=0)
      {
	// handle unsigned int case
	if(sampleFormat == ::io::tif::UINT)
	{
	  if(bitsPerSample == 8) ::io::util::convertPixelToFlat<S, uint8_t*, Swrapper>(src_begin, src_end, (uint8_t*)(dst_begin), channels, pad);
	  else if(bitsPerSample == 16) ::io::util::convertPixelToFlat<S, uint16_t*, Swrapper>(src_begin, src_end, (uint16_t*)(dst_begin), channels, pad);
	  else if(bitsPerSample == 32) ::io::util::convertPixelToFlat<S, uint32_t*, Swrapper>(src_begin, src_end, (uint32_t*)(dst_begin), channels, pad);
	  else throw customException("Unknown UINT pixel depth in TIF.");
	}

	// handle signed int case
	else if(sampleFormat == ::io::tif::INT)
	{
	  if(bitsPerSample == 8) ::io::util::convertPixelToFlat<S, int8_t*, Swrapper>(src_begin, src_end, (int8_t*)(dst_begin), channels, pad);
	  else if(bitsPerSample == 16) ::io::util::convertPixelToFlat<S, int16_t*, Swrapper>(src_begin, src_end, (int16_t*)(dst_begin), channels, pad);
	  else if(bitsPerSample == 32) ::io::util::convertPixelToFlat<S, int32_t*, Swrapper>(src_begin, src_end, (int32_t*)(dst_begin), channels, pad);
	  else throw customException("Unknown INT pixel depth in TIF.");	  
	}

	// handle floating point case
	else if(sampleFormat == ::io::tif::FLOAT)
	{
	  if(bitsPerSample == 32) ::io::util::convertPixelToFlat<S, float*, Swrapper>(src_begin, src_end, (float*)(dst_begin), channels, pad);
	  else if(bitsPerSample == 64) ::io::util::convertPixelToFlat<S, double*, Swrapper>(src_begin, src_end, (double*)(dst_begin), channels, pad);
	  else throw customException("Unknown FLOAT pixel depth in TIF.");	  
	}

	// unknown case
	else throw customException("Unknown pixel format in TIF.");

	// Done.
      }



      template<typename D, typename Dwrapper>
	void convertFlatToPixel(const void* src_begin, unsigned int channels, 
				const D& dst_begin, const D& dst_end,
				const ::io::tif::sample_format& sampleFormat, unsigned int bitsPerSample,
				const typename Dwrapper::value_type& pad=0)
      {
	// handle unsigned int case
	if(sampleFormat == ::io::tif::UINT)
	{
	  if(bitsPerSample == 8) ::io::util::convertFlatToPixel<const uint8_t*, D, Dwrapper>((const uint8_t*)(src_begin), channels, dst_begin, dst_end, pad);
	  else if(bitsPerSample == 16) ::io::util::convertFlatToPixel<const uint16_t*, D, Dwrapper>((const uint16_t*)(src_begin), channels, dst_begin, dst_end, pad);
	  else if(bitsPerSample == 32) ::io::util::convertFlatToPixel<const uint32_t*, D, Dwrapper>((const uint32_t*)(src_begin), channels, dst_begin, dst_end, pad);
	  else throw customException("Unknown UINT pixel depth in TIF.");
	}

	// handle signed int case
	else if(sampleFormat == ::io::tif::INT)
	{
	  if(bitsPerSample == 8) ::io::util::convertFlatToPixel<const int8_t*, D, Dwrapper>((const int8_t*)(src_begin), channels, dst_begin, dst_end, pad);
	  else if(bitsPerSample == 16) ::io::util::convertFlatToPixel<const int16_t*, D, Dwrapper>((const int16_t*)(src_begin), channels, dst_begin, dst_end, pad);
	  else if(bitsPerSample == 32) ::io::util::convertFlatToPixel<const int32_t*, D, Dwrapper>((const int32_t*)(src_begin), channels, dst_begin, dst_end, pad);
	  else throw customException("Unknown INT pixel depth in TIF.");	  
	}

	// handle floating point case
	else if(sampleFormat == ::io::tif::FLOAT)
	{
	  if(bitsPerSample == 32) ::io::util::convertFlatToPixel<const float*, D, Dwrapper>((const float*)(src_begin), channels, dst_begin, dst_end, pad);
	  else if(bitsPerSample == 64) ::io::util::convertFlatToPixel<const double*, D, Dwrapper>((const double*)(src_begin), channels, dst_begin, dst_end, pad);
	  else throw customException("Unknown FLOAT pixel depth in TIF.");	  
	}

	// unknown case
	else throw customException("Unknown pixel format in TIF.");

	// Done.
      }



} // end ::io::tif::detail namespace


#endif /* _BUFFER2D_TIF_DETAIL_H_ */
