#if !defined(ARROWHEAD_H)
#define ARROWHEAD_H
#include <vector>
#include "attribute.h"
#include "contour.h"

typedef enum {
    //Both big and small arrows
    MSC_ARROW_INVALID = 0,
    MSC_ARROW_NONE,
    MSC_ARROW_SOLID,
    MSC_ARROW_EMPTY,
    MSC_ARROW_LINE,
    MSC_ARROW_HALF,
    MSC_ARROW_DIAMOND,
    MSC_ARROW_DIAMOND_EMPTY,
    MSC_ARROW_DOT,
    MSC_ARROW_DOT_EMPTY,
    MSC_ARROW_SHARP,
    MSC_ARROW_SHARP_EMPTY,
    //Small arrows only
    MSC_ARROW_DOUBLE,
    MSC_ARROW_DOUBLE_EMPTY,
    MSC_ARROW_DOUBLE_LINE,
    MSC_ARROW_DOUBLE_HALF,
    MSC_ARROW_TRIPLE,
    MSC_ARROW_TRIPLE_EMPTY,
    MSC_ARROW_TRIPLE_LINE,
    MSC_ARROW_TRIPLE_HALF,
    //Big arrows only
    MSC_ARROW_EMPTY_INV,
    MSC_ARROW_STRIPES,
    MSC_ARROW_TRIANGLE_STRIPES
} MscArrowType;

inline bool MSC_ARROW_OK_FOR_ARROWS(MscArrowType t) {return t>0 && t<=MSC_ARROW_TRIPLE_HALF;}
inline bool MSC_ARROW_IS_DOUBLE(MscArrowType t) {return t>=MSC_ARROW_DOUBLE && t<=MSC_ARROW_DOUBLE_HALF;}
inline bool MSC_ARROW_IS_TRIPLE(MscArrowType t) {return t>=MSC_ARROW_TRIPLE && t<=MSC_ARROW_TRIPLE_HALF;}
inline bool MSC_ARROW_IS_HALF(MscArrowType t) {return t==MSC_ARROW_HALF || t==MSC_ARROW_DOUBLE_HALF || t==MSC_ARROW_TRIPLE_HALF;}
inline bool MSC_ARROW_IS_LINE(MscArrowType t) {return t==MSC_ARROW_LINE || t==MSC_ARROW_DOUBLE_LINE || t==MSC_ARROW_TRIPLE_LINE;}
inline bool MSC_ARROW_OK_FOR_BIG_ARROWS(MscArrowType t) {return t>0 && (t<=MSC_ARROW_SHARP_EMPTY || t>=MSC_ARROW_EMPTY_INV);}

typedef enum {
    MSC_ARROWS_INVALID = 0,
    MSC_ARROW_TINY,
    MSC_ARROW_SMALL,
    MSC_ARROW_NORMAL,
    MSC_ARROW_BIG,
    MSC_ARROW_HUGE
} MscArrowSize;


typedef enum {
    MSC_ARROW_END,
    MSC_ARROW_MIDDLE,
    MSC_ARROW_START
} MscArrowEnd;

class MscCanvas;

class ArrowHead
{
protected:
    /** Horizontal width & vertical depth of the arrow heads at MSC_ARROW_BIG. */
    static double baseArrowWidth, baseArrowHeight;
    static double baseDiamondSize;
    static double baseDotSize;
    static double arrowSizePercentage[6];
public:
    enum ArcType {NONE, ARROW, BIGARROW, ANY} type;
    MscLineAttr                   line;
    std::pair<bool, MscArrowSize> size;
    std::pair<bool, double>       xmul;
    std::pair<bool, double>       ymul;
    std::pair<bool, MscArrowType> endType;
    std::pair<bool, MscArrowType> midType;
    std::pair<bool, MscArrowType> startType;

    explicit ArrowHead(ArcType t=ANY) : type(t), size(true, MSC_ARROW_SMALL), xmul(true, 1), ymul(true, 1),
        endType(true, MSC_ARROW_SOLID), midType(true, MSC_ARROW_SOLID),  startType(true, MSC_ARROW_NONE) {}
    void Empty();
    bool IsComplete() const {return line.IsComplete() && size.first && endType.first && midType.first && startType.first && xmul.first && ymul.first;}
    ArrowHead & operator += (const ArrowHead &);
    bool AddAttribute(const Attribute &a, Msc *msc, StyleType t);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string &attr, Csh &csh, ArcType t);
    //tells what sort of MscArrowType should be drawn
    MscArrowType GetType(bool bidir, MscArrowEnd which) const;

//functions for normal (small) arrowheads
    //tells how wide and high a specific arrowhead
    XY getWidthHeight(bool bidir, MscArrowEnd which) const;
    //tells how wide a triangle is (=above for simple arrowheads, differs for DOUBLE and TRIPLE ones)
    double getTriWidth(bool bidir, MscArrowEnd which) const;
    //tells how much of the arrow line is covered by the arrowhead for text margin (on both sides of the entity line)
    DoublePair getWidths(bool forward, bool bidir, MscArrowEnd which, const MscLineAttr &mainline) const;
    //tells what area of the entity line is covered by the arrowhead
    Contour EntityLineCover(XY xy, bool forward, bool bidir, MscArrowEnd which) const;
    //Returns a clip contour covering the arrowhead and the rest of the chart (both sides)
    Contour ClipForLine(XY xy, double act_size, bool forward, bool bidir, MscArrowEnd which, const Block &total,
                        const MscLineAttr &mainline_left, const MscLineAttr &mainline_right) const;
    //Returns a contour covering the arrowhead
    Contour Cover(XY xy, double act_size, bool forward, bool bidir, MscArrowEnd which) const;
    //This actually draws an arrowhead
    void Draw(XY xy, double act_size, bool forward, bool bidir, MscArrowEnd which, MscCanvas *) const;

//functions for block arrow heads
    //The characteristic size of arrowhead
    XY getBigWidthHeight(bool bidir, MscArrowEnd which) const;
    //Returns true if this type of arrow will segment a big arrow as midtype
    bool bigDoesSegment(bool bidir, MscArrowEnd which) const;
    //tells how much of the arrow body is covered by the arrowhead (on both sides of the entity line)
    DoublePair getBigWidthsForBody(bool forward, bool bidir, MscArrowEnd which, double body_height) const;
    //tells the full width of the arrowhead (on both sides of the entity line) for ArcBigArrow::Width()
    DoublePair getBigWidthsForSpace(bool forward, bool bidir, MscArrowEnd which, double body_height) const;
    //Determines how much margin is needed for a text with this cover
    double getBigMargin(Contour text_cover, double sy, double dy, bool left, bool forward, bool bidir, MscArrowEnd which) const;
    //tells how much the arrow (overall) extends above or below sy and dy
    double bigYExtent(bool bidir, bool multisegment) const;
    Contour BigCoverOne(double x, double act_size, double sy, double dy, bool forward, bool bidir, MscArrowEnd which) const;
    Contour BigCover(std::vector<double> xPos, std::vector<double> act_size, double sy, double dy, 
                     bool bidir, int no_segment=-1) const;
    Contour BigEntityLineCover(const std::vector<double> &xPos, std::vector<double> act_size, double sy, double dy, bool bidir,
                               const std::vector<MscLineAttr> *lines, const Block &total) const;
    void BigDraw(const std::vector<double> &xPos, std::vector<double> act_size, 
                 double sy, double dy, bool bidir,  const MscShadowAttr &shadow,
                 const MscFillAttr &fill, const std::vector<MscLineAttr> *lines, MscCanvas *canvas,
                 const Contour *clip=NULL, bool shadow_x_neg=false, bool shadow_y_neg=false) const;
};

#endif //ARROWHEAD_H
