/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010,2011,2012 Zoltan Turanyi
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

#include <iostream>
#include <sstream>
#include <cassert>
#include <limits>
#include <cmath>
#include "msc.h"

using namespace std;

///////////////////////////////////////////////////////////////////////

//Inserts a horizontal space requirement.
//(Basically this says that entities with index e1 and e2 shall be
//at least xdist apart.)
//if e2 is DISTANCE_{LEFT,RIGHT} then we require that there shall be
//at least xdist space on the left/right side of entity e1. (If
//if e.g., the left the neighbouring entity wants xdist2 space on its
//right side, then eventually there will be xdist + xdist2 + XCoord(GAP)
//space between them.
//If there was a previous requirement for this (these) entities,
//we only increase if xdist is bigger.
void EntityDistanceMap::Insert(unsigned e1, int e2, double xdist)
{
    if (int(e1)==e2) return;
    if (e2==DISTANCE_LEFT) {
        std::map<unsigned, double>::iterator i = left.find(e1);
        if (i!=left.end() && i->second >= xdist) return;
        _ASSERT(e1>0);
        left[e1] = xdist;
    } else if (e2==DISTANCE_RIGHT) {
        std::map<unsigned, double>::iterator i = right.find(e1);
        if (i!=right.end() && i->second >= xdist) return;
        right[e1] = xdist;
    } else {
        IPair eip(e1, e2);
        std::map<IPair, double, IPairComp>::iterator i = pairs.find(eip);
        if (i!=pairs.end() && i->second >= xdist) return;
        pairs[eip] = xdist;
    }
}

//Query what was the (largest) requirement.
//e2 can again be DISTANCE_{LEFT,RIGHT}. we return 0 if there were no
//requirements for these entities.
double EntityDistanceMap::Query(unsigned e1, int e2) const
{
    if (int(e1)==e2) return 0;
    if (e2==DISTANCE_LEFT) {
        auto i = left.find(e1);
        if (i==left.end()) return 0;
        return i->second;
    }
    if (e2==DISTANCE_RIGHT) {
        auto i = right.find(e1);
        if (i==right.end()) return 0;
        return i->second;
    }
    IPair eip(e1, e2);
    auto i = pairs.find(eip);
    if (i==pairs.end()) return 0;
    return i->second;
}

//BoxSide distances are pair of distances between two neighbouring entities.
//One is a distance on the left side of the rightmost entity (say "e"); the other
//is a distance on the right side of the other entity (e+1). If nothing special happens,
//these two will be added and converted to a distance between e and e+1.
//However this distance requirement comes from inside a box and the box ends between
//e and e+1, we will have to space the side of the box appropriately and also make this wider
//by the thickness of the box line width and gaps.
//This is useful for arc elements (especially entity commands) that cover multiple disjoint
//areas (the shown entities) some of which can fall into a box around them, some of them
//can fall outside.
void EntityDistanceMap::InsertBoxSide(unsigned e, double l, double r)
{
    box_side[e].push_back(std::pair<double, double>(l, r));
}

//query the maximum boxside distance between e and e+1. left is true if we are interested
//in the max distance left of e+1 (and false if right from e)
std::pair<double, double> EntityDistanceMap::QueryBoxSide(unsigned e, bool left) const
{
    const auto l = box_side.find(e);
    if (l == box_side.end() || l->second.size()==0) return std::pair<double, double>(0,0);
    auto hit = l->second.begin();
    for (auto i = l->second.begin(); i!=l->second.end(); i++)
        if ((!left && i->second > hit->second) || (left && i->first > hit->first))
            hit = i;
    return *hit;
}

void EntityDistanceMap::CopyBoxSideToPair(double gap)
{
    for (auto k = box_side.begin(); k!=box_side.end(); k++) {
        double req = 0;
        for (auto l = k->second.begin(); l!=k->second.end(); l++)
            req = std::max(req, l->first + l->second);
        Insert(k->first, k->first+1, req + gap);
    }
}


//If there is X on the right side of e1 and Y on the e1+1 entity
//convert these to a distance of X+Y+gap between e1 and e1+1
void EntityDistanceMap::CombineLeftRightToPair_Sum(double gap)
{
    std::map<unsigned, double>::iterator i, j;
    for(i=right.begin(); i!=right.end(); ) {
        unsigned index = i->first;
        j = left.find(index+1);
        if (j == left.end()) {
            i++;
            continue;
        }
        double requirement = i->second + gap + j->second;
        right.erase(i++);
        left.erase(j);
        Insert(index, index+1, requirement);
    }
}

//If there is X on the right side of e1 and Y on the e1+1 entity
//convert these to a distance of max(X,Y)+gap between e1 and e1+1
//if, however the entity that we have selected via max() above was active,
//add active size to the distance
void EntityDistanceMap::CombineLeftRightToPair_Max(double gap, double act_size)
{
    std::map<unsigned, double>::iterator i, j;
    for(i=right.begin(); i!=right.end(); i++) {
        double my_right = i->second;
        if (was_activated.find(i->first) != was_activated.end())
            my_right += act_size;
        const unsigned index = i->first;
        double next_left = 0;
        j = left.find(index+1);
        if (j != left.end()) {
            next_left = j->second;
            if (was_activated.find(j->first) != was_activated.end())
                next_left += act_size;
            left.erase(j);
        }
        const double requirement = std::max(my_right, next_left) + gap; 
        Insert(index, index+1, requirement);
    }
    right.clear();
    //Add distances for whatever remained in "left"
    for(i=left.begin(); i!=left.end(); i++) {
        double my_left = i->second;
        if (was_activated.find(i->first) != was_activated.end())
            my_left += act_size;
        const unsigned index = i->first;
        if (index>0)  //ignore distances left of noentity
            Insert(index-1, index, my_left + gap);
    }
    left.clear();
}

//If there is X on the right side of e1 and nothing on the left side of e1+1
//or vice versa, then convert this side distance to X+gap between the two.
void EntityDistanceMap::CombineLeftRightToPair_Single(double gap)
{
    std::map<unsigned, double>::iterator i, j;
    for(i=right.begin(); i!=right.end(); ) {
        unsigned index = i->first;
        //skip if this is not single
        if (left.find(index+1) != left.end()) {
            i++;
            continue;
        }
        Insert(index, index+1, i->second + gap);
        right.erase(i++);
    }
    for(i=left.begin(); i!=left.end();) {
        unsigned index = i->first;
        //skip if this is not single
        if (right.find(index-1) != right.end()) {
            i++;
            continue;
        }
        Insert(index-1, index, i->second + gap);
        left.erase(i++);
    }
}

EntityDistanceMap &EntityDistanceMap::operator +=(const EntityDistanceMap &d)
{
    std::map<IPair, double, IPairComp>::const_iterator pi;
    std::map<unsigned, double>::const_iterator i;
    for(pi=d.pairs.begin(); pi!=d.pairs.end(); pi++)
        Insert(pi->first.first, pi->first.second, pi->second);
    for(i=d.left.begin(); i!=d.left.end(); i++)
        Insert(i->first, DISTANCE_LEFT, i->second);
    for(i=d.right.begin(); i!=d.right.end(); i++)
        Insert(i->first, DISTANCE_RIGHT, i->second);
    for(auto b = d.box_side.begin(); b!=d.box_side.end(); b++)
        box_side[b->first].insert(box_side[b->first].end(), b->second.begin(), b->second.end());
    return *this;
}

string EntityDistanceMap::Print()
{
    string s;
    std::map<unsigned, double>::iterator i;
    std::map<IPair, double, IPairComp>::const_iterator pi;
    s << "right"<<"\n";
    for(i=right.begin(); i!=right.end(); i++)
        s << "  " << i->first << " " << i->second << "\n";
    s << "left" << "\n";
    for(i=left.begin(); i!=left.end(); i++)
        s << "  " << i->first << " " << i->second << "\n";
    s << "pairs"<<"\n";
    for(pi=pairs.begin(); pi!=pairs.end(); pi++)
        s << "  (" << pi->first.first <<"," << pi->first.second<< ") " << pi->second << "\n";

    return s;
}


///////////////////////////////////////////////////////////////////////


//"Entities" member is responsible to delete ist contents
//"AutoGenEntities" is not, as its contents will be inserted into an
//CommandEntity in Msc::PostParseProcess()
Msc::Msc() :
    AllEntities(true), ActiveEntities(false), AutoGenEntities(false),
    Arcs(true), Notes(false), NoteBlockers(false), 
    total(0,0,0,0), drawing(0,0,0,0), 
    comments_right_side(0), copyrightTextHeight(0), headingSize(0)
{
    chartTailGap = 3;
    selfArrowYSize = 12;
    headingVGapAbove = 2;
    headingVGapBelow = 2;
    emphVGapOutside = 2;
    emphVGapInside = 2;
    arcVGapAbove = 0;
    arcVGapBelow = 3;
    discoVgap = 5;
    titleVgap = 10;
    subtitleVgap = 5;
    nudgeSize = 4;
    activeEntitySize = 14;
    compressGap = 2;
    hscaleAutoXGap = 5;
    sideNoteGap = 5;
    trackFrameWidth = 4;
    trackExpandBy = 2;

    pedantic=false;
    ignore_designs = false;

    current_file = Error.AddFile("[config]");
    
    //Add "plain" style 
    Designs["plain"].Plain();
    //Add global context, with "plain" design
    Contexts.push_back(Designs["plain"]);

    //Add virtual entities
    //NoEntity will be the one representing "NULL"
    //Arrows come between left_side and right_side.
    //Notes on the side come between left_note and left_side; and right_side & right_note
    NoEntity = new Entity(NONE_ENT_STR, NONE_ENT_STR, NONE_ENT_STR, -1002, -1002,
                          Contexts.back().styles["entity"], file_line(current_file, 0), false);
    LNote = new Entity(LNOTE_ENT_STR, LNOTE_ENT_STR, LNOTE_ENT_STR, -1001, -1001,
                       Contexts.back().styles["entity"], file_line(current_file, 0), false);
    LSide = new Entity(LSIDE_ENT_STR, LSIDE_ENT_STR, LSIDE_ENT_STR, -1000, -1000,
                       Contexts.back().styles["entity"], file_line(current_file, 0), false);
    RSide = new Entity(RSIDE_ENT_STR, RSIDE_ENT_STR, RSIDE_ENT_STR, 10000, 10000,
                       Contexts.back().styles["entity"], file_line(current_file, 0), false);
    RNote = new Entity(RNOTE_ENT_STR, RNOTE_ENT_STR, RNOTE_ENT_STR, 10001, 10001,
                       Contexts.back().styles["entity"], file_line(current_file, 0), false);
    
    AllEntities.Append(NoEntity);
    AllEntities.Append(LNote);
    AllEntities.Append(LSide);
    AllEntities.Append(RSide);
    AllEntities.Append(RNote);

    //This sets the global context to "plain" (redundant) 
    //and adds CommandEntities for lcomment.* and CommandBackground if needed.
    ArcBase *toadd;
    SetDesign(true, "plain", true, &toadd); 
    Arcs.Append(toadd);
}

Msc::~Msc() 
{
    //Proper deletion order
    Arcs.Empty();    //This must be before Notes, since TrackableElement::~ will use chart->Notes
}

//return value: 0 not found
//0: found, OK
//2: found, but is full, whereas should be partial
//3: found, but is partiall, whereas should be full
//also appends elements to Arcs !!! (e.g., background)
int Msc::SetDesign(bool full, const string&name, bool force, ArcBase **ret, const file_line_range &l)
{
    *ret = NULL;
    auto i = Designs.find(name);
    if (i==Designs.end())
        return 0;
    if (ignore_designs &&!force)
        return 1;
    Contexts.back() += i->second;
    ArcList list(true);
    if (!i->second.defBackground.IsEmpty())
        list.Append((new CommandNewBackground(this, i->second.defBackground))->AddAttributeList(NULL));
    if (!i->second.defCommentFill.IsEmpty() || !i->second.defCommentLine.IsEmpty()) {
        MscStyle s; //empty
        s.vline += i->second.defCommentLine;
        s.fill += i->second.defCommentFill;
        list.Append(CEForComments(s, l));
    }
    if (list.size())
        *ret = (new CommandArcList(this, &list))->AddAttributeList(NULL);
    if (full == i->second.is_full) return 1;
    return full ? 3 : 2;
}

string Msc::GetDesigns(bool full) const
{
    string retval;
    for (auto i = Designs.begin(); i!=Designs.end(); i++)
        if (i->second.is_full == full) {
            if (retval.length()) 
                retval.append(" ");
            retval.append(i->first);
        }
    return retval;
}

//Helper function. If the pos of *value is smaller (or larger) than i
//if one of the elements is .end() always the other is returned, different
//from operator < above (where .end() is smaller)
EIterator Msc::EntityMinMaxByPos(EIterator i, EIterator j, bool min) const
{
    if (*j==NoEntity) return i;
    if (*i==NoEntity) return j;
    if (min ^ ((*i)->pos < (*j)->pos))
        return j;
    else
        return i;
};

/* Finds an entity in AllEntities. If not found, it creates one */
EIterator Msc::FindAllocEntity(const char *e, file_line_range l)
{
    if (e==NULL || e[0] == 0) {
        _ASSERT (AllEntities.Find_by_Ptr(NoEntity) != AllEntities.end());
        return AllEntities.Find_by_Ptr(NoEntity);
    }
    EIterator ei = AllEntities.Find_by_Name(e);
    if (*ei == NoEntity) {
        if (pedantic)
            Error.Warning(l.start, "Unknown entity '" + string(e)
                        + "'. Assuming implicit definition.",
                        "This may be a mistyped entity name."
                        " Try turning 'pedantic' off to remove these messages.");
        //Entity *entity = new Entity(e, e, e, GetEntityMaxPos()+1, Contexts.back().styles["entity"], 
        //                            l.start, false);
        //AllEntities.Append(entity);
        EntityDef *ed = new EntityDef(e, this);
        ed->SetLineEnd(l);
        ed->AddAttributeList(NULL, NULL, l.start);
        ed->show.first = ed->show.second = true; //start turned on
        AutoGenEntities.Append(ed);
        ei = AllEntities.Find_by_Name(e);
    }
    return ei;
}

//ei is points to AllEntities. If grouped we return its leftmost or rightmost descendant
//which may be several groups below. If stop_at_collapsed is true, we do not go down to
//the descendants of a collapsed entity, if false, we treat as if all entities were expanded
EIterator Msc::FindLeftRightDescendant(EIterator ei, bool left, bool stop_at_collapsed)
{
    while ((*ei)->children_names.size() && !(stop_at_collapsed && (*ei)->collapsed)) {
        if ((*ei)->children_names.size()==0) break;
        EIterator tmp = AllEntities.Find_by_Name(*(*ei)->children_names.begin());
        for (auto i = ++(*ei)->children_names.begin(); i!=(*ei)->children_names.end(); i++)
            tmp = EntityMinMaxByPos(tmp, AllEntities.Find_by_Name(*i), left);
        ei = tmp;
    }
    return ei;
}

//Searches in AllEntities for the highest parent of i which is collapsed and return it
//If no parent is collapsed, it returns i.
//In essence it tells, which entity is shown on the chart for i
//If i is a grouped entity which is not collapsed, it will not show, but we return it.
//Virtual entities (NoEntity, leftside, leftnote, etc. will return themselves)
EIterator Msc::FindActiveParentEntity(EIterator i)  
{
    if ((*i)->parent_name.length() == 0) return i;
    EIterator j = AllEntities.Find_by_Name((*i)->parent_name);
    if (*j==NoEntity) return i;
    EIterator k = FindActiveParentEntity(j);
    if (j!=k) return k;  //An ancestor of our parent is collapsed: we show that guy
    if ((*j)->collapsed) return j;  //our parent would show and is collapsed: we show him
    return i; //our parent would show and is not collapsed: we show up
}

//If a parent of "ei" is collapsed, return that
//if "ei" is an expanded group entity with no parents collapsed, return the laft/rightmost descendant
//that shows
EIterator Msc::FindWhoIsShowingInsteadOf(EIterator ei, bool left)
{
    EIterator sub = FindActiveParentEntity(ei);
    if (sub != ei) return sub; //a parent of ours show instead of us
    return FindLeftRightDescendant(sub, left, true); //a child of us shows
}


string Msc::ListGroupedEntityChildren(EIterator ei) 
{
    if ((*ei)->children_names.size() == 0) return "'" + (*ei)->name + "'";
    string s = "'";
    for (auto i = (*ei)->children_names.begin(); i!=(*ei)->children_names.end(); i++)
        s.append(ListGroupedEntityChildren(AllEntities.Find_by_Name(*i))).append("', '");
    s.erase(s.length()-3);
    return s;
}

//Check if the entity is a grouped one. If so, return true and give an error msg
bool Msc::ErrorIfEntityGrouped(EIterator ei, file_line l) 
{
    if ((*ei)->children_names.size()==0) return false;
    Error.Error(l, "Group entity '" + (*ei)->name + "' cannot be used here.", 
                "Use one of its members (" + ListGroupedEntityChildren(ei) + ") instead."); 
    return true;
}

bool Msc::IsMyParentEntity(const string &children, const string &parent)
{
    if (children == parent) return false;
    const string myparent = (*AllEntities.Find_by_Name(children))->parent_name;
    if (myparent.length()==0) return false;
    if (myparent == parent) return true;
    return IsMyParentEntity(myparent, parent);
}

//Get the "pos" of the highest entity (without leftside, rightside and noentity)
double Msc::GetEntityMaxPos() const
{
    double ret = -1;  //first entity will be return + 1, which will be zero 
    for (auto i=AllEntities.begin(); i!=AllEntities.end(); i++)
        if (!IsVirtualEntity(*i) && ret < (*i)->pos)
            ret = (*i)->pos;
    return ret;
}
            
//Get the "pos_exp" of the highest entity (without leftside, rightside and noentity)
double Msc::GetEntityMaxPosExp() const
{
    double ret = -1;  //first entity will be return + 1, which will be zero 
    for (auto i=AllEntities.begin(); i!=AllEntities.end(); i++)
        if (!IsVirtualEntity(*i) && ret < (*i)->pos_exp)
            ret = (*i)->pos_exp;
    return ret;
}
            
ArcArrow *Msc::CreateArcArrow(MscArcType t, const char*s, file_line_range sl,
                              const char*d, bool fw, file_line_range dl)
{
    if (strcmp(s,d))
        return new ArcDirArrow(t, s, sl, d, dl, this, fw, Contexts.back().styles["arrow"]);
    MscStyle style = Contexts.back().styles["arrow"];
    style.text.Apply("\\pr");
    return new ArcSelfArrow(t, s, sl, this, style, selfArrowYSize);
}

ArcBigArrow *Msc::CreateArcBigArrow(const ArcBase *base)
{
    const ArcDirArrow *arrow = dynamic_cast<const ArcDirArrow *>(base);
    //We can only get ArcSelfArrow or ArcDirArrow here
    if (!arrow) {
        Error.Error(base->file_pos.start, "Big arrows cannot point back to the same entity. Ignoring it.");
        return NULL;
    }
    return new ArcBigArrow(*arrow, Contexts.back().styles["blockarrow"]);
}

void Msc::AddArcs(ArcList *a)
{
    Arcs.insert(Arcs.end(), a->begin(), a->end());
    a->clear();
    delete a;
}

CommandEntity *Msc::CEForComments(const MscStyle &s, const file_line_range &l)
{
    EntityDef *led = new EntityDef(LNOTE_ENT_STR, this);
    led->SetLineEnd(l);
    led->style += s;
    EntityDefHelper *ledh = led->AddAttributeList(NULL, NULL, file_line());
    EntityDef *red = new EntityDef(RNOTE_ENT_STR, this);
    red->SetLineEnd(l);
    red->style += s;
    EntityDefHelper *redh = red->AddAttributeList(NULL, NULL, file_line());
    redh->Prepend(ledh);
    delete ledh;
    CommandEntity *ce = new CommandEntity(redh, this, true);
    ce->AddAttributeList(NULL);
    return ce;
}

ArcBase *Msc::AddAttribute(const Attribute &a)
{
    //Chart options cannot be styles
    _ASSERT(a.type != MSC_ATTR_STYLE);

    if (a.Is("msc") || a.Is("msc+")) {
        const bool full = a.Is("msc");
        if (!a.CheckType(MSC_ATTR_STRING, Error)) return NULL;
        const file_line_range line(a.linenum_attr.start, a.linenum_value.end);
        ArcBase *ret;
        switch (SetDesign(full, a.value, false, &ret, line)) { 
        case 0:
            Error.Error(a, true, "Unknown chart design: '" + a.value +
                        "'. Ignoring design selection.",
                        "Available designs are: " + GetDesigns(full) +".");
            break;
        case 2:
            Error.Warning(a, true, "Use of '+=' to set a full design.", "Use 'msc = " + a.value + "' to suppress this warning.");
            break;
        case 3:
            Error.Warning(a, true, "Use of '=' to apply a partial design.", "Use 'msc += " + a.value + "' to suppress this warning.");
        case 1:
            break;
        default:
            _ASSERT(0);
            break;
        }
        return ret;
    }
    if (a.Is("hscale")) {
        if (a.type == MSC_ATTR_NUMBER && a.number>=0.01 && a.number <= 100) {
            Contexts.back().hscale.first = true;
            Contexts.back().hscale.second = a.number;
            return NULL;
        } else if (a.type == MSC_ATTR_STRING && a.value == "auto") {
            Contexts.back().hscale.first = true;
            Contexts.back().hscale.second = -1;
            return NULL;
        }
        a.InvalidValueError("0.01..100' or 'auto", Error);
        return NULL;
    }
    if (a.Is("compress")) {
        if (!a.CheckType(MSC_ATTR_BOOL, Error)) return NULL;
        Contexts.back().compress.first = true;
        Contexts.back().compress.second = a.yes;
        return NULL;
    }
    if (a.Is("indicator")) {
        if (!a.CheckType(MSC_ATTR_BOOL, Error)) return NULL;
        Contexts.back().indicator.first = true;
        Contexts.back().indicator.second = a.yes;
        return NULL;
    }
    if (a.StartsWith("text")) {
        Contexts.back().text.AddAttribute(a, this, STYLE_OPTION); //generates error if needed
        return NULL;
    }
    if (a.Is("numbering")) {
        if (!a.CheckType(MSC_ATTR_BOOL, Error)) return NULL;
        Contexts.back().numbering.first=true;
        Contexts.back().numbering.second = a.yes;
        return NULL;
    }
    if (a.Is("numbering.pre")) {
        Contexts.back().numberingStyle.pre.first = true;
        Contexts.back().numberingStyle.pre.second = a.value;
        StringFormat::ExpandReferences(Contexts.back().numberingStyle.pre.second, this,
                                          a.linenum_value.start, NULL,
                                          true, StringFormat::LABEL);
        return NULL;
    }
    if (a.Is("numbering.post")) {
        Contexts.back().numberingStyle.post.first = true;
        Contexts.back().numberingStyle.post.second = a.value;
        StringFormat::ExpandReferences(Contexts.back().numberingStyle.post.second, this,
                                          a.linenum_value.start, NULL,
                                          true, StringFormat::LABEL);
        return NULL;
    }
    if (a.Is("numbering.append")) {
        std::vector<NumberingStyleFragment> nsfs;
        if (NumberingStyleFragment::Parse(this, a.linenum_value.start, a.value.c_str(), nsfs))
            Contexts.back().numberingStyle.Push(nsfs);
        return NULL;
    }
    if (a.Is("numbering.format")) {
        std::vector<NumberingStyleFragment> nsfs;
        if (NumberingStyleFragment::Parse(this, a.linenum_value.start, a.value.c_str(), nsfs)) {
            int off = Contexts.back().numberingStyle.Apply(nsfs);
            if (off > 0) {
                string msg = "Numbering here is ";
                msg << off << " levels deep, and you specified more (" << nsfs.size();
                msg << ") levels of formatting. Ignoring option.";
                Error.Error(a, true, msg);
            }
        }
        return NULL;
    }

    if (a.Is("pedantic")) {
        if (!a.CheckType(MSC_ATTR_BOOL, Error)) return NULL;
        pedantic = a.yes;
        return NULL;
    }
    if (a.Is("angle")) {
        if (!a.EnsureNotClear(Error, STYLE_ARC)) return NULL;
        if (!a.CheckType(MSC_ATTR_NUMBER, Error)) return NULL;
        if (a.number<0 || a.number>45) {
            string x;
            if (a.number<0) x = "0";
            if (a.number>45) x = "45";
            Error.Warning(a, true, "Using " + x + " degrees instead of the specified value.",
                "The slant angle must be between 0 and 45 degrees.");
            if (a.number<0) Contexts.back().slant_angle.second = 0;
            else if (a.number>45) Contexts.back().slant_angle.second = 45;
        } else 
            Contexts.back().slant_angle.second = a.number;
        Contexts.back().slant_angle.first = true;
        return NULL;
    }
    if (a.StartsWith("background")) {
        MscFillAttr fill;
        fill.Empty();
        if (fill.AddAttribute(a, this, STYLE_OPTION)) { //generates error if needed
            Contexts.back().defBackground += fill;
            return (new CommandNewBackground(this, fill))->AddAttributeList(NULL);
        }
        return NULL;
    }
    if (a.StartsWith("comment")) {
        if (CaseInsensitiveBeginsWith(a.name.substr(8), "line") ||
            CaseInsensitiveBeginsWith(a.name.substr(8), "fill")) {
            const bool line = CaseInsensitiveBeginsWith(a.name.substr(8), "line");
            MscStyle toadd; //empty
            bool OK;
            if (line) {
                OK = toadd.line.AddAttribute(a, this, STYLE_OPTION); //generates errors if needed
                if (OK) std::swap(toadd.line, toadd.vline); //option shall be stored in vline
            } else
                OK = toadd.fill.AddAttribute(a, this, STYLE_OPTION); //generates errors if needed
            if (OK) {
                Contexts.back().defCommentFill += toadd.fill;
                Contexts.back().defCommentLine += toadd.vline;
                return CEForComments(toadd, file_line_range(a.linenum_attr.start, a.linenum_value.end));
            }
            //fallthrough till error if not "OK"
        }
    }
    
    string ss;
    Error.Error(a, false, "Option '" + a.name + "' not recognized. Ignoring it.");
    return NULL;
}

//Add an attribute only if it can be part of a design. Others trigger error.
//This is called when a design definition is in progress.
bool Msc::AddDesignAttribute(const Attribute &a)
{
    if (a.Is("numbering.append")) 
        goto error;
    if (a.Is("numbering.format")) {
        std::vector<NumberingStyleFragment> nsfs;
        if (NumberingStyleFragment::Parse(this, a.linenum_value.start, a.value.c_str(), nsfs)) {
            int off = Contexts.back().numberingStyle.Apply(nsfs);
            if (off > 0) {
                string msg = "Only the format of the top level number can be set as part of the design definition.";
                msg << " Ignoring option.";
                Error.Error(a, true, msg);
            }
        }
        return true;
    }
    if (a.StartsWith("numbering") || a.Is("compress") || a.Is("hscale") || a.Is("msc") || a.Is("msc+") ||
        a.StartsWith("text") || a.StartsWith("comment") || a.StartsWith("background")) {
        ArcBase *ret = AddAttribute(a);
        if (ret)
            delete ret;
        return true;
    }
error:
    Error.Error(a, false, "Cannot set option '" + a.name +
                  "' as part of a design definition. Ignoring it.");
    return false;
}

void Msc::AttributeNames(Csh &csh, bool designOnly)
{
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "msc", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "hscale", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "compress", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "numbering", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "numbering.pre", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "numbering.post", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "numbering.format", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "numbering.append", HINT_ATTR_NAME));
    StringFormat::AttributeNames(csh);
    if (designOnly) return;
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "pedantic", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "background.color", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "background.color2", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "background.gradient", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "lcomment.line.color", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "lcomment.line.type", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "lcomment.line.width", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "lcomment.fill.color", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "lcomment.fill.color2", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "lcomment.fill.gradient", HINT_ATTR_NAME));
    //csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "lcomment.line.radius", HINT_ATTR_NAME));
    //csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "lcomment.line.corner", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "rcomment.line.color", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "rcomment.line.type", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "rcomment.line.width", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "rcomment.fill.color", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "rcomment.fill.color2", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "rcomment.fill.gradient", HINT_ATTR_NAME));
    //csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "rnote.line.radius", HINT_ATTR_NAME));
    //csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "rnote.line.corner", HINT_ATTR_NAME));
}

bool Msc::AttributeValues(const std::string attr, Csh &csh)
{
    if (CaseInsensitiveEqual(attr,"msc")) {
        csh.AddDesignsToHints(true);
        return true;
    }
    if (CaseInsensitiveEqual(attr,"msc+")) {
        csh.AddDesignsToHints(false);
        return true;
    }
    if (CaseInsensitiveEqual(attr,"hscale")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number>", HINT_ATTR_VALUE, false));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE) + "auto", HINT_ATTR_VALUE));
        return true;
    }
    if (CaseInsensitiveEqual(attr,"compress") ||
        CaseInsensitiveEqual(attr,"numbering") ||
        CaseInsensitiveEqual(attr,"pednatic")) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE) + "yes", HINT_ATTR_VALUE, true, CshHintGraphicCallbackForYesNo, CshHintGraphicParam(1)));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE) + "no", HINT_ATTR_VALUE, true, CshHintGraphicCallbackForYesNo, CshHintGraphicParam(0)));
        return true;
    }
    if (CaseInsensitiveBeginsWith(attr, "text"))
        return StringFormat::AttributeValues(attr, csh);
    if (CaseInsensitiveBeginsWith(attr, "lcomment.line") ||
        CaseInsensitiveBeginsWith(attr, "rcomment.line"))
        return MscLineAttr::AttributeValues(attr, csh);
    if (CaseInsensitiveBeginsWith(attr, "lcomment.fill") ||
        CaseInsensitiveBeginsWith(attr, "rcomment.fill"))
        return MscFillAttr::AttributeValues(attr, csh);

    if (CaseInsensitiveBeginsWith(attr,"background")) {
        MscFillAttr::AttributeValues(attr, csh);
        return true;
    }
    if (CaseInsensitiveEqual(attr,"numbering.pre")||
        CaseInsensitiveEqual(attr,"numbering.post")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<\"text\">", HINT_ATTR_VALUE, false));
        return true;
    }
    if (CaseInsensitiveEqual(attr,"numbering.format")||
        CaseInsensitiveEqual(attr,"numbering.append")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<\"numbering format\">", HINT_ATTR_VALUE, false));
        return true;
    }
    return false;
}


void Msc::PushContext(bool empty)
{
    if (empty) {
        Contexts.push_back(Context());
    } else
        Contexts.push_back(Contexts.back());
}

ArcBase *Msc::PopContext()
{
    if (Contexts.size()<2) return NULL;
    const bool full = Contexts.back().is_full;
    const size_t old_size = full ? Contexts.back().numberingStyle.Size() : 0;
    Contexts.pop_back();
    //if numbering continues with the same amount of levels (or if this was a partial
    //context used to specify a design), no action will be needed
    //in PostParseProcess, so we do not generate any Command arcs.
    if (!full || old_size == Contexts.back().numberingStyle.Size())
        return NULL;
    //if number of levels is less in the outer context, we will need to trim the Numbering list
    //during PostParseProcess after processing all arcs in the inner context, so we insert
    //a CommandNumbering, which first trims the Numbering list to the new size and then increments
    //the last number, so after 1.2.2 comes 1.3
    if (old_size > Contexts.back().numberingStyle.Size())
        return new CommandNumbering(this, CommandNumbering::INCREMENT, Contexts.back().numberingStyle.Size());
    //We should never get here, but if the length is increasing, we just expand the Numbering list
    return new CommandNumbering(this, CommandNumbering::SIZE, Contexts.back().numberingStyle.Size());
}

void Msc::ParseText(const char *input, const char *filename)
{
    current_file = Error.AddFile(filename);
    if (strlen(input) > std::numeric_limits<unsigned>::max())
        Error.Error(file_line(), "Input text is longer than 4Gbyte. Bailing out.");
    else 
        MscParse(*this, input, (unsigned)strlen(input));
}

MscDirType Msc::GetTouchedEntitiesArcList(const ArcList &al, EntityList &el, MscDirType dir) const
{
    for (auto i = al.begin(); i!=al.end(); i++) {
        EntityList el2(false);
        MscDirType dir2 = (*i)->GetToucedEntities(el2);
        //update combined direction
        switch (dir2) {
        case MSC_DIR_BIDIR:
            dir = MSC_DIR_BIDIR; 
            break;
        case MSC_DIR_LEFT:
        case MSC_DIR_RIGHT:
            if (dir == MSC_DIR_INDETERMINATE) dir = dir2;
            else if (dir != dir2) dir = MSC_DIR_BIDIR;
            break;
        case MSC_DIR_INDETERMINATE:
            break;
        }
        //merge the two lists
        for (auto ei2 = el2.begin(); ei2!=el2.end(); ei2++) 
            if (el.Find_by_Ptr(*ei2) == el.end())
                el.Append(*ei2);
    }
    return dir;
}

string Msc::Print(int ident) const
{
    string s = AllEntities.Print(ident).append("\n");
    s.append(Arcs.Print(ident)).append("\n");
    return s;
}

void Msc::PostParseProcessArcList(MscCanvas &canvas, bool hide, ArcList &arcs, bool resetiterators,
                                  EIterator &left, EIterator &right,
                                  Numbering &number, bool top_level, TrackableElement **target)
{
    if (arcs.size()==0) return;
    for (ArcList::iterator i = arcs.begin(); i != arcs.end(); /*none*/) {
        //Splice in CommandArcList members
        CommandArcList *al = dynamic_cast<CommandArcList *>(*i);
        if (al) {
            al->MoveContent(arcs, i);
            //al now empty
            delete al;
            arcs.erase(i++);  //we will also check the content of al for further CommandArcLists
            continue;
        } else 
            i++;
    }
    //If a CommandNote is immediately after an CommandEntity, take it out 
    //and temporarily store it in the CommandEntity (for re-insertation further below)
    for (ArcList::iterator i = ++arcs.begin(); i != arcs.end(); /*none*/) {
        ArcList::iterator prev = i; prev--;
        CommandEntity *ce = dynamic_cast<CommandEntity *>(*prev);
        CommandNote *cn = dynamic_cast<CommandNote *>(*i);
        if (ce == NULL || cn == NULL) {
            i++;
            continue;
        }
        ce->TmpStoreNote(cn);
        arcs.erase(i++);
    }

    for (ArcList::iterator i = arcs.begin(); i != arcs.end(); /*none*/) {
        if (resetiterators) {
            right = left = AllEntities.Find_by_Ptr(NoEntity);
            _ASSERT (left != AllEntities.end());
        }
        //Combine subsequent CommandEntities
        CommandEntity * const ce = dynamic_cast<CommandEntity *>(*i);
        while (ce) {
            ArcList::iterator j = i;
            j++;
            if (j==arcs.end()) break;
            CommandEntity * const ce2 = dynamic_cast<CommandEntity *>(*j);
            if (!ce2 || ce->internally_defined != ce2->internally_defined) break;
            ce->Combine(ce2);
            delete ce2;
            arcs.erase(j);
            //i remains at this very same CommandEntity!
        }
        TrackableElement * const old_target = *target;
        ArcBase *replace = (*i)->PostParseProcess(canvas, hide, left, right, number, top_level, target);
        //if the new target is somewhere inside "i" (or is exactly == to "i")
        //NOTE: *target is never set to NULL, only to DELETE_NOTE or to an Arc
        if (*target != old_target && replace != (*i)) {
            //If we remove an arc that could have been noted, we set the target to DELETE_NOTE, as well
            if (replace == NULL) 
                *target = DELETE_NOTE;
            //if we replace to a different Arc, redirect notes to that
            else if (replace->CanBeNoted())
                *target = replace;
            else 
                *target = old_target;
        }
        //Do not add an ArcIndicator, if previous thing was also an ArcIndicator on the same entity
        ArcIndicator *ai = dynamic_cast<ArcIndicator*>(replace);
        if (ai && i!=arcs.begin()) {
            ArcIndicator *ai2 = dynamic_cast<ArcIndicator*>(*--ArcList::iterator(i));
            if (ai2 && ai2->Combine(ai)) {
                if (*target == replace) //redirect note to previous indicator
                    *target = ai2;
                delete replace;
                replace = NULL;
            }
        }

        //Ok, reinsert temporarily stored notes from commandentities (if any)
        CommandEntity * const cee = dynamic_cast<CommandEntity *>(replace);
        if (cee) 
            cee->ReinsertTmpStoredNotes(arcs, i);
        
        if (replace == *i) i++;
        else {
            delete *i;
            if (replace == NULL) arcs.erase(i++);
            else {
                CommandArcList *al = dynamic_cast<CommandArcList *>(replace);
                if (al == NULL)
                    (*i++) = replace;
                else {
                    auto j = i;
                    i++; //next element to PostParseProcess
                    al->MoveContent(arcs, j);  //this content was ppp'd
                    //al now empty
                    delete al;
                    arcs.erase(j);  //delete replaced element
                }
            }
        }
    }
}

void Msc::PostParseProcess(MscCanvas &canvas)
{
    //remove those entities from "force_entity_collapse" which are not defined as entities
    for (auto i = force_entity_collapse.begin(); i!=force_entity_collapse.end(); /*nope*/)
        if (*AllEntities.Find_by_Name(i->first) == NoEntity)
            force_entity_collapse.erase(i++);
        else i++;

    //Sort the defined entities as will be displayed from left to right
    AllEntities.SortByPos();

    //Now create a list of active Entities
    //Note that virtual entities will be added
    ActiveEntities.clear();
    for (auto i = AllEntities.begin(); i!=AllEntities.end(); i++) {
        const EIterator j = FindActiveParentEntity(i);
        if (i==j && (*i)->children_names.size() && !(*i)->collapsed) continue;
        if (ActiveEntities.size()==0) 
            ActiveEntities.Append(*j);  //first active entity, likely be NoEntity
        else if (*ActiveEntities.Find_by_Name((*j)->name) == NoEntity)
            ActiveEntities.Append(*j);  //a new active entity, not yet added
        else if ((*ActiveEntities.rbegin()) != *j) {
            //already added but not as last one
            Error.Error((*j)->file_pos, "Entities grouped into entity '" + (*j)->name +"' are not consecutive.",
                                        "May result in unintended consequences.");
            Error.Error((*i)->file_pos, (*j)->file_pos, "Entity '" + (*i)->name, "' may be one outlier.");
        }
    }   

    //Set index field in Entities
    unsigned index = 0;
    for (EIterator temp = ActiveEntities.begin(); temp!=ActiveEntities.end(); temp++, index++)
        (*temp)->index = index;

    //Find the first real entity
    EIterator tmp = ActiveEntities.begin();
    while (tmp != ActiveEntities.end() && IsVirtualEntity(*tmp))
        tmp++;

    //Ensure that leftmost real entity pos == 2*MARGIN
    double rightmost = 0;
    if (tmp != ActiveEntities.end()) {
        double leftmost = (*tmp)->pos - 2*MARGIN;
        for  (EIterator i = AllEntities.begin(); i != AllEntities.end(); i++) 
            (*i)->pos -= leftmost;
        //Find rightmost entity's pos
        for  (EIterator i = ActiveEntities.begin(); i != ActiveEntities.end(); i++) {
            if (IsVirtualEntity(*i)) continue;
            if (rightmost < (*i)->pos)
                rightmost = (*i)->pos;
        }
    } else {
        //if there are no real entities, just noentity, lside & rside, set this wide
        //so that copyright banner fits
        rightmost = 3*MARGIN;
    }
    //Set the position of the virtual side entities & resort
    const_cast<double&>(LNote->pos) = 0;
    const_cast<double&>(LSide->pos) = MARGIN;
    const_cast<double&>(RSide->pos) = rightmost + MARGIN;
    const_cast<double&>(RNote->pos) = rightmost + MARGIN + MARGIN;
    ActiveEntities.SortByPos();

    if (Arcs.size()==0) return;

    if (AutoGenEntities.size()) {
        //Add the Auto generated entities to the list of entities
        //Add them to the the first arc if it is an EntityCommand,
        //Otherwise, generate a new entity command as first arc
        ArcList::iterator i = Arcs.begin();
        if ((*i)->type != MSC_COMMAND_ENTITY) {
            CommandEntity *ce = new CommandEntity(new EntityDefHelper, this, false);
            ce->AddAttributeList(NULL);
            i = Arcs.insert(i, ce);
        }
        dynamic_cast<CommandEntity*>(*i)->AppendToEntities(AutoGenEntities);
    }
    //Set all entity's shown to false, to avoid accidentally showing them via (heading;) before definition
    for (auto i = ActiveEntities.begin(); i!=ActiveEntities.end(); i++)
        (*i)->running_shown = EEntityStatus::SHOW_OFF; //not shown not active

    //Traverse Arc tree and perform post-parse processing
    Numbering number; //starts at a single level from 1
    EIterator dummy1, dummy2;
    dummy2 = dummy1 = AllEntities.Find_by_Ptr(NoEntity);
    _ASSERT(dummy1 != AllEntities.end());
    TrackableElement *note_target = NULL;
    PostParseProcessArcList(canvas, false, Arcs, true, dummy1, dummy2, number, true, &note_target);
}

void Msc::DrawEntityLines(MscCanvas &canvas, double y, double height,
                          EIterator from, EIterator to)
{
    //No checking of iterators!! Call with caution
    //"to" is not included!!
    canvas.ClipInverse(HideELinesHere);
    double act_size = canvas.HasImprecisePositioning() ? floor(activeEntitySize/2) : activeEntitySize/2;
    while(from != to) {
        const EntityStatusMap & status= (*from)->status;
        XY up(XCoord(from), y);
        XY down(up.x, y);
        const double till = y+height;
        for (/*none*/; up.y < till; up.y = down.y) {
            const double show_till = status.ShowTill(up.y);
            const double style_till = status.StyleTill(up.y);
            const MscStyle &style = status.GetStyle(up.y);
            down.y = std::min(std::min(show_till, style_till), till);
            //we are turned off below -> do nothing
            if (!status.GetStatus(up.y).IsOn())
                continue;
            if (status.GetStatus(up.y).IsActive()) {
                const double show_from = status.ShowFrom(up.y);
                Block outer_edge;
                //if we may start an active rectangle here...
                if (!status.GetStatus(show_from).IsActive() || !status.GetStatus(show_from).IsOn()) 
                    outer_edge.y.from = up.y;
                //...or an active rectangle may have started earlier
                else
                    outer_edge.y.from = std::max(show_from, 0.);
                outer_edge.y.till = std::min(show_till, total.y.till);
                outer_edge.x.from = up.x - act_size; 
                outer_edge.x.till = up.x + act_size;
                Block clip(total);
                bool doClip = false;
                if (outer_edge.y.from < up.y) {
                    clip.y.from = up.y;
                    doClip = true;
                }
                if (outer_edge.y.till > down.y) {
                    clip.y.till = down.y;
                    doClip = true;
                }
                outer_edge.Expand(-style.vline.LineWidth()/2);  //From now on this is the midpoint of the line, as it should be
               
                if (doClip)
                    canvas.Clip(clip);
                canvas.Fill(style.vline.CreateRectangle_ForFill(outer_edge), style.vfill);
                canvas.Line(style.vline.CreateRectangle_Midline(outer_edge), style.vline);
                if (doClip)
                    canvas.UnClip();
            } else {
                const double offset = 0; //canvas->HasImprecisePositioning() ? 0 : fmod_negative_safe(style.vline.width.second/2, 1.);
                const XY start = up+XY(offset,0);
                //last param is dash_offset. Cairo falls back to image surface if this is not zero ???
                canvas.Line(start, down+XY(offset,0), style.vline); 
            }
        }
        from++;
    }
    canvas.UnClip();
}

void Msc::WidthArcList(MscCanvas &canvas, ArcList &arcs, EntityDistanceMap &distances)
{
    //Indicate entities active and showing already at the beginning of the list
    //(this will be updated with entities activated later)
    //(so that we can calcualte with the width of their entityline)
    for (auto i = ActiveEntities.begin(); i!=ActiveEntities.end(); i++) 
        if ((*i)->running_shown == EEntityStatus::SHOW_ACTIVE_ON) 
            distances.was_activated.insert((*i)->index);
    for (ArcList::iterator i = arcs.begin();i!=arcs.end(); i++) 
        (*i)->Width(canvas, distances);
}

//Places a full list of elements starting at y position==0
//Calls Height() for each element (recursively) and takes "compress" and "parallel" into account
//We always place each element on an integer coordinates
//returns the total height of the list and its coverage in "cover"
double Msc::HeightArcList(MscCanvas &canvas, ArcList::iterator from, ArcList::iterator to, AreaList &cover, bool reflow)
{
    cover.clear();
    double y = 0;              //vertical position of the current element
    double y_upper_limit = 0;  //we will never shift compress higher than this runnning value
                               //(any element marked with "parallel" will set this to its top)
    double y_bottom = 0;       //the highest element bottom we have seen 
                               //(will be returned, not always that of the last element)
    bool previous_was_parallel = false;
    bool had_parallel_above = false;
    //Zero-height arcs shall be positioned to the same place
    //as the first non-zero height arc below them (so that
    //if that arc is compressed up, they are not _below_
    //that following arc. So we store what was the last non-zero
    //height arc so we can go back and adjust the zero height ones
    ArcList::iterator first_zero_height = to;

    for (ArcList::iterator i = from; i!=to; i++) {
        AreaList arc_cover;
        double h = (*i)->Height(canvas, arc_cover, reflow);

        //increase h, if arc_cover.Expand() (in "Height()") pushed outer boundary. This ensures that we
        //maintain at least compressGap/2 amount of space between elements even without compress
        h = std::max(h, arc_cover.GetBoundingBox().y.till);
        double touchpoint = y;
        if ((*i)->IsCompressed() || previous_was_parallel) {
            //if arc is of zero height, just collect it.
            //Its position may depend on the next arc if that is compressed.
            if (h==0) {
                if (first_zero_height == to) first_zero_height = i;
                continue;
            }
            const double new_y = std::max(y_upper_limit, -cover.OffsetBelow(arc_cover, touchpoint));
            //Here the new_y can be larger than the one before, if some prior element 
            //prevented the current one to shift all the way to below the previous one.
            if ((*i)->IsCompressed()) 
                y = new_y;
            else //we must have previous_was_parallel==true here
                //if the immediately preceeding element (not including zero_height_ones) was
                //marked with "parallel", we attempt to shift this element up to the top of that one
                //even if the current element is not marked by "compress".
                //If we can shift it all the way to the top of the previous element, we place it
                //there. But if we can shift only halfway, we place it strictly under the previous
                //element - as we are not compressing.
                //Note that "y_upper_limit" contains the top of the preceeding element (marked with "parallel")
                if (new_y == y_upper_limit) 
                    touchpoint = y = y_upper_limit;
                else 
                    touchpoint = y; //OffsetBelow() may have destroyed it above
        } else if (had_parallel_above && h>0) {
            //If this element is not compressed, but we had an element marked with "parallel" above
            //it may be that we overlap with a prior element, so we want to avoid that.
            //But we place zero_height elements to just below the previous one nevertheless
            //We also keep touchpoint==y for the very same reason
            double dummy_touchpoint;
            y = std::max(y, -cover.OffsetBelow(arc_cover, dummy_touchpoint));
        }
        touchpoint = floor(touchpoint+0.5);
        y = ceil(y);
        //We got a non-zero height or a non-compressed one, flush zero_height ones (if any)
        while (first_zero_height != to && first_zero_height != i)
            (*first_zero_height++)->ShiftBy(touchpoint);
        first_zero_height = to;
        //Shift the arc in question to its place
        (*i)->ShiftBy(y);
        arc_cover.Shift(XY(0,y));
        y_bottom = std::max(y_bottom, y+h);
        //If we are parallel draw the rest of the block in one go
        if ((*i)->IsParallel()) {
            //kill the mainline of the last arc (in "i")
            arc_cover.InvalidateMainLine();
            //indicate that elements be 
            had_parallel_above = true;
            y_upper_limit = y;
        }
        previous_was_parallel = (*i)->IsParallel();
        cover += arc_cover;
        y = y_bottom;
    }
    //position any remaining zero-heright items at the bottom
    while (first_zero_height != to)
        (*first_zero_height++)->ShiftBy(y);
    return y_bottom;
}

//This one places a list (arcs) at start_y (which is supposed to be
//well below area_top), and calls Height for all of them and ShifyBy.
//Then, if the first element has compress==yes or forcecompress==true, it
//compresses the list up taking area_top into account, but no higher than top_y
//Returns the bottommost coordinate touched by the list after completion.
//The whole reason for this function is to prevent a big empty space
//between the first and second arrow, when the first arrow can be
//compressed a lot, but the second not. Here we move all the arrows
//as one block only up till none of them collides with something.
//No matter what input parameters we get we always place the list at an integer
//y coordinate
//If ret_cover is not null, we return the rsulting cover of the list at the pos where placed
double Msc::PlaceListUnder(MscCanvas &canvas, ArcList::iterator from, ArcList::iterator to, double start_y,
                           double top_y, const AreaList &area_top, bool reflow, bool forceCompress,
                           AreaList *ret_cover)
{
    if (from==to) return 0;
    AreaList cover;
    double h = HeightArcList(canvas, from, to, cover, reflow);
    double touchpoint;
    double new_start_y = std::max(top_y, -area_top.OffsetBelow(cover, touchpoint));
    //if we shifted up, apply shift only if compess is on
    if (new_start_y < start_y) {
        if (forceCompress || (*from)->IsCompressed()) 
            start_y = new_start_y;    
    } else //if we shifted down, apply it in any case
        start_y = new_start_y;
    start_y = ceil(start_y);
    ShiftByArcList(from, to, start_y);
    if (ret_cover)
        ret_cover->swap(cover.Shift(XY(0, start_y)));
    return start_y + h;
}

void Msc::ShiftByArcList(ArcList::iterator from, ArcList::iterator to, double y)
{
    while (from!=to)
        (*from++)->ShiftBy(y);
}

//Find the smallest elements between indexes [i, j) and bring them up to the
//value of the second smallest element, but overall do not add more than
//max_sum (round up). Return how much of max_sum has remained.
// E.g, if [i, j) are 10, 13, 15, 10, 10 and max_sum=10
//              make: 13, 13, 15, 13, 13 and return 10-9 = 1;
// E.g., max_sum = 5, return 12, 13, 15, 12, 12 and return 0 (round up).
double  MscSpreadBetweenMins(vector<double> &v, unsigned i, unsigned j, double max_sum)
{
    double minvalue = v[i];     //the value of the smallest member
    double secondvalue = v[i];  //the value of the second smallest member
    unsigned count = 1;           //count of how many do we have of minvalue
    for (unsigned t=i+1; t<j; t++)
        if (v[t]<minvalue) {
            secondvalue = minvalue;
            minvalue = v[t];
            count = 1;
        } else if (v[t] == minvalue)
            count ++;
        else if (secondvalue > v[t])
            secondvalue = v[t];
    double amount;
    //See if we distribute all of it: if all elements are equal or max_sum is small
    if (secondvalue == minvalue || (secondvalue - minvalue)*count > max_sum) {
        amount = (max_sum+count-1)/count; //round up
        max_sum = 0;
    } else {
        amount = secondvalue-minvalue;
        max_sum -= amount*count;
    }
    //Do the distribution
    for (;i<j; i++)
        if (v[i] == minvalue)
            v[i]+= amount;
   return max_sum;
}


//Calculate total.x and y. Ensure they are integers
void Msc::CalculateWidthHeight(MscCanvas &canvas)
{
    yPageStart.clear();
    yPageStart.push_back(0);
    HideELinesHere.clear();
    if (Arcs.size()==0) return;
    if (total.y.Spans() > 0) return; //already done?
    //start with width calculation, that is used by many elements
    //First reset running shown of entities, this will be used during Width() pass
    //for (auto i=AllEntities.begin(); i!=AllEntities.end(); i++)
    //    (*i)->running_shown = EEntityStatus::SHOW_OFF;
    EntityDistanceMap distances;
    //Add distance for arcs,
    //needed for hscale=auto, but also for entity width calculation and side note size calculation
    WidthArcList(canvas, Arcs, distances);
    distances.CombineLeftRightToPair_Max(hscaleAutoXGap, activeEntitySize/2);
    distances.CombineLeftRightToPair_Single(hscaleAutoXGap);
    distances.CopyBoxSideToPair(hscaleAutoXGap);
    
    double unit = XCoord(1);
    const double lnote_size = distances.Query(NoEntity->index, LNote->index)/unit;
    const double rnote_size = distances.Query(RNote->index, RNote->index+1)/unit;
    total.x.from = 0;
    total.y.from = 0;
    if (GetHScale()<0) {
        //Now go through all the pairwise requirements and calc actual pos.
        //dist will hold required distance to the right of entity with index []
        vector<double> dist(ActiveEntities.size(), 0);
        dist[0] = 0;
        dist[LSide->index] = XCoord(MARGIN_HSCALE_AUTO);
        dist[RSide->index-1] = XCoord(MARGIN_HSCALE_AUTO);
        //distances.pairs starts with requiremenst between neighbouring entities
        //and continues with requirements between second neighbours, ... etc.
        //we process these sequentially
        for (auto i = distances.pairs.begin(); i!=distances.pairs.end(); i++) {
            //Get the requirement
            double toadd = i->second;
            //Substract the distance already there
            for (unsigned f = i->first.first; f!=i->first.second; f++)
                toadd -= dist[f];
            //If there is need to increase start with the closer ones
            //and gradually move to larger ones.
            while (toadd>0)
                toadd = MscSpreadBetweenMins(dist, i->first.first,
                                                i->first.second, toadd);
        }
        //Now dist[i] contains the needed space on the right of entity index i
        //Consider "sideNoteGap"
        if (lnote_size) {
            dist[LNote->index] += 2*sideNoteGap;
        }
        if (rnote_size) {
            dist[RSide->index] += 2*sideNoteGap;
        }
        double curr_pos = MARGIN_HSCALE_AUTO; //This is here only for bkw comp!!
        unsigned index = 0;
        for (EIterator j = ActiveEntities.begin(); j!=ActiveEntities.end(); j++) {
            (*j)->pos = curr_pos;
            ////Mark all parents of this active entity (they are not active) with this "pos"
            ////In the end this will make any grouped entity to have the same "pos" as
            ////one of its active descendants. It is unspecified, which, but that does not
            ////matter, we just need this in CommandEntity::Width, where we want to find
            ////the leftmost and rightmost active descendant of a grouped node
            //EIterator j_loc = j;
            //while ((*j_loc)->parent_name.length()>0) {
            //    j_loc = AllEntities.Find_by_Name((*j_loc)->parent_name);
            //    _ASSERT(*j_loc != NoEntity);
            //    (*j_loc)->pos = curr_pos;
            //}
            //advance curr_pos to the next entity
            curr_pos += ceil(dist[index++])/unit;    //take integer space, so XCoord will return integer
        }
        total.x.till = XCoord(curr_pos)+1;
    } else {
        //In postparseprocess we set the virtual entity's pos as follows
        //in the second column we show how they should be if there are
        //side commets of L and R size resp. (including 2*sideNoteGap)
        //Noentity     = 0             |  0                             | 0 
        //LNote        = 0             |  L/unit                        | L/unit
        //LSide        = MARGIN        |  L/unit + MARGIN               | L/unit
        //first entity = 2*MARGIN      |  L/unit + 2*MARGIN             | L/unit
        //last entity  = x             |  L/unit + x                    | L/unit
        //RSIDE        = x + MARGIN    |  L/unit + x + MARGIN           | L/unit
        //RNOTE        = x + 2*MARGIN  |  L/unit + x + 2*MARGIN         | L/unit
        //rotal.x      = x + 2*MARGIN  |  L/unit + x + 2*MARGIN + R/unit| L/unit + R/unit

        //Here we only adjust the space for notes on the side
        if (lnote_size) {
            //Shift all entities (use twice of "sideNoteGap")
            const double diff = lnote_size + sideNoteGap*2/unit;
            for (auto ei = ActiveEntities.Find_by_Ptr(LNote); ei != ActiveEntities.end(); ei++)
                (*ei)->pos += diff;
        }
        total.x.till = XCoord(RNote->pos)+1; //XCoord is always integer
        if (rnote_size) 
            total.x.till += rnote_size*unit + 2*sideNoteGap;
    }
    //Consider the copyright text
    StringFormat sf;
    sf.Default();
    XY crTexSize = Label(copyrightText, canvas, sf).getTextWidthHeight().RoundUp();
    if (total.x.till < crTexSize.x) total.x.till = crTexSize.x;
    copyrightTextHeight = crTexSize.y;
    comments_right_side = total.x.till; //save: total.x.till may be later increased by note placement

    //Turn on entity line for side note lines if there are side notes
    if (lnote_size) 
        LNote->status.SetStatus(0, EEntityStatus::SHOW_ON);
    if (rnote_size) 
        RNote->status.SetStatus(0, EEntityStatus::SHOW_ON);

    drawing.x.from = XCoord(LNote->pos) + sideNoteGap;
    drawing.x.till = XCoord(RNote->pos) - sideNoteGap;
    AreaList cover;
    total.y.till = HeightArcList(canvas, Arcs.begin(), Arcs.end(), cover, false) + chartTailGap;
    total.y.till = ceil(std::max(total.y.till, cover.GetBoundingBox().y.till));
    drawing.y = total.y;  
}

void Msc::PlaceWithMarkersArcList(MscCanvas &canvas, ArcList &arcs, double autoMarker)
{
    for (auto j = arcs.begin(); j != arcs.end(); j++)
        (*j)->PlaceWithMarkers(canvas, autoMarker);
}


void Msc::PlaceFloatingNotes(MscCanvas &canvas)
{
    Block new_total;
    new_total.MakeInvalid();
    for (auto note = Notes.begin(); note!=Notes.end(); note++) {
        (*note)->PlaceFloating(canvas);
        new_total += (*note)->GetAreaToDraw().GetBoundingBox();
    }
    if (new_total.IsInvalid()) return;
    new_total.Expand(sideNoteGap);
    total += new_total;
}


void Msc::PostPosProcessArcList(MscCanvas &canvas, ArcList &arcs)
{
    for (auto j = arcs.begin(); j != arcs.end(); j++)
        (*j)->PostPosProcess(canvas);
}

void Msc::CompleteParse(MscCanvas::OutputType ot, bool avoidEmpty)
{

    //Allocate (non-sized) output object and assign it to the chart
    //From this point on, the chart sees xy dimensions
    MscCanvas canvas(ot);

    //Sort Entities, add numbering, fill in auto-calculated values,
    //and throw warnings for badly constructed diagrams.
    headingSize = 0;
    PostParseProcess(canvas); 
    FinalizeLabelsArcList(Arcs, canvas);

    //Calculate chart size
    CalculateWidthHeight(canvas);

    //If the chart ended up empty we may want to display something
    if (total.y.till <= chartTailGap && avoidEmpty) {
        //Add the Empty command
        Arcs.push_front((new CommandEmpty(this))->AddAttributeList(NULL));
        _ASSERT(Contexts.size() && Contexts.back().hscale.first);
        Contexts.back().hscale.second = -1;
        //Redo calculations
        total.x.till = total.y.till = 0;  //"from" members still zero, only notes can take it negative
        CalculateWidthHeight(canvas);
        //Luckily Width and DrawCover calls do not generate error messages,
        //So Errors collected so far are OK even after redoing this
    }

    PlaceWithMarkersArcList(canvas, Arcs, -1);
    PlaceFloatingNotes(canvas);

    total.x.from = floor(total.x.from);
    total.y.from = floor(total.y.from);
    total.x.till = ceil(total.x.till);
    total.y.till = ceil(total.y.till);

    //A final step of prcessing, checking for additional drawing warnings
    PostPosProcessArcList(canvas, Arcs);

    //Sort elements in AllCovers, so that the ones we draw later show up later
    struct {
        bool operator()(const Area &a1, const Area &a2) {
            _ASSERT(a1.arc!=NULL && a2.arc!=NULL);
            return a1.arc->draw_pass < a2.arc->draw_pass;
        }
    } comp;
    AllCovers.sort(comp);

    //Delete LSide and RSide actions if there are no side comments
    if (LNote->pos == LSide->pos) 
        LSide->status.Reset();
    if (RNote->pos == RSide->pos) 
        RSide->status.Reset();

    Error.Sort();
}


void Msc::DrawArcs(MscCanvas &canvas, DrawPassType pass)
{
    DrawArcList(canvas, Arcs, pass);
}

//page is 0 for all, 1..n for individual pages
void Msc::DrawCopyrightText(MscCanvas &canvas, unsigned page)
{
    if (total.x.Spans()<=0 || page > yPageStart.size()) return;
    StringFormat sf;
    sf.Default();
    Label label(copyrightText, canvas, sf);
    label.Draw(canvas, total.x.from, total.x.till, page==0 || page>=yPageStart.size() ? total.y.till : yPageStart[page]);
}

void Msc::DrawPageBreaks(MscCanvas &canvas)
{
    if (yPageStart.size()<=1) return;
    if (total.y.Spans()<=0) return;
    MscLineAttr line;
    line.type.second = LINE_DASHED;
    StringFormat format;
    format.Default();
    format.Apply("\\pr\\-");
    Label label;
    for (unsigned page=1; page<yPageStart.size(); page++) {
        char text[20];
        const double y = yPageStart[page];
        XY d;
        canvas.Line(XY(total.x.from, y), XY(total.x.till, y), line);
        sprintf(text, "page %u", page);
        label.Set(text, canvas, format);
        label.Draw(canvas, total.x.from, total.x.till, y-label.getTextWidthHeight().y);
    }
}

void Msc::Draw(MscCanvas &canvas, bool pageBreaks)
{
    if (total.y.Spans() <= 0) return;
	//Draw small marks in corners, so EMF an WMF spans correctly
	MscLineAttr marker(LINE_SOLID, MscColorType(255,255,255), 0.1, CORNER_NONE, 0);
	canvas.Line(XY(total.x.from,total.y.from), XY(total.x.from+1,total.y.from), marker);
	canvas.Line(XY(total.x.till,total.y.till), XY(total.x.till-1,total.y.till), marker);
	//draw background
    if (Background.size()) {
        MscFillAttr fill_bkg(MscColorType(255,255,255), GRADIENT_NONE);
        fill_bkg += Background.begin()->second;
        double y = Background.begin()->first;
	    for (auto i = ++Background.begin(); i!=Background.end(); i++) {
            canvas.Fill(Block(XY(total.x.from,y), XY(total.x.till,i->first)), fill_bkg);
            fill_bkg += i->second;
            y = i->first;
        }
        if (y < total.y.till) 
            canvas.Fill(Block(XY(total.x.from,y), XY(total.x.till,total.y.till)), fill_bkg);
    }
    //Add background for side notes (if any)
    for (unsigned u = 0; u<2; u++) {
        const Entity * const note = u==0 ? LNote : RNote;
        const Entity * const side = u==0 ? LSide : RSide;
        if (note->pos == side->pos) continue;
        const Range x = u==0 ? Range(total.x.from, XCoord(LNote->pos)) : Range(XCoord((RNote)->pos), total.x.till); 
        double pos = total.y.from, till = total.y.from;
        do {
            till = note->status.StyleTill(till);
            if (till<total.y.till && note->status.GetStyle(pos).fill == note->status.GetStyle(till).fill)
                continue;  //go until "fill" changes
            if (till>total.y.till) till = total.y.till;
            canvas.Fill(Block(x, Range(pos, till)), note->status.GetStyle(pos).fill);
            pos = till;
        } while (pos<total.y.till);
    }
	//Draw page breaks
    if (pageBreaks)
        DrawPageBreaks(canvas);
    DrawArcs(canvas, DRAW_BEFORE_ENTITY_LINES);
	//Draw initial set of entity lines (boxes will cover these and redraw)
    DrawEntityLines(canvas, total.y.from, total.y.till);
    DrawArcs(canvas, DRAW_AFTER_ENTITY_LINES);
    DrawArcs(canvas, DRAW_DEFAULT);
    DrawArcs(canvas, DRAW_AFTER_DEFAULT);
    DrawArcs(canvas, DRAW_NOTE);
    DrawArcs(canvas, DRAW_AFTER_NOTE);

    /* Debug: draw Debug Shapes */
    for (auto i=DebugContours.begin(); i!=DebugContours.end(); i++) {
        i->fill.MakeComplete();
        i->line.MakeComplete();
        canvas.Fill(i->area, i->fill);
        canvas.Line(i->area, i->line);
    }
    // End of debug */


    /* Debug: draw entity lines 
    cairo_set_source_rgb(cr, 0, 0, 1);
    cairo_set_line_width(cr,2);
    HideELinesArea.Line(cr);
    // End of debug */

    /* Debug: draw cover 
    contour::Bitmap bitmap(unsigned(total.x), unsigned(total.y));
    bitmap.FillList(AllCovers);
    bitmap.DrawOnto(canvas.GetContext());
    // End of debug */

    /* Debug: draw float_map 
    Bitmap original_map_imp(unsigned(ceil(total.x)), unsigned(ceil(total.y)));
    Bitmap original_map_all(unsigned(ceil(total.x)), unsigned(ceil(total.y)));
    for (auto i = NoteMapImp.begin(); i!=NoteMapImp.end(); i++)
        original_map_imp.Fill(**i);
    for (auto i = NoteMapAll.begin(); i!=NoteMapAll.end(); i++)
        original_map_all.Fill(**i);
    original_map_all.DrawOnto(canvas.GetContext());
    // End of debug */

    /* Debug: draw float_map 
    unsigned m2 = 1;
    contour::Bitmap bitmap(unsigned(total.x), unsigned(total.y));
    for (auto i = AllArcs.begin(); i!=AllArcs.end(); i++)
        bitmap.Fill(i->second->GetNoteMap());
    bitmap.CreateDownscale(m2).DrawOnto(canvas.GetContext(), m2);
    Contour tri(XY(0,0), XY(50,30), XY(30,50));
    contour::Bitmap tri_b(51,51);
    tri_b.Fill(tri);
    unsigned x=100, y=100;
    if (bitmap.Position(tri_b, x, y, Contour(), 4, 100))
        tri_b.DrawOnto(canvas.GetContext(), 1, x, y);
    else 
        tri.Shift(XY(100,100)).Line(canvas.GetContext());
    // End of debug */
}

void Msc::DrawToOutput(MscCanvas::OutputType ot, const XY &scale, const string &fn, bool bPageBreaks)
{
    if (yPageStart.size()<=1) {
        MscCanvas canvas(ot, total, copyrightTextHeight, fn, scale);
        switch (canvas.Status()) {
        case MscCanvas::ERR_FILE: Error.Error(file_line(0, 0), "Could not open file '" + fn + "'."); return;
        case MscCanvas::ERR_PARAM: Error.Error(file_line(0, 0), "Internal param problem when opening canvas."); return;
        case MscCanvas::ERR_CANVAS: Error.Error(file_line(0, 0), "Could not open canvas."); return;
        case MscCanvas::ERR_DONE: Error.Error(file_line(0, 0), "Whops, internal error."); return;
        default: break;
        }
        Draw(canvas, bPageBreaks);
        canvas.PrepareForCopyrightText(); //Unclip the banner text exclusion clipped in SetOutput()
        DrawCopyrightText(canvas);
        return;
    }
    for (unsigned page=1; page<=yPageStart.size(); page++) {
        MscCanvas canvas(ot, total, copyrightTextHeight, fn, scale, &yPageStart, page);
        switch (canvas.Status()) {
        case MscCanvas::ERR_FILE: Error.Error(file_line(0, 0), "Could not open file '" + fn + "'."); return;
        case MscCanvas::ERR_PARAM: Error.Error(file_line(0, 0), "Internal param problem when opening canvas."); return;
        case MscCanvas::ERR_CANVAS: Error.Error(file_line(0, 0), "Could not open canvas."); return;
        case MscCanvas::ERR_DONE: Error.Error(file_line(0, 0), "Whops, internal error."); return;
        default: break;
        }
        Draw(canvas, false);
        canvas.PrepareForCopyrightText(); //Unclip the banner text exclusion clipped in SetOutput()
        DrawCopyrightText(canvas, page);
    }
}

void Msc::InvalidateNotesToThisTarget(const TrackableElement *target)
{
    for(auto i=Notes.begin(); i!=Notes.end(); i++) 
        if ((*i)->GetTarget() == target)
            (*i)->Invalidate();
}

void Msc::RemoveFromNotes(const CommandNote *note)
{
    for(auto i=Notes.begin(); i!=Notes.end(); /*nope*/) 
        if (*i==note) Notes.erase(i++);
        else i++;
}
