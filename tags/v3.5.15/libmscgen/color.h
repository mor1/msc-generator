/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010,2011,2012,2013 Zoltan Turanyi
    Distributed under GNU Affero General Public License.

    Msc-generator is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Msc-generator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with Msc-generator.  If not, see <http://www.gnu.org/licenses/>.
*/

/** @file color.h The declaration of classes for colors.
 * @ingroup libmscgen_files */

#if !defined(COLOR_H)
#define COLOR_H

#include <map>
#include <string>
#include "error.h"

/** Describes a color with transparency. */
class ColorType
{
public:
    unsigned char r; ///<Red component
    unsigned char g; ///<Green component
    unsigned char b; ///<Blue component
    unsigned char a; ///<Alpha component governing transparency, 255 is opaque
    bool          valid; ///<True if the color is valid
    ColorType() : r(0), g(0), b(0), a(255), valid(false) {}
    ColorType(unsigned char R, unsigned char G, unsigned char B, unsigned char A=255) :
        r(R), g(G), b(B), a(A), valid(true) {}
    explicit ColorType(const std::string&);
    /** Converts a 32-bit unsigned value to a color.*/
    explicit ColorType(unsigned);
    /** Converts the color to a 32-bit value.*/
    unsigned ConvertToUnsigned() const {return ((unsigned(r))<<24) + ((unsigned(g))<<16) + ((unsigned(b))<<8) + (unsigned(a));}
    bool operator==(const ColorType &x) const
        {return (r==x.r && g==x.g && b==x.b && a==x.a && valid && x.valid) ||
                (!valid && !x.valid);}
    std::string Print(void) const; ///<Prints the color description of color into a string.
//    ColorType &operator +=(const ColorType &c);
    /** Mixes two colors.*/
    ColorType operator +(const ColorType &c) const; 
    /** Creates a lighter version with same transparency.
     * @param [in] p How much lighter we shall be. Zero means no change, 1 is completely white./
     * @returns The lighter version.*/
    ColorType Lighter(double p) const
        {return ColorType((unsigned char)(r+p*(255-r)), (unsigned char)(g+p*(255-g)), (unsigned char)(b+p*(255-b)), (unsigned char)a);}
    /** Creates a darker version with same transparency.
     * @param [in] p How much darker we shall be. Zero means no change, 1 is completely black./
     * @returns The lighter version.*/
    ColorType Darker(double p) const
        {return ColorType((unsigned char)(r*(1-p)), (unsigned char)(g*(1-p)), (unsigned char)(b*(1-p)), (unsigned char)a);}
    /** Creates a more transparent version.
     * @param [in] p How much more transparent we shall be. Zero means no change, 1 is completely transparent./
     * @returns The lighter version.*/
    ColorType MoreTransparent(double p) const
        {return ColorType(r, g, b, (unsigned char)(a*(1-p)));}
    /** Creates a less transparent version.
     * @param [in] p How much less transparent we shall be. Zero means no change, 1 is completely opaque./
     * @returns The lighter version.*/
    ColorType MoreOpaque(double p) const
        {return ColorType(r, g, b, (unsigned char)(a+p*(255-a)));}
    /** Creates a non-transparent version as if on white background*/
	ColorType FlattenAlpha(void) const
	    {return ColorType(unsigned(255-r)*(255-a)/255+r, unsigned(255-g)*(255-a)/255+g, unsigned(255-b)*(255-a)/255+b);}
};

/** A collection of named colors*/
class ColorSet : public std::map<std::string, ColorType>
{
public:
    bool AddColor(const std::string&, const std::string&, MscError &error, FileLineColRange linenum);
    ColorType GetColor(const std::string &s) const;
};


#endif //COLOR_H
