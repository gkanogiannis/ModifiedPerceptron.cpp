/*
 *
 * ModifiedPerceptron_F1
 *
 * Copyright (C) 2021 Anestis Gkanogiannis <anestis@gkanogiannis.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */

#include 	<core/ModifiedPerceptron_F1.h>
#include 	<core/Trainer.h>
#include 	<datastructures/DataVectors.h>

#include 	<cstdio>
#include 	<iostream>
#include	<math.h>
#include	<limits>

ModifiedPerceptron_F1::ModifiedPerceptron_F1(ClassVector & classVector, DocsLIST & documentVectors, unsigned int iterations, FILE * outTxt, bool verbose, bool silent) :
	classVector(classVector) , documentVectors(documentVectors)
	{
	initialize(iterations,outTxt,verbose,silent);
}

ModifiedPerceptron_F1::~ModifiedPerceptron_F1() {
	clear();
}

void ModifiedPerceptron_F1::initialize(unsigned int iterations, FILE *outTxt, bool verbose, bool silent){
	this->iterations = iterations;
	this->outTxt = outTxt;
	this->verbose = verbose;
	this->silent = silent;

	//TPHeap = 0;
	//FPHeap = 0;
	//FNHeap = 0;
	//TNHeap = 0;
	FPArray = 0;
	FNArray = 0;
	FPCentroid = 0;
	FNCentroid = 0;
}

void ModifiedPerceptron_F1::modify(){
	unsigned int iteration = 0;
	double maxF1 = - std::numeric_limits<double>::infinity();
	ClassVector * maxClassVector = new ClassVector();

	//Compute F1 for iteration 0 and update max
	clear();
	fillHeaps();
	double F1 = computeF1();
	if(F1 > maxF1){
		maxF1 = F1;
		if(maxClassVector!=0) {
			delete maxClassVector;
		}
		maxClassVector = new ClassVector(classVector);
	}

	if(verbose && !silent){
		std::cout<<std::endl<<"it="<<iteration<<"\tF1="<<F1<<"\tTP="<<TP<<"\tFP="<<FP<<"\tFN="<<FN<<"\tTN="<<TN<<std::endl;
		fprintf(outTxt,"\nit=%u\tF1=%.8f\tTP=%lu\tFP=%lu\tFN=%lu\tTN=%lu\n",iteration,F1,TP,FP,FN,TN);
	}

	if(F1==1.0 && !silent){
		std::cout<<"\niterations="<<iteration<<std::endl;
		fprintf(outTxt,"\niterations=%u\n",iteration);
		std::cout<<"maxF1="<<F1<<"\tTP="<<TP<<"\tFP="<<FP<<"\tFN="<<FN<<"\tTN="<<TN<<std::endl;
		fprintf(outTxt,"maxF1=%.8f\tTP=%lu\tFP=%lu\tFN=%lu\tTN=%lu\n",F1,TP,FP,FN,TN);
		return;
	}

	do{
		iteration++;
		iterations--;

		if(verbose && !silent){
			//std::cout<<"it="<<iteration<<"\t";
			//fprintf(outTxt,"it=%u\t",iteration);
		}
		else if (!silent){
			std::cout<<"*";
			std::cout.flush();
		}

		//Compute FN,FP Centroids
		computeMiscclasifiedCentroids();

		double heta = 0.0;
		ClassVector * dummy;
		DocumentVector * e;

		if(FN!=0 && FP!=0){
			dummy = new ClassVector();
			ClsNODES * nodes = new ClsNODES(classVector.getNodes().size());
			dummy->setNodes(*nodes);
			dummy->addWithDocumentVector(*FNCentroid, 1.0);
			dummy->addWithDocumentVector(*FPCentroid, -1.0);
			e = new DocumentVector(*dummy);
			delete dummy;

			double eNorm = e->computeNorm();
			heta = - (classVector.similarityWithDocumentVector(*e))/(eNorm*eNorm);

			classVector.addWithDocumentVector(*e, heta);
			DocumentVector * p = new DocumentVector(*FNCentroid);
			classVector.setPosThresholdVector(*p);
			DocumentVector * n = new DocumentVector(*FPCentroid);
			classVector.setNegThresholdVector(*n);
			classVector.setThreshold(classVector.similarityWithDocumentVector(*FNCentroid));

			delete e;
		}
		else if(FN==0){
			dummy = new ClassVector();
			ClsNODES * nodes = new ClsNODES(classVector.getNodes().size());
			dummy->setNodes(*nodes);
			dummy->addWithDocumentVector(classVector.getPosThresholdVector(), 1.0);
			dummy->addWithDocumentVector(*FPCentroid, -1.0);
			e = new DocumentVector(*dummy);
			delete dummy;

			double eNorm = e->computeNorm();
			heta = - (classVector.similarityWithDocumentVector(*e))/(eNorm*eNorm);

			classVector.addWithDocumentVector(*e, heta);
			DocumentVector * p = new DocumentVector(*FPCentroid);
			classVector.setNegThresholdVector(*p);
			classVector.setThreshold(classVector.similarityWithDocumentVector(*FPCentroid));

			delete e;
		}
		else if(FP==0){
			dummy = new ClassVector();
			ClsNODES * nodes = new ClsNODES(classVector.getNodes().size());
			dummy->setNodes(*nodes);
			dummy->addWithDocumentVector(*FNCentroid, 1.0);
			dummy->addWithDocumentVector(classVector.getNegThresholdVector(), -1.0);
			e = new DocumentVector(*dummy);
			delete dummy;

			double eNorm = e->computeNorm();
			heta = - (classVector.similarityWithDocumentVector(*e))/(eNorm*eNorm);

			classVector.addWithDocumentVector(*e, heta);
			DocumentVector * n = new DocumentVector(*FNCentroid);
			classVector.setPosThresholdVector(*n);
			classVector.setThreshold(classVector.similarityWithDocumentVector(*FNCentroid));

			delete e;
		}

		//Compute F1 for iteration i and update max
		clear();
		fillHeaps();
		F1 = computeF1();

		if(F1 > maxF1){
			maxF1 = F1;
			if(maxClassVector!=0){
				delete maxClassVector;
			}
			maxClassVector = new ClassVector(classVector);
		}

		if(verbose && !silent){
			std::cout<<"it="<<iteration<<"\tF1="<<F1<<"\tTP="<<TP<<"\tFP="<<FP<<"\tFN="<<FN<<"\tTN="<<TN<<std::endl;
			fprintf(outTxt,"it=%u\tF1=%.8f\tTP=%lu\tFP=%lu\tFN=%lu\tTN=%lu\n",iteration,F1,TP,FP,FN,TN);
		}

		if(F1 == 1.0){
			break;
		}
	}
	while(iterations>0);

	classVector.replace(*maxClassVector);
	delete maxClassVector;

	//Compute F1 and heaps for maxclassvector
	clear();
	fillHeaps();
	F1 = computeF1();

	if(!silent){
		std::cout<<"\niterations="<<iteration<<std::endl;
		fprintf(outTxt,"\niterations=%u\n",iteration);
		std::cout<<"maxF1="<<F1<<"\tTP="<<TP<<"\tFP="<<FP<<"\tFN="<<FN<<"\tTN="<<TN<<std::endl;
		fprintf(outTxt,"maxF1=%.8f\tTP=%lu\tFP=%lu\tFN=%lu\tTN=%lu\n",F1,TP,FP,FN,TN);
	}

	return;
}

void ModifiedPerceptron_F1::fillHeaps(){
	DocsLIST::iterator iter;
	for(iter=documentVectors.begin(); iter!=documentVectors.end(); ++iter){
		DocumentVector & documentVector = *iter;
		double similarity = classVector.similarityWithDocumentVector(documentVector);

		if(fabs(similarity - classVector.getThreshold()) < Trainer::epsilon){
			if(documentVector.isPositiveExample()){
				TP++;
				//TPHeap->push(sNode);
			}
			else{
				TN++;
				//TNHeap->push(sNode);
			}
		}
		else if(similarity > classVector.getThreshold()){
			if(documentVector.isPositiveExample()){
				TP++;
				//TPHeap->push(sNode);
			}
			else{
				FP++;
				FPArray->push_back(SimilarityNode(similarity,&documentVector));
				//FPHeap->push(sNode);
			}
		}
		else if(similarity < classVector.getThreshold()){
			if(documentVector.isPositiveExample()){
				FN++;
				FNArray->push_back(SimilarityNode(similarity,&documentVector));
				//FNHeap->push(sNode);
			}
			else{
				TN++;
				//TNHeap->push(sNode);
			}
		}
	}
}

void ModifiedPerceptron_F1::computeMiscclasifiedCentroids(){
	ClassVector * dummy = 0;

	if(FN > 0){
		dummy = new ClassVector();
		ClsNODES * nodes = new ClsNODES(classVector.getNodes().size());
		dummy->setNodes(*nodes);
		std::vector<SimilarityNode>::iterator iter;
		for(iter=FNArray->begin(); iter!=FNArray->end(); iter++){
			dummy->addWithDocumentVector(*(*iter).getDocumentVector(),1.0);
		}
		dummy->normalize((double)FN);
		if(FNCentroid!=0) {
			delete FNCentroid;
		}
		FNCentroid = new DocumentVector(*dummy);
		FNCentroid->setPositiveExample(true);
		delete dummy;
	}

	if(FP > 0){
		dummy = new ClassVector();
		ClsNODES * nodes = new ClsNODES(classVector.getNodes().size());
		dummy->setNodes(*nodes);
		std::vector<SimilarityNode>::iterator iter;
		for(iter=FPArray->begin(); iter!=FPArray->end(); iter++){
			dummy->addWithDocumentVector(*(*iter).getDocumentVector(),1.0);
		}
		dummy->normalize((double)FP);
		if(FPCentroid!=0){
			delete FPCentroid;
		}
		FPCentroid = new DocumentVector(*dummy);
		FPCentroid->setPositiveExample(false);
		delete dummy;
	}
}

double ModifiedPerceptron_F1::computeF1(){
	double p = 0.0;
	double r = 0.0;
	double F1 = 0.0;
	if(TP+FP!=0){
		p = (double)TP / (double)(TP+FP);
	}
	if(TP+FN!=0){
		r = (double)TP / (double)(TP+FN);
	}
	if(p+r!=0.0){
		F1 = 2.0*p*r / (p+r);
	}
	return F1;
}

void ModifiedPerceptron_F1::clear(){
	TP = TN = FP = FN = 0;

	//if(TPHeap!=0){
		//delete TPHeap;
	//}
	//TPHeap = new MINHEAP();
	//if(FNHeap!=0) {
		//delete FNHeap;
	//}
	//FNHeap = new MAXHEAP();
	//if(FPHeap!=0) {
		//delete FPHeap;
	//}
	//FPHeap = new MINHEAP();
	//if(TNHeap!=0) {
		//delete TNHeap;
	//}
	//TNHeap = new MAXHEAP();

	if(FNArray!=0){
		FNArray->clear();
		delete FNArray;
	}
	FNArray = new std::vector<SimilarityNode>();

	if(FPArray!=0){
		FPArray->clear();
		delete FPArray;
	}
	FPArray = new std::vector<SimilarityNode>();

	if(FNCentroid!=0) {
		delete FNCentroid;
	}
	FNCentroid = new DocumentVector();
	if(FPCentroid!=0) {
		delete FPCentroid;
	}
	FPCentroid = new DocumentVector();
}
