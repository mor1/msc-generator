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
#include <algorithm>
#include "msc.h"

using namespace std;
using contour::deg2rad;
using contour::rad2deg;

string ArcCommand::Print(int ident) const
{
    string ss;
    ss << string(ident*2, ' ');
    ss << PrintType();
    return ss;
}

//////////////////////////////////////////////////////////////////////////////////////

CommandEntity::CommandEntity(EntityDefHelper *e, Msc *msc, bool in)
    : ArcCommand(MSC_COMMAND_ENTITY, msc), 
    tmp_stored_notes(true), internally_defined(in)
    //tmp_stored_notes is responsible for its content - if the CommandEntity is
    //destroyed during parse, these notes must also get deleted
{
    full_heading = (e==NULL);
    if (e) {
        entities.splice(entities.end(), e->entities);
        tmp_stored_notes.splice(tmp_stored_notes.end(), e->notes);
        tmp_stored_note_targets.splice(tmp_stored_note_targets.end(), e->note_targets);
        target_entity = e->target;
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

void CommandEntity::AttachComment(CommandNote *cn)
{
    _ASSERT(CanBeNoted());
    _ASSERT(entities.size());
    (*entities.rbegin())->AttachComment(cn);
}

//This takes ownership of the note and stores it temporarily.
//We also store what is the name of the entity the note is made to.
//You can store notes before CommandEntity::PostParseProcess.
//These will be removed in Msc::PostParseProcessArcList, and will be
//inserted into the arc list after the commandEntity
void CommandEntity::TmpStoreNote(CommandNote *cn) 
{
    tmp_stored_note_targets.push_back(target_entity);
    tmp_stored_notes.Append(cn);
}

void CommandEntity::ReinsertTmpStoredNotes(ArcList &list, ArcList::iterator after)
{
    _ASSERT(tmp_stored_notes.size() == tmp_stored_note_targets.size());
    _ASSERT(entities.size()); //After PostParseProcess, we must have entities even if a heading command
    after++;  // list::insert insterts _before_ the given iterator
    while (tmp_stored_notes.size()) {
        const string target_name = tmp_stored_note_targets.begin()->length() 
                                      ? *tmp_stored_note_targets.begin() 
                                      : target_entity;
        //here "target_name" can be empty, if entities are virtual
        if (target_name.length()) {
			EIterator ent = chart->AllEntities.Find_by_Name(target_name);
			_ASSERT(*ent != chart->NoEntity);
			EIterator ent_parent = chart->FindActiveParentEntity(ent);
            if (ent_parent == ent) {
                for (auto i=entities.begin(); i!=entities.end(); i++)
                    if (*(*i)->itr == *ent) {
                        //set target of note 
                        (*tmp_stored_notes.begin())->SetTarget(*i);  
                        list.insert(after, *tmp_stored_notes.begin());
                        break;
                    }
            } else {
                //The noted entity is hidden due to a collapsed parent
                //silently drop notes to it
                delete *tmp_stored_notes.begin();
            }
        }
        //OK, we either reinserted or deleted the note, remove from tmp list
        tmp_stored_notes.erase(tmp_stored_notes.begin());
        tmp_stored_note_targets.erase(tmp_stored_note_targets.begin());
    }
}

void CommandEntity::MoveMyContentAfter(EntityDefHelper &e)
{
    e.entities.splice(e.entities.end(), entities);
    e.notes.splice(e.notes.end(), tmp_stored_notes);
    e.note_targets.splice(e.note_targets.end(), tmp_stored_note_targets);
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

//This is called only from "Combine" and Msc::PostParseProcess() 
//(for autogen entities), so definitely
//after AddAttrLits and before PostParseProcess, so here are the members
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
    _ASSERT(ce->internally_defined == internally_defined);
    //Always keep the line_pos of the "heading" command
    //If we are already one, keep ours
    if (!full_heading && ce->full_heading)
        file_pos = ce->file_pos;
    if (ce->full_heading) full_heading = true;
    AppendToEntities(ce->entities);
    ce->entities.clear();
    //move potential notes, as well
    tmp_stored_notes.splice(tmp_stored_notes.end(), ce->tmp_stored_notes);
    tmp_stored_note_targets.splice(tmp_stored_note_targets.end(), ce->tmp_stored_note_targets);
    target_entity = ce->target_entity;
    CombineComments(ce); //noves notes from 'ce' to us
}


CommandEntity *CommandEntity::ApplyPrefix(const char *prefix)
{
    _ASSERT(!internally_defined);
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
ArcBase* CommandEntity::PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right, 
                                         Numbering &, bool top_level, TrackableElement **target)
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
    EntityDef *heading_target=NULL;
    if (full_heading)
        for (auto i = chart->AllEntities.begin(); i!=chart->AllEntities.end(); i++) {
            if (!(*i)->running_shown.IsOn()) continue;
            if ((*i)->children_names.size()) continue;
            unsigned entsize = entities.size();
            EntityDef *ed = FindAddEntityDefForEntity((*i)->name, this->file_pos); //use the file_pos of the CommandEntity
            ed->draw_heading = true;
            if (entsize != entities.size()) 
                heading_target = ed;
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
    //Also, set target to the entity we received in the constructor
    for (auto i_def = entities.begin(); i_def != entities.end(); i_def++) {
        if (target_entity == (*i_def)->name && !internally_defined)
            *target = *i_def;
        if (!(*i_def)->draw_heading) continue;
        const EIterator ei = (*i_def)->itr;
        left =  chart->EntityMinByPos(left,  chart->FindWhoIsShowingInsteadOf(ei, true));
        right = chart->EntityMaxByPos(right, chart->FindWhoIsShowingInsteadOf(ei, false));
        (*i_def)->parsed_label.Set((*ei)->label, canvas, (*ei)->running_style.text);
        double w = (*i_def)->Width();
        if ((*ei)->maxwidth < w) (*(*i_def)->itr)->maxwidth = w;
    }
    if (target_entity.length()==0 && heading_target)
        *target = heading_target;
    hidden = hide;
    return this;
}


//Here we have EIterators in entitydefs that point to AllEntities (in contrast to
//all other arcs, where PostParseProcess will convert to iterators in AllActiveEntities)
//If an entity we list here is not collapsed and have children, then it will
//be drawn as containing other entities.
//Since parents are in the beginning of the list, we will go and add distances from the back
//and use the added distances later in the cycle when processing parents
void CommandEntity::Width(MscCanvas &, EntityDistanceMap &distances)
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

//Here we add to "cover", do not overwrite it
double CommandEntity::NoteHeightHelper(MscCanvas &canvas, AreaList &cover, double &l, double &r)
{
    TrackableElement::NoteHeightHelper(canvas, cover, l, r);
    for (auto i_def = entities.begin(); i_def!=entities.end(); i_def++) 
        (*i_def)->NoteHeightHelper(canvas, cover, l, r);
    return std::max(l, r);
}

double CommandEntity::Height(MscCanvas &canvas, AreaList &cover, bool reflow)
{
    if (!valid || hidden) return height=0;
    if (reflow) {
        cover = cover_at_0;
        return std::max(height, NoteHeight(canvas, cover));
    }
    Range hei(0,0);
    //Those entities explicitly listed will have their own EntityDef for this line.
    //Thus their area will be stored there and not in CommandEntity->area
    //But, still put those into "cover" so they can be considered for placement
    //There are other entities shown here, those triggered by a heading command.
    //They have the line info of the entity command.

    //We go backwards, so that contained entities get calculated first
    unsigned num_showing = 0;
    for (auto i = entities.rbegin(); !(i==entities.rend()); i++) {
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
    _ASSERT(!internally_defined); //internally defined entitydefs should not show a heading
    //Ensure overall startpos is zero
    ShiftBy(-hei.from + chart->headingVGapAbove);
    cover.Shift(XY(0,-hei.from + chart->headingVGapAbove));
    cover_at_0 = cover;
    height = chart->headingVGapAbove + hei.Spans() + chart->headingVGapBelow;
    return std::max(height, NoteHeight(canvas, cover));
}

void CommandEntity::ShiftBy(double y)
{
    if (!valid) return;
    for (auto i = entities.begin(); i!=entities.end(); i++)
        (*i)->ShiftBy(y);
    ArcCommand::ShiftBy(y);
}

void CommandEntity::PostPosProcess(MscCanvas &canvas)
{
    if (!valid) return;
    ArcCommand::PostPosProcess(canvas);
    for (auto i = entities.begin(); i!=entities.end(); i++)
        (*i)->PostPosProcess(canvas);
    if (height>0 && !hidden) {
        if (chart->headingSize == 0) chart->headingSize = yPos + height;
        chart->headingSize = std::min(chart->headingSize, yPos + height);
    }
}

void CommandEntity::Draw(MscCanvas &canvas, DrawPassType pass)
{
    if (!valid) return;
    for (auto i = entities.begin(); i!=entities.end(); i++) {
        if ((*i)->draw_heading && pass==draw_pass)
            (*i)->Draw(canvas);
    }
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
    Block b(chart->GetDrawing().x.from, chart->GetDrawing().x.till, 
            -chart->nudgeSize/2, chart->nudgeSize/2);
    area_draw = b;
    draw_is_different = true; //area is empty - never find this
    return 0;
}

void CommandNewpage::PostPosProcess(MscCanvas &/*canvas*/)
{
    if (!valid) return;
    chart->yPageStart.push_back(yPos);
}

//////////////////////////////////////////////////////////////////////////////////////

double CommandNewBackground::Height(MscCanvas &/*canvas*/, AreaList &, bool reflow)
{
    height = 0;
    if (!valid || reflow) return 0;
    Block b(chart->GetDrawing().x.from, chart->GetDrawing().x.till, 
            -chart->nudgeSize/2, chart->nudgeSize/2);
    area_draw = b;
    draw_is_different = true; //area is empty - never find this
    return 0;
}

void CommandNewBackground::PostPosProcess(MscCanvas &/*canvas*/)
{
    if (!valid) return;
    auto i = chart->Background.find(yPos);
    if (i == chart->Background.end())
        chart->Background[yPos] = fill;
    else 
        i->second += fill;
}

//////////////////////////////////////////////////////////////////////////////////////
ArcBase* CommandNumbering::PostParseProcess(MscCanvas &/*canvas*/, bool hide, EIterator &/*left*/, EIterator &/*right*/, 
                                            Numbering &number, bool /*top_level*/, TrackableElement ** /*target*/)
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
    Block b(chart->GetDrawing().x.from, chart->GetDrawing().x.till, 
            offset-chart->nudgeSize/2, offset+chart->nudgeSize/2);
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

double CommandEmpty::Height(MscCanvas &canvas, AreaList &cover, bool)
{
    if (!valid) return height = 0;
    yPos = 0;
    const XY wh = parsed_label.getTextWidthHeight();
    const double mid = chart->GetDrawing().x.MidPoint();
    const Area a(Block(mid-wh.x/2, mid+wh.x/2, EMPTY_MARGIN_Y, EMPTY_MARGIN_Y+wh.y), this);
    cover = GetCover4Compress(a);
    height = wh.y + EMPTY_MARGIN_Y*2;
    return std::max(height, NoteHeight(canvas, cover));
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

    const double mid = chart->GetDrawing().x.MidPoint();
    const Block b(XY(mid-width/2 , yPos+EMPTY_MARGIN_Y),
                  XY(mid+width/2 , yPos+EMPTY_MARGIN_Y+height));

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
    EIterator &/*left*/, EIterator &/*right*/, 
    Numbering &/*number*/, bool /*top_level*/, TrackableElement ** /*target*/)
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
    EIterator &/*left*/, EIterator &/*right*/, 
    Numbering &/*number*/, bool /*top_level*/, TrackableElement ** /*target*/)
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
        area = Block(chart->GetDrawing().x.from, chart->GetDrawing().x.till, 0, dist);
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
    defaultDesign.styles.GetStyle("symbol").AttributeNames(csh);
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
    if (defaultDesign.styles.GetStyle("symbol").AttributeValues(attr, csh)) return true;
    return false;
}

ArcBase* CommandSymbol::PostParseProcess(MscCanvas &/*canvas*/, bool hide, EIterator &/*left*/, EIterator &/*right*/, 
                                         Numbering &/*number*/, bool /*top_level*/, TrackableElement ** target)
{
    *target = this;
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
    return this;
}

void CommandSymbol::Width(MscCanvas &canvas, EntityDistanceMap &distances)
{
    ArcCommand::Width(canvas, distances);
}


double CommandSymbol::Height(MscCanvas &canvas, AreaList &cover, bool reflow)
{
    if (reflow) {
        if (style.shadow.offset.second)
            cover = area + area.CreateShifted(XY(style.shadow.offset.second, style.shadow.offset.second));
        else
            cover = area;
        return std::max(height, NoteHeight(canvas, cover));
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
    height = outer_edge.y.till + style.shadow.offset.second;
    return std::max(height, NoteHeight(canvas, cover));
}

void CommandSymbol::ShiftBy(double y)
{
    ArcCommand::ShiftBy(y);
    if (outer_edge.y.IsInvalid()) return;
    outer_edge.y.Shift(y);
}

void CommandSymbol::PlaceWithMarkers(MscCanvas &/*cover*/, double /*autoMarker*/)
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
    area.mainline = Block(chart->GetDrawing().x, area.GetBoundingBox().y);
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

CommandNote::CommandNote(Msc*msc, bool is_note, const char *pt, const file_line_range &ptm)
    : ArcLabelled(MSC_COMMAND_NOTE, msc, msc->Contexts.back().styles[is_note ? "note" : "comment"]),
    is_float(is_note), target(NULL), point_toward(pt ? pt : ""), point_toward_pos(ptm),
    float_dist(false, 0), float_dir_x(0), float_dir_y(0)
{
    draw_pass = DRAW_NOTE;
}

CommandNote::~CommandNote()
{
    if (chart)
        chart->RemoveFromNotes(this);
}


bool CommandNote::AddAttribute(const Attribute &a)
{
    //we handle note.pos here, making it stronger
    if (is_float && a.EndsWith("pos")) {
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
            case MscNoteAttr::LEFT:     float_dir_x=-1; float_dir_y=0; break;
            case MscNoteAttr::RIGHT:    float_dir_x=+1; float_dir_y=0; break;
            case MscNoteAttr::UP:       float_dir_x=0; float_dir_y=-1; break;
            case MscNoteAttr::DOWN:     float_dir_x=0; float_dir_y=+1; break;
            case MscNoteAttr::LEFT_UP:    float_dir_x=-1; float_dir_y=-1; break;
            case MscNoteAttr::LEFT_DOWN:  float_dir_x=-1; float_dir_y=+1; break;
            case MscNoteAttr::RIGHT_UP:   float_dir_x=+1; float_dir_y=-1; break;
            case MscNoteAttr::RIGHT_DOWN: float_dir_x=+1; float_dir_y=+1; break;
            }
        } else 
            a.InvalidValueError(CandidatesFor(tmp), chart->Error);
        return true;
    }
    return ArcLabelled::AddAttribute(a);
}

void CommandNote::AttributeNames(Csh &csh, bool is_float)
{
    ArcLabelled::AttributeNames(csh);
    defaultDesign.styles.GetStyle(is_float ? "note" : "comment").AttributeNames(csh);
}

bool CommandNote::AttributeValues(const std::string attr, Csh &csh, bool is_float)
{
    if (defaultDesign.styles.GetStyle(is_float ? "note" : "comment").AttributeValues(attr, csh)) return true;
    return ArcLabelled::AttributeValues(attr, csh);
}

ArcBase* CommandNote::PostParseProcess(MscCanvas &canvas, bool hide, EIterator &left, EIterator &right, 
                                       Numbering &number, bool top_level, TrackableElement **note_target)
{
    if (!valid) return NULL;
    if (label.length()==0) {
        chart->Error.Error(file_pos.start, is_float ? "Notes" : "Comments" +
                           string(" must contain a label. Ignoring note."), 
            "Try adding a 'label' attribute or text after a colon (':').");
        valid = false;
        return NULL;
    }
    //Now try to attach to the target, if not yet attached (as is the case for comments to entities)
    if (target == NULL) {
        target = *note_target;
        if (target == NULL) {
            valid = false;
            chart->Error.Error(file_pos.start, "This note has no prior element to note on. Ignoring it.",
                "Every note must follow a visible element which it makes a remark on.");
            return NULL;
        }
    }
    //We do everything here even if we are hidden (numbering is not impacted by hide/show or collapse/expand)
    //Do not call ArcLabelled::PostParseProcess, as we do not increase numbering for notes
    ArcBase *ret = ArcBase::PostParseProcess(canvas, hide, left, right, number, top_level, note_target);

    if (target == DELETE_NOTE || hide) 
        return NULL; //silently drop note - our target has disappeared, as well
    //OK, now attach the note
    if (is_float) {
        chart->Notes.Append(this);
    } else {
        target->AttachComment(this);
    }
    return ret;
}

void CommandNote::FinalizeLabels(MscCanvas &canvas)
{
    if (!valid) return;
    const ArcLabelled *al = dynamic_cast<const ArcLabelled*>(target);
    if (al) {
        numberingStyle = al->numberingStyle;
        number_text = al->number_text;
        style.numbering.second = al->style.numbering.second && number_text.length(); //skip numbering if target has no number
    } else
        style.numbering.second = false; //if target is not an ArcLabelled (e.g., EntityDef)
    ArcLabelled::FinalizeLabels(canvas);
}

void CommandNote::Width(MscCanvas &/*canvas*/, EntityDistanceMap &distances)
{
    if (!valid) return;
    //ArcCommand::Width(canvas, distances); We may not have notes, do NOT call ancerstor
    if (is_float) {
        halfsize = parsed_label.getTextWidthHeight()/2 + XY(style.line.LineWidth(), style.line.LineWidth());
    } else {
        //Here we only make space if the note is on the side
        const double w = parsed_label.getTextWidthHeight().x;
        if (style.side.second == SIDE_LEFT)
            distances.Insert(chart->LNote->index, DISTANCE_LEFT, w);
        else if (style.side.second == SIDE_RIGHT)
            distances.Insert(chart->RNote->index, DISTANCE_RIGHT, w);
    }
}

double CommandNote::Height(MscCanvas &/*canvas*/, AreaList &/*cover*/, bool reflow) 
{
    if (!valid) return 0;
    if (!is_float && !reflow)  //Only comments, notes will be added after placement
        chart->NoteBlockers.Append(this); 
    return height = 0;
}

Contour CommandNote::CoverBody(MscCanvas &/*canvas*/, const XY &center) const//places upper left corner to 0,0
{
    _ASSERT(is_float);
    return style.line.CreateRectangle_Midline(center-halfsize, center+halfsize);
}

const double pointer_width_min=10, pointer_width_max=50, pointer_width_div=50;

double CommandNote::pointer_width(double distance) const
{
    _ASSERT(is_float);
    switch (style.note.pointer.second) {
    default: _ASSERT(0);
    case MscNoteAttr::NONE:
        return 0;
    case MscNoteAttr::CALLOUT:
        return std::min(pointer_width_max, pointer_width_min + distance/pointer_width_div);
    case MscNoteAttr::BLOCKARROW:
        return style.arrow.getBigWidthHeight(style.arrow.endType.second, style.line).y;
    case MscNoteAttr::ARROW:
        return style.line.LineWidth();
    }
}

Contour CommandNote::cover_pointer(MscCanvas &/*canvas*/, const XY &pointto, const XY &center) const //places upper left corner of the body to 0,0
{
    _ASSERT(is_float);
    const double l = center.Distance(pointto);
    if (contour::test_zero(l)) return Contour();
    const double width = pointer_width(l);
    _ASSERT(style.note.IsComplete());
    switch (style.note.pointer.second) {
    default: _ASSERT(0);
    case MscNoteAttr::NONE: return Contour();
    case MscNoteAttr::CALLOUT: {
        const XY a = (center-pointto).Rotate90CCW()/l*width/2;
        return Contour(pointto, center-a, center+a); }
    case MscNoteAttr::BLOCKARROW:
    case MscNoteAttr::ARROW:
        break;
    }
    //Do an arrow between pointto - XY(l,0) and pointto
    std::vector<double> v(2), a(2);
    v[0] = pointto.x - l; v[1] = pointto.x;
    a[0] = a[1] = 0;
    Contour ret;
    if (style.note.pointer.second == MscNoteAttr::BLOCKARROW) {
        const double size_mul = 3;
        std::vector<Contour> vc;
        v[0] *= size_mul;
        v[1] *= size_mul;
        ret = style.arrow.BigContour(v, a, (pointto.y-width/3)*size_mul, (pointto.y+width/3)*size_mul, true, false, NULL, vc);
        ret.Scale(1/size_mul);
    } else {
        const Contour clip = style.arrow.ClipForLine(pointto, 0, true, false, MSC_ARROW_END,
                                         Block(v[0],v[1], 
                                         chart->GetDrawing().y.from, chart->GetDrawing().y.till),
                                         style.line, style.line);
        ret = style.arrow.Cover(pointto, 0, true, false, MSC_ARROW_END, style.line, style.line);
        ret += Contour(v[0], v[1], pointto.y-width/2, pointto.y+width/2) * clip;
    }
    //Now rotate around pointto so that startpoint is in "center"
    const double deg =rad2deg(atan2(-(center-pointto).y, -(center-pointto).x));
    ret.RotateAround(pointto, deg);
    return ret;
}

//dir_x is +2 at the right edge of the box searching, +1 in the right third
//+ in the middle third, -1 in the left third and -2 along the left edge.
//We return the region to check and also a starting point
Contour CommandNote::GetRegionMask(const Block &outer, const XY &center, int dir_x, int dir_y)
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

    return Contour(A, B, center);
}

//If the user proscribed an 'at' clause, get the points on the
//contour of the target, which can be pointer targets.
//return empty vector if any point on the contour can do
std::vector<std::pair<XY, XY>> CommandNote::GetPointerTarget() const
{
    _ASSERT(is_float);
    std::vector<std::pair<XY, XY>> ret;
    if (point_toward.length()==0) return ret;
    auto ei = chart->AllEntities.Find_by_Name(point_toward);
    auto mi = chart->Markers.find(point_toward);
    if (*ei == chart->NoEntity && mi== chart->Markers.end()) {
        chart->Error.Error(point_toward_pos.start, "'" + point_toward + "' is neither an entity nor a marker. Ignoring it.");
        return ret;
    }
    DoubleMap<bool> section(false);
    if (ei != chart->AllEntities.end()) {
        if (mi != chart->Markers.end()) {
            chart->Error.Warning(point_toward_pos.start, "You have specified both an entity and a marker with the name '" + point_toward + "'. I use the entity here.");
            chart->Error.Warning((*ei)->file_pos, point_toward_pos.start, "Place of the entity definition.");
            chart->Error.Warning(mi->second.first, point_toward_pos.start, "Place of the marker definition.");
        }
        ret.resize(2);
        const double x = chart->XCoord((*ei)->pos);
        const Range y = target->GetAreaToNote().GetBoundingBox().y;
        const Edge e(XY(x, y.from-1), XY(x, y.till+1));
        const Range r = target->GetAreaToNote().CutWithTangent(e, ret[0], ret[1]);
        if (r.IsInvalid()) {
            const Range r2 = target->GetAreaToNote2().CutWithTangent(e, ret[0], ret[1]);
            if (r2.IsInvalid()) {
                chart->Error.Error(point_toward_pos.start, "Entity '" + point_toward + "' is not above the target of the note. Ignoring the 'at' clause.");
                chart->Error.Error(target->file_pos.start, point_toward_pos.start, "This is the target of the note.");
                ret.clear();
            }
        }
        return ret;
    } 
    //now we must have a valid marker in "mi"
    ret.resize(2);
    const double y = mi->second.second;
    const Range x = target->GetAreaToNote().GetBoundingBox().x;
    const Edge e(XY(x.from-1, y), XY(x.till+1, y));
    const Range r = target->GetAreaToNote().CutWithTangent(e, ret[0], ret[1]);
    if (r.IsInvalid()) {
        const Range r2 = target->GetAreaToNote2().CutWithTangent(e, ret[0], ret[1]);
        if (r2.IsInvalid()) {
            chart->Error.Error(point_toward_pos.start, "Marker '" + point_toward + "' is not level with the target of the note. Ignoring the 'at' clause.");
            chart->Error.Error(target->file_pos.start, point_toward_pos.start, "This is the target of the note.");
            chart->Error.Error(mi->second.first, point_toward_pos.start, "Place of the marker definition.");
            ret.clear();
            }
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
    bool operator <(const score_t &o) const {return a<o.a? true : a==o.a? b<o.b ? true : b==o.b ? c<o.c : false : false;}
    //bool operator <(const score_t &o) const {return a*100+b*10+c<o.a*100+o.b*10+o.c;}
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
const score_t penalty_for_outside_drawing(0, -10);
const score_t penalty_for_outside_total(0, -10); //in addition to drawing
const score_t penalty_for_covered_region_mul(0, -20); //multiplied by a [0..1] value (% of region free)
const score_t penalty_body_cover_something(0, -10);
const score_t penalty_body_cover_something_mul(0, -10); //multiplies a [0..1] value (percent of the comment covering)
const score_t penalty_body_cover_imp(0, -50);
const score_t penalty_body_cover_imp_mul(0, -50); //multiplies a [0..1] value (percent of the comment covering)
const score_t penalty_for_pointer_angle_mul(0, -0.07); //multiplies a [0..90] value (degree)
const score_t penalty_for_pointer_vs_target_angle_mul(0, -0.3); //muls a [0..60] value (angle - 30 degree)
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
    _ASSERT(is_float);
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
    switch (cov.GetBoundingBox().RelationTo(chart->GetDrawing())) {
    case contour::REL_A_IN_HOLE_OF_B:
    case contour::REL_A_INSIDE_B:
    case contour::REL_A_IS_EMPTY:
        return; //we are fully inside the drawing area
    default:
        _ASSERT(0);
    case contour::REL_OVERLAP:
    case contour::REL_APART:
        cover_penalty += penalty_for_outside_drawing;
        break;
    }
    switch (cov.GetBoundingBox().RelationTo(chart->GetTotal())) {
    case contour::REL_A_INSIDE_B:
    case contour::REL_A_IS_EMPTY:
        return; //we are fully inside the total surface
    case contour::REL_OVERLAP:
    case contour::REL_APART:
        cover_penalty += penalty_for_outside_total;
        break;
    default: //all else is an error
        _ASSERT(0);
    }
}

//tangent is a fw tangent
void CommandNote::SlantPenalty(const XY &pointto, const XY &center, const XY &tangent,
                               score_t &slant_penalty) const
{
    _ASSERT(is_float);
    const XY meroleges = (tangent-pointto).Rotate90CCW().Normalize();
    const XY irany = (center-pointto).Normalize();
    const double dev_from_90 = rad2deg(acos(std::min(1.0, fabs(irany.DotProduct(meroleges))))); //between 0..90
    _ASSERT(dev_from_90>=0 && dev_from_90<=90);
    if (dev_from_90 >= 30)
        slant_penalty += penalty_for_pointer_vs_target_angle_mul *
            (dev_from_90 - 30);
}

//This returns a point inside c on the p1->p2 line.
//The point should be as inside as possible.
bool CommandNote::GetAPointInside(const Contour &c, const XY &p1, const XY &p2, XY &ret)
{
    if (p1.test_equal(p2)) {
        if (c.IsWithin(p1)==contour::WI_OUTSIDE) return false;
        ret = p1;
        return true;
    }
    DoubleMap<bool>map(false);
    c.Cut(p1, p2, map);
    double r;
    if (!GetAPointInside(map, r)) return false;
    ret = p1 + (p2-p1)*r;
    return true;
}

bool CommandNote::GetAPointInside(const DoubleMap<bool> &map, double &ret)
{
    Range candidate_range(0,0); //width of 0
    double pos = -CONTOUR_INFINITY;
    do {
        double next_set = map.Till(pos);
        while (!*map.Get(next_set) && next_set < CONTOUR_INFINITY)
            next_set = map.Till(next_set);
        if (next_set == CONTOUR_INFINITY) break;
        pos = map.Till(next_set); //we do not cycle until false, we treal two true sections after one another as separate
        if (pos == CONTOUR_INFINITY) break;
        if (pos-next_set > candidate_range.Spans()) {
            candidate_range.from = next_set;
            candidate_range.till = pos;
        }
    } while (pos < CONTOUR_INFINITY);
    if (0==candidate_range.Spans()) return false;
    ret = candidate_range.MidPoint();
    return true;
}

//This is called for notes from Msc::CompleteParse, just before PostPosProcess
void CommandNote::PlaceFloating(MscCanvas &canvas)
{
   if (!valid) return;
    _ASSERT(is_float);
    /* how many belts will we have around target */
    const unsigned region_distances = 3;
    /* if belts starting from 0 (nearest) to region_distances-1, what shall be the 
       default order of search */
    static const unsigned distances_search_order[region_distances] = {1, 0, 2};
    /* What is the pixel distance from the target of the belt margins */
    const static double region_distance_sizes[region_distances+1] = {0, 10, 30, 100};

    //Do a shorter alias
    const unsigned RD = region_distances;
    if (!float_dist.first)
        float_dist = style.note.def_float_dist;
    if (float_dir_x==0 && style.note.def_float_x.first) 
        float_dir_x = style.note.def_float_x.second;
    if (float_dir_y==0 && style.note.def_float_y.first) 
        float_dir_y = style.note.def_float_y.second;

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
            ////score softer default preferences
            //RB.score.b += ScoreRegion(style.note.def_float_dist,
            //                          style.note.def_float_x.second,
            //                          style.note.def_float_y.second,
            //                          RB.dist - RD/2, RB.x, RB.y)/2;
        }

    //Now duplicate the whole shabang, and factor in which map to avoid
    //0 means to avoid all screen elements
    //1 means to avoid only the important parts
    for (unsigned u=0; u<RD*12; u++) {
        region_blocks[u+RD*12] = region_blocks[u];
        region_blocks[u].map = 0;
        region_blocks[u+RD*12].map = 1;
        region_blocks[u+RD*12].score += penalty_body_cover_something;
    }

    //OK, now sort according to score
    std::sort(region_blocks, region_blocks+RD*24);

    //Create the region belts
    const Block total = chart->GetTotal().CreateExpand2D(halfsize);
    const XY note_gap(chart->compressGap, chart->compressGap);
    Contour region_belts[region_distances];
    const Contour &contour_target = target->GetAreaToNote();
    if (contour_target.IsEmpty()) {
        chart->Error.Warning(file_pos.start, "The target of this note has no shape, I cannot point the note to anything. Ignoring note.");
        chart->Error.Warning(target->file_pos.start, file_pos.start, "This is the target of the note.");
    }
    const XY target_centroid = contour_target.Centroid();
    const std::vector<std::pair<XY,XY>> target_points = GetPointerTarget(); //call only once, as it emits errors
    //Region belts are created via Expand2D to avoid spikes
    Contour prev = contour_target.CreateExpand2D(XY(region_distance_sizes[region_distances], region_distance_sizes[region_distances])+halfsize+note_gap);
    _ASSERT(!prev.IsEmpty());
    for (int i = region_distances-1; i>=0; i--) {
        Contour next = contour_target.CreateExpand2D(XY(region_distance_sizes[i], region_distance_sizes[i])+halfsize+note_gap);
        _ASSERT(!next.IsEmpty());
        region_belts[i] = prev - next;
        _ASSERT(!region_belts[i].IsEmpty());
        region_belts[i] *= total;  //limit to chart space 
        _ASSERT(!region_belts[i].IsEmpty());
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
    const Block AOI = contour_target.GetBoundingBox().CreateExpand(region_distance_sizes[RD]).Expand2D(halfsize);

    Contour block_all_exp(total), block_imp_exp(total);
    Contour block_all, block_imp;
    for (auto i = chart->NoteBlockers.rbegin(); !(i==chart->NoteBlockers.rend()); i++) {
        if (target == *i) continue; //avoid clashes of the target with the note during arrow placement
        if ((*i)->GetAreaToDraw().GetBoundingBox().Overlaps(AOI)) {
            block_all_exp -= (*i)->GetAreaToDraw().CreateExpand2D(halfsize+note_gap);
            block_all += (*i)->GetAreaToDraw();
        }
        if ((*i)->GetAreaImportant().GetBoundingBox().Overlaps(AOI)) {
            block_imp_exp -= (*i)->GetAreaImportant().CreateExpand2D(halfsize+note_gap);
            block_imp += (*i)->GetAreaImportant();
        }
    }
    //Take out the target from pointer_map, so that anything inside it
    //does not block us
    //block_all -= contour_target;
    //block_imp += contour_target;
    //chart->DebugContours.push_back(Msc::ContourAttr(block_all, MscFillAttr(MscColorType(255,128,255, 128))));
    //chart->DebugContours.push_back(Msc::ContourAttr(block_imp, MscFillAttr(MscColorType(255,255,128, 128))));

    XY best_center;
    XY best_pointto;
    score_t best_point = worst_point;
    score_t penalty = worst_point;

    Contour contour_target_rot; //Preallocate - rotation does not change number of elements

    //We divide the belt around the target into 12 sections and 3 ("region_distances")
    //belts at ever greater distance (for near/far). We have two such sets one for
    //the important and one for all elements.
    //Process through each such section (or block)
    for (unsigned rno=0; rno<RD*24; rno++) {
        using namespace std::rel_ops;
        const region_block_t &RB = region_blocks[rno];
        if (RB.score <= best_point)
            break; //no chance of getting any better
        const Contour &map = RB.map ? block_imp_exp : block_all_exp;
        //Intersect the belt section with the map: get all the points
        //where the center of the note body can go.
        const Block &outer = region_belts[RD-1].GetBoundingBox();
        const Contour region_mask = GetRegionMask(outer, target_centroid, RB.x, RB.y) * region_belts[RB.dist];
        const Contour region = region_mask * map;
        if (region.IsEmpty()) continue; 
        Contour region_rot; //Preallocate - rotation does not change number of elements
        
        //chart->DebugContours.push_back(Msc::ContourAttr(map, MscFillAttr(MscColorType(255,255,0, 50))));
        //chart->DebugContours.push_back(Msc::ContourAttr(region_mask, MscLineAttr(), MscFillAttr(MscColorType(0,0,255, 128))));

        const score_t region_penalty = penalty_for_covered_region_mul * 
            (1 - region.GetArea()/region_mask.GetArea());

        //Ok, a note can be placed in this region.
        //Now find a suitable position inside the region and a
        //suitable end for the arrow that lies on the line between

        //We try to place both the body inside 'region' and its pointer
        //simultaneously. We will go by angle. The arrow of a note should preferably be
        //perpendicular to the defnodetartget line. If that is not possible we attempt
        //ever smaller angles, till we find one. Angles are in radians below.
        const double angle_step_unit = deg2rad(10);
        //starting angle is the angle specified by the sector of the belt we are in
        //it shall be zero if we want to start with a vertical down center->pos arrangement
        const double start_angle = -atan2(double(sign(RB.x)), double(sign(-RB.y)));  //deliberate swap of x & y

        //Do it differently if we have target points
        if (target_points.size()) {
            for (unsigned tpu=0; tpu<target_points.size(); tpu++) {
                const XY tp = target_points[tpu].first;
                const XY tangent = target_points[tpu].second;
                XY c, cc;
                if (!region.TangentFrom(tp, c, cc)) continue;
                const Contour arrowspace = region + Contour(tp, c, cc);
                //if (Contour::IsResultOverlapping(arrowspace.RelationTo(contour_target, true)))
                //    continue;
                //chart->DebugContours.push_back(Msc::ContourAttr(arrowspace, MscFillAttr(MscColorType(0,0,0, 128))));
                //chart->DebugContours.push_back(Msc::ContourAttr(region, MscFillAttr(MscColorType(255,0,0, 128))));

                for (double angle_step = angle_step_unit, 
                            angle = start_angle;
                    fabs(angle_step)<=M_PI/2;
                    angle += angle_step,
                    angle_step = -angle_step + ((angle_step > 0) ? -angle_step_unit : angle_step_unit)) {

                    //Calculate the penalty of the angle (in relation to the region)
                    score_t local_penalty = region_penalty;
                    local_penalty += (angle_step>deg2rad(10)) ?
                         penalty_for_pointer_angle_mul*rad2deg(fabs(angle_step)) :
                         neutral_point;
                    if (local_penalty < penalty) break; //we cannot get any better

                    XY center;
                    if (!GetAPointInside(region, tp, tp+XY(0,-100).Rotate(cos(-angle), sin(-angle)), center))
                        continue;

                    SlantPenalty(tp, center, tangent, local_penalty);
                    if (local_penalty < penalty) continue; //futile, try next point

                    //calc blocking elements for this region
                    const Contour region_block_all = block_all * arrowspace;
                    const Contour region_block_imp = block_imp * arrowspace;

                    CoverPenalty(tp, center, canvas, block_all, block_imp, local_penalty);
                    if (local_penalty < penalty) continue; //futile, try next point

                    //Calculate final score
                    score_t point = RB.score;
                    point += local_penalty;
                    if (point > best_point) {
                        //OK, if we get better, replace
                        best_point = point;
                        best_center = center;
                        best_pointto = tp;
                        penalty = local_penalty;
                        //chart->DebugContours.push_back(Msc::ContourAttr(CoverAll(canvas, best_pointto, best_center)));
                    }
                    if (!(penalty < neutral_point)) break; //we have what we want
                }//for: cylce angles
                if (!(penalty < neutral_point)) break; //we have what we want
            }//for: cylce through tp's
        } else { //no target points
            //"arrowspace" contains all the possible space the arrow can go through
            XY c[2], cc[2];
            if (!region.TangentFrom(contour_target, c, cc)) continue;
            const Contour arrowspace = (region + contour_target + Contour(cc[0], cc[1], c[1]) + Contour(c[1], c[0], cc[0])).ClearHoles(); 
            Contour arrowspace_rot; //Preallocate - rotation does not change number of elements

            //calc blocking elements for this region
            const Contour region_block_all = block_all * arrowspace;
            const Contour region_block_imp = block_imp * arrowspace;

            //chart->DebugContours.push_back(Msc::ContourAttr(arrowspace, MscFillAttr(MscColorType(0,0,0, 128))));
            //chart->DebugContours.push_back(Msc::ContourAttr(region, MscFillAttr(MscColorType(255,0,0, 128))));
            //chart->DebugContours.push_back(Msc::ContourAttr(block_imp, MscFillAttr(MscColorType(255,128,255, 128))));

            //From here we try to place both the body inside 'region' and its pointer
            //simultaneously. We go by angle. The arrow of a note should preferably be
            //perpendicular to the defnodetartget line. If that is not possible we attempt
            //ever smaller angles, till we find one. Angles are in radians below.
            for (double angle_step = angle_step_unit, 
                        angle = start_angle;
                fabs(angle_step)<=M_PI/2;
                angle += angle_step,
                angle_step = -angle_step + ((angle_step > 0) ? -angle_step_unit : angle_step_unit)) {

                //The angle penalty is added if we deviate much from the angle suitable
                //for this region. It is independent of the chart, depends only on "angle"
                //and as we cycle in the angle for cycle it only becomes worse.
                //This is split from other penalties, so that we can stop earlier in this
                //cycle, when other penalties are better than the angle penalty.
                score_t local_penalty = region_penalty;
                local_penalty += (angle_step>deg2rad(10)) ?
                    penalty_for_pointer_angle_mul*rad2deg(fabs(angle_step)) :
                    neutral_point;

                if (local_penalty <= penalty) break; //we cannot get any better

                XY pointto, tangent, center;

                const double sa = sin(angle);
                const double ca = cos(angle);
                const double deg_ang = rad2deg(angle);
                //We rotate the tangent line and "region" by angle and see if their
                //x coordinates overlap.
                contour_target_rot = contour_target; //Normally involves no memory re-alloc
                contour_target_rot.Rotate(deg_ang);  //in-place rotate
                region_rot = region;                 //Normally involves no memory re-alloc since previous for cycle
                region_rot.Rotate(deg_ang);
                //if (!contour_target_rot.GetBoundingBox().x.Overlaps(region_rot.GetBoundingBox().x)) continue;
                //if there is an overlap, we may find a good placement else go to next angle
                //region_rot or contour_target_rot may not be contiguous
                //Calculate the x ranges of region where we can place the arrow
                DoubleMap<int> region_ranges(0);
                //Add the rotated region's pieces to the region ranges
                for (unsigned ru = 0; ru<region_rot.size(); ru++)
                    region_ranges.Set(region_rot[ru].GetBoundingBox().x, 10000);
                //Warning contour_target_rot may have multiple overlapping ranges!
                for (unsigned ctu = 0; ctu<contour_target_rot.size(); ctu++)
                    region_ranges.Add(contour_target_rot[ctu].GetBoundingBox().x, 1);
                //OK, we are interested in where the value is greater than "10001" (both cover)
                DoubleMap<bool> region_ranges_bool(false);
                bool was = false, running=false;
                for (auto i = ++region_ranges.begin(); i!=region_ranges.end(); i++)
                    if ((i->second>=10001) != running) {
                        running = i->second >= 10001;
                        region_ranges_bool.Set(i->first, running);
                        was |= running;
                    }
                if (!was)    //rotated region and contour has no overlapping x-axis range,
                    continue; //we will never find a match with this angle

                //Now try to find a place first avoiding all visible blockers,
                //then only the important ones
                //We set success to true if we find a non-blocking "pointto" and "center",
                //but we determine some "pointto" and "center", in all the cases
                bool success = false;
                for (unsigned bl = 0; bl<2; bl++) {
                    DoubleMap<bool> canplacehere(region_ranges_bool);
                    //rotate the map, which contains the blocking contours
                    const Contour blockers_rot = (bl==0 ? region_block_all : region_block_imp).CreateRotated(deg_ang);
                    //take each independent contour and substract their x extent
                    //from the places to consider
                    for (unsigned u = 0; u<blockers_rot.size(); u++)
                        canplacehere.Set(blockers_rot[u].GetBoundingBox().x, false);
                    //Now canplacehere is true where we can place a pointer
                    canplacehere.Prune(); //merge neighbouring sections
                    XY pointto_rot;
                    if (GetAPointInside(canplacehere, pointto_rot.x))
                        success = true; //we found a range, where there is no overlap
                    else {
                        //there is no position without some overlap, pick a good position
                        //without considering blockers
                        if (!GetAPointInside(region_ranges_bool, pointto_rot.x)) {
                            _ASSERT(0);
                            continue;
                        }
                    }
                    //now find center
                    if (!GetAPointInside(region_rot, XY(pointto_rot.x,0), XY (pointto_rot.x,100), center)) {
                        _ASSERT(0);
                        continue;
                    }
                    //finally, calculate pointto_rot.y. make a cut in "region_rot" and
                    //select the point closer to "center"
                    std::pair<XY,XY> fromtill[2];
                    const Range r = contour_target_rot.CutWithTangent(center, center+XY(0,100), fromtill[0], fromtill[1]);
                    _ASSERT(!r.IsInvalid());
                    _ASSERT(contour::test_equal(fromtill[0].first.x, pointto_rot.x));
                    _ASSERT(contour::test_equal(fromtill[1].first.x, pointto_rot.x));
                    const unsigned index = fabs(r.from)>fabs(r.till); //the point closer to "center"
                    pointto_rot.y = fromtill[index].first.y;
                    //Now we have a pointto_rot, find our corresponding center_rot
                    //(on the same x coordinate, but somewhere inside region_rot, as close to ideal as we can)
                    //rotate back
                    pointto = pointto_rot.Rotate(ca, -sa);
                    tangent = fromtill[index].second.Rotate(ca, -sa);
                    center.Rotate(ca, -sa);
                    center.Round(); //so that we avoid roinding mismatches
                    if (success) break; //we have found a good place, not blocking either of the maps
                } //for: blockers
                //Now we have a valid "center" and "pointto" with their
                //"local_coverage_penalty" calculated.
                //Calculate penalty if note body or pointer covers something.
                //chart->DebugContours.push_back(Msc::ContourAttr(Contour(pointto,5), MscLineAttr(), MscFillAttr(MscColorType(255,0,0, 50))));
                SlantPenalty(pointto, center, tangent, local_penalty);
                if (local_penalty < penalty) continue; //futile, try next angle
                CoverPenalty(pointto, center, canvas, block_all, block_imp, local_penalty);
                if (local_penalty < penalty) continue; //futile, try next angle

                //Calculate final score
                score_t point = RB.score;
                point += local_penalty;
                if (point > best_point) {
                    //OK, if we get better, replace
                    best_point = point;
                    best_center = center;
                    best_pointto = pointto;
                    penalty = local_penalty;
                    //chart->DebugContours.push_back(Msc::ContourAttr(CoverAll(canvas, best_pointto, best_center)));
                }
            }  //for: angle
        }//else: we had no target points
    } //for: regions
    if (best_point <= worst_point) {
        //we did not succeed. Pick any point from the first region
        const region_block_t &RB = region_blocks[0];
        const Block &outer = region_belts[RD-1].GetBoundingBox();
        const Contour region_mask = GetRegionMask(outer, target_centroid, RB.x, RB.y) * region_belts[RB.dist];
        _ASSERT(!region_mask.IsEmpty());
        if (target_points.size()) {
            best_center = region_mask.Centroid();
            if (best_center.Distance(target_points[0].first) < best_center.Distance(target_points[0].second))
                best_pointto = target_points[0].first;
            else
                best_pointto = target_points[0].second;
        } else {
            //if region is empty we just do it to get "any" placement
            //pick center of the (covered) region and get a point
            best_center = region_mask.Centroid();
            const XY c = contour_target.Centroid();
            std::pair<XY, XY> from, till;
            if (contour_target.CutWithTangent(c, best_center, from, till).IsInvalid()) {
                _ASSERT(0);
            }
            best_pointto = till.first;
        }
        //Not successful
        //chart->Error.Error(file_pos.start, "Could not place this note.");
        //valid = false;
        //return;
    }
    pos_center = best_center;
    point_to = best_pointto;
    //now a hack: decrease halfsize by half a linewidth, from now on CoverAll
    //is assumed to return the midline.
    halfsize.x -= style.line.LineWidth()/2;
    halfsize.y -= style.line.LineWidth()/2;
    area = CoverAll(canvas, best_pointto, best_center);
    area_important = area;
    chart->NoteBlockers.Append(this);
}

//return height, but place to "y" (below other notes)
//This is called from Height() of the target for comments
void CommandNote::PlaceSideTo(MscCanvas &, AreaList &cover, double &y)
{
   if (!valid) return;
    _ASSERT(!is_float);
    yPos = y;
    if (style.side.second == SIDE_LEFT)
        area = parsed_label.Cover(chart->sideNoteGap, 
                                  chart->XCoord(chart->LNote->pos)-chart->sideNoteGap, 
                                  yPos);
    else 
        area = parsed_label.Cover(chart->XCoord(chart->RNote->pos) + chart->sideNoteGap, 
                                  chart->comments_right_side-chart->sideNoteGap, 
                                  yPos);
    area.arc = this;
    height = area.GetBoundingBox().y.till - y + chart->arcVGapBelow;
    y += height;
    cover += GetCover4Compress(area);
}

void CommandNote::ShiftCommentBy(double y)
{
    _ASSERT(!is_float);
    ArcLabelled::ShiftBy(y);
    //if (is_float) {
    //    pos_center.y += y;
    //    point_to.y += y;
    //}
}


void CommandNote::Draw(MscCanvas &canvas, DrawPassType pass)
{
    if (!valid || pass!=draw_pass) return;
    if (is_float) {
        Contour cover;
        if (style.note.pointer.second == MscNoteAttr::ARROW) {
            cover = CoverBody(canvas, pos_center);
            const Range r = cover.CreateExpand(style.line.Spacing()).Cut(point_to, pos_center);
            const double len = r.from * point_to.Distance(pos_center);
            style.arrow.TransformCanvasForAngle(rad2deg(atan2(-(pos_center-point_to).y, -(pos_center-point_to).x)), 
                                                      canvas, point_to.x, point_to.y);
            std::vector<double> v(2), a(2);
            v[0] = point_to.x - len; v[1] = point_to.x;
            a[0] = a[1] = 0;
            const Contour clip = style.arrow.ClipForLine(point_to, 0, true, false, MSC_ARROW_END,
                                             Block(v[0],v[1], 
                                             chart->GetDrawing().y.from, chart->GetDrawing().y.till),
                                             style.line, style.line);
            canvas.Clip(clip);
            canvas.Line(point_to, point_to - XY(len,0), style.line);
            canvas.UnClip();
            style.arrow.Draw(point_to, 0, true, false, MSC_ARROW_END, style.line, style.line, &canvas);
            style.arrow.UnTransformCanvas(canvas);
        } else
            cover = CoverAll(canvas, point_to, pos_center);
        canvas.Shadow(cover.CreateExpand(style.line.Spacing()), style.shadow);
        canvas.Fill(cover.CreateExpand(-style.line.Spacing()), style.fill);
        canvas.Line(cover, style.line);
        const double w2 = halfsize.x - style.line.LineWidth();
        parsed_label.Draw(canvas, pos_center.x-w2, pos_center.x+w2,
            pos_center.y-halfsize.y+style.line.LineWidth());
        return;
    } else switch (style.side.second) {
    default:
        _ASSERT(0);
        return;
    case SIDE_LEFT:
        parsed_label.Draw(canvas, chart->sideNoteGap, 
                          chart->XCoord(chart->LNote->pos)-chart->sideNoteGap, 
                          yPos);
        return;
    case SIDE_RIGHT:
        parsed_label.Draw(canvas, chart->XCoord(chart->RNote->pos) + chart->sideNoteGap, 
                          chart->comments_right_side - chart->sideNoteGap, 
                          yPos);
        return;
    }
}

