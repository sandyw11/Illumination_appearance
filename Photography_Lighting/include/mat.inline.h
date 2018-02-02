#if defined(_MAT_H_) && !defined(_MAT_INLINE_H_)
#define _MAT_INLINE_H_

#include <cassert>

//////////////////////////
// Inspector operator() //
//////////////////////////
template<typename T>
typename mat<T>::reference mat<T>::operator()(int x, int y) 
{ 
  // sanity check
  assert(x < width() && y < height()); 
  assert(x >= 0 && y >= 0);

  // store data column major!
  return _data[x*height() + y]; 
}


////////////////////////////////
// Inspector const operator() //
////////////////////////////////
template<typename T>
typename mat<T>::const_reference mat<T>::operator()(int x, int y) const 
{ 
  // sanity check
  assert(x < width() && y < height());
  assert(x >= 0 && y >= 0);

  // store data column major!
  return _data[x*height() + y]; 
}


/////////////////////////////////
// Inspector linear operator[] //
/////////////////////////////////
template<typename T>
typename mat<T>::reference mat<T>::operator[](int i)
{
  assert(i < width()*height());
  return _data[i];
}


/////////////////////////////////
// Inspector linear operator[] //
/////////////////////////////////
template<typename T>
typename mat<T>::const_reference mat<T>::operator[](int i) const
{
  assert(i < width()*height());
  return _data[i];
}


///////////////
// Operator= //
///////////////
template<typename T>
mat<T>& mat<T>::operator=(const mat<T>& src)
{
  // sanity check
  if(&src == this) return *this;
	    
  // copy
  mat<T> temp(src);
  _swap(temp);
    
  return *this;
}

///////////////
// Operator* //
///////////////
template<typename T>
mat<T> mat<T>::operator*(const mat<T>& other) const
{
  // sanity check
  assert(other.height() == width());

  // multiply (storage order independent!)
  mat<T> result(other.width(), height());
  for(int i=0; i < result.width(); i++)
    for(int j=0; j < result.height(); j++)
    {
      T temp = 0;
      for(int k=0; k < width(); k++)
	temp += (*this)(k, j) * other(i, k);
      result(i,j) = temp;
    }
  return result;
}


///////////////
// Operator* //
///////////////
template<typename T>
mat<T> mat<T>::operator*(typename mat<T>::const_reference val) const
{
  mat<T> result(width(), height());
  std::transform(begin(), end(), result.begin(), std::bind2nd(std::multiplies<T>(), val));
  return result;
}


///////////////
// Operator/ //
///////////////
template<typename T>
mat<T> mat<T>::operator/(typename mat<T>::const_reference val) const
{
  mat<T> result(width(), height());
  std::transform(begin(), end(), result.begin(), std::bind2nd(std::divides<T>(), val));
  return result;
}


///////////////
// Operator^ //
///////////////
template<typename T>
mat<T> mat<T>::operator^(typename mat<T>::const_reference val) const
{
  mat<T> result(width(), height());
  std::transform(begin(), end(), result.begin(), std::bind2nd(raises<T,T>(), val));
  return result;
}


///////////////
// Operator+ //
///////////////
template<typename T>
mat<T> mat<T>::operator+(const mat<T>& other) const
{
  // sanity check
  assert(width() == other.width());
  assert(height() == other.height());

  // add
  mat<T> result(width(), height());
  std::transform(begin(), end(), other.begin(), result.begin(), std::plus<T>());

  return result;
}


///////////////
// Operator- //
///////////////
template<typename T>
mat<T> mat<T>::operator-(const mat<T>& other) const
{
  // sanity check
  assert(width() == other.width());
  assert(height() == other.height());

  // subtract
  mat<T> result(width(), height());
  std::transform(begin(), end(), other.begin(), result.begin(), std::minus<T>());
    
  return result;
}


////////////////
// Operator+= //
////////////////
template<typename T>
mat<T>& mat<T>::operator+=(const mat<T>& other)
{
  // sanity check
  assert(width() == other.width());
  assert(height() == other.height());

  // add
  std::transform(begin(), end(), other.begin(), begin(), std::plus<T>());

  return *this;
}


////////////////
// Operator-= //
////////////////
template<typename T>
mat<T>& mat<T>::operator-=(const mat<T>& other)
{
  // sanity check
  assert(width() == other.width());
  assert(height() == other.height());

  // subtract
  std::transform(begin(), end(), other.begin(), begin(), std::minus<T>());
    
  return *this;
}


///////////////
// Threshold //
///////////////
template<typename T>
mat<T>& mat<T>::threshold(typename mat<T>::const_reference value)
{
  return threshold(-value, +value);
}


template<typename T>
mat<T>& mat<T>::threshold(typename mat<T>::const_reference neg_value, typename mat<T>::const_reference pos_value)
{
  for(iterator itr = begin(); itr != end(); itr++)
    if(*itr < pos_value && *itr > neg_value) *itr = (T)(0);
  return *this;
}


/////////
// Abs //
/////////
template<typename T>
mat<T>& mat<T>::Abs(void) 
{
  for(iterator itr = begin(); itr != end(); itr++)
    if(*itr < (T)(0)) *itr = -(*itr);
  return *this;
}


///////////////
// transpose //
///////////////
template<typename T>
mat<T> mat<T>::transpose(void) const
{
  mat<T> result(height(), width());
    
  for(int w=0; w < width(); w++)
    for(int h=0; h < height(); h++)
      result(h, w) = (*this)(w, h);

  return result;
}


///////////////////////
// _swap (protected) //
///////////////////////
template<typename T>
void mat<T>::_swap(mat<T>& swp)
{
  std::swap(_width, swp._width);
  std::swap(_height, swp._height);
  std::swap(_data, swp._data);
}


///////////////////////
// _init (protected) //
///////////////////////
template<typename T>
void mat<T>::_init(int width, int height, const T* data)
{
  this->_data = NULL;
  _alloc(width,height);
  if(data != NULL) std::copy(&data[0], &data[width*height], this->_data);
}


////////////////////////
// _alloc (protected) //
////////////////////////
template<typename T>
void mat<T>::_alloc(int width, int height)
{
  _dealloc();
  unsigned int size = width*height;
  if(size == 0) return;
  this->_data = new T[size];
  this->_width = width;
  this->_height = height;
}


//////////////////////////
// _dealloc (protected) //
//////////////////////////
template<typename T>
void mat<T>::_dealloc(void)
{
  if(_data != NULL) delete[] _data;
  _data = NULL;
  _width = _height = 0;
}


//////////////////////////
// _compare (protected) //
//////////////////////////
template<typename T>
int mat<T>::_compare(const mat<T>& m) const
{
  // handle different sizes
  if(m.width() != width() || m.height() != height()) return -1;
  
  // handle same size
  int pos = 0;
  int neg = 0;
  int zero = 0;
  
  const_iterator mitr = m.begin();
  for(const_iterator itr = begin(); itr != end(); itr++, mitr++)
    if(*itr < *mitr) neg++;
    else if(*itr > *mitr) pos++;
    else zero++;

  // Done.
  if(zero == width()*height()) return 0;
  else if(pos == neg) return -1;
  else return pos - neg;
}

#endif /* _MAT_INLINE_H_ */
