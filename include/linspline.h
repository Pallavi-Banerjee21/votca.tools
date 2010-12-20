/*
 * Copyright 2009 The VOTCA Development Team (http://www.votca.org)
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

#ifndef _LINSPLINE_H
#define	_LINSPLINE_H

#include "spline.h"
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/vector_expression.hpp>
#include <iostream>

namespace votca { namespace tools {

namespace ub = boost::numeric::ublas;

/**
 * \brief A Linear Spline Class
 *
 *  class supports linear interpolation and linear fit of data
*/

class LinSpline : public Spline
{
public:
    // default constructor
    LinSpline() {};
    //LinSpline() :
    //    _boundaries(splineNormal) {}

    // destructor
    ~LinSpline() {};

    // construct an interpolation spline
    // x, y are the the points to construct interpolation, both vectors must be of same size
    void Interpolate(ub::vector<double> &x, ub::vector<double> &y);

    // fit spline through noisy data
    // x,y are arrays with noisy data, both vectors must be of same size
    void Fit(ub::vector<double> &x, ub::vector<double> &y);

    // Calculate the function value
    double Calculate(const double &x);

    // Calculate the function derivative
    double CalculateDerivative(const double &x);

    // Calculate the function value for a whole array, story it in y
    template<typename vector_type1, typename vector_type2>
    void Calculate(vector_type1 &x, vector_type2 &y);

    // Calculate the derivative value for a whole array, story it in y
    template<typename vector_type1, typename vector_type2>
    void CalculateDerivative(vector_type1 &x, vector_type2 &y);
    

protected:
    // a,b for piecewise splines: ax+b
    ub::vector<double> a;
    ub::vector<double> b;
};

inline double LinSpline::Calculate(const double &r)
{
    int interval =  getInterval(r);
    return a(interval)*r + b(interval);
}

inline double LinSpline::CalculateDerivative(const double &r)
{
    int interval =  getInterval(r);
    return a(interval);
}

}}

#endif	/* _LINSPLINE_H */

