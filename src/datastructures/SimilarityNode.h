/**
 * @file SimilarityNode.h
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

#ifndef SIMILARITYNODE_H_
#define SIMILARITYNODE_H_

#include 	<datastructures/DataVectors.h>

class SimilarityNode {
private:
	/**
	 * Similarity
	 */
	double similarity;

	/**
	 * A DocumentVector
	 */
	DocumentVector * documentVector;
public:
	/**
	 * Default constructor
	 */
	SimilarityNode();

	/**
	 * Constructor
	 */
	SimilarityNode(const SimilarityNode &);

	/**
	 * Default destructor
	 */
	~SimilarityNode();

	/**
	 * Constructor
	 */
	SimilarityNode(double, DocumentVector *);

	/**
	 * similarity getter
	 */
    double getSimilarity() const {return similarity;}

    /**
     * similarity setter
     */
    void setSimilarity(double similarity) {this->similarity = similarity;}

    /**
     * documentVector getter
     */
    DocumentVector * getDocumentVector() const {return documentVector;}

    /**
     * documentVector setter
     */
    void setDocumentVector(DocumentVector * documentVector) {this->documentVector = documentVector;}

    /**
     * Greater comparator struct
     */
   	struct greater_SimilarityNode{
   		bool operator()(SimilarityNode & first, SimilarityNode & second){
   			if(first.getSimilarity() >= second.getSimilarity()){
   				return true;
   			}
   			else{
   				return false;
   			}
   		}
   	};

    /**
     * Greater comparator struct (pointers)
     */
	struct greater_SimilarityNode_pointer{
		bool operator()(SimilarityNode * first, SimilarityNode * second){
			if(first->getSimilarity() >= second->getSimilarity()){
				return true;
			}
			else{
				return false;
			}
		}
	};

	/**
	 * Less comparator struct
	 */
	struct less_SimilarityNode{
		bool operator()(SimilarityNode & first, SimilarityNode & second){
			if(first.getSimilarity() < second.getSimilarity()){
				return true;
			}
			else{
				return false;
			}
		}
	};

	/**
	 * Less comparator struct (pointers)
	 */
	struct less_SimilarityNode_pointer{
		bool operator()(SimilarityNode * first, SimilarityNode * second){
			if(first->getSimilarity() < second->getSimilarity()){
				return true;
			}
			else{
				return false;
			}
		}
	};
};

#endif
