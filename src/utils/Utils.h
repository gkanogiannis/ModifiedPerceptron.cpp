/**
 * @file Utils.h
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
 * Utils class defines various utility methods
 */

#ifndef UTILS_H_
#define UTILS_H_

#include 	<datastructures/SimilarityNode.h>

#include 	<sstream>
#include 	<string>
#include 	<vector>
#include 	<queue>

/**
 * List of DocumentVectors struct
 */
typedef std::vector<DocumentVector> DocsLIST;

/**
 * Minimum Heap of SimilarityNodes struct
 */
typedef std::priority_queue<SimilarityNode,std::vector<SimilarityNode>,SimilarityNode::greater_SimilarityNode > MINHEAP;

/**
 * Maximum Heap of SimilarityNodes struct
 */
typedef std::priority_queue<SimilarityNode,std::vector<SimilarityNode>,SimilarityNode::less_SimilarityNode > MAXHEAP;

class Utils {
public:
	/**
	 * Default constructor
	 */
	Utils();

	/**
	 * Default destructor
	 */
	virtual ~Utils();

	/**
	 * Splits a string into tokens
	 */
	static void SplitString(std::string const &str, std::vector<std::string> &tokens, std::string const &delimiters = " "){
		// Skip delimiters at beginning.
		std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
		// Find first "non-delimiter".
		std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

		while (std::string::npos != pos || std::string::npos != lastPos){
			// Found a token, add it to the vector.
			tokens.push_back(str.substr(lastPos, pos - lastPos));
			// Skip delimiters.  Note the "not_of"
			lastPos = str.find_first_not_of(delimiters, pos);
			// Find next "non-delimiter"
			pos = str.find_first_of(delimiters, lastPos);
		}
	}

	/**
	 * Joins a set of string into one string
	 */
	static std::string vectorOfStringsToString(std::vector<std::string> * vec){
		std::stringstream ss;
		ss<<"[";
		for(unsigned int i=0; i<vec->size(); i++){
			ss<<vec->at(i)<<",";
		}
		ss<<"]";
		return ss.str();
	}

	/**
	 * Trims a string
	 */
	static void trimString(std::string & str){
	  std::string::size_type pos = str.find_last_not_of(' ');
	  if(pos != std::string::npos) {
	    str.erase(pos + 1);
	    pos = str.find_first_not_of(' ');
	    if(pos != std::string::npos) str.erase(0, pos);
	  }
	  else str.erase(str.begin(), str.end());
	}

	/**
	 * Converts a string into Template T
	 */
	template <class T>
	static T stoa(const std::string & s){
		T t;
		std::istringstream iss(s);
		if (!(iss>>t))
			throw "Can't convert";
		return t;
	}
};

#endif
