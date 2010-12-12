#if !defined(ARROWHEAD_H)
#define ARROWHEAD_H
#include <vector>
#include "attribute.h"
#include "trackable.h" //for Block

typedef enum {
    MSC_ARROW_INVALID = 0,
    MSC_ARROW_NONE,
    MSC_ARROW_SOLID,
    MSC_ARROW_EMPTY,
    MSC_ARROW_LINE,
    MSC_ARROW_HALF,
    MSC_ARROW_DIAMOND,
    MSC_ARROW_DIAMOND_EMPTY,
    MSC_ARROW_DOT,
    MSC_ARROW_DOT_EMPTY
} MscArrowType;

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

class MscDrawer;

class ArrowHead
{
protected:
    /** Horizontal width & vertical depth of the arrow heads at MSC_ARROW_BIG. */
    static double baseArrowWidth, baseArrowHeight;
    static double baseDiamondSize;
    static double baseDotSize;
    static double arrowSizePercentage[6];


    void big_end_path(double &xpos, double x, double sy, double dy, bool dir,
                      bool up, MscDrawer *, MscArrowType, MscLineType, double lw) const;
    void big_mid_path(double &xpos, double x, double sy, double dy,
                      bool up, MscDrawer *, MscArrowType, MscLineType, double lw) const;
    void big_path(std::vector<double> xPos, double sy, double dy,
                  MscDrawer *, MscLineType type, bool bidir, double lw) const;
    Block big_cover_block(double x, double sx, double dy,
                          bool bidir, MscArrowEnd which, bool forward, MscDrawer *msc) const;

public:
    MscLineAttr                   line;
    std::pair<bool, MscArrowSize> size;
    std::pair<bool, MscArrowType> endType;
    std::pair<bool, MscArrowType> midType;
    std::pair<bool, MscArrowType> startType;

    ArrowHead() :
        size(true, MSC_ARROW_SMALL), endType(true, MSC_ARROW_SOLID),
        midType(true, MSC_ARROW_SOLID),  startType(true, MSC_ARROW_NONE) {}
    void Empty();
    ArrowHead & operator += (const ArrowHead &);
    bool AddAttribute(const Attribute &a, Msc *msc, StyleType t);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string &attr, Csh &csh);

    MscArrowType GetType(bool bidir, MscArrowEnd which) const;
    XY getWidthHeight(bool bidir, MscArrowEnd which, MscDrawer *) const;
    std::pair<double, double> getWidthForLine(bool bidir, MscArrowEnd which, MscDrawer *) const;
    Range EntityLineCover(XY xy, bool forward, bool bidir, MscArrowEnd which,
                          MscDrawer *msc) const;
    void Draw(XY xy, bool forward, bool bidir, MscArrowEnd which, MscDrawer *) const;

    XY getBigWidthHeight(MscArrowType type, MscDrawer *msc) const;
    XY getBigWidthHeight(bool bidir, MscArrowEnd which, MscDrawer *msc) const
        {return getBigWidthHeight(GetType(bidir, which), msc);}
    double getBigEndWidthExt(bool bidir, MscArrowEnd which, MscDrawer *msc) const;
    double getBigEndWidthMargin(bool bidir, MscArrowEnd which, MscDrawer *msc) const;

    double Big_yExtent(bool bidir, bool segmented, MscDrawer *) const;
    void DrawBig(const std::vector<double> &xPos, double sy, double dy,
                 bool bidir, MscDrawer *);
    void FillBig(const std::vector<double> &xPos, double sy, double dy,
                 bool bidir, MscDrawer *, MscFillAttr);
    void CoverBig(const std::vector<double> &xPos, double sy, double dy,
                  bool bidir, MscDrawer * msc, Geometry &cover) const;
    void ClipBig(std::vector<double> xPos, double sy, double dy,
                 bool bidir, MscDrawer *m) const;
};

#endif //ARROWHEAD_H
