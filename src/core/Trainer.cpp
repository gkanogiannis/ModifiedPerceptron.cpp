/**
 * @file Trainer.cpp
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
 * Trainer class trains a classifier
 */

#include 	<core/Trainer.h>
#include 	<core/ModifiedPerceptron_F1.h>
#include 	<core/Thresholder.h>
#include 	<utils/DocumentClassVectorUtils.h>
#include 	<utils/StringTokenizer.h>

#include 	<cstdio>
#include 	<limits>
#include 	<list>
#include 	<string>
#include 	<time.h>
#include	<vector>

/**
 * Epsilon value for comparisons
 */
const double Trainer::epsilon = Utils::stoa<double>("1.0E-15");

/**
 * Constructor using user defined command line options
 */
Trainer::Trainer(std::map<std::string,std::string> * options) :
	options(options)
	{
}

/**
 * Default destructor
 */
Trainer::~Trainer() {
}

/**
 * Initalizes this Trainer
 */
void Trainer::initialize(){
	classVector = new ClassVector();
	outTxt = fopen(options->at("outTxt").c_str(),"w");
	outModel = fopen(options->at("outModel").c_str(),"w");
	trainFeaturesIds = new std::set<unsigned long>();
	numberOfTrainExamples = new unsigned long[1];
	numberOfTrainExamples[0] = 0;
	maxTrainFeaturesId = new unsigned long[1];
	maxTrainFeaturesId[0] = 0;
}

/**
 * Gets the current Runtime
 */
double Trainer::getRuntime(){
	/* returns the current processor time in hundredth of a second */
	clock_t start;
	start = clock();
	return((double)start/((double)(CLOCKS_PER_SEC)/100.0));
}

/**
 * Starts the training process
 */
void Trainer::startOperation(){
	double wallStartTime = 0.0;
	double wallEndTime = 0.0;

	wallStartTime = getRuntime();

	initialize();

	if(options->at("silent")=="false"?true:false){
		std::cout 	<< options->at("inData") << std::endl;
		fputs(options->at("inData").c_str(), outTxt);fputs("\n", outTxt);
	}

	documentVectors = DocumentClassVectorUtils::getDocumentVectors(&(options->at("inData")), Utils::stoa<unsigned long>(options->at("lowmem")), numberOfTrainExamples, trainFeaturesIds, maxTrainFeaturesId);

	if(options->at("silent")=="false"?true:false){
		std::cout 	<< "Number of TrainExamples="<<numberOfTrainExamples[0]<<" , Number of TrainFeatures="<<trainFeaturesIds->size()<<" , MaxTrainFeaturesId="<<maxTrainFeaturesId[0]<<std::endl;
		fprintf(outTxt,"Number of TrainExamples=%lu , Number of TrainFeatures=%lu , MaxTrainFeaturesId=%lu\n",numberOfTrainExamples[0],(unsigned long)trainFeaturesIds->size(),maxTrainFeaturesId[0]);
	}

	classVector->setName(options->at("inData"));
	ClsNODES * nodes1 = new ClsNODES(maxTrainFeaturesId[0]);
	classVector->setNodes(*nodes1);

	ClassVector * dummy = new ClassVector();
	dummy->setName("dummy");
	ClsNODES * nodes2 = new ClsNODES(maxTrainFeaturesId[0]);
	dummy->setNodes(*nodes2);

	DocsLIST::iterator iter;
	for (iter = documentVectors->begin(); iter != documentVectors->end(); ++iter){
		DocumentVector & documentVector = *iter;

		if(documentVector.isPositiveExample()){
			classVector->addWithDocumentVector(documentVector, 1.0);
			classVector->setSize(classVector->getSize()+1);
		}
		else{
			dummy->addWithDocumentVector(documentVector, 1.0);
			dummy->setSize(dummy->getSize()+1);
		}
	}

	if(options->at("silent")=="false"?true:false){
		std::cout 	<< "Positive examples="<<classVector->getSize()<<std::endl;
		fprintf(outTxt,"Positive examples=%lu\n",classVector->getSize());
		std::cout 	<< "Negative examples="<<dummy->getSize()<<std::endl;
		fprintf(outTxt,"Negative examples=%lu\n",dummy->getSize());
	}

	classVector->normalize((double)classVector->getSize());
	dummy->normalize((double)(numberOfTrainExamples[0]-classVector->getSize()));

	classVector->addWithClassVector(*dummy, -1.0);

	delete dummy;

	if(Utils::stoa<unsigned int>(options->at("iter")) > 0){

		Thresholder * thresholder = new Thresholder(*classVector,*documentVectors);

		//Hyperplane for recall=1
		thresholder->computeThreshold(0);
		delete thresholder;

		ModifiedPerceptron_F1 * mope = new ModifiedPerceptron_F1(*classVector,*documentVectors,Utils::stoa<unsigned int>(options->at("iter")),outTxt,options->at("verbose")=="true"?true:false,options->at("silent")=="true"?true:false);
		mope->modify();
		delete mope;

		DocumentClassVectorUtils::saveClassVector(*classVector, outModel);
	}
	else{
		Thresholder * thresholder = new Thresholder(*classVector,*documentVectors);
		thresholder->computeThreshold(0);
		delete thresholder;
		DocumentClassVectorUtils::saveClassVector(*classVector, outModel);
	}

	wallEndTime = getRuntime();

	if(options->at("silent")=="false"?true:false){
		printf("\nWallTime=%.2f sec\n\n", (wallEndTime-wallStartTime)/100.0);
		fprintf(outTxt,"\nWallTime=%.2f sec\n\n", (wallEndTime-wallStartTime)/100.0);
	}

	fclose(outModel);
	fclose(outTxt);
}
