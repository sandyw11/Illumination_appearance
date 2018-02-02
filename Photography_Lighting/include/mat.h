#ifndef _MAT_H_
#define _MAT_H_

#include <ostream>
#include <cassert>
#include <cmath>
#include <numeric>
#include <algorithm>
#include "raises.h"

template<typename T>
struct mat {
public:
  /////////////
  // Typedef //
  /////////////
  typedef T            value_type;
  typedef T&           reference;
  typedef const T&     const_reference;
  typedef T*           iterator;
  typedef const T*     const_iterator;

  //////////////////
  // Constructors //
  //////////////////
  mat(unsigned int width=0, unsigned int height=0) { _init(width,height,(T*)(NULL)); }

  template<typename Iterator>
  mat(unsigned int width, unsigned int height, Iterator data=NULL) { _init(width,height,data); }

  mat(const mat<T>& src) { _init(src.width(), src.height(), src.begin()); }

  /////////////////
  // Destructors //
  /////////////////
  ~mat(void) { _dealloc(); }

  //////////////////////
  // Iterator Support //
  //////////////////////
  const_iterator begin(void) const { return _data; }
  const_iterator end(void) const   { return &(_data[_width*_height]); }

  iterator begin(void) { return _data; }
  iterator end(void)   { return &(_data[_width*_height]); }

  ////////////////
  // Inspectors //
  ////////////////
  reference operator()(int x, int y);
  const_reference operator()(int x, int y) const ;

  reference operator[](int i);
  const_reference operator[](int i) const;

  int width(void) const  { return _width; }
  int height(void) const { return _height; }
  int size(void) const   { return _width*_height; }

  ///////////////
  // Operator= //
  ///////////////
  mat<T>& operator=(const mat<T>& src);
  mat<T>& operator=(const_reference val) { std::fill(begin(), end(), val); return *this; }

  ///////////////
  //   Math    //
  // Operators //
  ///////////////
  mat<T> operator*(const mat<T>& other) const;
  mat<T> operator*(const_reference val) const;
  mat<T> operator/(const_reference val) const;
  mat<T> operator^(const_reference val) const;
  mat<T> operator+(const mat<T>& other) const;
  mat<T> operator-(const mat<T>& other) const;

  mat<T>& operator^=(const_reference val)  { std::transform(begin(), end(), begin(), std::bind2nd(raises<T,T>(), val)); return *this; }
  mat<T>& operator*=(const_reference val)  { std::transform(begin(), end(), begin(), std::bind2nd(std::multiplies<T>(), val)); return *this; }
  mat<T>& operator/=(const_reference val)  { std::transform(begin(), end(), begin(), std::bind2nd(std::divides<T>(), val)); return *this; }
  mat<T>& operator+=(const_reference val)  { std::transform(begin(), end(), begin(), std::bind2nd(std::plus<T>(), val)); return *this; }
  mat<T>& operator-=(const_reference val)  { std::transform(begin(), end(), begin(), std::bind2nd(std::minus<T>(), val)); return *this; }

  mat<T>& operator+=(const mat<T>& other);
  mat<T>& operator-=(const mat<T>& other);


  /////////////
  // Compare //
  /////////////
  bool operator==(const mat<T>& other) const { return (_compare(other) == 0); }
  bool operator!=(const mat<T>& other) const { return (_compare(other) != 0); }
  bool operator<(const mat<T>& other) const  { return (_compare(other) < 0); }
  bool operator<=(const mat<T>& other) const { return (_compare(other) <= 0); }
  bool operator>(const mat<T>& other) const  { return (_compare(other) > 0); }
  bool operator>=(const mat<T>& other) const { return (_compare(other) >= 0); }


  //////////////////////
  // Matrix Functions //
  //////////////////////
  T length(void) const  { return std::inner_product(begin(), end(), begin(), (T)(0)); }
  T sum(void) const     { return std::accumulate(begin(), end(), (T)(0)); }
  T average(void) const { return sum() / (_width * _height); }


  mat<T>& threshold(const_reference value);
  mat<T>& threshold(const_reference neg_value, const_reference pos_value);
  mat<T>& Abs(void); 
  mat<T> transpose(void) const;


  /////////////
  // Friends //
  /////////////
  friend void swap(mat<T>& a, mat<T>& b) { a._swap(b); }

  friend std::ostream& operator<<(std::ostream& s, const mat<T>& m)
  {
    for(int j=0; j < m.height(); j++)
    {
      for(int i=0; i < m.width(); i++)
	s << m(i,j) << ", ";
      s << "\r\n";
    }
    return s;
  }

protected:
  ///////////////////////
  // Protected Methods //
  ///////////////////////
  void _init(int width, int height, const T* data=NULL);
  void _swap(mat<T>& swp);
  void _alloc(int width, int height);
  void _dealloc(void);
  int _compare(const mat<T>& m) const;

  //////////////////
  // Data Members //
  //////////////////
  iterator _data;
  int _width, _height;
};

////////////////////
// Inline Methods //
////////////////////
#include "mat.inline.h"

#endif /* _MAT_H_ */
