#ifndef _BUFFER2DILLEGALSIZE_H_
#define _BUFFER2DILLEGALSIZE_H_

#include "customException.h"

using namespace std;

class buffer2dIllegalSize : public customException {
 public:
  buffer2dIllegalSize(void) : customException("2D buffer has an illigal size") {}
  buffer2dIllegalSize(const buffer2dIllegalSize& error) : customException(error) {}
  buffer2dIllegalSize& operator=(const buffer2dIllegalSize& error) { _assign(error); return *this; }
  virtual ~buffer2dIllegalSize(void) throw() {}
};

#endif /* _BUFFER2DILLEGALSIZE_H_ */
