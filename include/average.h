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

/* 
 * File:   average.h
 * Author: ruehle
 *
 * Created on July 17, 2008, 2:08 PM
 */

#ifndef _AVERAGE_H
#define	_AVERAGE_H

// do not use this calss yet!
template<typename T>
class Average
{
public:
    Average();
    ~Average() {}
    
    void Process(T &v);
    template<typename iterator_type>
    void ProcessRange(const iterator_type &begin, const iterator_type   &end);
    double CalcDev();
    double GetAv();
    
private:
    
    T _av; // average
    T _m2; // second moment
    size_t _n;
};

template<typename T>
Average<T>::Average()
: _n(0) {}

template <>
inline Average<double>::Average()
: _n(0), _av(0), _m2(0) {}

template<typename T>
inline void Average<T>::Process(T &value)
{
    if(_n==0){_av = value; _m2= value*value;}
    else {
        _av = _av*(double)_n/(double)(_n+1) + value / (double)(_n+1);
        _n++;
        _m2 += value*value;
    }
}

template<>
inline void Average<double>::Process(double &value)
{
    _av = _av*(double)_n/(double)(_n+1) + value / (double)(_n+1);
    _n++;
    _m2 += value*value;
}

template<typename T>
template<typename iterator_type>
void Average<T>::ProcessRange(const iterator_type &begin, const iterator_type   &end){ 
    for(iterator_type iter=begin; iter!=end; ++iter){
        Process(*iter);
    }
}

template<typename T>
double Average<T>::CalcDev(){
    double dev = 0.0;
    dev = sqrt((_m2-_n*_av*_av)/(_n-1));
    return dev;
}

template<typename T>
double Average<T>::GetAv(){
    return _av;
}
#endif	/* _AVERAGE_H */

