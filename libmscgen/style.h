#ifndef STYLE_H
#define STYLE_H

#include "attribute.h"
#include "arrowhead.h"
#include "stringparse.h"

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
    bool AddAttribute(const Attribute &a, Msc *m);
    std::string Print(int ident = 0) const;
};

class StyleSet : public std::map<std::string, MscStyle>
{
public:
    bool numbering;
    bool compress;

    MscStyle defaultStyle;
    StyleSet() : numbering(false), compress(false) {} //def style is empty
    StyleSet(const MscStyle &a, bool num, bool comp) :
        defaultStyle(a), numbering(num), compress(comp) {};
    const MscStyle &GetStyle(const string&) const;
};

class Design
{
public:
    StyleSet styles;
    ColorSet colors;
    double hscale;
    Design() {Reset();}
    void Reset();
};

#endif //STYLE_H