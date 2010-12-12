#ifndef STYLE_H
#define STYLE_H

#include "attribute.h"
#include "arrowhead.h"
#include "stringparse.h"
#include "numbering.h"
#include "csh.h"

struct MscStyle
{
    MscLineAttr line, vline;
    MscFillAttr fill;
    MscShadowAttr shadow;
    ArrowHead arrow;
    StringFormat text;
    std::pair<bool, unsigned char> solid;
    std::pair<bool, bool> numbering;
    std::pair<bool, bool> compress;

    StyleType type;

    bool f_line, f_vline, f_fill, f_shadow, f_text, f_arrow, f_solid, f_numbering, f_compress;

    MscStyle(StyleType tt=STYLE_STYLE); //Has all the components, but is empty
    MscStyle(StyleType tt, bool a, bool t, bool l, bool f, bool s, bool vl, bool so, bool nu, bool co);
    void Empty();
    MscStyle &operator +=(const MscStyle &toadd);
    MscStyle operator +(const MscStyle &toadd) const
        {return MscStyle(*this) += toadd;}
    bool AddAttribute(const Attribute &a, Msc *m);
    void AttributeNames(Csh &csh) const;
    bool AttributeValues(const std::string &attr, Csh &csh) const;
    std::string Print(int ident = 0) const;
};

class StyleSet : public std::map<std::string, MscStyle>
{
    MscStyle defaultStyle;
public:
    StyleSet() {} //def style is empty
    explicit StyleSet(const MscStyle &a) : defaultStyle(a) {}
    const MscStyle &GetStyle(const string&) const;
};

class Context
{
public:
    bool           numbering;
    bool           compress;
    StyleSet       styles;
    ColorSet       colors;
    NumberingStyle numberingStyle;
    Context() : numbering(false), compress(false) {}
};

class Design : public Context
{
public:
    double hscale;
    Design() {Reset();}
    void Reset();
};

#endif //STYLE_H