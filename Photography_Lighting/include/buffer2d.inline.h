///////////////////////////////////
// Inline Methods for buffer2d.h //
///////////////////////////////////

#include <algorithm>
#include "copyFrom.h"

using namespace std;


//////////////////////////////
// operation                //
//////////////////////////////
template<typename T>
template<typename Operation>
inline buffer2d<T> buffer2d<T>::operation(Operation op) const
{
  buffer2d<T> result(width(), height());
  transform(begin(), end(), result.begin(), op);

  // done.
  return result;
}


//////////////////////////////
// operation                //
//////////////////////////////
template<typename T>
template<typename Operation>
inline buffer2d<T>& buffer2d<T>::operation(Operation op)
{
  transform(begin(), end(), begin(), op);

  // done.
  return *this;
}


//////////////////////////////
// operation                //
//////////////////////////////
template<typename T>
template<typename Operation>
inline buffer2d<T> buffer2d<T>::operation(const buffer2d<T>& b, Operation op) const
{
  // sanity check
  _checkSize(b);

  // create result
  buffer2d<T> result(width(), height());
  transform(begin(), end(), b.begin(), result.begin(), op);

  // done.
  return result;
}


//////////////////////////////
// operation                //
//////////////////////////////
template<typename T>
template<typename Operation>
inline buffer2d<T>& buffer2d<T>::operation(const buffer2d<T>& b, Operation op)
{
  // sanity check
  _checkSize(b);

  // create result
  transform(begin(), end(), b.begin(), begin(), op);

  // done.
  return *this;
}


//////////////////////////////
// resize                   //
//                          //
// Resizes the buffer. The  //
// content of the current   //
// buffer is lost!          //
//////////////////////////////
template<typename T>
inline void buffer2d<T>::resize(buffer2d<T>::size_type newWidth, buffer2d<T>::size_type newHeight)
{
  buffer2d<T> temp(newWidth, newHeight);
  swap(temp, *this);
}


//////////////////////////////
// remap                    //
//                          //
// Remap the content of the //
// buffer to a new base type//
// The content of this is   //
// destroyed.  The width    //
// of the result is adjusted//
// by sizeof(T) / sizeof(S) //
//////////////////////////////
template<typename T>
  template<typename S>
inline void buffer2d<T>::remap(buffer2d<S>& result)
{
  result._deallocate();
  result._data = static_cast<S*>(_data);            
  result._width = _width * sizeof(T) / sizeof(S);   
  result._height = _height;                         
  result._size = result._width * result._height;

  _data = NULL;
  _width = 0;
  _height = 0;
  _size = 0;

  // Done.
}


//////////////////////////////
// allocate                 //
//////////////////////////////
template<typename T>
inline void buffer2d<T>::_allocate(buffer2d<T>::size_type width, buffer2d<T>::size_type height, buffer2d<T>::const_iterator init)
{
  _size = width * height;

  // sanity check: special care if height or width == 0
  if(_size == 0)
  {
    _width = _height = 0;
    _data = NULL;
  }
  else
  {
    // allocate
    _width = width;
    _height = height;
    _data = new T[_size];

    // copy data if requested
    if(init) copyFrom(init, begin(), end());
  }

  // done.
}


//////////////////////////////
// deallocate               //
//////////////////////////////
template<typename T>
inline void buffer2d<T>::_deallocate(void)
{
  // deallocate if allocated
  if(_data) delete[] _data;
  
  // reset
  _data = NULL;
  _width = 0;
  _height = 0;
  _size = 0;

  // done.
}


//////////////////////////////
// _swap                    //
//////////////////////////////
template<typename T>
inline void buffer2d<T>::_swap(buffer2d<T>& b)
{
  swap(_width, b._width);
  swap(_height, b._height);
  swap(_size, b._size);
  swap(_data, b._data);
}


//////////////////////////////
// _assign                  //
//////////////////////////////
template<typename T>
inline void buffer2d<T>::_assign(const buffer2d<T>& src)
{
  // sanity check
  if(this == & src) return;

  // copy
  buffer2d<T> temp(src);
  _swap(temp);

  // done.
}


