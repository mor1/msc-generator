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
#include <cassert>
#include "geometry_xarea.h"

namespace geometry {

/////////////////////////////////////////  MscPolygonList implementation

//Substract all holes from surfaces (we are the surfaces)
//drop holes that were not fully inside at least one surface
//we assume we hold al positive polygons, holes hold all inside-out ones
//polygons can fully contain other polygons, but not touch or have real intersects
//same for holes
void PosMscPolygonList::NormalizeWith(InvMscPolygonList &holes)
{
	if (!boundingBox.Overlaps(holes.boundingBox)) {
		holes.clear();
		return;
	}
	PosMscPolygonList s;
	InvMscPolygonList h;
	for (auto hole_i=holes.begin(); hole_i!=holes.end(); /*none*/) {
		bool keep = false;
		for (auto surf_i=begin(); surf_i!=end(); /*none*/) 
			switch (surf_i->PolyProcess(*hole_i, s, h, false)) {
			case MscPolygon::OK:
				//a hole intersects with a polygon, result is the decreased size polygon
				//replace the current polygon with the results
				erase(surf_i++);
				splice_with_bb(surf_i, s);
				break;
			case MscPolygon::A_IS_EMPTY:
				erase(surf_i++);
				break;
			case MscPolygon::B_IS_EMPTY:
				holes.erase(hole_i++);
				continue;
			case MscPolygon::BOTH_EMPTY:
			case MscPolygon::COMPLEMENT:   //polygon plugs hole: remove both
				erase(surf_i);
				holes.erase(hole_i++);
				continue;
			case MscPolygon::APART_INSIDE: //non-inversed hole is inside the polygon
				keep = true;                 //this hole is to keep
				//fallthrough
			case MscPolygon::A_INSIDE_B:
				//a polygon is fully inside an (inverse) hole. 
				//which means in normal terms the hole is not in the polygon
			case MscPolygon::APART_OUTSIDE:
				//can only be if the positive surface is inside the inverted hole
				//do nothing
				surf_i++;
				break;
			case MscPolygon::SAME:         //An inverted hole cannot be equal to a non-inverted polygon
			case MscPolygon::B_INSIDE_A:   //An inverted hole cannot be inside a non-inverted polygon
				_ASSERT(0);
		}
		if (!keep) {
			holes.erase(hole_i++);
			continue;
		}
		hole_i++;
	}
}

void PosMscPolygonList::Union(const PosMscPolygon &b, InvMscPolygonList &holes)
{
	if (b.size()==0) return;
	if (!boundingBox.Overlaps(b.boundingBox)) {
		append(b);
		return;
	}
	PosMscPolygonList s;
	InvMscPolygonList h;
	PosMscPolygon current_blob(b);
	for (auto i=begin(); i!=end(); /*none*/)
		switch (i->PolyProcess(current_blob, s, h, true)) {
		case MscPolygon::OK:             //real union with an existing surface 
			if (s.size()==0) 
				_ASSERT(s.size()>0);
			current_blob.swap(*s.begin());   //update blob with a positive union 
			//add holes. Holes formed this way cannot overlap with existing ones
			//since they are fully outside of the original surfaces
			holes.splice_with_bb(h);
			//fallthrough: now delete i
		case MscPolygon::A_IS_EMPTY:
		case MscPolygon::A_INSIDE_B:     // i is fully covered by b, delete it
			erase(i++);
			break;
		case MscPolygon::APART_OUTSIDE:  //go to next in queue, no holes to add
			i++;
			break;
		case MscPolygon::SAME:           //no change
			return;
		case MscPolygon::B_INSIDE_A:     //b (and the blob) is fully contained in existing surface
			return;                      //here we need to check for nested polygons 
		case MscPolygon::B_IS_EMPTY:     //should have been caught above
		case MscPolygon::BOTH_EMPTY:     //oh my
		case MscPolygon::APART_INSIDE:   //this can only be us being inside-out (= a hole)
		case MscPolygon::COMPLEMENT:     //well, b and us both are clockwise->this cannot be
		default:
			_ASSERT(0);
		}
	append(current_blob);
}

void PosMscPolygonList::Intersect(const PosMscPolygon &b)
{
	if (b.size()==0) return;
	if (!boundingBox.Overlaps(b.boundingBox)) {
		clear();
		return;
	}
	PosMscPolygonList s;
	InvMscPolygonList h;
	for (auto i=begin(); i!=end(); /*none*/)
		switch (i->PolyProcess(b, s, h, false)) {
		case MscPolygon::B_INSIDE_A:     //b is fully in one of our members
		case MscPolygon::SAME:     
			clear();                     //return just b
			append(b);
			return;                      //we will hit no other element
		case MscPolygon::A_IS_EMPTY:
		case MscPolygon::APART_OUTSIDE:  //go to next in queue, remove current one
			erase(i++);			
			break;
		case MscPolygon::A_INSIDE_B:     //this one is fully inside b=>keep it
			i++;
			break;                       
		case MscPolygon::OK:             //real intersect with our polygon
			erase(i++);  
			//splice resuling fragments before current position -> not 
			//to be processed again (works with both cw or countercw b)
			splice_with_bb(i, s);
			break;
		case MscPolygon::B_IS_EMPTY:   //should have been caught above
		case MscPolygon::BOTH_EMPTY:   //oh my
		case MscPolygon::COMPLEMENT:   //cannot be for two positive surfaces
		case MscPolygon::APART_INSIDE: //cannot be for two positive surfaces
			_ASSERT(0);
		}
}

//returns true of b as a hole is fully inside one polygon
//and thus shall be added to holes
void PosMscPolygonList::Intersect(const InvMscPolygon &b, InvMscPolygonList &holes)
{
	if (b.size()==0) return;
	if (!boundingBox.Overlaps(b.boundingBox)) 
		return;
	PosMscPolygonList s;
	InvMscPolygonList h;
	for (auto i=begin(); i!=end(); /*none*/)
		switch (i->PolyProcess(b, s, h, false)) {
		case MscPolygon::COMPLEMENT:     //b is exactly our inverse
			erase(i);                    //kill this, but keep all other polygons
			return;
		case MscPolygon::APART_OUTSIDE:  //i is inside the hole -> delete it
		case MscPolygon::A_IS_EMPTY:     //go to next in queue, remove current one
			erase(i++);			
			break;
		case MscPolygon::A_INSIDE_B:     //this one is fully inside b=>keep it
			i++;
			break;                       
		case MscPolygon::APART_INSIDE:   //b (hole) is inside us
			holes.append(b);
			return;         
		case MscPolygon::OK:             //real intersect with our polygon
			erase(i++);  
			//splice resuling fragments before current position -> not 
			//to be processed again 
			splice_with_bb(i, s);
			break;
		case MscPolygon::B_IS_EMPTY:   //should have been caught above
		case MscPolygon::BOTH_EMPTY:   //oh my
		case MscPolygon::SAME:         //cannot be for a pos and an inv polygon
		case MscPolygon::B_INSIDE_A:   //can only happen if b is clockwise, as i is so
			_ASSERT(0);
		}
}

void PosMscPolygonList::Intersect(const PosMscPolygonList &b) 
{
	if (!boundingBox.Overlaps(b.boundingBox)) {
		clear();
		return;
	}
	PosMscPolygonList result;
	PosMscPolygonList s;
	InvMscPolygonList h;
	for (auto i=begin(); i!=end(); i++)
		for (auto j=b.begin(); j!=b.end(); j++)
			switch (i->PolyProcess(*j, s, h, false)) {
			case MscPolygon::A_INSIDE_B:     //this one is fully inside b=>keep it
			case MscPolygon::SAME:     
				j = --b.end();                 //this i is handled permanently
				result.append(*i);           //append *i;
				break;
			case MscPolygon::B_INSIDE_A:     //b is fully in one of our members
				result.append(b);
				break;
			case MscPolygon::OK:             //real intersect with our polygon
				result.append(s);
				break;                       //we will hit no other element
			case MscPolygon::A_IS_EMPTY:
			case MscPolygon::BOTH_EMPTY:   
				j = --b.end();
			case MscPolygon::APART_OUTSIDE:  //go to next in queue, remove current one
			case MscPolygon::B_IS_EMPTY:     //no action 
				break;                       
			case MscPolygon::COMPLEMENT:   //cannot be for two positive surfaces
			case MscPolygon::APART_INSIDE: //cannot be for two positive surfaces
			default:
				_ASSERT(0);
			}
	swap(result);
}


//this is very much like a union of holes
void InvMscPolygonList::Intersect(const InvMscPolygon &b, PosMscPolygonList &surfaces)
{
	if (b.size()==0) return;
	if (!boundingBox.Overlaps(b.boundingBox)) {
		append(b);
		return;
	}
	PosMscPolygonList s;
	InvMscPolygonList h;
	InvMscPolygon current_blob(b);
	for (auto i=begin(); i!=end();  /*none*/)
		switch (i->PolyProcess(current_blob, s, h, false)) {
		case MscPolygon::OK:             //real union with an existing surface 
			_ASSERT(h.size()>0);
			current_blob.swap(*h.begin());   //update blob with a bigger hole 
			//add positive fragments. surfaces formed this way cannot overlap with existing ones
			//since they are fully outside of the original holes
			surfaces.splice_with_bb(s);
			//fallthrough: now delete i
		case MscPolygon::A_IS_EMPTY:
		case MscPolygon::B_INSIDE_A:     // the blob is the bigger hole, delete the smaller one
			erase(i++);
			break;
		case MscPolygon::APART_INSIDE:   //the two holes do not meet: keep them
			i++;
			break;
		case MscPolygon::SAME:           //no change to us
			return;
		case MscPolygon::A_INSIDE_B:     //the blob hole is inside one of our holes
			return;                      //here we need to check for nested holes 
		case MscPolygon::B_IS_EMPTY:     //should have been caught above
		case MscPolygon::BOTH_EMPTY:     //oh my
		case MscPolygon::APART_OUTSIDE:  //this can only be for two holes
		case MscPolygon::COMPLEMENT:     //well, b and us both are counterclockwise->this cannot be
		default:
			_ASSERT(0);
		}
	append(current_blob);
	return;
}

//return true if b is not touching any hole edges and is inside one fully
//this means b is fully inside one of the holes and will need to be added as an extra positive surface
bool InvMscPolygonList::Union(const PosMscPolygon &b)
{
	if (b.size()==0) return false;
	if (!boundingBox.Overlaps(b.boundingBox)) {
		return false;
	}
	PosMscPolygonList s;
	InvMscPolygonList h;
	bool inside = false;
	bool touched = false;
	for (auto i=begin(); i!=end(); /*none*/)
		switch (i->PolyProcess(b, s, h, true)) {
		case MscPolygon::OK:             //real union with an existing surface 
			if (s.size()>0)
				_ASSERT (0);        //union should really be the universe minus some holes in h
			splice_with_bb(i, h);        //insert them before i
			touched = true;
			//fallthrough: now delete i
		case MscPolygon::A_IS_EMPTY:
		case MscPolygon::APART_INSIDE:   //b fully cover the hole->remove it
			erase(i++);
			break;
		case MscPolygon::APART_OUTSIDE:  //b fully inside the hole->flag for return
			inside = true;
		case MscPolygon::B_INSIDE_A:     //b is outside the hole, just move on
			i++;
			break;
		case MscPolygon::B_IS_EMPTY:     //shpould have been caught above
		case MscPolygon::BOTH_EMPTY:     //oh my
		case MscPolygon::SAME:           //well, b is clockwise, we are not->this cannot be
		case MscPolygon::A_INSIDE_B:     //A is inside out, b is not, this should not be
		default:
			_ASSERT(0);
		}
	return inside && !touched;
}

void InvMscPolygonList::Union(const InvMscPolygonList &b) 
{
	if (!boundingBox.Overlaps(b.boundingBox)) {
		clear();
		return;
	}
	//this is like intersect of the holes, i will use that wording in comments
	InvMscPolygonList result;
	PosMscPolygonList s;
	InvMscPolygonList h;
	for (auto i=begin(); i!=end(); i++)
		for (auto j=b.begin(); j!=b.end(); j++)
			switch (i->PolyProcess(*j, s, h, true)) {
			case MscPolygon::SAME:           //well, b is clockwise, we are not->this cannot be
			case MscPolygon::B_INSIDE_A:     //hole in i is fully inside hole in j
				result.append(*i);
				j = b.end();
				break;
			case MscPolygon::A_INSIDE_B:     //hole in j is fully inside hole in i
				result.append(*j);
				break;
			case MscPolygon::OK:             //real union with an existing surface 
				_ASSERT (s.size()==0);        //union should really be the universe minus some holes in h
				result.splice_with_bb(h);            //insert 
				break;
			case MscPolygon::A_IS_EMPTY:
			case MscPolygon::BOTH_EMPTY:     //skip this hole in a
				j = b.end();
				break;
			case MscPolygon::APART_INSIDE:   //holes do not touch: no intersect
			case MscPolygon::B_IS_EMPTY:     //no action
				break;
			case MscPolygon::APART_OUTSIDE:  //b fully inside the hole->flag for return
			case MscPolygon::COMPLEMENT:     //cannot be for two negative surfaces
			default:
				_ASSERT(0);
			}
	swap(result);
}

/////////////////////////////////////////  GeoArea implementation


bool GeoArea::IsWithin(XY p) const 
{
	if (surfaces.boundingBox.IsWithin(p) == WI_OUTSIDE) return WI_OUTSIDE;
	is_within_t ret;
	int dummy;
	for (auto i = holes.begin(); i!= holes.end(); i++) 
		if (WI_INSIDE == (ret = i->IsWithin(p, dummy))) continue; //holes are outside-in, this means outside the hole
		else return ret; //on edge, vertex or in a hole (marked by WI_OUTSIDE)
	for (auto i = surfaces.begin(); i!= surfaces.end(); i++) 
		if (WI_OUTSIDE == (ret = i->IsWithin(p, dummy))) continue; 
		else return ret; //on edge, vertex or in a surface 
	return WI_OUTSIDE;
}


GeoArea &GeoArea::operator += (const GeoArea &b) 
{
	if (!surfaces.boundingBox.Overlaps(b.surfaces.boundingBox)) {  // do not touch
		surfaces.append(b.surfaces);
		holes.append(b.holes);
		return *this;
	}
	//create a copy of b's holes and substract our surface from them
	InvMscPolygonList new_b_holes = b.holes;
	for (auto i=surfaces.begin(); i!=surfaces.end(); i++)
		new_b_holes.Union(*i);
	//and vice versa
	InvMscPolygonList new_a_holes = holes;
	for (auto i=b.surfaces.begin(); i!=b.surfaces.end(); i++) 
		new_a_holes.Union(*i);
	//create an intersection of holes
	holes.Union(b.holes);
	//splice all holes (they cannot intersect...)
	holes.splice_with_bb(new_a_holes);
	holes.splice_with_bb(new_b_holes);
	//Add all of b's surfaces to us 
	for (auto i=b.surfaces.begin(); i!=b.surfaces.end(); i++) 
		surfaces.Union(*i, holes);  //extra holes appended to "holes" by Union

	mainline += b.mainline;
	return *this;
}

GeoArea &GeoArea::operator *= (const GeoArea &b) 
{
	if (!surfaces.boundingBox.Overlaps(b.surfaces.boundingBox)) {  // do not touch
		surfaces.clear();
		holes.clear();
		return *this;
	}
	PosMscPolygonList collect, tmp;
	//create an intersect of the positive surfaces
	surfaces.Intersect(b.surfaces);

	//substract both holesets (dropping unnecessary holes in the process)
	InvMscPolygonList new_b_holes = b.holes;
	surfaces.NormalizeWith(holes);
	surfaces.NormalizeWith(new_b_holes);
	//Now combine the two holesets
	PosMscPolygonList dummy;
	for (auto i=new_b_holes.begin(); i!=new_b_holes.end(); i++)
		holes.Intersect(*i, dummy);  
	surfaces.NormalizeWith(holes);

	mainline *= b.mainline;
	return *this;
}


GeoArea &GeoArea::operator -= (const GeoArea &b) 
{
	if (!surfaces.boundingBox.Overlaps(b.surfaces.boundingBox)) {  // do not touch
		return *this;
	}

	//first substract the surfaces of b from us
	for (auto i=b.surfaces.begin(); i!=b.surfaces.end(); i++)
		surfaces.Substract(*i, holes);  
	//calculate pieces of our surface that fell into holes of b
	PosMscPolygonList surf_fragments(surfaces);
	for (auto i=b.holes.begin(); i!=b.holes.end(); i++)
		surf_fragments.Substract(*i);    //intersection of holes and surfaces
	//combine the two surface sets
	for (auto i=surf_fragments.begin(); i!=surf_fragments.end(); i++) 
		surfaces.Union(*i, holes);  //extra holes appended to "holes" by Union
	//now limit the holes to the new surface
	surfaces.NormalizeWith(holes);

	//substract mainlines, too
	//if any of our from or till falls into g's mainline, act.
	if (b.mainline.IsWithin(mainline.from)) mainline.from=b.mainline.till; 
	if (b.mainline.IsWithin(mainline.till)) mainline.till=b.mainline.from;
	//we may end up being invalid (=empty) (=till<from);
	return *this;
}
void GeoArea::Line(cairo_t *cr) const
{
	double dash[] = {2, 2};
	holes.Path(cr);
	cairo_set_dash(cr, dash, 2, 0);
	cairo_stroke(cr);
	surfaces.Path(cr);
	cairo_set_dash(cr, dash, 0, 0);
	cairo_stroke(cr);
}

void GeoArea::Fill(cairo_t *cr) const
{
	surfaces.Path(cr);
	holes.Path(cr);
	cairo_fill(cr);
}

void GeoArea::swap(GeoArea &a) 
{
	surfaces.swap(a.surfaces); 
	holes.swap(a.holes); 
	std::swap(mainline, a.mainline);
}

}; //namespace
