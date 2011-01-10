#if !defined(TRACKABLE_H)
#define TRACKABLE_H

#include <set>
#include <climits>
#include <cmath>
#include <utility>
#include "error.h" //for file_line
#include "contour_area.h" //for file_line

//This is a set of Areas, that may overlap
class TrackableElement {
protected:
    bool linenum_final; //true if file_pos below is the final value
    Area area;          //The area covered by the element...
    double yPos;        //...drawn at this point
    Area area_draw;     //The area to draw when highlighting the element
    bool draw_is_different;
public:
    file_line_range file_pos;
    TrackableElement() : linenum_final(false), draw_is_different(false), yPos(0) {area_draw.arc = area.arc = this;}
    TrackableElement(const TrackableElement&);
    void SetLineEnd(file_line_range l, bool f=true);
    virtual void ShiftBy(double y);
    const Area &GetAreaToSearch() const {return area;};
    const Area &GetAreaToDraw() const {return draw_is_different ? area_draw : area;}
};


#endif //TRACKABLE_H
