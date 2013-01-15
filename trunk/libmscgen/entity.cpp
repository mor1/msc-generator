/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010,2011,2012,2013 Zoltan Turanyi
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

/** Creates a new entity.
 * @param [in] n The name of the entity.
 * @param [in] l The label of the entity.
 * @param [in] ol The label of the entity as specified by the user.
 * @param [in] p The position of the entity.
 * @param [in] pe The position of the entity, if all group entities were expanded.
 * @param [in] entity_style The style of the entity at definition.
 * @param [in] fp The location of the entity definition in the input file.
 * @param [in] coll True if we are group, but show collapsed. */
Entity::Entity(const string &n, const string &l, const string &ol,
    double p, double pe, const MscStyle &entity_style, const file_line &fp,
               bool coll) :
    name(n), label(l), orig_label(ol), file_pos(fp), pos(p), pos_exp(pe),
    index(0), status(entity_style),
    running_style(entity_style), running_shown(EEntityStatus::SHOW_OFF),
    maxwidth(0), collapsed(coll)
{
}

/** Add a list of child entities, making us a group one.
 * Check that all entities in "children" 
 * - have no parent; 
 * - exist in chart->AllEntities; 
 * - are actually defined now by this very EntityDef that is included in "children". 
 * 
 * Then add them to our children list and make us their parent.
 * Also set their and our "pos" to the leftmost of them, if we are collapsed*/
void Entity::AddChildrenList(const EntityDefList *children, Msc *chart)
{
    if (!children || children->size()==0) return;
    double min_pos = MaxVal(min_pos);
    for (auto i=children->begin(); i!=children->end(); i++) {
        EIterator ei = chart->AllEntities.Find_by_Name((*i)->name);
        _ASSERT(*ei != chart->NoEntity);
        if (!(*i)->defining) {
            chart->Error.Error((*i)->file_pos.start, "Cannot make an already existing entity part of a group.",
                               "Entity remains outside this group.");
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

    if (children_names.size())
        running_style += chart->Contexts.back().styles[collapsed?"collapsed_entity":"expanded_entity"];
}

/** Returns the width of the entityline taking active and onoff status into account.
 * It relies on running_shown, running_style.
 * Use this during PostParseProcess.
 * Returns 0 if no entity line (turned off)*/
double Entity::GetRunningWidth(double activeEntitySize) const
{
    if (!running_shown.IsOn()) return 0;
    if (running_shown.IsActive()) return activeEntitySize;
    return running_style.vline.LineWidth();
}

/** Prints the entity name and position*/
string Entity::Print(int ident) const
{
    string ss;
    ss << string(ident*2, ' ') << name << " pos:" << pos;
    return ss;
}

/** True if the position of `e1` is smaller than that of `e2`. NULL is smaller than everything.*/
inline bool SmallerByPos(Entity *e1, Entity *e2)
{
    if (e1==NULL) return true;
    if (e2==NULL) return false;
    return e1->pos < e2->pos;
}

/** True if the `pos_exp` of `e1` is smaller than that of `e2`. NULL is smaller than everything.*/
inline bool SmallerByPosExp(Entity *e1, Entity *e2)
{
    if (e1==NULL) return true;
    if (e2==NULL) return false;
    return e1->pos_exp < e2->pos_exp;
}

void EntityList::SortByPos(void)
{
    PtrList<Entity>::sort(SmallerByPos);
}

void EntityList::SortByPosExp(void)
{
    PtrList<Entity>::sort(SmallerByPosExp);
}

/** Create an EntityDef for entity named `s` onto the chart `chart`
 * Make all attributes unset, empty the style (but activate those style
 * elements that are set in the default entity style in chart->Contexts*/
EntityDef::EntityDef(const char *s, Msc* msc) : Element(msc),
    name(s),
    label(false, "", file_line()),
    pos(false, 0, file_line()),                    //field 'pos.second' is used even if no such attribute
    rel(false, "", file_line()),
    collapsed(false, false, file_line()),
    show(true, true),                              //if no attributes, we are ON if defined
    active(true, false, file_line()),              //if no attributes, we are not active if defined
    show_is_explicit(false),
    active_is_explicit(false),
    style(msc->Contexts.back().styles["entity"]),  //we will Empty it but use it for f_* values
    defining(false),
    draw_heading(false)
{
    style.Empty();
}

/** Take an attribute and apply it to us.
 *
 * We consider attributes 'label`, `pos`, `relative`, `show`, `collapsed`,
 * 'actve', 'color', applicable style attributes and any style at the current 
 * context in `chart`. 
 * At a problem, we generate an error into chart->Error.
 * @param [in] a The attribute to apply.
 * @returns True, if the attribute was recognized as ours (may have been a bad value though).*/
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
    if (a.Is("active")) {
        if (!a.CheckType(MSC_ATTR_BOOL, chart->Error)) return true;
        // MSC_ATTR_CLEAR is handled above
        active.second = a.yes;
        active.first = true;
        active.third = a.linenum_attr.start;
        active_is_explicit = true;
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
    if (Element::AddAttribute(a)) return true;
    a.InvalidAttrError(chart->Error);
    return false;
};

/** Add a list of attributes to us and a list of potential child objects.
 * This function is always called, even if there are no attributes specified 
 * (l will be NULL in that case), except for automatically generated entities.
 * (Or rather to the automatically generated EntityDef of automatically generated
 * Entity objects.)
 * If the Entity named by this EntityDef does not yet exist, we create one, set all
 * its parameters and add it to chart->AllEntities.
 * Any children are already defined at this point, so we can modify their "parent_name" field.
 * @param al The list of attributes to apply.
 * @param ch The list of arcs, we shall take our children from. This is specified 
 *           specified after the entity definition in braces. These may contain objects
 *           other than CommandEntity and CommandNote, in that case we need to give an error.
 * @param [in] l The position of the EntityDef in the file.
 * @return An EntityDefHelper which contains us and our children (if any), plus all notes.
 *         We will be the first EntityDef in the returned list, children will come after.*/
EntityDefHelper* EntityDef::AddAttributeList(AttributeList *al, ArcList *ch, file_line l)
{
    EIterator i = chart->AllEntities.Find_by_Name(name);
    if (*i != chart->NoEntity) {
        // Existing entity: kill auto-assumed "show=on" and "active=no"
        show.first = false;
        active.first = false;
        // Leave show_explicit as false
        //take the entity's draw_pass as default
        draw_pass = (*i)->running_draw_pass;
    } else {
        //indicate that this EntityDef created the Entity
        defining = true;
        draw_pass = DRAW_DEFAULT;
    }

    // Process attribute list, "style" is empty (emptied in constructor)
    if (al) {
        for (AttributeList::iterator j=al->begin(); j!=al->end(); j++)
            AddAttribute(**j);
        delete al;
    }

    EntityDefHelper *ret = new EntityDefHelper;
    //If we have children, add them to "ret->entities"
    string note_target_name = name;
    if (ch) {
        for (auto j = ch->begin(); j!=ch->end(); /*nope*/) {
            CommandEntity * const ce = dynamic_cast<CommandEntity *>(*j);
            CommandNote * const cn = dynamic_cast<CommandNote *>(*j);
            if (ce!=NULL && !ce->IsFullHeading()) {
                ce->MoveMyContentAfter(*ret);  //ce is emptied out of all EntityDefs and tmp stored notes
                note_target_name = (*(ret->entities.rbegin()))->name;
                j++;
            } else if (cn!=NULL) {
                ret->notes.Append(cn);
                ret->note_targets.push_back(note_target_name);
                ch->erase(j++);
            } else {
                chart->Error.Error((*j)->file_pos.start, "Only entity definitions are allowed here. Ignoring this.");
                j++;
            }
        }
        delete ch;
    }

    bool make_collapsed = false;
    //Check that we apply certain attributes the right way for grouped entities
    if (ret->entities.size()) {
        if (pos.first || rel.first)
            chart->Error.Error(pos.first ? pos.third : rel.third,
                               "The position of grouped entities is derived from its member entities.",
                               " Ignoring attribute.");
        pos.first = rel.first = false;
        if (active.first && active_is_explicit)
            chart->Error.Error(active.third, "You cannot directly activate or deactivate a grouped entity.",
                               "The active/inactive state of grouped entities is derived from its member entities. Ignoring attribute.");
        active.first = active_is_explicit = false;
        //We start fiddling with collapsed.second.
        //It will have a meaning from now on even if collapsed.first is false
        if (collapsed.first)
            make_collapsed = collapsed.second;
        else
            collapsed.second = false;
        auto force_itr = chart->force_entity_collapse.find(name);
        if (force_itr != chart->force_entity_collapse.end()) {
            make_collapsed = force_itr->second;
            //If force_entity_collapse is same as chart value, remove
            if (make_collapsed == force_itr->second)
                chart->force_entity_collapse.erase(force_itr);
        }
    } else {
        if (collapsed.first)
            chart->Error.Error(collapsed.third, "Only grouped entities can be collapsed.",
                               " Ignoring attribute.");
        collapsed.first = false;
    }

    if (*i == chart->NoEntity) {
        double position = chart->GetEntityMaxPos()+1;
        double position_exp = chart->GetEntityMaxPosExp()+1;
        if (rel.first) {
            //Look up the entity in a potential 'relative' attribute
            EntityList::iterator j = chart->AllEntities.Find_by_Name(rel.second);
            if (*j == chart->NoEntity) {
                if (chart->pedantic) {
                    string s = "Cound not find entity '" + rel.second;
                    s += "' in attribute 'relative'. Ignoring attriute.";
                    chart->Error.Error(rel.third, s);
                } else {
                    //Entity *e = new Entity(rel.second, rel.second, rel.second,
                    //                       chart->GetEntityMaxPos()+1,
                    //                       chart->Contexts.back().styles["entity"],
                    //                       rel.third, false);
                    //chart->AllEntities.Append(e);
                    EntityDef *ed = new EntityDef(rel.second.c_str(), chart);
                    ed->AddAttributeList(NULL, NULL, rel.third);
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
                else {
                    const EIterator right = chart->FindLeftRightDescendant(j, pos.second<0, false);
                    position     = (*right)->pos;
                    position_exp = (*right)->pos_exp;
                }
            } else {
                position     = (*j)->pos;
                position_exp = (*j)->pos_exp;
            }
        }

        //Add the effect of pos attribute to position
        if (pos.first) {
            if (pos.second<-10 || pos.second>10) {
                string msg = "Exterme value for 'pos' attribute: '";
                msg << pos.second << "'. Ignoring it.";
                chart->Error.Error(pos.third, msg, "Use a value between [-10..10].");
            } else {
                position     += pos.second;
                position_exp += pos.second;
            }
        }

        //If no show attribute, set it to "ON", but keep show_is_explicit false
        if (!show.first)
            show.second = show.first = true;

        const char *style_name;
        if (ret->entities.size()) //we are group entity
            style_name = make_collapsed ? "entitygroup_collapsed" : "entitygroup";
        else
            style_name = "entity";

        //create a fully specified string format for potential \s() \f() \c() and \mX() in label
        //also take a proper starting style and add the contents of "style" (from attributes)
        MscStyle style_to_use = chart->Contexts.back().styles[style_name];
        style_to_use.text = chart->Contexts.back().text;                     //default text
        style_to_use.text +=chart->Contexts.back().styles[style_name].text;  //entity style text
        style_to_use += style;

        //If "entity" style contains no "indicator" value (the default in plain)
        //we use the value from the context (true by default)
        if (!style_to_use.indicator.first) {
            style_to_use.indicator.first = true;
            style_to_use.indicator.second = chart->Contexts.back().indicator.second;
        }

        //Create parsed label
        string orig_label = label.first?label.second:name;
        string proc_label = orig_label;
        StringFormat::ExpandReferences(proc_label, chart, linenum_label_value,
                                          &style_to_use.text, false, true, StringFormat::LABEL);

        //Allocate new entity with correct label and children and style
        Entity *e = new Entity(name, proc_label, orig_label, position, position_exp,
                               style_to_use, file_pos.start, make_collapsed);
        e->AddChildrenList(&ret->entities, chart);  //also fixes positions & updates running_style
        e->running_draw_pass = draw_pass;
        //Add to entity list
        chart->AllEntities.Append(e);
        if (!chart->IsVirtualEntity(e))
            ret->target = name;  //if we were a group entity, use us as target for a subsequent note
    } else {
        file_line p;
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
        if (!chart->IsVirtualEntity(*i))
            ret->target = name;  //if we were a group entity, use us as target for a subsequent note
    }

    //Prepend this entity to the list (list is empty if no children)
    ret->entities.Prepend(this);
    return ret;
}


/** Add the attribute names we take to `csh`.*/
void EntityDef::AttributeNames(Csh &csh)
{
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "color", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "label", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "show", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "collapsed", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "pos", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "relative", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "active", HINT_ATTR_NAME));
    defaultDesign.styles.GetStyle("entity").AttributeNames(csh);
    Element::AttributeNames(csh);
}

/** Add a list of possible attribute value names to `csh` for attribute `attr`.*/
bool EntityDef::AttributeValues(const std::string attr, Csh &csh)
{
    if (CaseInsensitiveEqual(attr,"color")) {
        csh.AddColorValuesToHints();
        return true;
    }
    if (CaseInsensitiveEqual(attr,"label")) {
        return true;
    }
    if (CaseInsensitiveEqual(attr,"show") || CaseInsensitiveEqual(attr,"collapsed")
        || CaseInsensitiveEqual(attr,"active")) {
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
    if (Element::AttributeValues(attr, csh)) return true;
    if (defaultDesign.styles.GetStyle("entity").AttributeValues(attr, csh)) return true;
    return false;
}

/** Prints the attributes of EntityDef*/
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

/** Merges EntityDef objects referring to the same entity.
 * Used if the same entity is mentioned twice on the same list
 * or in a following list.
 * This function moves notes of `ed` to us, 
 * combines show, active & style members.*/
void EntityDef::Combine(EntityDef *ed)
{
    if (ed->show.first) 
        show = ed->show;
    if (ed->active.first) 
        active = ed->active;
    style += ed->style;
    CombineComments(ed);
 }

/** Returns how wide the entity is with this formatting, not including its shadow.*/
double EntityDef::Width() const
{
    double inner = parsed_label.getTextWidthHeight().x;
    if ((*itr)->children_names.size() && style.indicator.second && (*itr)->collapsed)
        inner = std::max(inner, GetIndiactorSize().x + 2*chart->emphVGapInside);
    const double width = ceil(style.line.LineWidth()*2 + inner);
    return width + fmod_negative_safe(width, 2.); //always return an even number
}

/** Lay out the entity head at y position zero.
 * We fill in `indicator_ypos_offset`, `outer_edge` and `area`, `area_draw`
 * and `area_important`. We add ourselves to the list of elements
 * which should not be covered by notes. In short this is a mini version of
 * the ArcBase::Layout() function called from CommandEntity::Layout().
 * Called only if the EntityDef displays a header.
 * Must not be called twice.
 * @param [out] cover Returns our cover and mainline.
 * @param [in] children A list of our children showing here, already laid out.
 * @returns The y range we occupy. Can `from` be negative if we are a group entity, 
 *          since non-group children entities will be laid out to y==0. */
Range EntityDef::Height(Area &cover, const EntityDefList &children)
{
    const XY wh = parsed_label.getTextWidthHeight();
    const double lw = style.line.LineWidth();
    const double x = chart->XCoord((*itr)->pos); //integer
    if (children.size()==0) {
        if ((*itr)->children_names.size() && style.indicator.second)
            indicator_ypos_offset = wh.y + lw + chart->emphVGapInside;
        else
            indicator_ypos_offset = -1;
        const double width = Width();
        const double indicator_height = (indicator_ypos_offset > 0) ? GetIndiactorSize().y + 2*chart->emphVGapInside : 0;
        const double height = ceil(chart->headingVGapAbove + wh.y + indicator_height + 2*lw + chart->headingVGapBelow);

        //do not include shadow in anything... but the returned height (uses for non-compressed placement)
        outer_edge = Block(x-ceil(width/2), x+ceil(width/2),
                           chart->headingVGapAbove /*- indicator_height*/,
                           height - chart->headingVGapBelow /*-indicator_height*/);

        area = style.line.CreateRectangle_OuterEdge(outer_edge.CreateExpand(-lw/2));
        area_draw.clear();
        draw_is_different = false;
        area_draw_is_frame = false;
    } else {
        indicator_ypos_offset = -1;
        outer_edge.x.from = chart->XCoord((*left_ent)->pos) - left_offset;
        outer_edge.x.till = chart->XCoord((*right_ent)->pos) + right_offset;
        double top = 0, bottom = 0;
        for (auto i = children.begin(); i!=children.end(); i++) {
            top =    std::min(top,    (*i)->outer_edge.y.from);
            bottom = std::max(bottom, (*i)->outer_edge.y.till + (*i)->style.shadow.offset.second);
        }
        outer_edge.y.from = top - chart->headingVGapAbove - ceil(wh.y + lw);
        outer_edge.y.till = bottom + chart->headingVGapBelow + lw;

        area = style.line.CreateRectangle_OuterEdge(outer_edge.CreateExpand(-lw/2));
        area_draw = area.CreateExpand(chart->trackFrameWidth) - area;
        draw_is_different = true;
        area_draw_is_frame = true;
    }
    area.arc = this;
    //Add shadow to outer_edge and place that to cover
    Area my_cover(Block(outer_edge).Shift(XY(style.shadow.offset.second,style.shadow.offset.second)) += outer_edge, this);
    my_cover.mainline = Block(chart->GetDrawing().x, outer_edge.y);
    cover = std::move(my_cover);
    const Block b = outer_edge.CreateExpand(-lw/2);
    area_important = parsed_label.Cover(b.x.from, b.x.till, b.y.from + lw/2);
    if (children.size()) 
        area_draw += area_important;
    chart->NoteBlockers.Append(this);
    return Range(outer_edge.y.from, outer_edge.y.till + style.shadow.offset.second);
}

/** Add a small block blocking notes for EntityDef objects displaying no heading.
 * This function is called iff the EntityDef shows no header.*/
void EntityDef::AddAreaImportantWhenNotShowing()
{
    //we do not draw this, but nevertheless define a small block here
    //if we are hidden, find someone who shows and has a valid "pos"
    const EIterator e = chart->FindWhoIsShowingInsteadOf(itr, false); 
    //"e" may be equal to "itr" if we are not hidden
    const double xpos = chart->XCoord((*e)->pos);
    const double w2 = style.line.LineWidth()/2;
    area_important = Block(xpos - w2, xpos + w2, -chart->compressGap/2, +chart->compressGap/2);
    area_to_note = area_important;
    chart->NoteBlockers.Append(this);
}

/** Record y-position dependent status of the Entity. 
 * Called when the EntityDef is ShiftBy()ed to its final position.
 * We hide entity lines behind us, record our style and status
 * in Entity::status and if we are a group entity we
 * create a control (for the GUI).*/
void EntityDef::PostPosProcess(MscCanvas &canvas)
{
    if (draw_heading && !hidden) {
        chart->HideEntityLines(outer_edge);
        if ((*itr)->children_names.size())
            Element::controls.push_back((*itr)->collapsed ? MSC_CONTROL_EXPAND : MSC_CONTROL_COLLAPSE);
    }
    const EEntityStatus old_status = (*itr)->status.GetStatus(yPos);
    EEntityStatus new_status = old_status;
    if (show.first)
        new_status.Show(show.second);
    if (active.first)
        new_status.Activate(active.second);
    if (new_status != old_status)
        (*itr)->status.SetStatus(yPos, new_status);
    (*itr)->status.ApplyStyle(yPos, style);
    //if (((*itr)->status.GetStatus(yPos)!=EntityStatusMap::SHOW_OFF) != shown) {
    //    if (shown)
    //        chart->Error.Warning(file_pos.start, "Entity '" + name + "' is shown due to this line, but is later turned "
    //                             "off in a parallel block above this position.", "May not be what intended.");
    //    else
    //        chart->Error.Warning(file_pos.start, "Entity '" + name + "' is not shown at to this line, but is later turned "
    //                             "on in a parallel block above this position.", "May not be what intended.");
    //}
    Element::PostPosProcess(canvas);
}

/** Draw an entity heading 
 * We use the layout calculated in Height() and affected by ShiftBy()
 * and we use the style we finalized in CommandEntity::PostParseProcess()*/
void EntityDef::Draw(MscCanvas &canvas)
{
    const double lw = style.line.LineWidth();

    Block b(outer_edge);
    MscLineAttr line2 = style.line;   //style.line.radius corresponds to midpoint of line
    line2.radius.second = std::min(std::min(outer_edge.y.Spans()/2 - lw, outer_edge.x.Spans()/2 - lw),
                                    line2.radius.second);
    if (line2.radius.second>0)
        line2.radius.second += lw-line2.width.second/2.;  //expand to outer edge
    b.Expand(-line2.width.second/2.);
    canvas.Shadow(b, style.line, style.shadow);
    if (style.fill.color.first && style.fill.color.second.valid) {
        b.Expand(-lw+style.line.width.second);
        line2.radius.second += -lw+style.line.width.second; //only decreases radius
        canvas.Fill(b, style.line, style.fill);
    }
    Block b2(outer_edge);
    b2.Expand(-lw/2);
    line2 = style.line;
    line2.radius.second -= lw/2;
    canvas.Line(b2, style.line);

    //Draw text
    parsed_label.Draw(canvas, b2.x.from, b2.x.till, b2.y.from + lw/2);
    //Draw indicator
    if (indicator_ypos_offset > 0)
        DrawIndicator(XY(outer_edge.Centroid().x, outer_edge.y.from + indicator_ypos_offset),
                      &canvas);
}

