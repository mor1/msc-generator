#ifndef STYLE_H
#define STYLE_H

#include "attribute.h"
#include "arrowhead.h"
#include "stringparse.h"
#include "numbering.h"
#include "csh.h"

typedef enum {
    SIDE_INVALID = 0,
    SIDE_LEFT,SIDE_RIGHT
} MscSideType;
bool CshHintGraphicCallbackForSide(MscCanvas *canvas, CshHintGraphicParam p);

struct MscStyle
{
    MscLineAttr line, vline;
    MscFillAttr fill, vfill;
    MscShadowAttr shadow;
    ArrowHead arrow;
    StringFormat text;
    std::pair<bool, unsigned char> solid;
    std::pair<bool, MscSideType> side; //for pipes, verticals or notes
    std::pair<bool, bool> numbering;
    std::pair<bool, bool> compress;
    std::pair<bool, bool> indicator;

    StyleType type;

    bool f_line, f_vline, f_fill, f_vfill, f_shadow, f_text, f_solid, f_numbering, f_compress, f_side, f_indicator;
    ArrowHead::ArcType f_arrow;

    MscStyle(StyleType tt=STYLE_STYLE); //Has all the components, but is empty
    MscStyle(StyleType tt, ArrowHead::ArcType a, bool t, bool l, bool f, bool s, bool vl, bool so, bool nu, bool co, bool si, bool i, bool vf);
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
    bool           indicator;
    double         slant_angle;
    StringFormat   text;
    StyleSet       styles;
    ColorSet       colors;
    NumberingStyle numberingStyle;
    Context() : numbering(false), compress(false), indicator(true), slant_angle(0) {text.Default();}
};

class Design : public Context
{
public:
    double hscale;
    Design() {Reset();}
    void Reset();
};

#endif //STYLE_H