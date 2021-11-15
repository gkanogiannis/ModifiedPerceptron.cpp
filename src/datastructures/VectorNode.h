/**
 * @file VectorNode.h
 * @author Anestis Gkanogiannis  <anestis.gk@unb.ca>
 * @version 1.0 01/01/2012
 *
 * @section LICENSE
 *
 * Copyright (C) <2012>  <Anestis Gkanogiannis> <anestis.gk@unb.ca>
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

#ifndef VECTORNODE_H_
#define VECTORNODE_H_

class VectorNode {
private:
	/*
	 * termId
	 */
	unsigned long termId;

	/*
	 * termWeight
	 */
	double termWeight;

public:
    /**
     * Default Constructor
     */
	VectorNode();

    /**
     * Constructor
     */
	VectorNode(const VectorNode &);

	/**
	 * Constructor
	 */
	VectorNode(unsigned long, double);

	void replace(const VectorNode &);

	/**
	 * termId getter
	 */
	unsigned long getTermId() const {return termId;}

	/**
	 * termId setter
	 */
	void setTermId(unsigned long termId){this->termId = termId;}

	/**
	 * termWeight getter
	 */
	double getTermWeight() const {return termWeight;}

	/**
	 * termWeight setter
	 */
	void setTermWeight(double termWeight){this->termWeight = termWeight;}
};

#endif
