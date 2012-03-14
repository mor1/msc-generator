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
#include "msc.h"

using namespace std;

string ArcCommand::Print(int ident) const
{
    string ss;
    ss << string(ident*2, ' ');
    ss << PrintType();
    return ss;
}

//////////////////////////////////////////////////////////////////////////////////////

CommandEntity::CommandEntity(EntityDefList *e, Msc *msc)
    : ArcCommand(MSC_COMMAND_ENTITY, msc)
{
    full_heading = (e==NULL);
    if (e) {
        entities.splice(entities.end(), *e);
        delete e;
    }
}


bool CommandEntity::AddAttribute(const Attribute &)
{
    return false;
}

void CommandEntity::AttributeNames(Csh &)
{
    return;
}

bool CommandEntity::AttributeValues(const std::string, Csh &)
{
    return false;
}

TrackableElement* CommandEntity::AttachNote(CommandNote *cn)
{
    _ASSERT(CanBeNoted());
    if (entities.size())
        return (*entities.rbegin())->AttachNote(cn);
    else 
        return ArcBase::AttachNote(cn);
}

string CommandEntity::Print(int ident) const
{
    string ss;
    ss << string(ident*2, ' ');
    ss << "Entity Command";
    if (full_heading) ss<<"(full_heading)";
    for (auto i = entities.begin();i != entities.end(); i++)
        ss << "\n" << (*i)->Print(ident+1);
    return ss;
}

//This is called only from "Combine" and Msc::PostParseProcess(), so definitely
//After AddAttrLits and before PostParseProcess, so here are the members
//with comments on how to merge a later entityDef into a former one.
//name                 //This is const, shall be the same
//label;               //This can only be set once: keep the former
//linenum_label_value; //pos of label text, irrelevant in later one
//pos;                 //THis is used only in AddAttributeList
//rel;                 //THis is used only in AddAttributeList
//collapsed;           //THis is used only in AddAttributeList
//show;                //the latter shall overwrite the former one
//active;              //the latter shall overwrite the former one
//show_is_explicit;    //ignore, This is only used in ApplyPrefix which is only called during parse
//itr;                 //this is set during PostParse, ignore
//style;               //this is finalized during PostParse, combine latter into former
//parsed_label;        //will be set during PostParse, ignore
//defining;            //keep former
//shown;               //ignore, will be set in PostParse
void CommandEntity::AppendToEntities(const EntityDefList &e)
{
    for (auto i = e.begin(); i!=e.end(); i++) {
        auto i2 = entities.begin();
        for (/*nope*/; i2!=entities.end(); i2++)
            if ((*i2)->name == (*i)->name) break;
        if (i2 == entities.end()) {
            entities.Append(*i);
        } else {
            (*i2)->style += (*i)->style;
            if ((*i)->show.first)
                (*i2)->show = (*i)->show;
            if ((*i)->active.first)
                (*i2)->active = (*i)->active;
        }
    }
}

void CommandEntity::Combine(CommandEntity *ce)
{
    if (!ce) return;
    if (!ce->valid) return;
    //Always keep the line_pos of the "heading" command
    //If we are already one, keep ours
    if (!full_heading && ce->full_heading)
        file_pos = ce->file_pos;
    if (ce->full_heading) full_heading = true;
    AppendToEntities(ce->entities);
    ce->entities.clear();
    CombineNotes(ce); //noves notes from 'ce' to us
}


CommandEntity *CommandEntity::ApplyPrefix(const char *prefix)
{
    for (auto i=entities.begin(); i!=entities.end(); i++) {
		if (CaseInsensitiveEqual(prefix, "show") || CaseInsensitiveEqual(prefix, "hide")) {
			if ((*i)->show_is_explicit) continue;	
			(*i)->show.first = true;	
			(*i)->show.second = CaseInsensitiveEqual(prefix, "show");
		} else if (CaseInsensitiveEqual(prefix, "activate") || CaseInsensitiveEqual(prefix, "deactivate")) {
            if ((*i)->active_is_explicit) continue;
			(*i)->active.first = true;	
			(*i)->active.second = CaseInsensitiveEqual(prefix, "activate");
            (*i)->active.third = (*i)->file_pos.start;
        }
    }
    return this;
}

void CommandEntity::ApplyShowToChildren(const string &name, bool show)
{
    EIterator j_ent = chart->AllEntities.Find_by_Name(name);
    if ((*j_ent)->children_names.size()==0) {
        for (auto i_def = entities.begin(); i_def != entities.end(); i_def++) 
            if ((*i_def)->name == name) {
                (*i_def)->show.first = true;
                (*i_def)->show.second = show;
                return;
            }
        EntityDef *ed = new EntityDef(name.c_str(), chart);
        ed->AddAttribute(Attribute("show", show, file_line_range(), file_line_range(), NULL));
        entities.Append(ed);
    } else {
        for (auto s = (*j_ent)->children_names.begin(); s != (*j_ent)->children_names.end(); s++) 
            ApplyShowToChildren(*s, show);
    }
}


//Adds and entitydef for "entity" uses running_show and running_style
EntityDef* CommandEntity::FindAddEntityDefForEntity(const string &entity, const file_line_range &l)
{
    //find if already have a def for this
    for (auto i_def = entities.begin(); i_def != entities.end(); i_def++) 
        if ((*i_def)->name == entity) return *i_def;
    const EIterator jj_ent = chart->AllEntities.Find_by_Name(entity);
    _ASSERT(*jj_ent != chart->NoEntity);
    EntityDef *ed = new EntityDef(entity.c_str(), chart);
    ed->itr = jj_ent;
    ed->style = (*jj_ent)->running_style;
    ed->file_pos = l;
    ed->show.first = ed->active.first = false;
    entities.Append(ed);
    return ed;
}

//return active if any of the children are active and ON if any of them are ON
//go recursive all the way deep - ignore the "running_shown" of parents
EEntityStatus CommandEntity::GetCombinedStatus(const std::set<string>& children) const
{
    EEntityStatus ret = EEntityStatus::SHOW_OFF;
    for (auto s = children.begin(); s!=children.end(); s++) {
        auto i = chart->AllEntities.Find_by_Name(*s);
        _ASSERT(*i != chart->NoEntity);
        EEntityStatus es;
        if ((*i)->children_names.size()) es = GetCombinedStatus((*i)->children_names);
        else es = (*i)->running_shown;
        if (es.IsActive()) ret.Activate(true);
        if (es.IsOn()) ret.Show(true);
    }
    return ret;
}

/* The following rules apply.
 * Each entity we can possibly have is represented in chart->AllEntities and is of
 * class Entity. 
 * Each time we name an entity in an entity command, we allocate an EntityDef object.
 * If the "defining" member is true this mention of the entity was used to define the
 * entity. If it is false, this mention of the entity is a subsequent one.
 * An entity can have zero or one EntityDefs with defining=true (zero if the entity
 * was implicitly defined via e.g., an arrow definition).
 * When an entity is mentioned in an entity command any (or all) of the three things
 * can be done: turn it on/off, activate/deactivate it or change the style. An entity
 * that is on can be turned on again, in which case we draw an entity heading of it.
 * Similar, the heading command draws an entity heading for all entities that are currently
 * on. This is emulated by adding EntityDefs for such entities with show set to on.
 *
 * During the PostParse process we keep up-to date the "running_show" and "running_style"
 * members of the entities. (Memmbers of class "Entity".) These are used by arrows, etc.
 * to find out if certain entities are on/off or active, etc.
 * In addition, we store the "running_style" and "running_show" in every EntityDef, as well;
 * and then in the PosPos process, when x and y coordinates are already set, we copy them
 * to Entity::status, so that when we draw entity lines we know what to draw at what 
 * coordinate. The entity headings are drawn based on the EntityDef::style member.
 *
 * The EntityDef::draw_heading member tells if a heading should be drawn for this heading
 * at this point. The "show" and "active" members tell if there was "show" or "active"
 * attributes set (perhaps via show/hide/activate/deactivate commands). If so, the
 * "show_is_explicit"/"active_is_explicit" are set. (This is used to prevent overwriting
 * an explicitly written [show=yes/no] with a "show" command. Thus 'show aaa [show=no]' 
 * will result in show=false. Otherwise any newly defined entitydef
 * has show set to true and active set to false - the default for newly defined entities.

 * The algorithm here is the following.
 *
 * 0. Merge entitydefs corresponding to the same entity. (They are in the same order 
 *    as in the source file, so copying merging "active", "show" and "style" is enough,
 *    since all other attributes can only be used at definition, which can be the first 
 *    one only.)
 * 1. Apply the style elements specified by the user to the entity's "running_style"
 * 2. If a grouped entity is listed with show=yes/no, we copy this attribute to all of its
 *    listed children (unless they also have an explicit show attribute, which overrides that
 *    of the group). We also prepend any unlisted children and copy the attribute.
 *    At the same time, we remove the show attribute from the grouped entities.
 *    This will result in that if a grouped entity is turned on or off all of its 
 *    children (and their children, too) will get turned on or off, as well.
 * 3. Apply all listed entities show and active attributes to "running_show" & "draw_heading"
 * 4. Update the "running_shown" of all parents based on the status of leaf children,
 *    if any change, add a corresponding entitydef (if needed)
 * 5. If we are a heading command, add entities where running_show indicates shown
 * 6. Order the listed entities such that parents come first and their children after
 * 7. Set "draw_heading" of the children of collapsed parents to false
 * 8. For those that draw a heading, update "left" and "right", process label & record max width
 */

//TODO: What if multiple entitydefs are present for the same entity? We should merge them
ArcBase* CommandEntity::PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right, Numbering &,
                                     bool top_level)
{
    if (!valid) return NULL;
    at_top_level = top_level;
    if (full_heading && !top_level)
        chart->Error.Warning(file_pos.start, "The command 'heading' is specified "
                             "inside a parallel block. May display incorrectly.");

    //0. First merge entitydefs of the same entity, so that we have at most one for each entity.
    for (auto i_def = entities.begin(); i_def != entities.end(); /*nope*/) {
        //find first entity of this name
        EntityDef *ed = FindAddEntityDefForEntity((*i_def)->name, file_pos); //second param dummy, we never add here
        if (ed == *i_def) i_def++; 
        else {
            //OK, "ed" is an EntityDef before i_def, combine them.
            _ASSERT(!(*i_def)->defining);
            //show_is_explicit and active_is_explicit makes no role beyond this, so ignore
            ed->Combine(*i_def);
            //Ok, delete i_def
            delete *i_def;
            entities.erase(i_def++);
        }
    }
    
    //1. Then apply the style changes of this command to the running style of the entities
    for (auto i_def = entities.begin(); i_def != entities.end(); i_def++) {
        const EIterator j_ent = chart->AllEntities.Find_by_Name((*i_def)->name);
        (*i_def)->itr = j_ent;
        //Make the style of the entitydef fully specified using the accumulated style info in Entity
        (*j_ent)->running_style += (*i_def)->style;  //(*i)->style is a partial style here specified by the user
    }
    //2. Copy show on/off attribute from grouped entities to their children
    for (auto i_def = entities.begin(); i_def != entities.end(); i_def++) {
        const Entity *ent = *(*i_def)->itr;
        //if the entity is a grouped entity with a show/hide attribute, 
        //add an entitydef to our list for those children, who have no entitidefs
        //yet in the list. For those children, who have, just set show attribute
        //if not set yet
        //new entitydefs are added to the end of the list - and get processed for further 
        //children
        if ((*i_def)->show.first && ent->children_names.size()) 
            for (auto ss = ent->children_names.begin(); ss!=ent->children_names.end(); ss++) 
                FindAddEntityDefForEntity(*ss, (*i_def)->file_pos)->show = (*i_def)->show;
    }
    //3. Decide if we will draw a heading for these entities & update running state
    for (auto i_def = entities.begin(); i_def != entities.end(); i_def++) {
        Entity *ent = *(*i_def)->itr;
        //Decide, if this entitydef will draw a heading or not
        //It can get drawn because we 1) said show=yes, or
        //2) because it is on, we mention it (without show=yes) and it is
        //a full heading.
        (*i_def)->draw_heading = ((*i_def)->show.second && (*i_def)->show.first) 
                                 || (full_heading && ent->running_shown.IsOn());
        //Adjust the running status of the entity, this is valid *after* this command. 
        //This is just for the Height process knwos whch entity is on/off
        if ((*i_def)->show.first)
            ent->running_shown.Show((*i_def)->show.second);
        //Update the style of the entitydef
        (*i_def)->style = ent->running_style;	 //(*i)->style now become the full style to use from this point
        //reflect any "active" attribute in the running_shown variable 
        if ((*i_def)->active.first) 
            ent->running_shown.Activate((*i_def)->active.second);
    }
    //4. Now we are guaranteed to have all leaf children's runnin_shown status right.
    //However, we can have parents, whose child(ren) changed status and we need to update that
    //We will need to add EntityDefs here for such parents (so they update status in PostPos, too)
    for (auto j_ent = chart->AllEntities.begin(); j_ent != chart->AllEntities.end(); j_ent++) {
        if ((*j_ent)->children_names.size()==0) continue;
        EEntityStatus es_new = GetCombinedStatus((*j_ent)->children_names);
        EEntityStatus es_old = (*j_ent)->running_shown;
        if (es_old == es_new) continue;
        //ok, shown status has changed, add/lookup entitydef
        EntityDef *ed = FindAddEntityDefForEntity((*j_ent)->name, this->file_pos);
        if (es_new.IsOn() != es_old.IsOn()) {
            ed->show.first = true;
            ed->show.second = es_new.IsOn();
        }
        if (es_new.IsActive() != es_old.IsActive()) {
            ed->active.first = true;
            ed->active.second = es_new.IsActive();
        }
        (*j_ent)->running_shown = es_new;
    }

    //5. A "heading" command, we have to draw all entities that are on
    //for these we create additional EntityDefs and append them to entities
    //Only do this for children (non-grouped entities)
    if (full_heading)
        for (auto i = chart->AllEntities.begin(); i!=chart->AllEntities.end(); i++) {
            if (!(*i)->running_shown.IsOn()) continue;
            if ((*i)->children_names.size()) continue;
            EntityDef *ed = FindAddEntityDefForEntity((*i)->name, this->file_pos); //use the file_pos of the CommandEntity
            ed->draw_heading = true;
        }

    //6. Order the list (lousy bubblesort)
    //any descendant should come after any of their anscestors, but we can only compare
    //direct parent-child, so we go through the list until we see change
    bool changed;
    do {
        changed = false;
        for (auto i_def = ++entities.begin(); i_def != entities.end(); i_def++) 
            for (auto i_def2 = entities.begin(); i_def2 != i_def; i_def2++) 
                if ((*(*i_def2)->itr)->parent_name == (*i_def)->name) {
                    std::swap(*i_def, *i_def2);
                    changed = true;
                }
    } while (changed);

    //At this point the list contains all entities that 
    //- is mentioned by the user (style or status change)
    //- shall be shown a heading due to heading command
    //- has a parent (ancestor) or children (descendant) in the above mentioned two categories
    //Collapse is not considered yet, we will do it below

    //7. Finally prune the list: do not show those that shall not be displayed due to collapse
    for (auto i_def = entities.begin(); i_def != entities.end(); i_def++) 
        if (chart->FindActiveParentEntity((*i_def)->itr) != (*i_def)->itr) 
            (*i_def)->draw_heading = false;

    //8. At last we have all entities among "entities" that will show here/change status or style
    //Go through them and update left, right and the entities' maxwidth
    //Also, PostParseProcess their notes, too
    for (auto i_def = entities.begin(); i_def != entities.end(); i_def++) {
        (*i_def)->PostParseProcessNotes(canvas, hide, at_top_level);
        if (!(*i_def)->draw_heading) continue;
        const EIterator ei = (*i_def)->itr;
        left =  chart->EntityMinByPos(left,  chart->FindWhoIsShowingInsteadOf(ei, true));
        right = chart->EntityMaxByPos(right, chart->FindWhoIsShowingInsteadOf(ei, false));
        (*i_def)->parsed_label.Set((*ei)->label, canvas, (*ei)->running_style.text);
        double w = (*i_def)->Width();
        if ((*ei)->maxwidth < w) (*(*i_def)->itr)->maxwidth = w;
    }
    //process comments attached to the CommandEntity (from heading commands)
    PostParseProcessNotes(canvas, hide, at_top_level);
    hidden = hide;
    return this;
}

void CommandEntity::MoveNotesToChart()
{
    ArcCommand::MoveNotesToChart();
    for (auto i_def = entities.begin(); i_def != entities.end(); i_def++) 
        (*i_def)->MoveNotesToChart();
}


//Here we have EIterators in entitydefs that point to AllEntities (in contrast to
//all other arcs, where PostParseProcess will convert to iterators in AllActiveEntities)
//If an entity we list here is not collapsed and have children, then it will
//be drawn as containing other entities.
//Since parents are in the beginning of the list, we will go and add distances from the back
//and use the added distances later in the cycle when processing parents
void CommandEntity::Width(MscCanvas &/*canvas*/, EntityDistanceMap &distances)
{
    if (!valid || hidden) return;
    //Add distances for entity heading
    //Start by creating a map in which distances are ordered by index
    std::map<int, pair<double, double>> dist; //map the index of an active entity to spaces left & right
    //in PostParseProcess we created an entitydef for all entities shown here. 
    //"full_heading" not even checked here
    for (auto i = entities.rbegin(); !(i==entities.rend()); i++) {
        //Take entity height into account or draw it if show=on was added
        if (!(*i)->draw_heading) continue;
        if ((*(*i)->itr)->children_names.size() == 0 || (*(*i)->itr)->collapsed) {
            const double halfsize = (*(*i)->itr)->maxwidth/2;
            const unsigned index = (*(*i)->itr)->index;
            dist[index] = pair<double, double>(halfsize, halfsize);
            (*i)->right_ent = (*i)->left_ent = (*i)->itr;
            (*i)->right_offset = (*i)->left_offset = 0;
        } else {
            //grouped entity, which is not collapsed
            //find leftmost and rightmost active entity 
            //and expand us by linewidth and space
            const EIterator j_ent = (*i)->itr;
            double expand = chart->emphVGapInside + (*i)->style.line.LineWidth();
            (*i)->left_ent = chart->FindWhoIsShowingInsteadOf(j_ent, true);
            (*i)->right_ent= chart->FindWhoIsShowingInsteadOf(j_ent, false);
            (*i)->left_offset = dist[(*(*i)->left_ent)->index].first += expand; 
            (*i)->right_offset = dist[(*(*i)->right_ent)->index].second += expand; 
            //If this is a group entity containing only one element, ensure its label fit
            if ((*i)->left_ent == (*i)->right_ent) {
                if (dist[(*(*i)->left_ent)->index].first < (*(*i)->itr)->maxwidth/2) {
                    dist[(*(*i)->left_ent)->index].first = (*(*i)->itr)->maxwidth/2;
                    (*i)->left_offset = (*(*i)->itr)->maxwidth/2;
                }
                if (dist[(*(*i)->right_ent)->index].second < (*(*i)->itr)->maxwidth/2) {
                    dist[(*(*i)->right_ent)->index].second = (*(*i)->itr)->maxwidth/2;
                    (*i)->right_offset = (*(*i)->itr)->maxwidth/2;
                }
            } else {
                //Insert a requirement between left_ent and right_ent, so that our width will fit (e.g., long text)
                distances.Insert((*(*i)->left_ent)->index, (*(*i)->right_ent)->index,
                                 (*(*i)->itr)->maxwidth - (*i)->left_offset - (*i)->right_offset);
            }
        }
    }
    if (dist.size()) {
        //Now convert neighbouring ones to box_side distances, and add the rest as normal side distance
        distances.Insert(dist.begin()->first,  DISTANCE_LEFT, dist.begin()->second.first); //leftmost distance
        distances.Insert(dist.rbegin()->first, DISTANCE_RIGHT,dist.rbegin()->second.second); //rightmost distance
        for (auto d = dist.begin(); d!=--dist.end(); d++) {
            auto d_next = d; d_next++;
            if (d->first == d_next->first-1) //neighbours
                distances.InsertBoxSide(d->first, d->second.second, d_next->second.first);
            else {
                distances.Insert(d->first, DISTANCE_RIGHT, d->second.second);
                distances.Insert(d_next->first, DISTANCE_LEFT, d_next->second.first);
            }
        }
    }
    //Now add some distances for activation (only for non-grouped or collapsed entities)
    for (auto i = entities.begin(); i!=entities.end(); i++) {
        if ((*(*i)->itr)->children_names.size() == 0 || (*(*i)->itr)->collapsed) {
            if ((*i)->show.first) 
                (*(*i)->itr)->running_shown.Show((*i)->show.second);
            if ((*i)->active.first) 
                (*(*i)->itr)->running_shown.Activate((*i)->active.second);
            if ((*(*i)->itr)->running_shown == EEntityStatus::SHOW_ACTIVE_ON) 
                distances.was_activated.insert((*(*i)->itr)->index);
        }
    }
}

double CommandEntity::Height(MscCanvas &canvas, AreaList &cover, bool reflow)
{
    if (!valid || hidden) return height=0;
    if (reflow) {
        cover = cover_at_0;
        return height;
    }
    Range hei(0,0);
    //Those entities explicitly listed will have their own EntityDef for this line.
    //Thus their area will be stored there and not in CommandEntity->area
    //But, still put those into "cover" so they can be considered for placement
    //There are other entities shown here, those triggered by a heading command.
    //They have no line info and they do not add
    //their "area" to the allcovers of the chart in EntityDef::PostPosProcess.
    //Instead we add their area to this->area now

    //We go backwards, so that contained entities get calculated first
    double xpos_all = 0, xpos_showing = 0;
    unsigned num_showing = 0;
    for (auto i = entities.rbegin(); !(i==entities.rend()); i++) {
        const double xpos = chart->XCoord((*(*i)->itr)->pos);
        xpos_all += xpos;
        if (!(*i)->draw_heading) {
            if (!reflow) (*i)->AddNoteMapWhenNotShowing();
            continue;
        }
        //Collect who is my children in this list
        EntityDefList edl(false);
        for (auto ii = entities.rbegin(); !(ii==entities.rend()); ii++) 
            if ((*ii)->draw_heading && *ii != *i && chart->IsMyParentEntity((*ii)->name, (*i)->name))
                edl.Append(*ii);
        //EntityDef::Height places children entities to yPos==0
        //Grouped entities may start at negative yPos.
        //We collect here the maximum extent
        //Note: Height() also adds the cover to the entitydef's area and
        //fills (*i)->note_map and (*i)->def_note_target;
        Area entity_cover;
        hei += (*i)->Height(entity_cover, edl); 
        cover += GetCover4Compress(entity_cover);
        area += (*i)->GetAreaToSearch();
        xpos_showing += xpos;
        num_showing ++;
    }
    if (num_showing)
        def_note_target = XY(0, xpos_showing/num_showing);
    else
        def_note_target = XY(0, xpos_all/entities.size());
    if (!num_showing) 
        return height = 0; //if no headings show
    //Ensure overall startpos is zero
    ShiftBy(-hei.from + chart->headingVGapAbove);
    cover.Shift(XY(0,-hei.from + chart->headingVGapAbove));
    cover_at_0 = cover;
    return height = chart->headingVGapAbove + hei.Spans() + chart->headingVGapBelow;
}

void CommandEntity::ShiftBy(double y)
{
    if (!valid) return;
    for (auto i = entities.begin(); i!=entities.end(); i++)
        (*i)->ShiftBy(y);
    ArcCommand::ShiftBy(y);
}

void CommandEntity::PostPosProcess(MscCanvas &canvas, double autoMarker)
{
    if (!valid) return;
    ArcCommand::PostPosProcess(canvas, autoMarker);
    for (auto i = entities.begin(); i!=entities.end(); i++)
        (*i)->PostPosProcess(canvas, autoMarker);
    if (height>0 && !hidden) {
        if (chart->headingSize == 0) chart->headingSize = yPos + height;
        chart->headingSize = std::min(chart->headingSize, yPos + height);
    }
}

void CommandEntity::Draw(MscCanvas &canvas, DrawPassType pass)
{
    if (!valid) return;
    if (pass!=draw_pass) return;
    for (auto i = entities.begin(); i!=entities.end(); i++)
        if ((*i)->draw_heading)
            (*i)->Draw(canvas);
}

//////////////////////////////////////////////////////////////////////////////////////

bool CommandNewpage::AddAttribute(const Attribute &)
{
    return false;
}

void CommandNewpage::AttributeNames(Csh &)
{

}

bool CommandNewpage::AttributeValues(const std::string, Csh &)
{
    return false;
}

double CommandNewpage::Height(MscCanvas &/*canvas*/, AreaList &, bool reflow)
{
    height = 0;
    if (!valid || reflow) return 0;
    Block b(0, chart->total.x, -chart->nudgeSize/2, chart->nudgeSize/2);
    area_draw = b;
    draw_is_different = true; //area is empty - never find this
    return 0;
}

void CommandNewpage::PostPosProcess(MscCanvas &/*canvas*/, double)
{
    if (!valid) return;
    chart->yPageStart.push_back(yPos);
}

//////////////////////////////////////////////////////////////////////////////////////

double CommandNewBackground::Height(MscCanvas &/*canvas*/, AreaList &, bool reflow)
{
    height = 0;
    if (!valid || reflow) return 0;
    Block b(0, chart->total.x, -chart->nudgeSize/2, chart->nudgeSize/2);
    area_draw = b;
    draw_is_different = true; //area is empty - never find this
    return 0;
}

void CommandNewBackground::PostPosProcess(MscCanvas &/*canvas*/, double)
{
    if (!valid) return;
    chart->Background[yPos] = fill;
}

//////////////////////////////////////////////////////////////////////////////////////
ArcBase* CommandNumbering::PostParseProcess(MscCanvas &/*canvas*/, bool hide, EIterator &/*left*/, EIterator &/*right*/, Numbering &number, bool /*top_level*/)
{
    if (!valid) return NULL;
    if (hide) hidden = true;
    if ((action & SIZE) && length)
        number.SetSize(unsigned(length));
    if (action & INCREMENT)
        ++number;
    if (action & DECREMENT)
        --number;
    return this;
}

//////////////////////////////////////////////////////////////////////////////////////

CommandMark::CommandMark(const char *m, file_line_range ml, Msc *msc) :
    ArcCommand(MSC_COMMAND_MARK, msc), name(m)
{
    map<string, Msc::MarkerType>::iterator i = chart->Markers.find(name);
    if (i != chart->Markers.end()) {
		chart->Error.Error(ml.start, "Marker '"+name+"' has already been defined. Keeping old definition.");
        chart->Error.Error(i->second.first,  ml.start, "Location of previous definition.");
        valid = false;
        return;
    }
    chart->Markers[name].first = ml.start;
    chart->Markers[name].second = -1001;
    offset = 0;
}

bool CommandMark::AddAttribute(const Attribute &a)
{
    if (a.Is("offset")) {
        if (!a.EnsureNotClear(chart->Error, STYLE_ARC)) return true;
        if (!a.CheckType(MSC_ATTR_NUMBER, chart->Error)) return true;
        offset = a.number;
        return true;
    }
    return ArcBase::AddAttribute(a);
}

void CommandMark::AttributeNames(Csh &csh)
{
    ArcBase::AttributeNames(csh);
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME)+"offset", HINT_ATTR_NAME));
}

bool CommandMark::AttributeValues(const std::string attr, Csh &csh)
{
    if (CaseInsensitiveEqual(attr,"offset")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable()+"<number>", HINT_ATTR_VALUE, false));
        return true;
    }
    if (ArcBase::AttributeValues(attr, csh)) return true;
    return false;
}

double CommandMark::Height(MscCanvas &/*canvas*/, AreaList &, bool reflow)
{
    height = 0;
    if (!valid || reflow) return 0;
    Block b(0, chart->total.x, offset-chart->nudgeSize/2, offset+chart->nudgeSize/2);
    area_draw = b;
    draw_is_different = true; //area is empty - never find this
    return 0;
}

void CommandMark::ShiftBy(double y)
{
    if (!valid) return;
    ArcCommand::ShiftBy(y);
    chart->Markers[name].second = yPos+offset;
}

//////////////////////////////////////////////////////////////////////////////////////

#define EMPTY_MARGIN_X 50
#define EMPTY_MARGIN_Y 5

CommandEmpty::CommandEmpty(Msc *msc) : ArcCommand(MSC_COMMAND_EMPTY, msc)
{
}

void CommandEmpty::Width(MscCanvas &canvas, EntityDistanceMap &distances)
{
    if (!valid) return;
    StringFormat format;
    format.Default();
    format.Apply("\\pc\\mu(10)\\md(10)\\ml(10)\\mr(10)\\c(255,255,255)\\b\\i");
    parsed_label.Set(string("\\i\\bEmpty chart"), canvas, format);
    const unsigned lside_index = (*chart->ActiveEntities.Find_by_Name(LSIDE_ENT_STR))->index;
    const unsigned rside_index = (*chart->ActiveEntities.Find_by_Name(RSIDE_ENT_STR))->index;
    const double width = parsed_label.getTextWidthHeight().x + 2*EMPTY_MARGIN_X;
    distances.Insert(lside_index, rside_index, width);
}

double CommandEmpty::Height(MscCanvas &/*canvas*/, AreaList &cover, bool)
{
    if (!valid) return height = 0;
    yPos = 0;
    const XY wh = parsed_label.getTextWidthHeight();
    const Area a(Block((chart->total.x-wh.x)/2, (chart->total.x+wh.x)/2, EMPTY_MARGIN_Y, EMPTY_MARGIN_Y+wh.y), this);
    cover = GetCover4Compress(a);
    return height = wh.y + EMPTY_MARGIN_Y*2;
}

void CommandEmpty::Draw(MscCanvas &canvas, DrawPassType pass)
{
    if (!valid) return;
    if (pass!=draw_pass) return;
    const double width  = parsed_label.getTextWidthHeight().x;
    const double height = parsed_label.getTextWidthHeight().y;
    MscLineAttr line;
    line.width.second = 3;
    line.corner.second = CORNER_ROUND;
    line.radius.second = 10;

    MscFillAttr fill;
    fill.color.second = MscColorType(0,0,128);
    fill.gradient.second = GRADIENT_BUTTON;

    MscShadowAttr shadow;
    shadow.offset.second = 5;
    shadow.blur.second = 5;

    Block b(XY((chart->total.x-width)/2 , yPos+EMPTY_MARGIN_Y),
            XY((chart->total.x+width)/2 , yPos+EMPTY_MARGIN_Y+height));

    canvas.Shadow(b, line, shadow);
    canvas.Fill(b, line, fill);
    canvas.Line(b, line);
}

/////////////////////////////////////////////////////////////////
CommandHSpace::CommandHSpace(Msc*msc, const NamePair*enp) :
    ArcCommand(MSC_COMMAND_HSPACE, msc), format(msc->Contexts.back().text),
    label(false, string()), space(false, 0)
{
    if (enp==NULL) {
        valid=false;
        return;
    }
    if (enp->src.length()) {
        src = chart->FindAllocEntity(enp->src.c_str(), enp->sline);
        sline = enp->sline;
    } else
        src = chart->AllEntities.Find_by_Ptr(chart->LSide);
    if (enp->dst.length()) {
        dst = chart->FindAllocEntity(enp->dst.c_str(), enp->dline);
        dline = enp->dline;
    } else
        dst = chart->AllEntities.Find_by_Ptr(chart->RSide);
    delete enp;
}

bool CommandHSpace::AddAttribute(const Attribute &a)
{
    if (a.Is("label")) {
        if (!a.CheckType(MSC_ATTR_STRING, chart->Error)) return true;
        //MSC_ATTR_CLEAR is OK above with value = ""
        label.first = true;
        label.second = a.value;
        return true;
    }
    if (a.Is("space")) {
        if (!a.CheckType(MSC_ATTR_NUMBER, chart->Error)) return true;
        space.first = true;
        space.second = a.number;
        return true;
    }
    if (format.AddAttribute(a, chart, STYLE_ARC)) return true;
    return ArcCommand::AddAttribute(a);
}

void CommandHSpace::AttributeNames(Csh &csh)
{
    ArcCommand::AttributeNames(csh);
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "label", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "space", HINT_ATTR_NAME));
    StringFormat::AttributeNames(csh);
}

bool CommandHSpace::AttributeValues(const std::string attr, Csh &csh)
{
    if (CaseInsensitiveEqual(attr,"label")) {
        return true;
    }
    if (CaseInsensitiveEqual(attr,"space")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number>", HINT_ATTR_VALUE, false));
        return true;
    }
    if (StringFormat::AttributeValues(attr, csh)) return true;
    if (ArcCommand::AttributeValues(attr, csh)) return true;
    return false;
}

ArcBase* CommandHSpace::PostParseProcess(MscCanvas &/*canvas*/, bool /*hide*/,
    EIterator &/*left*/, EIterator &/*right*/, Numbering &/*number*/, bool /*top_level*/)
{
    if (!valid) return NULL;
    if (!label.first && !space.first) {
        chart->Error.Error(file_pos.start, "You must specify either a numeric space or a lable for the hspace command.");
        return NULL;
    }
    //Give error if user specified groupe entities
    if (chart->ErrorIfEntityGrouped(src, sline.start)) return NULL;
    if (chart->ErrorIfEntityGrouped(dst, sline.start)) return NULL;
    //check if src and dst has disappeared, also make src&dst point to
    //chart->AllActiveEntities
    src = chart->ActiveEntities.Find_by_Ptr(*chart->FindActiveParentEntity(src));
    dst = chart->ActiveEntities.Find_by_Ptr(*chart->FindActiveParentEntity(dst));
    //we keep ourselves even if src/dst has disappeared
    return this; 
}


void CommandHSpace::Width(MscCanvas &canvas, EntityDistanceMap &distances)
{
    if (!valid) return;
    double dist = space.second; //0 if not specified by user
    if (label.second.length())
        dist += Label(label.second, canvas, format).getTextWidthHeight().x;
    if (dist<0)
        chart->Error.Error(file_pos.start, "The horizontal space specified is negative. Ignoring it.");
    else
        distances.Insert((*src)->index, (*dst)->index, dist);
}


//////////////////////////////////////////////////////////////////////////////////

CommandVSpace::CommandVSpace(Msc*msc)  : ArcCommand(MSC_COMMAND_VSPACE, msc),
    format(msc->Contexts.back().text), label(false, string()),
    space(false, 0), compressable(false)
{
}

bool CommandVSpace::AddAttribute(const Attribute &a)
{
    if (a.Is("label")) {
        if (!a.CheckType(MSC_ATTR_STRING, chart->Error)) return true;
        //MSC_ATTR_CLEAR is OK above with value = ""
        label.first = true;
        label.second = a.value;
        return true;
    }
    if (a.Is("space")) {
        if (!a.CheckType(MSC_ATTR_NUMBER, chart->Error)) return true;
        space.first = true;
        space.second = a.number;
        return true;
    }
    if (a.Is("compressable")) {
        if (!a.CheckType(MSC_ATTR_BOOL, chart->Error)) return true;
        compressable = a.yes;
        return true;
    }
    if (format.AddAttribute(a, chart, STYLE_ARC)) return true;
    return ArcCommand::AddAttribute(a);
}

void CommandVSpace::AttributeNames(Csh &csh)
{
    ArcCommand::AttributeNames(csh);
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "label", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "space", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "compressable", HINT_ATTR_NAME));
    StringFormat::AttributeNames(csh);
}

bool CommandVSpace::AttributeValues(const std::string attr, Csh &csh)
{
    if (CaseInsensitiveEqual(attr,"label")) {
        return true;
    }
    if (CaseInsensitiveEqual(attr,"space")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number>", HINT_ATTR_VALUE, false));
        return true;
    }
    if (CaseInsensitiveEqual(attr,"compressable")) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE)+"yes", HINT_ATTR_VALUE, true, CshHintGraphicCallbackForYesNo, CshHintGraphicParam(1)));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRVALUE)+"no", HINT_ATTR_VALUE, true, CshHintGraphicCallbackForYesNo, CshHintGraphicParam(0)));
        return true;
    }
    if (StringFormat::AttributeValues(attr, csh)) return true;
    if (ArcCommand::AttributeValues(attr, csh)) return true;
    return false;
}

ArcBase* CommandVSpace::PostParseProcess(MscCanvas &/*canvas*/, bool /*hide*/,
    EIterator &/*left*/, EIterator &/*right*/, Numbering &/*number*/, bool /*top_level*/)
{
    if (!valid) return NULL;
    if (!label.first && !space.first) {
        chart->Error.Error(file_pos.start, "You must specify either a numeric space or a lable for the vspace command.");
        return NULL;
    }
    return this;
}

double CommandVSpace::Height(MscCanvas &canvas, AreaList &cover, bool reflow)
{
    if (reflow) return ArcCommand::Height(canvas, cover, reflow);
    double dist = space.second;
    if (label.second.length())
        dist += Label(label.second, canvas, format).getTextWidthHeight().y;
    if (dist<0)
        chart->Error.Error(file_pos.start, "The vertical space specified is negative. Ignoring it.");
    if (dist<=0)
        return height = 0;
    if (!compressable) {
        area = Block(0, chart->total.x, 0, dist);
        cover = GetCover4Compress(area);
    }
    return height = dist;
}


//////////////////////////////////////////////////////////////////////////////////

ExtVertXPos::ExtVertXPos(const char *s, const file_line_range &sl, const VertXPos *p) :
VertXPos(*p), side_line(sl)
{
    if (!valid) {
        side = BAD_SIDE;
        return;
    }
    if (CaseInsensitiveEqual(s, "left"))
        side = LEFT;
    else if (CaseInsensitiveEqual(s, "center"))
        side = CENTER;
    else if (CaseInsensitiveEqual(s, "right"))
        side = RIGHT;
    else
        side = BAD_SIDE;
}

ExtVertXPos::ExtVertXPos(const VertXPos *p) :
VertXPos(*p), side(CENTER), side_line()
{
    if (!valid) {
        side = BAD_SIDE;
        return;
    }
}

const double CommandSymbol::ellipsis_space_ratio = 2.0/3.0;


CommandSymbol::CommandSymbol(Msc*msc, const char *symbol, const NamePair *enp,
                const ExtVertXPos *vxpos1, const ExtVertXPos *vxpos2) :
    ArcCommand(MSC_COMMAND_SYMBOL, msc),
    style(chart->Contexts.back().styles["symbol"]),
    hpos1(vxpos1 ? *vxpos1 : ExtVertXPos(*msc)),
    hpos2(vxpos2 ? *vxpos2 : ExtVertXPos(*msc)),
    vpos(enp ? *enp : NamePair(NULL, file_line_range(), NULL, file_line_range())),
    xsize(false, 0), ysize(false, 0), size(MSC_ARROW_SMALL)
{
    if (CaseInsensitiveEqual(symbol, "arc"))
        symbol_type = ARC;
    else if (CaseInsensitiveEqual(symbol, "rectangle"))
        symbol_type = RECTANGLE;
    else if (CaseInsensitiveEqual(symbol, "..."))
        symbol_type = ELLIPSIS;
    else {
        valid = false;
        return;
    }
    if (symbol_type == ELLIPSIS) {
        if (hpos1.side != ExtVertXPos::NONE && hpos2.side != ExtVertXPos::NONE) {
            chart->Error.Error(hpos2.e1line.start, "Symbol '...' can only have one vertical position indicated. Ignoring second one.");
            hpos2.side = ExtVertXPos::NONE;
        }
        if (vpos.dst.length()>0 && vpos.src.length()>0) {
            chart->Error.Error(vpos.dline.start, "Symbol '...' can only have one horizontal position indicated. Ignoring second one.");
            vpos.src.clear();
        }
        style.fill.color.second = style.line.color.second;
    }
}

bool CommandSymbol::AddAttribute(const Attribute &a)
{
    if (a.Is("xsize")) 
        switch (symbol_type) {
        case ELLIPSIS:
            chart->Error.Error(a, false, "Attribute 'xsize' is valid only for arc and rectangle symbols.", "Try using just 'size' instead.");
            return true;
        default:
            if (!a.CheckType(MSC_ATTR_NUMBER, chart->Error)) return true;
            xsize.first = true;
            xsize.second = a.number;
            return true;
        }
    if (a.Is("ysize")) 
        switch (symbol_type) {
        case ELLIPSIS:
            chart->Error.Error(a, false, "Attribute 'ysize' is valid only for arc and rectangle symbols.", "Try using just 'size' instead.");
            return true;
        default:
            if (!a.CheckType(MSC_ATTR_NUMBER, chart->Error)) return true;
            ysize.first = true;
            ysize.second = a.number;
            return true;
        }
    if (a.Is("size")) 
        switch (symbol_type) {
        case ELLIPSIS:
            {
                ArrowHead ah; //use this type to decode 'small', 'normal', etc...
                ah.Empty();
                ah.AddAttribute(a, chart, STYLE_ARC);
                if (ah.size.first) size = ah.size.second;
            }
            return true;
        default:
            chart->Error.Error(a, false, "Attribute 'size' is valid only for '...' symbols.", "Try using 'xsize' and/or 'ysize' instead.");
            return true;
        }
    if (a.Is("draw_time")) {  //We add this even though it is in ArcBase::AddAttribute, but ArcCommand::AddAttribute does not call that
        if (!a.EnsureNotClear(chart->Error, STYLE_ARC)) return true;
        if (a.type == MSC_ATTR_STRING && Convert(a.value, draw_pass)) return true;
        a.InvalidValueError(CandidatesFor(draw_pass), chart->Error);
        return true;
    }
    if (style.AddAttribute(a, chart)) return true;
    return ArcCommand::AddAttribute(a);
}

void CommandSymbol::AttributeNames(Csh &csh)
{
    ArcCommand::AttributeNames(csh);
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "xsize", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "ysize", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "size", HINT_ATTR_NAME));
    Design().styles["symbol"].AttributeNames(csh);
}

bool CommandSymbol::AttributeValues(const std::string attr, Csh &csh)
{
    if (CaseInsensitiveEqual(attr,"xsize")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number>", HINT_ATTR_VALUE, false));
        return true;
    }
    if (CaseInsensitiveEqual(attr,"ysize")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number>", HINT_ATTR_VALUE, false));
        return true;
    }
    if (CaseInsensitiveEqual(attr,"size")) {
        ArrowHead::AttributeValues(attr, csh, ArrowHead::ANY);
        return true;
    }
    if (ArcCommand::AttributeValues(attr, csh)) return true;
    if (Design().styles["symbol"].AttributeValues(attr, csh)) return true;
    return false;
}

ArcBase* CommandSymbol::PostParseProcess(MscCanvas &canvas, bool hide, EIterator &/*left*/, EIterator &/*right*/, Numbering &/*number*/, bool /*top_level*/)
{
    if (!valid) return NULL;
    if (vpos.src.length()) {
        std::map<string, Msc::MarkerType>::const_iterator i = chart->Markers.find(vpos.src);
        if (i == chart->Markers.end()) {
            chart->Error.Error(file_pos.start, "Cannot find marker '" + vpos.src + "'."
                " Ignoring symbol.");
            return NULL;
        }
    }
    if (vpos.dst.length()) {
        std::map<string, Msc::MarkerType>::const_iterator i = chart->Markers.find(vpos.dst);
        if (i == chart->Markers.end()) {
            chart->Error.Error(file_pos.start, "Cannot find marker '" + vpos.dst + "'."
                " Ignoring symbol.");
            return NULL;
        }
    }
    if (hpos1.side == ExtVertXPos::BAD_SIDE) {
        chart->Error.Error(hpos1.side_line.start, "Invalid horizontal position designator. Ignoring symbol.", "Use 'left', 'right' or 'center'.");
        return NULL;
    }
    if (hpos2.side == ExtVertXPos::BAD_SIDE) {
        chart->Error.Error(hpos2.side_line.start, "Invalid horizontal position designator. Ignoring symbol.", "Use 'left', 'right' or 'center'.");
        return NULL;
    }
    if (hpos1.side == ExtVertXPos::NONE) {
        chart->Error.Error(hpos1.side_line.start, "You need to specify at least one horizontal position designator. Ignoring symbol.", "Use 'left at', 'right at' or 'center at'.");
        return NULL;
    }
    if (hpos1.side == hpos2.side) {
        chart->Error.Error(hpos2.side_line.start, "You cannot specify the same horizontal position designator twice. Ignoring symbol.");
        chart->Error.Error(hpos1.side_line.start, hpos2.side_line.start, "Here is the first designator.");
        return NULL;
    }
    if (chart->ErrorIfEntityGrouped(hpos1.entity1, hpos1.e1line.start)) return NULL;
    if (chart->ErrorIfEntityGrouped(hpos1.entity2, hpos1.e2line.start)) return NULL;
    if (chart->ErrorIfEntityGrouped(hpos2.entity1, hpos2.e1line.start)) return NULL;
    if (chart->ErrorIfEntityGrouped(hpos2.entity2, hpos2.e2line.start)) return NULL;
    if (hide) return NULL;

    hpos1.entity1 = chart->ActiveEntities.Find_by_Ptr(*chart->FindActiveParentEntity(hpos1.entity1));
    hpos1.entity2 = chart->ActiveEntities.Find_by_Ptr(*chart->FindActiveParentEntity(hpos1.entity2));
    hpos2.entity1 = chart->ActiveEntities.Find_by_Ptr(*chart->FindActiveParentEntity(hpos2.entity1));
    hpos2.entity2 = chart->ActiveEntities.Find_by_Ptr(*chart->FindActiveParentEntity(hpos2.entity2));

    if (hpos1.side > hpos2.side) std::swap(hpos1, hpos2);
    switch (symbol_type) {
    case ARC:
    case RECTANGLE:
        if (hpos2.side == ExtVertXPos::NONE && xsize.first == false) {
            xsize.first = true;
            xsize.second = 10; //default size;
        }
        if (!(vpos.dst.length() || vpos.src.length()) && ysize.first == false) {
            ysize.first = true;
            ysize.second = 10; //default size;
        }
        break;
    case ELLIPSIS:
        static const double ellipsis_sizes[] = {0, 1.5, 3, 5, 8, 15}; //INVALID, TINY, SMALL, NORMAL, LARGE, HUGE
        xsize.first = ysize.first = true;
        xsize.second = ellipsis_sizes[size];
        ysize.second = xsize.second*(3+2*ellipsis_space_ratio);
    }
    PostParseProcessNotes(canvas, hide, at_top_level);
    return this;
}

void CommandSymbol::Width(MscCanvas &/*canvas*/, EntityDistanceMap &/*distances*/)
{
}

double CommandSymbol::Height(MscCanvas &canvas, AreaList &cover, bool reflow)
{
    if (reflow) {
        if (style.shadow.offset.second)
            cover = area + area.CreateShifted(XY(style.shadow.offset.second, style.shadow.offset.second));
        else
            cover = area;
        return height;
    }

    //Calculate x positions
    const double lw = style.line.LineWidth();
    double x1 = hpos1.CalculatePos(*chart);
    switch (hpos2.side) {
    case ExtVertXPos::NONE:
        switch (hpos1.side) {
        case ExtVertXPos::LEFT:
            outer_edge.x.from = x1;
            outer_edge.x.till = x1 + xsize.second;
            break;
        case ExtVertXPos::RIGHT:
            outer_edge.x.from = x1 - xsize.second;
            outer_edge.x.till = x1;
            break;
        case ExtVertXPos::CENTER:
            outer_edge.x.from = x1 - xsize.second/2;
            outer_edge.x.till = x1 + xsize.second/2;
            break;
        default:
            _ASSERT(0);
            break;
        }
        break;
    case ExtVertXPos::RIGHT:
        outer_edge.x.till = hpos2.CalculatePos(*chart);
        if (hpos1.side == ExtVertXPos::LEFT)
            outer_edge.x.from = x1;
        else //can only be center
            outer_edge.x.from = 2*x1 - outer_edge.x.till;
        break;
    case ExtVertXPos::CENTER:
        //here hpos1 can only be LEFT
        outer_edge.x.from = x1;
        outer_edge.x.till = 2*hpos2.CalculatePos(*chart) - x1;
        break;
    default:
        _ASSERT(0);
        break;
    }
    outer_edge.x.Expand(lw/2);
    //if no markers were specified, we draw it here and assign room
    //else we are done here
    if (vpos.src.length() || vpos.dst.length()) {
        outer_edge.y.MakeInvalid();
        return height = 0;
    }
    outer_edge.y.from = 0;
    outer_edge.y.till = lw + ysize.second;

    CalculateAreaFromOuterEdge();
    note_map = area;
    def_note_target = note_map.Centroid();
    if (!reflow) {
        chart->NoteMapImp.Append(&note_map);
        chart->NoteMapAll.Append(&area);
    }
    if (style.shadow.offset.second)
        cover = area + area.CreateShifted(XY(style.shadow.offset.second, style.shadow.offset.second));
    else
        cover = area;
    return height = outer_edge.y.till + style.shadow.offset.second;
}

void CommandSymbol::ShiftBy(double y)
{
    ArcCommand::ShiftBy(y);
    if (outer_edge.y.IsInvalid()) return;
    outer_edge.y.Shift(y);
}

void CommandSymbol::PostPosProcess(MscCanvas &/*cover*/, double /*autoMarker*/)
{
    if (!outer_edge.y.IsInvalid()) return;
    //We used markers, caculate "area" and "outer_edge.y" now
    if (vpos.src.length())
        outer_edge.y.from = chart->Markers.find(vpos.src)->second.second;
    if (vpos.dst.length())
        outer_edge.y.till = chart->Markers.find(vpos.dst)->second.second;

    if (vpos.src.length()==0)
        outer_edge.y.from = outer_edge.y.till - ysize.second;
    if (vpos.dst.length()==0)
        outer_edge.y.till = outer_edge.y.from + ysize.second;

    if (outer_edge.y.from > outer_edge.y.till)
        std::swap(outer_edge.y.from, outer_edge.y.till);
    else if (outer_edge.y.from == outer_edge.y.till)
        outer_edge.y.Expand(ysize.second/2);

    outer_edge.y.Expand(style.line.LineWidth()/2);

    CalculateAreaFromOuterEdge();
}

void CommandSymbol::CalculateAreaFromOuterEdge()
{
    switch (symbol_type) {
        case ARC:
            area = Contour(outer_edge.Centroid(), outer_edge.x.Spans()/2,
                           outer_edge.y.Spans()/2);
            break;
        case RECTANGLE:
            area = style.line.CreateRectangle_OuterEdge(outer_edge);
            break;
        case ELLIPSIS:
            const double r = outer_edge.x.Spans()/2;
            XY c(outer_edge.x.MidPoint(), outer_edge.y.from + r);
            area = Contour(c, r, r);
            area += Contour(c + XY(0, (2+2*ellipsis_space_ratio)*r), r, r);
            area += Contour(c + XY(0, (4+4*ellipsis_space_ratio)*r), r, r);
            break;
    }
    area.arc = this;
    area.mainline = Block(Range(0, chart->total.x), area.GetBoundingBox().y);
}

void CommandSymbol::Draw(MscCanvas &canvas, DrawPassType pass)
{
    if (pass!=draw_pass) return;
    switch (symbol_type) {
        case ARC:
        case ELLIPSIS:
            canvas.Shadow(area, style.shadow);
            canvas.Fill(area.CreateExpand(-style.line.LineWidth()/2-style.line.Spacing()),
                        style.fill);
            canvas.Line(area.CreateExpand(-style.line.LineWidth()/2), style.line);
            break;
        case RECTANGLE:
            //canvas operations on blocks take the midpoint
            const Block mid = outer_edge.CreateExpand(-style.line.LineWidth()/2);
            canvas.Shadow(mid, style.line, style.shadow);
            canvas.Fill(mid, style.line, style.fill);
            canvas.Line(mid, style.line);
            break;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

/* Note syntax
 ** NOTE left/right/center AT <ent>-<ent>: label [attrs];
 ** NOTE AT <ent>-<ent>: label [attrs];
 **   - center is default
 ** NOTE: label [attrs];
 */


CommandNote::CommandNote(Msc*msc, const ExtVertXPos *evxpos, AttributeList *al)
    : ArcLabelled(MSC_COMMAND_NOTE, msc, msc->Contexts.back().styles["note"]),
    extvertxpos(evxpos?*evxpos:ExtVertXPos(*msc)),
    float_dir_x(0), float_dir_y(0), float_dist(-1)
{
    draw_pass = NOTE;
    AddAttributeList(al);
    target = chart->last_notable_arc;
    if (target == NULL) {
        valid = false;
        chart->Error.Error(file_pos.start, "This note has no prior element to note on. Ignoring it.",
            "Every note must follow a visible element which it makes a remark on. "
            "You cannot start a scope with a note either.");
        return;
    }
    target = target->AttachNote(this);
    if (target == NULL) //Did not succeed in attaching to an existing target. Error has already been issued.
        valid = false;
}

bool CommandNote::AddAttribute(const Attribute &a)
{
    if (a.Is("point_toward")) {
        if (!a.CheckType(MSC_ATTR_STRING, chart->Error)) return true;
        //MSC_ATTR_CLEAR is OK above with value = ""
        //which will mean chart->NoEntity in "point_toward_iterator"
        point_toward_iterator = chart->FindAllocEntity(a.value.c_str(), a.linenum_value);
        return true;
    }
    if (a.Is("ypos")) {
        if (!a.CheckType(MSC_ATTR_STRING, chart->Error)) return true;
        //MSC_ATTR_CLEAR is OK above with value = ""        
        ypos_marker = a.value;
        ypos_marker_linenum = a.linenum_value.start;
        return true;
    }
    return ArcLabelled::AddAttribute(a);
}

void CommandNote::AttributeNames(Csh &csh)
{
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "point_toward", HINT_ATTR_NAME));
    csh.AddToHints(CshHint(csh.HintPrefix(COLOR_ATTRNAME) + "ypos", HINT_ATTR_NAME));
    ArcLabelled::AttributeNames(csh);
}

bool CommandNote::AttributeValues(const std::string attr, Csh &csh)
{
    if (CaseInsensitiveEqual(attr,"point_toward")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<entity>", HINT_ATTR_VALUE, false));
        return true;
    }
    if (CaseInsensitiveEqual(attr,"ypos")) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<marker>", HINT_ATTR_VALUE, false));
        return true;
    }
    return ArcLabelled::AttributeValues(attr, csh);
}

/*Must not use left, right, and number, or TrackableElement::PostParseProcessNotes wont work */
ArcBase* CommandNote::PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right, Numbering &number, bool top_level)
{
    if (!valid) return NULL;
    if (label.length()==0) {
        chart->Error.Error(file_pos.start, "A note must contain text. Ignoring note.", 
            "Try adding a 'label' attribute or text after a colon (':').");
        valid = false;
        return NULL;
    }
    //if user specified 
    if (style.note.layout.second == MscNoteAttr::LEFTRIGHT) {
        style.note.layout.second = chart->last_note_is_on_left ? MscNoteAttr::RIGHT : MscNoteAttr::LEFT;
        chart->last_note_is_on_left ^= true;
    }
    //We do everything here even if we are hidden (numbering is not impacted by hide/show or collapse/expand)
    //Do not call ArcLabelled::PostParseProcess, as we do not increase numbering for notes
    return ArcBase::PostParseProcess(canvas, hide, left, right, number, top_level);
}

void CommandNote::FinalizeLabels(MscCanvas &canvas)
{
    const ArcLabelled *al = dynamic_cast<const ArcLabelled*>(target);
    if (al) {
        numberingStyle = al->numberingStyle;
        number_text = al->number_text;
    }
    ArcLabelled::FinalizeLabels(canvas);
}

void CommandNote::Width(MscCanvas &/*canvas*/, EntityDistanceMap &distances)
{
    //Here we only make space if the note is on the side
    const double w = parsed_label.getTextWidthHeight().x;
    if (style.note.layout.second == MscNoteAttr::LEFT)
        distances.Insert(chart->LNote->index, chart->LSide->index, w);
    else if (style.note.layout.second == MscNoteAttr::RIGHT)
        distances.Insert(chart->RSide->index, chart->RNote->index, w);
}

Contour CommandNote::CoverBody(MscCanvas &canvas) const//places upper left corner to 0,0
{
    return style.line.CreateRectangle_Midline(XY(0,0), parsed_label.getTextWidthHeight());
}

Contour CommandNote::cover_pointer(MscCanvas &canvas, const XY &pointto) const //places upper left corner of the body to 0,0
{
    const double width_min=10, width_max=50, width_div=50;

    const Contour body = CoverBody(canvas);
    if (inside(body.IsWithin(pointto))) return Contour();
    const XY center = body.GetBoundingBox().Centroid();
    const double l = center.Distance(pointto);
    const double startwidth = std::min(width_max, width_min + l/width_div);
    const XY a = (center-pointto).Rotate90CCW()/l*startwidth/2;
    return Contour(pointto, center-a, center+a);
}

void CommandNote::Place(MscCanvas &canvas, const XY &origin, const XY &pointto) 
{
    const Block playground(XY(0,0), chart->total);
    if (!playground.IsWithinBool(origin) || !playground.IsWithinBool(pointto)) {
        _ASSERT(0);
        return;
    }
    pos = origin;
    point_to = pointto;
    area = CoverAll(canvas, pointto-origin).Shift(pos);
}


//double CommandNote::Height(MscCanvas &/*canvas*/, AreaList &/*cover*/, bool /*reflow*/)
//{
//    auto i = chart->Markers.find(ypos_marker);
//    if (i == chart->Markers.end()) {
//        chart->Error.Error(ypos_marker_linenum, "Marker '" + ypos_marker + "' not defined. Ignoring 'ypos' attribute.");
//        ypos_marker.clear();
//        ypos = -1;
//    } else {
//        ypos = i->second.second;
//    }
//    //XXX fix note layout here
//    return height = 0;
//}

void CommandNote::ShiftBy(double y)
{
    ArcLabelled::ShiftBy(y);
    pos.y += y;
    point_to.y += y;
}


//void CommandNote::PostPosProcess(MscCanvas &/*cover*/, double /*autoMarker*/)
//{
//}

void CommandNote::Draw(MscCanvas &canvas, DrawPassType pass)
{
    if (pass!=draw_pass) return;
    const Contour cover = CoverAll(canvas, point_to-pos).Shift(pos);
    canvas.Shadow(cover.CreateExpand(style.line.Spacing()), style.shadow);
    canvas.Fill(cover.CreateExpand(-style.line.Spacing()), style.fill);
    canvas.Line(cover.CreateExpand(style.line.Spacing()), style.line);
    parsed_label.Draw(canvas, pos.x, pos.x+parsed_label.getTextWidthHeight().x, pos.y);
}

