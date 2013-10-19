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


#ifndef __VOTCA_TOOLS_COLORS_H
#define	__VOTCA_TOOLS_COLORS_H

namespace votca { namespace tools {

/**
    \brief stores color codes for colorful help output    
 * 
 * "\x1b[%dm"
 * 
 * 0: reset colors/style
 * 1: bold
 * 4: underline
 * 30 - 37: black, red, green, yellow, blue, magenta, cyan, and white text
 * 40 - 47: black, red, green, yellow, blue, magenta, cyan, and white background
 * A, B, C, D - moves cursor one line above, under, right, left
 * 
 * example: "\x1b[1;35;42m"
 */

namespace Colors 
{
    static const char Empty[]       = {0};
    
    static const char Reset[]       = { 0x1b, '[', '0', ';', '3', '9', 'm', 0 };

    static const char Black[]       = { 0x1b, '[', '0', ';', '3', '0', 'm', 0 };
    static const char Red[]         = { 0x1b, '[', '0', ';', '3', '1', 'm', 0 };
    static const char Green[]       = { 0x1b, '[', '0', ';', '3', '2', 'm', 0 };
    static const char Yellow[]      = { 0x1b, '[', '0', ';', '3', '3', 'm', 0 };
    static const char Blue[]        = { 0x1b, '[', '0', ';', '3', '4', 'm', 0 };
    static const char Magenta[]     = { 0x1b, '[', '0', ';', '3', '5', 'm', 0 };
    static const char Cyan[]        = { 0x1b, '[', '0', ';', '3', '6', 'm', 0 };
    static const char White[]       = { 0x1b, '[', '0', ';', '3', '7', 'm', 0 };

    static const char BoldBlack[]   = { 0x1b, '[', '1', ';', '3', '0', 'm', 0 };
    static const char BoldRed[]     = { 0x1b, '[', '1', ';', '3', '1', 'm', 0 };
    static const char BoldGreen[]   = { 0x1b, '[', '1', ';', '3', '2', 'm', 0 };
    static const char BoldYellow[]  = { 0x1b, '[', '1', ';', '3', '3', 'm', 0 };
    static const char BoldBlue[]    = { 0x1b, '[', '1', ';', '3', '4', 'm', 0 };
    static const char BoldMagenta[] = { 0x1b, '[', '1', ';', '3', '5', 'm', 0 };
    static const char BoldCyan[]    = { 0x1b, '[', '1', ';', '3', '6', 'm', 0 };
    static const char BoldWhite[]   = { 0x1b, '[', '1', ';', '3', '7', 'm', 0 };

 

};

class ColorScheme
{
public:
    virtual const char *_reset()   const = 0;
    virtual const char *_black()   const = 0;
    virtual const char *_red()     const = 0;
    virtual const char *_green()   const = 0;
    virtual const char *_yellow()  const = 0;
    virtual const char *_blue()    const = 0;
    virtual const char *_magenta() const = 0;
    virtual const char *_cyan()    const = 0;
    virtual const char *_white()   const = 0;
};

class ColorSchemeBase 
{
public:
    virtual const char *Reset() const = 0;
    virtual const char *Black() const = 0;
    virtual const char *Red() const = 0;
    virtual const char *Green() const = 0;
    virtual const char *Yellow() const = 0;
    virtual const char *Blue() const = 0;
    virtual const char *Magenta() const = 0;
    virtual const char *Cyan() const = 0;
    virtual const char *White() const = 0;
};

template <typename TColorScheme>
class Color : public ColorSchemeBase
{    
    TColorScheme _cs;   
public:
    const char *Reset() const { return _cs._reset(); }
    const char *Black() const { return _cs._black(); }
    const char *Red() const { return _cs._red(); }
    const char *Green() const { return _cs._green(); }
    const char *Yellow() const { return _cs._yellow(); }
    const char *Blue() const { return _cs._blue(); }
    const char *Magenta() const { return _cs._magenta(); }
    const char *Cyan() const { return _cs._cyan(); }
    const char *White() const { return _cs._white(); } 

};

class csDefault : public ColorScheme
{
public:
    const char *_reset()   const  {return Colors::Empty;}
    const char *_black()   const  {return Colors::Empty;}
    const char *_red()     const  {return Colors::Empty;}
    const char *_green()   const  {return Colors::Empty;}
    const char *_yellow()  const  {return Colors::Empty;}
    const char *_blue()    const  {return Colors::Empty;}
    const char *_magenta() const  {return Colors::Empty;}
    const char *_cyan()    const  {return Colors::Empty;}
    const char *_white()   const  {return Colors::Empty;}
};

class csRGB  : public ColorScheme
{
public:
    const char *_reset()   const  {return Colors::Reset;}
    const char *_black()   const  {return Colors::Black;}
    const char *_red()     const  {return Colors::Red;}
    const char *_green()   const  {return Colors::Green;}
    const char *_yellow()  const  {return Colors::Yellow;}
    const char *_blue()    const  {return Colors::Blue;}
    const char *_magenta() const  {return Colors::Magenta;}
    const char *_cyan()    const  {return Colors::Cyan;}
    const char *_white()   const  {return Colors::White;}
};

extern Color<csDefault> DEFAULT_COLORS;

}}

#endif	/* __VOTCA_TOOLS_COLORS_H */

