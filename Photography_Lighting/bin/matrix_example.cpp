#include <iostream>

#include "mat.h"
#include "mat_operations.h"

int main(int argc, char** argv)
{
  float avalues[12] = {1.0f, 2.0f, 3.0f, 4.0f, 
		       8.0f, 7.0f, 6.0f, 5.0f, 
		       9.0f, 4.0f, 7.0f, 5.0f};
		       
  float bvalues[4] = {1.0f, 6.0f, 1.0f, 2.0f};

  // create matrices and copy the values from 'avalues' and 'bvalues'
  mat<float> a(3, 4, avalues);
  mat<float> b(1, 4, bvalues);

  // output
  std::cerr << a << std::endl;
  std::cerr << b << std::endl;

  std::cerr << "a(0,0): " << a(0,0) << std::endl;
  std::cerr << "a(1,0): " << a(1,0) << std::endl;
  std::cerr << "a(0,2): " << a(0,2) << std::endl;

  // test least squares
  least_squares(a, b);
  std::cerr << std::endl << "Solution to ax=b: " << std::endl << b << std::endl;

  // Done.
  return 0;
}
