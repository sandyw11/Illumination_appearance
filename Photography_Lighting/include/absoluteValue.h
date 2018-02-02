#ifndef _ABSOLUTEVALUE_H_
#define _ABSOLUTEVALUE_H_

#include <functional>
#include <cmath>

using namespace std;

template<typename T>
struct absoluteValue : public unary_function<T, T>
{
  T operator()(const T& t) const { return abs(t); }
};


#endif /* _ABSOLUTEVALUE_H_ */
