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
#include <algorithm>

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
void CommandEntity::Width(MscCanvas &canvas, EntityDistanceMap &distances)
{
    if (!valid || hidden) return;
    ArcCommand::Width(canvas, distances);
    //Add distances for entity heading
    //Start by creating a map in which distances are ordered by index
    std::map<int, pair<double, double>> dist; //map the index of an active entity to spaces left & right
    //in PostParseProcess we created an entitydef for all entities shown here. 
    //"full_heading" not even checked here
    for (auto i = entities.rbegin(); !(i==entities.rend()); i++) {
        (*i)->TrackableElement::Width(canvas, distances); //process comments to entitydefs
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

double CommandEntity::Height(MscCanvas &/*canvas*/, AreaList &cover, bool reflow)
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
    unsigned num_showing = 0;
    for (auto i = entities.rbegin(); !(i==entities.rend()); i++) {
        const double xpos = chart->XCoord((*(*i)->itr)->pos);
        if (!(*i)->draw_heading) {
            if (!reflow) (*i)->AddAreaImportantWhenNotShowing();
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
        //fills (*i)->area_important 
        Area entity_cover;
        hei += (*i)->Height(entity_cover, edl); 
        cover += GetCover4Compress(entity_cover);
        area += (*i)->GetAreaToSearch();
        area_to_note += (*i)->GetAreaToNote();
        num_showing ++;
    }
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
    ArcCommand::Width(canvas, distances);
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

void CommandSymbol::Width(MscCanvas &canvas, EntityDistanceMap &distances)
{
    ArcCommand::Width(canvas, distances);
}

double CommandSymbol::Height(MscCanvas &/*canvas*/, AreaList &cover, bool reflow)
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
    area_important = area;
    if (!reflow) chart->NoteBlockers.Append(this);
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


CommandNote::CommandNote(Msc*msc, const char *pt, file_line_range ptm, AttributeList *al)
    : ArcLabelled(MSC_COMMAND_NOTE, msc, msc->Contexts.back().styles["note"]),
    point_toward(pt ? pt : ""), point_toward_pos(ptm),
    float_dist(false, 0), float_dir_x(0), float_dir_y(0)
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
    //we handle note.pos here, making it stronger
    if (a.EndsWith("pos")) {
        if (!a.CheckType(MSC_ATTR_STRING, chart->Error)) return true;
        //MSC_ATTR_CLEAR is OK above with value = ""        
        MscNoteAttr::pos_t tmp;
        if (a.value.length()==0) {
            //delete even the default orientation
            float_dir_x = float_dir_y = 0;
            float_dist.first=false;
            style.note.def_float_dist.second = style.note.def_float_x.second = 
                style.note.def_float_y.second = 0; 
        } else if (Convert(a.value, tmp)) {
            switch(tmp) {
            default:
            case MscNoteAttr::POS_INVALID: _ASSERT(0); break;
            case MscNoteAttr::POS_NEAR: float_dist.first=true; float_dist.second=-1; break;
            case MscNoteAttr::POS_FAR:  float_dist.first=true; float_dist.second=+1; break;
            case MscNoteAttr::LEFT:     float_dir_x=-1; break;
            case MscNoteAttr::RIGHT:    float_dir_x=+1; break;
            case MscNoteAttr::UP:       float_dir_y=-1; break;
            case MscNoteAttr::DOWN:     float_dir_y=+1; break;
            }
        } else 
            a.InvalidValueError(CandidatesFor(tmp), chart->Error);
        return true;
    }
    return ArcLabelled::AddAttribute(a);
}

void CommandNote::AttributeNames(Csh &csh)
{
    ArcLabelled::AttributeNames(csh);
}

bool CommandNote::AttributeValues(const std::string attr, Csh &csh)
{
    return ArcLabelled::AttributeValues(attr, csh);
}

/*Must not use left, right, and number, or TrackableElement::PostParseProcessNotes wont work */
ArcBase* CommandNote::PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right, Numbering &number, bool top_level)
{
    if (!valid) return NULL;
    if (label.length()==0) {
        chart->Error.Error(file_pos.start, "A note must contain a label. Ignoring note.", 
            "Try adding a 'label' attribute or text after a colon (':').");
        valid = false;
        return NULL;
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
    halfsize = parsed_label.getTextWidthHeight()/2 + XY(style.line.LineWidth(), style.line.LineWidth());
    //ArcCommand::Width(canvas, distances); We may not have notes
    //Here we only make space if the note is on the side
    const double w = parsed_label.getTextWidthHeight().x;
    if (style.note.layout.second == MscNoteAttr::LEFTSIDE)
        distances.Insert(chart->LNote->index, chart->LSide->index, w);
    else if (style.note.layout.second == MscNoteAttr::RIGHTSIDE)
        distances.Insert(chart->RSide->index, chart->RNote->index, w);
}

Contour CommandNote::CoverBody(MscCanvas &/*canvas*/, const XY &center) const//places upper left corner to 0,0
{
    return style.line.CreateRectangle_Midline(center-halfsize, center+halfsize);
}

const double pointer_width_min=10, pointer_width_max=50, pointer_width_div=50;

double CommandNote::pointer_width(double distance) const
{
    return std::min(pointer_width_max, pointer_width_min + distance/pointer_width_div);
}

Contour CommandNote::cover_pointer(MscCanvas &/*canvas*/, const XY &pointto, const XY &center) const //places upper left corner of the body to 0,0
{
    const double l = center.Distance(pointto);
    if (contour::test_zero(l)) return Contour();
    const double startwidth = pointer_width(l);
    const XY a = (center-pointto).Rotate90CCW()/l*startwidth/2;
    return Contour(pointto, center-a, center+a);
}

//dir_x is +2 at the right edge of the box searching, +1 in the right third
//+ in the middle third, -1 in the left third and -2 along the left edge.
//We return the region to check and also a starting point
Contour CommandNote::GetRegionMask(const Block &outer, int dir_x, int dir_y)
{
    XY A, B;
    const XY third(outer.x.Spans()/3, outer.y.Spans()/3);
    switch (dir_x) {
    default:
        _ASSERT(0);
    case -2: A.x = B.x = outer.x.from; break;
    case -1: A.x = outer.x.from; B.x = A.x + third.x; break;
    case  0: A.x = outer.x.from + third.x; B.x = A.x + third.x; break;
    case +1: B.x = outer.x.till; A.x = B.x - third.x; break;
    case +2: B.x = A.x = outer.x.till;
    }
    switch (dir_y) {
    default:
        _ASSERT(0);
    case -2: A.y = B.y = outer.y.from; break;
    case -1: A.y = outer.y.from; B.y = A.y + third.y; break;
    case  0: A.y = outer.y.from + third.y; B.y = A.y + third.y; break;
    case +1: B.y = outer.y.till; A.y = B.y - third.y; break;
    case +2: B.y = A.y = outer.y.till;
    }

    const XY center = outer.Centroid();
    const XY end = (A+B)/2;
    return Contour(A, B, center);
}

//If the user proscribed an 'at' clause, get the points on the 
//contour of the target, which can be pointer targets.
//return empty vector if any point on the contour can do
std::vector<XY> CommandNote::GetPointerTarget() const
{
    std::vector<XY> ret;
    if (point_toward.length()==0) return ret;
    auto ei = chart->AllEntities.Find_by_Name(point_toward);
    auto mi = chart->Markers.find(point_toward);
    if (ei== chart->AllEntities.end() && mi== chart->Markers.end()) {
        chart->Error.Error(point_toward_pos.start, "'" + point_toward + "' is neither an entity neither a marker. Ignoring it.");
        return ret;
    }
    DoubleMap<bool> section(false);
    if (ei != chart->AllEntities.end()) {
        if (mi != chart->Markers.end()) {
            chart->Error.Warning(point_toward_pos.start, "You have specified both an entity and a marker with the name '" + point_toward + "'. I use the entity here.");
            chart->Error.Warning((*ei)->file_pos, point_toward_pos.start, "Place of the entity definition.");
            chart->Error.Warning(mi->second.first, point_toward_pos.start, "Place of the marker definition.");
        }
        const double x = chart->XCoord((*ei)->pos);
        target->GetAreaToNote().VerticalCrossSection(x, section);
        auto i = section.begin();
        bool b = i->second;
        while (++i!=section.end()) 
            if (!b || !i->second)  //we store true<->false changes and false->false ones, but not true->true ones
                if (i->first!=CONTOUR_INFINITY)
                    ret.push_back(XY(x, i->first));
        if (ret.size()==0) {
            chart->Error.Error(point_toward_pos.start, "Entity '" + point_toward + "' is not above the target of the note. Ignoring the 'at' clause.");
            chart->Error.Error(target->file_pos.start, point_toward_pos.start, "This is the target of the note.");
        }
        return ret;
    } 
    //now we must have a valid marker in "mi"
    const double y = mi->second.second;
    target->GetAreaToNote().CreateSwapXYd().VerticalCrossSection(y, section);
    auto i = section.begin();
    bool b = i->second;
    while (++i!=section.end()) 
        if (!b || !i->second)  //we store true<->false changes and false->false ones, but not true->true ones
            if (i->first!=CONTOUR_INFINITY)
                ret.push_back(XY(i->first, y));
    if (ret.size()==0) {
        chart->Error.Error(point_toward_pos.start, "Marker '" + point_toward + "' is not level with the target of the note. Ignoring the 'at' clause.");
        chart->Error.Error(target->file_pos.start, point_toward_pos.start, "This is the target of the note.");
        chart->Error.Error(mi->second.first, point_toward_pos.start, "Place of the marker definition.");
    }
    return ret;
}

template<typename T> int sign(T a) {return a>0 ? +1 : a==0 ? 0 : -1;}

//This struct holds a score
struct score_t {
    double a, b, c;
    score_t() {}
    score_t(double aa, double bb = 0, double cc = 0) : a(aa), b(bb), c(cc) {}
    score_t & operator +=(const score_t &o) {a+=o.a; b+=o.b; c+=o.c; return *this;}
    score_t operator *(double m) const {return score_t(a*m, b*m, c*m);}
    //bool operator <(const score_t &o) const {return a<o.a? true : a==o.a? b<o.b ? true : b==o.b ? c<o.c : false : false;}
    bool operator <(const score_t &o) const {return a*100+b*10+c<o.a*100+o.b*10+o.c;}
};

//These are added to the region_blocks[].score.a or .b, can be any value
const double dist_award_match = 0;
const double dist_penalty_mul = -10;
const double corner_award_match = +15;
const double corner_award_neighbour_side = +5;
const double corner_penalty_neighbour_corner = -15;
const double corner_penalty_other_side = -20;
const double corner_penalty_opposite = -30;
const double side_award_match = +15;
const double side_award_neighbour = +10;
const double side_penalty_neighbour_side = -5;
const double side_penalty_other_corner = -20;
const double side_penalty_opposite = -30;
//the below penalties are calculated after placement, must be negative
//so that we can detect when the score cannot improve any better and bail out
const score_t penalty_body_cover_something(0, -10);
const score_t penalty_body_cover_something_mul(0, -10); //multiplies a [0..1] value (percent of the comment covering)
const score_t penalty_body_cover_imp(0, -50);
const score_t penalty_body_cover_imp_mul(0, -50); //multiplies a [0..1] value (percent of the comment covering)
const score_t penalty_for_pointer_angle_mul(0, -0.1); //multiplies a [0..90] value (degree)
const score_t penalty_for_pointer_vs_target_angle_mul(0, -0.5); //muls a [0..60] value (angle - 30 degree)
const double worst_value = -MaxVal(worst_value);
const score_t worst_point(worst_value, worst_value, worst_value);
const score_t neutral_point(0,0,0);

double ScoreRegion(const std::pair<bool, int> &wanted_dist,
                   int wanted_x, int wanted_y,
                   int dist, int x, int y)
{

    //if distance is specified and matches, we award +10 points,
    //    if does not match we penalize diff*5 points
    //if two directions specified, we award 15 points to the corner
    //      and 5 points to the neighbouring sides
    //      -15 points to neighbouring corners, -20 to other sides and -30 to opposite corner
    //       CPNC   CAN  CAM
    //        +--  ----  ==+  <-if this corner is selected
    //    CPNC|            || CAM = Corner_Award_Match (see consts above)
    //
    //    CPOS|            | CAN
    //
    //     CPO|            | CPNC
    //        +--  ----  --+
    //        CPO  CPOS  CPNC
    //if only one direction is specified, we award +15 to that side and +10 to neighbouring half-corners
    //      -5 to the other sides, -20 to other corner half sides and -30 to opposite side
    //        SAN   SAM  SAN
    //        +--  ====  --+
    //    SPNS|     ^      | SPNS
    //            if this
    //    SPNS|    side    | SPNS
    //           selected
    //    SPOC|            | SPOC
    //        +--  ----  --+
    //        SPO  SPO   SPO
    double score = 0;
    if (wanted_dist.first) {   //0 = not specified, -1:near, +1 far, -2, +2...
        if (dist == wanted_dist.second)
            score += dist_award_match;
        else
            score +=
                dist_penalty_mul * abs(dist - wanted_dist.second);
    }
    if (wanted_x==0 && wanted_y!=0) {
        if (x == 0 && y == 2*wanted_y)
            score += side_award_match;
        else if (y == 2*wanted_y)
            score += side_award_neighbour;
        else if (y == wanted_y || y==0)
            score += side_penalty_neighbour_side;
        else if (y == -wanted_y)
            score += side_penalty_other_corner;
        else
            score += side_penalty_opposite;
    } else if (wanted_x!=0 && wanted_y==0) {
        if (y == 0 && x == 2*wanted_x)
            score += side_award_match;
        else if (x == 2*wanted_x)
            score += side_award_neighbour;
        else if (x == wanted_x || x==0)
            score += side_penalty_neighbour_side;
        else if (x == -wanted_x)
            score += side_penalty_other_corner;
        else
            score += side_penalty_opposite;
    } else if (wanted_x!=0 && wanted_y!=0) {
        if (sign(wanted_x) == sign(x) &&
            sign(wanted_y) == sign(y))
            score += corner_award_match;
        else if (sign(wanted_x) == sign(x))
            score += y ? corner_penalty_neighbour_corner : corner_award_neighbour_side;
        else if (sign(wanted_y) == sign(y))
            score += x ? corner_penalty_neighbour_corner : corner_award_neighbour_side;
        else if (x == 0 || y == 0)
            score += corner_penalty_other_side;
        else
            score += corner_penalty_opposite;
    }
    return score;
}

void CommandNote::CoverPenalty(const XY &pointto, const XY &center, MscCanvas &canvas,
                               const Contour &block_all, const Contour &block_imp,
                               score_t &cover_penalty) const
{
    const Contour cov = CoverAll(canvas, pointto, center);
    const double cov_area = cov.GetArea();
    const double cov_target_area = (cov * target->GetAreaToNote()).GetArea(); 
    const double cov_sng_ratio = (cov * block_all).GetArea()/ cov_area;
    const double cov_imp_ratio = (cov_target_area + (cov * block_imp).GetArea()) / cov_area;

    if (cov_sng_ratio) {
        cover_penalty += penalty_body_cover_something;
        cover_penalty += penalty_body_cover_something_mul * cov_sng_ratio;
    }
    if (cov_imp_ratio) {
        cover_penalty += penalty_body_cover_imp;
        cover_penalty += penalty_body_cover_imp_mul * cov_imp_ratio;
        //penalize extra if arrow crosses the target itself
        if (cov_target_area>1) {
            cover_penalty += penalty_body_cover_imp;
            cover_penalty += penalty_body_cover_imp_mul * (cov_target_area/cov_area);
        }
    }
    //Penalize if the pointer has a too narrow angle with the side of the target
    XY tmp, tmp2;
    XY on_target = pointto; //assuming it is on the surface
    const contour::is_within_t t = target->GetAreaToNote().Tangents(on_target, tmp, tmp2);
    if (t!=contour::WI_ON_EDGE && t!=contour::WI_ON_VERTEX) {
        target->GetAreaToNote().Distance(pointto, on_target);
        const contour::is_within_t t2 = target->GetAreaToNote().Tangents(on_target, tmp, tmp2);
        _ASSERT(t2==contour::WI_ON_EDGE || t2==contour::WI_ON_VERTEX);
    }
    const double a1 = acos((tmp - pointto).Normalize().DotProduct((center-on_target).Normalize()));
    const double a2 = acos((tmp2- pointto).Normalize().DotProduct((center-on_target).Normalize()));
    const double dev_from_90 = std::min(fabs(M_PI/2-a1), fabs(M_PI/2-a2)) * 180./M_PI;
    if (dev_from_90 >= 30) 
        cover_penalty += penalty_for_pointer_vs_target_angle_mul *
            (dev_from_90 - 30);
}

void CommandNote::PlaceFloating(MscCanvas &canvas)
{
    const unsigned region_distances = 3;
    static const unsigned distances_search_order[region_distances] = {1, 0, 2};
    const static double region_distance_sizes[region_distances+1] = {0, 10, 30, 100};

    //Do a shorter alias
    const unsigned RD = region_distances;
    //Normalize attributes
    if (abs(float_dist.second) > RD/2)
        float_dist.second = float_dist.second>0 ? RD/2 : -int(RD/2);
    float_dir_x = std::max(-1, std::min(+1, float_dir_x));
    float_dir_y = std::max(-1, std::min(+1, float_dir_y));

    //This struct holds the searching pattern.
    //x_pos is +2 at the right edge of the box searching, +1 in the right third
    //+ in the middle third, -1 in the left third and -2 along the left edge.
    //"dist" can be between [0..region_distances)
    struct region_block_t {
        int x;
        int y;
        int dist;
        score_t score;
        int map;
        bool operator <(const region_block_t &o) const {return score>o.score;} //HIGHER score first
    } region_blocks[24*region_distances] = {{+1,-2}, {+2,-1}, {0,-2}, {-1,-2},
                                            {-2,+1}, {+1,+2}, {0,+2}, {+2,+1},
                                            {-2,-1}, {-1,+2}, {-2,0}, {+2, 0}};
    //add tie-breaking scores
    for (unsigned u=0; u<12; u++) {
        region_blocks[u].score.a = 0;
        region_blocks[u].score.b = 0;
        region_blocks[u].score.c = 12-u;
        region_blocks[u].dist = distances_search_order[0];
    }
    //Now we copy the above directional patterns as many times as many distances there are
    for (unsigned u=1; u<region_distances; u++)
        for (unsigned v=0; v<12; v++) {
            region_blocks[u*12+v] = region_blocks[v];
            region_blocks[u*12+v].dist = distances_search_order[u];
            region_blocks[u].score.c += 3*u; //default prefs for distance
        }

    //now we score these according to the note's attributes.
    for (unsigned u=0; u<RD; u++)
        for (unsigned v=0; v<12; v++) {
            region_block_t &RB = region_blocks[u*12+v];
            //score hard user preferences
            RB.score.a += ScoreRegion(float_dist, float_dir_x, float_dir_y,
                                      RB.dist - RD/2, RB.x, RB.y); //distance is normalized back to -1..+1
            //score softer default preferences
            RB.score.c += ScoreRegion(style.note.def_float_dist,
                                      style.note.def_float_x.second,
                                      style.note.def_float_y.second,
                                      RB.dist - RD/2, RB.x, RB.y)*5; //distance is normalized back to -1..+1
        }

    //OK, now sort according to score
    std::sort(region_blocks, region_blocks+RD*24);

    //Now duplicate the whole shabang, and factor in which map to avoid
    //0 means to avoid all screen elements
    //1 means to avoid only the important parts
    for (unsigned u=0; u<RD*12; u++) {
        region_blocks[u+RD*12] = region_blocks[u];
        region_blocks[u].map = 0;
        region_blocks[u+RD*12].map = 1;
    }

    //Create the region belts
    const double default_expand_size = 5;
    Contour region_belts[region_distances];
    const Contour &contour_target = target->GetAreaToNote(); 
    std::vector<XY> target_points = GetPointerTarget(); //call only once!!!
    const Contour target_2D_expanded = contour_target.CreateExpand2D(halfsize);
    Contour prev = target_2D_expanded.CreateExpand(region_distance_sizes[region_distances], EXPAND_MITER, EXPAND_MITER);
    for (int i = region_distances-1; i>=0; i--) {
        Contour next = target_2D_expanded.CreateExpand(region_distance_sizes[i], EXPAND_MITER, EXPAND_MITER);
        region_belts[i] = prev - next;
        prev.swap(next);
    }

    //Create three maps containing places the center of the note body can go.
    //Here we simplisticly assume the body is a box of size 2*halfsize.
    //"map_all" contains a hole for all the drawn elements (expanded) around the
    //  target, plus small rectangles for non-drawn important places
    //  such as entity lines turned off.
    //  (We create an area of interest that is a box around the target including
    //   any place the center of the note can go - we care for blocking elements
    //   only here. AOI is used as a sort of limit to computation)
    //  We consider elements expanded by "halfsize": if the center of the note falls inside
    //  the map - the note will not overlap with the elements in NoteMapAll.
    //"map_imp" contains a hole for the important parts of elements. Each element
    //  specifies this during "Height()" and we expand2d it smae as for above.
    //"map_pointer_all" is inverse: it contains a positive surface for all elements
    //  inside AOI this is used to calculate how much the arrow covers
    //"map_pointer_imp" is also inverse, it just contains the important parts
    //  even the arrow should avoid it.
    const Block total(halfsize, chart->total-halfsize);
    const Block AOI = contour_target.GetBoundingBox().CreateExpand(region_distance_sizes[RD]).Expand2D(halfsize);

    Contour block_all_exp(total), block_imp_exp(total);
    Contour block_all, block_imp;
    for (auto i = chart->NoteBlockers.begin(); i!=chart->NoteBlockers.end(); i++) {
        if (target == *i) continue; //avoid clashes of the target with the note during arrow placement
        if ((*i)->GetAreaToDraw().GetBoundingBox().Overlaps(AOI)) {
            block_all_exp -= (*i)->GetAreaToDraw().CreateExpand2D(halfsize);
            block_all += (*i)->GetAreaToDraw();
        }
        if ((*i)->GetAreaImportant().GetBoundingBox().Overlaps(AOI)) {
            block_imp_exp -= (*i)->GetAreaImportant().CreateExpand2D(halfsize);
            block_imp += (*i)->GetAreaImportant();
        }
    }
    //Take out the target from pointer_map, so that anything inside it
    //does not block us
    //block_all -= contour_target;
    //block_imp += contour_target;
    
    XY best_center;
    XY best_pointto;
    score_t best_point = worst_point;

    //We divide the belt around the target into 12 sections and 3 ("region_distances")
    //belts at ever greater distance (for near/far). We have two such sets one for
    //the important and one for all elements.
    //Process through each such section (or block)
    for (unsigned rno=0; rno<RD*24; rno++) {
        const region_block_t &RB = region_blocks[rno];
        if (RB.score < best_point)
            break; //no chance of getting any better
        const Contour &map = RB.map ? block_imp_exp : block_all_exp;
        //Intersect the belt section with the map: get all the points
        //where the center of the note body can go.
        const Block &outer = region_belts[RD-1].GetBoundingBox();
        const Contour region = (GetRegionMask(outer, RB.x, RB.y) * map) * 
                                region_belts[RB.dist];
        if (region.IsEmpty()) continue;

        //Ok, a note can be placed in this region.
        //Now find a suitable position inside the region and a
        //suitable end for the arrow that lies on the line between
        //"arrowspace" contains all the possible space the arrow can go through
        XY c[2], cc[2];
        if (!region.TangentFrom(contour_target, c, cc)) continue;
        const Contour arrowspace = (region + contour_target +
            Contour(cc[0], cc[1], c[1]) + Contour(c[1], c[0], cc[0])).ClearHoles();

        //calc blocking elements for this region
        const Contour region_block_all = block_all * arrowspace;
        const Contour region_block_imp = block_imp * arrowspace;

        //chart->DebugContours.push_back(Msc::ContourAttr(arrowspace, MscFillAttr(MscColorType(0,0,0, 128))));
        //chart->DebugContours.push_back(Msc::ContourAttr(region, MscFillAttr(MscColorType(255,0,0, 128))));
        //chart->DebugContours.push_back(Msc::ContourAttr(block_imp, MscFillAttr(MscColorType(255,128,255, 128))));

        const XY ideal_center = region.Centroid();
        //Sort target_points so that we process them from the ones closes to the ideal_center
        //(does nothing if "target_points" is empty)
        const struct {
            XY from;
            bool operator ()(const XY &a, const XY &b) {return from.DistanceSqr(a) < from.DistanceSqr(b);}
        } compare = {ideal_center};
        std::sort(target_points.begin(), target_points.end(), compare);

        //From here we try to place both the body inside 'region' and its pointer
        //simultaneously. We go by angle. The arrow of a note should preferably be
        //perpendicular to the defnodetartget line. If that is not possible we attempt
        //ever smaller angles, till we find one. Angles are in radians below.
        const double angle_step_unit = 10/(180/M_PI);
        double angle_step = angle_step_unit;
        //starting angle is the angle specified by the sector of the belt we are in
        //it shall be zero if we want to start with a vertical center->pos arrangement
        double angle = -atan2(double(sign(RB.x)), double(sign(-RB.y)));  //deliberate swap of x & y

        XY center, pointto;
        score_t coverage_penalty = worst_point;
        for (/*nope*/;
            fabs(angle_step)<=M_PI/2;
            angle += angle_step,
            angle_step = -angle_step + ((angle_step > 0) ? -angle_step_unit : angle_step_unit)) {

            //The angle penalty is added if we deviate much from the angle suitable
            //for this region. It is independent of the chart, depends only on "angle"
            //and as we cycle in the angle for cycle it only becomes worse.
            //This is split from other penalties, so that we can stop earlier in this 
            //cycle, when other penalties are better than the angle penalty.
            const score_t angle_penalty = (angle_step>10./180*M_PI) ?
                penalty_for_pointer_angle_mul*(180/M_PI)*fabs(angle_step) :
                neutral_point;
            //if the other penalties are not better than the angle we may not find better
            //scores with later angles (as the angle penalty will get only worse,
            //while the cover penalty cannot get better than neutral)
            if (coverage_penalty > angle_penalty) break;
                
            //We rotate the tangent line and "region" by angle and see if their
            //x coordinates overlap.
            const double sa = sin(angle);
            const double ca = cos(angle);
            const double deg_ang = angle*180/M_PI;
            const XY ideal_center_rot = XY(ideal_center).Rotate(ca, sa);
            const Contour contour_target_rot = contour_target.CreateRotated(deg_ang);
            const Contour region_rot = region.CreateRotated(deg_ang);
            //if (!contour_target_rot.GetBoundingBox().x.Overlaps(region_rot.GetBoundingBox().x)) continue;
            //if there is an overlap, we may find a good placement else go to next angle
            //region_rot or contour_target_rot may not be contiguous
            //Calculate the x ranges of region where we can place the arrow
            DoubleMap<int> region_ranges(0);
            //Add the rotated region's pieces to the region ranges
            for (unsigned ru = 0; ru<region_rot.size(); ru++)
                region_ranges.Set(region_rot[ru].GetBoundingBox().x, 1);
            for (unsigned ctu = 0; ctu<contour_target_rot.size(); ctu++)
                region_ranges.Add(contour_target_rot[ctu].GetBoundingBox().x, 1);
            //OK, we are interested in where the value equals "2" (both cover)
            //Now try to find a place first avoiding all visible blockers,
            //then only the important ones
            //We set success to true if we find a non-blocking "pointto" and "center",
            //but we determine some "pointto" and "center", in all the cases
            bool success = false;  
            score_t local_coverage_penalty = worst_point;
            for (unsigned bl = 0; bl<2; bl++) {
                //rotate the map, which contains the blocking contours
                const Contour blockers_rot = (bl==0 ? region_block_all : region_block_imp).CreateRotated(deg_ang);

                DoubleMap<int> canplacehere(0);
                //On first iteration we copy, else we move
                if (bl==0) canplacehere = region_ranges;
                else canplacehere = std::move(region_ranges);
                //take each independent contour and substract their x extent
                //from the places to consider
                for (unsigned u = 0; u<blockers_rot.size(); u++) 
                    canplacehere.Set(blockers_rot[u].GetBoundingBox().x, 0);
                //Now canplacehere is "2" where we can place a pointer

                //Now if we have a set of points to target, pick one
                //Else select regions
                if (target_points.size()) {
                    for (unsigned tp=0; tp<target_points.size(); tp++) {
                        const XY tp_rot =XY(target_points[tp]).Rotate(ca, sa);
                        //if (*canplacehere.Get(tp_rot.x)!=2) continue;
                        ////Now see if the pointer would cross the target or not
                        //DoubleMap<bool> map(false);
                        //contour_target_rot.VerticalCrossSection(tp_rot.x, map);
                        //double y = std::min(tp_rot.y, ideal_center_rot.y);
                        //const double till = std::max(tp_rot.y, ideal_center_rot.y);
                        //while (y<till) 
                        //    if (*map.Get(y)) break;
                        //    else y = map.Till(y);
                        //if (y<till)    //we found a piece of the target between 
                        //    continue;  //tp_rot and ideal_center_rot: next tp

                        //Now calculate corresponding center
                        DoubleMap<bool>map(false); 
                        region_rot.VerticalCrossSection(tp_rot.x, map);
                        map.Prune();
                        XY cand_center;
                        cand_center.x = tp_rot.x;
                        if (*map.Get(ideal_center_rot.y))
                            cand_center.y = ideal_center_rot.y;
                        else if (ideal_center_rot.y - map.From(ideal_center_rot.y) <
                                    map.Till(ideal_center_rot.y) - ideal_center_rot.y)
                            cand_center.y = map.From(ideal_center_rot.y);
                        else
                            cand_center.y = map.Till(ideal_center_rot.y);
                        if (cand_center.y == CONTOUR_INFINITY || cand_center.y == -CONTOUR_INFINITY)
                            continue; //we skip this point if it is not even on the line with region
                        cand_center.Rotate(ca, -sa); //rotate back
                        cand_center.Round();

                        //Calculate penalty if note body or pointer covers something.
                        score_t very_local_cover_penalty = neutral_point;
                        CoverPenalty(target_points[tp], cand_center, canvas, block_all, block_imp, very_local_cover_penalty);
                        if (very_local_cover_penalty > local_coverage_penalty) {
                            center = cand_center;
                            pointto = target_points[tp];  //the original value, no need to rotate back
                            local_coverage_penalty = very_local_cover_penalty;
                            success |= *canplacehere.Get(tp_rot.x)==2;
                            //if we found a point where there is no overlap, what more can we hope for?
                            if (!(local_coverage_penalty < neutral_point)) break;
                        } 
                    }
                } else { //here we have no points set by an 'at' clause
                    //Now pick the range that is widest
                    Range candidate_range(0,0); //width of 0
                    double pos = -CONTOUR_INFINITY;
                    do {
                        double next_set = canplacehere.Till(pos);
                        while (*canplacehere.Get(next_set)<2 && next_set < CONTOUR_INFINITY)
                            next_set = canplacehere.Till(next_set);
                        if (next_set == CONTOUR_INFINITY) break;
                        pos = canplacehere.Till(next_set);
                        while (*canplacehere.Get(pos)==2 && pos < CONTOUR_INFINITY)
                            pos = canplacehere.Till(pos);
                        if (pos == CONTOUR_INFINITY) break;
                        _ASSERT(*canplacehere.Get(next_set) == 2 && *canplacehere.Get(pos) < 2);
                        if (pos-next_set > candidate_range.Spans()) {
                            candidate_range.from = next_set;
                            candidate_range.till = pos;
                        }
                    } while (pos < CONTOUR_INFINITY);
                    XY pointto_rot;
                    if (candidate_range.Spans()) {
                        //we found a range, where there is no overlap, pick an x from it
                        if (candidate_range.Spans() < pointer_width_max)
                            pointto_rot.x = candidate_range.MidPoint();
                        else {
                            candidate_range.Expand(-pointer_width_max/2);
                            if (ideal_center_rot.x<candidate_range.from)
                                pointto_rot.x = candidate_range.from;
                            else if (ideal_center_rot.x>candidate_range.till)
                                pointto_rot.x = candidate_range.till;
                            else
                                pointto_rot.x = ideal_center_rot.x;
                            success = true;
                        }
                    } else {
                        //there is no position without some overlap, pick a good position
                        //for now we just pick the middle of the target 
                        pointto_rot.x = contour_target_rot.GetBoundingBox().x.MidPoint();
                    }
                    //OK, pointto_rot.x is now set. Find pointto_rot.y to be
                    //on the rotated target_line
                    const Range &y = contour_target_rot.GetBoundingBox().y;
                    const double r = contour_target_rot.Cut(XY(pointto_rot.x, y.from), XY(pointto_rot.x, y.till)).from;
                    pointto_rot.y = y.from + (y.till-y.from)*r;
                    //Now we have a pointto_rot, find our corresponding center_rot
                    //(on the same x coordinate, but somewhere inside region_rot, as close to ideal as we can)
                    DoubleMap<bool> map(false);
                    region_rot.VerticalCrossSection(pointto_rot.x, map);
                    map.Prune();
                    center.x = pointto_rot.x;
                    if (*map.Get(ideal_center_rot.y))
                        center.y = ideal_center_rot.y;
                    else if (ideal_center_rot.y - map.From(ideal_center_rot.y) <
                                map.Till(ideal_center_rot.y) - ideal_center_rot.y)
                        center.y = map.From(ideal_center_rot.y);
                    else
                        center.y = map.Till(ideal_center_rot.y);
                    pointto = pointto_rot.Rotate(ca, -sa);  //destroys pointto_rot
                    //Adjust to surely point to the contour of target
                    if (!target->GetAreaToNote().IsEmpty()) {
                        Range r = target->GetAreaToNote().Cut(pointto, center);
                        if (!r.IsInvalid())
                            pointto = pointto + (center-pointto)*r.till;
                    }
                    center.Rotate(ca, -sa);
                    center.Round();
                    //Calculate penalty if note body or pointer covers something.
                    CoverPenalty(pointto, center, canvas, block_all, block_imp, local_coverage_penalty);
                }
                if (success) break; //we have found a good place, not blocking either of the maps
            } //for: blockers

            //Now we usually have a valid "center" and "pointto" with their 
            //"local_coverage_penalty" calculated. The exception if none of 
            //the target_point[]s is in line with the region at this angle. 
            //In that case local_coverage_penalty == worst_point.
            //Calculate final score
            score_t point = RB.score;
            point += angle_penalty;
            point += local_coverage_penalty;

            if (point > best_point) {
                //OK, if we get better, replace
                best_point = point;
                best_center = center;
                best_pointto = pointto;
                coverage_penalty = local_coverage_penalty;
                //chart->DebugContours.push_back(Msc::ContourAttr(CoverAll(canvas, best_pointto, best_center)));
            }
        }  //for: angle
    } //for: regions
    if (best_point > worst_point) {
        PlaceTo(canvas, best_pointto, best_center);
    } else {
        //Not successful
        chart->Error.Error(file_pos.start, "Could not place this note.");
        valid = false;
    }
}



void CommandNote::PlaceTo(MscCanvas &canvas, const XY &pointto, const XY &center)
{
    const Block playground(XY(0,0), chart->total);
    if (!playground.IsWithinBool(center) || !playground.IsWithinBool(pointto)) {
        _ASSERT(0);
        return;
    }
    pos_center = center;
    point_to = pointto;
    area = CoverAll(canvas, pointto, center);
    area_important = area;
    //instead of Height() we add ourselves to the list here
    chart->NoteBlockers.Append(this);
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
    pos_center.y += y;
    point_to.y += y;
}


//void CommandNote::PostPosProcess(MscCanvas &/*cover*/, double /*autoMarker*/)
//{
//}

void CommandNote::Draw(MscCanvas &canvas, DrawPassType pass)
{
    if (pass!=draw_pass) return;
    const Contour cover = CoverAll(canvas, point_to, pos_center);
    canvas.Shadow(cover, style.shadow);
    canvas.Fill(cover.CreateExpand(-style.line.LineWidth()+style.line.Spacing()), style.fill);
    canvas.Line(cover.CreateExpand(-style.line.LineWidth()/2), style.line);
    const double w2 = halfsize.x - style.line.LineWidth();
    parsed_label.Draw(canvas, pos_center.x-w2, pos_center.x+w2,
        pos_center.y-halfsize.y+style.line.LineWidth());
}

