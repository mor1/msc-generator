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

/** @file attribute.h The declaration of basic classes for attribue handling, line, fill, shadows and notes.
 * @ingroup libmscgen_files */

#if !defined(ATTRIBUTE_H)
#define ATTRIBUTE_H

#include <string>
#include <sstream>
#include <list>
#include "color.h"
#include "csh.h"
#include "contour.h"

using contour::XY;
using contour::Range;
using contour::Block;
using contour::Edge;
using contour::Contour;

using contour::DoubleMap;
using contour::fmod_negative_safe;

using contour::EXPAND_MITER;
using contour::EXPAND_BEVEL;
using contour::EXPAND_ROUND;
using contour::EXPAND_MITER_BEVEL;
using contour::EXPAND_MITER_ROUND;

using std::string;

/** Helper class, holds two doubles.
 * Used to store left/right marging values.*/
class DoublePair : public std::pair<double, double> 
{
public:
    DoublePair() {};
    DoublePair(double a, double b) : std::pair<double, double>(a,b) {}
    void swap() {std::swap(first, second);}
};

/** Supply string with a good append ("<<") operator.*/
template <class T>
    string &operator <<(string&s, T t) {
        std::ostringstream ss;
        ss << t;
        s.append(ss.str());
        return s;
    };

/** Return length the common prefix in two strings (case insensitive). */
unsigned CaseInsensitiveCommonPrefixLen(const char *a, const char *b);
/** Checks if `b` is a prefix of `a`.
 * Returns 
 * - 0 if `a` does not begin with `b`
 * - 1 if a begins with `b`
 * - 2 if a == b
 * 
 * - Rule 1: the empty string ("") begins with a NULL pointer string, but is not equal to it
 * - Rule 2: every string begins with both the empty string ("") or a NULL pointer string*/
int CaseInsensitiveBeginsWith(const char *a, const char *b);
/** Checks if `b` is a prefix of `a`.
 * Returns 
 * - 0 if `a` does not begin with `b`
 * - 1 if a begins with `b`
 * - 2 if a == b
 * 
 * - Rule 1: the empty string ("") begins with a NULL pointer string, but is not equal to it
 * - Rule 2: every string begins with both the empty string ("") or a NULL pointer string*/
inline int CaseInsensitiveBeginsWith(const string &a, const char *b)
    {return CaseInsensitiveBeginsWith(a.c_str(), b);}
/** Checks if the two strings are identical (case insensitive)*/
inline bool CaseInsensitiveEqual(const char *a, const char *b)
    {return CaseInsensitiveBeginsWith(a,b)==2;}
/** Checks if the two strings are identical (case insensitive)*/
inline bool CaseInsensitiveEqual(const string &a, const char *b)
    {return CaseInsensitiveEqual(a.c_str(), b);}
/** Checks if the two strings are identical (case insensitive)*/
inline bool CaseInsensitiveEqual(const char *a, const string &b)
    {return CaseInsensitiveEqual(a, b.c_str());}
/** Checks if the two strings are identical (case insensitive)*/
inline bool CaseInsensitiveEqual(const string &a, const string &b)
    {return CaseInsensitiveEqual(a.c_str(), b.c_str());}

/** Checks how an attribute name ends (dot-sensitive).
 * Note that attribute names can be segmented with dots. This function
 * checks if one or more of the last segments are equal to `a`.
 * Thus 'line.width' ends with 'width' or 'line.width' but not with 'dth'.*/
bool CaseInsensitiveEndsWith(const char *base, const char *a);
/** Checks how an attribute name ends (dot-sensitive).
 * Note that attribute names can be segmented with dots. This function
 * checks if one or more of the last segments are equal to `a`.
 * Thus 'line.width' ends with 'width' or 'line.width' but not with 'dth'.*/
inline bool CaseInsensitiveEndsWith(const string &base, const char *a)
    {return CaseInsensitiveEndsWith(base.c_str(), a);}


///////////////////////////////////////////////////////////////////////

/** A list of object pointers.
 * Used as a container supporting polymorphic objects.
 * It can be created such that it destroys its objects at deletion 
 * or it does not. This is called whether the list is 
 * "responsible" for its objects or not. Not being responsible 
 * allows a second list of pointers to the same objects.*/
template <class Object>
class PtrList : public std::list<Object*>
{
public:
    const bool responsible; ///<Ture if we have to delete our objects in the destructor.
    using std::list<Object*>::begin;
    using std::list<Object*>::end;
    using std::list<Object*>::push_back;
    using std::list<Object*>::push_front;
    using std::list<Object*>::splice;
    using std::list<Object*>::clear;

    explicit PtrList(bool r=true) : std::list<Object*>(), responsible(r) {}
    /** Copy constructor. 
     * Does not work, if the list is responsible for its content and not empty*/
    PtrList(const PtrList<Object> &o) : std::list<Object*>(o), responsible(o.responsible) {_ASSERT(!responsible || o.size()==0);}
    PtrList(PtrList<Object> &&o) : std::list<Object*>(std::move(static_cast<std::list<Object*>&>(o))), responsible(o.responsible) {o.clear();}
    PtrList & operator = (const PtrList<Object> &o) {std::list<Object*>::operator=(o); const_cast<bool&>(responsible)=o.responsible; _ASSERT(!responsible); return *this;}
    PtrList & operator = (PtrList<Object> &&o) {std::list<Object*>::operator=(std::move(o)); const_cast<bool&>(responsible)=o.responsible; return *this;}
    PtrList *Append(Object *o)
        {if (o) push_back(o); return this;} ///<Append a pointer to the list
    PtrList *Append(PtrList<Object> *l)
		{if (l) {_ASSERT(responsible==l->responsible); splice(end(), *l);} return this;} ///<Append a list to us
    PtrList *Prepend(Object *o)
        {if (o) push_front(o); return this;} ///<Prepend a pointer to the list
    PtrList *Prepend(PtrList<Object> *l)
	    {if (l) {_ASSERT(responsible==l->responsible); splice(begin(), *l);} return this;} ///<Prepend a list to us
    void Empty() 
        {if (responsible) for (auto i=begin(); i!=end(); i++) delete(*i); clear();} ///<Empty the list, deleting objets if `responsible` is true.
	~PtrList() 
        {Empty();}
    string Print(int ident=0) const 
        {string s;
            for (auto i = begin(); i!=end(); i++) {
                if (i!=begin()) s.append("\n");
                s.append((*i)->Print(ident));
            }
            return s;} ///<Prints the list, each element in a new line. Works, if objects have Print() method.
};

//Enum helper functions////////////////////////////////////////////

/** A helper class encapsulating enumerations associated with string labels.
 * Used to translate between strings such as "solid" or "dotted" and
 * their respective enumeration value. 
 * One has to define the string labels for each enum type as a 
 * specialization of the "names" member. There the first value (index of zero)
 * is assumed to be the invalid enumeration (not matched or used), and the array
 * shall end with an empty string.*/
template <typename Enum>
class EnumEncapsulator {
public:
    Enum value;
    const static char names[][ENUM_STRING_LEN];
    EnumEncapsulator(Enum a) : value(a) {}
};

/** Converts an enum value to its string label*/
template <typename Enum>
string PrintEnum(Enum e) {
    EnumEncapsulator<Enum> a(e);
    return string(a.names[a.value]);
};

/** Converts a string label to its enumerator value (case insensitive).
 * @param [in] name The sting label to convert.
 * @param [out] a The resulting value.
 * @returns False if `name` did not match any label associated with the type of `a`.*/
template <typename Enum>
bool Convert(const std::string &name, Enum &a) 
{
    const EnumEncapsulator<Enum> dummy(a);
    for (unsigned i=1; EnumEncapsulator<Enum>::names[i][0]; i++)
        if (CaseInsensitiveEqual(name, EnumEncapsulator<Enum>::names[i])) {
            a = Enum(i);
            return true;
        }
    return false;
};

/** Returns a slash ('/') separated list of possible values for the type of `dummy1`.*/
template <typename Enum>
string CandidatesFor(Enum dummy1)
{
    const EnumEncapsulator<Enum> dummy2(dummy1);
    string s(EnumEncapsulator<Enum>::names[1]);
    for (int i=2; EnumEncapsulator<Enum>::names[i][0]; i++){
        s.append("/");
        s.append(EnumEncapsulator<Enum>::names[i]);
    }
    return s;
};


////////////////////////////////////////////////////////////

/** Describes the type of the value the attribute was assigned.*/
enum EAttrType {
    MSC_ATTR_CLEAR = 0,  ///<The attribute was assigned no value, like "line.width="
    MSC_ATTR_STRING,     ///<The attribute was assigned a general string
    MSC_ATTR_BOOL,       ///<The attribute was assigned "yes" or "no"
    MSC_ATTR_NUMBER,     ///<The attribute was assigned a (floating-point) number
    MSC_ATTR_STYLE       ///<The construct was not an attribute, but a style specification (no "=" sign, like "strong")
};

/** Describes the context of the use of a style or attribute.*/
enum EStyleType {
    STYLE_STYLE,   ///<The attribute is to be applied to a style definition or the style is a user defined style
    STYLE_DEFAULT, ///<The attribute is to be applied to a default style or the style is a default stlye such as "box" or "arrow" and MUST be complete.
    STYLE_DEF_ADD, ///<The style is an additional default style, such as "->" and does not have to be complete.
    STYLE_ARC,     ///<The attribute is to be applied to an arc or the style object is part of an arc describing it
    STYLE_OPTION   ///<The attribute is indeed a chart option
};

/** Stores a "<name>=<value>" construct during parsing.
 *
 * This can either be an arc attribute, an entity attribute or a chart option.
 * When parsing attribute lists, we create list of these attributes and
 * pass them to the AddAttributeList() function of ArcBase descendants, EntityApp
 * and Msc (for chart options). In this function, the actual Attribute objects
 * are consumed and deleted*/
class Attribute
{
public:
    EAttrType        type;           ///<The type of value the attribute was assigned
    string           name;           ///<The name of the attribute: the string before the "=" sign
    string           value;          ///<The string representation of the value of the attribute: the string after the "=" sign. Set even if the type is not MSC_ATTR_STRING.
    double           number;         ///<If the type is MSC_ATTR_NUMBER, this contains the value
    bool             yes;            ///<If the type is MSC_ATTR_BOOL, this contains the value
    FileLineColRange linenum_attr;   ///<The location of the attribute name in the source file
    FileLineColRange linenum_value;  ///<The location of the attribute value in the source file
    mutable bool     error;          ///<True if the attribute specification had an error.XXX

    /** Creates a string attribute or one with no value (MSC_ATTR_CLEAR) */
    Attribute(const char*a, const char *s, FileLineColRange l, FileLineColRange v) :
        type(s?MSC_ATTR_STRING:MSC_ATTR_CLEAR), name(a), value(s?s:""),
        linenum_attr(l), linenum_value(v), error(false) {}
    /** Creates a string attribute or one with no value (MSC_ATTR_CLEAR) */
    Attribute(const char*a, const string &s, FileLineColRange l, FileLineColRange v) :
        type(MSC_ATTR_STRING), name(a), value(s),
        linenum_attr(l), linenum_value(v), error(false) {}
    /** Creates an attribute with a number as value */
    Attribute(const char*a, double n, FileLineColRange l, FileLineColRange v, const char *s) :
        type(MSC_ATTR_NUMBER), name(a), value(s), number(n),
        linenum_attr(l), linenum_value(v), error(false)  {}
    /** Creates an attribute with a bool as value */
    Attribute(const char*a, bool b, FileLineColRange l, FileLineColRange v, const char *s) :
        type(MSC_ATTR_BOOL), name(a), value(s), yes(b),
        linenum_attr(l), linenum_value(v), error(false) {}
    /** Creates a style specification */
    Attribute(const char*a, FileLineColRange l) :
        type(MSC_ATTR_STYLE), name(a),
        linenum_attr(l), linenum_value(l), error(false) {}

    string Print(int ident=0) const;
    bool Is(const char *a) const
        {return CaseInsensitiveEqual(a, name);}  ///<Returns true if the name of the attribute equals `a` (case insensitive)
    bool EndsWith(const char *a) const;
    bool StartsWith(const char *a) const;
    bool CheckType(EAttrType t, MscError &error) const;
    bool CheckColor(const ColorSet &colors, MscError &error) const;
    void InvalidValueError(const string &candidates, MscError &error) const;
    void InvalidAttrError(MscError &error) const;
    void InvalidStyleError(MscError &error) const;
    bool EnsureNotClear(MscError &error, EStyleType type) const;
};

/** A list of Attribute object pointers*/
typedef PtrList<Attribute> AttributeList;

class Msc;

////////////////////////////////////////////////////////////

/** Describes the type of a line.*/
enum ELineType {
    LINE_INVALID = 0,  ///<Invalid value
    LINE_NONE,         ///<No line shall be drawn
    LINE_SOLID,        ///<Continuous, single line
    LINE_DOTTED,       ///<Dotted, single line
    LINE_DASHED,       ///<Dashed, single line
    LINE_LONG_DASHED,  ///<Single line with long dashes
    LINE_DASH_DOT,     ///<Dash-dot single line
    LINE_DOUBLE,       ///<Continuous double line
    LINE_TRIPLE,       ///<Continuous triple line
    LINE_TRIPLE_THICK  ///<Continuous triple line, with the middle line being thicker
};

/** True if the line type is continuous (single, double or triple).*/
inline bool IsLineTypeContinuous(ELineType t) {return t!=LINE_DOTTED && t!=LINE_DASHED && t!=LINE_LONG_DASHED && t!=LINE_DASH_DOT;}
/** True if the line is of double type*/
inline bool IsLineTypeDouble(ELineType t) {return t==LINE_DOUBLE;}
/** True if the line is of triple type*/
inline bool IsLineTypeTriple(ELineType t) {return t==LINE_TRIPLE || t==LINE_TRIPLE_THICK;}
/** True if the line is either of double or triple type*/
inline bool IsLineTypeDoubleOrTriple(ELineType t) {return IsLineTypeDouble(t) || IsLineTypeTriple(t);}
/** Returns how thick is the line if line width is 1. E.g., a double line is 3x thicker, two for the two lines and one for the space in between.*/
inline double LineWidthMultiplier(ELineType t) {return t==LINE_NONE ? 0 : t==LINE_DOUBLE ? 3 : t==LINE_TRIPLE ? 5 : t==LINE_TRIPLE_THICK ? 6 : 1;}

/** Describes the possible rectangle corner types*/
enum ECornerType {
    CORNER_INVALID = 0, ///<The invalid value
    CORNER_NONE,        ///<The basic rectangular corner type
    CORNER_ROUND,       ///<The corner is rounded
    CORNER_BEVEL,       ///<The corner is chopped 
    CORNER_NOTE         ///<Only the right-upper corner is special: a small note flip
};

/** Returns by how much the corner radius is to be increased if the rectangle is expanded by 1 pixel.*/
inline double RadiusIncMultiplier(ECornerType t) {return t==CORNER_ROUND ? 1 : t==CORNER_BEVEL || t==CORNER_NOTE ? tan(22.5*M_PI/180) : 0;}

/** Stores the properties of a line (including rectangle corner style).*/
class LineAttr {
protected:
    Contour CreateRectangle_ForFill_Note(double x1, double x2, double y1, double y2) const;  
    Contour CreateRectangle_InnerEdge_Note(double x1, double x2, double y1, double y2) const;  
public:
    std::pair<bool, ELineType> type;   ///<The style of the line. Not set if `first` is false.
    std::pair<bool, ColorType> color;  ///<The color of the line. Not set if `first` is false.
    std::pair<bool, double>    width;  ///<The width of the line. Not set if `first` is false.
    ///<The radius of the corners. Not set if `first` is false. 
    ///<Ignored if `corner` is CORNER_NONE. It is also used for pipes to set the size of the oval
    ///<and also for ArcSelfArrow to set the radius of the arrow.
    std::pair<bool, double>      radius; 
    std::pair<bool, ECornerType> corner; ///<The style of the corners. Not set if `first` is false. 
    LineAttr();   
    LineAttr(ELineType t)  {Empty(); type.first = true;  type.second = t;}  ///<Creates an empty style, where only the type is set to `t`.
    LineAttr(ColorType c) {Empty(); color.first = true; color.second = c;} ///<Creates an empty style, where only the color is set to `c`.
    LineAttr(ELineType t, ColorType c, double w, ECornerType ct, double r) :
        type(true, t), color(true, c), width(true, w), radius(true, r), corner(true, ct) {} ///<Creates a fully specified line style with the attributes given.
    void Empty() {type.first = color.first = width.first = corner.first = radius.first = false;} ///<Clear all content from the line style.
    bool IsEmpty() const {return !type.first && !color.first && !width.first && !corner.first && !radius.first;} ///<False if any of the line attributes are set.
    bool IsComplete() const {return type.first && color.first && width.first && corner.first && radius.first;} ///<True if all of the line attributes are set.
    void MakeComplete();
    LineAttr &operator +=(const LineAttr&a); ///<Applies `a` to us: sets all our attributes, which are set in `a` to the value in `a`; leaves the rest unchanged.
    bool operator == (const LineAttr &a);

    //functions about line style
    bool IsContinuous() const {_ASSERT(type.first); return IsLineTypeContinuous(type.second);} ///<True if the line style is continuous.
    bool IsDouble() const {_ASSERT(type.first); return IsLineTypeDouble(type.second);} ///<True if the line style is double.
    bool IsTriple() const {_ASSERT(type.first); return IsLineTypeTriple(type.second);} ///<True if the line style is triple.
    bool IsDoubleOrTriple() const {return IsDouble() || IsTriple();} ///<True if the line style is double or triple.
    /** Returns how much the midline of the outer lines are spaced from the midline of the total line.
     * For single lines return 0. For doubles it returns the width. For triple it returns 2x the width, for triple thick, it returns 2.5 x the width.*/
    double Spacing() const {_ASSERT(width.first); return width.second * (IsDouble() ? 1 : IsTriple() ? (type.second == LINE_TRIPLE_THICK)?2.5:2.0 : 0);}
    /** Assuming a triple line, how wide is the middle line.*/
    double TripleMiddleWidth() const {_ASSERT(IsTriple()&&width.first); return width.second*((type.second == LINE_TRIPLE_THICK)?2.0:1.0);}
    /** Returns the total line width.*/
    double LineWidth() const {_ASSERT(type.first && width.first); return width.second * LineWidthMultiplier(type.second);}
    //double RadiusIncMul() const {_ASSERT(type.first); return ::RadiusIncMultiplier(corner.second);}
    /** Updates the `radius` if a corresponding rectangle is expanded by `gap`.*/
    LineAttr& Expand(double gap) {_ASSERT(IsComplete()); if (radius.second>0 && corner.second != CORNER_NONE) radius.second = std::max(0., radius.second + gap*::RadiusIncMultiplier(corner.second)); return *this;}
    /** If given the midline of a box by (x1,x2,y1,y2), what is the maximum radius for our line width and style.*/
    double MaxRadius(double x1, double x2, double y1, double y2) const {return std::max(0., std::min(fabs(x2-x1), fabs(y2-y1)-LineWidth())/2);}
    /** If given the midline of a box by (x1,x2,y1,y2), sanitize our `radius`.*/
    double SaneRadius(double x1, double x2, double y1, double y2) const {return std::max(0., std::min(radius.second, MaxRadius(x1, x2, y1, y2)));}
    /** If given the midline of a box by `b`, sanitize our `radius`.*/
    double SaneRadius(const Block &b) const {return SaneRadius(b.x.from, b.x.till, b.y.from, b.y.till);}
    /** Return the dash pattern to be used for this line style. `num` returns the length of the pattern.*/
    const double * DashPattern(unsigned &num) const;

    virtual bool AddAttribute(const Attribute &a, Msc *msc, EStyleType t);
    static void AttributeNames(Csh &csh, const string &prefix);
    static bool AttributeValues(const std::string &attr, Csh &csh);
    string Print(int ident = 0) const;

    /** @name Rectangle shape creators.
     * The below 12 functions creates various rectangle contours using the line style
     * corner style, line width and radius of us. Four functions, each having three variants in how
     * the input rectangle is specified (four coordinates, two XY structs or a Block). 
     * In each case the input rectangle specifies the midline of the final rectangle, that is
     * the middle of the line around the rectangle. The line then lies halfway outside and
     * halfway inside around this contour. Similar, the radius attribute of us is understood
     * as the radius at the midline (the outer edge will have a somewhat larger radius, whereas
     * the inner edge a somewhat smaller one).*/
    /** @{ */

    /** Creates the midline of the rectange according to corner style. 
     * This one does not assume anything about wether the resulting rectange should be the
     * outer edge or inner edge of the line - just uses the radius value and coordinates
     * as they are. Here we simply add round and bevel corners if applicable.
     * For CORNER_NOTE it creates the outer line only.
     * You can specify a radius different from the one stored in us. Leaving that out
     * defaults to the radius stored in `this`*/ 
    Contour CreateRectangle_Midline(double x1, double x2, double y1, double y2, double r = -1) const;  
    /** Creates the midline of the rectange according to corner style. 
     * This one does not assume anything about wether the resulting rectange should be the
     * outer edge or inner edge of the line - just uses the radius value and coordinates
     * as they are. Here we simply add round and bevel corners if applicable.
     * For CORNER_NOTE it creates the outer line only.*/ 
    Contour CreateRectangle_Midline(const XY &s, const XY &d) const {return CreateRectangle_Midline(s.x, d.x, s.y, d.y);}
    /** Creates the midline of the rectange according to corner style. 
     * This one does not assume anything about wether the resulting rectange should be the
     * outer edge or inner edge of the line - just uses the radius value and coordinates
     * as they are. Here we simply add round and bevel corners if applicable.
     * For CORNER_NOTE it creates the outer line only.*/ 
    Contour CreateRectangle_Midline(const Block &b) const {return CreateRectangle_Midline(b.x.from, b.x.till, b.y.from, b.y.till);}
    
    /** Creates a contour representing the outer edge of the rectangle.
     * This one considers linewidth and returns the outer edge of the line.
     * For CORNER_NOTE it creates the outer line only.*/
    Contour CreateRectangle_OuterEdge(double x1, double x2, double y1, double y2) const {const double lw2 = LineWidth()/2; return CreateRectangle_Midline(x1-lw2, x2+lw2, y1-lw2, y2+lw2, radius.second+lw2*::RadiusIncMultiplier(corner.second));}
    /** Creates a contour representing the outer edge of the rectangle.
     * This one considers linewidth and returns the outer edge of the line.
     * For CORNER_NOTE it creates the outer line only.*/
    Contour CreateRectangle_OuterEdge(const XY &s, const XY &d) const {const double lw2 = LineWidth()/2; return CreateRectangle_Midline(s.x-lw2, d.x+lw2, s.y-lw2, d.y+lw2, radius.second+lw2*::RadiusIncMultiplier(corner.second));}
    /** Creates a contour representing the outer edge of the rectangle.
     * This one considers linewidth and returns the outer edge of the line.
     * For CORNER_NOTE it creates the outer line only.*/
    Contour CreateRectangle_OuterEdge(const Block &b) const {const double lw2 = LineWidth()/2; return CreateRectangle_Midline(b.x.from-lw2, b.x.till+lw2, b.y.from-lw2, b.y.till+lw2, radius.second+lw2*::RadiusIncMultiplier(corner.second));}
    
    /** Returns the area to fill for a rectangle.
     * This one considers linewidth and returns the middle of the inner line (for double or triple)
     * (or the middle of the line for single).
     * For CORNER_NOTE it creates the inner line and the triangle
     * (ret[0] will be the body and ret[1] will be the triangle).*/
    Contour CreateRectangle_ForFill(double x1, double x2, double y1, double y2) const;  
    /** Returns the area to fill for a rectangle.
     * This one considers linewidth and returns the middle of the inner line (for double or triple)
     * (or the middle of the line for single).
     * For CORNER_NOTE it creates the inner line and the triangle
     * (ret[0] will be the body and ret[1] will be the triangle).*/
    Contour CreateRectangle_ForFill(const XY &s, const XY &d) const {return CreateRectangle_ForFill(s.x, d.x, s.y, d.y);}
    /** Returns the area to fill for a rectangle.
     * This one considers linewidth and returns the middle of the inner line (for double or triple)
     * (or the middle of the line for single).
     * For CORNER_NOTE it creates the inner line and the triangle
     * (ret[0] will be the body and ret[1] will be the triangle).*/
    Contour CreateRectangle_ForFill(const Block &b) const {return CreateRectangle_ForFill(b.x.from, b.x.till, b.y.from, b.y.till);}
    
    /** Returns the inner edge of the line around a rectangle.
     * This one considers linewidth and returns the inner edge of the inner line 
     * For CORNER_NOTE it creates the inner line only.*/
    Contour CreateRectangle_InnerEdge(double x1, double x2, double y1, double y2) const;  
    /** Returns the inner edge of the line around a rectangle.
     * This one considers linewidth and returns the inner edge of the inner line 
     * For CORNER_NOTE it creates the inner line only.*/
    Contour CreateRectangle_InnerEdge(const XY &s, const XY &d) const {return CreateRectangle_InnerEdge(s.x, d.x, s.y, d.y);}
    /** Returns the inner edge of the line around a rectangle.
     * This one considers linewidth and returns the inner edge of the inner line 
     * For CORNER_NOTE it creates the inner line only.*/
    Contour CreateRectangle_InnerEdge(const Block &b) const {return CreateRectangle_InnerEdge(b.x.from, b.x.till, b.y.from, b.y.till);}
    /** @} */

    DoublePair CalculateTextMargin(Contour textCover, double rect_top) const; 
};


inline Contour LineAttr::CreateRectangle_ForFill(double x1, double x2, double y1, double y2) const
{
    if (!IsDoubleOrTriple()) 
        return CreateRectangle_Midline(x1, x2, y1, y2);
    if (corner.second != CORNER_NOTE) { //We have double or triple line here
        const double s = Spacing();
        return LineAttr(*this).Expand(-s).CreateRectangle_Midline(x1+s, x2-s, y1+s, y2-s);
    }
    return CreateRectangle_ForFill_Note(x1, x2, y1, y2);    
}

inline Contour LineAttr::CreateRectangle_InnerEdge(double x1, double x2, double y1, double y2) const
{
    if (corner.second == CORNER_NOTE) 
        return CreateRectangle_InnerEdge_Note(x1, x2, y1, y2);    
    const double lw2 = LineWidth()/2;
    return CreateRectangle_Midline(x1+lw2, x2-lw2, y1+lw2, y2-lw2);
}

/** Describes gradient fill direction*/
enum EGradientType {
    GRADIENT_INVALID = 0,  ///<The invalid value
    GRADIENT_NONE, ///<No gradient in fill - single color only
    GRADIENT_OUT,  ///<Radial gradient outwards from center
    GRADIENT_IN,   ///<Radial gradient inwards toward center
    GRADIENT_DOWN, ///<Linear gradient downwards
    GRADIENT_UP,   ///<Linear gradient upwards
    GRADIENT_LEFT, ///<Linear gradient leftwards
    GRADIENT_RIGHT,///<Linear gradient rightwards
    GRADIENT_BUTTON///<Linear gradient with multiple changes creating a button effect 
};

/** Stores the properties of a fill (color and gradient).*/
struct FillAttr {
public:
    std::pair<bool, ColorType> color;        ///<The color of the fill. Not set if `first` is false.
    std::pair<bool, ColorType> color2;       ///<The secondary color of the fill if we use a gradient. Not set if `first` is false.
    std::pair<bool, EGradientType> gradient;  ///<The gradient of the fill. Not set if `first` is false.
    FillAttr();
    FillAttr(ColorType c) {Empty(); color.first = true; color.second = c;} ///<Creates an empty style, where only the color is set to `c`.
    FillAttr(ColorType c, EGradientType g) :
        color(true, c), gradient(true,g) {} ///<Creates a fully specified fill style with the attributes given.
    FillAttr(ColorType c, ColorType c2) : 
        color(true, c), color2(true, c2), gradient(false, GRADIENT_INVALID) {} ///<Creates an incomplete fill style with two colors given.
    FillAttr(ColorType c, ColorType c2, EGradientType g) :
        color(true, c), color2(true, c2), gradient(true,g) {} ///<Creates a fully specified fill style with the attributes given.
    void Empty() {color.first = color2.first = gradient.first = false;} ///<Clear all content from the fill style.
    void MakeComplete();
    bool IsEmpty() const {return !color.first && !color2.first && !gradient.first;} ///<False if any of the line attributes are set. 
    bool IsComplete() const {return color.first && gradient.first;} ///<True if all of the line attributes are set. (But color2 is not needed.)
    FillAttr &operator +=(const FillAttr&a); ///<Applies `a` to us: sets all our attributes, which are set in `a` to the value in `a`; leaves the rest unchanged.
    bool operator == (const FillAttr &a) const;
    virtual bool AddAttribute(const Attribute &a, Msc *msc, EStyleType t);
    static void AttributeNames(Csh &csh, const string &prefix);
    static bool AttributeValues(const std::string &attr, Csh &csh);
    string Print(int ident = 0) const;
};

/** Stores the properties of shadows (color, offset and blur depth).*/
struct ShadowAttr {
public:
    std::pair<bool, ColorType> color; ///<The color of the shadow at its darkest - can be somewhat transparent. Not set if `first` is false.
	std::pair<bool, double> offset;      ///<The offset of the shadow from the object, indicating how much the object is above the surface. Not set if `first` is false.
	std::pair<bool, double> blur;        ///<Indicating how many pixels are blurred at the edge of the shadow. Not set if `first` is false.
    ShadowAttr();
    ShadowAttr(ColorType c) {Empty(); color.first = true; color.second = c;} ///<Creates an empty style, where only the color is set to `c`.
    void Empty() {color.first = offset.first = blur.first=false;} ///<Clear all content from the shadow style.
    void MakeComplete();
    bool IsComplete() const {return color.first && offset.first && blur.first;} ///<True if all of the shadow attributes are set. 
    ShadowAttr &operator +=(const ShadowAttr&a); ///<Applies `a` to us: sets all our attributes, which are set in `a` to the value in `a`; leaves the rest unchanged.
    bool operator == (const ShadowAttr &a);
    virtual bool AddAttribute(const Attribute &a, Msc *msc, EStyleType t);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string &attr, Csh &csh);
    string Print(int ident = 0) const;
};

bool CshHintGraphicCallbackForYesNo(Canvas *canvas, CshHintGraphicParam p);

/** Stores the properties of notes (pointer type and position; width).*/
struct NoteAttr {
public:
    enum EPointerType {POINTER_INVALID=0, NONE, CALLOUT, ARROW, BLOCKARROW};
    enum EPosType {POS_INVALID=0, POS_NEAR, POS_FAR, LEFT, RIGHT, UP, DOWN, LEFT_UP, LEFT_DOWN, RIGHT_UP, RIGHT_DOWN};
	std::pair<bool, EPointerType> pointer;
    std::pair<bool, int> def_float_dist;
    std::pair<bool, int> def_float_x;
    std::pair<bool, int> def_float_y;
    struct {bool first; double second; std::string str;} width; ///<The width specified by the used. second<0 means the string contains the width
    NoteAttr() {Empty(); MakeComplete();} ///<Create a fully specified note style with default values (Callout type, no specific position preference.)
    void Empty() {pointer.first = def_float_dist.first = def_float_x.first = def_float_y.first = width.first = false;} ///<Clear all content from the note style.
    void MakeComplete();
    bool IsComplete() const {return pointer.first && def_float_dist.first && def_float_x.first && def_float_y.first;} ///<True if all of the note attributes are set. 
    NoteAttr &operator +=(const NoteAttr&a); ///<Applies `a` to us: sets all our attributes, which are set in `a` to the value in `a`; leaves the rest unchanged.
    bool operator == (const NoteAttr &a);
    virtual bool AddAttribute(const Attribute &a, Msc *msc, EStyleType t);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string &attr, Csh &csh);
    string Print(int ident = 0) const;
    static bool CshHintGraphicCallbackForLayout(Canvas *canvas, CshHintGraphicParam p);
    static bool CshHintGraphicCallbackForPointer(Canvas *canvas, CshHintGraphicParam p);
    static bool CshHintGraphicCallbackForPos(Canvas *canvas, CshHintGraphicParam p);
};



#endif
