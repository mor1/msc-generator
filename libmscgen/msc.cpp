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
#include <climits>
#include "msc.h"

using namespace std;

bool Geometry::Overlaps(const Geometry &g, double gap) const
{
    for(set<Block>::const_iterator i=cover.begin(); i!=cover.end(); i++)
        for(set<Block>::const_iterator j=g.cover.begin(); j!=g.cover.end(); j++)
            if (j->Overlaps(*i, gap))
                return true;
    return false;
}

MscStyle::MscStyle(StyleType tt) : type(tt)
{
    f_line=f_vline=f_fill=f_shadow=f_text=f_arrow=f_solid=f_numbering=f_compress=true;
    Empty();
}

MscStyle::MscStyle(StyleType tt, bool a, bool t, bool l, bool f, bool s, bool vl, bool so, bool nu, bool co) :
    type(tt), f_arrow(a), f_text(t), f_line(l), f_vline(vl), f_fill(f),
    f_shadow(s), f_solid(so), f_numbering(nu), f_compress(co)
{
    solid.first=so;
    solid.second = 128;
    compress.first = co;
    compress.second = false;
    numbering.first = nu;
    numbering.second = false;
}

void MscStyle::Empty()
{
    line.Empty();
    vline.Empty();
    fill.Empty();
    text.Empty();
    arrow.Empty();
    shadow.Empty();
    solid.first = false;
    compress.first = false;
    numbering.first = false;
}

MscStyle & MscStyle::operator +=(const MscStyle &toadd)
{
    if (toadd.f_line && f_line) line += toadd.line;
    if (toadd.f_vline && f_vline) vline += toadd.vline;
    if (toadd.f_fill && f_fill) fill += toadd.fill;
    if (toadd.f_shadow &&f_shadow) shadow += toadd.shadow;
    if (toadd.f_text && f_text) text += toadd.text;
    if (toadd.f_arrow && f_arrow) arrow += toadd.arrow;
    if (toadd.f_solid && f_solid && toadd.solid.first) solid = toadd.solid;
    if (toadd.f_compress && f_compress && toadd.compress.first) compress = toadd.compress;
    if (toadd.f_numbering && f_numbering && toadd.numbering.first) numbering = toadd.numbering;
    return *this;
}

bool MscStyle::AddAttribute(const Attribute &a, Msc *msc)
{
    if (a.type == MSC_ATTR_STYLE) {
        if (msc->StyleSets.top().find(a.name) == msc->StyleSets.top().end()) {
            a.InvalidStyleError(msc->Error);
            return true;
        }
        operator +=(msc->StyleSets.top()[a.name]);
        return true;
    }
    if (a.Is("line.width")) {
        if (f_arrow) arrow.AddAttribute(a, msc, type);
        if (f_line) line.AddAttribute(a, msc, type);
        return f_arrow || f_line;
    }
    if ((a.StartsWith("text") || a.Is("ident")) && f_text)
        return text.AddAttribute(a, msc, type);
    if (a.StartsWith("line") && f_line)
        return line.AddAttribute(a, msc, type);
    if (a.StartsWith("vline") && f_vline)
        return vline.AddAttribute(a, msc, type);
    if (a.StartsWith("fill") && f_fill)
        return fill.AddAttribute(a, msc, type);
    if (a.StartsWith("shadow") && f_shadow)
        return shadow.AddAttribute(a, msc, type);
    if ((a.StartsWith("arrow") || a.Is("arrowsize")) && f_arrow)
        return arrow.AddAttribute(a, msc, type);
    if (a.Is("solid") && f_solid) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, type))
                solid.first = false;
            return true;
        }
        if (a.type == MSC_ATTR_NUMBER && a.number>=0 && a.number <= 255) {
            solid.first = true;
            solid.second = a.number<=1 ? (unsigned char)(a.number*255) : (unsigned char)a.number;
            return true;
        }
        a.InvalidValueError("0..1' or '0..255", msc->Error);
    }
    if (a.Is("compress")) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, type))
                compress.first = false;
            return true;
        }
        if (!a.CheckType(MSC_ATTR_BOOL, msc->Error)) return true;
        compress.first = true;
        compress.second = a.yes;
        return true;
    }
    if (a.Is("number")) {
        if (a.type == MSC_ATTR_CLEAR) {
            if (a.EnsureNotClear(msc->Error, type))
                numbering.first = false;
            return true;
        }
        if (a.type != MSC_ATTR_BOOL) {
            msc->Error.Error(a, true, "The 'number' attribute must be 'yes' or 'no' for styles. Ignoring it.");
            return true;
        }
        numbering.first = true;
        numbering.second = a.yes;
        return true;
    }
    return false;
}

string MscStyle::Print(int ident) const
{
    string s = "style:(";
    if (f_line) s.append(line.Print());
    if (f_vline) s.append(vline.Print());
    if (f_fill) s.append(fill.Print());
    if (f_shadow) s.append(shadow.Print());
    if (f_solid) s.append("solid:").append(solid.second?"yes":"no").append("\n");
//    if (f_arrow) s.append(arrow.Print());
//    if (f_text) s.append(text.Print());
    s.append(")");
    return s;
}

const MscStyle &StyleSet::GetStyle(const string &s) const
{
    const_iterator i = find(s);
    if (i==end()) return defaultStyle;
    else return i->second;
};

void Design::Reset()
{
    hscale = 1.0;

    colors.clear();
    colors["none"]  = MscColorType(  0,   0,   0, 0);
    colors["black"] = MscColorType(  0,   0,   0);
    colors["white"] = MscColorType(255, 255, 255);
    colors["red"]   = MscColorType(255,   0,   0);
    colors["green"] = MscColorType(  0, 255,   0);
    colors["blue"]  = MscColorType(  0,   0, 255);
    colors["yellow"]= MscColorType(255, 255,   0);
    colors["gray"]  = MscColorType(150, 150, 150);
    colors["lgray"] = MscColorType(200, 200, 200);

    styles.clear();
    styles.numbering=false;
    styles.compress=false;

    MscStyle style(STYLE_DEFAULT, true, true, true, false, false, false, false, true, true); //no fill, shadow, vline solid
    style.compress.first = false;
    style.line.radius.second = -1;
    styles["arrow"] = style;

    style.Empty();
    style.type = STYLE_STYLE;
    style.compress.first = false;
    style.line.type.first = true;
    style.line.type.second = LINE_SOLID;
    styles["->"] = style;
    style.line.type.second = LINE_DOTTED;
    styles[">"] = style;
    style.line.type.second = LINE_DASHED;
    styles[">>"] = style;
    style.line.type.second = LINE_DOUBLE;
    styles["=>"] = style;

    style= MscStyle(STYLE_DEFAULT, true, true, true, true, false, false, false, true, true);  //no shadow, vline solid
    style.compress.first = false;
    style.line.radius.second = -1;
    styles["blockarrow"] = style;

    style.Empty();
    style.type = STYLE_STYLE;
    style.line.type.first = true;
    style.line.type.second = LINE_SOLID;
    styles["block->"] = style;
    style.line.type.second = LINE_DOTTED;
    styles["block>"] = style;
    style.line.type.second = LINE_DASHED;
    styles["block>>"] = style;
    style.line.type.second = LINE_DOUBLE;
    styles["block=>"] = style;

    style= MscStyle(STYLE_DEFAULT, true, true, true, true, false, false, false, true, true);  //no shadow, vline solid
    style.line.radius.second = -1;
    styles["vertical"] = style;

    style.Empty();
    style.type = STYLE_STYLE;
    style.line.type.first = true;
    style.line.type.second = LINE_SOLID;
    styles["vertical->"] = style;
    style.line.type.second = LINE_DOTTED;
    styles["vertical>"] = style;
    style.line.type.second = LINE_DASHED;
    styles["vertical>>"] = style;
    style.line.type.second = LINE_DOUBLE;
    styles["vertical=>"] = style;

    style.arrow.startType.first = true;
    style.arrow.startType.second = MSC_ARROW_NONE;
    style.arrow.midType.first = true;
    style.arrow.midType.second = MSC_ARROW_NONE;
    style.arrow.endType.first = true;
    style.arrow.endType.second = MSC_ARROW_NONE;
    style.line.type.second = LINE_SOLID;
    styles["vertical--"] = style;
    style.line.type.second = LINE_DASHED;
    styles["vertical++"] = style;
    style.line.type.second = LINE_DOTTED;
    styles["vertical.."] = style;
    style.line.type.second = LINE_DOUBLE;
    styles["vertical=="] = style;

    style = MscStyle(STYLE_DEFAULT, false, true, true, false, false, true, false, true, true); //no arrow, fill, shadow solid
    style.vline.Empty();
    style.line.type.second = LINE_NONE;
    styles["divider"] = style;

    style.Empty();
    style.type = STYLE_STYLE;
    style.line.type.first = true;
    style.line.type.second = LINE_DOTTED;
    styles["---"] = style;
    style.Empty();
    style.vline.type.first = true;
    style.vline.type.second = LINE_DOTTED;
    style.text.Apply("\\mu(10)\\md(10)");
    styles["..."] = style;

    style = MscStyle(STYLE_DEFAULT, false, true, true, true, true, false, false, true, true); //no arrow, vline solid
    style.compress.first = false;
    styles["emptyemphasis"] = style;
    style.text.Apply("\\pl");
    style.line.type.first = false;
    styles["emphasis"] = style;

    style.Empty();
    style.type = STYLE_STYLE;
    style.line.type.first = true;
    style.line.type.second = LINE_SOLID;
    styles["--"] = style;
    style.line.type.second = LINE_DASHED;
    styles["++"] = style;
    style.line.type.second = LINE_DOTTED;
    styles[".."] = style;
    style.line.type.second = LINE_DASHED;
    style.line.type.second = LINE_DOUBLE;
    styles["=="] = style;

    style = MscStyle(STYLE_DEFAULT, false, true, true, true, true, false, true, true, true); //no arrow, vline
    style.compress.first = false;
    style.line.radius.second = 5;
    styles["pipe"] = style;

    style.Empty();
    style.type = STYLE_STYLE;
    style.line.type.first = true;
    style.line.type.second = LINE_SOLID;
    styles["pipe--"] = style;
    style.line.type.second = LINE_DASHED;
    styles["pipe++"] = style;
    style.line.type.second = LINE_DOTTED;
    styles["pipe.."] = style;
    style.line.type.second = LINE_DASHED;
    style.line.type.second = LINE_DOUBLE;
    styles["pipe=="] = style;


    style = MscStyle(STYLE_DEFAULT, false, true, true, true, true, true, false, false, false); //no arrow, solid numbering compress
    styles["entity"] = style;

    style = MscStyle(STYLE_STYLE); //has everything, but is empty
    MscLineAttr line(MscColorType(150,150,150));
    style.line += line;;
    style.vline += line;
    style.arrow.line += line;
    style.text.SetColor(line.color.second);
	style.text.Apply("\\i");
    styles["weak"] = style;

    style.Empty();
    line.Empty();
    line.width.first = true;
    line.width.second = 2.0;
    style.line += line;
    style.vline += line;
    style.arrow.line += line;
    style.text.Apply("\\b");
    styles["strong"] = style;
}

void EntityStatusMap::SetRange(Range pos, EntityStatus status)
{
    if (pos.from == pos.till) return;
    std::map<double, EntityStatus>::iterator i;
    i = storage.upper_bound(pos.till);  //the value just after pos.till
    EntityStatus es = defaultStatus;
    if (i!=storage.begin()) {//if there is acually a setting at till
        //store line status that shall be restored at till
        i--;  //i now points to or before pos.till
        es = i->second;
        //remove all state changes between from and till
        while (pos.from < i->first)
            if (i==storage.begin()) {
                storage.erase(i);
                break;
            } else
                storage.erase(i--);
    }
    storage[pos.from] = status;
    storage[pos.till] = es;
}

EntityStatus EntityStatusMap::Get(double pos) const
{
    std::map<double, EntityStatus>::const_iterator i;
    i = storage.upper_bound(pos);  //the value just after pos
    if (i==storage.begin())
        return defaultStatus;
    return (--i)->second;
}

double EntityStatusMap::Till(double pos) const
{
    std::map<double, EntityStatus>::const_iterator i;
    i = storage.upper_bound(pos);
    if (i==storage.end())
        return INT_MAX; //big num if no further status line changes
    return i->first;
}

string EntityStatusMap::Print(int ident)
{
    string s(ident*2, ' ');
    for (std::map<double, EntityStatus>::iterator i=storage.begin(); i!=storage.end(); i++)
        s << i->first << ":" << (i->second.status?"on":"off") << "\n";
    return s;
}

template class PtrList<Entity>;

Entity::Entity(const string &n, const string &l, double p, Msc* msc) :
    name(n), label(l), pos(p), index(0), chart(msc)
{
	style = chart->StyleSets.top()["entity"];
	status.SetDefaultStatus(EntityStatus(style.vline, false));
}

string Entity::Print(int ident) const
{
    string ss;
    ss << string(ident*2, ' ') << name << " pos:" << pos;
    return ss;
}

double Entity::Width()
{
    if (name == LSIDE_ENT_STR || name == RSIDE_ENT_STR) return 0;
    Label parsed_label(label, chart, style.text);
    XY wh = parsed_label.getTextWidthHeight();
	return style.line.LineWidth()*2 + wh.x + style.shadow.offset.second;
}

//This can be called multiple times with final=true.
//Once for each time we draw this entity heading
double Entity::DrawHeight(double y, Geometry &g, bool draw, bool final)
{
    double xcoord = chart->XCoord(pos);
    Label parsed_label(label, chart, style.text);
    XY wh = parsed_label.getTextWidthHeight();
    double lw = style.line.LineWidth();
	double height = chart->headingVGapAbove + wh.y + chart->headingVGapBelow +
		              2*lw + style.shadow.offset.second;
    XY s(xcoord - wh.x/2 - lw, y);
    XY d(xcoord + wh.x/2 + lw, y + height - style.shadow.offset.second);
    Block b(s,d+XY(0, style.shadow.offset.second));
    g.cover.insert(b);
    g.mainline.Extend(b.y);
    s.y += chart->headingVGapAbove;
    d.y -= chart->headingVGapBelow;
    if (final)
        status.HideRange(Range(s.y, d.y));
    if (!draw) return height;
    //Fill color
	if (draw && style.fill.color.first && style.fill.color.second.valid) {
        chart->filledRectangle(s, d, style.fill, style.line.radius.second);
        chart->shadow(s, d, style.shadow, style.line.radius.second, true);
	}
    //Draw line around
    chart->rectangle(s, d, style.line);
    //Draw text
    set<Block> dummy;
    parsed_label.DrawCovers(xcoord, xcoord, s.y + lw, dummy, true);
    return height;
}


inline bool SmallerByPos(Entity *e1, Entity *e2)
{
    if (e1==NULL) return true;
    if (e2==NULL) return false;
    return e1->pos < e2->pos;
}

void EntityList::SortByPos(void)
{
    PtrList<Entity>::sort(SmallerByPos);
}

EntityDef::EntityDef(const char *s, Msc* chart) : name(s),
    style(STYLE_ARC, false/*arrow*/, true, true, true, true, true,
	  false /*solid*/, false /*numbering*/, false /*compress*/)
{
    label.first = false;
    pos.first = false;
    pos.second = 0; //field 'pos' is used even if no such attribute
    rel.first = false;
    show.second = show.first = true; //if no attributes, we are ON
    style.Empty();
}

EntityDef* EntityDef::AddAttributeList(AttributeList *l, Msc *msc)
{
    EIterator i = msc->Entities.Find_by_Name(name);
    if (i != msc->NoEntity) {
        // Existing entity: kill auto-assumed "show=on"
        show.first = false;
    }

    // Process attribute list
    if (l) {
        for (AttributeList::iterator j=l->begin(); j!=l->end(); j++)
            AddAttribute(**j, msc);
        delete l;
    }

    if (i == msc->NoEntity) {
        //Calculate pos attribute
        if (rel.first) {
            //Look up the entity in a potential 'relative' attribute
            EntityList::iterator j = msc->Entities.Find_by_Name(rel.second);
            if (j == msc->NoEntity) {
                if (msc->pedantic) {
                    string s;
                    s << "Cound not find entity '" << rel.second;
                    s << "' in attribute 'relative'. Ignoring attriute.";
                    msc->Error.Error(rel.third, s);
                    pos.second += msc->Entity_max_pos;
                } else {
                    Entity *e;
                    e = new Entity(rel.second, rel.second, msc->Entity_max_pos, msc);
                    msc->Entities.Append(e);
                    msc->AutoGenEntities.Append(new EntityDef(rel.second.c_str(), msc));
                    pos.second += msc->Entity_max_pos;  //the pos of the new entity
                    msc->Entity_max_pos ++;  //Increment for the new entity
                }
            }
        } else
            pos.second += msc->Entity_max_pos;

        //If no show attribute, set it to "ON"
        if (!show.first)
            show.second = show.first = true;

        //Allocate new entity with correct label
        Entity *e = new Entity(name, label.first?label.second:name, pos.second, msc);
        //Add to entity list
        msc->Entities.Append(e);
        //Calculate potential pos for next entity
        msc->Entity_max_pos = std::max(msc->Entity_max_pos, pos.second+1);
    } else {
        // An existing entity. Disallow attributes that change drawing positions
        if (label.first && label.second != (*i)->label)
            msc->Error.Error(label.third,
                             "Cannot change the label of an entity after declaration. Keeping old label :'"
                             + (*i)->label + "'.");
        if (pos.first)
            msc->Error.Error(pos.third,
                               "Cannot change the position of an entity after declaration. Ignoring attribute 'pos'.");
        if (rel.first)
            msc->Error.Error(rel.third,
                               "Cannot change the position of an entity after declaration. Ignoring attribute 'relative'.");
    }
    return this;
}

bool EntityDef::AddAttribute(const Attribute& a, Msc *msc)
{
    if (a.type == MSC_ATTR_STYLE) {
        if (msc->StyleSets.top().find(a.name) == msc->StyleSets.top().end()) {
            a.InvalidStyleError(msc->Error);
            return true;
        }
        style += msc->StyleSets.top()[a.name];
        return true;
    }
    string s;
    if (a.Is("label")) {
        if (!a.CheckType(MSC_ATTR_STRING, msc->Error)) return true;
        //if MSC_ATTR_CLEAR, we are OK above and a.value is ""
        label.first = true;
        label.second = a.value;
        label.third = a.linenum_attr;
        return true;
    }
    if (a.Is("pos")) {
        if (a.type == MSC_ATTR_CLEAR) {
            pos.first = true;
            pos.second = 0;
            pos.third = a.linenum_attr;
            return true;
        }
        if (!a.CheckType(MSC_ATTR_NUMBER, msc->Error)) return true;
        pos.first = true;
        pos.second = a.number;
        pos.third = a.linenum_attr;
        return true;
    }
    if (a.Is("relative")) {
        if (!a.CheckType(MSC_ATTR_STRING, msc->Error)) return true;
        //if MSC_ATTR_CLEAR, we are OK above and a.value is ""
        rel.second = a.value;
        rel.first = true;
        rel.third = a.linenum_attr;
        return true;
    }
    if (a.Is("show")) {
        if (!a.CheckType(MSC_ATTR_BOOL, msc->Error)) return true;
        // MSC_ATTR_CLEAR is handled above
        show.second = a.yes;
        show.first = true;
        return true;
    }
    if (a.Is("color")) {
        bool was = false;
        // MSC_ATTR_CLEAR is handled by individual attributes below
        if (style.f_line) {
            style.line.AddAttribute(a, msc, style.type);
            was = true;
        }
        if (style.f_vline) {
            style.vline.AddAttribute(a, msc, style.type);
            was = true;
        }
        if (style.f_text) {
            style.text.AddAttribute(a, msc, style.type);
            was = true;
        }
        return was;
    }
    if (style.AddAttribute(a, msc)) return true;
    if (a.Is("id")) {
        s << "Attribute '"<< a.name <<"' is no longer supported. Ignoring it.";
        msc->Error.Error(a, false, s, "Try '\\^' inside a label for superscript.");
        return false;
    }
    a.InvalidAttrError(msc->Error);
    return false;
};

string EntityDef::Print(int ident) const
{
    string ss;
    ss << string(ident*2, ' ') << name;

    if (label.first) ss << "(\"" << label.second << "\")";
    if (pos.first) ss << " pos:" << pos.second;
    if (rel.first) ss << " rel:" << rel.second;
    if (show.first) ss << " show:" << show.second;
    ss << " " << style.Print();
    return ss;
};


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

EIterator Msc::FindAllocEntity(const char *e, file_line l, bool*validptr)
{
    if (e==NULL)
        return NoEntity;
    EIterator ei = Entities.Find_by_Name(e);
    if (ei == NoEntity) {
        if (pedantic)
            Error.Warning(l, "Unknown entity '" + string(e)
                          + "'. Assuming implicit definition.",
                          "This may be a mistyped entity name."
                          " Try turning 'pedantic' off to remove these messages.");
        Entity *entity = new Entity(e, e, Entity_max_pos++, this);
        Entities.Append(entity);
        AutoGenEntities.Append(new EntityDef(e, this));
        ei = Entities.Find_by_Name(e);
    }
    return ei;
}

ArcArrow *Msc::CreateArcArrow(MscArcType t, const char*s, file_line sl, const char*d, file_line dl, file_line l)
{
    if (strcmp(s,d))
        return new ArcDirArrow(t,s,sl,d,dl,l, this, StyleSets.top()["arrow"]);
    MscStyle style = StyleSets.top()["arrow"];
    style.text.Apply("\\pr");
    return new ArcSelfArrow(t,s,l, this, style, selfArrowYSize);
}

ArcBigArrow *Msc::CreateArcBigArrow(const ArcBase *base)
{
    const ArcDirArrow *arrow = dynamic_cast<const ArcDirArrow *>(base);
    //We can only get ArcSelfArrow or ArcDirArrow here
    if (!arrow) {
        Error.Error(base->linenum, "Big arrows cannot point back to the same entity. Ignoring it.");
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
            Error.Warning(a.linenum_value, "Unknown chart design: '" + a.value +
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
    if (a.Is("strict")) {
        if (!a.CheckType(MSC_ATTR_BOOL, Error)) return true;
        Error.strict = a.yes;
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


void Msc::PushContext()
{
    ColorSets.push(ColorSets.top());
    StyleSets.push(StyleSets.top());
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
        if (ce) {
            ArcList::iterator j = i;
            j++;
            if (j!=arcs.end()) {
                CommandEntity *ce2 = dynamic_cast<CommandEntity *>(*j);
                if (ce2) {
                    ce->Combine(ce2);
                    delete ce2;
                    arcs.erase(j);
                    continue; //i remains at this very same CommandEntity!
                }
            }
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
        i = Arcs.insert(i, new CommandEntity(new EntityDefList, file_line(0, 0), this));
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



void Msc::HideEntityLines(const std::set<Block> &blocks)
{
    for (EIterator i = Entities.begin(); i!=Entities.end(); i++) {
        if ((*i)->name == NONE_ENT_STR) continue;
        if ((*i)->name == LSIDE_ENT_STR) continue;
        if ((*i)->name == RSIDE_ENT_STR) continue;
        double xpos = XCoord((*i)->pos);
        for (set<Block>::const_iterator b = blocks.begin(); b!=blocks.end(); b++)
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
        double till = y+height;
        while (up.y < till) {
            EntityStatus s = (*from)->status.Get(up.y);
            down.y = min((*from)->status.Till(up.y), till);
            if (s.status)
                line(up, down, s.line);
            up.y = down.y;
        }
        from++;
    }
}

//This is not symmetric. We have the 'a' blocks established
//and want to see if the 'b' blocks (supposedly below the 'a' ones)
//can be shifted up, and by how much
double Msc::FindCollision(const set<Block> &a, const set<Block> &b,
                          double &CollisionYPos) const
{
    double y = INT_MAX;
    for (set<Block>::const_iterator i=a.begin(); i!=a.end(); i++)
        for (set<Block>::const_iterator j=b.begin(); j!=b.end(); j++)
			if (i->x.Overlaps(j->x, compressXGap))
                if (y > j->y.from - i->y.till - compressYGap) {
                    y = j->y.from - i->y.till - compressYGap;
                    CollisionYPos = i->y.till + compressYGap/2;
                }
    return y;
}

void Msc::WidthArcList(ArcList &arcs, EntityDistanceMap &distances)
{
    for (ArcList::iterator i = arcs.begin();i!=arcs.end(); i++)
        (*i)->Width(distances);
}

double Msc::DrawHeightArcList(ArcList &arcs, double y, Geometry &g,
                              bool draw, bool final, bool &prevCompress,
                              double autoMarker)
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
    ArcList::iterator first_zero_height = arcs.end();

    for (ArcList::iterator i = arcs.begin(); i!=arcs.end(); i++) {
        double size;
        //if we draw y will be ignored by arcs' DrawHeight
        //so we need not calculate compress again
        if (!draw && (*i)->compress && prevCompress) {
            Geometry geom;
            size = (*i)->DrawHeight(y, geom, false, false, autoMarker);
            if (size == 0) {
                if (first_zero_height == arcs.end())
                    first_zero_height = i;
                continue;
            }
            double up, CollisionYPos = -1;
            if (geom.mainline.from == INT_MAX && geom.cover.size()==0) {
                //if the arc has nothing to draw, do not move up
                up =0;
            } else {
                up = FindCollision(g.cover, geom.cover, CollisionYPos);
                up = min (up, geom.mainline.from - g.mainline.till);
                //if (up<0) up =0;
            }
            //Now we know how much be can compess it up
            //position known, re-invoke any previous zero-height at this pos
            //so that they can store correct y pos if final==true
            // use the median of the two mainlines, if both exists
            // and the returned collision position is outside
            if (g.mainline.till != 0 && geom.mainline.from != INT_MAX)
                if (CollisionYPos < g.mainline.till ||
                    CollisionYPos > geom.mainline.from-up)
                    CollisionYPos = (g.mainline.till + geom.mainline.from-up)/2;
            while (first_zero_height != i && first_zero_height != arcs.end())
                (*first_zero_height++)->DrawHeight(CollisionYPos, g, false, final,
                                                   autoMarker);
            first_zero_height = arcs.end();
            //recalculate cover again for current arc and add it's cover to g
            size = (*i)->DrawHeight(y-up, g, false, final, autoMarker);
            size -= up;
        } else {
            //if we do not yet draw and last arcs before a non-compressed one
            //were of zero height re-invoke them at the current pos
            if (!draw) {
                while (first_zero_height != i && first_zero_height != arcs.end())
                    (*first_zero_height++)->DrawHeight(y, g, false, final, autoMarker);
                first_zero_height = arcs.end();
            }
            size = (*i)->DrawHeight(y, g, draw, final, autoMarker);
        }
        prevCompress = (*i)->compress;
        y += size;
        if (largest_y < y)
            largest_y = y;
    }
    //if we do not yet draw and last arcs were of zero height
    //re-invoke them at final pos
    if (!draw)
        while (first_zero_height != arcs.end())
            (*first_zero_height++)->DrawHeight(y, g, false, final, autoMarker);

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
        distances.CombineLeftRightToPair_Max(hscaleAutoXGap);
        distances.CombineLeftRightToPair_Single(hscaleAutoXGap);

        if (hscale<0) {
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
        //not draw but final
        bool prevCompress = false;
        double height = DrawHeightArcList(Arcs, 0, g, false, true, prevCompress, -1);
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


void Msc::ParseText(const char *input, unsigned length, const char *filename)
{
    current_pos.file = Error.AddFile(filename);
    MscParse(*this, input, length);
}

void Msc::CompleteParse(OutputType ot)
{
    //Allocate (non-sized) output object and assign it to the chart
    //From this point on, the chart sees xy dimensions
    SetOutput(ot);

    //Sort Entities, add numbering, fill in auto-calculated values,
    //and throw warnings for badly constructed diagrams.
    PostParseProcess();

    CalculateWidthHeight();
    //A final step of prcessing, checking for additional drawing warnings
    PostHeightProcess();
    CloseOutput();
}

void Msc::DrawCopyrightText(int page)
{
	if (totalWidth==0 || !cr) return;
	XY size, dummy;
	GetPagePosition(page, dummy, size);
	std::set<Block> dummy2;
	Label label(copyrightText, this, StringFormat());
	if (white_background) {
	    MscFillAttr fill_bkg(MscColorType(255,255,255), GRADIENT_NONE);
		filledRectangle(XY(0,size.y), XY(totalWidth,size.y+label.getTextWidthHeight().y), fill_bkg);
	}
	label.DrawCovers(0, totalWidth, size.y, dummy2, true);
}

void Msc::Draw(void)
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

    DrawEntityLines(0, totalHeight);
    //draw and not final (both true would be confusing)
    Geometry g;
    bool prevCompress = false;
    DrawHeightArcList(Arcs, 0, g, true, false, prevCompress, -1);
}

void Msc::DrawToOutput(OutputType ot, const string &fn)
{
    if (yPageStart.size()<=1) {
        SetOutput(ot, fn, -1);
        Draw();
		UnClip(); //Unclip the banner text exclusion clipped in SetOutput()
		DrawCopyrightText();
        CloseOutput();
        return;
    }
    for (unsigned page=0; page<yPageStart.size(); page++) {
        SetOutput(ot, fn, page);
        Draw();
		UnClip(); //Unclip the banner text exclusion clipped in SetOutput()
		DrawCopyrightText(page);
        CloseOutput();
    }
}

