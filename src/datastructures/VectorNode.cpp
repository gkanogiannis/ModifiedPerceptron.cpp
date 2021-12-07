/**
 * @file VectorNode.cpp
 * @author Anestis Gkanogiannis  <anestis@gkanogiannis.com>
 * @version 1.0 01/01/2012
 *
 * @section LICENSE
 *
 * Copyright (C) <2012>  <Anestis Gkanogiannis> <anestis@gkanogiannis.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/copyleft/gpl.html>.
 *
 * @section DESCRIPTION
 *
 * VectorNode class holds a termId:termWeight pair
 */

#include 	<datastructures/VectorNode.h>

/**
 * Default constructor
 */
VectorNode::VectorNode(){
	setTermId(0);
	setTermWeight(0.0);
}

/**
 * Constructor
 */
VectorNode::VectorNode(const VectorNode & other){
	setTermId(other.getTermId());
	setTermWeight(other.getTermWeight());
}

/**
 * Constructor
 */
VectorNode::VectorNode(unsigned long termId, double termWeight){
	setTermId(termId);
	setTermWeight(termWeight);
}

void VectorNode::replace(const VectorNode & other){
	setTermId(other.getTermId());
	setTermWeight(other.getTermWeight());
}
