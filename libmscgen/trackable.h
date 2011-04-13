#if !defined(TRACKABLE_H)
#define TRACKABLE_H

#include <set>
#include <climits>
#include <cmath>
#include <utility>
#include "error.h" //for file_line
#include "contour_area.h" //for file_line

typedef enum {
    MSC_CONTROL_INVALID,
    MSC_CONTROL_EXPAND,
    MSC_CONTROL_COLLAPSE
} MscControlType;

class Msc;
class MscCanvas;

//This is a set of Areas, that may overlap
class TrackableElement {
protected:
    static const XY control_size;
    Msc * const chart;
    bool   linenum_final; //true if file_pos below is the final value
    Area   area;          //The area covered by the element...
    double yPos;          //...drawn at this point
    Area   area_draw;     //The area to draw when highlighting the element
    bool   draw_is_different;  //True is area_draw is different from area
    bool   area_draw_is_frame; /* if so, we will not expand area_draw in PostPosProcess */
    std::vector<MscControlType> 
           controls;      //Controls added for this box  
    Block  control_location; //Top-left corner of controls

public:
    file_line_range file_pos;
    explicit TrackableElement(Msc *m);
    TrackableElement(const TrackableElement&);
    void SetLineEnd(file_line_range l, bool f=true);
    virtual void ShiftBy(double y);
    const Area &GetAreaToSearch() const {return area;};
    const Area &GetAreaToDraw() const {return draw_is_different ? area_draw : area;}
    const std::vector<MscControlType>& GetControls() const {return controls;}
    const Block &GetControlLocation() const {return control_location;}
    virtual void PostPosProcess(double);
    void DrawControls(MscCanvas*, double size);
    MscControlType WhichControl(const XY &xy);
};


#endif //TRACKABLE_H
