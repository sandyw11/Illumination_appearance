#ifndef _UNSUPPORTEDFORMAT_H_
#define _UNSUPPORTEDFORMAT_H_

#include "customException.h"

using namespace std;

class unsupportedFormat : public customException {
 public:
  unsupportedFormat(void) : customException("unsupported file format") {}
  unsupportedFormat(const unsupportedFormat& error) : customException(error) {}
  unsupportedFormat& operator=(const unsupportedFormat& error) { _assign(error); return *this; }
  virtual ~unsupportedFormat(void) throw() {}
};

#endif /* _UNSUPPORTEDFORMAT_H_ */
