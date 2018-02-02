///////////////////////////////
// Inline Methods of vec3d.h //
///////////////////////////////

#include "zeroLengthVector.h"

using namespace std;

//////////////////////////////
// Constructors             //
//////////////////////////////
template<typename T>
inline vec3d<T>::vec3d(const T& value)
{
  this->x = value;
  this->y = value;
  this->z = value;
}


//////////////////////////////
// Constructors             //
//////////////////////////////
template<typename T>
inline vec3d<T>::vec3d(const T& x, const T& y, const T& z)
{
  this->x = x;
  this->y = y;
  this->z = z;
}


//////////////////////////////
// Constructors             //
//////////////////////////////
template<typename T>
 template<typename Itr>
  inline vec3d<T>::vec3d(const Itr& begin, const Itr& end, const T& pad, typename boost::disable_if<boost::is_fundamental<Itr> >::type* dummy)
{
  Itr itr = begin;
  this->x = (itr != end) ? *itr++ : pad;
  this->y = (itr != end) ? *itr++ : pad;
  this->z = (itr != end) ? *itr : pad;
}

//////////////////////////////
// Copy Constructors        //
//////////////////////////////
template<typename T>  
inline vec3d<T>::vec3d(const vec3d<T>& v)
{
  x = v.x;
  y = v.y;
  z = v.z;
}


//////////////////////////////
// Operator+=               //
//////////////////////////////
template<typename T>
inline vec3d<T>& vec3d<T>::operator+=(const T& s)
{
  x += s;
  y += s;
  z += s;
  return *this;
}


//////////////////////////////
// Operator+=               //
//////////////////////////////
template<typename T>
inline vec3d<T>& vec3d<T>::operator+=(const vec3d<T>& v)
{
  x += v.x;
  y += v.y;
  z += v.z;
  return *this;
}


//////////////////////////////
// Operator-=               //
//////////////////////////////
template<typename T>
inline vec3d<T>& vec3d<T>::operator-=(const T& s)
{
  x -= s;
  y -= s;
  z -= s;
  return *this;
}


//////////////////////////////
// Operator-=               //
//////////////////////////////
template<typename T>
inline vec3d<T>& vec3d<T>::operator-=(const vec3d<T>& v)
{
  x -= v.x;
  y -= v.y;
  z -= v.z;
  return *this;
}


//////////////////////////////
// Operator*=               //
//////////////////////////////
template<typename T>
inline vec3d<T>& vec3d<T>::operator*=(const T& s)
{
  x *= s;
  y *= s;
  z *= s;
  return *this;
}


//////////////////////////////
// Operator*=               //
//////////////////////////////
template<typename T>
inline vec3d<T>& vec3d<T>::operator*=(const vec3d<T>& v)
{
  x *= v.x;
  y *= v.y;
  z *= v.z;
  return *this;
}


//////////////////////////////
// Operator/=               //
//////////////////////////////
template<typename T>
inline vec3d<T>& vec3d<T>::operator/=(const T& s)
{
  x /= s;
  y /= s;
  z /= s;
  return *this;
}


//////////////////////////////
// Operator/=               //
//////////////////////////////
template<typename T>
inline vec3d<T>& vec3d<T>::operator/=(const vec3d<T>& v)
{
  x /= v.x;
  y /= v.y;
  z /= v.z;
  return *this;
}


//////////////////////////////
// Operator^=               //
//////////////////////////////
template<typename T>
inline vec3d<T>& vec3d<T>::operator^=(const T& s)
{
  x = pow(x, s);
  y = pow(y, s);
  z = pow(z, s);
  return *this;
}


//////////////////////////////
// cross                    //
//////////////////////////////
template<typename T>  
inline vec3d<T> vec3d<T>::cross(const vec3d<T>& v) const
{
  return vec3d<T>(v.z*y - v.y*z,
		  v.x*z - v.z*x,
		  v.y*x - v.x*y);
}


//////////////////////////////
// cross                    //
//////////////////////////////
template<typename T>  
inline vec3d<T> vec3d<T>::normalize(void) const 
{
  T l = length();
  if(l == (T)(0)) throw zeroLengthVector();
  return *this / l;
}


//////////////////////////////
// _swap                    //
//////////////////////////////
template<typename T>  
inline void vec3d<T>::_swap(vec3d<T>& v)
{
  swap(v.x, x);
  swap(v.y, y);
  swap(v.z, z);
}


//////////////////////////////
// _assign                  //
//////////////////////////////
template<typename T>
inline void vec3d<T>::_assign(const vec3d<T>& v)
{
  x = v.x;
  y = v.y;
  z = v.z;
}
