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
/** @file msc.cpp The definition of the classes Msc, EntityDistanceMap and DistanceMapVertical .
 * @ingroup libmscgen */

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
void EntityDistanceMap::Insert(unsigned e1, int e2, double xdist, bool hspace)
{
    if (int(e1)==e2) return;
    if (e2==DISTANCE_LEFT) {
        _ASSERT(!hspace); //hspace shall insert only entity-to-entity distances
        std::map<unsigned, double>::iterator i = left.find(e1);
        if (i!=left.end() && i->second >= xdist) return;
        _ASSERT(e1>0);
        left[e1] = xdist;
    } else if (e2==DISTANCE_RIGHT) {
        _ASSERT(!hspace);
        std::map<unsigned, double>::iterator i = right.find(e1);
        if (i!=right.end() && i->second >= xdist) return;
        right[e1] = xdist;
    } else {
        IPair eip(e1, e2);
        std::map<IPair, double, IPairComp> &collection_to_use = hspace ? hspace_pairs : pairs;
        auto i = collection_to_use.find(eip);
        if (i!=collection_to_use.end() && i->second >= xdist) return;
        collection_to_use[eip] = xdist;
    }
}

//Query what was the (largest) requirement.
//e2 can again be DISTANCE_{LEFT,RIGHT}. we return 0 if there were no
//requirements for these entities.
double EntityDistanceMap::Query(unsigned e1, int e2, bool hspace) const
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
    const std::map<IPair, double, IPairComp> &collection_to_use = hspace ? hspace_pairs : pairs;
    auto i = collection_to_use.find(eip);
    if (i==collection_to_use.end()) return 0;
    return i->second;
}

bool EntityDistanceMap::IncreaseIfNonZero(unsigned e1, int e2, double d, bool hspace)
{
    const double q = Query(e1, e2, hspace);
    if (q) {
        Insert(e1, e2, d+q, hspace);
        return true;
    }
    return false;
}


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
    auto hit = *l->second.begin();
    for (auto i : l->second)
        if ((!left && i.second > hit.second) || (left && i.first > hit.first))
            hit = i;
    return hit;
}

void EntityDistanceMap::CombineBoxSideToPair(double gap)
{
    for (const auto k : box_side) {
        double req = 0;
        for (auto l : k.second)
            req = std::max(req, l.first + l.second);
        Insert(k.first, k.first+1, req + gap);
    }
    box_side.clear();
}


//If there is X on the right side of e1 and Y on the e1+1 entity
//convert these to a distance of X+Y+gap between e1 and e1+1
void EntityDistanceMap::CombinePairedLeftRightToPair_Sum(double gap)
{
    std::map<unsigned, double>::iterator i, j;
    for(i=right.begin(); i!=right.end(); /*nope*/) {
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
void EntityDistanceMap::CombineUnPairedLeftRightToPair(double gap)
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
    for(auto pi : d.pairs)
        Insert(pi.first.first, pi.first.second, pi.second);
    for(auto pi : d.hspace_pairs)
        Insert(pi.first.first, pi.first.second, pi.second);
    for(auto i : d.left)
        Insert(i.first, DISTANCE_LEFT, i.second);
    for(auto i : d.right)
        Insert(i.first, DISTANCE_RIGHT, i.second);
    for(auto b : d.box_side)
        box_side[b.first].insert(box_side[b.first].end(), b.second.begin(), b.second.end());
    was_activated.insert(d.was_activated.begin(), d.was_activated.end());
    had_l_comment |= d.had_l_comment;
    had_r_comment |= d.had_r_comment;
    return *this;
}

string EntityDistanceMap::Print()
{
    string s;
    s << "right"<<"\n";
    for(auto i : right)
        s << "  " << i.first << " " << i.second << "\n";
    s << "left" << "\n";
    for(auto i : left)
        s << "  " << i.first << " " << i.second << "\n";
    s << "pairs"<<"\n";
    for(auto pi : pairs)
        s << "  (" << pi.first.first <<"," << pi.first.second<< ") " << pi.second << "\n";

    return s;
}

void DistanceMapVerticalElement::Insert(unsigned e1, int e2, double d)
{
    if (e2!=DISTANCE_LEFT && e2!=DISTANCE_RIGHT) {
        _ASSERT(0);
        return;
    }
    std::map<unsigned, double> &store = e2==DISTANCE_LEFT ? left : right;
    auto i = store.find(e1);
    if (i==store.end())
        store[e1] = d;
    else
        i->second = std::max(i->second, d);
}

double DistanceMapVerticalElement::Query(unsigned e1, int e2)
{
    if (e2!=DISTANCE_LEFT && e2!=DISTANCE_RIGHT) {
        _ASSERT(0);
        return 0;
    }
    std::map<unsigned, double> &store = e2==DISTANCE_LEFT ? left : right;
    auto i = store.find(e1);
    return i==store.end() ? 0 : i->second;
}

/** Takes the maximum of distances stored for each entity. Ignores marker name.*/
DistanceMapVerticalElement &DistanceMapVerticalElement::operator += (const DistanceMapVerticalElement &d)
{
    for (auto &m : d.left) {
        auto i = left.find(m.first);
        if (i==left.end())
            left[m.first] = m.second;
        else
            i->second = std::max(i->second, m.second);
    }
    for (auto &m : d.right) {
        auto i = right.find(m.first);
        if (i==right.end())
            right[m.first] = m.second;
        else
            i->second = std::max(i->second, m.second);
    }
    left_entity  = std::min(left_entity,  d.left_entity);
    right_entity = std::max(right_entity, d.right_entity);
    return *this;
}

/** Insert a side distance between the two given markers (which may come in any order)
 * if one of the markers is not yet found (or is empty) we insert the side distance
 * from the valid marker to the currently last one. If none found, we do nothing. 
 * Both markers may equal to .end() - this is the case of future markers, not
 * yet added. */
void DistanceMapVertical::Insert(unsigned e1, int e2, double d, iterator since, iterator to)
{
    //Find first marker
    auto i = elements.begin();
    while (i!=elements.end() && i!=since && i!=to)
        i++;
    if (i==elements.end()) return;
    //insert in all subsequent ranges till we find the other one
    do {
        (i++)->Insert(e1, e2, d);
    } while (i!=elements.end() && i!=since && i!=to);
}


/** Takes the max of distances between the two markers.
 * One of the markers can be empty meaning the top of the chart.
 * One of the markers can be MARKER_HERE_STR meaning the end of the list.
 * If a marker is not found, it is assumed to be below us and we
 * treat it as if it were MARKER_HERE_STR.
 * The two markers are equal (or none found), we return zero distances. */
DistanceMapVerticalElement DistanceMapVertical::Get(const_iterator m1, const_iterator m2) const
{
    //We assume none of the marker name we store is equal to MARKER_HERE_STR...
    DistanceMapVerticalElement ret;
    if (m1==m2)
        return ret;
    bool had_one = false;
    for (const_iterator i = elements.begin(); i!=elements.end(); i++) {
        if (i == m1 || i == m2) {
            if (had_one)
                return ret;
            had_one = true;
            ret += *i;
        } else if (had_one)
            ret += *i;
    }
    return ret;
}

///////////////////////////////////////////////////////////////////////

const char LabelInfo::labelTypeChar[] = "-EADBbPVCSN";

/** We do some processing only for tracking (which is the ability on the GUI.
    to highlight elements and map them to their line number and vice versa).
    If `prepare_for_tracking` is not set, we omit these steps.
    - Each element computes its `area` and potentially `area_to_draw` members.
    - `AllCovers` AreaList is filled with a copy of the `area` member of each Element (to search in).
    - `AllArcs` is a file-position index of all Element objects.
*/

//"Entities" member is responsible to delete ist contents
//"AutoGenEntities" is not, as its contents will be inserted into an
//CommandEntity in Msc::PostParseProcess()
Msc::Msc() :
    AllEntities(true), ActiveEntities(false), AutoGenEntities(false),
    Arcs(true), EndNotes(true), Notes(false), NoteBlockers(false), 
    total(0,0,0,0), drawing(0,0,0,0), 
    copyrightTextHeight(0), headingSize(0), noLabels(0), noOverflownLabels(0)
{
    chartTailGap = 3;
    selfArrowYSize = 12;
    headingVGapAbove = 2;
    headingVGapBelow = 2;
    boxVGapOutside = 2;
    boxVGapInside = 2;
    arcVGapAbove = 0;
    arcVGapBelow = 3;
    discoVgap = 3;
    titleVgap = 10;
    subtitleVgap = 5;
    nudgeSize = 4;
    activeEntitySize = 14;
    compressGap = 2;
    hscaleAutoXGap = 5;
    sideNoteGap = 5;
    defWNoteWidth = 0.8;
    trackFrameWidth = 4;
    trackExpandBy = 2;
    entityShapeHeight = 60;

    pedantic=false;
    ignore_designs = false;
    simple_arc_parallel_layout = false;
    prepare_for_tracking = true;

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
                          Contexts.back().styles["entity"], FileLineCol(current_file, 0), false);
    LNote = new Entity(LNOTE_ENT_STR, LNOTE_ENT_STR, LNOTE_ENT_STR, -1001, -1001,
                       Contexts.back().styles["entity"], FileLineCol(current_file, 0), false);
    LSide = new Entity(LSIDE_ENT_STR, LSIDE_ENT_STR, LSIDE_ENT_STR, -1000, -1000,
                       Contexts.back().styles["entity"], FileLineCol(current_file, 0), false);
    RSide = new Entity(RSIDE_ENT_STR, RSIDE_ENT_STR, RSIDE_ENT_STR, 10000, 10000,
                       Contexts.back().styles["entity"], FileLineCol(current_file, 0), false);
    RNote = new Entity(RNOTE_ENT_STR, RNOTE_ENT_STR, RNOTE_ENT_STR, 10001, 10001,
                       Contexts.back().styles["entity"], FileLineCol(current_file, 0), false);
    EndEntity = new Entity(END_ENT_STR , END_ENT_STR , END_ENT_STR , 10002, 10002,
                           Contexts.back().styles["entity"], FileLineCol(current_file, 0), false);
    
    AllEntities.Append(NoEntity);
    AllEntities.Append(LNote);
    AllEntities.Append(LSide);
    AllEntities.Append(RSide);
    AllEntities.Append(RNote);
    AllEntities.Append(EndEntity);

    //Add default markers
    Markers[MARKER_BUILTIN_CHART_TOP_STR].line.MakeInvalid();
    Markers[MARKER_BUILTIN_CHART_BOTTOM_STR].line.MakeInvalid();

    //This sets the global context to "plain" (redundant) 
    //and adds CommandEntities for lcomment.* and CommandBackground if needed.
    ArcBase *toadd;
    SetDesign(true, "plain", true, &toadd); 
    Arcs.Append(toadd);
}

Msc::~Msc() 
{
    //Proper deletion order
    Arcs.Empty();    //This must be before Notes, since Element::~ will use chart->Notes
}

/** Applies a design to the chart.
 * @param [in] full True if the design to add is a full design, else it is a partial only
 * @param [in] name The name of the design to apply
 * @param [in] force If true, we apply the design even though Msc::ignore_designs is true
 * @param [out] ret An arc (a list of arcs) that should be inserted to the chart at this point  (background changes, comment line changes), or NULL if none
 * @param [in] l The place in the input file, where the design change was proscribed.
 * @returns 0 if design not found; 1 if found and applied or found and ignored (via ignore_designs);
 *          2 if found and applied, but is full, whereas should be partial;
 *          3 if found and applied, but is partiall, whereas should be full */
int Msc::SetDesign(bool full, const string&name, bool force, ArcBase **ret, const FileLineColRange &l)
{
    *ret = NULL;
    auto i = Designs.find(name);
    if (i==Designs.end())
        return 0;
    if (ignore_designs &&!force)
        return 1;
    if (i->second.is_full)
        Contexts.back() = i->second;
    else
        Contexts.back() += i->second;
    ArcList *list = new ArcList(true);
    if (!i->second.defBackground.IsEmpty()) {
        ArcBase *arc = new CommandNewBackground(this, i->second.defBackground);
        arc->AddAttributeList(NULL);
        list->Append(arc);
    }
    if (!i->second.defCommentFill.IsEmpty() || !i->second.defCommentLine.IsEmpty()) {
        MscStyle s; //empty
        s.vline += i->second.defCommentLine;
        s.fill += i->second.defCommentFill;
        list->Append(CEForComments(s, l));
    }
    //ArcParallel here becomes internally defined
    *ret = list->size() ? new ArcParallel(this, list) : NULL;
    if (full == i->second.is_full) return 1;
    return full ? 3 : 2;
}

/** Returns a space delimited string of all full or partial design names.*/
string Msc::GetDesignNames(bool full) const
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

EIterator Msc::FindAllocEntity(const char *e, const FileLineColRange &l)
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
        EntityApp *ed = new EntityApp(e, this);
        ed->SetLineEnd(l);
        ed->AddAttributeList(NULL, NULL, l.start);
        ed->show.first = ed->show.second = true; //start turned on
        AutoGenEntities.Append(ed);
        ei = AllEntities.Find_by_Name(e);
    }
    return ei;
}

/** Find the leftmost or rightmost descendant of an entity.
 * We search for the ultimate descendant, which is not a grouped entity itself and which 
 * may be several nested groups below.
 * @param [in] ei An interator in AllEntities to search the descendant for. 
 *                If  not a group entity itself is returned.
 * @param [in] left Governs if the leftmost or rightmost descendant is searched.
 * @param [in] stop_at_collapsed If true, we do not go down to the descendants of a 
 *             collapsed entity, so we may return a group entity, which is collapsed or a 
 *             non group entity. If false, we work as if all entities were expanded.
 * @returns An inerator pointing to AllEntities. */
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

/** Find the lowest anscestor, which is not hidden due to collapsed group entities.
 * - If `i` is not part (child) of a group entity, we return `i`.
 * - If `i` is part of a group and none of our parents and ancestors are collapsed, we return `i`.
 * - If `i` is part of a group and some parents are collapsed, we return the highest, which is
 *   collapsed (and thus is not itself hidden).
 * In essence we return, which entity is shown on the chart for `i`.
 * (Virtual entities (NoEntity, leftside, leftnote, etc. will return themselves.)*/
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

/** Return a non-grouped or collapsed entity that shows for `i`.
 * If `i` itself is a not hidden not collapsed group entity, we return
 * either the left or rightmost descendant that is not hidden.
 * If `i` itself does not show, we return an ascendent that does.*/
EIterator Msc::FindWhoIsShowingInsteadOf(EIterator ei, bool left)
{
    EIterator sub = FindActiveParentEntity(ei);
    if (sub != ei) return sub; //a parent of ours show instead of us
    return FindLeftRightDescendant(sub, left, true); //a child of us shows
}

/** Return a comma separated list of names of the descendant entities in single quotation marks. 
 * If `ei` is not a group entity its name is returned.
 * The order of the descendants is a result of a depth first tree traversal. */
string Msc::ListGroupedEntityChildren(EIterator ei) 
{
    if ((*ei)->children_names.size() == 0) return "'" + (*ei)->name + "'";
    string s;
    for (const auto &i : (*ei)->children_names)
        s.append(ListGroupedEntityChildren(AllEntities.Find_by_Name(i))).append(", ");
    s.erase(s.length()-2);
    return s;
}

/** Emit an error and return true if the entity is grouped. */
bool Msc::ErrorIfEntityGrouped(EIterator ei, FileLineCol l) 
{
    if ((*ei)->children_names.size()==0) return false;
    Error.Error(l, "Group entity '" + (*ei)->name + "' cannot be used here.", 
                "Use one of its members (" + ListGroupedEntityChildren(ei) + ") instead."); 
    return true;
}
/** Returns true if `child` is a (not necessarily direct) descendant of `parent`*/
bool Msc::IsMyParentEntity(const string &child, const string &parent)
{
    if (child == parent) return false;
    const string myparent = (*AllEntities.Find_by_Name(child))->parent_name;
    if (myparent.length()==0) return false;
    if (myparent == parent) return true;
    return IsMyParentEntity(myparent, parent);
}

/** Get the "pos" of the highest entity (excluding virtual entities)*/
double Msc::GetEntityMaxPos() const
{
    double ret = -1;  //first entity will be return + 1, which will be zero 
    for (auto i=AllEntities.begin(); i!=AllEntities.end(); i++)
        if (!IsVirtualEntity(*i) && ret < (*i)->pos)
            ret = (*i)->pos;
    return ret;
}
            
/** Get the "pos_exp" of the highest entity (excluding virtual entities)*/
double Msc::GetEntityMaxPosExp() const
{
    double ret = -1;  //first entity will be return + 1, which will be zero 
    for (auto i=AllEntities.begin(); i!=AllEntities.end(); i++)
        if (!IsVirtualEntity(*i) && ret < (*i)->pos_exp)
            ret = (*i)->pos_exp;
    return ret;
}
            
/** Create either an ArcSelfArrow or an ArcDirArrow.
 * @param [in] data The type of the arrow segment (dotted, double, etc.) 
 *                  and the position of any loss indication (*).
 * @param [in] s The name of the source entity.
 * @param [in] sl The place where `s` is mentioned in the input file.
 * @param [in] d The name of the destination entity.
 * @param [in] fw True if the arrow is defined as 's->d'; false if 'd<-s'
 * @param [in] dl The place where `d` is mentioned in the input file.
 * @returns The created object(with the right default style).*/
ArcArrow *Msc::CreateArcArrow(ArrowSegmentData data, const char*s, const FileLineColRange &sl,
                              const char*d, bool fw, const FileLineColRange &dl)
{
    if (strcmp(s,d))
        return new ArcDirArrow(data, s, sl, d, dl, this, fw, Contexts.back().styles["arrow"]);
    if (data.lost != EArrowLost::NOT)
        Error.Error(data.lost_pos.CopyTo().start, "No support for arrows pointing to the same entity in this version. Ignoring asterisk.");
    StyleCoW style = Contexts.back().styles["arrow"];
    style.write().text.Apply("\\pr");
    return new ArcSelfArrow(data.type, s, sl, this, style, selfArrowYSize);
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

/** Creates a CommandEntity object for the two comment lines and areas.
 * @param [in] s The new style of the comment lines and areas - only the vline and fill part will be used.
 * @param [in] l The range of the input file, that results in the change of comment line style.
 * @returns The returned CommandEntity will hold two EntityApp objects: one for the left and 
 * one for the right virtual entity representing the line of the comments.*/
CommandEntity *Msc::CEForComments(const MscStyle &s, const FileLineColRange &l)
{
    EntityApp *led = new EntityApp(LNOTE_ENT_STR, this);
    led->SetLineEnd(l);
    led->style.write() += s;
    EntityAppHelper *ledh = led->AddAttributeList(NULL, NULL, FileLineCol());
    EntityApp *red = new EntityApp(RNOTE_ENT_STR, this);
    red->SetLineEnd(l);
    red->style.write() += s;
    EntityAppHelper *redh = red->AddAttributeList(NULL, NULL, FileLineCol());
    redh->Prepend(ledh);
    delete ledh;
    CommandEntity *ce = new CommandEntity(redh, this, true);
    ce->AddAttributeList(NULL);
    return ce;
}

/** Applies a chart option 
 * @param [in] a The attribute to add
 * @returns An arc to be inserted to the list of arcs at this point. 
 * These can be CommandBackground, CommandEntity (to change comment line style), or 
 * both of them in an ArcList (for applying design changes).*/
ArcBase *Msc::AddAttribute(const Attribute &a)
{
    //Chart options cannot be styles
    _ASSERT(a.type != MSC_ATTR_STYLE);

    if (a.Is("msc") || a.Is("msc+")) {
        const bool full = a.Is("msc");
        if (!a.CheckType(MSC_ATTR_STRING, Error)) return NULL;
        const FileLineColRange line(a.linenum_attr.start, a.linenum_value.end);
        ArcBase *ret;
        switch (SetDesign(full, a.value, false, &ret, line)) { 
        case 0:
            Error.Error(a, true, "Unknown chart design: '" + a.value +
                        "'. Ignoring design selection.",
                        "Available designs are: " + GetDesignNames(full) +".");
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
        Contexts.back().vspacing.first = true;
        Contexts.back().vspacing.second = a.yes ? DBL_MIN : 0;
        return NULL;
    }
    if (a.Is("vspacing")) {
        if (a.type == MSC_ATTR_STRING && a.value == "compress") {
            Contexts.back().vspacing.first = true;
            Contexts.back().vspacing.second = DBL_MIN;
        }
        if (!a.CheckType(MSC_ATTR_NUMBER, Error)) return NULL;
        Contexts.back().vspacing.first = true;
        Contexts.back().vspacing.second = a.number;
        return NULL;
    }
    if (a.Is("indicator")) {
        if (!a.CheckType(MSC_ATTR_BOOL, Error)) return NULL;
        Contexts.back().indicator.first = true;
        Contexts.back().indicator.second = a.yes;
        return NULL;
    }
    if (a.Is("auto_heading")) {
        if (!a.CheckType(MSC_ATTR_BOOL, Error)) return NULL;
        Contexts.back().auto_heading.first = true;
        Contexts.back().auto_heading.second = a.yes;
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
                                       false, true, StringFormat::LABEL);
        return NULL;
    }
    if (a.Is("numbering.post")) {
        Contexts.back().numberingStyle.post.first = true;
        Contexts.back().numberingStyle.post.second = a.value;
        StringFormat::ExpandReferences(Contexts.back().numberingStyle.post.second, this,
                                       a.linenum_value.start, NULL,
                                       false, true, StringFormat::LABEL);
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
        FillAttr fill;
        fill.Empty();
        if (fill.AddAttribute(a, this, STYLE_OPTION)) { //generates error if needed
            Contexts.back().defBackground += fill;
            ArcBase *arc = new CommandNewBackground(this, fill);
            arc->AddAttributeList(NULL);
            return arc;
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
                return CEForComments(toadd, FileLineColRange(a.linenum_attr.start, a.linenum_value.end));
            }
            //fallthrough till error if not "OK"
        } else if (CaseInsensitiveBeginsWith(a.name.substr(8), "side") ||
                   CaseInsensitiveBeginsWith(a.name.substr(8), "text")) {
            Attribute new_a(a);
            new_a.name.erase(0,8); //erase "comment."
            Contexts.back().styles["comment"].write().AddAttribute(new_a, this);
            return NULL;
        }
    }
    if (a.Is("classic_parallel_layout")) {
        if (!a.CheckType(MSC_ATTR_BOOL, Error)) return NULL;
        simple_arc_parallel_layout = a.yes;
        Error.Warning(a, false, "This chart option is deprecated and will be removed.",
            "Use the '[layout=overlap]' attribute before a series of parallel blocks to achieve the same effect.");
        return NULL;
    }
    if (a.Is("file.url")) {
        if (!a.EnsureNotClear(Error, STYLE_OPTION)) return NULL;
        file_url = a.value;
        return NULL;
    }
    if (a.Is("file.info")) {
        if (!a.EnsureNotClear(Error, STYLE_OPTION)) return NULL;
        if (file_info.length())
            file_info.append("\n").append(a.value);
        else
            file_info = a.value;
        return NULL;
    }

    string ss;
    Error.Error(a, false, "Option '" + a.name + "' not recognized. Ignoring it.");
    return NULL;
}

/** Add an attribute only if it can be part of a design. 
 * Other attributes trigger error. This is called when a design definition is in progress. 
 * @param [in] a The attribute to add.
 * @returns True if the attribute is recognized and added, false if not (error is generated)*/
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
    if (a.StartsWith("numbering") || a.Is("compress") || a.Is("auto_heading") || 
        a.Is("hscale") || a.Is("msc") || a.Is("msc+") || a.StartsWith("text") || 
        a.StartsWith("comment") || a.StartsWith("background")) {
        //Since we set chart options inside a design definition, 
        //we allow "msc = design" commands to take effect.
        bool saved_ignore_designs = ignore_designs;
        ignore_designs = false;
        ArcBase *ret = AddAttribute(a);
        ignore_designs = saved_ignore_designs;
        if (ret)
            delete ret;
        return true;
    }
error:
    Error.Error(a, false, "Cannot set option '" + a.name +
                  "' as part of a design definition. Ignoring it.");
    return false;
}

/** Adds the attribute names as hints to `csh`. 
 * If `designOnly` is true, only the attributes applicable for chart designs are added.*/
void Msc::AttributeNames(Csh &csh, bool designOnly)
{
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "msc", 
        "Apply a full or partial chart design using 'msc=<full_design> or msc+=<partial_design>, respectively.", 
        EHintType::ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "hscale", 
        "Set the horizontal scaling factor (default is 1) and thereby infuence the width of the chart. Use 'auto' for automatic scaling.",
        EHintType::ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "compress", 
        "Turn this on to shift all elements upwards until it bumps in to the element above resulting in compressing the chart vertically.",
        EHintType::ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "vspacing",
        "Specify the vertical spacing above each element.",
        EHintType::ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "auto_heading",
        "Turn this on to automatically display entity headings at the top of each page (after 'newpage;' commands).",
        EHintType::ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "numbering", 
        "Turning this on will make labelled elements auto-numbered.",
        EHintType::ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "numbering.pre", 
        "Set the text to prepend to label numbers. E.g., use 'Step ' to achieve 'Step 1', 'Step 2', etc.",
        EHintType::ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "numbering.post", 
        "Set the text to append to label numbers. E.g., use ':' to achieve '1:', '2:', etc.",
        EHintType::ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "numbering.format", 
        "Set the format of auto-numbering, including text format, number type (like roman numbers) and the number of numbering levels.",
        EHintType::ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "numbering.append", 
        "Append a new numbering level, to have, e.g., 2.1, which lasts until the next closing brace ('}').",
        EHintType::ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "numbering.*",
        "Auto numbering related options, like format or the number of levels.",
        EHintType::ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "angle",
        "Make arrows slanted by this degree.",
        EHintType::ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "background.color", 
        "Set the color of the background from this point downwards.",
        EHintType::ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "background.color2", 
        "Set the second color of the background (useful for two-color gradients).", 
        EHintType::ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "background.gradient", 
        "Set the gradient type of the background from this point downwards.", 
        EHintType::ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "background.*",
        "Set the background fill from this point downwards.",
        EHintType::ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "comment.line.color", 
        "Set the color of the line separating side comments from the chart (if any).",
        EHintType::ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "comment.line.type", 
        "Set the type of the line separating side comments from the chart (if any).",
        EHintType::ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "comment.line.width",
        "Set the width of the line separating side comments from the chart (if any).",
        EHintType::ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "comment.line.*",
        "Set the attributes of the line separating side comments from the chart (if any).",
        EHintType::ATTR_NAME));
    //csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "comment.line.radius", EHintType::ATTR_NAME));
    //csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "comment.line.corner", EHintType::ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "comment.fill.color",
        "Set the color of the background of the side comments (if any).",
        EHintType::ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "comment.fill.color2", 
        "Set the second color of the background of the side comments (if any), useful for two-color gradients.",
        EHintType::ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "comment.fill.gradient",
        "Set the gradient of the background fill of the side comments (if any).",
        EHintType::ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "comment.fill.*",
        "Set the attributes of the background fill of the side comments (if any).",
        EHintType::ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "comment.side", 
        "Use this option to set the default side for comments.",
        EHintType::ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "comment.*",
        "Options governing the appearance and location of chart comments.",
        EHintType::ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "file.url", 
        "Use this option to attach the URL of the file. Useful for design libraries.",
        EHintType::ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "file.info", 
        "Use this option to attach a description to the file. Useful for design libraries.", 
        EHintType::ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "file.*",
        "Use this option to attach a meta information to the file. Useful for design libraries.",
        EHintType::ATTR_NAME));
    
    StringFormat::AttributeNames(csh, "comment.text.");
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "comment.text.*",
        "Set the text attributes of the side comments (if any).",
        EHintType::ATTR_NAME));
    StringFormat::AttributeNames(csh, "text.");
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "text.*",
        "Set the default text attributes.",
        EHintType::ATTR_NAME));
    if (designOnly) return;
    
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "classic_parallel_layout", 
        "Turn this on to make parallel blocks be laid out allowing overlaps. (Deprecated, will be removed.)", 
        EHintType::ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_OPTIONNAME) + "pedantic", 
        "Turn this on to generate an error for entities that are used without explicit declaration.",
        EHintType::ATTR_NAME));
}

/** Adds the possible attribute values for a given attribute to csh.
 * Returns true if we have recognize the attribute*/
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
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number>", 
            "The default scaling is 1 and you can specify a multiplier. Use a larger number to make the space between entities larger.",
            EHintType::ATTR_VALUE, false));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE) + "auto", 
            "This automatically sets the spacing between the entities to just as much as needed.",
            EHintType::ATTR_VALUE));
        return true;
    }
    if (CaseInsensitiveEqual(attr,"angle")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number>", 
            "Makes arrows to slant downwards and is measured in degrees.",
            EHintType::ATTR_VALUE, false));
        return true;
    }
    if (CaseInsensitiveEqual(attr,"compress") ||
        CaseInsensitiveEqual(attr,"numbering") ||
        CaseInsensitiveEqual(attr,"auto_heading") ||
        CaseInsensitiveEqual(attr,"classic_parallel_layout") ||
        CaseInsensitiveEqual(attr,"pednatic")) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE) + "yes", NULL, EHintType::ATTR_VALUE, true, CshHintGraphicCallbackForYesNo, CshHintGraphicParam(1)));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE) + "no", NULL, EHintType::ATTR_VALUE, true, CshHintGraphicCallbackForYesNo, CshHintGraphicParam(0)));
        return true;
    }
    if (CaseInsensitiveBeginsWith(attr, "text") || 
        CaseInsensitiveBeginsWith(attr, "comment.text") )
        return StringFormat::AttributeValues(attr, csh);
    if (CaseInsensitiveBeginsWith(attr, "comment.line"))
        return LineAttr::AttributeValues(attr, csh);
    if (CaseInsensitiveBeginsWith(attr, "comment.fill"))
        return FillAttr::AttributeValues(attr, csh);
    if (CaseInsensitiveBeginsWith(attr, "comment.side")) {
        const char * const descriptions[] =
        {NULL, "Place comments on the left side.", "Place comments on the right side.", 
        "Place comments to the end of the chart (i.e., convert to endnotes).", ""};

        for (auto s = ESide::LEFT; s<=ESide::END; s = ESide(int(s)+1))
            if (IsValidSideValue(ESideType::ANY, s))
                csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE)+EnumEncapsulator<ESide>::names[unsigned(s)], 
                                       descriptions[unsigned(s)],
                                       EHintType::ATTR_VALUE, true, CshHintGraphicCallbackForSide, 
                                       CshHintGraphicParam(s)));
        return true;
    }

    if (CaseInsensitiveBeginsWith(attr,"background")) {
        FillAttr::AttributeValues(attr, csh);
        return true;
    }
    if (CaseInsensitiveEqual(attr,"numbering.pre")||
        CaseInsensitiveEqual(attr,"numbering.post")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<\"text\">", NULL, EHintType::ATTR_VALUE, false));
        return true;
    }
    if (CaseInsensitiveEqual(attr,"numbering.format")||
        CaseInsensitiveEqual(attr,"numbering.append")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<\"numbering format\">", NULL, EHintType::ATTR_VALUE, false));
        return true;
    }
    if (CaseInsensitiveEqual(attr, "file.url")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<\"quoted URL of file\">", NULL, EHintType::ATTR_VALUE, false));
        return true;
    }
    if (CaseInsensitiveEqual(attr, "file.info")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<\"quoted description of file\">", NULL, EHintType::ATTR_VALUE, false));
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
	const size_t len = strlen(input);
    Progress.RegisterBulk(MscProgress::PARSE, len);
    if (len > std::numeric_limits<unsigned>::max())
        Error.FatalError(FileLineCol(), "Input text is longer than 4Gbyte. Bailing out.");
    else 
        MscParse(*this, input, (unsigned)len);
}

/** Get an (unordered) list of entities that arrows and boxes in this list touche.
 * We also determine the general direction of the arrows contained within.
 * If there are arrows in both directions (or any bidir arrow) we returm
 * MSC_DIR_BIDIR. If there are no directional arrows (only self-arrows or boxes)
 * we return MSC_DIR_INDETERMINATE.
 * @param [in] al The list of arcs to consider.
 * @param el We append the resulting entity list to this.
 * @param [in] dir The direction of the arrows contained on a previous
 *                 call to this function (with which we combine our directions).
 * @return The direction of the arrows in `al` */
EDirType Msc::GetTouchedEntitiesArcList(const ArcList &al, EntityList &el,
                                        EDirType dir) const
{
    for (auto i : al) {
        EntityList el2(false);
        EDirType dir2 = i->GetToucedEntities(el2);
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
        for (auto ei2 : el2) 
            if (el.Find_by_Ptr(ei2) == el.end())
                el.Append(ei2);
    }
    return dir;
}

string Msc::Print(int indent) const
{
    string s = AllEntities.Print(indent).append("\n");
    s.append(Arcs.Print(indent)).append("\n");
    return s;
}

/** Perform post-parse processing on an ArcList 
 * See the notes for the libmscgen module for a details of what it includes.
 * @param [in] canvas The canvas that can be used to learn geometry (font sizes, etc.)
 * @param [in] hide True if this list will get hidden due to a box around it being collapsed.
 *                  We still post-parse process them to collect info, reflect chart options, etc.
 * @param arcs The list to process.
 * @param [in] resetiterators If true, we call each arc in the list with a freshly empty left
 *                            and right iterators - thus we do not attempt to collect the left
 *                            and right extents of the arc list. Useful only for the top level
 *                            arclist.
 * @param left If any element in the list touches an entity leftward than this, we update
 *             this parameter with that entity. Used for box auto-sizing.
 * @param right If any element in the right touches an entity leftward than this, we update
 *              this parameter with that entity. Used for box auto-sizing.
 * @param number This is the running value for automatic numbering. It always contain the next
 *               number to assign.
 * @param target Contains the arc to which a subsequent note or comment will be targeted.
 *               Updated on return for a potential note or command after this arc list.
*/
void Msc::PostParseProcessArcList(Canvas &canvas, bool hide, ArcList &arcs, bool resetiterators,
                                  EIterator &left, EIterator &right,
                                  Numbering &number, Element **target)
{
    if (arcs.size()==0) return;
    //If a CommandNote is immediately after an CommandEntity, take it out 
    //and temporarily store it in the CommandEntity (for re-insertation further below)
    //But only if the commandentity was not internally defined!!
    for (ArcList::iterator i = ++arcs.begin(); i != arcs.end(); /*none*/) {
        ArcList::iterator prev = i; prev--;
        CommandEntity *ce = dynamic_cast<CommandEntity *>(*prev);
        CommandNote *cn = dynamic_cast<CommandNote *>(*i);
        if (ce == NULL || cn == NULL || ce->internally_defined) {
            i++;
            continue;
        }
        ce->TmpStoreNote(cn);
        arcs.erase(i++);
    }

    //Main loop, PostParseProcess each arc in the list
    ArcBase *prev_arc = NULL;
    for (ArcList::iterator i = arcs.begin(); i != arcs.end(); /*none*/) {
        //Merge in internally defined ArcParallel's
        //We insert an internally defined ArcParallel at the beginning,
        //which contains entities for the comment lines. These shall be
        //merged with autogen Entities.
        //Internally defined ArcParallels sure have just one block.
        ArcParallel *par = dynamic_cast<ArcParallel*>(*i);
        if (par && par->internally_defined) {
            //insert them *after* i
            ArcList::iterator j = i;
            arcs.splice(++j, par->blocks.front().arcs);
            delete *i;
            arcs.erase(i++);
            continue;
        }

        if (resetiterators) {
            right = left = AllEntities.Find_by_Ptr(NoEntity);
            _ASSERT (left != AllEntities.end());
        }
        //Merge subsequent CommandEntities
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
        Element * const old_target = *target;
        ArcBase *replace = (*i)->PostParseProcess(canvas, hide, left, right, number, target, prev_arc);
        //if the new target is somewhere inside "i" (or is exactly == to "i")
        //NOTE: *target is never set to NULL, only to DELETE_NOTE or to an Arc
        if (*target != old_target && replace != (*i)) {
            //If we remove an arc that could have been noted, we set the target 
            //to DELETE_NOTE, as well, to remove the target, too.
            if (replace == NULL) 
                *target = DELETE_NOTE;
            //if we replace to a different Arc, redirect notes to that
            else if (replace->CanBeNoted())
                *target = replace;
            else 
                *target = old_target;
        }
        //If we are about to replace to an indicator, remove it,
        //if previous thing was also an ArcIndicator on the same entity
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
        
        //Ok, final actions: replacement plus report progress.
        //We wither remove i from the list or increase the iterator to the next element
		//Below we only do progress report for elements doneand not deleted
        if (replace == *i) {
            //The arc requested no replacement
			Progress.DoneItem(MscProgress::POST_PARSE, (*i)->myProgressCategory);
            //If this is an endnote, move it to EndNotes
            CommandNote *cn = dynamic_cast<CommandNote*>(*i);
            if (cn && cn->GetStyle().read().side.second == ESide::END) {
                EndNotes.Append(cn);
                arcs.erase(i++);
            } else {
                if ((*i)->CanBeAlignedTo())
                    prev_arc = *i;
                i++;
            }
		} else {
            //The arc requested a replacement
            //Note that any replacement already has its postparseprocess called
            delete *i; //destroy the arc (*i becomes invalid)
            if (replace) {
                Progress.DoneItem(MscProgress::POST_PARSE, replace->myProgressCategory);
                *i = replace;
                i++;
                if (replace->CanBeAlignedTo())
                    prev_arc = replace;
            } else 
                arcs.erase(i++); //Nothing to replace to: remove pointer from list
        }
    }
}

/** Performs the post parse processing for the whole chart 
 * See the notes for the libmscgen module for a details of what it includes.
 * (We sort entities, determine which shows, append automatically generated 
 * entities, and process Msc::Arcs.) */
void Msc::PostParseProcess(Canvas &canvas)
{
	Progress.StartSection(MscProgress::POST_PARSE);
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
    for (auto pEntity : ActiveEntities)
        pEntity->index = index++;

    //Find the first real entity
    EIterator iLeftmost = ActiveEntities.begin();
    while (iLeftmost != ActiveEntities.end() && IsVirtualEntity(*iLeftmost))
        iLeftmost++;

    //Ensure that leftmost real entity pos == 2*MARGIN
    //Note that the distance between LSide and the first real entity 
    //will be auto-scaled.
    double rightmost = 0;
    if (iLeftmost != ActiveEntities.end()) {
        double leftmost = (*iLeftmost)->pos - 2*MARGIN;
        for  (auto pEntity : AllEntities) 
            pEntity->pos -= leftmost;
        //Find rightmost entity's pos
        for  (auto pEntity : ActiveEntities) 
            if (!IsVirtualEntity(pEntity) && rightmost < pEntity->pos)
                rightmost = pEntity->pos;
    } else {
        //if there are no real entities, just noentity, lside & rside, set this wide
        //so that copyright banner fits
        rightmost = 3*MARGIN;
    }
    //Set the position of the virtual side entities & resort
    const_cast<double&>(NoEntity->pos) = 0;
    const_cast<double&>(LNote->pos) = 0;
    const_cast<double&>(LSide->pos) = MARGIN; 
    const_cast<double&>(RSide->pos) = rightmost + MARGIN; 
    const_cast<double&>(RNote->pos) = rightmost + MARGIN + MARGIN;
    const_cast<double&>(EndEntity->pos) = rightmost + MARGIN + MARGIN;
    ActiveEntities.SortByPos();

    if (Arcs.size()==0) return;

    if (AutoGenEntities.size()) {
        //Add the Auto generated entities to the first definition of entities
        //Search for this kind of first definition CommandEntity from the
        //beginning or Arcs, but skip objects of the following type
        //- CommandArcList: these are inserted by chart options only
        //- ArcDividers with no entities: these are likely (sub)title commands
        //If we find a CommandEntity having only the above before it,
        //append the automatically generated entities to that.
        //Otherwise, generate a new entity command at that location
        ArcList::iterator i = Arcs.begin();
        //Skip over the titles and chart options
        while (i!=Arcs.end() && ((*i)->type==MSC_ARC_ARCLIST || (*i)->type==MSC_COMMAND_TITLE ||
               (*i)->type==MSC_COMMAND_SUBTITLE))
            i++;
        //Insert a new CommandEntity if we have none
        if (i==Arcs.end() || (*i)->type != MSC_COMMAND_ENTITY) {
            CommandEntity *ce = new CommandEntity(new EntityAppHelper, this, false);
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
    Element *note_target = NULL;
    PostParseProcessArcList(canvas, false, Arcs, true, dummy1, dummy2, number, &note_target);

    //Reinsert end-notes at the end (they have been post-processed)
    if (EndNotes.size()) {
        //Add separator
        CommandEndNoteSeparator *ens = new CommandEndNoteSeparator(this);
        ens->AddAttributeList(NULL);
        Arcs.Append(ens);
        //Append end-notes
        Arcs.splice(Arcs.end(), EndNotes);
    }
}

/** Draw the entity lines in a vertical region and between two entities 
 * We draw entity lines according to their styles and activation status.
 * We do not draw them where they shall be hidden (by, e.g., a label).
 * @param canvas The canvas onto which w draw.
 * @param [in] y Top of the region to draw in
 * @param [in] height Height of the region to draw in
 * @param [in] from The leftmost entity to draw the line for
 * @param [in] to The entity right of the rightmost entity to draw for.*/
void Msc::DrawEntityLines(Canvas &canvas, double y, double height,
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
            const MscStyle &style = status.GetStyle(up.y).read();
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
                const bool doClip = outer_edge.y.from < up.y || outer_edge.y.till > down.y;
               
                if (doClip)
                    canvas.Clip(Block(outer_edge.x, Range(up.y, down.y)));
                outer_edge.Expand(-style.vline.LineWidth()/2);  //From now on this is the midpoint of the line, as it should be
                canvas.Fill(style.vline.CreateRectangle_ForFill(outer_edge), style.vfill);
                canvas.Line(style.vline.CreateRectangle_Midline(outer_edge), style.vline);
                if (doClip)
                    canvas.UnClip();
            } else {
                const double offset = canvas.HasImprecisePositioning() ? 0 : fmod_negative_safe(style.vline.width.second/2, 1.);
                if (canvas.NeedsStrokePath_rclBoundsFix()) {
                    /* We draw such a shape for an entity line:
                     *  .....   Where dotted lines are invisible.
                     *  :   :   The visible vertical part is the entity line
                     *  : +--   The visible horizontal parts are serifs to expand the
                     *  : |     bounding box of the stroke.
                     *  : |     This is needed so that the resulting metafile
                     *  --+     retains the entity line even when shrunk.
                     *          Surprisingly the bounds remain large even if we clip 
                     *          away the serifs (which we do). */
                    const double gap = 10 + style.vline.LineWidth();
                    const Block clip (up   + XY(-gap+offset,0), 
                                      down + XY( gap+offset,0));
                    canvas.Clip(clip);
                    const XY points[] = {down + XY(-2*gap+offset,  +gap),
                                         up   + XY(-2*gap+offset, -gap*2),
                                         up   + XY(+2*gap+offset, -gap*2),
                                         up   + XY(+2*gap+offset, -gap),
                                         up   + XY(     0+offset, -gap),
                                         down + XY(     0+offset, +gap)};
                    Contour line_shape;
                    line_shape.assign_dont_check(points);
                    line_shape[0][0].visible = false;
                    line_shape[0][1].visible = false;
                    line_shape[0][2].visible = false;
                    canvas.Line(line_shape, style.vline);
                    canvas.UnClip();
                } else 
                    canvas.Line(up+XY(0,offset), down+XY(0,offset), style.vline);
            }
        }
        from++;
    }
    canvas.UnClip();
}

/** Calls the Width() function for all arcs in the list and collects distance requirements to `distances` */
void Msc::WidthArcList(Canvas &canvas, ArcList &arcs, EntityDistanceMap &distances,
                       DistanceMapVertical &vdist)
{
    //Indicate entities active and showing already at the beginning of the list
    //(this will be updated with entities activated later)
    //(so that we can calcualte with the width of their entityline)
    for (auto pEntity : ActiveEntities) 
        if (pEntity->running_shown == EEntityStatus::SHOW_ACTIVE_ON) 
            distances.was_activated.insert(pEntity->index);
    for (auto pArc : arcs) {
        pArc->Width(canvas, distances, vdist);
		Progress.DoneItem(MscProgress::WIDTH, pArc->myProgressCategory);
	}
}


/** Places a full list of elements starting at y position==0
 * Calls Layout() for each element (recursively) and takes "compress" and 
 * "parallel" attributes into account.
 * We always place each element on an integer y coordinate.
 * Automatic pagination is ignored by this function and is applied later instead.
 * Ensures that elements in the list will have non-decreasing yPos order - 
 * thus an element later in the list will have same or higher yPos as any previous.
 * @param canvas The canvas to calculate geometry on.
 * @param arcs The list of arcs to place
 * @param cover We add the area covered by each arc to this list.
 * @returns the total height of the list*/
double Msc::LayoutArcList(Canvas &canvas, ArcList &arcs, AreaList *cover)
{
    std::list<LayoutColumn> y;
    y.emplace_back(&arcs, 0);
    return LayoutParallelArcLists(canvas, y, cover);
}


/** Find how much we need to push arc_cover down (assumed to be laid out at y==0)
 * to be below all children of 'sibling' (but not 'sibling' itself, just its
 * 'completed_children_covers'). We completely ignore mainlines here. 
 * Uses offsets meaning upwards movement, as OffsetBelow.*/
double FindOffsetInChildren(const std::list<LayoutColumn> &columns, const LayoutColumn *sibling,
                            const AreaList &arc_cover, double &touchpoint, double offset)
{
    offset = sibling->completed_children_covers.OffsetBelow(arc_cover, touchpoint, offset, false);
    for (auto &col : columns)
        if (col.parent == sibling) {
            offset = col.covers.OffsetBelow(arc_cover, touchpoint, offset, false);
            offset = col.completed_children_covers.OffsetBelow(arc_cover, touchpoint, offset, false);
            if (col.number_of_children)
                offset = FindOffsetInChildren(columns, &col, arc_cover, touchpoint, offset);
        }
    return offset;
}

/** Finds how much 'arc_cover' (laid out at y==0) shall be shifted down not to 
 * collide with previously laid out elements of 'columns'. 
 * In general we test our own column and sibling colums - but for sibling columns
 * we ignore mainlines, since we can be besides elements of sibling columns, there
 * only overlap needs to be avoided.
 * Since LayoutColumn::covers only contain the cover of the elements in that column,
 * and nothing from the parent column, we also need to test the parents (and the
 * sibling of the parents, without mainlines, too).
 * So the algoritm starts form the current column 'to_test' checks offset of it and
 * its siblings (mainlines ignored), then steps to the parent and repeats until
 * we check the siblings at the top level (where parent==NULL).
 * 'compress' tells us if the arc in question shall be compressed or not.
 * In the latter case there is no need to check if the element to be placed 
 * collides with prior elements in its column or that of parents, since we 
 * will attempt to place it strictly below all those. So as a performance
 * optimization we avoid calling OffsetBelow().
 * @param [in] columns The list of columns under processing
 * @param [in] to_test The column we are processing now.
 * @param [in] arc_cover The cover of the element we place, assumed to be laid out at y==0.
 * @param [out] touchpoint The y value at which the prior elements and this element touches.
 * @param [in] compress True if the element will be compressed (or the prior element was 
 *                      parallel) and we need to check elements in our column, too.
 * @returns how much we shall shift the element down (from its current position at y==0)*/
double FindOffset(const std::list<LayoutColumn> &columns, const LayoutColumn *to_test, 
                  const AreaList &arc_cover, double &touchpoint, bool compress)
{
    //now try pushing upwards against previous elements in this column,
    //against the body (no mainline) of previous elements parallel to us
    //(sibling columns) and all our parents (and their siblings).
    double offset = CONTOUR_INFINITY;
    while (to_test) {
        for (auto &col : columns)
            if (&col == to_test->parent)
                //For our parents, we test against any covers (without mainlines) 
                //that are from completed sibling columns of ours.
                offset = col.completed_children_covers.OffsetBelow(arc_cover, touchpoint, offset, false);
            else if (col.parent == to_test->parent && (compress || &col!=to_test)) {
                //For our column we consider the mainlines, but for
                //neighbouring columns (the same parent as our column, but not our column), 
                //we do not (we can be besides them, but not overlap)
                offset = col.covers.OffsetBelow(arc_cover, touchpoint, offset, &col==to_test);
                //for our siblings, we traverse their children - we need to avoid those, too.
                if (&col!=to_test && col.number_of_children)
                    offset = FindOffsetInChildren(columns, &col, arc_cover, touchpoint, offset);
            }
            to_test = to_test->parent;
    }
    return -offset;
}

/** Places a set of parallel lists of elements starting at y position==0.
 * Calls Layout() for each element (recursively) and takes "compress" and
 * "parallel" attributes into account.
 * Attempts to avoid collisions and a balanced progress in each list.
 * If we encounter ArcParallels with layout==one_by_one_merge, we do not
 * call their ArcParallel::Layout(), but merge them among these lists.
 * This is the main (only) layout function for lists, this one is used
 * even for non-parallel lists.
 * We always place each element on an integer y coordinate.
 * Automatic pagination is ignored by this function and is applied later instead.
 * Ensures that elements in the list will have non-decreasing yPos order -
 * thus an element later in the list will have same or higher yPos as any previous.
 * @param canvas The canvas to calculate geometry on.
 * @param columns The list of arc lists to place
 * @param cover We add the area covered by each arc to this list, if non NULL.
 * @returns the total combined height of the lists.*/
double Msc::LayoutParallelArcLists(Canvas &canvas, std::list<LayoutColumn> &columns, AreaList *cover)
{                              
    //we count arcs we placed, so that we can see which column got treated last
    unsigned current_action = 1;
    //use this counter to add new columns
    unsigned counter = columns.size();
    //The lowestmost element - to return as height
    double total_bottom = 0;     
    //cover of those elements that were in top-level columns (no parent) 
    //that have completed. It is important to keep this to avoid
    //overlap with completed columns (since we delete completed columns)
    AreaList top_completed_children_cover;

    //test for fast path
    //if 0) caller requested no cover returned; 1) we do a single column only; 
    //2) have no ArcParallels with ONE_BY_ONE_MERGE;
    //3) have no elements marked as compress and 4) have no elements marked as parallel
    //Then we do not need to maintain covers
    const bool need_covers = cover || columns.size() != 1 ||
        columns.front().list->end() != std::find_if(columns.front().list->begin(), columns.front().list->end(),
        [](const ArcBase* const arc) {return arc->IsParallel() || arc->IsCompressed() ||
        (dynamic_cast<const ArcParallel*>(arc) && 
         dynamic_cast<const ArcParallel*>(arc)->layout==ArcParallel::ONE_BY_ONE_MERGE &&
         dynamic_cast<const ArcParallel*>(arc)->blocks.size()>1); });

    while (columns.size()) {
        //Select the next column to pick an element from.
        //- lists with no children are sorted earlier (we must have at least 
        //  one column with no children)
        //- among those we selet the one with lowest y, 
        //- if there are several with the same y then we select the one with 
        //  the lowest 'last_action' (the one we took an element longest time ago)
        //- finally (and this can happen only after the insertion of new columns from 
        //  an ArcParallel where last action is all equal) we tie break on the 'column' number 
        LayoutColumn &now = *std::min_element(columns.begin(), columns.end(),
            [](const LayoutColumn &a, const LayoutColumn &b) 
            {return std::tie(a.number_of_children, a.y, a.last_action, a.column) < 
                    std::tie(b.number_of_children, b.y, b.last_action, b.column);});
        _ASSERT(now.number_of_children==0);
        _ASSERT(now.list->end()!=now.arc);
        //Zero-height arcs shall be positioned to the same place
        //as the first non-zero height arc below them (so that
        //if that arc is compressed up, they are not _below_
        //that following arc. So we store them until we know
        //where shall we put them.
        ArcList zero_height(false);

        //Start cycle till the last element (arc) in the list, but we 
        //will exit as soon as we added an element of nonzero height.
        for (; now.arc!=now.list->end(); now.arc++) {
            _ASSERT((*now.arc)->IsValid()); //should have been removed by PostParseProcess
            AreaList arc_cover;
            //Check if this is a parallel blocks with one_by_one_merge layout
            ArcParallel *par = dynamic_cast<ArcParallel*>(*now.arc);
            if (par && par->IsValid() && par->blocks.size() &&
                par->layout==ArcParallel::ONE_BY_ONE_MERGE) {
                //'comp' tells us if the first element can be compressed or not
                //if not, we set the upper limit to use_y
                const bool comp = now.previous_was_parallel || par->IsCompressed();
                const double upper_limit = comp ? now.y_upper_limit : now.y;
                //Store value for y_upper_limit to be used after all colums of this 
                //ArcParallel has been completed.
                now.y_upper_limit = now.y;
                //do not consider vspacing if at the beginning of a scope.
                if (now.y) now.y += par->GetVSpacing();
                par->SetYPos(now.y);
                for (auto &col : par->blocks)
                    columns.emplace_back(&col.arcs, counter++, now.y, upper_limit,
                                         comp, &now, now.last_action);
                now.number_of_children = par->blocks.size();
                Progress.DoneItem(MscProgress::LAYOUT, (*now.arc)->myProgressCategory);
                break; // this will jump over element processing
                //now.arc is left pointing to the ArcParallel
                //now.y_upper_limit points to upper limits used if this ArcParallel is 'parallel'
                //Note: we kept any items potentially in zero_length. They will be 
                //placed at the top of the first non-zero element in one of the columns now inserted
            }

            (*now.arc)->Layout(canvas, need_covers ? &arc_cover : NULL);
            Progress.DoneItem(MscProgress::LAYOUT, (*now.arc)->myProgressCategory);
            double h = (*now.arc)->GetFormalHeight();
            //increase h, if arc_cover.Expand() (in "Layout()") pushed outer boundary. This ensures that we
            //maintain at least compressGap/2 amount of space between elements even without compress.
            //Note we fail to do this unless need_cover is true. No problem for now.
            h = std::max(h, arc_cover.GetBoundingBox().y.till);
            //if arc is of zero height, just collect it.
            //Its position may depend on the next arc if that is compressed.
            if (h==0) {
                zero_height.Append(*now.arc);
                continue;
            }
            now.last_action = ++current_action;
            double touchpoint = now.y;
            double use_y = now.y;
            if ((*now.arc)->IsCompressed() || now.previous_was_parallel) {
                //now try pushing upwards against previous elements in this column,
                //against the body (no mainline) of previous elements parallel to us
                //(sibling columns) and all our parents (and their siblings).
                double offset = FindOffset(columns, &now, arc_cover, touchpoint, true);
                offset = -top_completed_children_cover.OffsetBelow(arc_cover, touchpoint, -offset, false);
                use_y = std::max(now.y_upper_limit, offset);
                if (!(*now.arc)->IsCompressed()) {
                    //we must have previous_was_parallel==true here
                    //if the immediately preceeding element (not including zero_height_ones) was
                    //marked with "parallel", we attempt to shift this element up to the top of that one
                    //even if the current element is not marked by "compress".
                    //If we can shift it all the way to the top of the previous element, we place it
                    //there. But if we can shift only halfway, we place it strictly under the previous
                    //element - as we are not compressing.
                    //Note that "y_upper_limit" contains the top of the preceeding element (marked with "overlap")
                    if (use_y == now.y_upper_limit)
                        touchpoint = now.y_upper_limit;
                    else
                        //Note: OffsetBelow() may have destroyed touchpoint above
                        touchpoint = use_y; //(which is == now.y). 
                }
            } else {
                //This element is not compressed. But as we may lay out parallel blocks
                //it may be that we overlap with an element from anothe column, so we want to avoid that.
                //But we place zero_height elements to just below the previous one nevertheless
                //We also keep touchpoint==now.y for this very reason.
                double dummy_touchpoint;
                double offset = FindOffset(columns, &now, arc_cover, dummy_touchpoint, false);
                offset = -top_completed_children_cover.OffsetBelow(arc_cover, touchpoint, -offset, false);
                use_y = std::max(use_y, offset);
            }
            //Now use_y contains where this element shall be placed, and now.y the
            //bottommost in this column (and its parents).
            //Add extra space (even if above was parallel), move touchpoint by half
            //But do not add extra space at the beginning of a scope
            const double extra = now.y ? (*now.arc)->GetVSpacing() : 0;
            touchpoint += extra/2;
            use_y += extra;
            touchpoint = floor(touchpoint+0.5);
            use_y = ceil(use_y);
            //We got a non-zero height or a non-compressed one, flush zero_height ones (if any)
            for (auto pArc : zero_height)
                pArc->ShiftBy(touchpoint);
            zero_height.clear();
            //Shift the arc in question to its place
            (*now.arc)->ShiftBy(use_y);
            arc_cover.Shift(XY(0, use_y));
            //If the caller wanted to have all covers back, add this one to the list.
            if (cover)
                *cover += arc_cover;
            //update column state 
            now.y_upper_limit = use_y; //Do not allow anyone later in the list to be placed above us
            now.previous_was_parallel = (*now.arc)->IsParallel();            
            now.y_bottom_all = std::max(now.y_bottom_all, use_y+h);
            if ((*now.arc)->IsOverlap()) {
                //Set the normal (non-compressed pos of the next arc)
                //as the top of the current arc
                now.y = use_y;
            } else {
                now.y = std::max(now.y, use_y + h);
                //Update covers 
                if ((*now.arc)->IsParallel()) 
                    arc_cover.InvalidateMainLine(); //kill the mainline if we are parallel
                now.covers += arc_cover; //arc_cover contains the mainline here (unless parallel)
            }
            //This was a non-zero height element, we break and pick
            //the next arc from the column with the topmost current bottom.
            now.arc++; //for loop increment will not be called, so we increment now.arc here.
            break;
        } //for cycling the zero height elements
        //We have changed now.y, now.last_action (or have inserted new columns
        //if we process an ArcParallel of ONE_BY_ONE_MERGE layout), so 
        //the "smallest" element in y has changed.
        //if this column is not done repeat cyclye - at the beginning
        //we will select a new column to pick an element from.
        if (now.arc!=now.list->end())
            continue;

        //Ok, we have finished with this column - close it.
        //first, position any remaining zero-heright items at the bottom
        for (auto pArc : zero_height)
            pArc->ShiftBy(now.y);
        zero_height.clear();
        LayoutColumn* us = &now;
        while (1) {
            //Next, see if we have a parent.
            LayoutColumn* parent = us->parent;
            if (parent) {
                //If so, check if that has any 
                //other children left. If not continue processing that column.
                //If that column is also completed, check if that has any parent too.
                //->this leads to a cycle as long as we have childless parents of 
                //completed columns.
                //Start by updating our parent column
                if (!(*parent->arc)->IsOverlap()) {
                    //if we are not part of an ArcParallel that has been marked as 'overlap'
                    //move the bottom (where the next uncompressed will be placed)
                    parent->y = std::max(parent->y, us->y);
                }
                //bottom_all needs to be moved always to be able to calculate total height
                parent->y_bottom_all = std::max(parent->y_bottom_all, us->y_bottom_all);
                parent->last_action = std::max(parent->last_action, us->last_action);
                //copy cour cover to the cover of the parent column
                parent->completed_children_covers += std::move(us->covers);
            } else
                top_completed_children_cover += std::move(us->covers);
            total_bottom = std::max(total_bottom, us->y_bottom_all);
            //erase 'us' from the list
            columns.erase(std::find_if(columns.begin(), columns.end(),
                [us](const LayoutColumn &a) {return &a==us; }));
            //'us' is now invalid
            if (!parent)
                break; //stop if no parent
            _ASSERT(parent->number_of_children);
            _ASSERT(parent->list->end() != parent->arc);
            //decrement the children list of the parent
            if (--parent->number_of_children == 0) {
                //OK now parent has no more children, continue processing arcs in it.
                //this is testing if the parallel block has 'parallel' keyword in front
                parent->previous_was_parallel = (*parent->arc)->IsParallel();
                if ((*parent->arc)->IsOverlap()) {
                    //and keep the position of the next element the same
                    parent->y = parent->y_upper_limit;
                } else {
                    if (parent->previous_was_parallel) 
                        //if ArcParallel was 'parallel', remove the mainline of all elements inside the parallel block
                        parent->completed_children_covers.InvalidateMainLine();
                    //copy children's cover to us (does not happen if ArcParallel was 'overlap')
                    parent->covers += std::move(parent->completed_children_covers);
                }
                //set the height of the ArcParallel we have just completed
                //(for verticals if they refer to it)
                ArcParallel * const pArc = dynamic_cast<ArcParallel*>(*parent->arc);
                _ASSERT(pArc);
                pArc->SetBottom(parent->y_bottom_all);
                //go to next arc - when we added parallel blocks, we left 'arc' pointing to the ArcParallel
                parent->arc++;
                if (parent->arc != parent->list->end())
                    break;
                //OK 'parent' is a completed column - do the cycle again.
                us = parent;
            } else //parent->number_of_chilren > 0
                break; //if the parent has children, we do not delete the parent, just continue processing columns
        } //while(proc->parent)
    }
    return total_bottom;
}


/** Places a list of arcs at below an already laid out part of the chart.
 * The list of arcs is normally placed at y coordinate `start_y` (which is supposed 
 * to be well below the already laid out part. The elements in the arc list are
 * placed by calling their Layout() and ShiftBy() functions.
 * Then, if the first element has its 'compress' attribute set or the `forcecompress`
 * parameter is true, it shifts the whole newly laid out list upwards until it bumps 
 * into `area_top`, but no higher than `top_y`.
 * The whole reason for this function is to prevent a big empty space
 * between the first and second arrow, when the first arrow can be
 * compressed a lot, but the second not. Here we move all the arrows (arcs)
 * as one block only up till sone of them collides with something.
 * No matter what input parameters we get we always place the list at an integer
 * y coordinate.
 * @param canvas The canvas to calculate geometry on.
 * @param arcs The list of arcs to place.
 * @param [in] start_y Place the list here (or above if we compress)
 * @param [in] top_y Never compress the top of the list above this y coordinate.
 * @param [in] area_top When the list is compressed, avoid overlap with these areas
 * @param [in] forceCompress Always attempt to move the list upwards, even if the 
 *                           first arc has its vspacing attribute set to non DBL_MIN.
 * @param [out] retCover If not NULL, we return the resulting cover of the list 
 *                       at the final position placed.
 * @returns The bottommost y coordinate touched by any arc on the list after completion. */
double Msc::PlaceListUnder(Canvas &canvas, ArcList &arcs, double start_y,
                           double top_y, const AreaList &area_top, bool forceCompress,
                           AreaList *retCover)
{
    if (arcs.size()==0) return 0;
    AreaList cover;
    double h = LayoutArcList(canvas, arcs, &cover);
    double touchpoint;
    double new_start_y = std::max(top_y, -area_top.OffsetBelow(cover, touchpoint));
    //if we shifted up, apply shift only if compess is on
    if (new_start_y < start_y) {
        if (forceCompress || (*arcs.begin())->IsCompressed()) 
            start_y = new_start_y;    
    } else //if we shifted down, apply it in any case
        start_y = new_start_y;
    //Add extra space (even if above was parallel)
    start_y = ceil(start_y + (*arcs.begin())->GetVSpacing());
    ShiftByArcList(arcs, start_y);
    if (retCover)
        retCover->swap(cover.Shift(XY(0, start_y)));
    return start_y + h;
}

/** Shifts a whole arc list up or down */
void Msc::ShiftByArcList(ArcList &arcs, double y)
{
    for (auto pArc : arcs) 
        pArc->ShiftBy(y);
}

/** Inserts an automatic page breaks to a list of arcs
 * @param canvas The canvas to calculate geometry on.
 * @param arcs The list of arc lists to insert the page break into
 * @param [in] i The arcs before which the page break shall be inserted.
 * @param [in] pageBreak The y coordinate of the page break in chart space
 * @param [in] addHeading If true, the page break will also display an automatic heading.*/             
void Msc::InsertAutoPageBreak(Canvas &canvas, ArcList &arcs, ArcList::iterator i,
                              double pageBreak, bool addHeading)
{
    CommandNewpage *cnp = new CommandNewpage(this, false);
    const Attribute a("auto_heading", addHeading, FileLineColRange(), FileLineColRange());
    cnp->AddAttribute(a);
    cnp->AddAttributeList(NULL);
    //We skip FinalizeLabels as they do nothing for CommandNewpage nor CommandEntity
    EIterator dummy1 = AllEntities.Find_by_Ptr(NoEntity);
    EIterator dummy2 = AllEntities.Find_by_Ptr(NoEntity);
    Numbering dummy3; //will not be used by a CommandEntity
    Element *dummy4 = NULL; //target of any notes, pretend we have none  
    cnp->PostParseProcess(canvas, false, dummy1, dummy2, dummy3, &dummy4, NULL); //if addheading is set it will generate a CommandEntity and call its PostParseProcess
    EntityDistanceMap distances;
    DistanceMapVertical vd;
    cnp->Width(canvas, distances, vd);
    cnp->Layout(canvas, NULL);
    cnp->ShiftBy(pageBreak);
    arcs.insert(i, cnp);
}


//Walk with keep_with_next__from in 'arcs' ultil 'i' and
//record the max bottom in lowest
//Set keep_with_next__from to arcs.end() in the end
void clear_keep_with_next__from(ArcList &arcs, ArcList::iterator &keep_with_next__from,
                                ArcList::iterator i, double &lowest)
{
    if (keep_with_next__from != arcs.end()) {
        do {
            lowest = std::max(lowest, (*keep_with_next__from)->GetVisualYExtent(true).till);
        } while (keep_with_next__from++!=i);
        keep_with_next__from = arcs.end();
    } else
        lowest = std::max(lowest, (*i)->GetVisualYExtent(true).till);
}

/** Walks an ArcList and inserts a page break and optionally a heading.
 *
 * We do not insert a page break, if the indicated position is all above or
 * below the whole list.
 * We also do not insert a page break, if it has already been inserted,
 * in that case we merely shift the elements that fall after the page
 * break down to accomodate a potential heading.
 * Elements that the page break goes through that can split themselves
 * around the page break, will do so. Others will be shifted to the next
 * page.
 * @param canvas Used to determine the size of the heading to insert.
 * @param arcs The list to walk. Can get modified if we insert a page
 *             break, plus some elements may get shifted down.
 * @param [in] netPrevPageSize The size of the page before the pageBreak to insert.
 *             Used to determine if a non-splittable object is larger 
 *             than the full page. In this case we break it.
 * @param [in] pageBreak The y coordinate where we shall insert the page break.
 *                       Essentially the bottom of the page.
 *                       Note that due to rounding errors, all elements shifted
 *                       to the next page will see the page to start at
 *                       `pageBreak+1`. Also, we shift or split elements if they 
 *                       are not completely above `pageBreak-1`
 * @param addCommandNewpage True if no CommandNewPage is added yet and it needs 
 *                    to be added. If we have inserted the CommandNewPage
 *                    object, then false is returned in it.
 * @param [in] addHeading True if we shall add a heading after automatic
 *                        page breaks.
 * @param [in] canChangePBPos If true, we can change the page break 
                              position. Due to the fact that we do a single-
                              step process, this shall be set only for
                              the top level Msc::Arcs list.
   @param [in] dontshiftall If true and all elements of the list need to be
                            shifted to the next page, do nothing. This is used
                            for boxes, when the label of the box wants to be kept
                            on the same page as the first contenet element.
   @returns How much the list got longer, or -1 if `dontshiftall` was true
            and all elements of the page would need to be shifted.
 */
double Msc::PageBreakArcList(Canvas &canvas, ArcList &arcs, double netPrevPageSize,
                             double pageBreak, bool &addCommandNewpage, bool addHeading,
                             bool canChangePBPos, bool dontshiftall)
{
    //if no arcs or we are all below the page Break, there is nothing to do
    //Here we test for formal beginning as it may be that it is not the first element 
    //that has its visual top the topmost.
    if (arcs.size()==0 || (*arcs.begin())->GetFormalPos() > pageBreak) return -1;
    double shift = 0;
    /** Indicates which element was the first in a series of elemenst
     * where all of them have `keep_with_next` or `parallel` set.
     * Will be maintained only as long as `shift` is zero and used only
     * when it becomes non-zero, thus we abandon its value after. */
    ArcList::iterator keep_with_next__from = arcs.end();
    double lowest_on_prev_page = pageBreak - netPrevPageSize+1;
    double lowest_on_next_page = pageBreak+1;
    double topmost_in_kwn = 0;
    for (auto i=arcs.begin(); i!=arcs.end(); i++) {
        /* Our rules on how to handle stuff.
         * SplitByPageBreak() must be called for all elements, as this is
         * responsible to maintain the running state of entities.
         * The main tool to handle pagination is that we insert the page
         * break and shift all elements below to get to the next page.
         * We maintain a "shift" value showing how much we need to
         * shift upcoming elements in the list. As a general rule
         * this value can only increase, to preserve layout.
         * However, if at any element this would push the topmost
         * point of that element much below than the bottommost point of
         * prior elements, we reduce shift for these two points to equal.
         * (This is a poor measure to prevent full re-layout.)
         * To this end we also maintain "lowest_on_next_page",
         * in which we maintain the lowest point of elements
         * on the next page processed (after shifted).
         * We also maintain 'topmost_in_kwn' holding the topmost
         * point among the elements between 'lowest_till_kwn_from'
         * and 'i' - undefined if 'lowest_till_kwn_from' is arcs.end().
         * We also maintain "lowest_on_prev_page" to see if at the end
         * we can move the page break upwards (only if canChangePBPos
         * is true).
         * Plus, we pay attention not to insert a page break, where the
         * next upcoming element is a (manual) page break (ignoring
         * zero-height elements in-between).
         * Thus the rule is
         * - If the element is fully above the page break, we leave it.
           - If the element is on the page break,
              + if this is the first such element, we shift it below
                as much as needed to get to the top of the new page.
              + if there have been previous shifts, ideally we should
                reflow the chart. For that we would need a copy of
                the cover for all elements - I deem that too expensive now.
                So for now we merely attempt to shift so that it falls
                below the page break - and increase the shift by that
                much.
           - If the element is fully below the page break, we shift it as
             much as the maximum of the shift yet.
         */
        ArcList res;
        const double s = (*i)->SplitByPageBreak(canvas, netPrevPageSize, pageBreak,
                                                addCommandNewpage, addHeading, res);
        if (s==-2) {
            //ignore (verticals, etc)
            //but break keep_with_next series.
            //Note that the keep_with_next list contains elements only before
            //we have reached pageBreak - after that it is always empty
            if (keep_with_next__from != arcs.end())
                clear_keep_with_next__from(arcs, keep_with_next__from, i, lowest_on_prev_page);
            continue;
        }
        _ASSERT(lowest_on_prev_page<=pageBreak);
        const Range r = (*i)->GetVisualYExtent(true);
        lowest_on_next_page = std::max(lowest_on_next_page, r.till);
        if (s>=0) {
            //The element fell on the page break and split itself.
            ///We do not shift it, just increase shift for subsequent elements.
            if (res.size()) {
                //The element has split itself and some parts of it ended up in
                //new elements. These are returned in "res". We need to insert
                //those after the current element, but add page break before.
                auto j = i;
                j++; //the element following the current element - insert before
                if (addCommandNewpage)
                    InsertAutoPageBreak(canvas, arcs, j, pageBreak, addHeading);
                addCommandNewpage = false;
                i = j; i--; //The page break (or heading) - continue processing just after this
                arcs.splice(j, res, res.begin(), res.end());
            }
            shift = ceil(std::max(shift, s)); //increase shift for later elements
            //we do not shift `i`, but perhaps later elements we got in 'res'
            //we also clear kwn_from so that we do not get shifted by a later element
            keep_with_next__from = arcs.end();
            lowest_on_prev_page = pageBreak-1; //previous page is full.
            dontshiftall = false; //we kept something on previous page, so we will not shift all anyway
            continue;
        }
        _ASSERT(res.size()==0);
        _ASSERT(s==-1);
        //if the element is fully above the page break,
        //we do nothing, just maintain keep_with_next__from
        if (r.till <= pageBreak-1) {
            //if we keep an element of nonzero height on the
            //previous page, clean `dontshiftall`, since we will not shift
            //all by now.
            if (r.from < r.till)
                dontshiftall = false;
            if (!(*i)->IsKeepWithNext() && !(*i)->IsParallel()) {
                clear_keep_with_next__from(arcs, keep_with_next__from, i, lowest_on_prev_page);
                continue;
            }
            if (keep_with_next__from == arcs.end()) {
                keep_with_next__from = i;
                topmost_in_kwn = r.from;
            } else
                topmost_in_kwn = std::min(topmost_in_kwn, r.from);
            continue;
        }
        //if the element falls on the page break or is below and cannot split itself
        //- we check `dontshiftall` - if true we return from this function with -1
        //- we insert PB (if not yet inserted)
        //- adjust the shift, so that we do not overshift

        if (dontshiftall) return -1; //we may have not shifted anyone yet - the list is intact

        if (addCommandNewpage) {
            //Insert CommandPageBreak if needed. It may modify headingSize.
            //Insert only if next element is not a page break.
            //Insert before keep_with_next__from or if that is arcs.end() then before 'i'.

            //first find if next element is a page break
            auto iii = keep_with_next__from==arcs.end() ? i : keep_with_next__from;
            while (iii != arcs.end() && (*iii)->GetFormalHeight()==0 &&
                   (*iii)->type!=MSC_COMMAND_NEWPAGE)
                iii++;
            if (iii != arcs.end() && (*iii)->type==MSC_COMMAND_NEWPAGE) {
                //an upcoming page break. Adjust shift so that that element gets to `pageBreak`
                shift = ceil(pageBreak - (*iii)->GetVisualYExtent(true).from);
                keep_with_next__from = arcs.end(); //do not move prior elements
            } else {
                InsertAutoPageBreak(canvas, arcs,
                                    keep_with_next__from == arcs.end() ? i : keep_with_next__from,
                                    pageBreak, addHeading);
                addCommandNewpage = false;
            }
            lowest_on_next_page = std::max(lowest_on_next_page, pageBreak+1); //next page empty
        }

        //ensure we fall on the next page
        if (keep_with_next__from == arcs.end()) {
            topmost_in_kwn = r.from;
            keep_with_next__from = i;
        }
        const double old_shift = shift;
        shift = ceil(std::max(shift, pageBreak+1 - topmost_in_kwn));
        //if we shift a whole page, do not shift at all - this is an element bigger than a page
        if (shift >= netPrevPageSize) {
            shift = old_shift;
            keep_with_next__from = arcs.end();
            lowest_on_prev_page = pageBreak-1; //previous page is full.
            continue;
        }
        //Test if we shift too much
        //shift = ceil(std::min(shift, old_lowest_on_next_page - topmost_in_kwn));

        //Now shift elements between 'keep_with_next__from` till 'i' (inclusive)
        //beware of the ++ in the while()
        do {
            (*keep_with_next__from)->ShiftBy(shift);
            lowest_on_next_page = std::max(lowest_on_next_page,
                (*keep_with_next__from)->GetVisualYExtent(true).till);
        } while (keep_with_next__from++ != i && keep_with_next__from != arcs.end());

        keep_with_next__from  = arcs.end();
    }

    //Now test if we can move the page break up
    lowest_on_prev_page = ceil(lowest_on_prev_page);
    if (!canChangePBPos || lowest_on_prev_page >= pageBreak-1)
        return shift;
    for (auto &pArc : arcs)
        if (pArc->GetVisualYExtent(true).from>=pageBreak)
            pArc->ShiftBy(lowest_on_prev_page - (pageBreak-1)); //negative
    return shift + lowest_on_prev_page - (pageBreak-1);
}

 /** Collect the y position of page breaks in the list to pageBreakData. */
void Msc::CollectPageBreakArcList(ArcList &arcs)
{
    for (auto &pArc : arcs)
        pArc->CollectPageBreak();
}

/** Automatically paginate an already laid out chart.
 * @param canvas The canvas on which to calculate geometries.
 * @param [in] pageSize The height of a page in pixels.
 * @param [in] addHeading If true automatically inserted page breaks will also
 *                        have entity headings. */
void Msc::AutoPaginate(Canvas &canvas, double pageSize, bool addHeading)
{
    _ASSERT(floor(pageSize)==pageSize);
    //Here `total` is set, `drawing` is set the same
    //All elements (except notes, verticals and floating symbols) have been placed.
    //But page and entity status is not yet collected.
    pageBreakData.assign(1, PageBreakData(0.0, false));
    CollectPageBreakArcList(Arcs);
    pageBreakData.push_back(PageBreakData(total.y.till, false)); //not really a page start, but helps us
    for (unsigned u=0; u<pageBreakData.size()-1; u++) {
        if (pageBreakData[u+1].y - pageBreakData[u].y < pageSize-2)
            continue;
        //We need to insert a page break, find the effiective size of the page
        double netPrevPageSize = pageSize;
        if (pageBreakData[u].autoHeadingSize < pageSize)
            netPrevPageSize -= pageBreakData[u].autoHeadingSize;
        bool addCommandNewpage = true;
        //Here we re-use Entity::running_* members of AllEntities.
        //(These were used during parsing, not needed any longer.)
        //We re-create the running status so that if we need to insert a heading,
        //we know what to insert.
        if (addHeading)
            for (auto pEntity : AllEntities)
                pEntity->running_shown = EEntityStatus::SHOW_OFF;
        total.y.till += PageBreakArcList(canvas, Arcs, netPrevPageSize, pageBreakData[u].y + netPrevPageSize+1,
                                         addCommandNewpage, addHeading, true, false);
        //Regenerate page breaks
        pageBreakData.assign(1, PageBreakData(0.0, false));
        CollectPageBreakArcList(Arcs);
        pageBreakData.push_back(PageBreakData(total.y.till, false)); //not really a page start, but helps us
    }
    pageBreakData.pop_back();
    drawing.y = total.y; //keep this invariant before placing notes
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


/** Lay out a parsed chart and calculate its extents. 
 * This function calls the Width() and Layout() functions of 
 * arcs to set x and y coordinates, respectively.
 * It also performs automatic pagination, if needed.
 * As a result, we fill in Msc::total, Msc::drawing, Msc::comments_right_side,
 * and Msc::copyrightTextHeight. We ensure that Msc::total is all integer.
 * @param canvas The canvas to calculate geometry on
 * @param [in] autoPaginate If true, we do automatic pagination
 * @param [in] addHeading If true, the automatic page breaks will also add a heading
 * @param [in] pageSize Determines the page size for automatic pagination
 * @param [in] fitWidth If true and automatic pagination is on, we select a zoom value
 *                      to fit the width of the page and normalize the height of the page
 *                      with it.*/
void Msc::CalculateWidthHeight(Canvas &canvas, bool autoPaginate, 
                               bool addHeading, XY pageSize, bool fitWidth)
{
    pageBreakData.assign(1, PageBreakData(0.0, false));
    HideELinesHere.clear();
    if (Arcs.size()==0) return;
    if (total.y.Spans() > 0) return; //already done?

    //start with width calculation, that is used by many elements
    Progress.StartSection(MscProgress::WIDTH);
    EntityDistanceMap distances;
    DistanceMapVertical vdist;
    vdist.InsertMarker(MARKER_BUILTIN_CHART_TOP_STR);
    //Add distance for arcs,
    //needed for hscale=auto, but also for entity width calculation and side note size calculation
    WidthArcList(canvas, Arcs, distances, vdist);
    distances.CombineLeftRightToPair_Max(hscaleAutoXGap, activeEntitySize/2);
    distances.CombineBoxSideToPair(hscaleAutoXGap);

    //Turn on entity line for side note lines if there are side notes
    if (distances.had_l_comment) 
        LNote->status.SetStatus(0, EEntityStatus::SHOW_ON);
    if (distances.had_r_comment) 
        RNote->status.SetStatus(0, EEntityStatus::SHOW_ON);

    const double unit = XCoord(1);

    //Add a gap to side comment lanes (if any)
    const bool lhs  = distances.IncreaseIfNonZero(NoEntity->index, LNote->index, sideNoteGap*2, true);
    const bool ls = lhs || distances.IncreaseIfNonZero(NoEntity->index, LNote->index, sideNoteGap*2);
    const bool rhs = distances.IncreaseIfNonZero(RNote->index, EndEntity->index, sideNoteGap*2, true);
    const bool rs = rhs || distances.IncreaseIfNonZero(RNote->index, EndEntity->index, sideNoteGap*2);

    //The algorithm to calculate spacing for side comments is as follows.
    //If there are no comments, we use zero.
    //Else if we have hscale=auto, we use the maximum of hscale specified and 
    //     label requested spaces. If that is zero, we use XCoord(DEFAULT_COMMENT_SIZE)
    //Else if the user specified something (via hspace), we use that
    //Else we use XCoord(DEFAULT_COMMENT_SIZE)
    //So, here is how we implement it: if no space required (*), but we have comments, 
    //we insert to hspace_distances
    //(*) For hscale=auto, we check both normal and hspace distances, for fixed hscale, only 
    //hspace distances. 
    if (distances.had_l_comment && (GetHScale()>=0 ? !lhs : !ls)) 
        distances.Insert(NoEntity->index, LNote->index, XCoord(DEFAULT_COMMENT_SIZE), true);
    if (distances.had_r_comment && (GetHScale()>=0 ? !rhs : !rs))
        distances.Insert(RNote->index, EndEntity->index, XCoord(DEFAULT_COMMENT_SIZE), true);

    //Add some margins between note line and side line.
    //Add to hspace distance, since these will be needed even for fixed h scale
    distances.Insert(LNote->index, LSide->index, XCoord(MARGIN), true);
    distances.Insert(RSide->index, RNote->index, XCoord(MARGIN), true);
    if (GetHScale()>=0) {
        //Copy the actual pos's to distances.hscape_pairs (where we may have
        //additional requirements from hspace commands.
        for (auto ei = ++ActiveEntities.begin(); ei!=ActiveEntities.end(); ei++) {
            const EIterator prev_ei = --EIterator(ei);
            distances.Insert((*prev_ei)->index, (*ei)->index, 
                             unit*((*ei)->pos - (*prev_ei)->pos), true);
        }
        //Copy the collected distance between LSide and the leftmost real entity; 
        //and between the rightmost real entity and RSide to the hspace 
        //distances. We do autoscaling at the left and right side, such that
        //no element of the chart falls off.
        //(If there are no real entities the two below commands work on the
        //same indexes. They are still needed e.g., for charts with a
        //title only.)
        distances.Insert(LSide->index, LSide->index+1, 
                         distances.Query(LSide->index, LSide->index+1), true);
        distances.Insert(RSide->index-1, RSide->index, 
                         distances.Query(RSide->index-1, RSide->index), true);
        //Do the same for the space between LNote<->LSide and RSide<->RNote for 
        //verticals operating there.
        distances.Insert(LNote->index, LSide->index, 
                         distances.Query(LNote->index, LSide->index), true);
        distances.Insert(RSide->index, RNote->index, 
                         distances.Query(RSide->index, RNote->index), true);
    } else {
        //Merge hspace distances to normal distances
        for (auto i : distances.GetHSpacePairs()) 
            distances.Insert(i.first.first, i.first.second, i.second);
    }
    //Now if we have auto-scaling, the space requirements are in distances.pairs
    //if we have fixed scaling the requirements are in distances.hspace_pairs

    //Go through all the pairwise requirements and calc actual pos.
    //dist will hold required distance to the right of entity with index []
    vector<double> dist(ActiveEntities.size(), 0);
    //distances.pairs starts with requiremenst between neighbouring entities
    //and continues with requirements between second neighbours, ... etc.
    //we process these sequentially
    for (auto i : GetHScale()<0 ? distances.GetPairs() : distances.GetHSpacePairs()) {
        //Get the requirement
        double toadd = i.second;
        //Substract the distance already there
        for (unsigned f = i.first.first; f!=i.first.second; f++)
            toadd -= dist[f];
        //If there is need to increase start with the closer ones
        //and gradually move to larger ones.
        while (toadd>0)
            toadd = MscSpreadBetweenMins(dist, i.first.first,
                                            i.first.second, toadd);
    }
    //Now dist[i] contains the needed space on the right of entity index i
    //Kill comment space if we have no comments on that side (user could still have
    //specified comments)
    if (!distances.had_l_comment) dist[0] = 0;
    if (!distances.had_r_comment) dist[dist.size()-2] = 0;

    //Copy dist[i] values to the pos field of entities
    double curr_pos = 0; 
    unsigned index = 0;
    for (auto pEntity : ActiveEntities) {
        pEntity->pos = curr_pos;
        curr_pos += ceil(dist[index++])/unit;    //take integer space, so XCoord will return integer
    }

    total.x.from = 0;
    total.x.till = XCoord(EndEntity->pos)+1; //XCoord is always integer

    //Consider the copyright text
    StringFormat sf;
    sf.Default();
    XY crTexSize = Label(copyrightText, canvas, sf).getTextWidthHeight().RoundUp();
    if (total.x.till < crTexSize.x) total.x.till = crTexSize.x;
    copyrightTextHeight = crTexSize.y;

    drawing.x.from = XCoord(LNote->pos) + sideNoteGap;
    drawing.x.till = XCoord(RNote->pos) - sideNoteGap;

    Progress.StartSection(MscProgress::LAYOUT);
    noLabels = noOverflownLabels = 0;
    total.y.from = 0;
    total.y.till = LayoutArcList(canvas, Arcs, NULL) + chartTailGap;
    //total.y.till = ceil(std::max(total.y.till, cover.GetBoundingBox().y.till));
    drawing.y = total.y;  

    //record the top and bottom positions.
    //Note that notes may extend above and below, resp., but this is OK, 
    //we want these to refer to the top and bottom without considering notes.
    Markers[MARKER_BUILTIN_CHART_TOP_STR].y = total.y.from;
    Markers[MARKER_BUILTIN_CHART_BOTTOM_STR].y = total.y.till;

    //If at least 10% of the labels are overflown (but at least 3), emit warning
    if (noOverflownLabels>2 && noOverflownLabels > noLabels/10) 
        if (GetHScale()>0 && !Contexts.back().text.IsWordWrap()) {
            string ss;
            ss << noOverflownLabels;
            Error.Warning(FileLineCol(Error.Files.size()-1, 1, 1), 
                 "There are " + ss + " labels wider than the space available.",
                 "Consider using horizontal auto-scaling ('hscale=auto;') or "
                 "word wrapping ('text.wrap=yes;').");
        }
    if (autoPaginate) {
        if (fitWidth)
            pageSize.y *= total.x.Spans()/pageSize.x;
        pageSize.y = floor(pageSize.y);
        //This scaling may be screwed if notes expand the chart in the Y direction.
        //But what can we do..?
        Progress.StartSection(MscProgress::AUTOPAGINATE);
        AutoPaginate(canvas, pageSize.y - copyrightTextHeight, addHeading);
        Progress.DoneItem(MscProgress::AUTOPAGINATE, 1);
    } else
        CollectPageBreakArcList(Arcs);
}

/** Places elements that can be placed only when marker positions are known (verticals, some symbols).
 * Calls ArcBase::PlaceWithMarkers for all elements in the list.*/
void Msc::PlaceWithMarkersArcList(Canvas &canvas, ArcList &arcs)
{
    for (auto pArc : arcs) {
        pArc->PlaceWithMarkers(canvas);
        Progress.DoneItem(MscProgress::PLACEWITHMARKERS, pArc->myProgressCategory);
    }
}

/* Places notes */
void Msc::PlaceFloatingNotes(Canvas &canvas)
{
    Block new_total;
    new_total.MakeInvalid();
    for (auto pNote : Notes) {
        pNote->PlaceFloating(canvas);
        new_total += pNote->GetAreaToDraw().GetBoundingBox();
        Progress.DoneItem(MscProgress::NOTES, pNote->myProgressCategory);
    }
    if (new_total.IsInvalid()) return;
    new_total.Expand(sideNoteGap);
    total += new_total;
}

/** Invalidate all notes that point to this element. 
 * Used in the destructor of Element - we also hide notes pointing to it.*/
void Msc::InvalidateNotesToThisTarget(const Element *target)
{
    for(auto pNote : Notes)
        if (pNote->GetTarget() == target)
            pNote->Invalidate();
}

/** Remove a note from the list of notes.
 * Used from the destructor of CommandNote */
void Msc::RemoveFromNotes(const CommandNote *note)
{
    for(auto i=Notes.begin(); i!=Notes.end(); /*nope*/)
        if (*i==note) Notes.erase(i++);
        else i++;
}

/** Calls PostPosProcess for all members of the arc list */
void Msc::PostPosProcessArcList(Canvas &canvas, ArcList &arcs)
{
    for (auto pArc : arcs) {
        pArc->PostPosProcess(canvas);
		Progress.DoneItem(MscProgress::POST_POS, pArc->myProgressCategory);
	}
}

/** Calls RegisterCover for all members of the arc list */
void Msc::RegisterCoverArcList(ArcList &arcs, EDrawPassType pass)
{
    for (auto pArc : arcs) 
        pArc->RegisterCover(pass);
}


/** Prepares a chart for drawing after parse.
 * We perform post-parse processing, lay out the chart and perform
 * post-positioning processing. After this Draw() functions can be called.
 * If autoPaginate is true, pagesize.y is the page height.
 * If fitWidth is true, we adjust page height to get a scaling 
 * so that the chart fits the width.
 * @param [in] ot The type of the output (PDF, PNG, etc.)
 * @param [in] avoidEmpty If true, we generate a rounded box saying "empty chart" to 
 *                        avoid a completely empty chart.
 * @param [in] autoPaginate If true, we do automatic pagination
 * @param [in] addHeading If true, the automatic page breaks will also add a heading
 * @param [in] pageSize Determines the page size for automatic pagination
 * @param [in] fitWidth If true and automatic pagination is on, we select a zoom value
 *                      to fit the width of the page and normalize the height of the page
 *                      with it.
 * @param [in] collectLinkInfo If true, we also fill the ismapData member, collecting
 *                             where are links on the chart.*/
void Msc::CompleteParse(Canvas::EOutputType ot, bool avoidEmpty, 
                        bool autoPaginate, bool addHeading, XY pageSize, 
                        bool fitWidth, bool collectLinkInfo)
{
    if (autoPaginate)
        Progress.RegisterBulk(MscProgress::AUTOPAGINATE, 1);

    //Allocate (non-sized) output object and assign it to the chart
    //From this point on, the chart sees xy dimensions
    Canvas canvas(ot);

    //Sort Entities, add numbering, fill in auto-calculated values,
    //and throw warnings for badly constructed diagrams.
    headingSize = 0;
    PostParseProcess(canvas); 
	Progress.StartSection(MscProgress::FINALIZE_LABELS);
    FinalizeLabelsArcList(Arcs, canvas);

    //Calculate chart size
    CalculateWidthHeight(canvas, autoPaginate, addHeading, pageSize, fitWidth);

    //If the chart ended up empty we may want to display something
    if (total.y.till <= chartTailGap && avoidEmpty) {
        //Add the Empty command
        ArcBase *arc = new CommandEmpty(this);
        arc->AddAttributeList(NULL);
        Arcs.push_front(arc);
        Progress.DoneItem(MscProgress::LAYOUT, MscProgress::EMPTY);
        _ASSERT(Contexts.size() && Contexts.back().hscale.first);
        Contexts.back().hscale.second = -1;
        //Redo calculations
        total.x.till = total.y.till = 0;  //"from" members still zero, only notes can take it negative
        CalculateWidthHeight(canvas, false, false, XY(), false);  //no automatic pagination here
        //Luckily Width and Layout calls do not generate error messages,
        //So Errors collected so far are OK even after redoing this
    }

    Progress.StartSection(MscProgress::PLACEWITHMARKERS);
    PlaceWithMarkersArcList(canvas, Arcs);
    Progress.StartSection(MscProgress::NOTES);
    PlaceFloatingNotes(canvas);

    total.x.from = floor(total.x.from);
    total.y.from = floor(total.y.from);
    total.x.till = ceil(total.x.till);
    total.y.till = ceil(total.y.till);

    Progress.StartSection(MscProgress::POST_POS);
    //A final step of prcessing, checking for additional drawing warnings
    PostPosProcessArcList(canvas, Arcs);

    //Collect the covers in AllCovers
    if (prepare_for_tracking) {
        RegisterCoverArcList(Arcs, DRAW_BEFORE_ENTITY_LINES);
        RegisterCoverArcList(Arcs, DRAW_AFTER_ENTITY_LINES);
        RegisterCoverArcList(Arcs, DRAW_DEFAULT);
        RegisterCoverArcList(Arcs, DRAW_AFTER_DEFAULT);
        RegisterCoverArcList(Arcs, DRAW_NOTE);
        RegisterCoverArcList(Arcs, DRAW_AFTER_NOTE);
    }

    //Delete LSide and RSide actions if there are no side comments
    if (LNote->pos == LSide->pos) 
        LSide->status.Reset();
    if (RNote->pos == RSide->pos) 
        RSide->status.Reset();

    Error.Sort();

    if (collectLinkInfo)
        CollectIsMapElementsArcList(Arcs, canvas);
}

/** Draw arcs in an arc list 
 * @param canvas The canvas to draw on
 * @param [in] arcs The list of arcs to draw
 * @param [in] yDrawing Draw only arcs in this vertical range 
 *                      (speedup for drawing only a single page)
 * @param [in] pass Draw only elements which need to be drawn in this drawing pass*/
void Msc::DrawArcList(Canvas &canvas, ArcList &arcs, Range yDrawing, EDrawPassType pass)
{
    for (auto pArc : arcs)
        if (pArc->GetVisualYExtent(true).Overlaps(yDrawing)) {
            pArc->Draw(canvas, pass);
            if (pArc->draw_pass == pass)
                Progress.DoneItem(MscProgress::DRAW, pArc->myProgressCategory);
        }
}

/** Draw the chart 
 * This function draws entity lines and arcs (all passes)
 * @param canvas The canvas to draw on
 * @param [in] yDrawing Draw only arcs in this vertical range 
 *                      (speedup for drawing only a single page)
 * @param [in] pageBreaks If true, we draw dashed lines for page breaks and page numbers. */
void Msc::DrawChart(Canvas &canvas, Range yDrawing, bool pageBreaks)
{
    if (total.y.Spans() <= 0) return;
	//draw background
    if (Background.size()) {
        FillAttr fill_bkg(ColorType(255,255,255), GRADIENT_NONE);
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
            if (till<total.y.till && note->status.GetStyle(pos).read().fill == note->status.GetStyle(till).read().fill)
                continue;  //go until "fill" changes
            if (till>total.y.till) till = total.y.till;
            canvas.Fill(Block(x, Range(pos, till)), note->status.GetStyle(pos).read().fill);
            pos = till;
        } while (pos<total.y.till);
    }
	//Draw page breaks
    if (pageBreaks)
        DrawPageBreaks(canvas);
    DrawArcList(canvas, Arcs, yDrawing, DRAW_BEFORE_ENTITY_LINES);
	//Draw initial set of entity lines (boxes will cover these and redraw)
    DrawEntityLines(canvas, yDrawing.from, yDrawing.till);
    DrawArcList(canvas, Arcs, yDrawing, DRAW_AFTER_ENTITY_LINES);
    DrawArcList(canvas, Arcs, yDrawing, DRAW_DEFAULT);
    DrawArcList(canvas, Arcs, yDrawing, DRAW_AFTER_DEFAULT);
    DrawArcList(canvas, Arcs, yDrawing, DRAW_NOTE);
    DrawArcList(canvas, Arcs, yDrawing, DRAW_AFTER_NOTE);
}

/** Draw the page breaks */
void Msc::DrawPageBreaks(Canvas &canvas)
{
    if (pageBreakData.size()<=1) return;
    if (total.y.Spans()<=0) return;
    LineAttr line;
    StringFormat format;
    format.Default();
    format.Apply("\\pr\\-");
    Label label;
    for (unsigned page=1; page<pageBreakData.size(); page++) {
        char text[20];
        const double y = pageBreakData[page].y;
        line.type.second = pageBreakData[page].manual ? LINE_DASHED : LINE_DOTTED;
        canvas.Line(XY(total.x.from, y), XY(total.x.till, y), line);
        sprintf(text, "page %u", page);
        label.Set(text, canvas, format);
        label.Draw(canvas, total.x.from, total.x.till, y-label.getTextWidthHeight().y);
    }
}

/** Draw the header and footer for a page. 
 * This means a potential automatic heading and the copyright text 
 * @param canvas The canvas to draw on
 * @param [in] page The page to draw for. Zero means the whole chart. */
void Msc::DrawHeaderFooter(Canvas &canvas, unsigned page) 
{
    canvas.PrepareForHeaderFoorter();

    //Draw Copyright Text
    StringFormat sf;
    sf.Default();
    Label label(copyrightText, canvas, sf);
    label.Draw(canvas, total.x.from, total.x.till, page==0 || page>=pageBreakData.size() ? total.y.till : pageBreakData[page].y);

    //Draw autoheading, if any
    if (page && pageBreakData[page-1].autoHeading)
        //autoHeading is supposedly shifted to just above the page break
        pageBreakData[page-1].autoHeading->Draw(canvas, pageBreakData[page-1].autoHeading->draw_pass);
}

/** Draws one complete chart page or all pages.
 * We draw header, footer and all chart content.
 * Expects the context to be prepared and will unprpare it.
 * @param canvas The canvas to draw on
 * @param [in] pageBreaks Governs if we draw page break indicators for multi-page charts.
 * @param [in] page The page to draw. Zero means the whole chart. */
void Msc::DrawComplete(Canvas &canvas, bool pageBreaks, unsigned page)
{
    Progress.StartSection(MscProgress::DRAW);
    if (page>pageBreakData.size() || total.x.Spans()<=0) return;
    Range yDrawing;
    if (page) {
        yDrawing.from = pageBreakData[page-1].y;
        yDrawing.till = page < pageBreakData.size() ? pageBreakData[page].y : total.y.till;
    } else
        yDrawing = total.y;
    DrawChart(canvas, yDrawing, page ? false : pageBreaks);

    DrawHeaderFooter(canvas, page);
}

/** Draws the chart into one of more files.
 * If the chart contains only one page, we create a single file. If there 
 * are multiple pages, we create one for each. If case of PDF output format
 * if pageSize is nonzero, we create a single file with multiple pages.
 * 'pageSize' can only be valid (positive `x` and `y`) if file format is PDF (which supports 
 * multiple pages). In that
 * case `scale` can be <zero,zero> indicating that the chart should be fitted to page width.
 * This is the only drawing function that can place an error into 'Error' if generateErrors is set.
 * Scale contains a list of scales to try.
 * @param [in] ot The format of output. Determines what type of an output file to create.
 * @param [in] scale The scaling value requested by the user. Must be all-positive, if the
 *                   margins are valid.
 * @param [in] fn The name of the file to create. In case of multiple files, we append a number.
 * @param [in] bPageBreak If true, we draw dashed lines for page break, when the 
 *                        whole chart is drawn in one.
 * @param [in] ignore_pagebreaks If true, we draw a multi-page chart onto a single file 
 *                               (if pagesize is zero) or a single page (if pagesize is nonzero)
 *                               ignoring page breaks.
 * @param [in] pageSize If non-zero, a fixed page size is used and a single multi-page 
 *                      file will be created (only with PDF).
 * @param [in] margins For fixed size pages, these are the margins
 * @param [in] ha The horizontal alignment for fix size pages (-1: left, 0: center, +1:right)
 * @param [in] va The vertical alignment for fix size pages (-1:up, 0: center, +1:bottom)
 * @param [in] generateErrors If true and we cannot avoid overfill, we geenrate a warining.
 *                            Errors are also generated on file creation and similar hard errors.
 * @returns False on error (but not on warning) irrespective of `generateErrors`. */
bool Msc::DrawToFile(Canvas::EOutputType ot, const XY &scale, 
                     const string &fn, bool bPageBreak, bool ignore_pagebreaks, 
                     const XY &pageSize, const double margins[4], 
                     int ha, int va, bool generateErrors)
{
    const unsigned from = pageBreakData.size()<=1 || ignore_pagebreaks ? 0 : 1;
    const unsigned till = pageBreakData.size()<=1 || ignore_pagebreaks ? 0 : pageBreakData.size();
    if (pageSize.x<=0 || pageSize.y<0) 
        for (unsigned page=from; page<=till; page++) {
            if (ot==Canvas::ISMAP) {
                string fileName = fn;
                if (page>0) {  //append page number if not all pages written
                    char num[3] = "00";
                    num[0] = char('0'+page/10);
                    num[1] = char('0'+page%10);
                    fileName.insert(fn.find_last_of('.'), num);
                }
                FILE *fout = fopen(fn.c_str(), "wt");
                if (fout==NULL) {
                    Error.FatalError(FileLineCol(0, 0), "Could not open output file '" + fn + "'."); 
                    return false;
                }
                Canvas canvas(ot);
                CollectIsMapElementsArcList(Arcs, canvas);
                for (auto &e : ismapData) {
                    if (page>0 && pageBreakData[page-1].y <= e.rect.x.from &&
                        pageBreakData[page].y > e.rect.x.from) 
                        e.rect.Shift(XY(0, -pageBreakData[page-1].y));
                    const string s = e.Print();
                    fprintf(fout, "%s\n", s.c_str());
                }
                fclose(fout);
            } else {
                Canvas canvas(ot, total, copyrightTextHeight, fn, scale, &pageBreakData, page);
                if (canvas.ErrorAfterCreation(generateErrors ? &Error : NULL, &pageBreakData, true)) return false;
                DrawComplete(canvas, bPageBreak, page);
            }
        }
    else {
        _ASSERT(ot!=Canvas::ISMAP);
        Canvas canvas(ot, total, fn, scale, pageSize, margins, ha, va, copyrightTextHeight, &pageBreakData);
        if (canvas.ErrorAfterCreation(generateErrors ? &Error : NULL, &pageBreakData, true)) return false;
        for (unsigned page = from; page<=till; page++) {
            DrawComplete(canvas, bPageBreak, page);
            if (page<till)
                if (!canvas.TurnPage(&pageBreakData, page+1, generateErrors ? &Error : NULL))
                    return false;
        }
    }
    return true;
}

#ifdef CAIRO_HAS_WIN32_SURFACE
/** Draw the chart or one page of it to a metafile
 * @param [in] ot The format of output. Determines what type of metafile we use.
 *                can only be WMF, EMF or EMFWMF.
 * @param [in] page The page to draw, zero for the whole chart.
 * @param [in] bPageBreaks If true and we draw the whole chart and the chart has multiple 
 *                         pages, draw dashed lines for page breaks.
 * @param [in] fallback_image_resolution Draw fallback images at this resolution.
 * @param [out] metafile_size Return the size of the resultant metafile in bytes
 * @param [out] fallback_images Return the area covered by fallback images (in chart space)
 * @param [in] generateErrors If true and we cannot avoid overfill, we geenrate a warining.
 *                            Errors are also generated on file creation and similar hard errors.
 * @returns The EMF handle of the metafile created. */
HENHMETAFILE Msc::DrawToMetaFile(Canvas::EOutputType ot,  
                                 unsigned page, bool bPageBreaks, 
                                 double fallback_image_resolution,
                                 size_t *metafile_size,
                                 Contour *fallback_images,
                                 bool generateErrors)
{
    _ASSERT(ot==Canvas::WMF || ot==Canvas::EMF || ot==Canvas::EMFWMF);
    if (ot!=Canvas::WMF && ot!=Canvas::EMF && ot!=Canvas::EMFWMF)
        return 0;
    if (page>0) bPageBreaks = false;
    Canvas canvas(ot, HDC(NULL), total, copyrightTextHeight, XY(1,1), &pageBreakData, page);
    if (canvas.ErrorAfterCreation(generateErrors ? &Error : NULL, &pageBreakData, true)) return 0;
	if (fallback_image_resolution>0)
        canvas.SetFallbackImageResolution(fallback_image_resolution);
    DrawComplete(canvas, bPageBreaks, page);
    HENHMETAFILE ret = canvas.CloseAndGetEMF();
    if (fallback_images)
        *fallback_images = std::move(canvas.GetFallbackImagePlaces());
    if (metafile_size)
        *metafile_size = canvas.GetMetaFileSize();
    return ret;
}

/** Draw the chart or one page of it to a metafile or printer Device Context
 * @param [in] ot The format of output. Determines what type of metafile we use.
 *                can only be WMF, EMF, EMFWMF or PRINTER.
 * @param hdc The DC to draw onto
 * @param [in] scale Scale the chart by this amount
 * @param [in] page The page to draw, zero for the whole chart.
 * @param [in] bPageBreaks If true and we draw the whole chart and the chart has multiple 
 *                         pages, draw dashed lines for page breaks.
 * @param [in] fallback_image_resolution Draw fallback images at this resolution.
 * @param [in] generateErrors If true and we cannot avoid overfill, we geenrate a warining.
 *                            Errors are also generated on file creation and similar hard errors.
 * @returns The size of the metafile or zero at error.*/
size_t Msc::DrawToDC(Canvas::EOutputType ot, HDC hdc, const XY &scale,
                   unsigned page, bool bPageBreaks,
                   double fallback_image_resolution, bool generateErrors)
{
    _ASSERT(ot == Canvas::WMF || ot == Canvas::EMF ||
            ot == Canvas::EMFWMF || ot == Canvas::PRINTER);
    if (page>0) bPageBreaks = false;
    Canvas canvas(ot, hdc, total, copyrightTextHeight, scale, &pageBreakData, page);
    if (canvas.ErrorAfterCreation(generateErrors ? &Error : NULL, &pageBreakData, true)) return 0;
	if (fallback_image_resolution>0)
        canvas.SetFallbackImageResolution(fallback_image_resolution);
    DrawComplete(canvas, bPageBreaks, page);
    canvas.CloseOutput();
    return canvas.GetMetaFileSize();
}

#endif

/** Draw the whole chart (unscaled) to a cairo recording surface.
 * @param [in] ot The format of output. Determines what type of approximations we use.
 * @param [in] bPageBreaks If true and the chart has multiple 
 *                         pages, draw dashed lines for page breaks.
 * @param [in] generateErrors If true and we cannot avoid overfill, we geenrate a warining.
 *                            Errors are also generated on file creation and similar hard errors.
 * @returns The resultant cairo recording surface.
 */
cairo_surface_t *Msc::DrawToRecordingSurface(Canvas::EOutputType ot, bool bPageBreaks,
                                             bool generateErrors)
{
    cairo_surface_t *ret = cairo_recording_surface_create(CAIRO_CONTENT_COLOR_ALPHA, NULL);
    cairo_status_t status = cairo_surface_status(ret);
    if (status != CAIRO_STATUS_SUCCESS) {
        cairo_surface_destroy(ret);
        return NULL;
    }
    Canvas canvas(ot, ret, total, copyrightTextHeight, XY(1., 1.), &pageBreakData);
    if (canvas.ErrorAfterCreation(generateErrors ? &Error : NULL, &pageBreakData, true)) {
        cairo_surface_destroy(ret);
        return NULL;
    }
    DrawComplete(canvas, bPageBreaks, 0);
    return ret;
}

/** Replay the chart or one page of it onto a cairo recording surface using a version previously 
 * recorded via DrawToRecordingSurface().
 * This function is used when the user views just one page.
 * @param [in] full A recording suface onto which we previously recorded the drawing 
 *                  of the chart using DrawToRecordingSurface().
 * @param [in] page The page to draw, zero for the whole chart.
 * @param [in] generateErrors If true and we cannot avoid overfill, we geenrate a warining.
 *                            Errors are also generated on file creation and similar hard errors.
 * @returns The resultant cairo recording surface.
 */
cairo_surface_t *Msc::ReDrawOnePage(cairo_surface_t *full, unsigned page, 
                                    bool generateErrors)
{
    cairo_surface_t *ret = cairo_recording_surface_create(CAIRO_CONTENT_COLOR_ALPHA, NULL);
    cairo_status_t status = cairo_surface_status(ret);
    if (status != CAIRO_STATUS_SUCCESS) {
        cairo_surface_destroy(ret);
        return NULL;
    }
    Canvas canvas(Canvas::SVG, ret, total, copyrightTextHeight, XY(1., 1.), &pageBreakData, page);
    if (canvas.ErrorAfterCreation(generateErrors ? &Error : NULL, &pageBreakData, true)) {
        cairo_surface_destroy(ret);
        return NULL;
    }
    cairo_set_source_surface(canvas.GetContext(), full, 0, 0);
    cairo_paint(canvas.GetContext());

    DrawHeaderFooter(canvas, page);
    return ret;
}
