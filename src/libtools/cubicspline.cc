/* 
 * Copyright 2009-2011 The VOTCA Development Team (http://www.votca.org)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <votca/tools/cubicspline.h>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <votca/tools/linalg.h>
#include <gsl/gsl_linalg.h>
#include <iostream>
#include <cmath>

namespace votca { namespace tools {

using namespace std;

void CubicSpline::Interpolate(ub::vector<double> &x, ub::vector<double> &y)
{    
    if(x.size() != y.size())
        throw std::invalid_argument("error in CubicSpline::Interpolate : sizes of vectors x and y do not match");
    
    const int N = x.size();
    
    // adjust the grid
    _r.resize(N);
    _f.resize(N);
    _f2.resize(N);
    
    // create vector proxies to individually access f and f''

    // copy the grid points into f
    _r = x;
    _f = y;
    _f2 = ub::zero_vector<double>(N);
    
    // not calculate the f''
    ub::matrix<double> A(N, N);
    A = ub::zero_matrix<double>(N,N);
    
    for(int i=0; i<N - 2; ++i) {
            _f2(i+1) = -( A_prime_l(i)*_f(i)
            + (B_prime_l(i) - A_prime_r(i)) * _f(i+1)
            -B_prime_r(i) * _f(i+2));

            A(i+1, i) = C_prime_l(i);
            A(i+1, i+1) = D_prime_l(i) - C_prime_r(i);
            A(i+1, i+2) = -D_prime_r(i);
    }
    
    switch(_boundaries) {
        case splineNormal:
            A(0, 0) = 1;
            A(N - 1, N-1) = 1;
            break;
        case splinePeriodic:
            A(0,0) = 1; A(0,N-1) = -1;
            A(N-1,0) = 1; A(N-1,N-1) = -1;
            break;
        case splineDerivativeZero:
	    throw std::runtime_error("erro in CubicSpline::Interpolate: case splineDerivativeZero not implemented yet");
	    break;
    }

    votca::tools::linalg_qrsolve(_f2, A, _f2);
}

void CubicSpline::Fit(ub::vector<double> &x, ub::vector<double> &y)
{
    if(x.size() != y.size())
        throw std::invalid_argument("error in CubicSpline::Fit : sizes of vectors x and y do not match");
    
    const int N = x.size();
    const int ngrid = _r.size();
    
    // construct the equation
    // A*u = b
    // where u = { {f[i]}, {f''[i]} }
    // and b[i] = y[i] for 0<=i<N
    // and b[i]=0 for i>=N (for smoothing condition)
    // A[i,j] contains the data fitting + the spline smoothing conditions
    
    ub::matrix<double> A(N, 2*ngrid);
    ub::vector<double> b(N);    
    ub::matrix<double> B_constr(ngrid, 2*ngrid);  // Matrix with smoothing conditions

    A = ub::zero_matrix<double>(N, 2*ngrid);
    b  = ub::zero_vector<double>(N);
    B_constr = ub::zero_matrix<double>(ngrid, 2*ngrid);
    
    // Construct smoothing matrix
    AddBCToFitMatrix(B_constr, 0);

    // construct the matrix to fit the points and the vector b
    AddToFitMatrix(A, x, 0);
    b = -y; // why is it -y?

    // now do a constrained qr solve
    ub::vector<double> sol(2*ngrid);
    votca::tools::linalg_constrained_qrsolve(sol, A, b, B_constr);

    // check vector "sol" for nan's
    for(int i=0; i<2*ngrid; i++) {
        if( (isinf(sol(i))) || (isnan(sol(i))) ) {
            throw std::runtime_error("error in CubicSpline::Fit : value nan occurred due to wrong fitgrid boundaries");
        }
    }

    _f = ub::vector_range<ub::vector<double> >(sol, ub::range (0, ngrid));
    _f2 = ub::vector_range<ub::vector<double> >(sol, ub::range (ngrid, 2*ngrid));
}

}}
