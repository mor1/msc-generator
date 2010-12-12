#if !defined(ENTITY_H)
#define ENTITY_H

#include <cfloat>
#include "trackable.h"
#include "style.h"

template <typename type>
void SetMapRange(std::map<double, type> &Map, Range pos, type value)
{
    if (pos.till <= pos.from) return;
    typename std::map<double, type>::iterator i = --Map.upper_bound(pos.till);
    if (i->first == pos.till) {
        Map.erase(Map.upper_bound(pos.from), i);
    } else {
        Map[pos.till] = i->second;
        Map.erase(Map.upper_bound(pos.from), ++i);
    }
    Map[pos.from] = value;
}

class EntityStatusMap
{
public:
	typedef enum {SHOW_OFF, SHOW_ON, ACTIVE, ACTIVE_OFF} EStatus;
protected:
    std::map<double, MscStyle>      styleStatus;   //style of the entity line at and beyond a position
    std::map<double, EStatus> showStatus;    //tells if the entity is turned on or off or active
    std::map<double, bool>          hideStatus;    //if true, the vline is hiden by e.g., a text
public:
    explicit EntityStatusMap(const MscStyle &def) {
        styleStatus[-DBL_MAX] = styleStatus[DBL_MAX] = def;
        hideStatus[-DBL_MAX]  = hideStatus[DBL_MAX]  = true;
        showStatus[-DBL_MAX]  = showStatus[DBL_MAX]  = SHOW_OFF;
    }
    void ApplyStyle(double pos, const MscStyle &style)
    {styleStatus[pos] = GetStyle(pos); styleStatus[pos] += style;}
    void ApplyStyleRange(Range pos, const MscStyle &style) {
        styleStatus[pos.till] = GetStyle(pos.till);
        styleStatus[pos.from] = GetStyle(pos.from) + style;
        for (std::map<double, MscStyle>::iterator i = styleStatus.upper_bound(pos.from); i!=styleStatus.find(pos.till); i++)
            i->second += style;
    }
    void SetStatus(double pos, EStatus status)
        {showStatus[pos] = status;}
    void HideRange(Range pos)
        {SetMapRange(hideStatus, pos, false);}
    const MscStyle &GetStyle(double pos) const
        {return (--styleStatus.upper_bound(pos))->second;}
    EStatus GetStatus(double pos) const
        {return (--showStatus.upper_bound(pos))->second;}
    bool GetHideStatus(double pos) const
        {return (--hideStatus.upper_bound(pos))->second;}
	double Till(double pos) const {
        return std::min(std::min(hideStatus.upper_bound(pos)->first,
                                 showStatus.upper_bound(pos)->first),
                       styleStatus.upper_bound(pos)->first);
    }
};

class Msc;

class Entity
{
protected:
    Msc             *chart;
public:
    const string     name;     // Name of Entity as referenced in src file
    const string     label;    // Label of the entity (==name if none)
    const string     orig_label; //the text the user specified for label (for error msgs)
    double           pos;      // 0 for the 1st, 1 for 2nd, etc. 1.5 for one in-between
    unsigned         index;    // counts entities left to right
    EntityStatusMap  status;   // contains vertical line status & type & color
    MscStyle         running_style;  //Used during PostParse process to make EntityDef::style's fully specified
    double           maxwidth;       //Used during PostParse process to collect the maximum width of the entity

    Entity(const string &n, const string &l, const string &ol, double p, Msc *msc);
    virtual ~Entity() {};
    string Print(int ident = 0) const;
    double DrawHeight(double yPos, Geometry &g, bool draw, bool final);
	double Width(const MscStyle &style);
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
        void SortByPos(void);
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

/* Class allocated during parse */
class EntityDef : public TrackableElement
{
    public:
        //Store attribute values added in an entity command subsequent to declaration
        const string                   name;
        triplet<bool,string,file_line> label;
        file_line                      linenum_label_value; //pos of label text
        triplet<bool,double,file_line> pos;
        triplet<bool,string,file_line> rel;
        std::pair<bool,bool>           show;
        MscStyle                       style;

        explicit EntityDef(const char *s, Msc* chart);

        virtual EntityDef* AddAttributeList(AttributeList *, Msc *msc);
        virtual bool AddAttribute(const Attribute&, Msc *msc);
        static void AttributeNames(Csh &csh);
        static bool AttributeValues(const std::string attr, Csh &csh);
        virtual string Print(int ident=0) const;
};

typedef PtrList<EntityDef> EntityDefList;




#endif //ENTITY_H