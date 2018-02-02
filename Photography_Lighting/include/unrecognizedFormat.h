#ifndef _UNRECOGNIZEDFORMAT_H_
#define _UNRECOGNIZEDFORMAT_H_

#include "customException.h"

using namespace std;

class unrecognizedFormat : public customException {
 public:
  unrecognizedFormat(const string& filename) : customException("unrecognized file format in " + filename) {}
  unrecognizedFormat(const unrecognizedFormat& error) : customException(error) {}
  unrecognizedFormat& operator=(const unrecognizedFormat& error) { _assign(error); return *this; }
  virtual ~unrecognizedFormat(void) throw() {}
};


#endif /* _UNRECOGNIZEDFORMAT_H_ */
