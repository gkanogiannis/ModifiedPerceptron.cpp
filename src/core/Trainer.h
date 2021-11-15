/**
 * @file Trainer.h
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

#ifndef TRAINER_H_
#define TRAINER_H_

#include 	<datastructures/DataVectors.h>
#include 	<datastructures/SimilarityNode.h>
#include	<utils/Utils.h>

#include	<cstdio>
#include 	<map>
#include	<set>

class Trainer {
private:
	/**
	 * Number of train examples
	 */
	unsigned long * numberOfTrainExamples;

	/**
	 * Train terms
	 */
	std::set<unsigned long> * trainFeaturesIds;

	/**
	 * Max train feature id
	 */
	unsigned long * maxTrainFeaturesId;

	/*
	 * Output model location
	 */
	FILE * outModel;

	/**
	 * Output txt file
	 */
	FILE * outTxt;

	/*
	 * List holding the DocumentVectors for the current ClassVector
	 */
	DocsLIST * documentVectors;

	/*
	 * ClassVector for the current category
	 */
	ClassVector * classVector;

	/**
	 * User defined command line options
	 */
	std::map<std::string, std::string> * options;

	/**
	 * Initalizes this Trainer
	 */
	void initialize();

	/**
	 * Gets the current Runtime
	 */
	double getRuntime();
public:
	/**
	 * Epsilon value for comparisons
	 */
	static const double epsilon;

	/**
	 * Constructor using user defined command line options
	 */
	Trainer(std::map<std::string,std::string> *);

	/**
	 * Default destructor
	 */
	~Trainer();

	/**
	 * Starts the training process
	 */
	void startOperation();
};

#endif
