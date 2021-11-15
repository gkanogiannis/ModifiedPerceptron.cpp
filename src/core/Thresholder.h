/**
 * @file Thresholder.h
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

#ifndef THRESHOLDER_H_
#define THRESHOLDER_H_

#include 	<core/Trainer.h>
#include 	<datastructures/DataVectors.h>

class Thresholder {
private:
	/**
	 * The ClassVector
	 */
	ClassVector & classVector;

	/**
	 * List of training DocumentVectors
	 */
	DocsLIST & documentVectors;

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
	 * Minimum Heap for SimilarityNodes
	 */
	MINHEAP * minHeap;

	/**
	 * Maximum Heap for SimilarityNodes
	 */
	MAXHEAP * maxHeap;

	/**
	 * The positive threshold DocumentVector
	 */
	DocumentVector * posThresholdVector;

	/**
	 * The negative threshold DocumentVector
	 */
	DocumentVector * negThresholdVector;

	/**
	 * Initializes this Thresholder
	 */
	void initialize();

	/**
	 * Returns the current F1
	 */
	double computeF1();

	/**
	 * Clears this Thresholder
	 */
	void clear();
public:
	/**
	 * Constructor using values
	 */
	Thresholder(ClassVector &, DocsLIST &);

	/**
	 * Default destructor
	 */
	~Thresholder();

	/**
	 * Computes the threshold for this ClassVector and that mode
	 */
	void computeThreshold(int);
};

#endif
