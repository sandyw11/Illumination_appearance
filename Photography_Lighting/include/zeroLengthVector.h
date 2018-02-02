#ifndef _ZEROLENGTHVECTOR_H_
#define _ZEROLENGTHVECTOR_H_

#include "customException.h"

using namespace std;

class zeroLengthVector : public customException {
 public:
  zeroLengthVector(void) : customException("vector has zero length") {}
  zeroLengthVector(const zeroLengthVector& error) : customException(error) {}
  zeroLengthVector& operator=(const zeroLengthVector& error) { _assign(error); return *this; }
  virtual ~zeroLengthVector(void) throw() {}
};

#endif /* _ZEROLENGTHVECTOR_H_ */
