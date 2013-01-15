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

/** @file style.h The declaration of styles (MscStyle) and contexts (Context).
 * @ingroup libmscgen_files */

#ifndef STYLE_H
#define STYLE_H

#include "attribute.h"
#include "arrowhead.h"
#include "stringparse.h"
#include "numbering.h"
#include "csh.h"

/** Describes the values of the 'side' attribute*/
typedef enum {
    SIDE_INVALID = 0,///<The invalid value.
    SIDE_LEFT,       ///<The left side
    SIDE_RIGHT       ///<The right side
} MscSideType;

bool CshHintGraphicCallbackForSide(MscCanvas *canvas, CshHintGraphicParam p);

/** A class bringing together all style-related attributes.
 * An instance of MscStyle may not *contain* all possible attributes.
 * For example only arrow arcs have arrowheads, thus for any other element
 * we shall not be able to specify arrowhead attributes. We say the style
 * instance of those elements 'does not contain' an arrowhead attribute.
 * This is achieved via a series of flags (members starting with `f_`)
 * in class MscStyle indicating which
 * set of attributes the instance contains or not. Attributes not contained
 * will not be accepted via AddAttribute or operator +=.
 *
 * The ArrowHead attributes are special in that they can not only be contained
 * by an instance of MscStyle or not contained, but one can also specify which
 * arrow type may be assigned: none (not contained), types for line arrows,
 * types for block arrows or any arrow type.
 *
 * In addition each attribue an MscStyle contains may or may not be set.
 * An attribute that is not set has no value and any such attribute makes
 * the style *incomplete*. Attributes not set can be assigned value via
 * operator += etc.
 *
 * An MscStyle object also reacts differently depending on the context of 
 * its use. Styles of arcs, for example, must be fully specified, since we
 * need a value to all attributes. Chart styles (such as `weak` or `strong`)
 * on the other hand may contain unset attributes - these attributes will not
 * be changed in the arc when we apply the style to an arc. The context of
 * use is captured in the `type` field of MscStyle.
 */
class MscStyle
{
protected:
    friend class Context;
    MscStyle(StyleType tt, ArrowHead::ArcType a, bool t, bool l, bool f, bool s, bool vl, 
             bool so, bool nu, bool co, bool si, bool i, bool vf, bool mr, bool n);
public:
    MscLineAttr line;     ///<The line attributes
    MscLineAttr vline;    ///<The vline attributes
    MscFillAttr fill;     ///<The fill attributes
    MscFillAttr vfill;    ///<The vfill attributes
    MscShadowAttr shadow; ///<The shadow attributes
    ArrowHead arrow;      ///<The arrow attributes
    StringFormat text;    ///<The text attributes
    std::pair<bool, unsigned char> solid;    ///<The value of the 'solid' attribute (for pipes). Not set if `first` is false.
    std::pair<bool, MscSideType>   side;     ///<The value of the 'side' attribute (for pipes, verticals or notes). Not set if `first` is false.
    std::pair<bool, bool>          numbering;///<The value of the 'number' attribute. Not set if `first` is false.
    std::pair<bool, bool>          compress; ///<The value of the 'compress' attribute. Not set if `first` is false.
    std::pair<bool, bool>          indicator;///<The value of the 'indicator' attribute. Not set if `first` is false.
    std::pair<bool, bool>          makeroom; ///<The value of the 'makeroom' attribute (for verticals and notes). Not set if `first` is false.
    MscNoteAttr note;     ///<The note attributes

    StyleType type;       ///<The context in which this instance is used.

    bool f_line;       ///<True if the style contains line attributes.
    bool f_vline;      ///<True if the style contains vline attributes.
    bool f_fill;       ///<True if the style contains fill attributes.
    bool f_vfill;      ///<True if the style contains vfill attributes.
    bool f_shadow;     ///<True if the style contains shadow attributes.
    bool f_text;       ///<True if the style contains text attributes.
    bool f_solid;      ///<True if the style contains the 'solid' attributes.
    bool f_numbering;  ///<True if the style contains the 'number' attributes.
    bool f_compress;   ///<True if the style contains the 'compress' attributes.
    bool f_side;       ///<True if the style contains the 'side' attributes.
    bool f_indicator;  ///<True if the style contains the 'indicator' attributes.
    bool f_makeroom;   ///<True if the style contains the 'makeroom' attributes.
    bool f_note;       ///<True if the style contains note attributes.
    ArrowHead::ArcType f_arrow; ///<Shows which type of arrow attributes the style contains.

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

/** A set of styles associated by name.
 * The set also contains a default style that is returned, when
 * a style name is not found.
 */
class StyleSet : public std::map<std::string, MscStyle>
{
    MscStyle defaultStyle; ///<The default style.
public:
    StyleSet() {} //def style is empty
    explicit StyleSet(const MscStyle &a) : defaultStyle(a) {}
    const MscStyle &GetStyle(const string&) const;
};

/** A context containing style and color definitions and chart option values.
 *
 * A *context* is a set of settings valid at a given place in the file during 
 * parse. It includes the current definition of colors, styles and a few global
 * option, such as 'compress', 'numbering', 'indicator', 'slant_angle' and 'hscale'.
 * (Other chart options are not really options, but rather generate
 * chart elements, such as background.*).
 * They also include the current comment line and fill style, the default font
 * and current numbering style. Any time you open a brace in the input file,
 * a new context is opened. If you change any value of it, it will get restored
 * at the closing brace. 
 * Contexts are said to be *full* if they contain a setting for all chart
 * options they contain and a complete value sets for all default styles.
 * By definition, only the default Design (plain) is full and any context
 * it is applied to. There is no other way for a context to become full.
 * Contexts encountered during parsing are always full (since they 
 * take value from a full design), except the context within a design 
 * definition, unless the design definition takes a full design as base
 * by containing an 'msc=' option. */
class Context
{
public:
    bool is_full;                        ///<True if the context contains a setting for all chart options and a complete value for all default styles.
    std::pair<bool, double> hscale;      ///<The 'hscale' chart option, unset if `second` is false.
    std::pair<bool, bool>   numbering;   ///<The 'numbering' chart option, unset if `second` is false.
    std::pair<bool, bool>   compress;    ///<The 'compress' chart option, unset if `second` is false.
    std::pair<bool, bool>   indicator;   ///<The 'indicator' chart option, unset if `second` is false.
    std::pair<bool, double> slant_angle; ///<The 'slant_angle' chart option, unset if `second` is false.
    MscLineAttr defCommentLine;          ///<The style of the comment line 
    MscFillAttr defCommentFill;          ///<The fill of the comment area 
    MscFillAttr defBackground;           ///<The background 
    StringFormat            text;        ///<The default text style
    StyleSet                styles;      ///<The set of named styles defined here
    ColorSet                colors;      ///<The set of named colors defined here
    NumberingStyle          numberingStyle;///<The numbering style in use
    Context() {Empty();}     ///<Create an empty context
    Context(bool) {Plain();} ///<Create the default context (with 'plain' design)
    void Empty(); ///<Make the context empty
    void Plain(); ///<Set the context to the 'plain' design
    /** Merges a context into us by applying members that are set in `o`*/
    Context &operator +=(const Context &o);
};

#endif //STYLE_H