/**
 * @file DocumentClassVectorUtils.h
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
 * DocumentClassVectorUtils class defines utility methods for DocumentVector and ClassVector
 * DocumentsReader class defines utility methods for reading DocumentVectors
 */

#ifndef DOCUMENTCLASSVECTORUTILS_H_
#define DOCUMENTCLASSVECTORUTILS_H_

#include 	<datastructures/DataVectors.h>
#include 	<utils/Utils.h>
#include 	<utils/StringTokenizer.h>

#include 	<iostream>
#include 	<fstream>
#include	<set>
#include 	<sstream>

class DocumentClassVectorUtils {
public:
	/**
	 * Reads DocumentVectors from a file into a DocsLIST
	 */
	static DocsLIST * getDocumentVectors(std::string *, unsigned long, unsigned long *, std::set<unsigned long> *, unsigned long *);
	static DocsLIST * getDocumentVectors2(std::string *, unsigned long, unsigned long *, std::set<unsigned long> *, unsigned long *);

	/**
	 * Parses a DocumentVector from a string
	 */
	static DocumentVector * parseDocumentVectorFromString(std::string *, std::set<unsigned long> *, unsigned long *);

	/**
	 * Saves a ClassVector to a file
	 */
	static void saveClassVector(ClassVector &, FILE *);

	/**
	 * Reads a ClassVector from a file
	 */
	static ClassVector * getClassVector(std::string *);

	/**
	 * Parses a ClassVector from a string
	 */
	static ClassVector * parseClassVectorFromString(std::string *);
};

class DocumentsReader {
private:
	/**
	 * Input file name
	 */
	std::string * input;

	/**
	 * Chunks of memory to use
	 */
	unsigned int lowmemChunk;

	/**
	 * Data read
	 */
	char * data;

	/**
	 * File stream
	 */
	FILE * stream;

	/**
	 * Stream position
	 */
	long position;

	/**
	 * Stream length
	 */
	long maxposition;

	/**
	 * Reach the EOF
	 */
	bool eof;

	/**
	 * Initializes a DocumentsReader class
	 */
	void initialize();
public:
	/**
	 * Default constructor
	 */
	DocumentsReader();

	/**
	 * Constructor using input file name and memory chunks
	 */
	DocumentsReader(std::string *, unsigned int);

	/**
	 * Default destructor
	 */
	~DocumentsReader();

	/**
	 * Stream has more data
	 */
	bool hasMore();

	/**
	 * Reads a single byte from the input stream
	 */
	char nextChar();
};

#endif
