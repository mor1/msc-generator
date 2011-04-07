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

#include <cmath>
#include "msc.h"

template class PtrList<Entity>;

Entity::Entity(const string &n, const string &l, const string &ol, 
    double p, const MscStyle &entity_style, const file_line &fp,
    bool coll) :
    maxwidth(0), running_style(entity_style),
    name(n), orig_label(ol), label(l), collapsed(coll),
    pos(p), index(0), status(entity_style), file_pos(fp)
{
}

//Check that all entities in edl have no parent; exist; are defined by this
//EntityDef in edl. Then add them to children list and make us their parent
//Also set their and our "pos" to the leftmost of them, if we are collapsed
void Entity::AddChildrenList(const EntityDefList *children, Msc *chart)
{
    if (!children || children->size()==0) return;
    double min_pos = DBL_MAX;
    for (auto i=children->begin(); i!=children->end(); i++) {
        EIterator ei = chart->AllEntities.Find_by_Name((*i)->name);
        _ASSERT(*ei != chart->NoEntity);
        if (!(*i)->defining) {
            chart->Error.Error((*i)->file_pos.start, "Cannot make an already existing entity part of a group.", 
                               " Entity remains outside this group.");
            chart->Error.Error((*ei)->file_pos, (*i)->file_pos.start, "Entity '" + (*ei)->name + "' was defined here.");
        } else {
            //if parent is already set this is a grandchilren
            if ((*ei)->parent_name.length()==0) {
                children_names.insert((*i)->name);
                (*ei)->parent_name = name;
            }
            //search for leftmost even among grandchildren
            min_pos = std::min(min_pos, (*ei)->pos);
        }
    }
    if (collapsed) {
        for (auto i=children->begin(); i!=children->end(); i++) {
            EIterator ei = chart->AllEntities.Find_by_Name((*i)->name);
            _ASSERT(*ei != chart->NoEntity);
            if ((*i)->defining) 
                (*ei)->pos = min_pos;
        }
    }
    pos = min_pos; // do this even if we are not collapsed (so that we take up no space)
}


string Entity::Print(int ident) const
{
    string ss;
    ss << string(ident*2, ' ') << name << " pos:" << pos;
    return ss;
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

EntityDef::EntityDef(const char *s, Msc* msc) : name(s),
    chart(msc), style(chart->Contexts.back().styles["entity"]),  //we will Empty it but use it for f_* values
    parsed_label(msc), implicit(false), defining(false)
{
    label.first = false;
    pos.first = false;
    pos.second = 0; //field 'pos.second' is used even if no such attribute
    rel.first = false;
    show.second = show.first = true; //if no attributes, we are ON
    show_is_explicit = false;
    style.Empty();
    collapsed.first = false;
}

bool EntityDef::AddAttribute(const Attribute& a)
{
    if (a.type == MSC_ATTR_STYLE) {
        if (chart->Contexts.back().styles.find(a.name) == chart->Contexts.back().styles.end()) {
            a.InvalidStyleError(chart->Error);
            return true;
        }
        style += chart->Contexts.back().styles[a.name];
        return true;
    }
    string s;
    if (a.Is("label")) {
        if (!a.CheckType(MSC_ATTR_STRING, chart->Error)) return true;
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
        if (!a.CheckType(MSC_ATTR_NUMBER, chart->Error)) return true;
        pos.first = true;
        pos.second = a.number;
        pos.third = a.linenum_attr.start;
        return true;
    }
    if (a.Is("relative")) {
        if (!a.CheckType(MSC_ATTR_STRING, chart->Error)) return true;
        //if MSC_ATTR_CLEAR, we are OK above and a.value is ""
        rel.second = a.value;
        rel.first = true;
        rel.third = a.linenum_attr.start;
        return true;
    }
    if (a.Is("collapsed")) {
        if (!a.CheckType(MSC_ATTR_BOOL, chart->Error)) return true;
        collapsed.second = a.yes;
        collapsed.first = true;
        collapsed.third = a.linenum_attr.start;
        return true;
    }
    if (a.Is("show")) {
        if (!a.CheckType(MSC_ATTR_BOOL, chart->Error)) return true;
        // MSC_ATTR_CLEAR is handled above
        show.second = a.yes;
        show.first = true;
        show_is_explicit = true;
        return true;
    }
    if (a.Is("color")) {
        bool was = false;
        // MSC_ATTR_CLEAR is handled by individual attributes below
        if (style.f_line) {
            style.line.AddAttribute(a, chart, style.type);
            was = true;
        }
        if (style.f_vline) {
            style.vline.AddAttribute(a, chart, style.type);
            was = true;
        }
        if (style.f_text) {
            style.text.AddAttribute(a, chart, style.type);
            was = true;
        }
        return was;
    }
    if (style.AddAttribute(a, chart)) return true;
    if (a.Is("id")) {
        s << "Attribute '"<< a.name <<"' is no longer supported. Ignoring it.";
        chart->Error.Error(a, false, s, "Try '\\^' inside a label for superscript.");
        return false;
    }
    a.InvalidAttrError(chart->Error);
    return false;
};

//This function is always called, even if there are no attributes specified (l will be NULL in that case)
//Except for automatically generated entities
//Any children are already defined at this point, so we can modify their "parent_name" field
EntityDefList* EntityDef::AddAttributeList(AttributeList *al, const ArcList *ch, file_line l)
{
    EIterator i = chart->AllEntities.Find_by_Name(name);
    if (*i != chart->NoEntity) {
        // Existing entity: kill auto-assumed "show=on"
        show.first = false;
        // Leave show_explicit as false
    } else {
        //indicate that this EntityDef created the Entity
		defining = true;
    }

    // Process attribute list
    if (al) {
        for (AttributeList::iterator j=al->begin(); j!=al->end(); j++)
            AddAttribute(**j);
        delete al;
    }

    //make style.text fully specified
    StringFormat to_use(chart->Contexts.back().text);       //default text
    to_use += chart->Contexts.back().styles["entity"].text; //entity style 
    to_use += style.text;                                   //attributes specified by user
    style.text = to_use;

    //If we have children, add them to "edl"
    EntityDefList *children = new EntityDefList;
	if (ch) {
        for (auto i = ch->begin(); i!=ch->end(); i++) {
            CommandEntity *ce = dynamic_cast<CommandEntity *>(*i);
            if (ce==NULL || ce->IsFullHeading())
                chart->Error.Error((*i)->file_pos.start, "Only entity definitions are allowed here. Ignoring this.");
            else 
                ce->MoveMyEntityDefsAfter(children);  //ce is emptied out of all EntityDefs
        }
        delete ch;
    }

    //Check that we apply certain attributes the right way for grouped entities
    if (children) {
        if (pos.first || rel.first) 
            chart->Error.Error(pos.first ? pos.third : rel.third, 
                               "The position of grouped entities is derived from its member entities.",
                               " Ignoring attribute.");
        pos.first = rel.first = false;
        //We start fiddling with collapsed.second.
        //It will have a meaning from now on even if collapsed.first is false
        if (!collapsed.first) 
            collapsed.second = false;
        auto force_itr = chart->force_entity_collapse.find(name);
        if (force_itr != chart->force_entity_collapse.end())
            collapsed.second = force_itr->second;
    } else {
        if (collapsed.first)
            chart->Error.Error(collapsed.third, "Only grouped entities can be collapsed.",
                               " Ignoring attribute.");
        collapsed.first = false;
    }

    if (*i == chart->NoEntity) {
        double position = chart->GetEntityMaxPos()+1;
        if (rel.first) {
            //Look up the entity in a potential 'relative' attribute
            EntityList::iterator j = chart->AllEntities.Find_by_Name(rel.second);
            if (*j == chart->NoEntity) {
                if (chart->pedantic) {
                    string s = "Cound not find entity '" + rel.second;
                    s += "' in attribute 'relative'. Ignoring attriute.";
                    chart->Error.Error(rel.third, s);
                } else {
                    Entity *e = new Entity(rel.second, rel.second, rel.second, 
                                           chart->GetEntityMaxPos()+1,
                                           chart->Contexts.back().styles["entity"], 
                                           rel.third, false);
                    chart->AllEntities.Append(e);
                    EntityDef *ed = new EntityDef(rel.second.c_str(), chart);
                    ed->AddAttributeList(NULL, NULL, file_line());
                    chart->AutoGenEntities.Append(ed);
                    //position remains at the old, not incremented Entity_max_pos
                    //the definedHere of the entityDef above will be set to true in chart->PostParseProcees
                }
            } else if ((*j)->children_names.size()) {
                //the entity in the "relative" attribute is a grouped one
                if (!pos.first) 
                    chart->Error.Error(rel.third, "Cannot put an entity exactly onto a grouped entity. Specify the 'pos' attribute.",
                                       " Ignoring attribute.");
                else if (pos.second==0)
                    chart->Error.Error(pos.third, "Cannot put an entity exactly onto a grouped entity. Specify the 'pos' attribute.",
                                       " Ignoring positioning attributes.");
                else 
                    position = (*chart->FindLeftRightmostChildren(j, pos.second<0))->pos;
            } else 
                position = (*j)->pos;
        }

        //Add the effect of pos attribute to position
        if (pos.first) {
            if (pos.second<-10 || pos.second>10) {
                string msg = "Exterme value for 'pos' attribute: '";
                msg << pos.second << "'. Ignoring it.";
                chart->Error.Error(pos.third, msg, "Use a value between [-10..10].");
            } else {
                position += pos.second;
            }
        }

        //If no show attribute, set it to "ON", but keep show_is_explicit false
        if (!show.first)
            show.second = show.first = true;

        string orig_label = label.first?label.second:name;
        string proc_label = orig_label;

        StringFormat::ExpandColorAndStyle(proc_label, chart, linenum_label_value,
                                          &style.text, true, StringFormat::LABEL);

        //Allocate new entity with correct label and children
        Entity *e = new Entity(name, proc_label, orig_label, position, 
                               chart->Contexts.back().styles["entity"], 
                               file_pos.start, collapsed.second);
        e->AddChildrenList(children, chart);  //also fixes positions
        //Add to entity list
        chart->AllEntities.Append(e);
    } else {
        file_line p(-1,0,0);
        // An existing entity. Disallow attributes that change drawing positions
        if (label.first && label.second != (*i)->label) 
            chart->Error.Error(p = label.third,
                             "Cannot change the label of an entity after declaration. Keeping old label :'"
                             + (*i)->orig_label + "'.");

        if (pos.first)
            chart->Error.Error(p = pos.third,
                               "Cannot change the position of an entity after declaration. Ignoring attribute 'pos'.");
        if (rel.first)
            chart->Error.Error(p = rel.third,
                               "Cannot change the position of an entity after declaration. Ignoring attribute 'relative'.");
        if (collapsed.first)
            chart->Error.Error(p = collapsed.third,
                               "You can only declare an entity collapsed at its definition. Ignoring attribute 'collapsed'.");
        //if any of the above errors, add extra info
        if (p.file!=-1)
            chart->Error.Error(file_pos.start, p, "Entity '" + name + "' was defined here.");
        label.first = pos.first = rel.first = collapsed.first = false;

        //Also indicate an error at any attempt to add children (even if "ch" did not
        //contain actual entities, it signals we had a "{ ... }" block
        if (ch) {
            chart->Error.Error(l, "You can specify an entity group only when first defining an entity. "
		                      "Ignoring grouping and placing entities just after.");
            chart->Error.Error(file_pos.start, l, "Entity '" + name + "' was defined here.");
        }
    }

    //Prepend this entity to the list (list is empty if no children)
    children->Prepend(this);
    return children;
}


void EntityDef::AttributeNames(Csh &csh)
{
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "color", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "label", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "show", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "collapsed", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "pos", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "relative", HINT_ATTR_NAME));
    MscStyle style(STYLE_DEFAULT, ArrowHead::NONE, true, true, true, true, true, false, false, false, false); //no arrow, solid numbering compress side
    style.AttributeNames(csh);

}

bool EntityDef::AttributeValues(const std::string attr, Csh &csh)
{
    if (CaseInsensitiveEqual(attr,"color")) {
        csh.AddColorValuesToHints();
        return true;
    }
    if (CaseInsensitiveEqual(attr,"label")) {
        return true;
    }
    if (CaseInsensitiveEqual(attr,"show") || CaseInsensitiveEqual(attr,"collapsed")) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE) + "yes", HINT_ATTR_VALUE, true, CshHintGraphicCallbackForYesNo, CshHintGraphicParam(1)));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE) + "no", HINT_ATTR_VALUE, true, CshHintGraphicCallbackForYesNo, CshHintGraphicParam(0)));
        return true;
    }
    if (CaseInsensitiveEqual(attr,"pos")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number>", HINT_ATTR_VALUE, false));
        return true;
    }
    if (CaseInsensitiveEqual(attr,"relative")) {
        csh.AddEntitiesToHints();
        return true;
    }
    MscStyle style(STYLE_DEFAULT, ArrowHead::NONE, true, true, true, true, true, false, false, false, false); //no arrow, solid numbering compress side
    if (style.AttributeValues(attr, csh)) return true;
    return false;
}

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

//returns how wide the entity is, not including its shadow
double EntityDef::Width() const
{
    const double width = ceil(style.line.LineWidth()*2 + parsed_label.getTextWidthHeight().x);
    return width + fmod(width, 2); //always return an even number
}

Range EntityDef::Height(AreaList &cover, const EntityDefList &children)
{
    const XY wh = parsed_label.getTextWidthHeight();
    const double lw = style.line.LineWidth();
    if (children.size()==0) {
        const double x = chart->XCoord((*itr)->pos); //integer
        const double width = lw*2 + wh.x;
        const double height = ceil(chart->headingVGapAbove + wh.y + chart->headingVGapBelow + 2*lw);

        //do not include shadow in anything... but the returned height (uses for non-compressed placement)
        outer_edge = Block(x-ceil(width/2), x+ceil(width/2), chart->headingVGapAbove, height - chart->headingVGapBelow);
    } else {
        outer_edge.x.from = chart->XCoord((*left_ent)->pos) - left_offset;
        outer_edge.x.till = chart->XCoord((*right_ent)->pos) + right_offset;
        double top = 0, bottom = 0;
        for (auto i = children.begin(); i!=children.end(); i++) {
            top =    std::min(top,    (*i)->outer_edge.y.from);
            bottom = std::max(bottom, (*i)->outer_edge.y.till + (*i)->style.shadow.offset.second);
        }
        outer_edge.y.from = top - chart->headingVGapAbove - ceil(wh.y + lw);
        outer_edge.y.till = bottom + chart->headingVGapBelow + lw;
    }
    area = style.line.CreateRectangle(outer_edge);
    area.arc = this;
    //Add shadow to outer_edge and place that to cover
    cover += Contour(Block(outer_edge) += Block(outer_edge).Shift(XY(style.shadow.offset.second,style.shadow.offset.second)));
    cover.mainline += outer_edge.y;
    return Range(outer_edge.y.from, outer_edge.y.till + style.shadow.offset.second);
}

void EntityDef::PostPosProcess()
{
    if (shown) 
        chart->HideEntityLines(outer_edge);
    if (show.first)
        (*itr)->status.SetStatus(yPos, show.second ? EntityStatusMap::SHOW_ON : EntityStatusMap::SHOW_OFF);
    //if (((*itr)->status.GetStatus(yPos)!=EntityStatusMap::SHOW_OFF) != shown) {
    //    if (shown)
    //        chart->Error.Warning(file_pos.start, "Entity '" + name + "' is shown due to this line, but is later turned "
    //                             "off in a parallel block above this position.", "May not be what intended.");
    //    else
    //        chart->Error.Warning(file_pos.start, "Entity '" + name + "' is not shown at to this line, but is later turned "
    //                             "on in a parallel block above this position.", "May not be what intended.");
    //}
    if (implicit) return;
    if (!area.IsEmpty()) {
        area = area.CreateExpand(chart->trackExpandBy);
        chart->AllCovers += area;
    }
    if (draw_is_different && !area_draw.IsEmpty())
        area_draw = area_draw.CreateExpand(chart->trackExpandBy);
    chart->AllArcs[file_pos] = this;
}


void EntityDef::Draw()
{
    const double lw = style.line.LineWidth();

    Block b(outer_edge);
    MscLineAttr line2 = style.line;   //style.line.radius corresponds to midpoint of line
    line2.radius.second = std::min(std::min(outer_edge.y.Spans()/2 - lw, outer_edge.x.Spans()/2 - lw),
                                    line2.radius.second);
    if (line2.radius.second>0) 
        line2.radius.second += lw-line2.width.second/2.;  //expand to outer edge
    b.Expand(-line2.width.second/2.);
    chart->Shadow(b, style.line, style.shadow);
    if (style.fill.color.first && style.fill.color.second.valid) {
        b.Expand(-lw+style.line.width.second);
        line2.radius.second += -lw+style.line.width.second; //only decreases radius
        chart->Fill(b, style.line, style.fill);
    }
    Block b2(outer_edge);
    b2.Expand(-lw/2);
    line2 = style.line;
    line2.radius.second -= lw/2;
    chart->Line(b2, style.line);

    //Draw text
    parsed_label.Draw(b2.x.from, b2.x.till, b2.y.from + lw/2);
}

