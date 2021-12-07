/**
 * @file ScoredPrecisionRecallEvaluation.cpp
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
 * ScoredPrecisionRecallEvaluation class evaluates the scored precison and recall
 */

#include 	<utils/ScoredPrecisionRecallEvaluation.h>
#include 	<utils/PrecisionRecallEvaluation.h>

#include 	<cmath>
#include 	<iostream>
#include 	<list>
#include 	<queue>
#include 	<vector>
#include 	<sstream>
#include	<limits>

/**
 * Default constructor
 */
ScoredPrecisionRecallEvaluation::ScoredPrecisionRecallEvaluation(){
	mNegativeRef = 0l;
	mPositiveRef = 0l;
}

/**
 * Default destructor
 */
ScoredPrecisionRecallEvaluation::~ScoredPrecisionRecallEvaluation(){
	mCases.clear();
}

/**
 * Adds a new Case
 */
void ScoredPrecisionRecallEvaluation::addCase(bool correct, double score) {
	Case c(correct,score);
	mCases.insert(c);
	if (correct) ++mPositiveRef;
	else ++mNegativeRef;
}

/**
 * Returns a Precision/Recall Curve
 */
std::vector<std::vector<double> > * ScoredPrecisionRecallEvaluation::prCurve(bool interpl) {
	PrecisionRecallEvaluation * eval = new PrecisionRecallEvaluation();
	std::vector<std::vector<double> > * prList = new std::vector<std::vector<double> >(0);
	MCASES::iterator it;
	for(it=mCases.begin(); it!=mCases.end(); it++){
		Case const & c = *it;
		bool correct = c.getCorrect();
		eval->addCase(correct,true);
		if(correct) {
			double r = (double)eval->truePositive() / (double)mPositiveRef;
			double p = eval->precision();
			std::vector<double> pr(0);
			pr.push_back(r);
			pr.push_back(p);
			prList->push_back(pr);
		}
	}
	delete eval;
	return interpolate(prList,interpl);
}

/**
 * Returns a Receiver Operator Characteristic Curve
 */
std::vector<std::vector<double> > * ScoredPrecisionRecallEvaluation::rocCurve(bool interpl) {
	PrecisionRecallEvaluation * eval = new PrecisionRecallEvaluation();
	std::vector<std::vector<double> > * prList = new std::vector<std::vector<double> >(0);
	MCASES::iterator it;
	for(it=mCases.begin(); it!=mCases.end(); it++){
		Case const & c = *it;
		bool correct = c.getCorrect();
		eval->addCase(correct,true);
		if(correct){
			double r = (double)eval->truePositive() / (double)mPositiveRef;
			double rr = 1.0f - ((double)eval->falsePositive() / (double)mNegativeRef);
			std::vector<double> pr(0);
			pr.push_back(r);
			pr.push_back(rr);
			prList->push_back(pr);
		}
	}
	delete eval;
	return interpolate(prList,interpl);
}

/**
 * Returns the Precision/Recall Break Even Point
 */
double ScoredPrecisionRecallEvaluation::prBreakevenPoint() {
	std::vector<std::vector<double> > * pr = prCurve(false);
	for (unsigned int i = 0; i < pr->size(); ++i)
		if (pr->at(i)[0] > pr->at(i)[1]){
			double bep = pr->at(i)[1];
			delete pr;
			return bep;
		}
	return 0.0f;
}

/**
 * Returns the Area Under the PR Curve
 */
double ScoredPrecisionRecallEvaluation::areaUnderPrCurve(bool interpolate) {
	return areaUnder(prCurve(interpolate));
}

/**
 * Returns the Area Under the ROC Curve
 */
double ScoredPrecisionRecallEvaluation::areaUnderRocCurve(bool interpolate) {
	return areaUnder(rocCurve(interpolate));
}

/**
 * Returns the Area Under a Curve
 */
double ScoredPrecisionRecallEvaluation::areaUnder(std::vector<std::vector<double> > * zeroOneStepFunction) {
	double area = 0.0;
	double lastX = 0.0;
	for (unsigned int i = 0; i < zeroOneStepFunction->size(); ++i) {
		double x = zeroOneStepFunction->at(i)[0];
		double height = zeroOneStepFunction->at(i)[1];
		double width = x - lastX;
		area += width * height; // step function
		lastX = x;
	}
	delete zeroOneStepFunction;
	return area;
}

/**
 * Returns the Average Precision
 */
double ScoredPrecisionRecallEvaluation::averagePrecision(){
	std::vector<std::vector<double> > * pr = prCurve(false);
	double sum = 0.0;
	for(unsigned int i = 0; i < pr->size(); i++){
		sum += pr->at(i)[1];
	}
	sum = sum / ((double)pr->size());
	delete pr;
	return sum;
}

/**
 * Returns the Maximum F1
 */
double ScoredPrecisionRecallEvaluation::maximumFMeasure(double beta){
	double maxF = 0.0;
	std::vector<std::vector<double> > * pr = prCurve(false);
	for(unsigned int i = 0; i < pr->size(); i++) {
		double f = PrecisionRecallEvaluation::fMeasure(beta,pr->at(i)[0],pr->at(i)[1]);
		maxF = std::max(maxF,f);
	}
	delete pr;
	return maxF;
}

/**
 * Interpolates a Curve
 */
std::vector<std::vector<double> > * ScoredPrecisionRecallEvaluation::interpolate(std::vector<std::vector<double> > * prList,bool interpl) {
	if(!interpl){
		return prList;
	}
	else{
		std::vector<std::vector<double> > * rev = new std::vector<std::vector<double> >(0);
		//Reverse
		for(int i=prList->size()-1; i>=0; i--){
			std::vector<double> & rp = prList->at(i);
			rev->push_back(rp);
		}
		delete prList;

		std::list<std::vector<double> > * llist = new std::list<std::vector<double> >(0);
		double maxP = std::numeric_limits<double>::min();
		for(unsigned int i=0; i<rev->size(); i++){
			std::vector<double> & rp = rev->at(i);
			double p = rp[1];
			 if (maxP < p) {
				 maxP = p;
				 llist->push_front(rp);
			 }
		}
		delete rev;

		std::vector<std::vector<double> > * ret = new std::vector<std::vector<double> >(llist->size());
		std::list<std::vector<double> >::iterator iter;
		for(iter=llist->begin(); iter!=llist->end(); iter++){
			std::vector<double> & rp = *iter;
			ret->push_back(rp);
		}
		delete llist;

		return ret;
	}
}

/**
 * Returns a string description
 */
std::string ScoredPrecisionRecallEvaluation::toString() {
	std::stringstream ss;
	ss << "BEP (PR break even point)="<<prBreakevenPoint()*100.0<< '\n';
	ss << "Area Under ROC Curve     ="<<areaUnderRocCurve(false)*100.0<< '\n';
	ss << "Area Under PR Curve      ="<<areaUnderPrCurve(false)*100.0<< '\n';
	ss << "Average Precision        ="<<averagePrecision()*100.0;

	//ss << "Area Under ROC Curve (interpolated)="<<areaUnderRocCurve(true)<< '\n';
	//ss << "Area Under PR Curve (interpolated)="<<areaUnderPrCurve(true)<< '\n';
    //ss << "Maximum F(1) Measure="<<maximumFMeasure(1.0)<< '\n';
    //ss << "Reciprocal Rank="<<reciprocalRank()<< '\n';
    //int[] ranks = new int[] { 5, 10, 25, 100, 500 };
    //for (int i = 0; i < ranks.length && mCases->size() < ranks[i]; ++i)
    	//ss << "Precision at "<<ranks[i]<<"="<<precisionAt(ranks[i])<< '\n';
    return ss.str();
}
