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

#ifndef _AKIMASPLINE_H
#define	_AKIMASPLINE_H

#include "spline.h"
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/vector_expression.hpp>
#include <iostream>

namespace votca { namespace tools {

namespace ub = boost::numeric::ublas;

/**
 *
 * \brief An Akima Spline Class
 *
 * does Akima interpolation based on the paper
 * "A new method of interpolation and smooth curve fitting based on local procedures"
 *
 * Fitting is not supported. In order to fit data, do linear fitting and interpolate
 * the linear fitted values by Akima interpolation.
 */

class AkimaSpline : public Spline
{    
public:
    // default constructor
    AkimaSpline() {};
    //AkimaSpline() :
    //    _boundaries(splineNormal) {}
    
    // destructor
    ~AkimaSpline() {};

    /**
     * \brief Calculate the slope according to the original Akima paper ("A New Method of Interpolation and Smooth Curve Fitting Based on Local Procedures")
     * \param slopes m1 to m4 of line segments connecting the five data points
     * \return slope
     * handles all special cases to determine the slope t based on slopes m1,m2,m3,m4
     */
    double getSlope(double m1, double m2, double m3, double m4);


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
    // p1,p2,p3,p4 and t1,t2 (same identifiers as in Akima paper, page 591)
    ub::vector<double> p0;
    ub::vector<double> p1;
    ub::vector<double> p2;
    ub::vector<double> p3;
    ub::vector<double> t;
};

inline double AkimaSpline::Calculate(const double &r)
{
    int interval =  getInterval(r);
    double z = r-_r[interval];
    return  p0(interval)
            + p1(interval)*z
            + p2(interval)*z*z
            + p3(interval)*z*z*z;
}

inline double AkimaSpline::CalculateDerivative(const double &r)
{
    int interval =  getInterval(r);
    double z = r-_r[interval];
    return  + p1(interval)
            + 2.0*p2(interval)*z
            + 3.0*p3(interval)*z*z;
}

inline double AkimaSpline::getSlope(double m1, double m2, double m3, double m4)
{
    if ((m1==m2) && (m3==m4)) {
        return (m2+m3)/2.0;
    } else {
        return (fabs(m4-m3)*m2 + fabs(m2-m1)*m3) / (fabs(m4-m3) + fabs(m2-m1));
    }
}

}}

#endif	/* _AKIMASPLINE_H */

