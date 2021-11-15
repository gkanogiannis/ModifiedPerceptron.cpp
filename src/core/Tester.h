/**
 * @file Evaluator.h
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

#ifndef EVALUATOR_H_
#define EVALUATOR_H_

#include 	<core/Trainer.h>

#include  	<map>
#include	<set>

class Tester {
private:
	/*
	 * Number of testing examples
	 */
	unsigned long * numberOfTestExamples;

	/**
	 * Testing terms
	 */
	std::set<unsigned long> * testFeaturesIds;

	/**
	 * Max testing feature id
	 */
	unsigned long * maxTestFeaturesId;

	/**
	 * Output predictions location
	 */
	FILE * outPred;

	/**
	 * Output txt file
	 */
	FILE * outTxt;

	/**
	 * List holding the DocumentVectors for the current ClassVector
	 */
	DocsLIST * documentVectors;

	/**
	 * ClassVector for the current class
	 */
	ClassVector * classVector;

	/**
	 * True Positives
	 */
	unsigned long TP;//a

	/**
	 * False Positives
	 */
	unsigned long FP;//b

	/**
	 * False Negatives
	 */
	unsigned long FN;//c

	/**
	 * True Negatives
	 */
	unsigned long TN;//d

	/**
	 * User defined command line options
	 */
	std::map<std::string, std::string> * options;

	/**
	 * Initalizes this Evaluator
	 */
	void initialize();

	/**
	 * Computes the current F1
	 */
	double computeF1();

	/**
	 * Gets the current Runtime
	 */
	double getRuntime();
public:
	/**
	 * Constructor using user defined command line options
	 */
	Tester(std::map<std::string,std::string> *);

	/**
	 * Default destructor
	 */
	virtual ~Tester();

	/**
	 * Starts the evaluating(testing) process
	 */
	void startOperation();
};

#endif
