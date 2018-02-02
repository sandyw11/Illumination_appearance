/////////////////////////////////////
// Generic buffer2d IO functions.  //
//                                 //
// Works for all fundamental types //
// and types with iterator support //
// Specialization required in other//
// cases.                          //
/////////////////////////////////////

#ifndef _BUFFER2DIO_H_
#define _BUFFER2DIO_H_

#include <string>

#include "buffer2d.h"
#include "iteratorWrapper.h"

//////////////////
// File formats //
//////////////////
#include "buffer2dIO.ppm.h"
#include "buffer2dIO.pfm.h"
#include "buffer2dIO.exr.h"
#include "buffer2dIO.png.h"
#include "buffer2dIO.jpg.h"
#include "buffer2dIO.tif.h"

using namespace std;

namespace io {

  ///////////////
  // Exporters //
  ///////////////
  template<typename T> void exportPFM(const string& filename, const buffer2d<T>& buf, const typename iteratorWrapper<T>::value_type& pad=0)                                               { pfm::_export<buffer2d<T>, iteratorWrapper<const T> >(filename, buf, pad); }
  template<typename T> void exportPPM(const string& filename, const buffer2d<T>& buf, const typename iteratorWrapper<T>::value_type& pad=0, ppm::bit_depth bitDepth=ppm::PPM8BIT)         { ppm::_export<buffer2d<T>, iteratorWrapper<const T> >(filename, buf, bitDepth, pad); }
  template<typename T> void exportEXR(const string& filename, const buffer2d<T>& buf, const typename iteratorWrapper<T>::value_type& pad=0, const exr::options& options=exr::options())   { exr::_export<buffer2d<T>, iteratorWrapper<const T> >(filename, buf, pad, options); }
  template<typename T> void exportPNG(const string& filename, const buffer2d<T>& buf, const typename iteratorWrapper<T>::value_type& pad=0)                                               { png::_export<buffer2d<T>, iteratorWrapper<const T> >(filename, buf, pad); }
  template<typename T> void exportJPG(const string& filename, const buffer2d<T>& buf, const typename iteratorWrapper<T>::value_type& pad=0, float compressionQuality=0.95f)               { jpg::_export<buffer2d<T>, iteratorWrapper<const T> >(filename, buf, pad, compressionQuality); }
  template<typename T> void exportTIF(const string& filename, const buffer2d<T>& buf, const typename iteratorWrapper<T>::value_type& pad=0, const tif::options& options=tif::options())   { tif::_export<buffer2d<T>, iteratorWrapper<const T> >(filename, buf, pad, options); }


  ///////////////
  // Importers //
  ///////////////
  template<typename T> void importPFM(const string& filename, buffer2d<T>& result, const typename iteratorWrapper<T>::value_type& pad=0)                                                   { pfm::_import<buffer2d<T>, iteratorWrapper<T> >(filename, result, pad); }
  template<typename T> void importPPM(const string& filename, buffer2d<T>& result, const typename iteratorWrapper<T>::value_type& pad=0)                                                   { ppm::_import<buffer2d<T>, iteratorWrapper<T> >(filename, result, pad); }
  template<typename T> void importEXR(const string& filename, buffer2d<T>& result, const typename iteratorWrapper<T>::value_type& pad=0, const exr::options& options=exr::options())       { exr::_import<buffer2d<T>, iteratorWrapper<T> >(filename, result, pad, options); }
  template<typename T> void importPNG(const string& filename, buffer2d<T>& result, const typename iteratorWrapper<T>::value_type& pad=0)                                                   { png::_import<buffer2d<T>, iteratorWrapper<T> >(filename, result, pad); }
  template<typename T> void importJPG(const string& filename, buffer2d<T>& result, const typename iteratorWrapper<T>::value_type& pad=0)                                                   { jpg::_import<buffer2d<T>, iteratorWrapper<T> >(filename, result, pad); }
  template<typename T> void importTIF(const string& filename, buffer2d<T>& result, const typename iteratorWrapper<T>::value_type& pad=0)                                                   { tif::_import<buffer2d<T>, iteratorWrapper<T> >(filename, result, pad); }

} // io namespace

#endif /* _BUFFER2DIO_H_ */
