#ifndef _eNDIAN_DETAIL_H_
#define _eNDIAN_DETAIL_H_

namespace endian {
  namespace detail {

    ///////////////////
    // Magic Numbers //
    ///////////////////
    const unsigned char _magicNumber[2] = {1, 0};
    const bool _isLittleEndian = (*(short *)(_magicNumber) == 1);

    ///////////////
    // swapOrder //
    ///////////////
    template<typename T>
      T swapOrder(const T& value)
      {
	// init
	T result;
	unsigned char* valueItr = &((unsigned char *)(&value))[sizeof(T) - 1];
	unsigned char* resultItr = (unsigned char *)(&result);
	unsigned char* resultEnd = &resultItr[sizeof(T)];
	
	// swap
	for(; resultItr != resultEnd; valueItr--, resultItr++)
	  *resultItr = *valueItr;

	// done.
	return result;
      } 
    

  } // end detail namespace
 
}   // end endian namespacee


#endif /* _eNDIAN_DETAIL_H_ */
