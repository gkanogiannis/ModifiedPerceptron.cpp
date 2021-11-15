/**
 * @file Evaluator.cpp
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
 * Evaluator class evaluates (testing) a trained classifier
 */

#include 	<core/Tester.h>
#include 	<utils/DocumentClassVectorUtils.h>
#include 	<utils/Utils.h>
#include	<utils/PrecisionRecallEvaluation.h>
#include	<utils/ScoredPrecisionRecallEvaluation.h>

#include	<cmath>
#include	<ctime>

/**
 * Constructor using user defined command line options
 */
Tester::Tester(std::map<std::string,std::string> * options) :
	options(options)
	{
}

/**
 * Default destructor
 */
Tester::~Tester() {
}

/**
 * Starts the evaluating(testing) process
 */
void Tester::startOperation(){
	double wallStartTime = 0.0;
	double wallEndTime = 0.0;

	wallStartTime = getRuntime();

	initialize();

	std::cout 	<< options->at("inData") << std::endl;
	fputs(options->at("inData").c_str(), outTxt);fputs("\n", outTxt);

	documentVectors = DocumentClassVectorUtils::getDocumentVectors(&(options->at("inData")), Utils::stoa<unsigned long>(options->at("lowmem")), numberOfTestExamples, testFeaturesIds, maxTestFeaturesId);

	std::cout 	<< "Number of TestExamples="<<numberOfTestExamples[0]<<" , Number of TestFeatures="<<(unsigned long)testFeaturesIds->size()<<" , MaxTestFeaturesId="<<maxTestFeaturesId[0]<<std::endl;
	fprintf(outTxt,"Number of TestExamples=%lu , Number of TestFeatures=%lu , MaxTestFeatures%lu=\n",numberOfTestExamples[0],(unsigned long)testFeaturesIds->size(),maxTestFeaturesId[0]);

	classVector = DocumentClassVectorUtils::getClassVector(&(options->at("inModel")));

	std::vector<SimilarityNode> * sNodeList = new std::vector<SimilarityNode>();
	DocsLIST::iterator iter1;
	for (iter1 = documentVectors->begin(); iter1 != documentVectors->end(); ++iter1){
		DocumentVector & documentVector = *iter1;
		double similarity = classVector->similarityWithDocumentVector(documentVector);
		sNodeList->push_back(SimilarityNode(similarity,&documentVector));
		fprintf(outPred,"%f\n",(similarity-classVector->getThreshold()));

		if(fabs(similarity-classVector->getThreshold()) < Trainer::epsilon){
			if(documentVector.isPositiveExample()){
				TP++;
			}
			else{
				TN++;
			}
		}
		else if(similarity > classVector->getThreshold()){
			if(documentVector.isPositiveExample()){
				TP++;
			}
			else{
				FP++;
			}
		}
		else if(similarity < classVector->getThreshold()){
			if(documentVector.isPositiveExample()){
				FN++;
			}
			else{
				TN++;
			}
		}
	}

	std::cout << "F1="<<computeF1()<<"\tTP="<<TP<<"\tFN="<<FN<<"\tFP="<<FP<<"\tTN="<<TN<<std::endl;
	fprintf(outTxt,"F1=%f\tTP=%lu\tFN=%lu\tFP=%lu\tTN=%lu\n",computeF1(),TP,FN,FP,TN);

	//Evaluations
	PrecisionRecallEvaluation * pre = new PrecisionRecallEvaluation(TP,FN,FP,TN);

	ScoredPrecisionRecallEvaluation * spre = new ScoredPrecisionRecallEvaluation();
	std::vector<SimilarityNode>::iterator iter2;
	for(iter2=sNodeList->begin(); iter2!=sNodeList->end(); iter2++){
		SimilarityNode & sNode = *iter2;
		spre->addCase(sNode.getDocumentVector()->isPositiveExample(),sNode.getSimilarity());
	}

	std::cout << "\n\tDetailed Evaluation\n";
	std::cout << "---------------------------------------\n";
	std::cout << pre->toString() << std::endl;
	std::cout << spre->toString() << std::endl;
	fprintf(outTxt,"\n\tDetailed Evaluation\n");
	fprintf(outTxt,"---------------------------------------\n");
	fprintf(outTxt,"%s\n",pre->toString().c_str());
	fprintf(outTxt,"%s\n",spre->toString().c_str());

	delete pre;
	delete spre;
	delete sNodeList;

	wallEndTime = getRuntime();

	printf("\nWallTime=%.2f sec\n\n", (wallEndTime-wallStartTime)/100.0);
	fprintf(outTxt,"\nWallTime=%.2f sec\n\n", (wallEndTime-wallStartTime)/100.0);

	fclose(outTxt);
	fclose(outPred);
}

/**
 * Initalizes this Evaluator
 */
void Tester::initialize(){
	outTxt = fopen(options->at("outTxt").c_str(),"w");
	outPred = fopen(options->at("outPred").c_str(),"w");
	testFeaturesIds = new std::set<unsigned long>();
	numberOfTestExamples = new unsigned long[1];
	numberOfTestExamples[0] = 0;
	maxTestFeaturesId = new unsigned long[1];
	maxTestFeaturesId[0] = 0;
}

/**
 * Gets the current Runtime
 */
double Tester::getRuntime(){
	/* returns the current processor time in hundredth of a second */
	clock_t start;
	start = clock();
	return((double)start/((double)(CLOCKS_PER_SEC)/100.0));
}

/**
 * Computes the current F1
 */
double Tester::computeF1(){
	double P = 0.0;
	double R = 0.0;
	double F1 = 0.0;
	if(TP+FP!=0){
		P = (double)TP / (double)(TP+FP);
	}
	if(TP+FN!=0){
		R = (double)TP / (double)(TP+FN);
	}
	if(P+R!=0.0){
		F1 = 2.0*P*R / (P+R);
	}
	return F1;
}
