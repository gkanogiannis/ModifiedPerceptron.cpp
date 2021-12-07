/**
 * @file DataVectors.h
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
 * DataVectors class defines ClassVector class and DocumentVector class
 */

#ifndef DATAVECTORS_H_
#define DATAVECTORS_H_

#include 	<datastructures/VectorNode.h>

#include 	<list>
#include 	<queue>
#include	<string>
#include 	<vector>

/**
 * Type definitions for DocumentVector and ClassVector VectorNodes
 */
typedef std::vector<VectorNode> DocNODES;
typedef std::vector<VectorNode> ClsNODES;

class ClassVector;

/**
 * Class to represent a document
 */
class DocumentVector {
private:
	/**
	 * docId of the document
	 */
	unsigned long docId;

	/**
	 * It is a positive example of a class
	 */
	bool positiveExample;

	/**
	 * It was correclty classified to a class
	 */
	bool correctlyClassified;

	/**
	 * The VectorNodes of the document representation
	 */
	DocNODES * nodes;
public:
	/**
	 * Default constructor
	 */
	DocumentVector();

	void clear();

	/**
	 * Constructor using another DocumentVector
	 */
	DocumentVector(const DocumentVector &);

	/**
	 * Constructor using another ClassVector
	 */
	DocumentVector(const ClassVector &);

	/**
	 * Default destructor
	 */
	~DocumentVector();

	/**
	 * Computes the second order norm of this DocumentVector
	 */
	double const computeNorm();

	/**
	 * Normalizes this DocumentVector
	 */
	void normalize(double);

	/**
	 * docId getter
	 */
	unsigned long getDocId() const {return docId;}

	/**
	 * docId setter
	 */
	void setDocId(unsigned long docId) {this->docId = docId;}

	/**
	 * positiveExample getter
	 */
	bool isPositiveExample() const {return positiveExample;}

	/**
	 * positiveExample setter
	 */
	void setPositiveExample(bool positiveExample) {this->positiveExample = positiveExample;}

	/**
	 * correctlyClassified getter
	 */
	bool isCorrectlyClassified() const {return correctlyClassified;}

	/**
	 * correctlyClassified setter
	 */
	void setCorrectlyClassified(bool correctlyClassified) {this->correctlyClassified = correctlyClassified;}

	/**
	 * nodes getter
	 */
	DocNODES & getNodes() const {return *nodes;}

	/**
	 * nodes setter
	 */
	void setNodes(DocNODES & nodes) {this->nodes = &nodes;}
};

/**
 * Class to represent a class(category)
 */
class ClassVector {
private:
	/**
	 * The name of the class
	 */
	std::string name;

	/**
	 * The number of positive examples in this class
	 */
	unsigned long size;

	/**
	 * The threshold value for this class
	 */
	double threshold;

	/**
	 * The positive DocumentVector for the threshold definition
	 */
    DocumentVector * posThresholdVector;

    /**
     * The negative DocumentVector for the threshold definition
     */
	DocumentVector * negThresholdVector;

	/**
	 * The VectorNodes of the class representation
	 */
	ClsNODES * nodes;
public:
	/**
	 * Default constructor
	 */
	ClassVector();

	void clear();

	/**
	 * Constructor using another ClassVector
	 */
	ClassVector(ClassVector &);

	void replace(const ClassVector &);

	/**
	 * Default destructor
	 */
	~ClassVector();

	/**
	 * Add this ClassVector with a DocumentVector
	 */
	void addWithDocumentVector(DocumentVector &, double);

	/*
	 * Add this ClassVector with another ClassVector
	 */
	void addWithClassVector(ClassVector &, double);

	/**
	 * Compute the second order norm of this ClassVector
	 */
	double const computeNorm();

	/**
	 * Normalizes this ClassVector
	 */
	void normalize(double);

	/**
	 * Multiply this ClassVector
	 */
	void multiply(double);

	/**
	 * Computes the similarity of this ClassVector with a DocumentVector
	 */
	double const similarityWithDocumentVector(DocumentVector &);

	/**
	 * Computes the similarity of this ClassVector with another ClassVector
	 */
	double const similarityWithClassVector(ClassVector &);

	/**
	 * name getter
	 */
    std::string getName() const {return name;}

    /**
     * name setter
     */
    void setName(std::string name){this->name = name;}

    /**
     * size getter
     */
    unsigned long getSize() const {return size;}

    /**
     * size setter
     */
    void setSize(unsigned long size){this->size = size;}

    /**
     * threshold getter
     */
    double getThreshold() const {return threshold;}

    /**
     * threshold setter
     */
    void setThreshold(double threshold){this->threshold = threshold;}

    /**
     * posThresholdVector getter
     */
    DocumentVector & getPosThresholdVector() const {return *posThresholdVector;}

    /**
     * nosThresholdVector setter
     */
    void setPosThresholdVector(DocumentVector & posThresholdVector){
    	if(this->posThresholdVector!=0){
    		delete this->posThresholdVector;
        	this->posThresholdVector = &posThresholdVector;
    	}
    	else{
    		this->posThresholdVector = 0;
    	}
    }

    /**
     * negThresholdVector getter
     */
    DocumentVector & getNegThresholdVector() const {return *negThresholdVector;}

    /**
     * negThresholdVector setter
     */
    void setNegThresholdVector(DocumentVector & negThresholdVector){
    	if(this->negThresholdVector!=0){
    		delete this->negThresholdVector;
    		this->negThresholdVector = &negThresholdVector;
    	}
    	else{
    		this->negThresholdVector = 0;
    	}
    }

    /**
     * nodes getter
     */
    ClsNODES & getNodes() const {return *nodes;}

    /**
     * nodes setter
     */
    void setNodes(ClsNODES & nodes) {this->nodes = &nodes;}
};

#endif
