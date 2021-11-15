/**
 * @file SimilarityNode.cpp
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
 * SimilarityNode class holds the similarity between ClassVector and DocumentVector
 */

#include 	<datastructures/SimilarityNode.h>

/**
 * Default constructor
 */
SimilarityNode::SimilarityNode(){
	setSimilarity(0.0);
}

/**
 * Constructor
 */
SimilarityNode::SimilarityNode(const SimilarityNode & other){
	setSimilarity(other.getSimilarity());
	setDocumentVector(other.getDocumentVector());
}

/**
 * Default destructor
 */
SimilarityNode::~SimilarityNode(){
}

/**
 * Constructor
 */
SimilarityNode::SimilarityNode(double similarity, DocumentVector * documentVector){
	setSimilarity(similarity);
	setDocumentVector(documentVector);
}
