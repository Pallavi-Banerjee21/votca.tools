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
 * File:   datacollection.h
 * Author: ruehle
 *
 * Created on May 10, 2007, 3:40 PM
 */

#ifndef _datacollection_H
#define	_datacollection_H

#include <assert.h>
#include <vector>
#include <map>
#include <sstream>
#include "tokenizer.h"

using namespace std;
   
template<typename T>
/**
    \brief This class handles a set of arrays which can be identified by name tags
    
    This class is a Container of arrays. The arrays can be accessed by specifying a name, 
    or whole groups of arrays can be selected using select an regular expressions.
    Regular expressions are not fully implemented at the moment. Instead, selections are performed
    using wildcard compare.
    
    Be aware that you might specify as typename if you define a container, array or iterator!
    There is currently no suppurt for user created groups, but will follow later.

*/
class DataCollection {
public:
    /**
        \brief The array class, extends vector by a name tag
     */
 class array  : public vector< T > 
     {
    public:
        array(string name) {_name = name; }
        const string &getName() { return _name; }
    private:
        string _name;            
     };
    //typedef vector< T > array;
    typedef vector< array * > container;
    typedef typename vector< array * >::iterator iterator;
     
    /**
        \brief class for array selection
     */
    class selection {
    public:
        selection() {}
        ~selection() {}
        
        typedef typename vector<array*>::iterator iterator;
        size_t size() { return _arrays.size(); } 
        bool empty() { return _arrays.empty(); }
        array &operator[](size_t i) { assert(i<_arrays.size()); return *(_arrays[i]); }
        
        void push_back(array *a) { _arrays.push_back(a); }
        void push_back(selection *s) { _arrays.insert(_arrays.end(), s->begin(), s->end()); }
        
        iterator begin() { return _arrays.begin(); }
        iterator end() { return _arrays.end(); }
    private:
        vector<array *> _arrays;
    };    
    
    /// constructor
    DataCollection() {}
    /// destructor
    ~DataCollection() { clear(); }
    
    
    /**
        \brief clears the data collection
    */    
    void clear();
    /**
     *  \ brief returns the number of arrays 
     */
    size_t size() { return _data.size(); }
    bool empty() { return _data.empty(); }
    array &operator[](int i) { assert(i<_data.size()); return *(_data[i]); }
    iterator begin() { return _data.begin(); }
    iterator end() { return _data.end(); }
    
    /**
        \brief create a new array
    */    
    array *CreateArray(string name);
    
    /*
        \brief create a new group
    */    
    //selection *CreateGroup(string group);
    
    /**
        \brief access the data container
     */
    container &Data() { return _data; }
    /**
        \brief access an array by name
     */
    array *ArrayByName(string name);
    
    /*
        \brief group an array by name
     */
    //selection *GroupByName(string name);
    
    /** 
        \brief select a set of arrays
     */
    selection *select(string strselection, selection *sel_append=NULL);
    
    //map<string, selection *> &Groups() { return _group_by_name; }
    
private:
    container _data;
    
    map<string, array *> _array_by_name;
    //map<string, selection *> _group_by_name;
};


template<typename T>
void DataCollection<T>::clear()
{
    {
        typename container::iterator iter;
        for(iter=_data.begin(); iter!=_data.end(); ++iter)
            delete *iter;
        _data.clear();
    }
/*    _array_by_name.clear();
    {
        typename map<string, selection * >::iterator iter;
        for(iter=_group_by_name.begin();iter!=_group_by_name.end();++iter)
            delete (*iter).second;
        _group_by_name.clear();
    }*/
}

template<typename T>
typename DataCollection<T>::array *DataCollection<T>::CreateArray(string name)
{
   
    
    assert(ArrayByName(name)==NULL);
    array *a = new array(name);    
    _data.push_back(a);
    _array_by_name[name.c_str()] = a;
    
    return a;
}

/*template<typename T>
typename DataCollection<T>::selection *DataCollection<T>::CreateGroup(string group)
{
    selection *s = new selection;
    _group_by_name[group] = s;
    return s;
}*/

template<typename T>
typename DataCollection<T>::array *DataCollection<T>::ArrayByName(string name)
{
    typename map<string,array*>::iterator i;
    i = _array_by_name.find(name);
    if(i == _array_by_name.end()) return NULL;
    return (*i).second;
}

/*template<typename T>
typename DataCollection<T>::selection *DataCollection<T>::GroupByName(string name)
{
    typename map<string,selection*>::iterator i;
    i = _group_by_name.find(name);
    if(i == _group_by_name.end()) return NULL;
    return (*i).second;
}*/   
    
template<typename T>
typename DataCollection<T>::selection *DataCollection<T>::select(string strselection, selection *sel_append)
{
    typename DataCollection<T>::selection *sel = sel_append;
    if(!sel_append) sel = new typename DataCollection<T>::selection;
    
    for(typename map<string,array*>::iterator i=_array_by_name.begin(); i!=_array_by_name.end();++i) {
        if(wildcmp(strselection.c_str(), (*i).second->getName().c_str()))
            sel->push_back((*i).second);
    }
    return sel;
}

ostream& operator<<(ostream& out, DataCollection<double>::selection &sel);

#endif	/* _datacontainer_H */

