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

/** @file arrowhead.h The declaration of arrowhead styles.
 * @ingroup libmscgen_files */

#if !defined(ARROWHEAD_H)
#define ARROWHEAD_H
#include <vector>
#include "attribute.h"
#include "contour.h"

/** Describes arrowhead types. Both for line and block arrows.*/
enum EArrowType {
    //Both big and small arrows
    MSC_ARROW_INVALID = 0,  ///<Invalid value
    MSC_ARROW_NONE,         ///<No arrowhead
    MSC_ARROW_SOLID,        ///<Line: filled triangle, Block: wider than the body 
    MSC_ARROW_EMPTY,        ///<Line: empty triangle, Block: as wide as body
    MSC_ARROW_LINE,         ///<Line: just two lines
    MSC_ARROW_HALF,         ///<Line: just one line
    MSC_ARROW_DIAMOND,      ///<A filled diamond shape
    MSC_ARROW_DIAMOND_EMPTY,///<An emmpty diamond shape
    MSC_ARROW_DOT,          ///<A filled circle
    MSC_ARROW_DOT_EMPTY,    ///<An empty circle
    MSC_ARROW_SHARP,        ///<A filled triangle with miters
    MSC_ARROW_SHARP_EMPTY,  ///<An empty triangle with miters
    //Line arrows only
    MSC_ARROW_DOUBLE,       ///<Line only: Two solid triangles
    MSC_ARROW_DOUBLE_EMPTY, ///<Line only: Two empty triangles
    MSC_ARROW_DOUBLE_LINE,  ///<Line only: Two set of lines
    MSC_ARROW_DOUBLE_HALF,  ///<Line only: Two single lines
    MSC_ARROW_TRIPLE,       ///<Line only: Three solid triangles
    MSC_ARROW_TRIPLE_EMPTY, ///<Line only: Three empty triangles
    MSC_ARROW_TRIPLE_LINE,  ///<Line only: Three set of lines
    MSC_ARROW_TRIPLE_HALF,  ///<Line only: Three single lines
    //Block arrows only
    MSC_ARROW_EMPTY_INV,    ///<Block only: Inward trinagle as wide as body
    MSC_ARROW_STRIPES,      ///<Block only: Three rectangles of increasing size
    MSC_ARROW_TRIANGLE_STRIPES ///<Block only: Three triangles of increasing size
};

/** True if the type is applicable for line arrows */
inline bool MSC_ARROW_OK_FOR_ARROWS(EArrowType t) {return t>0 && t<=MSC_ARROW_TRIPLE_HALF;}
/** True if type is of double. */
inline bool MSC_ARROW_IS_DOUBLE(EArrowType t) {return t>=MSC_ARROW_DOUBLE && t<=MSC_ARROW_DOUBLE_HALF;}
/** True if type is of triple */
inline bool MSC_ARROW_IS_TRIPLE(EArrowType t) {return t>=MSC_ARROW_TRIPLE && t<=MSC_ARROW_TRIPLE_HALF;}
/** True if type is a single line sort*/
inline bool MSC_ARROW_IS_HALF(EArrowType t) {return t==MSC_ARROW_HALF || t==MSC_ARROW_DOUBLE_HALF || t==MSC_ARROW_TRIPLE_HALF;}
/** True if type is line-like.*/
inline bool MSC_ARROW_IS_LINE(EArrowType t) {return t==MSC_ARROW_LINE || t==MSC_ARROW_DOUBLE_LINE || t==MSC_ARROW_TRIPLE_LINE;}
/** True if the type is applicable for block arrows */
inline bool MSC_ARROW_OK_FOR_BIG_ARROWS(EArrowType t) {return t>0 && (t<=MSC_ARROW_SHARP_EMPTY || t>=MSC_ARROW_EMPTY_INV);}
/** True if the type symmetric to a horizontal line. */
inline bool MSC_ARROW_IS_SYMMETRIC(EArrowType t) {return t==MSC_ARROW_NONE || (MSC_ARROW_DIAMOND<=t && t<=MSC_ARROW_DOT_EMPTY);}

/** Describes the size of the arrowhead, see ArrowHead::arrowSizePercentage*/
enum EArrowSize {
    MSC_ARROWS_INVALID = 0, ///<The invalid value. Used as 20% size for hint popup pictograms
    MSC_ARROW_TINY,  ///<30% size                                                                        
    MSC_ARROW_SMALL, ///<50% size
    MSC_ARROW_NORMAL,///<75% size
    MSC_ARROW_BIG,   ///<100% size
    MSC_ARROW_HUGE   ///<150% size
};

/** Describes the position of the arrowhead in the arrow.*/
enum EArrowEnd {
    MSC_ARROW_END,   ///<The arrowhead in the 'to' direction, e.g., in case of 'a->b', the arrowhead at 'b'. Both ends are like this for bidirectional arrows.
    MSC_ARROW_MIDDLE,///<An arrowhead at the middle of a multi-segment arrow, e.g., for 'a->b->c' the one at 'b'.
    MSC_ARROW_START  ///<The arrowhead in the 'from' direction, e.g., in case of 'a->b', the arrowhead at 'a'.
};

class Canvas;

/** Describes and draws arrowheads and block arrow contours.*/
class ArrowHead
{
protected:
    /** Horizontal width & vertical depth of the arrow heads at MSC_ARROW_BIG. */
    static const double baseArrowWidth;   ///<X-size of triangle for 100% size
    static const double baseArrowHeight;  ///<half of the Y-size of triangle for 100% size
    static const double baseDiamondSize;  ///<X and Y size for a diamond for 100% size
    static const double baseDotSize;      ///<circle radius for dots for 100% size
    static const double arrowSizePercentage[6]; ///<Percentage values for EArrowSize enum.

public:
    /** Describes the overall type of an arrow */
    enum EArcArrowType {
        NONE,     ///<Invalid
        ARROW,    ///<Line arrow
        BIGARROW, ///<Block arrow
        ANY,      ///<Can be either type
        NOTE      ///<Arrowhead for notes (CommandNote)
    } type;
    LineAttr                    line;     ///<The line style for the arrowhead
    std::pair<bool, EArrowSize> size;     ///<The size of the arrowhead. Not set if `first` is false.
    std::pair<bool, double>     xmul;     ///<The x-size multipler of the arrowhead. Not set if `first` is false.
    std::pair<bool, double>     ymul;     ///<The y-size multipler of the arrowhead. Not set if `first` is false.
    std::pair<bool, EArrowType> endType;  ///<The type of the arrowhead at the end of the arrow (also start for bidirectional arrows). Not set if `first` is false.
    std::pair<bool, EArrowType> midType;  ///<The type of the arrowhead at the midpoint of a multi-segment of the arrow. Not set if `first` is false.
    std::pair<bool, EArrowType> startType;///<The type of the arrowhead at the start of the arrow. Not set if `first` is false.

    /** Creates a default arrowhead style.
     * The style will accept any type (both line or block arrow types), 
     * small size, no size multipliers, solid triangle type, both at the end 
     * and in the middle, no arrowhead at the start and default line style
     * (black, solid, width of 1).*/
    explicit ArrowHead(EArcArrowType t=ANY) : type(t), size(true, MSC_ARROW_SMALL), xmul(true, 1), ymul(true, 1),
        endType(true, MSC_ARROW_SOLID), midType(true, MSC_ARROW_SOLID),  startType(true, MSC_ARROW_NONE) {}
    /** Deletes all content, by setting all `first` to false*/
    void Empty();
    /** True if all attributes are set*/
    bool IsComplete() const {return line.IsComplete() && size.first && endType.first && midType.first && startType.first && xmul.first && ymul.first;}
    /** Set attributes that are not yet set (use default values)*/
    void MakeComplete();
    /** Merge another arrowhead style to us by copying any attribute that is set there.*/
    ArrowHead & operator += (const ArrowHead &);
    bool AddAttribute(const Attribute &a, Msc *msc, EStyleType t);
    /** Add a list of attribute names we accept to `csh`.*/
    static void AttributeNames(Csh &csh, const string &prefix);
    /** Add a list of attribute values we accept for attribute `attr` to `csh`.*/
    static bool AttributeValues(const std::string &attr, Csh &csh, EArcArrowType t);
    /** Return the type of arrowhead at the end specified by `which`.*/
    EArrowType GetType(bool bidir, EArrowEnd which) const;
    EArrowType GetType(bool forward, bool bidir, EArrowEnd which, bool left) const;

    void TransformCanvasForAngle(double degree, Canvas &canvas, double sx, double y) const; 
    void UnTransformCanvas(Canvas &canvas) const;

    /** @name Functions for line arrowheads
     * @{ */
    /** Tells how wide and high a specific arrowhead. The y coordinate is just half the height.*/
    XY getWidthHeight(bool bidir, EArrowEnd which) const;
    /** Tells how wide one triangle is. Same as getWidthHeight() for simple arrowheads, but differs from it for DOUBLE and TRIPLE ones.*/
    double getTriWidth(bool bidir, EArrowEnd which) const;
    DoublePair getWidths(bool forward, bool bidir, EArrowEnd which, const LineAttr &) const;
    Contour EntityLineCover(XY xy, bool /*forward*/, bool bidir, EArrowEnd which) const;
    Contour ClipForLine(XY xy, double act_size, bool forward, bool bidir, EArrowEnd which, const Block &total,
                        const LineAttr &mainline_left, const LineAttr &mainline_right) const;
    Contour Cover(XY xy, double act_size, bool forward, bool bidir, EArrowEnd which,
                  const LineAttr &/*mainline_left*/, const LineAttr &/*mainline_right*/) const;
    //This actually draws an arrowhead
    void Draw(XY xy, double act_size, bool forward, bool bidir, EArrowEnd which, 
              const LineAttr &mainline_left, const LineAttr &mainline_right, Canvas *) const;
    /** @} */

    /** @name Functions for block arrowheads
     * @{ */
    /** The characteristic size of arrowhead. Internal.*/
    XY getBigWidthHeight(EArrowType type, const LineAttr &ltype) const;
    double getBigWidthsForSpace(bool /*bidir*/, EArrowType type, EArrowEnd /*which*/, 
                                double /*body_height*/, double act_size, const LineAttr &ltype) const;
    double getBigMargin(Contour text_cover, double sy, double dy, bool margin_side_is_left, 
                        bool bidir, EArrowType type, const LineAttr &ltype) const;
    double bigYExtent(bool bidir, bool forward=true, const std::vector<LineAttr> *lines=NULL) const;
    Contour BigContourOneEntity(double x, double act_size, double sy, double dy, bool bidir,
                                EArrowType type, EArrowEnd which, const LineAttr &ltype, 
                                bool left, double *body_margin=NULL) const;
    Contour BigContour(const std::vector<double> &xPos, const std::vector<double> &act_size, 
                       double sy, double dy, bool forward, bool bidir, 
                       const std::vector<LineAttr> *lines, 
                       std::vector<Contour> &result) const;
    Contour BigHeadContour(const std::vector<double> &xPos, const std::vector<double> &act_size, 
                           double sy, double dy, bool forward, bool bidir, 
                           const std::vector<LineAttr> *lines, double compressGap) const;
    void BigDrawFromContour(std::vector<Contour> &result, const std::vector<LineAttr> *lines,
                 const FillAttr &fill, const ShadowAttr &shadow, Canvas &canvas,
                 double angle_radian=0) const;
    void BigDrawEmptyMid(const std::vector<double> &xPos, double sy, double dy, 
                         Canvas &canvas, const Contour *clip=NULL) const;
    void BigCalculateAndDraw(const std::vector<double> &xPos, const std::vector<double> &act_size, 
                             double sy, double dy, bool forward, bool bidir,
                             const FillAttr &fill, const ShadowAttr &shadow, Canvas &canvas,
                             const Contour *clip=NULL, double angle_radian=0) const;
    /** @} */
};

bool CshHintGraphicCallbackForArrows(Canvas *canvas, EArrowType type, EArrowSize size, bool left);
bool CshHintGraphicCallbackForBigArrows(Canvas *canvas, CshHintGraphicParam p);


#endif //ARROWHEAD_H
