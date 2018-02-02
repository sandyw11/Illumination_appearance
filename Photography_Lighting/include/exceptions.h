#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_

#include <ostream>
#include <exception>

/////////////////////
// Output Operator //
/////////////////////
inline std::ostream& operator<<(std::ostream& s, const std::exception& 
e)
{
  s << e.what() << std::endl;
  return s;
}

////////////////////
// Exception List //
////////////////////
#include "buffer2dIllegalSize.h"
#include "unrecognizedFormat.h"
#include "unsupportedFormat.h"
#include "zeroLengthVector.h"
#include "fileNotFound.h"
#include "customException.h"

#endif /* _EXCEPTIONS_H_ */
