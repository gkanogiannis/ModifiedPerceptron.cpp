/**
 * @file Thresholder.cpp
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
 * Thresholder class computes threshold for a ClassVector
 */

#include 	<core/Thresholder.h>
#include 	<datastructures/DataVectors.h>

#include 	<iostream>
#include 	<limits>

/**
 * Constructor using values
 */
Thresholder::Thresholder(ClassVector & classVector, DocsLIST & documentVectors) :
	classVector(classVector), documentVectors(documentVectors)
	{
	initialize();
}

/**
 * Default destructor
 */
Thresholder::~Thresholder() {
}

/**
 * Initializes this Thresholder
 */
void Thresholder::initialize(){
	TP = TN = FP = FN = 0;
	minHeap = new MINHEAP();
	maxHeap = new MAXHEAP();
	posThresholdVector = 0;
	negThresholdVector = 0;
}

/**
 * Computes the threshold for this ClassVector and that mode
 */
void Thresholder::computeThreshold(int mode){//0 recall=1, 1 precison=1, 2 SCut
	double threshold = - std::numeric_limits<double>::infinity();

	if(mode==0){
		//recall=1
		clear();
		DocsLIST::iterator iter;
		for(iter=documentVectors.begin(); iter!=documentVectors.end(); ++iter){
			DocumentVector & documentVector = *iter;
			minHeap->push(SimilarityNode(classVector.similarityWithDocumentVector(documentVector),&documentVector));
		}
		const SimilarityNode * sNode = 0;
		const SimilarityNode * previousSNode = 0;
		while(!minHeap->empty()){
			previousSNode = sNode;
			sNode = &minHeap->top();
			if(sNode->getDocumentVector()->isPositiveExample()){
				posThresholdVector = new DocumentVector(*sNode->getDocumentVector());
				if(previousSNode != 0){
					negThresholdVector = new DocumentVector(*previousSNode->getDocumentVector());
					threshold = (sNode->getSimilarity()+previousSNode->getSimilarity())/2.0;
				}
				else{
					threshold = sNode->getSimilarity()/2.0;
				}
				break;
			}
			minHeap->pop();
		}
		classVector.setThreshold(threshold);
		classVector.setPosThresholdVector(*posThresholdVector);
		classVector.setNegThresholdVector(*negThresholdVector);
	}
	/*
	else if(mode==1){
		//precision=1
		clear();
		DocsLIST::iterator iter;
		for(iter=documentVectors.begin(); iter!=documentVectors.end(); ++iter){
			DocumentVector & documentVector = *iter;
			SimilarityNode sNode(classVector.similarityWithDocumentVector(documentVector),&documentVector);
			maxHeap->push(sNode);
		}
		while(!maxHeap->empty()){
			SimilarityNode sNode = maxHeap->top();
			if(!sNode.getDocumentVector()->isPositiveExample()){
				thresholdVector = sNode.getDocumentVector();
				threshold = sNode.getSimilarity();
				break;
			}
			maxHeap->pop();
			sNode.~SimilarityNode();
		}
		classVector.setThreshold(threshold);
		classVector.setNegThresholdVector(thresholdVector);
	}
	*/
	clear();
}

/**
 * Returns the current F1
 */
double Thresholder::computeF1(){
	double p = 0.0;
	double r = 0.0;
	double F1 = 0.0;
	if(TP+FP!=0){
		p = (double)TP / (double)(TP+FP);
	}
	if(TP+FN!=0){
		r = (double)TP / (double)(TP+FN);
	}
	if(p+r!=0.0f){
		F1 = 2.0f*p*r / (p+r);
	}
	return F1;
}

/**
 * Clears this Thresholder
 */
void Thresholder::clear(){
	TP = TN = FP = FN = 0;
	minHeap = new MINHEAP();
	maxHeap = new MAXHEAP();
}
