#if !defined(ATTRIBUTE_H)
#define ATTRIBUTE_H

#include <string>
#include <sstream>
#include <list>
#include "color.h"
#include "csh.h"
#include "contour_area.h"

using std::string;

class DoublePair : public std::pair<double, double> 
{
public:
    DoublePair() {};
    DoublePair(double a, double b) : std::pair<double, double>(a,b) {}
    void swap() {std::swap(first, second);}
};

// Needed to supply string with a good "<<" operator
template <class T>
    string &operator <<(string&s, T t) {
        std::ostringstream ss;
        ss << t;
        s.append(ss.str());
        return s;
    };

unsigned CaseInsensitiveCommonPrefixLen(const char *a, const char *b);
int CaseInsensitiveBeginsWith(const char *a, const char *b);
inline int CaseInsensitiveBeginsWith(const string &a, const char *b)
    {return CaseInsensitiveBeginsWith(a.c_str(), b);}
inline bool CaseInsensitiveEqual(const char *a, const char *b)
    {return CaseInsensitiveBeginsWith(a,b)==2;}
inline bool CaseInsensitiveEqual(const string &a, const char *b)
    {return CaseInsensitiveEqual(a.c_str(), b);}
inline bool CaseInsensitiveEqual(const char *a, const string &b)
    {return CaseInsensitiveEqual(a, b.c_str());}
inline bool CaseInsensitiveEqual(const string &a, const string &b)
    {return CaseInsensitiveEqual(a.c_str(), b.c_str());}

//this one returns true if ending is terminated at a dot "."
bool CaseInsensitiveEndsWith(const char *base, const char *a);
inline bool CaseInsensitiveEndsWith(const string &base, const char *a)
{return CaseInsensitiveEndsWith(base.c_str(), a);}


///////////////////////////////////////////////////////////////////////

template <class Object>
class PtrList : public std::list<Object*>
{
    bool    responsible;
    public:
        explicit PtrList(bool r=true) : std::list<Object*>()
            {responsible=r;}
        PtrList *Append(Object *o)
            {if (o) push_back(o); return this;}
        PtrList *Append(PtrList<Object> *l)
		    {if (l) {_ASSERT(responsible==l->responsible); splice(std::list<Object*>::end(), *l);} return this;}
        PtrList *Prepend(Object *o)
            {if (o) push_front(o); return this;}
        PtrList *Prepend(PtrList<Object> *l)
	        {if (l) {_ASSERT(responsible==l->responsible); splice(std::list<Object*>::begin(), *l);} return this;}        
		~PtrList()
            {typename std::list<Object*>::iterator i=std::list<Object*>::begin();
             if (responsible)
                 while (i!=std::list<Object*>::end())
                    delete *(i++);}
        string Print(int ident=0) const
            {typename std::list<Object*>::const_iterator i = std::list<Object*>::begin();
             string s;;
             while (i!=std::list<Object*>::end()) {
                s.append(((*(i++))->Print(ident)));
                if (i!=std::list<Object*>::end())
                    s.append("\n");
             }
             return s;}
};

//Enum helper functions////////////////////////////////////////////

template <typename Enum>
class EnumEncapsulator {
public:
    Enum value;
    const static char names[][ENUM_STRING_LEN];
    EnumEncapsulator(Enum a) : value(a) {}
};

template <typename Enum>
string PrintEnum(Enum e) {
    EnumEncapsulator<Enum> a(e);
    return string(a.names[a.value]);
};

template <typename Enum>
bool Convert(string name, Enum &a) {
    unsigned i;
    EnumEncapsulator<Enum> dummy(a);
    for(i=0; i<name.length(); i++)
        name[i] = char(tolower(name[i]));
    for (i=1; EnumEncapsulator<Enum>::names[i][0]; i++)
        if (name == EnumEncapsulator<Enum>::names[i]){
            a = Enum(i);
            return true;
        }
    return false;
};

template <typename Enum>
string CandidatesFor(Enum dummy1)
{
    EnumEncapsulator<Enum> dummy2(dummy1);
    string s(EnumEncapsulator<Enum>::names[1]);
    for (int i=2; EnumEncapsulator<Enum>::names[i][0]; i++){
        s.append("/");
        s.append(EnumEncapsulator<Enum>::names[i]);
    }
    return s;
};

class const_char_vector_t : public std::set<string>
{
public:
    void Add(const string &a) {insert(a);}
    void Add(const char names[][ENUM_STRING_LEN], const string &prefix);
};

////////////////////////////////////////////////////////////
typedef enum
{
    MSC_ATTR_CLEAR = 0,
    MSC_ATTR_STRING,
    MSC_ATTR_BOOL,
    MSC_ATTR_NUMBER,
    MSC_ATTR_STYLE
}
MscAttrType;

typedef enum
{
    STYLE_STYLE,
    STYLE_DEFAULT,
    STYLE_ARC,
    STYLE_OPTION
} StyleType;

class Attribute
{
    public:
        MscAttrType     type;
        string          name;
        string          value;
        double          number;
        bool            yes;
        file_line_range linenum_attr;
        file_line_range linenum_value;
        mutable bool    error;

        Attribute(const char*a, const char *s, file_line_range l, file_line_range v) :
            type(s?MSC_ATTR_STRING:MSC_ATTR_CLEAR), name(a), value(s?s:""),
            linenum_attr(l), linenum_value(v), error(false) {}
        Attribute(const char*a, double n, file_line_range l, file_line_range v, const char *s) :
            type(MSC_ATTR_NUMBER), name(a), value(s), number(n),
            linenum_attr(l), linenum_value(v), error(false)  {}
        Attribute(const char*a, bool b, file_line_range l, file_line_range v, const char *s) :
            type(MSC_ATTR_BOOL), name(a), value(s), yes(b),
            linenum_attr(l), linenum_value(v), error(false) {}
        Attribute(const char*a, file_line_range l) :
            type(MSC_ATTR_STYLE), name(a),
            linenum_attr(l), linenum_value(l), error(false) {}

        string Print(int ident=0) const;
        bool Is(const char *a) const
            {return CaseInsensitiveEqual(a, name);}
        bool EndsWith(const char *a) const;
        bool StartsWith(const char *a) const;
        bool CheckType(MscAttrType t, MscError &error) const;
        bool CheckColor(const ColorSet &colors, MscError &error) const;
        void InvalidValueError(const string &candidates, MscError &error) const;
        void InvalidAttrError(MscError &error) const;
        void InvalidStyleError(MscError &error) const;
        bool EnsureNotClear(MscError &error, StyleType type) const;
};

typedef PtrList<Attribute> AttributeList;

class Msc;

////////////////////////////////////////////////////////////

typedef enum {
    LINE_INVALID = 0,
    LINE_NONE,
    LINE_SOLID,
    LINE_DOTTED,
    LINE_DASHED,
    LINE_LONG_DASHED,
    LINE_DASH_DOT,
    LINE_DOUBLE,
    LINE_TRIPLE,
    LINE_TRIPLE_THICK
} MscLineType;

inline bool IsLineTypeContinuous(MscLineType t) {return t!=LINE_DOTTED && t!=LINE_DASHED && t!=LINE_LONG_DASHED && t!=LINE_DASH_DOT;}
inline bool IsLineTypeDouble(MscLineType t) {return t==LINE_DOUBLE;}
inline bool IsLineTypeTriple(MscLineType t) {return t==LINE_TRIPLE || t==LINE_TRIPLE_THICK;}
inline bool IsLineTypeDoubleOrTriple(MscLineType t) {return IsLineTypeDouble(t) || IsLineTypeTriple(t);}
inline double LineWidthMultiplier(MscLineType t) {return t==LINE_NONE ? 0 : t==LINE_DOUBLE ? 3 : t==LINE_TRIPLE ? 5 : t==LINE_TRIPLE_THICK ? 6 : 1;}

typedef enum {
    CORNER_INVALID = 0,
    CORNER_NONE,
    CORNER_ROUND,
    CORNER_BEVEL,
    CORNER_NOTE
} MscCornerType;

inline double RadiusIncMultiplier(MscCornerType t) {return t==CORNER_ROUND ? 1 : t==CORNER_BEVEL || t==CORNER_NOTE ? tan(22.5*M_PI/180) : 0;}

class MscLineAttr {
public:
    std::pair<bool, MscLineType>   type;
    std::pair<bool, MscColorType>  color;
    std::pair<bool, double>        width;
    std::pair<bool, double>        radius;
    std::pair<bool, MscCornerType> corner;
    MscLineAttr();
    MscLineAttr(MscLineType t)  {Empty(); type.first = true;  type.second = t;}
    MscLineAttr(MscColorType c) {Empty(); color.first = true; color.second = c;}
    MscLineAttr(MscLineType t, MscColorType c, double w, MscCornerType ct, double r) :
        type(true, t), color(true, c), width(true, w), radius(true, r), corner(true, ct) {}
    void Empty() {type.first = color.first = width.first = corner.first = radius.first = false;}
    bool IsComplete() const {return type.first && color.first && width.first && corner.first && radius.first;}
    void MakeComplete();
    MscLineAttr &operator +=(const MscLineAttr&a);
    bool operator == (const MscLineAttr &a);

    //functions about line style
    bool IsContinuous() const {_ASSERT(type.first); return IsLineTypeContinuous(type.second);}
    bool IsDouble() const {_ASSERT(type.first); return IsLineTypeDouble(type.second);}
    bool IsTriple() const {_ASSERT(type.first); return IsLineTypeTriple(type.second);}
    bool IsDoubleOrTriple() const {return IsDouble() || IsTriple();}
    double DoubleSpacing() const     {_ASSERT(IsDouble()&&width.first); return width.second;}
    double TripleSpacing() const     {_ASSERT(IsTriple()&&width.first); return width.second*((type.second == LINE_TRIPLE_THICK)?2.5:2.0);}
    double TripleMiddleWidth() const {_ASSERT(IsTriple()&&width.first); return width.second*((type.second == LINE_TRIPLE_THICK)?2.0:1.0);}
    double LineWidth() const {_ASSERT(type.first && width.first); return width.second * LineWidthMultiplier(type.second);}
    const double * DashPattern(int &num) const;
    double RadiusIncMul() const {_ASSERT(type.first); return ::RadiusIncMultiplier(corner.second);}

    virtual bool AddAttribute(const Attribute &a, Msc *msc, StyleType t);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string &attr, Csh &csh);
    string Print(int ident = 0) const;

    Contour CreateRectangle(double x1, double x2, double y1, double y2) const;
    Contour CreateRectangle(const XY &s, const XY &d) const {return CreateRectangle(s.x, d.x, s.y, d.y);}
    Contour CreateRectangle(const Block &b) const {return CreateRectangle(b.x.from, b.x.till, b.y.from, b.y.till);}

    DoublePair CalculateTextMargin(Area textCover, double rect_top) const; 
};

typedef enum {
    GRADIENT_INVALID = 0,
    GRADIENT_NONE,
    GRADIENT_OUT,
    GRADIENT_IN,
    GRADIENT_DOWN,
    GRADIENT_UP,
    GRADIENT_LEFT,
    GRADIENT_RIGHT,
    GRADIENT_BUTTON
} MscGradientType;

struct MscFillAttr {
public:
    std::pair<bool, MscColorType> color;
    std::pair<bool, MscColorType> color2;
    std::pair<bool, MscGradientType> gradient;
    MscFillAttr();
    MscFillAttr(MscColorType c) {Empty(); color.first = true; color.second = c;}
    MscFillAttr(MscColorType c, MscGradientType g) :
        color(true, c), gradient(true,g) {}
    MscFillAttr(MscColorType c, MscColorType c2) : 
        color(true, c), color2(true, c2), gradient(false, GRADIENT_INVALID) {}
    MscFillAttr(MscColorType c, MscColorType c2, MscGradientType g) :
        color(true, c), color2(true, c2), gradient(true,g) {}
    void Empty() {color.first = color2.first = gradient.first = false;}
    void MakeComplete();
    bool IsComplete() const {return color.first && gradient.first;} //color2 is not needed
    MscFillAttr &operator +=(const MscFillAttr&a);
    bool operator == (const MscFillAttr &a);
    virtual bool AddAttribute(const Attribute &a, Msc *msc, StyleType t);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string &attr, Csh &csh);
    string Print(int ident = 0) const;
};

struct MscShadowAttr {
public:
    std::pair<bool, MscColorType> color;
	std::pair<bool, double> offset;
	std::pair<bool, double> blur;
    MscShadowAttr();
    MscShadowAttr(MscColorType c) {Empty(); color.first = true; color.second = c;}
    void Empty() {color.first = offset.first = blur.first=false;}
    void MakeComplete();
    bool IsComplete() const {return color.first && offset.first && blur.first;}
    MscShadowAttr &operator +=(const MscShadowAttr&a);
    bool operator == (const MscShadowAttr &a);
    virtual bool AddAttribute(const Attribute &a, Msc *msc, StyleType t);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string &attr, Csh &csh);
    string Print(int ident = 0) const;
};

bool CshHintGraphicCallbackForYesNo(MscCanvas *canvas, CshHintGraphicParam p);

#endif
