/**
 * @file PrecisionRecallEvaluation.cpp
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
 * PrecisionRecallEvaluation class evaluates the precison and recall
 */

#include 	<utils/PrecisionRecallEvaluation.h>

#include 	<sstream>

/**
 * Default constructor
 */
PrecisionRecallEvaluation::PrecisionRecallEvaluation(){
	mTP = mFN = mFP = mTN = 0l;
}

/**
 * Constructor using values
 */
PrecisionRecallEvaluation::PrecisionRecallEvaluation(long tp, long fn, long fp, long tn) {
	mTP = tp;
	mFN = fn;
	mFP = fp;
	mTN = tn;
}

/**
 * Default destructor
 */
PrecisionRecallEvaluation::~PrecisionRecallEvaluation(){}

/**
 * Adds a new case
 */
void PrecisionRecallEvaluation::addCase(bool reference, bool response) {
	if (reference && response) ++mTP;
	else if (reference && (!response)) ++mFN;
	else if ((!reference) && response) ++mFP;
	else ++mTN;
}

/**
 * Returns a string description
 */
std::string PrecisionRecallEvaluation::toString(){
	std::stringstream ss;
	//ss<<"Total=" << total() << "\n";
	//ss<<"True Positive=" << truePositive() << '\n';
	//ss<<"False Negative=" << falseNegative() << '\n';
	//ss<<"False Positive=" << falsePositive() << '\n';
	//ss<<"True Negative=" << trueNegative() << '\n';
	//ss<<"Positive Reference=" << positiveReference() << '\n';
	//ss<<"Positive Response=" << positiveResponse() << '\n';
	//ss<<"Negative Reference=" << negativeReference() << '\n';
	//ss<<"Negative Response=" << negativeResponse() << '\n';
	ss << "Accuracy                 =" << accuracy()*100.0 << '\n';
	ss << "Precision                =" << precision()*100.0 << '\n';
	ss << "Recall                   =" << recall()*100.0 << '\n';
	ss << "F(1)                     =" << fMeasure(1, recall(), precision())*100.0;
	//ss<<"chi Squared=" << chiSquared() << '\n';
	//ss<<"phi Squared=" << phiSquared() << '\n';
	//ss<<"Accuracy Deviation=" << accuracyDeviation();
	return ss.str();
}
