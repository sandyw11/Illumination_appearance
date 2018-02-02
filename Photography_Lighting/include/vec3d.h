#ifndef _VEC3D_H_
#define _VEC3D_H_

#include <cmath>
#include <ostream>
#include <boost/type_traits.hpp>
#include <boost/utility/enable_if.hpp>

#undef abs

using namespace std;

template<typename T>
class vec3d {
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

  /////////////////////////
  // Public Data Members //
  /////////////////////////
  T x, y, z;


  //////////////////
  // Constructors //
  //////////////////
  vec3d(const T& value=(T)(0));
  vec3d(const T& x, const T& y, const T& z);
  vec3d(const vec3d<T>& v);

  template<typename Itr>
    vec3d(const Itr& begin, const Itr& end, const T& pad=(T)(0), typename boost::disable_if<boost::is_fundamental<Itr> >::type* dummy=0);

  ////////////////
  // Destructor //
  ////////////////
  ~vec3d(void) {}

  ///////////////////
  // Cast Operator //
  ///////////////////
  template<typename S>
    operator vec3d<S>() const { return vec3d<S>( (S)(x), (S)(y), (S)(z) ); }

  /////////////
  // Methods //
  /////////////
  const_iterator    begin(void) const     { return (iterator)(this); }
  iterator          begin(void)           { return (iterator)(this); }
  const_iterator    end(void) const       { return begin() + size(); }
  iterator          end(void)             { return begin() + size(); }

  size_type         size(void) const      { return 3; }

  const T& operator[](int index) const    { return begin()[index]; }
  T&       operator[](int index)          { return begin()[index]; }

  vec3d<T>& operator=(const vec3d<T>& v)  { _assign(v); return *this; }

  bool     operator==(const vec3d<T>& v) const { return (v.x == x) && (v.y == y) && (v.z == z); }
  bool     operator!=(const vec3d<T>& v) const { return (v.x != x) || (v.y != y) || (v.z != z); }

  bool     operator<(const T& val) const       { return squared_length() <  val*val; }
  bool     operator<=(const T& val) const      { return squared_length() <= val*val; }
  bool     operator>(const T& val) const       { return squared_length() >  val*val; }
  bool     operator>=(const T& val) const      { return squared_length() >= val*val; }

  vec3d<T> operator+(const T& s) const         { return vec3d<T>(x + s, y + s, z + s); }
  vec3d<T> operator+(const vec3d<T>& v) const  { return vec3d<T>(x + v.x, y + v.y, z + v.z); }
  vec3d<T> operator-(void) const               { return vec3d<T>(-x, -y, -z); }
  vec3d<T> operator-(const T& s) const         { return vec3d<T>(x - s, y - s, z - s); }
  vec3d<T> operator-(const vec3d<T>& v) const  { return vec3d<T>(x - v.x, y - v.y, z - v.z); }
  vec3d<T> operator*(const T& s) const         { return vec3d<T>(x*s, y*s, z*s); }
  vec3d<T> operator*(const vec3d<T>& v) const  { return vec3d<T>(x*v.x, y*v.y, z*v.z); }
  vec3d<T> operator/(const T& s) const         { return vec3d<T>(x/s, y/s, z/s); }
  vec3d<T> operator/(const vec3d<T>& v) const  { return vec3d<T>(x/v.x, y/v.y, z/v.z); }
  vec3d<T> operator^(const T& s) const         { return vec3d<T>(pow(x,s), pow(y,s), pow(z,s)); }

  vec3d<T>& operator+=(const T& s);
  vec3d<T>& operator+=(const vec3d<T>& v);
  vec3d<T>& operator-=(const T& s);
  vec3d<T>& operator-=(const vec3d<T>& v);
  vec3d<T>& operator*=(const T& s);
  vec3d<T>& operator*=(const vec3d<T>& v);
  vec3d<T>& operator/=(const T& s);
  vec3d<T>& operator/=(const vec3d<T>& v);
  vec3d<T>& operator^=(const T& s);

  T dot(const vec3d<T>& v) const              { return x*v.x + y*v.y + z*v.z; }
  T squared_length(void) const                { return dot(*this); }
  T length(void) const                        { return sqrt(squared_length()); }
  T squared_distance(const vec3d<T>& v) const { return (*this - v).squared_length(); }
  T distance(const vec3d<T>& v) const         { return sqrt(squared_distance(v)); }

  vec3d<T> cross(const vec3d<T>& v) const;
  vec3d<T> normalize(void) const;

  vec3d<T>  min(const vec3d<T> &v) { return vec3d<T>(std::min(x, v.x), std::min(y, v.y), std::min(z, v.z)); }
  vec3d<T>  max(const vec3d<T> &v) { return vec3d<T>(std::max(x, v.x), std::max(y, v.y), std::max(z, v.z)); }

  /////////////
  // Friends //
  /////////////
  friend void swap(vec3d<T>& a, vec3d<T>& b)                { return a._swap(b) ; }
  friend vec3d<T> pow(const vec3d<T>& v, const T& s)        { return (v ^ s); }
  friend vec3d<T> abs(const vec3d<T>& v)                    { return vec3d<T>( abs(v.x), abs(v.y), abs(v.z) ); }
  friend vec3d<T> max(const vec3d<T>& a, const vec3d<T>& b) { return vec3d<T>(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z)); }
  friend vec3d<T> min(const vec3d<T>& a, const vec3d<T>& b) { return vec3d<T>(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z)); }
  friend void normalize(vec3d<T>& v)                        { v = v.normalize(); }

  friend T squared_distance(const vec3d<T>& a, const vec3d<T>& b) { return a.squared_distance(b); }
  friend T Distance(const vec3d<T>& a, const vec3d<T>& b)         { return a.distance(b); }

  friend vec3d<T> operator*(const T& s, const vec3d<T>& v)        { return (v*s); }

  friend ostream& operator<<(ostream& s, const vec3d<T>& v)
  {
    s << "[" << v.x << ", " << v.y << ", " << v.z << "]";
    return s;
  }


 protected:
  ///////////////////////
  // Protected Methods //
  ///////////////////////
  void _swap(vec3d<T>& v);
  void _assign(const vec3d<T>& v);
};


////////////////////
// Inline Methods //
////////////////////
#include "vec3d.inline.h"

#endif /* _VEC3D_H_ */
