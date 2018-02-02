//////////////////////////
// buffer2dIO utilities //
//////////////////////////

#ifndef _BUFFER2DIO_UTIL_H_
#define _BUFFER2DIO_UTIL_H_

#include <boost/limits.hpp>
#include <boost/mpl/logical.hpp>
#include <boost/type_traits.hpp>
#include <boost/iterator/iterator_traits.hpp>

namespace io {
  namespace util {

    /////////////////////////////////////////////////////
    // Utility to cast from one pixel type to another  //
    // This utility takes care of normalization too.   //
    // E.g., 8bit to float, includes a division by 255 //
    /////////////////////////////////////////////////////
    template<typename FROM_TYPE, typename TO_TYPE, class Enable=void>
      class pixelTypeConvertor {
    public:
      typedef TO_TYPE   to_type;
      typedef FROM_TYPE from_type;

      to_type operator()(const from_type& src) const { return (to_type)(src); }
      unsigned int sizeFrom(void) const { return sizeof(from_type); }
      unsigned int sizeTo(void) const { return sizeof(to_type); }
      bool sameType(void) const { return false; }
    };


    template<typename TYPE>
      class pixelTypeConvertor<TYPE, TYPE, TYPE> {
    public:
      typedef TYPE     to_type;
      typedef TYPE     from_type;

      to_type operator()(const from_type& src) const { return (to_type)(src); }
      unsigned int sizeFrom(void) const { return sizeof(from_type); }
      unsigned int sizeTo(void) const { return sizeof(to_type); }
      bool sameType(void) const { return false; }
    };


    template<typename FROM_TYPE, typename TO_TYPE>
      class pixelTypeConvertor<FROM_TYPE, TO_TYPE, typename boost::enable_if< boost::mpl::and_<typename boost::is_floating_point<FROM_TYPE>::type, typename boost::is_integral<TO_TYPE>::type > >::type >
    {
    public:
      typedef TO_TYPE     to_type;
      typedef FROM_TYPE   from_type;

      // cast to double to ensure sufficient bits for the normalization if TO_TYPE = 32 bit.
      to_type operator()(const from_type& src) const { return (to_type)((double)(src) * (double)(std::numeric_limits<to_type>::max())); }
      unsigned int sizeFrom(void) const { return sizeof(from_type); }
      unsigned int sizeTo(void) const { return sizeof(to_type); }
      bool sameType(void) const { return true; }
    };


    template<typename FROM_TYPE, typename TO_TYPE>
      class pixelTypeConvertor<FROM_TYPE, TO_TYPE, typename boost::enable_if< boost::mpl::and_<typename boost::is_integral<FROM_TYPE>::type, typename boost::is_floating_point<TO_TYPE>::type > >::type >
    {
    public:
      typedef TO_TYPE   to_type;
      typedef FROM_TYPE from_type;

      // cast to double to ensure sufficient bits for the normalization if FROM_TYPE = 32 bit.
      to_type operator()(const from_type& src) const { return (to_type)((double)(src) / (double)(std::numeric_limits<from_type>::max())); }
      unsigned int sizeFrom(void) const { return sizeof(from_type); }
      unsigned int sizeTo(void) const { return sizeof(to_type); }
      bool sameType(void) const { return false; }
    };


    template<typename FROM_TYPE, typename TO_TYPE>
      class pixelTypeConvertor<FROM_TYPE, TO_TYPE, typename boost::enable_if< boost::mpl::and_<typename boost::is_integral<FROM_TYPE>::type, typename boost::is_integral<TO_TYPE>::type > >::type >
    {
    public:
      typedef TO_TYPE   to_type;
      typedef FROM_TYPE from_type;

      to_type operator()(const from_type& src) const 
      { 
	if(sizeTo() > sizeFrom()) return (to_type)(src) << (sizeTo() - sizeFrom());
	else return (to_type)(src >> (sizeFrom() - sizeTo()));
      }
      unsigned int sizeFrom(void) const { return sizeof(from_type); }
      unsigned int sizeTo(void) const { return sizeof(to_type); }
      bool sameType(void) const { return false; }
    };


    ///////////////////////////////////////////////
    // Convert a pixel-component to a flat array //
    // with padding                              //
    ///////////////////////////////////////////////
    template<typename S, typename D, class src_wrapper>
      void convertPixelToFlat(const S& src_begin, const S& src_end,
			      const D& dst_begin, unsigned int dst_channels, 
			      const typename src_wrapper::value_type& pad=0)
      {
	// pixel convertor
	typedef pixelTypeConvertor<typename src_wrapper::value_type, typename std::iterator_traits<D>::value_type> convertor_type;
	convertor_type convertor;
	typename convertor_type::to_type cpad = convertor(pad);

	// iterate
	D dst_itr = dst_begin;
	for(S src_itr=src_begin; src_itr != src_end; ++src_itr)
	{
	  // create wrapper
	  src_wrapper wrapper(*src_itr);
	  unsigned int count=0;

	  // copy
	  for(typename src_wrapper::const_iterator type_itr=wrapper.begin(); type_itr != wrapper.end() && count < dst_channels; ++type_itr, ++dst_itr, ++count)
	    *dst_itr = convertor(*type_itr);

	  // pad
	  for(; count < dst_channels; ++count, ++dst_itr)
	    *dst_itr = cpad;
	}

	// Done.
      }


    ///////////////////////////////////////////////
    // Convert a flat to a pixel-component array //
    // with padding                              //
    ///////////////////////////////////////////////
    template<typename S, typename D, class dst_wrapper>
      void convertFlatToPixel(const S& src_begin, unsigned int src_channels, 
			      const D& dst_begin, const D& dst_end,
			      const typename dst_wrapper::value_type& pad=0)
      {
	// pixel convertor
	typedef pixelTypeConvertor<typename std::iterator_traits<S>::value_type, typename dst_wrapper::value_type> convertor_type;
	convertor_type convertor;

	// iterate
	S src_itr = src_begin;
	for(D dst_itr=dst_begin; dst_itr != dst_end; ++dst_itr)
	{
	  // create wrapper
	  dst_wrapper wrapper(*dst_itr);
	  unsigned int count=0;

	  // copy & pad
	  for(typename dst_wrapper::iterator type_itr=wrapper.begin(); type_itr != wrapper.end(); ++type_itr, ++src_itr, ++count)
	  {
	    if(count < src_channels)
	      *type_itr = convertor(*src_itr);

	    else 
	      *type_itr = pad;
	  }
	}

	// Done.
      }
      

  } // end util namespace
} // end io namespace

#endif /* _BUFFER2DIO_UTIL_H_ */
