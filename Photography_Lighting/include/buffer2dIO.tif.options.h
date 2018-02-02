#if defined(_BUFFER2DIO_TIF_H_) and !defined( _BUFFER2DIO_TIF_OPTIONS_H_)
#define _BUFFER2DIO_TIF_OPTIONS_H_

namespace io {
  namespace tif {

    enum sample_format {
      UINT = 1,
      INT = 2,
      FLOAT = 3
    };

    enum compression {
      NONE = 1,
      LZW = 5
    };

    struct options {
    public:
      /////////////////
      // Constructor //
      /////////////////
      options(const sample_format& format=FLOAT, unsigned int bits=32, const compression& compressionType=LZW) : _sampleFormat(format), 
                                              _bitsPerSample(bits),
                                              _compressionType(compressionType)
      {
	checkOptions();
      }

      //////////////////////
      // Copy Constructor //
      //////////////////////
      options(const options& src) : _sampleFormat(src._sampleFormat),
                                    _bitsPerSample(src._bitsPerSample),
                                    _compressionType(src._compressionType)
      {
	// Do nothing
      }


      ////////////////
      // Inspectors //
      ////////////////
      sample_format sampleFormat(void) const { return _sampleFormat; }
      unsigned int bitsPerSample(void) const { return _bitsPerSample; }
      compression compressionType(void) const { return _compressionType; }

    private:
      ////////////////////
      // Private Method //
      ////////////////////
      void checkOptions(void) const
      {
	if(_sampleFormat == UINT || _sampleFormat == INT)
	  if(_bitsPerSample != 8 && _bitsPerSample != 16 && _bitsPerSample != 32) 
	    throw unsupportedFormat();

	if(_sampleFormat == FLOAT)
	  if(_bitsPerSample != 32 && _bitsPerSample != 64)
	    throw unsupportedFormat();
      }
      
      //////////////////
      // Private Data //
      //////////////////
      sample_format _sampleFormat;
      unsigned int _bitsPerSample;
      compression _compressionType;
    };

  } // end tif namespace
} // end io namespace

#endif /* _BUFFER2DIO_TIF_OPTIONS_H_ */
