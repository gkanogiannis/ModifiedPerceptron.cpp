/**
 * @file ScoredPrecisionRecallEvaluation.h
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

#ifndef SCOREDPRECISIONRECALLEVALUATION_H_
#define SCOREDPRECISIONRECALLEVALUATION_H_

#include 	<iostream>
#include 	<set>
#include 	<sstream>
#include 	<vector>

class Case;

/**
 * Set of Cases struct
 */
typedef std::set<Case, std::greater<Case> > MCASES;

//typedef std::priority_queue<Case,std::vector<Case>,std::greater<Case> > MCASES;

class ScoredPrecisionRecallEvaluation {
private:
	/**
	 * Cases in this evaluation
	 */
	MCASES mCases;

	/**
	 * Negative Reference
	 */
	long mNegativeRef;

	/**
	 * Positive Reference
	 */
    long mPositiveRef;
public:
    /**
     * Default constructor
     */
	ScoredPrecisionRecallEvaluation();

	/**
	 * Default destructor
	 */
	virtual ~ScoredPrecisionRecallEvaluation();

	/**
	 * Adds a new Case
	 */
	void addCase(bool, double);

	/**
	 * Returns a Precision/Recall Curve
	 */
	std::vector<std::vector<double> > * prCurve(bool);

	/**
	 * Returns a Receiver Operator Characteristic Curve
	 */
	std::vector<std::vector<double> > * rocCurve(bool);

	/**
	 * Returns the Precision/Recall Break Even Point
	 */
	double prBreakevenPoint();

	/**
	 * Returns the Area Under the PR Curve
	 */
	double areaUnderPrCurve(bool);

	/**
	 * Returns the Area Under the ROC Curve
	 */
	double areaUnderRocCurve(bool);

	/**
	 * Returns the Area Under a Curve
	 */
	double areaUnder(std::vector<std::vector<double> > *);

	/**
	 * Interpolates a Curve
	 */
	std::vector<std::vector<double> > * interpolate(std::vector<std::vector<double> > *,bool);

	/**
	 * Returns the Average Precision
	 */
	double averagePrecision();

	/**
	 * Returns the Maximum F1
	 */
	double maximumFMeasure(double);

	/**
	 * Returns a string description
	 */
	std::string toString();
};

class Case{
private:
	/**
	 * Correct classification
	 */
	bool mCorrect;

	/**
	 * Score
	 */
	double mScore;
public:
	/**
	 * Contructor using values
	 */
	Case(bool correct, double score) {mCorrect = correct;mScore = score;}

	/**
	 * Greater comparator
	 */
	bool operator> (Case const & thatCase) const {
		return (mScore>thatCase.getScore());
	}

	/**
	 * Less comparator
	 */
	bool operator< (Case const & thatCase) const {
		return(mScore<thatCase.getScore());
	}

	/**
	 * correct getter
	 */
	bool getCorrect() const {return mCorrect;}

	/**
	 * score getter
	 */
	double getScore() const {return mScore;}

	/**
	 * Returns a string description
	 */
	std::string const toString() const {
		std::stringstream ss;
		ss << mCorrect << " : " << mScore;
		return ss.str();
	}
};

#endif
