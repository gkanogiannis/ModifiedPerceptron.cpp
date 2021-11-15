/**
 * @file PrecisionRecallEvaluation.h
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

#ifndef PRECISIONRECALLEVALUATION_H_
#define PRECISIONRECALLEVALUATION_H_

#include 	<iostream>
#include 	<cmath>

class PrecisionRecallEvaluation {
private:
	/**
	 * Number of True Positives
	 */
	long mTP;

	/**
	 * Number of False Negatives
	 */
	long mFN;

	/**
	 * Number of False Positives
	 */
	long mFP;

	/**
	 * Number of True Negatives
	 */
    long mTN;
public:
    /**
     * Default constructor
     */
	PrecisionRecallEvaluation();

	/**
	 * Constructor using values
	 */
	PrecisionRecallEvaluation(long, long, long, long);

	/**
	 * Default destructor
	 */
	~PrecisionRecallEvaluation();

	/**
	 * True Positives getter
	 */
	long truePositive() {return mTP;}

	/**
	 * False NEgatives getter
	 */
	long falseNegative() {return mFN;}

	/**
	 * False Positives getter
	 */
	long falsePositive() {return mFP;}

	/**
	 * True Negatives getter
	 */
	long trueNegative() {return mTN;}

	/**
	 * Returns total number of decisions
	 */
	long total() {return mTP + mFP + mTN + mFN;}

	/**
	 * Returns Correct Response
	 */
	long correctResponse() {return truePositive() + trueNegative();}

	/**
	 * Returns Positive Reference
	 */
	long positiveReference() {return truePositive() + falseNegative();}

	/**
	 * Return Positive Response
	 */
	long positiveResponse() {return truePositive() + falsePositive();}

	/**
	 * Returns Negative Reference
	 */
	long negativeReference() {return trueNegative() + falsePositive();}

	/**
	 * Returns Negative Response
	 */
	long negativeResponse() {return trueNegative() + falseNegative();}

	/**
	 * Returns Precision
	 */
	double precision() {
		if(positiveResponse()==0) return 0.0;
		return (double)truePositive() / (double)positiveResponse();
	}

	/**
	 * Returns Recall
	 */
	double recall() {
		if(positiveReference()==0) return 0.0;
		return (double)truePositive() / (double)positiveReference();
	}

	/**
	 * Returns Accuracy
	 */
	double accuracy() {return (double)correctResponse() / (double)total();}

	/**
	 * Returns F1
	 */
	double static fMeasure(double beta, double recall, double precision) {
        if(recall==0.0 && precision==0.0) return 0.0;
		double betaSq = beta * beta;
        return (1.0 + betaSq) * recall * precision
            / (recall + (betaSq*precision));
    }

	/**
	 * Returns CHI^1
	 */
	double chiSquared() {
        double tp = truePositive();
        double tn = trueNegative();
        double fp = falsePositive();
        double fn = falseNegative();
        double tot = total();
        double diff = tp * tn - fp * fn;
        return tot * diff * diff
            / ((tp + fn) * (fp + tn) * (tp + fp) * (fn + tn));
    }

	/**
	 * Returns PHI^2
	 */
	double phiSquared() {return chiSquared() / (double) total();}

	/**
	 * Returns Accuracy Deviation
	 */
	double accuracyDeviation() {
        // e.g. p = 0.05 for a 5% conf interval
        double p = accuracy();
        double tot = total();
        double variance = p * (1.0 - p) / tot;
        return sqrt(variance);
    }

	/**
	 * Adds a new case
	 */
	void addCase(bool, bool);

	/**
	 * Returns a string description
	 */
	std::string toString();
};

#endif
