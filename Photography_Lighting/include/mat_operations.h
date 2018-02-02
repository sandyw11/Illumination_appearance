#ifndef _MAT_OPERATIONS_H_
#define _MAT_OPERATIONS_H_

#include "mat.h"
#include "tempArray.h"

namespace lapack {     // ! We need this namespace to avoid conflicts with STL

extern "C" {
#include "f2c.h"
#include "clapack.h"

#undef min             // ! must undef min/max macro to use STL
#undef max
#undef abs
#undef M
#undef N
}

}

using namespace lapack;

///////////////////////////////////
// Compute Eigenvalues & Vectors //
// Returns: eigenvalues          //
// Replaces matrix by eigenvectors/
// stored in colummns. Eigenvalues/
// sorted from lowest to highest //
///////////////////////////////////
template<typename T>
mat<T> eigenDecomposition(mat<T>& m)
{
  // NOT SUPPORTED TYPE!
  assert(false);
  return mat<T>();
}

template<>
mat<float> eigenDecomposition<float>(mat<float>& m)
{
  // sanity check
  assert(m.width() == m.height());
  int Size = m.width();

  // setup vars for LAPACK
  integer status;
  char JOBS = 'V';
  char UPLO = 'U';
  integer N = Size;
  integer LWORK = 1 + 6*Size + 2*Size*Size;
  integer LIWORK = 3 + 5*Size;
  mat<float> diagonal(1, Size);
  tempArray(float, work, 1 + 6*Size + 2*Size*Size);
  tempArray(integer, iwork, 3 + 5*Size);

  // call LAPACK
  ssyevd_(&JOBS, &UPLO, &N, m.begin(), &N, diagonal.begin(), work, &LWORK, iwork, &LIWORK, &status);

  // check status
  if(status < 0) assert(false);    // UNEXPECTED ERROR, one of the parameters is wrong??
  if(status > 0)
  {
    std::cerr << "WARNING: Degenerate matrix. Eigen-decomposition failed.\r\n";
  }

  return diagonal;
}

template<>
mat<double> eigenDecomposition<double>(mat<double>& m)
{
  // sanity check
  assert(m.width() == m.height());
  int Size = m.width();

  // setup vars for LAPACK
  integer status;
  char JOBS = 'V';
  char UPLO = 'U';
  integer N = Size;
  integer LWORK = 1 + 6*Size + 2*Size*Size;
  integer LIWORK = 3 + 5*Size;
  mat<double> diagonal(1, Size);
  tempArray(double, work, 1 + 6*Size + 2*Size*Size);
  tempArray(integer, iwork, 3 + 5*Size);

  // call LAPACK
  dsyevd_(&JOBS, &UPLO, &N, m.begin(), &N, diagonal.begin(), work, &LWORK, iwork, &LIWORK, &status);

  // check status
  if(status < 0) assert(false);    // UNEXPECTED ERROR, one of the parameters is wrong??
  if(status > 0)
  {
    std::cerr << "WARNING: Degenerate matrix. Eigen-decomposition failed.\r\n";
  }

  return diagonal;
}


///////////////////////////////////////////
//     Singular Value Decomposition      //
///////////////////////////////////////////
//                                       //
//      Matrix is destroyed!!!           //
//                                       //
// sigma = svd(m, U, V)                  //
// D = diagMat(sigma, V.Height, U.Width) //
// m = U*D*V                             //
///////////////////////////////////////////
template<typename T>
mat<T> svd(mat<T>& m, mat<T>& U, mat<T>& V)
{
  // NON SUPPORTED TYPE
  assert(false);
}

template<>
mat<float> svd<float>(mat<float>& m, mat<float>& U, mat<float>& V)
{
  // setup vars for LAPACK
  integer status;
  char JOBS = 'A';
  integer N = m.width();
  integer M = m.height();
  float optimalWork;
  integer LWORK = -1;
  integer LIWORK = 8 * std::min(M,N);
  tempArray(integer, iwork, LIWORK);

  mat<float> sigma(std::min(N, M), 1);
  U = mat<float>(M, M);
  V = mat<float>(N, N);
  
  if(N == 0 || M == 0)
  {
    U = (float)(0);
    V = (float)(0);
    sigma = (float)(0);
    return sigma;
  }

  // get optimal values for work spaces
  sgesdd_(&JOBS, &M, &N, m.begin(), &M, sigma.begin(), U.begin(), &M, V.begin(), &N, &optimalWork, &LWORK, iwork, &status);

  // sanity check
  if(status < 0) assert(false);    // UNEXPECTED ERROR, one of the parameters is wrong??

  // allocate workspace
  LWORK = (integer)(optimalWork);
  tempArray(float, work, LWORK);

  // compute svd (LAPACK)
  sgesdd_(&JOBS, &M, &N, m.begin(), &M, sigma.begin(), U.begin(), &M, V.begin(), &N, work, &LWORK, iwork, &status);

  // check status
  if(status < 0) assert(false);    // UNEXPECTED ERROR, one of the parameters is wrong??
  if(status > 0)
  {
    std::cerr << "ERROR: failed to converge, updating process failed.\r\n";
    assert(false);
  }

  // done.
  return sigma;
}

template<>
mat<double> svd<double>(mat<double>& m, mat<double>& U, mat<double>& V)
{
  // setup vars for LAPACK
  integer status;
  char JOBS = 'A';
  integer N = m.width();
  integer M = m.height();
  double optimalWork;
  integer LWORK = -1;
  integer LIWORK = 8 * std::min(M,N);
  tempArray(integer, iwork, LIWORK);

  mat<double> sigma(std::min(N, M), 1);
  U = mat<double>(M, M);
  V = mat<double>(N, N);

  if(N == 0 || M == 0)
  {
    U = (float)(0);
    V = (float)(0);
    sigma = (float)(0);
    return sigma;
  }
  // get optimal values for work spaces
  dgesdd_(&JOBS, &M, &N, m.begin(), &M, sigma.begin(), U.begin(), &M, V.begin(), &N, &optimalWork, &LWORK, iwork, &status);

  // sanity check
  if(status < 0) assert(false);    // UNEXPECTED ERROR, one of the parameters is wrong??

  // allocate workspace
  LWORK = (integer)(optimalWork);
  tempArray(double, work, LWORK);

  // compute svd (LAPACK)
  dgesdd_(&JOBS, &M, &N, m.begin(), &M, sigma.begin(), U.begin(), &M, V.begin(), &N, work, &LWORK, iwork, &status);

  // check status
  if(status < 0) assert(false);    // UNEXPECTED ERROR, one of the parameters is wrong??
  if(status > 0)
  {
    std::cerr << "ERROR: failed to converge, updating process failed.\r\n";
    assert(false);
  }

  // done.
  return sigma;
}


/////////////////////
// Compute Inverse //
/////////////////////
template<typename T>
void invert(mat<T>& m)
{
  // NOT SUPPORTED TYPE!
  assert(false);
}

template<>
void invert<float>(mat<float>& m)
{
  // sanity check
  assert(m.width() == m.height());
  int Size = m.width();

  // setup vars for LAPACK
  integer status;
  integer N = Size;
  tempArray(integer, ipiv, Size);
  tempArray(float, work, Size);
  integer LWORK = Size;
  
  // call LAPACK
  sgetrf_(&N, &N, m.begin(), &N, ipiv, &status);

  // check status
  if(status < 0) assert(false);    // UNEXPECTED ERROR, one of the parameters is wrong??
  if(status > 0)
  {
    std::cerr << "ERROR: Unable to compute inverse: " << status << "-th element is zero.\r\n";
    assert(false);
  }

  // call LAPACK
  sgetri_(&N, m.begin(), &N, ipiv, work, &LWORK, &status);

  // check status
  if(status < 0) assert(false);    // UNEXPECTED ERROR, one of the parameters is wrong??
  if(status > 0)
  {
    std::cerr << "ERROR: Unable to compute inverse: " << status << "-th element is zero.\r\n";
    assert(false);
  }
}

template<>
void invert<double>(mat<double>& m)
{
  // sanity check
  assert(m.width() == m.height());
  int Size = m.width();

  // setup vars for LAPACK
  integer status;
  integer N = Size;
  tempArray(integer, ipiv, Size);
  tempArray(double, work, Size);
  integer LWORK = Size;
  
  // call LAPACK
  dgetrf_(&N, &N, m.begin(), &N, ipiv, &status);

  // check status
  if(status < 0) assert(false);    // UNEXPECTED ERROR, one of the parameters is wrong??
  if(status > 0)
  {
    std::cerr << "ERROR: Unable to compute inverse: " << status << "-th element is zero.\r\n";
    assert(false);
  }

  // call LAPACK
  dgetri_(&N, m.begin(), &N, ipiv, work, &LWORK, &status);

  // check status
  if(status < 0) assert(false);    // UNEXPECTED ERROR, one of the parameters is wrong??
  if(status > 0)
  {
    std::cerr << "ERROR: Unable to compute inverse: " << status << "-th element is zero.\r\n";
    assert(false);
  }
}


/////////////////////////
// Solve Linear System //
/////////////////////////
// A and B are modified//
// B contains the sol. //
/////////////////////////
template<typename T>
void linear_solve(mat<T>& A, mat<T>& B)
{
  // NOT SUPPORTED TYPE!
  assert(false);
}

template<>
void linear_solve<float>(mat<float>& A, mat<float>& B)
{
  // sanity check
  assert(A.height() == B.height());
  int Size = B.height();
  int numSystems = B.width();

  // setup vars for LAPACK
  integer status;
  integer N = Size;
  integer NRHS = numSystems;
  tempArray(integer, ipiv, Size);
  
  // call LAPACK
  sgesv_(&N, &NRHS, A.begin(), &N, ipiv, B.begin(), &N, &status);

  // check status
  if(status < 0) assert(false);    // UNEXPECTED ERROR, one of the parameters is wrong??
  if(status > 0)
  {
    std::cerr << "ERROR: Unable to solve system: " << status << "-th element of U is zero.\r\n";
    assert(false);
  }

  // done.
}

template<>
void linear_solve<double>(mat<double>& A, mat<double>& B)
{
  // sanity check
  assert(A.height() == B.height());
  int Size = B.height();
  int numSystems = B.width();

  // setup vars for LAPACK
  integer status;
  integer N = Size;
  integer NRHS = numSystems;
  tempArray(integer, ipiv, Size);
  
  // call LAPACK
  dgesv_(&N, &NRHS, A.begin(), &N, ipiv, B.begin(), &N, &status);

  // check status
  if(status < 0) assert(false);    // UNEXPECTED ERROR, one of the parameters is wrong??
  if(status > 0)
  {
    std::cerr << "ERROR: Unable to solve system: " << status << "-th element of U is zero.\r\n";
    assert(false);
  }

  // done.
}


/////////////////////////
// Solve Least Squares //
// min |Ax - B|        //
/////////////////////////
// A and B are modified//
// B contains the sol. //
/////////////////////////
template<typename T>
void least_squares(mat<T>& A, mat<T>& B)
{
  // NOT SUPPORTED TYPE!
  assert(false);
}

template<>
void least_squares<float>(mat<float>& A, mat<float>& B)
{
  // sanity check
  assert(A.height() == B.height());

  // init
  int Bwidth = B.width();
  int Bheight = B.height();

  // setup vars for LAPACK
  char TRANS = 'N';
  integer M = A.height();
  integer N = A.width();
  integer NRHS = B.width();
  integer LDA = M;
  integer LDB = std::max(N, M);
  float optimalSize;
  integer LWORK = -1;
  integer status;
  
  // extend B if needed
  mat<float> extendedB;

  if(Bheight == LDB)
  {
    // no copy needed => swap
    std::swap(extendedB, B);
  }
  else {
    // extend => copy needed
    mat<float> temp(Bwidth, LDB);
    for(int i=0; i < Bheight; i++)
      for(int j=0; j < temp.width(); j++)
	temp(j,i) = B(j,i);
    std::swap(extendedB, temp);
  }

  // call LAPACK (get optimalSize)
  sgels_(&TRANS, &M, &N, &NRHS, A.begin(), &LDA, extendedB.begin(), &LDB, &optimalSize, &LWORK, &status);

  // allocate WORK
  LWORK = (integer)(optimalSize);
  tempArray(float, WORK, LWORK);

  // call LAPACK
  sgels_(&TRANS, &M, &N, &NRHS, A.begin(), &LDA, extendedB.begin(), &LDB, WORK, &LWORK, &status);

  // check status
  if(status < 0) assert(false);    // UNEXPECTED ERROR, one of the parameters is wrong??
  if(status > 0)
  {
    std::cerr << "ERROR: Unable to solve system: " << status << "-th element of U is zero.\r\n";
    assert(false);
  }

  // copy solution
  if(A.width() == Bheight)
  {
    // no copy needed => swap
    std::swap(extendedB, B);
  }
  else {
    mat<float> temp(Bwidth, A.width());

    // extend => copy needed
    for(int i=0; i < A.width(); i++)
      for(int j=0; j < temp.width(); j++)
	temp(j,i) = extendedB(j,i);

    std::swap(temp, B);
  }

  // done.
}

template<>
void least_squares<double>(mat<double>& A, mat<double>& B)
{
  // sanity check
  assert(A.height() == B.height());

  // init
  int Bwidth = B.width();
  int Bheight = B.height();

  // setup vars for LAPACK
  char TRANS = 'N';
  integer M = A.height();
  integer N = A.width();
  integer NRHS = B.width();
  integer LDA = M;
  integer LDB = std::max(N, M);
  double optimalSize;
  integer LWORK = -1;
  integer status;
  
  // extend B if needed
  mat<double> extendedB;

  if(Bheight == LDB)
  {
    // no copy needed => swap
    std::swap(extendedB, B);
  }
  else {
    // extend => copy needed
    mat<double> temp(Bwidth, LDB);
    for(int i=0; i < Bheight; i++)
      for(int j=0; j < temp.width(); j++)
	temp(j,i) = B(j,i);
    std::swap(extendedB, temp);
  }

  // call LAPACK (get optimalSize)
  dgels_(&TRANS, &M, &N, &NRHS, A.begin(), &LDA, extendedB.begin(), &LDB, &optimalSize, &LWORK, &status);

  // allocate WORK
  LWORK = (integer)(optimalSize);
  tempArray(double, WORK, LWORK);

  // call LAPACK
  dgels_(&TRANS, &M, &N, &NRHS, A.begin(), &LDA, extendedB.begin(), &LDB, WORK, &LWORK, &status);

  // check status
  if(status < 0) assert(false);    // UNEXPECTED ERROR, one of the parameters is wrong??
  if(status > 0)
  {
    std::cerr << "ERROR: Unable to solve system: " << status << "-th element of U is zero.\r\n";
    assert(false);
  }

  // copy solution
  if(A.width() == Bheight)
  {
    // no copy needed => swap
    std::swap(extendedB, B);
  }
  else {
    mat<double> temp(Bwidth, A.width());

    // extend => copy needed
    for(int i=0; i < A.width(); i++)
      for(int j=0; j < temp.width(); j++)
	temp(j,i) = extendedB(j,i);

    std::swap(temp, B);
  }

  // done.
}

#endif /* _MAT_OPERATIONS_H_ */
