/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010 Zoltan Turanyi
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

void TrackableElement::SetLineEnd(file_line_range l, bool f)
{
    if (linenum_final) return;
    linenum_final = f;
    file_pos = l;
}

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
double EntityDistanceMap::Query(unsigned e1, int e2)
{
    if (e1==e2) return 0;
    if (e2==DISTANCE_LEFT) {
        std::map<unsigned, double>::iterator i = left.find(e1);
        if (i==left.end()) return 0;
        return i->second;
    }
    if (e2==DISTANCE_RIGHT) {
        std::map<unsigned, double>::iterator i = right.find(e1);
        if (i==right.end()) return 0;
        return i->second;
    }
    IPair eip(e1, e2);
    std::map<IPair, double, IPairComp>::iterator i = pairs.find(eip);
    if (i==pairs.end()) return 0;
    return i->second;
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
    Entities(true),
    AutoGenEntities(false),
    Arcs(true),
    Entity_max_pos(0)
{
    chartTailGap = 3;
    selfArrowYSize = 12;
    headingVGapAbove = 2;
    headingVGapBelow = 2;
    emphVGapOutside = 2;
    emphVGapInside = 2;
    arcVGapAbove = 0;
    arcVGapBelow = 3;
    nudgeSize = 4;
    compressXGap = 2;
    compressYGap = 2;
    hscaleAutoXGap = 5;

    pedantic=false;
    ignore_designs = false;

    //Add topmost style and color sets (global context), all empty now
    StyleSets.push(StyleSet());
    ColorSets.push(ColorSet());

    //Add "plain" style - the default constructor of Design sets that
    Designs["plain"];
    //Apply "plain" design
    SetDesign("plain", true);

    //Add virtual entities
    Entity *entity = new Entity(NONE_ENT_STR, NONE_ENT_STR, -1001, this);
    Entities.Append(entity);
    NoEntity = Entities.begin();
    entity = new Entity(LSIDE_ENT_STR, LSIDE_ENT_STR, -1000, this);
    Entities.Append(entity);
    entity = new Entity(RSIDE_ENT_STR, RSIDE_ENT_STR, 10000, this);
    Entities.Append(entity);
}

void Msc::AddCSH(CshPos&pos, MscColorSyntaxType i)
{
    CshEntry e;
    e.first_pos = pos.first_pos;
    e.last_pos = pos.last_pos;
    e.color = i;
    CshList.push_back(e);
}

void Msc::AddCSH_AttrValue(CshPos& pos, const char *value, const char *name)
{
    if (!name || CaseInsensitiveEqual(name, "label") ||
        CaseInsensitiveEqual(name, "text.format")) {
        //This is a label or text.format
        AddCSH(pos, COLOR_LABEL_TEXT);
        //Add escape symbols
        StringFormat::ExtractCSH(pos.first_pos, value, *this);
    } else {
        // No match - regular attribute value
        AddCSH(pos, COLOR_ATTRVALUE);
    }
}

void Msc::AddCSH_ColonString(CshPos& pos, const char *value, bool processComments)
{
    CshPos colon = pos;
    colon.last_pos = colon.first_pos;
    AddCSH(colon, COLOR_COLON);
    pos.first_pos++;
    char *copy = strdup(value);
    if (processComments) {
        char *p = copy;
        while (*p!=0) {
            //search for #
            while (*p!=0 && *p!='#') p++;
            if (!*p) break;
            //if we hit a # count the \s before
            unsigned count = 0;
            //string starts with colon, so we are limited by that
            while (*(p-1-count) == '\\') count++;
            //if even number then replace comment with spaces till end of line
			if (count%2 == 0) {
				CshPos comment;
				comment.first_pos = pos.first_pos + (p - copy)-1;
                while (*p!=0 && *p!=0x0d && *p!=0x0a) *(p++) = ' ';
				comment.last_pos = pos.first_pos + (p - copy);
				AddCSH(comment, COLOR_COMMENT);
			} else
				p++; //step over the escaped #

        }
    }
    AddCSH_AttrValue(pos, copy+1, NULL);
    free(copy);
}

const char *const opt_names[] = {"msc", "hscale", "compress", "numbering",
"pedantic", "strict", ""};

const char *const attr_names[] = {"compress", "color", "label", "number", "id",
"pos", "relative", "show", "makeroom", "readfrom", "offset",
"text.color", "text.ident", "ident", "text.format",
"arrow", "arrowsize", "arrow.size", "arrow.type", "arrow.starttype", "arrow.midtype",
"arrow.endtype", "arrow.color",
"line.color", "line.type", "line.width", "line.radius",
"vline.color", "vline.type", "vline.width",
"fill.color", "fill.gradient", "shadow.color", "shadow.offset", "shadow.blur", ""};

int find_opt_attr_name(const char *name, const char * const array[])
{
    for (int i=0; array[i][0]; i++)
        switch (CaseInsensitiveBeginsWidth(array[i], name)) {
        case 1: return 1;
        case 2: return 2;
        }
    return 0;
}

void Msc::AddCSH_AttrName(CshPos&pos, const char *name, MscColorSyntaxType color)
{
    char const *const *array;
    if (color == COLOR_OPTIONNAME) array = opt_names;
    if (color == COLOR_ATTRNAME) array = attr_names;
    switch (find_opt_attr_name(name, array)) {
    case 0: AddCSH(pos, COLOR_ERROR); return;
    case 1: AddCSH(pos, MscColorSyntaxType(color+1)); return;
    case 2: AddCSH(pos, color); return;
    }
}

void Msc::AddCSH_EntityName(CshPos&pos, const char *name)
{
    if (CshEntityNames.insert(string(name)).second)
        AddCSH(pos, COLOR_ENTITYNAME_FIRST);
    else
        AddCSH(pos, COLOR_ENTITYNAME);
}

void Msc::ParseForCSH(const char *input, unsigned len)
{
    //initialize data struct
    CshList.clear();
    CshEntityNames.clear();
    CshParse(*this, input, len);
}

bool Msc::SetDesign(const string&name, bool force)
{
    std::map<string,Design>::const_iterator i = Designs.find(name);
    if (i==Designs.end())
        return false;
    if (ignore_designs &&!force)
        return true;
    for (ColorSet::const_iterator j = i->second.colors.begin(); j!=i->second.colors.end(); j++)
        ColorSets.top()[j->first] = j->second;
    for (StyleSet::const_iterator j = i->second.styles.begin(); j!=i->second.styles.end(); j++)
        StyleSets.top()[j->first] = j->second;
    StyleSets.top().numbering = i->second.styles.numbering;
    StyleSets.top().compress  = i->second.styles.compress;
    hscale = i->second.hscale;
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

EIterator Msc::FindAllocEntity(const char *e, file_line_range l, bool*validptr)
{
    if (e==NULL)
        return NoEntity;
    EIterator ei = Entities.Find_by_Name(e);
    if (ei == NoEntity) {
        if (pedantic)
            Error.Error(l.start, "Unknown entity '" + string(e)
                        + "'. Assuming implicit definition.",
                        "This may be a mistyped entity name."
                        " Try turning 'pedantic' off to remove these messages.");
        Entity *entity = new Entity(e, e, Entity_max_pos++, this);
        Entities.Append(entity);
        EntityDef *ed = new EntityDef(e, this);
        ed->SetLineEnd(l);
        AutoGenEntities.Append(ed);
        ei = Entities.Find_by_Name(e);
    }
    return ei;
}

ArcArrow *Msc::CreateArcArrow(MscArcType t, const char*s, file_line_range sl,
                              const char*d, file_line_range dl)
{
    if (strcmp(s,d))
        return new ArcDirArrow(t, s, sl, d, dl, this, StyleSets.top()["arrow"]);
    MscStyle style = StyleSets.top()["arrow"];
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
    return new ArcBigArrow(*arrow, StyleSets.top()["blockarrow"]);

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
        StyleSets.top().compress = a.yes;
        return true;
    }
    if (a.Is("numbering")) {
        if (!a.CheckType(MSC_ATTR_BOOL, Error)) return true;
        StyleSets.top().numbering = a.yes;
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
    if (a.Is("numbering") || a.Is("compress") || a.Is("hscale") || a.Is("msc"))
        return AddAttribute(a);
    Error.Warning(a, false, "Cannot set attribute '" + a.name +
                  "' as part of a design definition. Ignoring it.");
    return false;
}


void Msc::PushContext(bool empty)
{
    if (empty) {
        ColorSets.push(ColorSet());
        StyleSets.push(StyleSet());
        SetDesign("plain", true);
    } else {
        ColorSets.push(ColorSets.top());
        StyleSets.push(StyleSets.top());
    }
}

void Msc::PopContext()
{
    ColorSets.pop();
    StyleSets.pop();
}

string Msc::Print(int ident) const
{
    string s = Entities.Print(ident).append("\n");
    s.append(Arcs.Print(ident)).append("\n");
    return s;
}

void Msc::PostParseProcessArcList(ArcList &arcs, bool resetiterators,
                                  EIterator &left, EIterator &right,
                                  int &number, bool top_level)
{
    for (ArcList::iterator i = arcs.begin(); i != arcs.end(); i++) {
        if (resetiterators) {
            left = NoEntity;
            right = NoEntity;
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
        (*i)->PostParseProcess(left, right, number, top_level);
    }
}

void Msc::PostParseProcess(void)
{
    //Sort the defined entities as will be displayed from left to right
    Entities.SortByPos();

    //Set index field in Entities
    unsigned index = 0;
    for (EIterator temp = Entities.begin(); temp!=Entities.end(); temp++, index++)
        (*temp)->index = index;

    EIterator lside = Entities.Find_by_Name(LSIDE_ENT_STR);
    EIterator rside = Entities.Find_by_Name(RSIDE_ENT_STR);

    //Find the first real entity
    EIterator tmp = Entities.begin();
    if (tmp == NoEntity) tmp++;
    if (tmp == lside) tmp++;
    if (tmp == rside) tmp++;

    //Ensure that leftmost real entity pos == 2*MARGIN
    double rightmost = 0;
    if (tmp != Entities.end()) {
        double leftmost = (*tmp)->pos - 2*MARGIN;
        for  (EIterator i = Entities.begin(); i != Entities.end(); i++) {
            const_cast<double&>((*i)->pos) -= leftmost;
            if (i==lside || i==rside || i==NoEntity) continue;
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
    Entities.SortByPos();

    //Add the Auto generated entities to the list of entities
    //Add them to the the first arc if it is an EntityCommand,
    //Otherwise, generate a new entity command as first arc
    ArcList::iterator i = Arcs.begin();
    if (i == Arcs.end()) return; //we cannot have autogen entities either.
    if ((*i)->type != MSC_COMMAND_ENTITY)
        i = Arcs.insert(i, new CommandEntity(new EntityDefList, this));
    dynamic_cast<CommandEntity*>(*i)->AppendToEntities(AutoGenEntities);

    //Traverse Arc tree and perform post-parse processing
    //1. Add line numbers to labels
    //2. Calculate which entities to cover for auto-adjusting emphasis boxes
    //3. Print a few warning messages
    //4. Combine CommandEntities following one another into a signle one.
    int number = 1; //start numbering from 1.
    EIterator dummy1=NoEntity, dummy2=NoEntity;
    PostParseProcessArcList(Arcs, true, dummy1, dummy2, number, true);
}



void Msc::HideEntityLines(const Geometry &geom)
{
    for (EIterator i = Entities.begin(); i!=Entities.end(); i++) {
        if ((*i)->name == NONE_ENT_STR) continue;
        if ((*i)->name == LSIDE_ENT_STR) continue;
        if ((*i)->name == RSIDE_ENT_STR) continue;
        double xpos = XCoord((*i)->pos);
        for (set<Block>::const_iterator b = geom.GetCover().begin(); b!=geom.GetCover().end(); b++)
            if (b->x.from <= xpos && xpos <= b->x.till)
                (*i)->status.HideRange(b->y);
    }
}

void Msc::DrawEntityLines(double y, double height,
                          EIterator from, EIterator to)
{
    //No checking of iterators!! Call with caution
    //"to" is not included!!
    while(from != to) {
        XY up(XCoord((*from)->pos), y);
        XY down(up.x, y);
        const double till = y+height;
        while (up.y < till) {
            down.y = min((*from)->status.Till(up.y), till);
			if ((*from)->status.GetHideStatus(up.y) &&
				(*from)->status.GetStatus(up.y))
                line(up, down, (*from)->status.GetStyle(up.y).vline);
            up.y = down.y;
        }
        from++;
    }
}

//This is not symmetric. We have the 'a' blocks established
//and want to see if the 'b' blocks (supposedly below the 'a' ones)
//can be shifted up, and by how much
double Msc::FindCollision(const Geometry &a, const Geometry &b,
                          double &CollisionYPos) const
{
    double up = MAINLINE_INF;
	if (a.mainline.HasValidTill() && b.mainline.HasValidFrom()) {
		up = b.mainline.from - a.mainline.till - compressYGap;
		CollisionYPos = a.mainline.till + compressYGap/2;
	}
    for (set<Block>::const_iterator i=a.GetCover().begin(); i!=a.GetCover().end(); i++)
        for (set<Block>::const_iterator j=b.GetCover().begin(); j!=b.GetCover().end(); j++)
			if (i->x.Overlaps(j->x, compressXGap))
                if (up > j->y.from - i->y.till - compressYGap) {
                    up = j->y.from - i->y.till - compressYGap;
                    CollisionYPos = i->y.till + compressYGap/2;
                }
    return up;
}

void Msc::WidthArcList(ArcList &arcs, EntityDistanceMap &distances)
{
    for (ArcList::iterator i = arcs.begin();i!=arcs.end(); i++)
        (*i)->Width(distances);
}

//This one places a list at low_y, then compresses up taking g into account,
//but no higher than top_y (low_y is supposed to be the bottom of g).
//Compression is done if the first element in the list has compress=true
//or if forceCompress=true
//After placement covers are added to g_result, g is unchanged
//returns how much to advance y from low_y
//The whole reason for this function is to prevent a big empty space
//between the first and second arrow, when the first arrow can be
//compressed a lot, but the second not. Here we move all the arrows
//as one block only up till none of them collides with something.
double Msc::PlaceDrawListUnder(ArcList::iterator from, ArcList::iterator to,
                               double top_y, double low_y,
                               Geometry &g, Geometry &g_result,
                               bool draw, bool final, bool forceCompress)
{
    if (from == to) return 0;
    if (draw)
        return DrawHeightArcList(from, to, low_y, g_result, draw, final);
    double original_low_y = low_y;
    if ((*from)->IsCompressed() || forceCompress) {
        Geometry geom_content;
        //set an upper limit for the list - just to make them start fully below low_y
        Block limiter_low(0, totalWidth, low_y, low_y);
        geom_content += limiter_low;
        DrawHeightArcList(from, to, low_y, geom_content, false, false);
        geom_content -= limiter_low;
        //Now see how much we can shift the content upwards as one block
        if (geom_content.GetCover().size()>0) {
            Block limiter_top(0, totalWidth, top_y, top_y);
            g += limiter_top;
            low_y -= FindCollision(g, geom_content);
            g -= limiter_top;
        }
    }

    Block limiter_at_exact_position(0, totalWidth, low_y, low_y);
    g_result += limiter_at_exact_position;
    low_y += DrawHeightArcList(from, to, low_y, g_result, false, final);
    g_result -= limiter_at_exact_position;
    return low_y - original_low_y;
}

double Msc::DrawHeightArcList(ArcList::iterator from, ArcList::iterator to,
                              double y, Geometry &g,
                              bool draw, bool final, double autoMarker)
{
    double original_y = y;

    //largest_y is maintained so that the full bottom of the list is seen
    //for the cases when the last elements are compressed so that their
    //bottom is _higher_ then one of the previous ones.
    double largest_y = y;

    //Zero-height arcs shall be positioned to the same place
    //as the first non-zero height arc below them (so that
    //if that arc is compressed up, they are not _below_
    //that following arc. So we store what was the last non-zero
    //height arc so we can go back and adjust the zero height ones
    ArcList::iterator first_zero_height = to;

    //if one arc is set "parallel" the next one should be compressed,
    //even if not set so
    bool previousWasParallel = false;
    //the one coming after a parallel should not be compressed higher than this
    double lastNonParallelBottom = y;

    for (ArcList::iterator i = from; i!=to; i++) {
        double orig_mainline_till = g.mainline.till;
        double size;
        //if we draw y will be ignored by arcs' DrawHeight
        //so we need not calculate compress again
        if (!draw && (*i)->IsCompressed()) {
            Geometry geom;
            //Drawheight for non final locations can be called at fractional y
            size = (*i)->DrawHeight(y, geom, false, false, autoMarker);
            //if it is of zero height, just collect it, its pos may depend on
            //the next arc if that is compressed
            //Note that we do this here where we are only if we do not draw.
            if (size == 0) {
                if (first_zero_height == to)
                    first_zero_height = i;
                continue;
            }
            double up = 0, CollisionYPos = -1;
            if (geom.mainline.HasValidFrom() || geom.GetCover().size()>0)
                up = FindCollision(g, geom, CollisionYPos);
            //Now we know how much be can compess it up
            //position known, re-invoke any previous zero-height at this pos
            //so that they can store correct y pos if final==true
            // use the median of the two mainlines, if both exists
            // and the returned collision position is outside
            if (g.mainline.HasValidTill() && geom.mainline.HasValidFrom())
                if (CollisionYPos < g.mainline.till ||
                    CollisionYPos > geom.mainline.from-up)
                    CollisionYPos = (g.mainline.till + geom.mainline.from-up)/2;
            CollisionYPos = ceil(CollisionYPos);
            while (first_zero_height != i && first_zero_height != to)
                (*first_zero_height++)->DrawHeight(CollisionYPos, g, false, final,
                                                   autoMarker);
            first_zero_height = to;
            //recalculate cover again for current arc and add it's cover to g
            //We ensure DrawHeight is always called on integer y position for bitmaps
            double draw_y = ceil(y-up);
            draw_y += (*i)->DrawHeight(draw_y, g, false, final, autoMarker);
            size = draw_y - y;
        } else {
            //Ensure drawheight is only called at integer positions
            y = ceil(y);
            //if we do not yet draw and last arcs before a non-compressed one
            //were of zero height re-invoke them at the current pos
            if (!draw) {
                while (first_zero_height != i && first_zero_height != to)
                    (*first_zero_height++)->DrawHeight(y, g, false, final, autoMarker);
                first_zero_height = to;
            }
            size = (*i)->DrawHeight(y, g, draw, final, autoMarker);
        }
        y += size;
        if (largest_y < y)
            largest_y = y;
        //If we are parallel draw the rest of the block in one go
        if ((*i)->IsParallel()) {
            //First place the collected zero heights here (unlikely to have any)
            if (!draw) {
                while (first_zero_height != i && first_zero_height != to)
                    (*first_zero_height++)->DrawHeight(y, g, false, final, autoMarker);
                first_zero_height = to;
            }
            //reset mainline to original one
            g.mainline.till = orig_mainline_till;
            //Place blocks, always compress the first
            y += PlaceDrawListUnder(++i, to, y - size, y, g, g, draw, final, true);
            break; //we have drawn all
        }

    }
    //if we do not yet draw and last arcs were of zero height
    //re-invoke them at final pos
    if (!draw) {
        y = ceil(y);
        if (largest_y < y)
            largest_y = y;
        while (first_zero_height != to)
            (*first_zero_height++)->DrawHeight(y, g, false, final, autoMarker);
    }
    return largest_y - original_y;
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


void Msc::CalculateWidthHeight(void)
{
    yPageStart.clear();
    yPageStart.push_back(0);

    if (totalHeight == 0) {
        //start with width calculation, that is used by many elements
        EntityDistanceMap distances;
        //Add distance for arcs,
        //needed for hscale=auto, but also for entity width calculation
        WidthArcList(Arcs, distances);
        if (hscale<0) {
            distances.CombineLeftRightToPair_Max(hscaleAutoXGap);
            distances.CombineLeftRightToPair_Single(hscaleAutoXGap);

            //Now go through all the pairwise requirements and calc actual pos.
            //dist will hold required distance to the right of entity with index []
            vector<double> dist(Entities.size(), 0);
            dist[0] = 0;
            dist[1] = XCoord(MARGIN_HSCALE_AUTO);
            dist[dist.size()-2] = XCoord(MARGIN_HSCALE_AUTO);
            //distances.pairs starts with requiremenst between neighbouring entities
            //and continues with requirements between second neighbours, ... etc.
            //we process these sequentially
            for (map<IPair, double, IPairComp>::iterator i = distances.pairs.begin();
                 i!=distances.pairs.end(); i++) {
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
            for (EntityList::iterator j = Entities.begin(); j!=Entities.end(); j++) {
                (*j)->pos = curr_pos;
                curr_pos += dist[index++]/unit;
            }
            totalWidth = XCoord((*--(Entities.end()))->pos+MARGIN_HSCALE_AUTO);
        } else {
            totalWidth = XCoord((*--(Entities.end()))->pos+MARGIN);
        }
        XY crTexSize = Label(copyrightText, this, StringFormat()).getTextWidthHeight();
        if (totalWidth<crTexSize.x) totalWidth = crTexSize.x;

        copyrightTextHeight = crTexSize.y;
        Geometry g;
		g += Block(0, totalWidth, 0, 0); //limiter
        //not draw but final
        double height = DrawHeightArcList(Arcs.begin(), Arcs.end(), 0, g, false, true);
        totalHeight = height + chartTailGap;
    }
}

void Msc::PostHeightProcess(void)
{
    if (totalHeight == 0) return;
    for (ArcList::iterator j = Arcs.begin(); j != Arcs.end(); j++)
        (*j)->PostHeightProcess();
//    for (EntityList::iterator i = Entities.begin(); i!=Entities.end(); i++)
//        cerr << (*i)->name << std::endl << (*i)->status.Print(1);
}


void Msc::ParseText(const char *input, const char *filename)
{
    current_file = Error.AddFile(filename);
    MscParse(*this, input, strlen(input));
}

void Msc::CompleteParse(OutputType ot, bool avoidEmpty)
{

    //Allocate (non-sized) output object and assign it to the chart
    //From this point on, the chart sees xy dimensions
    SetOutput(ot);

    //Sort Entities, add numbering, fill in auto-calculated values,
    //and throw warnings for badly constructed diagrams.
    PostParseProcess();

    //Calculate chart size
    CalculateWidthHeight();

    //If the chart ended up empty we may want to display something
    if (totalHeight <= chartTailGap && avoidEmpty) {
        //Add the Empty command
        Arcs.push_front(new CommandEmpty(this));
        hscale = -1;
        //Redo calculations
        totalWidth = totalHeight = 0;
        CalculateWidthHeight();
        //Luckily Width and DrawCover calls do not generate error messages,
        //So Errors collected so far are OK even after redoing this
    }

    //A final step of prcessing, checking for additional drawing warnings
    PostHeightProcess();
    CloseOutput();

    Error.Sort();
}

void Msc::DrawCopyrightText(int page)
{
    if (totalWidth==0 || !cr) return;
    XY size, dummy;
    GetPagePosition(page, dummy, size);
    Label label(copyrightText, this, StringFormat());
    if (white_background) {
        MscFillAttr fill_bkg(MscColorType(255,255,255), GRADIENT_NONE);
        filledRectangle(XY(0,size.y), XY(totalWidth,size.y+label.getTextWidthHeight().y), fill_bkg);
    }
    Geometry dummy2;
    label.DrawCovers(0, totalWidth, size.y, dummy2, true);
}

void Msc::DrawPageBreaks()
{
    if (yPageStart.size()<=1) return;
    if (totalHeight==0 || !cr) return;
    MscLineAttr line;
    line.type.second = LINE_DASHED;
    StringFormat format;
    format.Apply("\\pr\\-");
    Label label(this);
    Geometry dummy;
    for (unsigned page=1; page<yPageStart.size(); page++) {
        char text[20];
        const double y = yPageStart[page];
        XY d;
        MscDrawer::line(XY(0, y), XY(totalWidth, y), line);
        sprintf(text, "page %d", page);
        label.Set(text, format);
        label.DrawCovers(0, totalWidth, y-label.getTextWidthHeight().y, dummy, true);
    }
}

void Msc::Draw(bool pageBreaks)
{
    if (totalHeight == 0 || !cr) return;
    MscFillAttr fill_bkg(MscColorType(255,255,255), GRADIENT_NONE);
    double y = 0;
    map<double, MscFillAttr>::iterator i = Background.begin();
    while (i!=Background.end()) {
        if (i->first != y) {
            if (y!=0 || white_background)
                filledRectangle(XY(0,y), XY(totalWidth,i->first), fill_bkg);
            y = i->first;
        }
        fill_bkg += i->second;
        i++;
    }
    if (y < totalHeight) {
        if (y!=0 || white_background)
            filledRectangle(XY(0,y), XY(totalWidth,totalHeight), fill_bkg);
    }
    if (pageBreaks)
        DrawPageBreaks();
    DrawEntityLines(0, totalHeight);
    //draw and not final (both true would be confusing)
    Geometry g;
    DrawHeightArcList(Arcs.begin(), Arcs.end(), 0, g, true, false);
}

void Msc::DrawToOutput(OutputType ot, const string &fn)
{
    if (yPageStart.size()<=1) {
        SetOutput(ot, fn, -1);
        Draw(false);
        UnClip(); //Unclip the banner text exclusion clipped in SetOutput()
        DrawCopyrightText();
        CloseOutput();
        return;
    }
    for (unsigned page=0; page<yPageStart.size(); page++) {
        SetOutput(ot, fn, page);
        Draw(false);
        UnClip(); //Unclip the banner text exclusion clipped in SetOutput()
        DrawCopyrightText(page);
        CloseOutput();
    }
}

