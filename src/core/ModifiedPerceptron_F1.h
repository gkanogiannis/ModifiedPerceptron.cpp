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
