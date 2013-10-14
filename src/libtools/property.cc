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

#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <expat.h>
#include <string.h>
#include <fstream>
#include <string>
#include <stack>
#include <iomanip>

#include <votca/tools/property.h>
#include <votca/tools/tokenizer.h>
#include <votca/tools/propertyiomanipulator.h>

#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

namespace votca { namespace tools {

// ostream modifier defines the output format, level, indentation
const int Property::IOindex = std::ios_base::xalloc(); 
   
Property &Property::get(const string &key)
{
    Tokenizer tok(key, ".");
    Tokenizer::iterator n;
    
    n = tok.begin();
    if(n==tok.end()) return *this;
    
    Property *p;
    map<string, Property*>::iterator iter;
    if(*n=="") {
        p = this;
    }
    else {
        iter = _map.find(*n);
    
        if(iter == _map.end())
            throw runtime_error("property not found: " + key);
    
        p = (((*iter).second));
    }
    ++n;
    try {
        for(; n!=tok.end(); ++n) {
            p = &p->get(*n);
        }
    }
    catch(string err) { // catch here to get full key in exception
        throw runtime_error("property not found: " + key);
    }
    
    return *p;
}

std::list<Property *> Property::Select(const string &filter)
{
    Tokenizer tok(filter, ".");
    
    std::list<Property *> selection;

    if(tok.begin()==tok.end()) return selection;
    
    selection.push_back(this);
        
    for (Tokenizer::iterator n = tok.begin();
            n != tok.end(); ++n) {
        std::list<Property *> childs;
        for (std::list<Property *>::iterator p = selection.begin();
                p != selection.end(); ++p) {
                for (list<Property>::iterator iter = (*p)->_properties.begin();
                    iter != (*p)->_properties.end(); ++iter) {
                    if (wildcmp((*n).c_str(), (*iter).name().c_str())) {
                        childs.push_back(&(*iter));
                    }
                }
        }
        selection = childs;        
    }

    return selection;
}

static void start_hndl(void *data, const char *el, const char **attr)
{
    stack<Property *> *property_stack =
        (stack<Property *> *)XML_GetUserData((XML_Parser*)data);

    Property *cur = property_stack->top();
    Property &np = cur->add(el, "");
    
    for (int i = 0; attr[i]; i += 2)
        np.setAttribute(attr[i], attr[i + 1]);    
    
    property_stack->push(&np);
}

static void end_hndl(void *data, const char *el)
{
    stack<Property *> *property_stack =
        (stack<Property *> *)XML_GetUserData((XML_Parser*)data);
    property_stack->pop();
}

void char_hndl(void *data, const char *txt, int txtlen)
{
    stack<Property *> *property_stack =
        (stack<Property *> *)XML_GetUserData((XML_Parser*)data);

    Property *cur = property_stack->top();
    cur->value().append(txt, txtlen);
}

bool load_property_from_xml(Property &p, string filename)
{
  XML_Parser parser = XML_ParserCreate(NULL);
  if (! parser)
    throw std::runtime_error("Couldn't allocate memory for xml parser");

  XML_UseParserAsHandlerArg(parser);
  XML_SetElementHandler(parser, start_hndl, end_hndl);
  XML_SetCharacterDataHandler(parser, char_hndl);

  ifstream fl;
  fl.open(filename.c_str());
  if(!fl.is_open())
    throw std::ios_base::failure("Error on open xml file: " + filename);

  stack<Property *> pstack;
  pstack.push(&p);

  XML_SetUserData(parser, (void*)&pstack);
  while(!fl.eof()) {
    string line;
    getline(fl, line);
    line=line + "\n";
    if (! XML_Parse(parser, line.c_str(), line.length(), fl.eof()))
      throw  std::ios_base::failure(filename + ": Parse error at line " +
          boost::lexical_cast<string>(XML_GetCurrentLineNumber(parser)) + "\n" +
          XML_ErrorString(XML_GetErrorCode(parser)));
  }
  fl.close();
  return true;
}

void PrintNodeTXT(std::ostream &out, Property &p, const int start_level, int level, string prefix, string offset)
{
    
    list<Property>::iterator iter;
        
    if((p.value() != "") || p.HasChilds() ) {
        
        if ( level >= start_level ) {
                if((p.value()).find_first_not_of("\t\n ") != std::string::npos)   
                        out << offset << prefix << " = " << p.value() << endl;
        } else {
            prefix="";
        }
    }
        
    for(iter = p.begin(); iter!=p.end(); ++iter) {
        if(prefix=="") {
            level++;
            PrintNodeTXT(out, (*iter), start_level, level, prefix + (*iter).name(), offset );
            level--;
        } else {
            level++;
            PrintNodeTXT(out, (*iter), start_level, level, prefix + "." + (*iter).name(), offset );
            level--;
        }
    }
    
}

void PrintNodeXML(std::ostream &out, Property &p, const int start_level, int level, string prefix,  string offset)
{
    list<Property>::iterator iter;       
    Property::AttributeIterator ia;
    bool _endl = true;
    
        // print starting only from the start_level (the first node (level 0) can be <> </>)
        if ( level >= start_level )  {
            // print the node name
            out << offset << "<" << p.name() ;
            // print the node attributes 
            for(ia = p.firstAttribute(); ia!=p.lastAttribute(); ++ia) 
                out << " " << ia->first << "=\"" << ia->second << "\"" ;
            out << ">";
            
            // print node value if it is not empty
            bool has_value = ( (p.value()).find_first_not_of("\t\n ") != std::string::npos );
            if( has_value ) { out << p.value(); _endl = false; }
            
            // check if we need the end of the line or not
            if( !has_value &&  p.HasChilds() ) out << endl;
            if( !has_value &&  !p.HasChilds() ) _endl = false;
        }
    
        // continue iteratively through the rest of the nodes
        for(iter = p.begin(); iter!=p.end(); ++iter) {
            level++; 
            if (  level > start_level ) offset += "\t"; 
            PrintNodeXML(out, (*iter), start_level, level, (*iter).name(), offset);
            if (  level > start_level ) offset.resize(offset.size()-1); 
            level--;           
        }
        
        if ( level >= start_level ) {
            if ( _endl ) {
                out << offset << "</" << p.name() << ">"  << endl;
            } else {
                out << "</" << p.name() << ">"  << endl;
            }
        } 
}
    
void PrintNodeTEX(std::ostream &out, Property &p, const int start_level, int level, string prefix,  string offset) {

    
    list<Property>::iterator iter;       
    string head_name;
    string _label(""); // reference of the xml file in the manual
    string _section(""); // reference of the description section in the manual
    string _help("");
    string _default(""); // default value if supplied
    string _unit(""); //unit, if supplied
 

    string header_format("\\subsection{%1%}\n"
                         "\\label{%2%}\n%3%\n"
                         "\\rowcolors{1}{invisiblegray}{white}\n"
                         "{\\small\n "
                         "\\begin{longtable}{m{3cm}|m{2cm}|m{1cm}|m{8cm}}\n"
                         " option & default & unit & description\\\\\n\\hline\n");    
    
    string footer_format("\\end{longtable}\n}\n"
                         "\\noindent Return to the description of \\slink{%1%}{\\texttt{%2%}}.\n");
    
    string body_format(" \\hspace{%1%pt}\\hypertarget{%2%}{%3%} & %4% & %5% & %6% \\\\\n");
    
    // if this is the head node, print the header
    if ( level == start_level )  {
            head_name = p.name();
            if ( p.hasAttribute("label") ) _label = p.getAttribute<string>("label");
            if ( p.hasAttribute("section") ) _section = p.getAttribute<string>("section");
            if ( p.hasAttribute("help") ) _help = p.getAttribute<string>("help");
            out << boost::format(header_format) % head_name % _label % _help;     
            prefix = p.name();
     } 
    
    
    if ( level > start_level ) {
    
        // if this node has children or a value or is not the first, start recursive printing
        if( ( p.value() != "" || p.HasChilds() ) && level > -1) {
            string _tex_name = boost::replace_all_copy( p.name(), "_", "\\_" );
            
            if ( p.hasAttribute("default") ) _default = p.getAttribute<string>("default");
            if ( p.hasAttribute("unit") ) _unit = p.getAttribute<string>("unit");
            if ( p.hasAttribute("help") ) _help = p.getAttribute<string>("help");
            
            out << boost::format(body_format) % int((level-start_level-1)*10) 
                    % prefix 
                    % _tex_name 
                    % _default
                    % _unit
                    % _help;
        }
    } 
   
    // continue iteratively through the rest of the nodes
    for(iter = p.begin(); iter != p.end(); ++iter) {
        if(prefix=="") {
            level++;
            PrintNodeTEX(out, (*iter), start_level, level, (*iter).name(), offset);
            level--;
        } else {
            level++;
            PrintNodeTEX(out, (*iter), start_level, level, prefix + "." + (*iter).name(), offset);
            level--;
        }
    }        

    // if this is the head node, print the footer
    if ( level == start_level )  out << boost::format(footer_format) % _section % head_name;
}

void PrintNodeHLP(std::ostream &out, Property &p, const int start_level, int level, string prefix,  string offset) {

     
    list<Property>::iterator iter;       
    string head_name;
    string _help("");
    string _unit("");
    string _default("");
    string fmt("t|%1%%|15t|%2%%|40t|%3%%|55t|%4%\n");
    int _offset = level;
    
    // if this is the head node, print the header
    if ( level == start_level ) {
            head_name = p.name();
            if ( p.hasAttribute("help") ) {
                if  ( p.hasAttribute("help") ) _help = p.getAttribute<string>("help");           
                out << boost::format(" %1%: %|18t| %2%\n") % head_name % _help;
            }
            _offset=0;
            out << boost::format("%|3" + fmt) % "OPTION" % "DEFAULT" % "UNIT" % "DESCRIPTION";
    } 
    
    if ( level > start_level ) {
            
            string ofmt;
            ofmt = "%|" + boost::lexical_cast<string>(_offset) + fmt;
            //cout << ofmt << " " << fmt << endl;
            
            if  ( p.hasAttribute("unit") ) _unit = p.getAttribute<string>("unit");
            if  ( p.hasAttribute("default") ) _default = p.getAttribute<string>("default") ;
            if  ( p.hasAttribute("help") ) _help = p.getAttribute<string>("help") ;
            if ( !_unit.empty() ) _unit = "[" + _unit + "]";
            if ( !_default.empty() ) _default = "(" + _default + ")";
            
            out << boost::format(ofmt)
                    % p.name() 
                    % _default
                    % _unit
                    % _help;
    }
    
    for(iter = p.begin(); iter != p.end(); ++iter) {
        if(prefix=="") {
            _offset = level + 2; level++;
            PrintNodeHLP(out, (*iter), start_level, level, (*iter).name(), offset);
            _offset = level - 2; level--;
        } else {
            _offset = level + 2; level++;
            PrintNodeHLP(out, (*iter), start_level, level, prefix + "." + (*iter).name(), offset);
            _offset = level - 2; level--;
        }
    }
}

std::ostream &operator<<(std::ostream &out, Property& p)
{
    if (!out.good())
        return out;

    std::ostream::sentry sentry(out);

    if(sentry)
    {
        // get the property format object attached to the stream
        PropertyIOManipulator *pm = 
                (PropertyIOManipulator*)out.pword(Property::getIOindex());
        
        string _indentation("");
        int _level = 0;
        PropertyIOManipulator::Type _type = PropertyIOManipulator::XML;
        if (pm) {
            _indentation = pm->getIndentation();
            _level = pm->getLevel();
            _type = pm->getType();
        }         
            
         switch( _type )
        {
        default:
            PrintNodeTXT(out, p, _level);
            case PropertyIOManipulator::XML:
            PrintNodeXML(out, p, _level, 0, "", _indentation);
            break;
        case PropertyIOManipulator::TXT:
            PrintNodeTXT(out, p, _level, 0, "", _indentation);
            break;
         case PropertyIOManipulator::TEX:            
            PrintNodeTEX(out, p, _level);
            break;
        case PropertyIOManipulator::HLP:            
            PrintNodeHLP(out, p, _level, 0, "", _indentation);
            break;
        }
        //out << endl;
    }

    return out;
};


}}
