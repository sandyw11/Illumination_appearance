#ifndef _RAISES_H_
#define _RAISES_H_

#include <functional>
#include <cmath>

using namespace std;

template<typename T, typename S>
struct raises : public binary_function<T, S, T>
{
  T operator()(const T& t, const S& s) const { return pow(t, s); }
};


#endif /* _RAISES_H_ */
