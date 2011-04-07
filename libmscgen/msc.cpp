/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010,2011 Zoltan Turanyi
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
#include <assert.h>
#include <math.h>
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
    if (e1==e2) return;
    if (e2==DISTANCE_LEFT) {
        std::map<unsigned, double>::iterator i = left.find(e1);
        if (i!=left.end() && i->second >= xdist) return;
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
    if (e1==e2) return 0;
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
void EntityDistanceMap::InsertBoxSide(unsigned e, double r, double l)
{
    box_side[e].push_back(std::pair<double, double>(r, l));
}

//query the maximum boxside distance between e and e+1. left is true if we are interested
//in the max distance left of e+1 (and false if right from e)
std::pair<double, double> EntityDistanceMap::QueryBoxSide(unsigned e, bool left) const
{
    const auto l = box_side.find(e);
    if (l == box_side.end() || l->second.size()==0) return std::pair<double, double>(0,0);
    auto hit = l->second.begin();
    for (auto i = l->second.begin(); i!=l->second.end(); i++)
        if ((left && i->second > hit->second) || (!left && i->first > hit->first))
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
void EntityDistanceMap::CombineLeftRightToPair_Max(double gap)
{
    std::map<unsigned, double>::iterator i, j;
    for(i=right.begin(); i!=right.end(); ) {
        unsigned index = i->first;
        j = left.find(index+1);
        if (j == left.end()) {
            i++;
            continue;
        }
        double requirement = max(i->second,j->second) + gap;
        right.erase(i++);
        left.erase(j);
        Insert(index, index+1, requirement);
    }
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
    AllEntities(true), ActiveEntities(false),
    AutoGenEntities(false),
    Arcs(true)
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
    nudgeSize = 4;
    compressGap = 2;
    hscaleAutoXGap = 5;
    trackFrameWidth = 4;
    trackExpandBy = 2;

    pedantic=false;
    ignore_designs = false;

    //Add topmost style and color sets (global context), all empty now
    Contexts.push_back(Context());

    //Add "plain" style - the default constructor of Design sets that
    Designs["plain"];
    //Apply "plain" design
    SetDesign("plain", true);

    std::set<string> children_names;
    //Add virtual entities
    Entity *entity = new Entity(NONE_ENT_STR, NONE_ENT_STR, NONE_ENT_STR, -1001, 
                                Contexts.back().styles["entity"], file_line(), false);
    AllEntities.Append(entity);
    NoEntity = *AllEntities.begin();
    entity = new Entity(LSIDE_ENT_STR, LSIDE_ENT_STR, LSIDE_ENT_STR, -1000, 
                        Contexts.back().styles["entity"], file_line(), false);
    AllEntities.Append(entity);
    entity = new Entity(RSIDE_ENT_STR, RSIDE_ENT_STR, RSIDE_ENT_STR, 10000, 
                        Contexts.back().styles["entity"], file_line(), false);
    AllEntities.Append(entity);
}

bool Msc::SetDesign(const string&name, bool force)
{
    std::map<string,Design>::const_iterator i = Designs.find(name);
    if (i==Designs.end())
        return false;
    if (ignore_designs &&!force)
        return true;
    Contexts.back().numbering = i->second.numbering;
    Contexts.back().compress  = i->second.compress;
    hscale = i->second.hscale;
    for (ColorSet::const_iterator j = i->second.colors.begin(); j!=i->second.colors.end(); j++)
        Contexts.back().colors[j->first] = j->second;
    for (StyleSet::const_iterator j = i->second.styles.begin(); j!=i->second.styles.end(); j++)
        Contexts.back().styles[j->first] = j->second;
	Contexts.back().numberingStyle = i->second.numberingStyle;
    return true;
}

string Msc::GetDesigns() const
{
    std::map<string,Design>::const_iterator i = Designs.begin();
    string retval = i->first;
    for (i++; i!=Designs.end(); i++)
        retval.append(" ").append(i->first);
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
EIterator Msc::FindAllocEntity(const char *e, file_line_range l, bool*validptr)
{
    if (e==NULL) {
        _ASSERT (AllEntities.Find_by_Ptr(NoEntity) != AllEntities.end());
        return AllEntities.Find_by_Ptr(NoEntity);
    }
    EIterator ei = AllEntities.Find_by_Name(e);
    if (*ei == NoEntity) {
        if (pedantic)
            Error.Error(l.start, "Unknown entity '" + string(e)
                        + "'. Assuming implicit definition.",
                        "This may be a mistyped entity name."
                        " Try turning 'pedantic' off to remove these messages.");
        Entity *entity = new Entity(e, e, e, GetEntityMaxPos()+1, Contexts.back().styles["entity"], 
                                    l.start, false);
        AllEntities.Append(entity);
        EntityDef *ed = new EntityDef(e, this);
        ed->SetLineEnd(l);
        ed->AddAttributeList(NULL, NULL, file_line());
        ed->show.first = ed->show.second = true; //start turned on
        AutoGenEntities.Append(ed);
        ei = AllEntities.Find_by_Name(e);
    }
    return ei;
}

//Searches in AllEntities for the highest parent of i which is collapsed and return it
//If no parent is collapsed, it returns i.
//In essence it tells, which entity is shown on the chart for i
//If i is a grouped entity which is not collapsed, it will not show, but we return it.
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

//ei is points to AllEntities. If grouped we return its leftmost or rightmost children
EIterator Msc::FindLeftRightmostChildren(EIterator ei, bool left)
{
    if ((*ei)->children_names.size()==0) return ei;
    EIterator ret = AllEntities.Find_by_Name(*(*ei)->children_names.begin());
    for (auto i = ++(*ei)->children_names.begin(); i!=(*ei)->children_names.end(); i++)
        ret = EntityMinMaxByPos(ret, AllEntities.Find_by_Name(*i), left);
    return ret;
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
        if ((*i)->name != NONE_ENT_STR && (*i)->name != LSIDE_ENT_STR && 
            (*i)->name != RSIDE_ENT_STR && ret < (*i)->pos)
            ret = (*i)->pos;
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

bool Msc::AddAttribute(const Attribute &a)
{
    //Chart options cannot be styles
    assert (a.type != MSC_ATTR_STYLE);

    if (a.Is("msc")) {
        if (!a.CheckType(MSC_ATTR_STRING, Error)) return true;
        if (!SetDesign(a.value, false))
            Error.Warning(a.linenum_value.start, "Unknown chart design: '" + a.value +
                          "'. Ignoring design selection.",
                          "Available styles are: " + GetDesigns() +".");
        return true;
    }
    if (a.Is("hscale")) {
        if (a.type == MSC_ATTR_NUMBER && a.number>=0.01 && a.number <= 100) {
            hscale = a.number;
            return true;
        } else if (a.type == MSC_ATTR_STRING && a.value == "auto") {
            hscale = -1;
            return true;
        }
        a.InvalidValueError("0.01..100' or 'auto", Error);
        return true;
    }
    if (a.Is("compress")) {
        if (!a.CheckType(MSC_ATTR_BOOL, Error)) return true;
        Contexts.back().compress = a.yes;
        return true;
    }
    if (a.StartsWith("text"))
        return Contexts.back().text.AddAttribute(a, this, STYLE_OPTION);
    if (a.Is("numbering")) {
        if (!a.CheckType(MSC_ATTR_BOOL, Error)) return true;
        Contexts.back().numbering = a.yes;
        return true;
    }
    if (a.Is("numbering.pre")) {
        Contexts.back().numberingStyle.pre = a.value;
        StringFormat::ExpandColorAndStyle(Contexts.back().numberingStyle.pre, this,
                                          a.linenum_value.start, NULL,
                                          true, StringFormat::LABEL);
        return true;
    }
    if (a.Is("numbering.post")) {
        Contexts.back().numberingStyle.post = a.value;
        StringFormat::ExpandColorAndStyle(Contexts.back().numberingStyle.post, this,
                                          a.linenum_value.start, NULL,
                                          true, StringFormat::LABEL);
        return true;
    }
    if (a.Is("numbering.append")) {
        std::vector<NumberingStyleFragment> nsfs;
        if (NumberingStyleFragment::Parse(this, a.linenum_value.start, a.value.c_str(), nsfs))
            Contexts.back().numberingStyle.Push(nsfs);
        return true;
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
        return true;
    }

    if (a.Is("pedantic")) {
        if (!a.CheckType(MSC_ATTR_BOOL, Error)) return true;
        pedantic = a.yes;
        return true;
    }

    string ss;
    Error.Error(a, false, "Option '" + a.name + "' not recognized. Ignoring it.");
    return false;
}

//Add an attribute only if it can be part of a design. Others trigger error.
//This is called when a design definition is in progress.
bool Msc::AddDesignAttribute(const Attribute &a)
{
    if (a.StartsWith("numbering") || a.Is("compress") || a.Is("hscale") || a.Is("msc") ||
        a.StartsWith("text"))
        return AddAttribute(a);
    Error.Warning(a, false, "Cannot set attribute '" + a.name +
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
}

bool Msc::AttributeValues(const std::string attr, Csh &csh)
{
    if (CaseInsensitiveEqual(attr,"msc")) {
        csh.AddDesignsToHints();
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
        SetDesign("plain", true);
    } else
        Contexts.push_back(Contexts.back());
}

ArcBase *Msc::PopContext()
{
    if (Contexts.size()<2) return NULL;
    int old_size = Contexts.back().numberingStyle.Size();
    Contexts.pop_back();
    //if numbering continues with the same amount of levels, no action will be needed
    //in PostParseProcess, so we do not generate any Command arcs.
    if (old_size == Contexts.back().numberingStyle.Size())
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
    MscParse(*this, input, strlen(input));
}

string Msc::Print(int ident) const
{
    string s = AllEntities.Print(ident).append("\n");
    s.append(Arcs.Print(ident)).append("\n");
    return s;
}

void Msc::PostParseProcessArcList(ArcList &arcs, bool resetiterators,
                                  EIterator &left, EIterator &right,
                                  Numbering &number, bool top_level)
{
    for (ArcList::iterator i = arcs.begin(); i != arcs.end(); /*none*/) {
        if (resetiterators) {
            right = left = AllEntities.Find_by_Ptr(NoEntity);
            _ASSERT (left != AllEntities.end());
        }
        //Combine subsequent CommandEntities
        CommandEntity *ce = dynamic_cast<CommandEntity *>(*i);
        while (ce) {
            ArcList::iterator j = i;
            j++;
            if (j==arcs.end()) break;
            CommandEntity *ce2 = dynamic_cast<CommandEntity *>(*j);
            if (!ce2) break;
            ce->Combine(ce2);
            delete ce2;
            arcs.erase(j);
            continue; //i remains at this very same CommandEntity!
        }
        ArcBase *replace = (*i)->PostParseProcess(left, right, number, top_level);
        if (replace == *i) i++;
        else if (replace != NULL) (*i++) = replace;
        else arcs.erase(i++);
    }
}

void Msc::PostParseProcess()
{
    //Sort the defined entities as will be displayed from left to right
    AllEntities.SortByPos();

    //Now create a list of active Entities
    ActiveEntities.clear();
    for (auto i = AllEntities.begin(); i!=AllEntities.end(); i++) {
        const EIterator j = FindActiveParentEntity(i);
        if (i==j && (*i)->children_names.size() && !(*i)->collapsed) continue;
        if (ActiveEntities.size()==0) 
            ActiveEntities.Append(*j);  //first active entity
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

    EIterator lside = ActiveEntities.Find_by_Name(LSIDE_ENT_STR);
    EIterator rside = ActiveEntities.Find_by_Name(RSIDE_ENT_STR);

    //Find the first real entity
    EIterator tmp = ActiveEntities.begin();
    if (*tmp == NoEntity) tmp++;
    if (tmp == lside) tmp++;
    if (tmp == rside) tmp++;

    //Ensure that leftmost real entity pos == 2*MARGIN
    double rightmost = 0;
    if (tmp != ActiveEntities.end()) {
        double leftmost = (*tmp)->pos - 2*MARGIN;
        for  (EIterator i = AllEntities.begin(); i != AllEntities.end(); i++) 
            (*i)->pos -= leftmost;
        //Find rightmost entity's pos
        for  (EIterator i = ActiveEntities.begin(); i != ActiveEntities.end(); i++) {
            if (i==lside || i==rside || *i==NoEntity) continue;
            if (rightmost < (*i)->pos)
                rightmost = (*i)->pos;
        }
    } else {
        //if there are no real entities, just noentity, lside & rside, set this wide
        //so that copyright banner fits
        rightmost = 3*MARGIN;
    }
    //Set the position of the virtual side entities & resort
    const_cast<double&>((*lside)->pos) = MARGIN;
    const_cast<double&>((*rside)->pos) = rightmost + MARGIN;
    ActiveEntities.SortByPos();

    //Add the Auto generated entities to the list of entities
    //Add them to the the first arc if it is an EntityCommand,
    //Otherwise, generate a new entity command as first arc
    ArcList::iterator i = Arcs.begin();
    if (i == Arcs.end()) return; //we cannot have autogen entities either.
    if ((*i)->type != MSC_COMMAND_ENTITY) {
        CommandEntity *ce = new CommandEntity(new EntityDefList, this);
        ce->AddAttributeList(NULL);
        i = Arcs.insert(i, ce);
    }
    dynamic_cast<CommandEntity*>(*i)->AppendToEntities(AutoGenEntities);

    //Set all entity's shown to false, to avoid accidentally showing them via (heading;) before definition
    for (auto i = ActiveEntities.begin(); i!=ActiveEntities.end(); i++)
        (*i)->shown = false;

    //Traverse Arc tree and perform post-parse processing
    Numbering number; //starts at a single level from 1
    EIterator dummy1, dummy2;
    dummy2 = dummy1 = AllEntities.Find_by_Ptr(NoEntity);
    _ASSERT(dummy1 != AllEntities.end());
    PostParseProcessArcList(Arcs, true, dummy1, dummy2, number, true);
}

void Msc::DrawEntityLines(double y, double height,
                          EIterator from, EIterator to)
{
    //No checking of iterators!! Call with caution
    //"to" is not included!!
    ClipInverse(HideELinesArea);
    while(from != to) {
        XY up(XCoord(from), y);
        XY down(up.x, y);
        const double till = y+height;
        while (up.y < till) {
            down.y = min((*from)->status.Till(up.y), till);
            if ((*from)->status.GetStatus(up.y)) {
                const MscLineAttr &vline = (*from)->status.GetStyle(up.y).vline;
                const XY offset(fmod(vline.width.second/2,1),0);
                const XY magic(0,0);  //HACK needed in windows: 0,1
                const XY start = up+offset-magic;
                Line(start, down+offset, vline, start.y); //last param is dash_offset
            }
            up.y = down.y;
        }
        from++;
    }
    UnClip();
}

void Msc::WidthArcList(ArcList &arcs, EntityDistanceMap &distances)
{
    for (ArcList::iterator i = arcs.begin();i!=arcs.end(); i++)
        (*i)->Width(distances);
}

//Draws a full list starting at position==0
//We always place each element on an integer coordinates
double Msc::HeightArcList(ArcList::iterator from, ArcList::iterator to, AreaList &cover)
{
    cover.clear();
    double y = 0;
    //Zero-height arcs shall be positioned to the same place
    //as the first non-zero height arc below them (so that
    //if that arc is compressed up, they are not _below_
    //that following arc. So we store what was the last non-zero
    //height arc so we can go back and adjust the zero height ones
    ArcList::iterator first_zero_height = to;

    for (ArcList::iterator i = from; i!=to; i++) {
        AreaList arc_cover;
        double h = (*i)->Height(arc_cover);
        _ASSERT(h>=arc_cover.GetBoundingBox().y.till);
        arc_cover = arc_cover.CreateExpand(compressGap/2);
        double touchpoint = y;
        if ((*i)->IsCompressed()) {
            //if arc is of zero height, just collect it.
            //Its position may depend on the next arc if that is compressed.
            if (h==0) {
                if (first_zero_height == to) first_zero_height = i;
                continue;
            }
            const double new_y = std::max(0.0, -cover.OffsetBelow(arc_cover, touchpoint));
            //Here new_y can be larger than y, if the copressGap requirement pushed the current arc (in "i")
            //further below than the original height of the arcs above would have dictated.
            //Since we do compression, we pick the smallest of the two values.
            y = std::min(y, new_y);
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
        Range save_mainline = cover.mainline;
        cover += arc_cover;
        y += h;

        //If we are parallel draw the rest of the block in one go
        if ((*i)->IsParallel()) {
            //restore mainline to the one before we added the arc marked as parallel
            cover.mainline = save_mainline;
            //Place blocks, always compress the first
            return PlaceListUnder(++i, to, y, y-h, cover, true);
        }
    }
    return y;
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
double Msc::PlaceListUnder(ArcList::iterator from, ArcList::iterator to, double start_y,
                           double top_y, const AreaList &area_top, bool forceCompress,
                           AreaList *ret_cover)
{
    if (from==to) return 0;
    AreaList cover;
    double h = HeightArcList(from, to, cover);
    double touchpoint;
    double new_start_y = std::max(top_y, -area_top.OffsetBelow(cover, touchpoint));
    //if we shifted up, apply shift only if compess is on
    if (forceCompress || (*from)->IsCompressed())
        if (new_start_y < start_y) start_y = new_start_y;
    //if we shifted down, apply it in any case
    else if (new_start_y > start_y) start_y = new_start_y;
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
void Msc::CalculateWidthHeight(void)
{
    yPageStart.clear();
    yPageStart.push_back(0);
    HideELinesArea.clear();
    if (Arcs.size()==0) return;
    if (total.y == 0) {
        //start with width calculation, that is used by many elements
        EntityDistanceMap distances;
        //Add distance for arcs,
        //needed for hscale=auto, but also for entity width calculation
        WidthArcList(Arcs, distances);
        if (hscale<0) {
            distances.CombineLeftRightToPair_Max(hscaleAutoXGap);
            distances.CombineLeftRightToPair_Single(hscaleAutoXGap);
            distances.CopyBoxSideToPair(hscaleAutoXGap);

            //Now go through all the pairwise requirements and calc actual pos.
            //dist will hold required distance to the right of entity with index []
            vector<double> dist(ActiveEntities.size(), 0);
            dist[0] = 0;
            dist[1] = XCoord(MARGIN_HSCALE_AUTO);
            dist[dist.size()-2] = XCoord(MARGIN_HSCALE_AUTO);
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
            double unit = XCoord(1);
            double curr_pos = MARGIN_HSCALE_AUTO;
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
            total.x = XCoord((*--(ActiveEntities.end()))->pos+MARGIN_HSCALE_AUTO)+1;
        } else {
            total.x = XCoord((*--(ActiveEntities.end()))->pos+MARGIN)+1; //XCoord is always integer
        }
        StringFormat sf;
        sf.Default();
        XY crTexSize = Label(copyrightText, this, sf).getTextWidthHeight().RoundUp();
        if (total.x<crTexSize.x) total.x = crTexSize.x;

        copyrightTextHeight = crTexSize.y;
        AreaList cover;
        total.y = HeightArcList(Arcs.begin(), Arcs.end(), cover) + chartTailGap;
        total.y = ceil(std::max(total.y, cover.GetBoundingBox().y.till));
    }
}

void Msc::PostPosProcessArcList(ArcList &arcs, double autoMarker)
{
    for (auto j = arcs.begin(); j != arcs.end(); j++)
        (*j)->PostPosProcess(autoMarker);
}

void Msc::CompleteParse(OutputType ot, bool avoidEmpty)
{

    //Allocate (non-sized) output object and assign it to the chart
    //From this point on, the chart sees xy dimensions
    SetOutput(ot);

    //Sort Entities, add numbering, fill in auto-calculated values,
    //and throw warnings for badly constructed diagrams.
    headingSize = 0;
    PostParseProcess(); 

    //Calculate chart size
    CalculateWidthHeight();

    //If the chart ended up empty we may want to display something
    if (total.y <= chartTailGap && avoidEmpty) {
        //Add the Empty command
        Arcs.push_front((new CommandEmpty(this))->AddAttributeList(NULL));
        hscale = -1;
        //Redo calculations
        total.x = total.y = 0;
        CalculateWidthHeight();
        //Luckily Width and DrawCover calls do not generate error messages,
        //So Errors collected so far are OK even after redoing this
    }

    //A final step of prcessing, checking for additional drawing warnings
    PostPosProcessArcList(Arcs, -1);
    CloseOutput();

    Error.Sort();
}

void Msc::DrawArcList(ArcList &arcs)
{
    for (ArcList::iterator i = arcs.begin();i!=arcs.end(); i++)
        (*i)->Draw();
}


void Msc::DrawCopyrightText(int page)
{
    if (total.x==0 || !cr) return;
    XY size, dummy;
    GetPagePosition(page, dummy, size);
    StringFormat sf;
    sf.Default();
    Label label(copyrightText, this, sf);
    if (white_background) {
        MscFillAttr fill_bkg(MscColorType(255,255,255), GRADIENT_NONE);
        Fill(Block(XY(0,size.y), XY(total.x,size.y+label.getTextWidthHeight().y)), fill_bkg);
    }
    label.Draw(0, total.x, size.y);
}

void Msc::DrawPageBreaks()
{
    if (yPageStart.size()<=1) return;
    if (total.y==0 || !cr) return;
    MscLineAttr line;
    line.type.second = LINE_DASHED;
    StringFormat format;
    format.Default();
    format.Apply("\\pr\\-");
    Label label(this);
    for (unsigned page=1; page<yPageStart.size(); page++) {
        char text[20];
        const double y = yPageStart[page];
        XY d;
        Line(XY(0, y), XY(total.x, y), line);
        sprintf(text, "page %d", page);
        label.Set(text, format);
        label.Draw(0, total.x, y-label.getTextWidthHeight().y);
    }
}

void Msc::Draw(bool pageBreaks)
{
    if (total.y == 0 || !cr) return;
	//Draw small marks in corners, so EMF an WMF spans correctly
	MscLineAttr marker(LINE_SOLID, MscColorType(255,255,255), 0.1, CORNER_NONE, 0);
	Line(XY(0,0), XY(1,0), marker);
	Line(XY(total.x,total.y), XY(total.x-1,total.y), marker);
	//draw background
    MscFillAttr fill_bkg(MscColorType(255,255,255), GRADIENT_NONE);
    double y = 0;
	map<double, MscFillAttr>::iterator i = Background.begin();
    while (i!=Background.end()) {
        if (i->first != y) {
            if (y!=0 || white_background)
                Fill(Block(XY(0,y), XY(total.x,i->first)), fill_bkg);
            y = i->first;
        }
        fill_bkg += i->second;
        if (white_background && fill_bkg.color.second.a == 0)
            fill_bkg.color.second = MscColorType(255,255,255);
        i++;
    }
    if (y < total.y) {
        if (y!=0 || white_background)
            Fill(Block(XY(0,y), XY(total.x,total.y)), fill_bkg);
    }
	//Draw page breaks
    if (pageBreaks)
        DrawPageBreaks();
	//Draw initial set of entity lines (boxes will cover these and redraw)
    DrawEntityLines(0, total.y);
    DrawArcList(Arcs);
    //cairo_set_source_rgb(cr, 0, 0, 1);
    //cairo_set_line_width(cr,2);
    //HideELinesArea.Line(cr);
}

void Msc::DrawToOutput(OutputType ot, double x_scale, double y_scale, const string &fn)
{
    if (yPageStart.size()<=1) {
        SetOutput(ot, x_scale, y_scale, fn, -1);
        Draw(false);
        PrepareForCopyrightText(); //Unclip the banner text exclusion clipped in SetOutput()
        DrawCopyrightText();
        CloseOutput();
        return;
    }
    for (unsigned page=0; page<yPageStart.size(); page++) {
        SetOutput(ot, x_scale, y_scale, fn, page);
        Draw(false);
        PrepareForCopyrightText(); //Unclip the banner text exclusion clipped in SetOutput()
        DrawCopyrightText(page);
        CloseOutput();
    }
}

