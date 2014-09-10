/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010,2011,2012,2013,2014 Zoltan Turanyi
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

/** @file element.h The declaration of Element - the base for all chart elements.
 * @ingroup libmscgen_files */

#if !defined(TRACKABLE_H)
#define TRACKABLE_H

#include <set>
#include <climits>
#include <cmath>
#include <utility>
#include "error.h" //for FileLineCol
#include "area.h"  //for Area
#include "style.h" //for style

/** Describes the type of a GUI control*/
enum EGUIControlType {
    MSC_CONTROL_INVALID=0,///<The invalid value
    MSC_CONTROL_EXPAND,   ///<A control to expand a collapsed element
    MSC_CONTROL_COLLAPSE, ///<A control to collapse an expanded group element (box or group entity, etc)
    MSC_CONTROL_ARROW     ///<A control to collapse a box to a block arrow
};

/** A list of drawing passes.*/
enum EDrawPassType {
    DRAW_INVALID = 0,         ///<The invalid value
    DRAW_BEFORE_ENTITY_LINES, ///<The first drawing pass: before the entity lines are drawn
    DRAW_AFTER_ENTITY_LINES,  ///<A drawing pass just after the entity lines are drawn
    DRAW_DEFAULT,             ///<The default drawing pass, when most elements are drawn
    DRAW_AFTER_DEFAULT,       ///<A drawing pass after the default pass
    DRAW_NOTE,                ///<A drawing pass to draw floating notes
    DRAW_AFTER_NOTE           ///<The last drawing pass: after drawing floating notes.
};

class Msc;
class Canvas;
class CommandNote;
class EntityDistanceMap;
class DistanceMapVertical;

/** Indicates that the note shall be deleted */
#define DELETE_NOTE ((Element * )1)
/** A list of CommandNote object pointers */
typedef PtrList<CommandNote> CommandNoteList;

/** The base object for elements on the chart (entity headings, arcs, boxes, commands, etc.)
 * This object contains a lot of members useful for the Windows GUI and not needed
 * for commandline operation. A potential refactoring would take these out. */
class Element {
public:
    /** An global instance of the `plain` design.
     * Used by descendants to initialize their `style` member.
     * Used by ArcBase::AddAttributeNames to see which style components an element has.
     * Used by CshContext::SetPlain() to take default style and color names and definitions.*/
    static const Context defaultDesign;

protected:
    static const XY control_size;       ///<The size of a GUI control
    static const XY indicator_size;     ///<The size of an indicator
    Msc * const     chart;              ///<The chart this element belongs to
    bool            hidden;             ///<True if we are hidden inside a collapsed box
    bool            linenum_final;      ///<True if `file_pos` member is the final value. Needed during parsing.
    mutable double  yPos;               ///<The y position of this element.
    mutable Area    area;               ///<The area covered by the element. This is used by tracking to recognize which element the pointer points to.
    mutable Contour area_draw;          ///<The area to draw when highlighting the element - if different from `area` (usually a frame, like for e.g., boxes with content)
    mutable bool    draw_is_different;  ///<True is `area_draw` is different from `area`.
    mutable bool    area_draw_is_frame; ///<True if `area_draw` is a frame. If so, we will not expand `area_draw` in PostPosProcess.
    mutable Contour area_to_note;       ///<If not empty the notes targeting this element will point towards this area.
    mutable Contour area_to_note2;      ///<If a note with "at" clause does not hit the 'area_to_note`, try this.

    CommandNoteList comments;           ///<A pointer to comments attached to this element. Comments exist independently and will not be deleted on the deletion of this Element.
    double          comment_height;     ///<Total height of the comments attached (max of the two sides)
    mutable Contour area_important;     ///<Those parts of `area`, which must not be covered by notes.

    mutable std::vector<EGUIControlType>
                   controls;           ///<GUI controls of this element.
    mutable Block  control_location;   ///<The area the GUI controls occupy.
    const StyleCoW 
           indicator_style;    ///<The style to be used if we need to replace this with an indicator
    /** Returns the size of an indicator that is supposed to replace us considering `indicator_style`*/
    XY GetIndiactorSize() const {const double a = indicator_style.read().shadow.offset.second+indicator_style.read().line.LineWidth()*2; return indicator_size+XY(a,a);}

public:
    EDrawPassType    draw_pass; ///<Gives the Z-order position of this arc 
    FileLineColRange file_pos;  ///<The location of the element in the source file.

    explicit Element(Msc *m);
    /** Copy constructor, but does not copy comments*/
    Element(const Element&);
    virtual ~Element();
    void SetLineEnd(FileLineColRange l, bool f = true);
    void ExpandLineEnd(FileLineColRange l, bool f = true);
    virtual void AttachComment(CommandNote *cn);
    void CombineComments(Element *); //move comments to us
    virtual bool AddAttribute(const Attribute &);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    virtual void ShiftBy(double y);
    /** An area over which if the mouse hoovers, we highlight the Element*/
    const Area &GetAreaToSearch() const   
        {return area;};
    /** An area to highlight when the mouse moves over the Element*/
    const Contour &GetAreaToDraw() const  
        {return draw_is_different ? area_draw : area;}
    /** An area which shall not get covered by notes, if possible. */
    const Contour &GetAreaImportant() const     
        {return area_important;}
    /** An area to the edge of which notes made to this element will point to*/
    const Contour &GetAreaToNote() const  
        {return area_to_note.IsEmpty() ? area : area_to_note;}
    /** An second try for an area to the edge of which notes with an 'at' clause will point to*/
    const Contour &GetAreaToNote2() const {return area_to_note2;}
    /** Return the list of GUI controls associated with this Element*/
    const std::vector<EGUIControlType>& GetControls() const {return controls;}
    void LayoutComments(Canvas &canvas, AreaList *cover) {double l=0, r=0; LayoutCommentsHelper(canvas, cover, l, r);}
    virtual void LayoutCommentsHelper(Canvas &canvas, AreaList *cover, double &l, double &r);
    virtual void PostPosProcess(Canvas &);
    /** Register the labels of the element in chart->labelData (recursively called)*/
    virtual void RegisterLabels() {}
    /** Collect link information for ismap generation.*/
    virtual void CollectIsMapElements(Canvas &) {}
    virtual void RegisterCover(EDrawPassType pass);

    /** Return the location of our GUI controls*/
    const Block &GetControlLocation() const {return control_location;}
    void DrawControls(cairo_t*, double size) const;
    EGUIControlType WhichControl(const XY &xy);

    Block GetIndicatorCover(const XY &pos);
    void DrawIndicator(XY pos, Canvas *canvas);
};


#endif //TRACKABLE_H
