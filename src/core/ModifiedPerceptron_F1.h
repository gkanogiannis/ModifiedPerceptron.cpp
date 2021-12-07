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

#ifndef ModifiedPerceptronF1_H_
#define ModifiedPerceptronF1_H_

#include 	<datastructures/DataVectors.h>
#include 	<utils/Utils.h>

#include 	<cstdio>
#include	<list>

class ModifiedPerceptron_F1 {
private:
	ClassVector & classVector;
	DocsLIST & documentVectors;
	unsigned int iterations;
	FILE * outTxt;
	bool verbose;
	bool silent;
	unsigned long TP;//a
	unsigned long FP;//b
	unsigned long FN;//c
	unsigned long TN;//d
	//MINHEAP * TPHeap;
	//MAXHEAP * FNHeap;
	//MINHEAP * FPHeap;
	//MAXHEAP * TNHeap;
	std::vector<SimilarityNode> * FPArray;
	std::vector<SimilarityNode> * FNArray;
	DocumentVector * FNCentroid;
	DocumentVector * FPCentroid;

	void initialize(unsigned int, FILE *, bool, bool);
	void fillHeaps();
	void computeMiscclasifiedCentroids();
	double computeF1();
	void clear();
public:
	ModifiedPerceptron_F1(ClassVector &, DocsLIST &, unsigned int, FILE *, bool, bool);
	~ModifiedPerceptron_F1();
	void modify();
};

#endif
