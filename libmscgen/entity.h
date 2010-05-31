#if !defined(ENTITY_H)
#define ENTITY_H

#include "trackable.h"
#include "style.h"

class EntityStatus
{
public:
    MscLineAttr  line;
    bool         status;
    EntityStatus() {}
    EntityStatus(const MscLineAttr &l, bool s) : status(s) {line=l;}
    bool operator == (const EntityStatus &a)
        {return line==a.line && status==a.status;}
};

class EntityStatusMap
{
    protected:
        std::map<double, EntityStatus> storage;
        EntityStatus defaultStatus;
    public:
        void SetDefaultStatus(EntityStatus def)
            {defaultStatus=def;}
        void Set(double pos, EntityStatus status)
            {storage[pos] = status;}
        void SetRange(Range pos, EntityStatus status);
        void HideRange(Range pos)
            {SetRange(pos, EntityStatus(MscLineAttr(LINE_NONE, MscColorType(), 0, 0), true));}
        EntityStatus Get(double pos) const;
        double Till(double pos) const;
        string Print(int ident = 0);

};

class Msc;

class Entity
{
protected:
    Msc             *chart;
public:
    const string     name;     // Name of Entity as referenced in src file
    const string     label;    // Label of the entity (==name if none)
    double           pos;      // 0 for the 1st, 1 for 2nd, etc. 1.5 for one in-between
    unsigned         index;    // counts entities left to right
    MscStyle         style;
    EntityStatusMap  status;   // contains vertical line status & type & color

    Entity(const string &n, const string &l, double p, Msc *msc);
    virtual ~Entity() {};
    string Print(int ident = 0) const;
    double DrawHeight(double yPos, Geometry &g, bool draw, bool final);
    double Width();
};

class EntityList : public PtrList<Entity>
{
    public:
        explicit EntityList(bool r=true) :PtrList<Entity>(r) {}
        //Returns ::begin() if not found, assuming NoEntity is there
        PtrList<Entity>::iterator Find_by_Name(const string &name)
            {PtrList<Entity>::iterator i = PtrList<Entity>::begin();
             while (i!=PtrList<Entity>::end()) {
                if ((*i)->name == name) return i;
                i++;}
             return PtrList<Entity>::begin();}
        void SortByPos(void);
};
typedef EntityList::iterator EIterator;


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
        triplet<bool,double,file_line> pos;
        triplet<bool,string,file_line> rel;
        std::pair<bool,bool>           show;
        MscStyle                       style;

        explicit EntityDef(const char *s, Msc* chart);

        virtual EntityDef* AddAttributeList(AttributeList *, Msc *msc);
        virtual bool AddAttribute(const Attribute&, Msc *msc);
        virtual string Print(int ident=0) const;
};

typedef PtrList<EntityDef> EntityDefList;




#endif //ENTITY_H