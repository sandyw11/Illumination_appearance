#if defined(_BUFFER2DIO_EXR_H_) and !defined(_BUFFER2DIO_EXR_OPTIONS_H_)
#define _BUFFER2DIO_EXR_OPTIONS_H_

#include "exceptions.h"

namespace io {
  namespace exr {

    enum compression {
      NONE=0,
      RLE=1,
      ZIPS=2,
      ZIP=3,
      PIZ=4,
      XR24=5,
      B44=6,
      B44A=7
    };

    enum pixel {
      UINT = 0,
      HALF = 1,
      FLOAT = 2
    };

    struct options {
    public:
      /////////////////////////
      // Default Constructor //
      /////////////////////////
      options(const compression& compressionType=ZIP, const pixel& pixelType=FLOAT) : _compressionType(compressionType), _pixelType(pixelType)
      {
	_channelNames.push_back("R");
	_channelNames.push_back("G");
	_channelNames.push_back("B");
      }

      ///////////////////////
      // Named Constructor //
      ///////////////////////
      options(const std::vector<std::string>& channelNames, const compression& compressionType=ZIP, const pixel& pixelType=FLOAT) : _compressionType(compressionType), _pixelType(pixelType)
      {
	for(std::vector<std::string>::const_iterator itr=channelNames.begin(); itr != channelNames.end(); ++itr)
	  _channelNames.push_back(*itr);
      }

      //////////////////////
      // Copy Constructor //
      //////////////////////
      options(const options& src) : _compressionType(src._compressionType),
                                    _pixelType(src._pixelType),
	                            _channelNames(src._channelNames)
      {
	// Do nothing
      }

      ////////////////
      // Inspectors //
      ////////////////
      bool hasNamedChannels(void) const         { return !_channelNames.empty(); }
      unsigned int numberOfChannels(void) const { return _channelNames.size(); }
      compression compressionType(void) const   { return _compressionType; }
      pixel pixelType(void) const               { return _pixelType; }

      std::string channelName(unsigned int index) const
      {
	if(index < _channelNames.size()) return _channelNames[index];
	else {
	  // return standard unnamed channel name
	  char str[1+(sizeof(index)/3)];
	  sprintf(str, "C%d", index);
	  return std::string(str);
	}
      }

    private:
      /////////////////////
      // Private Members //
      /////////////////////
      compression _compressionType;
      pixel _pixelType;
      std::vector<std::string> _channelNames;
    };


    options unnamedOptions(void) { return options(std::vector<std::string>()); }

  } // end exr namespace
}   // end io namespace

#endif /* _BUFFER2DIO_EXR_OPTIONS_H_ */
