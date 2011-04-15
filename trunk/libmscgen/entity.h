#if !defined(ENTITY_H)
#define ENTITY_H

#include "trackable.h"
#include "style.h"
#include "contour_area.h"

class EntityStatusMap
{
public:
	typedef enum {SHOW_OFF, SHOW_ON, ACTIVE, ACTIVE_OFF} EStatus;
protected:
    DoubleMap<MscStyle> styleStatus;   //style of the entity line at and beyond a position
    DoubleMap<EStatus>  showStatus;    //tells if the entity is turned on or off or active
public:
    explicit EntityStatusMap(const MscStyle &def) : styleStatus(def), showStatus(SHOW_OFF) {}
    void ApplyStyle(double pos, const MscStyle &style) {styleStatus.Add(pos, style);}
    void ApplyStyle(Range pos, const MscStyle &style) {styleStatus.Add(pos, style);}
    void SetStatus(double pos, EStatus status) {showStatus.Set(pos, status);}
    const MscStyle &GetStyle(double pos) const {return *styleStatus.Get(pos);}
    EStatus GetStatus(double pos) const {return *showStatus.Get(pos);}
	double Till(double pos) const {return std::min(showStatus.Till(pos), styleStatus.Till(pos));}
};

class Msc;
class EntityDef;
typedef PtrList<EntityDef> EntityDefList;


class Entity
{
public:
    const string     name;     // Name of Entity as referenced in src file
    const string     label;    // Label of the entity (==name if none)
    const string     orig_label; //the text the user specified for label (for error msgs)
    const file_line  file_pos; //Where the entity was initially defined
    double           pos;      // 0 for the 1st, 1 for 2nd, etc. 1.5 for one in-between
    const double     pos_exp;  // position if all entities were expanded (for a->b->c sanity checking)
    unsigned         index;    // counts entities left to right
    EntityStatusMap  status;   // contains vertical line status & type & color
    MscStyle         running_style;  //Used during PostParse process to make EntityDef::style's fully specified
    double           maxwidth;       //Used during PostParse process to collect the maximum width of the entity
    bool             shown;          //Used during Height process to see if it is shown

    string           parent_name;    //tells if we are part of an entity group
    std::set<string> children_names; //if we are an entity group, tells who are within us
    const bool       collapsed;      //true if we are group, but show collapsed

    Entity(const string &n, const string &l, const string &ol, double p, double pe,
           const MscStyle &entity_style, const file_line &fp, bool coll);
    void AddChildrenList(const EntityDefList *children, Msc *chart);
    virtual ~Entity() {};
    string Print(int ident = 0) const;
};

class EntityList : public PtrList<Entity>
{
    public:
        explicit EntityList(bool r=true) :PtrList<Entity>(r) {}
        //Returns ::begin() if not found, assuming NoEntity is there
        PtrList<Entity>::iterator Find_by_Name(const string &name) {
            PtrList<Entity>::iterator i = PtrList<Entity>::begin();
            while (i!=PtrList<Entity>::end()) {
                if ((*i)->name == name) return i;
                i++;}
                return PtrList<Entity>::begin();
        }
        PtrList<Entity>::iterator Find_by_Ptr(const Entity *p) {
            PtrList<Entity>::iterator i = PtrList<Entity>::begin();
            while (i!=PtrList<Entity>::end()) {
                if (*i == p) return i;
                i++;}
                return PtrList<Entity>::end();
        }

        void SortByPos(void);
        void SortByPosExp(void);
};
typedef EntityList::const_iterator EIterator;


template <class T1, class T2, class T3> struct triplet
{
    typedef T1 first_type;
    typedef T2 second_type;
    typedef T3 third_type;

    T1 first;
    T2 second;
    T3 third;
//  triplet() : first(T1()), second(T2(), third(T3()) {}
//  triplet(const T1& x, const T2& y, const T3& z) : first(x), second(y), third(z) {}
//  template <class U, class V, class W>
//    triplet (const triplet<U,V,W> &p) : first(p.first), second(p.second), third(p.third) { }
};

class ArcBase;
typedef PtrList<ArcBase> ArcList;
class EntityDef;
typedef PtrList<EntityDef> EntityDefList;

/* Class allocated during parse */
//Each occurrence of an entity in an enity command allocates an EntityDef.
//In contrast, there is only one Entity object per entity
class EntityDef : public TrackableElement
{
public:
    //Store attribute values added in an entity command subsequent to declaration
    const string                   name;
    triplet<bool,string,file_line> label;
    file_line                      linenum_label_value; //pos of label text
    triplet<bool,double,file_line> pos;
    triplet<bool,string,file_line> rel;
    triplet<bool,bool,file_line>   collapsed;
    std::pair<bool,bool>           show;
    bool                           show_is_explicit; //"show" comes from user text

    EIterator                      itr;         //this is set during PostParse, points to the entity
    MscStyle                       style;       //this is finalized during PostParse
    Label                          parsed_label;//this is finalized during PostParse
	bool                           defining;    //true if this is the first entityDef for this entity = this entityDef created the entity (set in AddAttrList)
    bool                           shown;       //true if in CommandEntity::Height we decided to show this entity

    mutable EIterator left_ent, right_ent;      //for a grouped entity the left & right active entity (but in AllEntities)
    mutable double left_offset, right_offset;   //for a groupe entity the offset from them (of outer edge of contained entity)
    mutable Block  outer_edge;
    mutable double indicator_ypos_offset;       //-1 if no indicator to be shown
    explicit EntityDef(const char *s, Msc* chart);

    virtual bool AddAttribute(const Attribute&);
    EntityDefList* AddAttributeList(AttributeList *, const ArcList *children, file_line l);
    static void AttributeNames(Csh &csh);
    static bool AttributeValues(const std::string attr, Csh &csh);
    virtual string Print(int ident=0) const;

    double Width() const;
    Range Height(AreaList &cover, const EntityDefList &edl);
    void ShiftBy(double y) {TrackableElement::ShiftBy(y); outer_edge.Shift(XY(0,y));}
    virtual void PostPosProcess(double);
    void Draw();
};





#endif //ENTITY_H