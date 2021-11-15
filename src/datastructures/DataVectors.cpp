/**
 * @file DataVectors.cpp
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
 * DataVectors class defines ClassVector class and DocumentVector class
 */

#include 	<datastructures/DataVectors.h>

#include	<cmath>
#include 	<iostream>
#include 	<stdexcept>
#include 	<limits>
#include 	<sstream>
#include 	<string>

/**
 * Default constructor
 */
DocumentVector::DocumentVector(){
	docId = 0;
	nodes = new DocNODES(0);
}

void DocumentVector::clear(){
	docId = 0;
	if(nodes!=0){
		nodes->clear();
		delete nodes;
	}
	nodes = new DocNODES(0);
}

/**
 * Constructor using another DocumentVector
 */
DocumentVector::DocumentVector(const DocumentVector & documentVector){
	DocNODES * nodes = new DocNODES();
	setNodes(*nodes);
	DocNODES::const_iterator iter;
	for(iter=documentVector.getNodes().begin(); iter!=documentVector.getNodes().end(); ++iter){
		const VectorNode & node = *iter;
		if(node.getTermId() > 0){
			//VectorNode * newNode = new VectorNode(node.getTermId(), node.getTermWeight());
			getNodes().push_back(VectorNode(node.getTermId(), node.getTermWeight()));
		}
	}
	setDocId(documentVector.getDocId());
	setPositiveExample(documentVector.isPositiveExample());
}

/**
 * Constructor using another ClassVector
 */
DocumentVector::DocumentVector(const ClassVector & classVector){
	DocNODES * nodes = new DocNODES();
	setNodes(*nodes);
	ClsNODES::const_iterator iter;
	for(iter=classVector.getNodes().begin(); iter!=classVector.getNodes().end(); ++iter){
	//for(unsigned long i=0; i<size; i++){
		const VectorNode & node = *iter;
		//VectorNode * node = classVector->getNodes()[i];
		if(node.getTermId() > 0){
			//VectorNode * newNode = new VectorNode(node.getTermId(), node.getTermWeight());
			getNodes().push_back(VectorNode(node.getTermId(), node.getTermWeight()));
		}
	}
	setDocId(0);
}

/**
 * Default destructor
 */
DocumentVector::~DocumentVector(){
	nodes->clear();
	delete nodes;
}

/**
 * Computes the second order norm of this DocumentVector
 */
double const DocumentVector::computeNorm(){
	double norm = 0.0;
	if(nodes->empty()){
		return 0.0;
	}
	DocNODES::iterator iter;
	for(iter=nodes->begin(); iter!=nodes->end(); ++iter){
		VectorNode & node = *iter;
		if(node.getTermId() > 0){
			norm += node.getTermWeight() * node.getTermWeight();
		}
	}
	norm = sqrt(norm);
	return norm;
}

/**
 * Normalizes this DocumentVector
 */
void DocumentVector::normalize(double norm){
	if(norm == 0.0){
		return;
	}
	if(nodes->empty()){
		return;
	}
	DocNODES::iterator iter;
	for(iter=nodes->begin(); iter!=nodes->end(); ++iter){
		VectorNode & node = *iter;
		if(node.getTermId() > 0){
			node.setTermWeight(node.getTermWeight() / norm);
		}
	}
}

/**
 * Default constructor
 */
ClassVector::ClassVector(){
	size = 0;
	threshold = 0.0;
	posThresholdVector = new DocumentVector();
	negThresholdVector = new DocumentVector();
	nodes = new ClsNODES(0);
}

void ClassVector::clear(){
	size = 0;
	threshold = 0.0;

	if(posThresholdVector!=0){
		delete posThresholdVector;
	}
	posThresholdVector = new DocumentVector();

	if(negThresholdVector!=0){
		delete negThresholdVector;
	}
	negThresholdVector = new DocumentVector();

	if(nodes!=0){
		nodes->clear();
		delete nodes;
	}
	nodes = new ClsNODES(0);
}

/**
 * Constructor using another ClassVector
 */
ClassVector::ClassVector(ClassVector & that){
	clear();
	ClsNODES * nodes = new ClsNODES(that.getNodes().size());
	setNodes(*nodes);
	ClsNODES::const_iterator iter;
	for(iter=that.getNodes().begin(); iter!=that.getNodes().end(); ++iter){
		const VectorNode & thatNode = *iter;
		if(thatNode.getTermId() > 0){
			getNodes().at(thatNode.getTermId()-1).replace(thatNode);

		}
	}
	setName(that.getName());
	setSize(that.getSize());
	setThreshold(that.getThreshold());
	DocumentVector * p = new DocumentVector(that.getPosThresholdVector());
	setPosThresholdVector(*p);
	DocumentVector * n = new DocumentVector(that.getNegThresholdVector());
	setNegThresholdVector(*n);
}

/**
 * Constructor using another ClassVector
 */
void ClassVector::replace(const ClassVector & that){
	clear();
	ClsNODES * nodes = new ClsNODES(that.getNodes().size());
	setNodes(*nodes);
	ClsNODES::const_iterator iter;
	for(iter=that.getNodes().begin(); iter!=that.getNodes().end(); ++iter){
		const VectorNode & thatNode = *iter;
		if(thatNode.getTermId() > 0){
			getNodes().at(thatNode.getTermId()-1) = VectorNode(thatNode.getTermId(),thatNode.getTermWeight());
		}
	}
	setName(that.getName());
	setSize(that.getSize());
	setThreshold(that.getThreshold());
	DocumentVector * p = new DocumentVector(that.getPosThresholdVector());
	setPosThresholdVector(*p);
	DocumentVector * n = new DocumentVector(that.getNegThresholdVector());
	setNegThresholdVector(*n);
}

/**
 * Default destructor
 */
ClassVector::~ClassVector(){
	clear();
}

/**
 * Add this ClassVector with a DocumentVector
 */
void ClassVector::addWithDocumentVector(DocumentVector & documentVector, double mul){
	if(documentVector.getNodes().empty()){
		return;
	}
	DocNODES::iterator iter;
	for(iter=documentVector.getNodes().begin(); iter!=documentVector.getNodes().end(); ++iter){
		VectorNode & thatNode = *iter;
		VectorNode & thisNode  = getNodes().at(thatNode.getTermId()-1);
		if(thisNode.getTermId() > 0){
			thisNode.setTermWeight(thisNode.getTermWeight() + thatNode.getTermWeight()*mul);
		}
		else{
			getNodes().at(thatNode.getTermId()-1) = VectorNode(thatNode.getTermId(),thatNode.getTermWeight()*mul);
		}
	}
}

/*
 * Add this ClassVector with another ClassVector
 */
void ClassVector::addWithClassVector(ClassVector & classVector, double mul){
	if(classVector.getNodes().size() < 1){
		return;
	}
	for(unsigned long i=0; i<classVector.getNodes().size(); i++){
		VectorNode & thatNode = classVector.getNodes().at(i);
		if(thatNode.getTermId() > 0){
			VectorNode & thisNode = getNodes().at(thatNode.getTermId()-1);
			if(thisNode.getTermId() > 0){
				thisNode.setTermWeight(thisNode.getTermWeight() + thatNode.getTermWeight()*mul);
			}
			else{
				getNodes().at(thatNode.getTermId()-1) = VectorNode(thatNode.getTermId(),thatNode.getTermWeight()*mul);
			}
		}
	}
}

/**
 * Compute the second order norm of this ClassVector
 */
double const ClassVector::computeNorm(){
	double norm = 0.0;
	if(getNodes().size() < 1){
		return 0.0;
	}
	ClsNODES::const_iterator iter;
	for(iter=getNodes().begin(); iter!=getNodes().end(); ++iter){
		const VectorNode & node = *iter;
		if(node.getTermId()>0){
			norm += node.getTermWeight() * node.getTermWeight();
		}
	}
	norm = sqrt(norm);
	return norm;
}

/**
 * Normalizes this ClassVector
 */
void ClassVector::normalize(double norm){
	if(getNodes().size() < 1){
		return;
	}
	if(norm==0.0){
		return;
	}
	ClsNODES::iterator iter;
	for(iter=getNodes().begin(); iter!=getNodes().end(); ++iter){
		VectorNode & node = *iter;
		if(node.getTermId()>0){
			node.setTermWeight(node.getTermWeight() / norm);
		}
	}
}

/**
 * Multiply this ClassVector
 */
void ClassVector::multiply(double mul){
	if(getNodes().size() < 1){
		return;
	}
	ClsNODES::iterator iter;
	for(iter=getNodes().begin(); iter!=getNodes().end(); ++iter){
		VectorNode & node = *iter;
		if(node.getTermId() > 0){
			node.setTermWeight(node.getTermWeight() * mul);
		}
	}
}

/**
 * Computes the similarity of this ClassVector with a DocumentVector
 */
double const ClassVector::similarityWithDocumentVector(DocumentVector & documentVector){
	double similarity = 0.0;
	if(documentVector.getNodes().empty() || getNodes().size()<1){
		return 0.0;
	}
	DocNODES::const_iterator iter;
	for(iter=documentVector.getNodes().begin(); iter!=documentVector.getNodes().end(); ++iter){
		const VectorNode & thatNode = *iter;
		if(thatNode.getTermId() < 1){
			continue;
		}
		try{
			VectorNode & thisNode = getNodes().at(thatNode.getTermId()-1);
			if(thisNode.getTermId()>0){
				similarity += thisNode.getTermWeight() * thatNode.getTermWeight();
			}
		}
		catch(std::out_of_range& e) {
		    continue;
		}
	}
	return similarity;
}

/**
 * Computes the similarity of this ClassVector with another ClassVector
 */
double const ClassVector::similarityWithClassVector(ClassVector & that){
	double similarity = 0.0;
	if(getNodes().size()<1 || that.getNodes().size()<1){
		return 0.0;
	}
	for(unsigned long i=0; i<that.getNodes().size(); i++){
		VectorNode & thatNode = that.getNodes().at(i);
		if(thatNode.getTermId() > 0){
			VectorNode & thisNode = getNodes().at(thatNode.getTermId()-1);
			if(thisNode.getTermId() > 0){
				similarity += thisNode.getTermWeight() * thatNode.getTermWeight();
			}
		}
	}
	return similarity;
}
