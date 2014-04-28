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

/** @file entity.h The declaration of entities and related classes.
 * @ingroup libmscgen_files */
#if !defined(ENTITY_H)
#define ENTITY_H

#include "element.h"
#include "style.h"

/**Desribes a Shape component. 
 * This is either one vertex of the shape, or a section delimiter, or a 
 * hint or text box. */
struct ShapeElement
{
    enum Type
    {
        LINE_TO = 0,
        MOVE_TO = 1,
        CURVE_TO = 2,
        CLOSE_PATH = 3,
        HINT_AREA = 4,
        TEXT_AREA = 5,
        SECTION_BG = 6,
        SECTION_FG_FILL = 7,
        SECTION_FG_LINE = 8
    };

    Type action;
    static const char act_code[];
    double x, y;
    double x1, y1;
    double x2, y2;
    string str;
    ShapeElement(Type aa, double a = 0, double b = 0, double c = 0, double d = 0, double e = 0, double f = 0)
        : action(aa), x(a), y(b), x1(c), y1(d), x2(e), y2(f) { if (aa>=SECTION_BG && a==int(a) && a>=0 && a<=2) aa = Type(SECTION_BG + unsigned(a));}
    ShapeElement(Type aa, const char *s)
        : action(aa), str(s? s: "") {}
    static int GetNumArgs(Type t) { return t==LINE_TO||t==MOVE_TO ? 2 : t==CURVE_TO ? 6 : t==HINT_AREA || t==TEXT_AREA ? 4 : t==CLOSE_PATH ? 0 : 1; }
    static string ErrorMsg(Type t, int numargs);
    string Write() const;
};

/**Describes a Shape*/
class Shape
{
    Block max;
    Block label_pos;
    Block hint_pos;
    Path bg, fg_fill, fg_line;
    mutable bool cover_fresh;
    mutable Contour cover;
    Path  &GetSection(unsigned section) { return section==0 ? bg : section==1 ? fg_fill : fg_line; }
    unsigned current_section;
    std::vector<Edge> current_section_data;
    XY current_section_point;
public:
    const Path &GetSection(unsigned section) const { return section==0 ? bg : section==1 ? fg_fill : fg_line; }
    const std::string name;
    const FileLineCol definition_pos;
    const std::string url;
    const std::string info;
    Shape() : cover_fresh(true), current_section(1), name(), definition_pos(), url(), info() { max.MakeInvalid(); label_pos.MakeInvalid(); hint_pos.MakeInvalid(); }
    Shape(const std::string &n, const FileLineCol &l, const std::string &u, const std::string &i);
    Shape(const std::string &n, const FileLineCol &l, const std::string &u, const std::string &i, Shape &&s);
    bool IsEmpty() const { return bg.IsEmpty() && fg_fill.IsEmpty() && fg_line.IsEmpty(); }
    const Contour &GetCover() const;
    void Add(ShapeElement &&e);
    Block GetLabelPos(const Block &o) const;
    Block GetHintPos() const { return hint_pos; }
    Block GetMax() const { return max; }
    const string &GetURL() const { return url; }
    const string &GetInfo() const { return info; }
    void CairoPath(cairo_t *cr, unsigned section, const Block &o) const;
};

/** Describes a collection of shapes */
class ShapeCollection
{
    std::vector<Shape> shapes;
public:
    ShapeCollection() = default;
    ShapeCollection(const ShapeCollection &) = default;
    ShapeCollection(ShapeCollection &&s) : shapes(std::move(s.shapes)) {}
    operator bool() const { return shapes.size()>0; }
    const Shape &operator [](unsigned sh) const { return shapes[sh]; }
    ShapeCollection & operator = (const ShapeCollection &);
    ShapeCollection & operator = (ShapeCollection &&o) { swap(o); return *this; }
    void swap(ShapeCollection &s) { shapes.swap(s.shapes); }
    bool Add(const char *shape_text, MscError &error);
    bool Add(const string &name, const FileLineCol &pos, 
             const std::string &u, const std::string &i, 
             Shape *s, MscError &error);
    unsigned ShapeNum() const { return shapes.size(); }
    int GetShapeNo(const string&sh_name) const;
    void  AttributeValues(Csh &csh) const;
    std::vector<const string*> ShapeNames() const;
    std::vector<string> ShapeNames(const string &partial) const;
    void Draw(Canvas &canvas, unsigned sh, const Block &o, const LineAttr &line, const FillAttr &fill) const;
    Contour Cover(unsigned sh, const Block &o) const;
    const Shape * GetShape(unsigned shape) { if (shape>=shapes.size()) return NULL; return &shapes[shape]; }
};


/** Describes the status of an entity, like shown, hidden or active.
 * This essentially encapsulates an enum and add helpers.*/
class EEntityStatus {
public:
    enum E {
        SHOW_OFF,       ///<The entity is not active and is turned off.
        SHOW_ON,        ///<The entity is not active but is turned on.
        SHOW_ACTIVE_ON, ///<The entity is active but is turned off.
        SHOW_ACTIVE_OFF ///<The entity is noth active and is on.
    };
private:
    E status; ///<The actual value.
public:
    EEntityStatus(E e) : status(e) {}
    EEntityStatus() : status(SHOW_OFF) {}
    /** True if the entity is shown.*/
    bool IsOn() const {return status==SHOW_ON||status==SHOW_ACTIVE_ON;}
    /** True if the entity is active */
    bool IsActive() const {return status==SHOW_ACTIVE_OFF||status==SHOW_ACTIVE_ON;}
    /** Shows/hides the entity with keeping its activation state.*/
    EEntityStatus& Show(bool on) {status = on ? (IsOn() ? status : status==SHOW_OFF ? SHOW_ON : SHOW_ACTIVE_ON) : (!IsOn() ? status : status==SHOW_ON ? SHOW_OFF : SHOW_ACTIVE_OFF); return *this;}
    /** Activates/deactivates the entity with keeping its show/hide state.*/
    EEntityStatus& Activate(bool on) {status = on ? (IsActive() ? status : status==SHOW_OFF ? SHOW_ACTIVE_OFF : SHOW_ACTIVE_ON) : (!IsActive() ? status : status==SHOW_ACTIVE_OFF ? SHOW_OFF : SHOW_ON); return *this;}
    bool operator ==(EEntityStatus o) const {return status == o.status;}
    bool operator ==(E o) const {return status == o;}
    EEntityStatus& operator =(E o) {status=o; return *this;}
};

/** A map showing the status and style of an entity along a floating point axis (the y axis).*/
class EntityStatusMap
{
public:

protected:
    contour::DoubleMap<StyleCoW>      styleStatus; ///<Style of the entity line, fill, text, etc. at and beyond a position
    contour::DoubleMap<EEntityStatus> showStatus;  ///<Status of the entity at and beyond a position
public:
    /** Creates a map with the entity being inactive and off everywhere and having style `def`*/
    explicit EntityStatusMap(const StyleCoW &def) : styleStatus(def), showStatus(EEntityStatus::SHOW_OFF) {}
    /** Applies a style at `pos` (merging styles) and onwards up till the next existing change.*/
    void ApplyStyle(double pos, const StyleCoW &style) {styleStatus.Add(pos, style);}
    /** Applies a style within range `pos` (merging styles).*/
    void ApplyStyle(Range pos, const StyleCoW &style) {styleStatus.Add(pos, style);}
    /** Set the entity status at `pos` and onwards up till the next existing change.*/
    void SetStatus(double pos, EEntityStatus status) {showStatus.Set(pos, status);}
    /** Get the current style at `pos`*/
    const StyleCoW &GetStyle(double pos) const {return *styleStatus.Get(pos);}
    /** Get the current status at `pos`*/
    EEntityStatus GetStatus(double pos) const {return *showStatus.Get(pos);}
    /** Return the next change of status strictly after `pos`. */
    double ShowTill(double pos) const {return showStatus.Till(pos);}
    /** Return the next change of style strictly after `pos`. */
    double StyleTill(double pos) const {return styleStatus.Till(pos);}
    /** Return the previous change of status strictly before `pos`. */
    double ShowFrom(double pos) const {return showStatus.From(pos);}
    /** Return the previous change of style strictly before `pos`. */
    double StyleFrom(double pos) const {return styleStatus.From(pos);}
    /** Clear all content, making the entity having the default style and being off/inactive everywhere.*/
    void Reset() {styleStatus.clear(); showStatus.clear();}
};

class Msc;
class EntityApp;
typedef PtrList<EntityApp> EntityAppList;

/** A class describing an entity.
 * Each entity is allocated exactly one such object.*/
class Entity
{
public:
    const string      name;      ///<Name of entity as referenced in src file
    const string      label;     ///<Label of the entity (==name if none)
    const string      orig_label;///<The text the user specified for label (for error msgs)
    const FileLineCol file_pos;  ///<The location of the definition of this entity in the input file (the first character of its name)
    const int         shape;     ///<The shape we shall use when drawing the entity. -1 if none.
    const EArrowSize  shape_size;///<It size
    double            pos;       ///<The position of the entity with hscale=1. E.g., 0 for the 1st, 1 for 2nd, etc. 1.5 for one in-between
    const double      pos_exp;   ///<The position of the entity if all group entities were expanded (for a->b->c sanity checking)
    unsigned          index;     ///<The index of the entity, counting entities left to right
    EntityStatusMap   status;    ///<Contains vertical line status & type & color and on/off active/inactive status.
    StyleCoW          running_style;     ///<Used during PostParse process to make EntityApp::style's fully specified. Also used in AutoPaginate to gather the style for automatically inserted headers.
    EEntityStatus     running_shown;     ///<Used during PostParse process to see if it is shown/hidden active/inactive.
    EDrawPassType     running_draw_pass; ///<The running Z-order position of this arc during PostParse process
    double            maxwidth;          ///<Used during PostParse process to collect the maximum width of the entity

    string            parent_name;    ///<Empty if we are not part of an entity group, otherwise the name of the parent entity.
    std::set<string>  children_names; ///<If we are an entity group, tells who are within us
    const bool        collapsed;      ///<True if we are group, but show collapsed

    Entity(const string &n, const string &l, const string &ol, double p, double pe,
           const StyleCoW &entity_style, const FileLineCol &fp, bool coll, 
           int sh, EArrowSize sh_size);
    void AddChildrenList(const EntityAppList *children, Msc *chart);
    double GetRunningWidth(double activeEntitySize) const;
    string Print(int ident = 0) const;
};

/** A list of Entity object pointers.
 * This is used both as a store of entities (Msc::AllEntities) and as
 * a secondary list to Entity objects allocated elsewhere (Msc::ActiveEntities).*/
class EntityList : public PtrList<Entity>
{
public:
    /** Create a list. If `r` true then we will delete all added objects at destruction */
    explicit EntityList(bool r=true) :PtrList<Entity>(r) {}
    /** Find an entity by name.
      * If not found, returns begin(), assuming NoEntity is there*/
    PtrList<Entity>::iterator Find_by_Name(const string &name) {
        PtrList<Entity>::iterator i = PtrList<Entity>::begin();
        while (i!=PtrList<Entity>::end()) {
            if ((*i)->name == name) return i;
            i++;}
            return PtrList<Entity>::begin();
    }
    /** Find an entity by pointer.
      * If not found, returns end()*/
    PtrList<Entity>::iterator Find_by_Ptr(const Entity *p) {
        PtrList<Entity>::iterator i = PtrList<Entity>::begin();
        while (i!=PtrList<Entity>::end()) {
            if (*i == p) return i;
            i++;}
            return PtrList<Entity>::end();
    }
    /** Find an entity by pointer.
    * If not found, returns end()*/
    PtrList<Entity>::iterator Find_by_Index(unsigned index)
    {
        PtrList<Entity>::iterator i = PtrList<Entity>::begin();
        while (i!=PtrList<Entity>::end()) {
            if ((*i)->index == index) return i;
            i++;
        }
        return PtrList<Entity>::end();
    }
    /** Sort the list by increasing `pos`*/
    void SortByPos(void);
    /** Sort the list by increasing `pos_exp`*/
    void SortByPosExp(void);
};
/** An iterator to an Entity pointer in a list.
 * Contrary to its name it is not an enumeration. */
typedef EntityList::const_iterator EIterator;

/** A collection of bool values for each entity by name to see which is collapsed and which is not.*/
typedef std::map<std::string,bool> EntityCollapseCatalog;

/** A generic 3-element tuple*/
template <class T1, class T2, class T3> struct triplet
{
    typedef T1 first_type;
    typedef T2 second_type;
    typedef T3 third_type;

    T1 first;
    T2 second;
    T3 third;
//  triplet() : first(T1()), second(T2(), third(T3()) {}
    triplet(const T1& x, const T2& y, const T3& z) : first(x), second(y), third(z) {}
//  template <class U, class V, class W>
//    triplet (const triplet<U,V,W> &p) : first(p.first), second(p.second), third(p.third) { }
};

class ArcBase;
/** A list of arcs (ArcBase descendants) by pointer*/
typedef PtrList<ArcBase> ArcList;
class EntityApp;
/** A list of EntityApp objects by pointer*/
typedef PtrList<EntityApp> EntityAppList;

/** A list of EntityApp objects with potential associated CommandNotes.
 * This is used during parsing to build up CommandEntity objects. */
struct EntityAppHelper
{
    EntityAppList          entities;     ///<A list of entity definitions. 
    PtrList<CommandNote>   notes;        ///<A set of notes associated with one of the entities in `entities` member.
    std::list<std::string> note_targets; ///<The name of entities the notes in `notes` correspond to. Has as many members as `notes`.
    std::string            target;       ///<A name of an entity in `entity` to be the target for a subsequent note.
    /** Prepend a set of entities, and merge the list of notes.*/
    EntityAppHelper *Prepend(EntityAppHelper*edh) {if (edh) {entities.Prepend(&edh->entities); notes.Prepend(&edh->notes); note_targets.splice(note_targets.begin(), edh->note_targets);} return this;} //leave "target" as that of the latter edh
};

class ArcDirArrow;

/** Describes information for each appearance of an entity in the input.
 * Each time we mention an entity (not as part of an arc, but to change its attributes)
 * one EntityApp is created. 
 * There is a special EntityApp for each Entity though, the first one, the one which 
 * actually *defines* the Entity. Parsing always generates EntityApp objects and 
 * if we see that the Entity does not exist yet, we create one Entity object.
 * So this class have to be able contain all things and entity definition needs,
 * like label, grouping and so on, not just the things that may be changed later on.
 * It is a descendant of of Element because these metions can be
 * highlighted individually in the chart.
 *
 * The creation of this object goes as follows.
 * (Note that entities can be listed separated by commas, each may have attributes
 * they can also have child entities behind them in curly braces, plus they can be 
 * preceeded by one of `show`, `hide`, `activate` and `deactivate` keyword, which
 * is syntactic sugar to set the corresponding attribute in all entitydefs in the 
 * comma-separated list.)
 * - first we parse it learning its name (`entity_string` rule in language.yy)
 * - then we parse its associated attributes (rule `full_arcattrlist_with_label`)
 *   and a potential list of arcs (`braced_arclist`). All this is in the rule
 *   `entity`.
 * - In rule `entity` we create the EntityApp object (the corresponding Entity
 *   may not exist if we define the entity here.
 * - We call EntityApp::AddAttributeList() with the parsed attribute list and
 *   arclist (if any). This function will check if the associated arclist
 *   contains only CommandEntity or CommandNote arcs, fixes the notes
 *   applies the attribute list and creates the Entity object if needed.*/
class EntityApp : public Element
{
public:
    const string                     name;                ///<The name of the Entity
    triplet<bool,string,FileLineCol> label;               ///<The label specified at this mention of the entity (if any). `third` contains the position of the attribute (name).
    FileLineCol                      linenum_label_value; ///<Locatin of label text (attribute value) in the input file (if any). Only a minor difference to `label.third`.
    triplet<bool,double,FileLineCol> pos;                 ///<The `pos=` attribute if specified by the user. `third` contains the location of the attribute (name) in the input file.
    triplet<bool,string,FileLineCol> rel;                 ///<The `rel=` attribute if specified by the user. `third` contains the location of the attribute (name) in the input file.
    int                              attr_shape;          ///<The number of the shape we shall use to draw (from "shape" attr). -1 if none.
    FileLineCol                      attr_shape_pos;      ///<Location of the 'shape' attr.
    EArrowSize                       attr_shape_size;     ///<The size of the shape.
    triplet<bool,bool,FileLineCol>   collapsed;           ///<The `collapsed=` attribute if specified by the user. `third` contains the location of the attribute (name) in the input file.
    std::pair<bool,bool>             show;                ///<The `show=` attribute if specified by the user.
    triplet<bool,bool,FileLineCol>   active;              ///<The `active=` attribute if specified by the user. `third` contains the location of the attribute (name) in the input file.
    bool                             show_is_explicit;    ///<True if a show attribute was specified by the user. In this case a "show/hide" command/prefix will not override the `show` member.
    bool                             active_is_explicit;  ///<True if an active attribute was specified by the user. In this case an "activate/deactivate" command/prefix will not override the `active` member.
    bool                             centerlined;         ///<true if entity changes shall be aligned to the centerline target (the arrow above the entity, defined below).
    const ArcDirArrow*               centerline_target;   ///<If we need to activate at the centerline of an arrow, this is the one. Else NULL.

    EIterator                        itr;         ///<Points to the entity in Msc::AllEntities, set during PostParse.
    StyleCoW                         style;       ///<The complete style of the entity at this point. Finalized during PostParse taking unset attributes from running_style.
    Label                            parsed_label;///<The complete label with the right formatting. Finalized during PostParse.
    bool                             defining;    ///<True if this is the first EntityApp object for this entity = this EntityApp created the entity (set in AddAttrList).
    bool                             draw_heading;///True if we have to draw heading of this entity at this point. Set in PostParse.

    mutable EIterator left_ent;          ///<For a grouped entity the leftmost of its active child entities (pointer to Msc::AllEntities), else same as `itr` member.
    mutable EIterator right_ent;         ///<For a grouped entity the rightmost of its active child entities (pointer to Msc::AllEntities), else same as `itr` member.
    mutable double left_offset;          ///<For a grouped entity the offset from its left outer edge till the middle of its leftmost active child entity, else zero.
    mutable double right_offset;         ///<For a grouped entity the offset from its right outer edge till the middle of its rightmost active child entity, else zero.
    mutable Block  outer_edge;           ///<The outer edge of a heading we have to draw, set in Height().
    mutable double indicator_ypos_offset;///<The y position of the indicator of a collapsed group entity within the entity heading. -1 if no indicator to be shown.

    EntityApp(const char *s, Msc* chart);

    virtual bool AddAttribute(const Attribute&);
    EntityAppHelper* AddAttributeList(AttributeList *, ArcList *children, FileLineCol l);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    virtual string Print(int ident=0) const;
    void Combine(EntityApp *ed); 

    double Width() const;
    Range Height(Area &cover, const EntityAppList &edl);
    void AddAreaImportantWhenNotShowing();
    void ShiftBy(double y) {Element::ShiftBy(y); outer_edge.Shift(XY(0,y));}
    virtual void PostPosProcess(Canvas &);
    void Draw(Canvas &);
};



#endif //ENTITY_H