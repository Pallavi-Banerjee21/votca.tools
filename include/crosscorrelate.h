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

#ifndef _crosscorrelate_H
#define	_crosscorrelate_H

#include <vector>
#include <iostream>
#include "datacollection.h"

using namespace std;

/**
    \brief class to calculate cross correlkations and autocorrelations

    This class produces a histogram out of a vector of values

    \todo implementation
*/
class CrossCorrelate
{
    public:
        /// constructor
        CrossCorrelate() {};
        /// destructor
        ~CrossCorrelate() {};
        
        /**
            calculate the cross correlation
         */
        //void CrossCorrelate(DataCollection<double>::selection *data1, 
        //    DataCollection<double>::selection *data2, bool average = false);
        
        /**
            calculate the auto correlation
               
         */
        void AutoCorrelate(DataCollection<double>::selection *data, bool average = false);

        // Calculates only the Fourier trafo
        void FFTOnly(vector <double>& ivec);
        
        // Calculates only the Discrete Cosine trafo
        void DCTOnly(vector <double>& ivec);
        
        // Calculates Fourier trafo and then auto correlation
        void AutoFourier(vector <double>& ivec);
        
        // Calculates Discrete Cosine trafo and then auto correlation
        void AutoCosine(vector <double>& ivec);
        
        // Calculates auto correlation via two Fourier trafos
        void AutoCorr(vector <double>& ivec);
        
        vector<double> &getData() { return _corrfunc; }
    private:
        vector<double> _corrfunc;
};

inline ostream& operator<<(ostream& out, CrossCorrelate &c)
{
    vector<double> &data = c.getData();
    for(size_t i=0; i<data.size(); i++) {
        out << i << " " << c.getData()[i] << endl;
    }
    return out;
}

#endif	/* _crosscorrelate_H */

