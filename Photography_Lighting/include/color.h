#ifndef _COLOR_H_
#define _COLOR_H_

#include <ostream>
#include <cmath>

////////////////////////
// Simple color Class //
////////////////////////

template<typename T>
class color {
public:
  typedef T                    value_type;
  typedef T*                   iterator;
  typedef const T*             const_iterator;
  typedef T&                   reference;
  typedef const T&             const_reference;

  //////////////////
  // Constructors //
  //////////////////
  color(const_reference value=(T)(0)) { r = g = b = value; }
  color(const_reference red, const_reference green, const_reference blue) { r = red; g = green;  b = blue; }
  color(const color<T>& c) { r = c.r; g = c.g; b = c.b; }

  ///////////////
  // Operators //
  ///////////////
  void swap(color<T>& swp) { std::swap(r, swp.r); std::swap(g, swp.g); std::swap(b, swp.b); }
  color<T>& operator=(const color<T>& src) { r = src.r; g = src.g; b = src.b; return *this; }

  iterator begin(void) { return &r; }
  iterator end(void) { return begin() + size(); }

  const_iterator begin(void) const { return &r; }
  const_iterator end(void) const { return begin() + size(); }

  unsigned int size(void) const { return 3; }
  T length(void) const { return (r*r + g*g + b*b); }
  T absSum(void) const { return std::abs(r) + std::abs(g) + std::abs(b); }
  T average(void) const { return (r+g+b) / 3.0f; }

  reference operator[](unsigned int idx) { return *(begin() + idx); }
  const_reference operator[](unsigned int idx) const { return *(begin() + idx); }

  color<T> operator+(const color<T>& c) const { return color<T>(r+c.r, g+c.g, b+c.b); }
  color<T> operator-(const color<T>& c) const { return color<T>(r-c.r, g-c.g, b-c.b); }
  color<T> operator*(const color<T>& c) const { return color<T>(r*c.r, g*c.g, b*c.b); }
  color<T> operator/(const color<T>& c) const { return color<T>(r/c.r, g/c.g, b/c.b); }
  color<T> operator*(const T& s) const { return color<T>(r*s, g*s, b*s); }
  color<T> operator/(const T& s) const { return color<T>(r/s, g/s, b/s); }
  color<T> operator^(const color<T>& c) const { return color<T>(pow(r, c.r), pow(g, c.g), pow(b, c.b)); }

  template<typename S>
    color<T> operator^(const S& s) const { return color<T>( pow(r, s), pow(g, s), pow(b, s) ); }

  color<T>& operator+=(const color<T>& c) { r+=c.r; g+=c.g; b+=c.b; return *this; }
  color<T>& operator-=(const color<T>& c) { r-=c.r; g-=c.g; b-=c.b; return *this; }
  color<T>& operator*=(const color<T>& c) { r*=c.r; g*=c.g; b*=c.b; return *this; }
  color<T>& operator/=(const color<T>& c) { r/=c.r; g/=c.g; b/=c.b; return *this; }
  color<T>& operator*=(const T& s) { r*=s; g*=s; b*=s; return *this; }
  color<T>& operator/=(const T& s) { r/=s; g/=s; b/=s; return *this; }

  template<typename S>
    color<T>& operator^=(const S& s) { *this = *this ^ s; return *this; }

  bool operator==(const color<T>& c) const { return (r == c.r && g == c.g && b == c.b); }
  bool operator!=(const color<T>& c) const { return !(*this == c); }
  bool operator<(const color<T>& c) const { return length() < c.length(); }
  bool operator<=(const color<T>& c) const { return length() <= c.length(); }
  bool operator>(const color<T>& c) const { return length() > c.length(); }
  bool operator>=(const color<T>& c) const { return length() >= c.length(); }
  
  /////////////
  // Friends //
  /////////////
  friend void swap(color<T>& a, color<T>& b) { a.swap(b); }
  friend T magnitude(const color<T>& c) { return c.absSum(); }

  template<typename S>
    friend color<T> pow(const color<T>& col, const S& s) { return col ^ s; }

  friend std::ostream& operator<<(std::ostream& s, const color<T>& col)
  {
    s << "(" << col.r << ", " << col.g << ", " << col.b << ")";
    return s;
  }

public:
  T r,g,b;
};

#endif /* _COLOR_H_ */
