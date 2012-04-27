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

class MscStyle
{
protected:
    friend class Context;
    MscStyle(StyleType tt, ArrowHead::ArcType a, bool t, bool l, bool f, bool s, bool vl, 
             bool so, bool nu, bool co, bool si, bool i, bool vf, bool mr, bool n);
public:
    MscLineAttr line, vline;
    MscFillAttr fill, vfill;
    MscShadowAttr shadow;
    ArrowHead arrow;
    StringFormat text;
    std::pair<bool, unsigned char> solid;
    std::pair<bool, MscSideType>   side; //for pipes, verticals or notes
    std::pair<bool, bool>          numbering;
    std::pair<bool, bool>          compress;
    std::pair<bool, bool>          indicator;
    std::pair<bool, bool>          makeroom; //for verticals and notes
    MscNoteAttr note;

    StyleType type;

    bool f_line, f_vline, f_fill, f_vfill, f_shadow, f_text, 
         f_solid, f_numbering, f_compress, f_side, f_indicator,
         f_makeroom, f_note;
    ArrowHead::ArcType f_arrow;

    MscStyle(StyleType tt=STYLE_STYLE); //Has all the components, but is empty
    void Empty();
    void MakeCompleteButText();
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
    bool is_full;
    std::pair<bool, double> hscale;
    std::pair<bool, bool>   numbering;
    std::pair<bool, bool>   compress;
    std::pair<bool, bool>   indicator;
    std::pair<bool, double> slant_angle;
    MscLineAttr defCommentLine;
    MscFillAttr defCommentFill;
    MscFillAttr defBackground;
    StringFormat            text;
    StyleSet                styles;
    ColorSet                colors;
    NumberingStyle          numberingStyle;
    Context() {Empty();}
    Context(bool) {Plain();}
    void Empty(); 
    void Plain();
    bool IsComplete() const {return hscale.first && defCommentLine.IsComplete() && defCommentFill.IsComplete() && defBackground.IsComplete() && numbering.first && compress.first && indicator.first && slant_angle.first && styles.size() && colors.size() && numberingStyle.IsComplete();}
    Context &operator +=(const Context &o);
};

#endif //STYLE_H