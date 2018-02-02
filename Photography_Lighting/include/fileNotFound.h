#ifndef _FILENOTFOUND_H_
#define _FILENOTFOUND_H_

#include "customException.h"

using namespace std;

class fileNotFound : public customException {
 public:
  fileNotFound(const string& filename, const string& type="reading/writing") : customException("unable to open " + filename + " for " + type) {}
  fileNotFound(const fileNotFound& error) : customException(error) {}
  fileNotFound& operator=(const fileNotFound& error) { _assign(error); return *this; }
  virtual ~fileNotFound(void) throw() {}
};


#endif /* _FILENOTFOUND_H_ */
