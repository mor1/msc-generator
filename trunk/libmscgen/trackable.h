#if !defined(TRACKABLE_H)
#define TRACKABLE_H

#include <set>
#include <climits>
#include <cmath>
#include <utility>
#include "error.h" //for file_line
#include "area.h"  //for Area
#include "style.h" //for style

typedef enum {
    MSC_CONTROL_INVALID,
    MSC_CONTROL_EXPAND,
    MSC_CONTROL_COLLAPSE, 
    MSC_CONTROL_ARROW
} MscControlType;

class Msc;
class MscCanvas;
class CommandNote;
class EntityDistanceMap;

typedef PtrList<CommandNote> CommandNoteList;
//This is a set of Areas, that may overlap
class TrackableElement {
public:
    typedef enum {INVALID, BEFORE_ENTITY_LINES, AFTER_ENTITY_LINES, DEFAULT, AFTER_DEFAULT, NOTE, AFTER_NOTE} DrawPassType;
protected:
    static const XY control_size;
    static const XY indicator_size;
    Msc * const     chart;
    bool            hidden;             //true if we are hidden inside a collapsed box
    bool            linenum_final;      //true if file_pos below is the final value
    Area            area;               //The area covered by the element...
    double          yPos;               //...drawn at this point
    Contour         area_draw;          //The area to draw when highlighting the element
    bool            draw_is_different;  //True is area_draw is different from area
    bool            area_draw_is_frame; /* if so, we will not expand area_draw in PostPosProcess */
    Contour         area_to_note;       //if not empty the notes will point towards this area

    CommandNoteList notes;              // Notes attached to this element
    Contour         area_important;     /* those parts of our coverage, which must not be covered by notes */

    std::vector<MscControlType> 
           controls;           //Controls added for this box  
    Block  control_location;   //Top-left corner of controls
    const MscStyle 
           indicator_style;  //The one to be used if we need to replace this with an indicator
    XY GetIndiactorSize() const {const double a = indicator_style.shadow.offset.second+indicator_style.line.LineWidth()*2; return indicator_size+XY(a,a);}

public:
    file_line_range file_pos;
    explicit TrackableElement(Msc *m);
    TrackableElement(const TrackableElement&);
    void SetLineEnd(file_line_range l, bool f=true);
    virtual TrackableElement* AttachNote(CommandNote *cn);
    void CombineNotes(TrackableElement *); //move notes to us
    virtual void MoveNotesToChart();
    virtual void ShiftBy(double y);
    const Area &GetAreaToSearch() const   //An area over which if the mouse hoovers, we highlight the element
        {return area;};
    const Contour &GetAreaToDraw() const  //An area to highlight when the mouse moves over
        {return draw_is_different ? area_draw : area;}
    const Contour &GetAreaToNote() const  //An area to the edge of which notes made to this element will point 
        {return area_to_note.IsEmpty() ? area : area_to_note;}
    const Contour &GetAreaImportant() const     //An area which shall possibly not get covered by notes
        {return area_important;}
    const std::vector<MscControlType>& GetControls() const {return controls;}
    const Block &GetControlLocation() const {return control_location;}
    virtual void PostParseProcessNotes(MscCanvas &canvas, bool hide, bool at_top_level);
    double NoteHeight(MscCanvas &canvas, AreaList &cover);
    virtual void PostPosProcess(MscCanvas &, double);

    void DrawControls(MscCanvas*, double size);
    MscControlType WhichControl(const XY &xy);

    Block GetIndicatorCover(const XY &pos);
    void DrawIndicator(XY pos, MscCanvas *canvas);
};


#endif //TRACKABLE_H
