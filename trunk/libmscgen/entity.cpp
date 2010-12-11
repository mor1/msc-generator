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

#include "msc.h"

template class PtrList<Entity>;

Entity::Entity(const string &n, const string &l, const string &ol, double p, Msc* msc) :
    chart(msc), maxwidth(0), running_style(chart->Contexts.back().styles["entity"]),
    name(n), orig_label(ol), label(l), pos(p), index(0), status(chart->Contexts.back().styles["entity"])
{
}

string Entity::Print(int ident) const
{
    string ss;
    ss << string(ident*2, ' ') << name << " pos:" << pos;
    return ss;
}

//returns how wide the entity is drawn with a certain style
double Entity::Width(const MscStyle &style)
{
    const Label parsed_label(label, chart, style.text);
    const XY wh = parsed_label.getTextWidthHeight();
    return style.line.LineWidth()*2 + wh.x + style.shadow.offset.second;
}

//This can be called multiple times with final=true.
//Once for each time we draw this entity heading
double Entity::DrawHeight(double y, Geometry &g, bool draw, bool final)
{
    const MscStyle &style = status.GetStyle(y);
    const double xcoord = chart->XCoord(pos);
    const Label parsed_label(label, chart, style.text);
    const XY wh = parsed_label.getTextWidthHeight();
    const double lw = style.line.LineWidth();
    const double height = chart->headingVGapAbove + wh.y + chart->headingVGapBelow +
        2*lw + style.shadow.offset.second;
    XY s(xcoord - wh.x/2 - lw, y);
    XY d(xcoord + wh.x/2 + lw, y + height - style.shadow.offset.second);
    if (!draw) {
        const Block b(s,d+XY(0, style.shadow.offset.second));
        g += b;
        g.mainline += b.y;
    }
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
    Geometry dummy;
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
        double position = msc->Entity_max_pos;
        if (rel.first) {
            //Look up the entity in a potential 'relative' attribute
            EntityList::iterator j = msc->Entities.Find_by_Name(rel.second);
            if (j == msc->NoEntity) {
                if (msc->pedantic) {
                    string s = "Cound not find entity '" + rel.second;
                    s += "' in attribute 'relative'. Ignoring attriute.";
                    msc->Error.Error(rel.third, s);
                } else {
                    msc->Entities.Append(new Entity(rel.second, rel.second, rel.second, msc->Entity_max_pos++, msc));
                    msc->AutoGenEntities.Append(new EntityDef(rel.second.c_str(), msc));
                    //position remains at the old, not incremented Entity_max_pos
                }
            } else {
                position = (*j)->pos;
            }
        }

        //Add the effect of pos attribute to position
        if (pos.first) {
            if (pos.second<-10 || pos.second>10) {
                string msg = "Exterme value for 'pos' attribute: '";
                msg << pos.second << "'. Ignoring it.";
                msc->Error.Error(pos.third, msg, "Use a value between [-10..10].");
            } else {
                position += pos.second;
            }
        }

        //If no show attribute, set it to "ON"
        if (!show.first)
            show.second = show.first = true;

        string orig_label = label.first?label.second:name;
        string proc_label = orig_label;

        StringFormat::ExpandColorAndStyle(proc_label, msc, linenum_label_value,
                                          &style.text, true, StringFormat::LABEL);


        //Allocate new entity with correct label
        Entity *e = new Entity(name, proc_label, orig_label, position, msc);
        //Add to entity list
        msc->Entities.Append(e);
        //Calculate potential pos for next entity
        msc->Entity_max_pos = std::max(msc->Entity_max_pos, position+1);
    } else {
        // An existing entity. Disallow attributes that change drawing positions
        if (label.first && label.second != (*i)->label)
            msc->Error.Error(label.third,
                             "Cannot change the label of an entity after declaration. Keeping old label :'"
                             + (*i)->orig_label + "'.");
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
        if (msc->Contexts.back().styles.find(a.name) == msc->Contexts.back().styles.end()) {
            a.InvalidStyleError(msc->Error);
            return true;
        }
        style += msc->Contexts.back().styles[a.name];
        return true;
    }
    string s;
    if (a.Is("label")) {
        if (!a.CheckType(MSC_ATTR_STRING, msc->Error)) return true;
        //if MSC_ATTR_CLEAR, we are OK above and a.value is ""
        label.first = true;
        label.second = a.value;
        label.third = a.linenum_attr.start;
        linenum_label_value = a.linenum_value.start;
        return true;
    }
    if (a.Is("pos")) {
        if (a.type == MSC_ATTR_CLEAR) {
            pos.first = true;
            pos.second = 0;
            pos.third = a.linenum_attr.start;
            return true;
        }
        if (!a.CheckType(MSC_ATTR_NUMBER, msc->Error)) return true;
        pos.first = true;
        pos.second = a.number;
        pos.third = a.linenum_attr.start;
        return true;
    }
    if (a.Is("relative")) {
        if (!a.CheckType(MSC_ATTR_STRING, msc->Error)) return true;
        //if MSC_ATTR_CLEAR, we are OK above and a.value is ""
        rel.second = a.value;
        rel.first = true;
        rel.third = a.linenum_attr.start;
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


