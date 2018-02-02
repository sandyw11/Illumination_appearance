#ifndef _BUFFER2D_H_
#define _BUFFER2D_H_

#include <ostream>
#include <algorithm>
#include <functional>

#include "raises.h"
#include "absoluteValue.h"
#include "exceptions.h"

template<typename T>
class buffer2d {
 public:
  ///////////////
  // type defs //
  ///////////////
  typedef T             value_type;
  typedef T*            iterator;
  typedef const T*      const_iterator;
  typedef T&            reference;
  typedef const T&      const_reference;
  typedef size_t        size_type;

  //////////////////
  // Constructors //
  //////////////////
  explicit buffer2d(size_type width=0, size_type height=0, const_iterator init=NULL) { _allocate(width, height, init); }
  buffer2d(const buffer2d<T>& b)                                                     { _allocate(b.width(), b.height(), b.begin()); }

  ////////////////
  // Destructor //
  ////////////////
  virtual ~buffer2d(void) { _deallocate(); }

  ///////////////
  // Iterators //
  ///////////////
  iterator          begin(void)         { return _data; }
  const_iterator    begin(void) const   { return _data; }
  iterator          end(void)           { return &(_data[size()]); }
  const_iterator    end(void) const     { return &(_data[size()]); }

  ////////////////
  // Inspectors //
  ////////////////
  size_type width(void) const  { return _width; }
  size_type height(void) const { return _height; }
  size_type size(void) const   { return _size; }

  bool empty(void) const { return (_width == 0) || (_height == 0); }

  const_reference operator()(size_type x, size_type y) const { return _at(x,y); }
  reference       operator()(size_type x, size_type y)       { return _at(x,y); }

  ///////////////
  // Operators //
  ///////////////
  buffer2d<T> operator=(const buffer2d<T>& b)        { _assign(b); return *this; }

  buffer2d<T> operator+(const T& s) const            { return operation(std::bind2nd(std::plus<T>(), s)); }
  buffer2d<T> operator+(const buffer2d<T>& b) const  { return operation(b, std::plus<T>()); }
  buffer2d<T> operator-(void) const                  { return operation(std::negate<T>()); }
  buffer2d<T> operator-(const T& s) const            { return operation(std::bind2nd(std::minus<T>(), s)); }
  buffer2d<T> operator-(const buffer2d<T>& b) const  { return operation(b, std::minus<T>()); }
  buffer2d<T> operator*(const T& s) const            { return operation(std::bind2nd(std::multiplies<T>(), s)); }
  buffer2d<T> operator*(const buffer2d<T>& b) const  { return operation(b, std::multiplies<T>()); }
  buffer2d<T> operator/(const T& s) const            { return operation(std::bind2nd(std::divides<T>(), s)); }
  buffer2d<T> operator/(const buffer2d<T>& b) const  { return operation(b, std::divides<T>()); }

  template<typename S>
    buffer2d<T> operator^(const S& s) const            { return operation(std::bind2nd(raises<T,S>(), s)); }

  buffer2d<T>& operator+=(const T& s)                { return operation(std::bind2nd(std::plus<T>(), s)); }
  buffer2d<T>& operator+=(const buffer2d<T>& b)      { return operation(b, std::plus<T>()); }
  buffer2d<T>& operator-=(const T& s)                { return operation(std::bind2nd(std::minus<T>(), s)); }
  buffer2d<T>& operator-=(const buffer2d<T>& b)      { return operation(b, std::minus<T>()); }
  buffer2d<T>& operator*=(const T& s)                { return operation(std::bind2nd(std::multiplies<T>(), s)); }
  buffer2d<T>& operator*=(const buffer2d<T>& b)      { return operation(b, std::multiplies<T>()); }
  buffer2d<T>& operator/=(const T& s)                { return operation(std::bind2nd(std::divides<T>(), s)); }
  buffer2d<T>& operator/=(const buffer2d<T>& b)      { return operation(b, std::divides<T>()); }

  template<typename S>
    buffer2d<T>& operator^=(const S& s)                { return operation(std::bind2nd(raises<T,S>(), s)); }

  /////////////
  // Methods //
  /////////////
  void resize(size_type newWidth, size_type newHeight);

  template<typename S>
    void remap(buffer2d<S>& result);

  void clear(const T& s=(T)(0)) { fill(begin(), end(), s); }
  void Abs(void)                { return operation(absoluteValue<T>()); }
  T max(void) const             { return *std::max_element(begin(), end()); }
  T min(void) const             { return *std::min_element(begin(), end()); }

  /////////////
  // Friends //
  /////////////
  friend void swap(buffer2d<T>& a, buffer2d<T>& b)               { a._swap(b); }
  friend buffer2d<T> pow(const buffer2d<T>& b, const T& s)       { return (b ^ s); }
  friend buffer2d<T> Abs(const buffer2d<T>& b)                   { return buffer2d<T>(b).Abs(); }

  friend buffer2d<T> operator*(const T& s, const buffer2d<T>& b) { return (b*s); }

  friend std::ostream& operator<<(std::ostream& s, const buffer2d<T>& b)
  {
    s << "(" << b.width() << ", " << b.height() << ")@" << (void *)(b.begin());
    return s;
  }

 protected:
  ///////////////////////
  // Protected Methods //
  ///////////////////////
  void _allocate(size_type width, size_type height, const_iterator init);
  void _deallocate(void);

  void _checkSize(const buffer2d<T>& b) const { if(width() != b.width() || height() != b.height()) throw buffer2dIllegalSize(); }

  template<typename Operation>
    buffer2d<T> operation(const buffer2d<T>& b, Operation op) const;

  template<typename Operation>
    buffer2d<T>& operation(const buffer2d<T>& b, Operation op);

  template<typename Operation>
    buffer2d<T> operation(Operation op) const;

  template<typename Operation>
    buffer2d<T>& operation(Operation op);

  void _swap(buffer2d<T>& b);
  void _assign(const buffer2d<T>& src);

  const_reference _at(size_type x, size_type y) const    { return _data[y*width()+x]; }
  reference       _at(size_type x, size_type y)          { return _data[y*width()+x]; }

  ////////////////////////////
  // Protected Data Members //
  ////////////////////////////
  T* _data;
  size_type _width, _height, _size;
};

////////////////////
// Inline Methods //
////////////////////
#include "buffer2d.inline.h"

#endif /* _BUFFER2D_H_ */
