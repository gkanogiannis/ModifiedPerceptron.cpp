/**
 * @file DocumentClassVectorUtils.cpp
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

#include 	<core/Trainer.h>
#include 	<utils/DocumentClassVectorUtils.h>

#include 	<algorithm>
#include	<limits>
#include	<set>

/**
 * Reads DocumentVectors from a file into a DocsLIST
 */
DocsLIST * DocumentClassVectorUtils::getDocumentVectors(std::string * fileName, unsigned long lowmemChunk, unsigned long * numberOfExamples, std::set<unsigned long> * featuresIds, unsigned long * maxFeaturesId){
	DocsLIST * ret = new DocsLIST();
	DocumentsReader docReader(fileName,lowmemChunk);
	DocumentVector * dv = new DocumentVector();
	double docClass;
	unsigned long termId;
	double termWeight;
	std::stringstream ss(std::stringstream::in | std::stringstream::out);
	bool waitingDocClass = true;
	bool waitingNodes = false;
	bool termIdOk = false;
	bool termWeightOk = false;
	bool ignore = false;
	while(docReader.hasMore()){
		//Read a single character
		char c = docReader.nextChar();
		//Ignoring character
		if(ignore && c!='\n') continue;
		//If a newline character
		//else if(ignore && c=='\n'){
			//ignore = false;
		//}
		if(c=='\n'){
			if(!termWeightOk){
				ss >> termWeight; ss.str(""); ss.clear();
				//VectorNode * vn = new VectorNode(termId,termWeight);
				if(termId > maxFeaturesId[0]){
					maxFeaturesId[0] = termId;
				}
				featuresIds->insert(termId);
				dv->getNodes().push_back(VectorNode(termId,termWeight));
			}
			double norm = dv->computeNorm();
			dv->normalize(norm);
			numberOfExamples[0] = numberOfExamples[0] + 1;
			dv->setDocId(numberOfExamples[0]);
			ret->push_back(*dv);
			//std::cout << "Read " << dv.getDocId() << std::endl;
			dv->clear();
			waitingDocClass = true;
			waitingNodes = false;
			termIdOk = false;
			termWeightOk = false;
			ignore = false;
			continue;
		}
		//If a : character
		else if(c==':'){
			ss >> termId; ss.str(""); ss.clear();
			termIdOk = true;
			termWeightOk = false;
			continue;
		}
		else if(c=='#'){
			ignore = true;
			continue;
		}
		//If not a space character, add it to the buffer.
		else if(c!=' ' && c!='\t'){
			ss.put(c);
			continue;
		}
		//If a space character.
		else{
			if(waitingDocClass){
				ss >> docClass; ss.str(""); ss.clear();
				if(docClass >= 0.0) 	dv->setPositiveExample(true);
				else				dv->setPositiveExample(false);
				waitingDocClass = false;
				waitingNodes = true;
				continue;
			}
			else if(waitingNodes && termIdOk){
				ss >> termWeight; ss.str(""); ss.clear();
				if(termId > maxFeaturesId[0]){
					maxFeaturesId[0] = termId;
				}
				featuresIds->insert(termId);
				dv->getNodes().push_back(VectorNode(termId,termWeight));
				termWeightOk = true;
				termIdOk = false;
				continue;
			}
		}
	}
	if(waitingNodes){
		if(!termWeightOk){
			ss >> termWeight; ss.str(""); ss.clear();
			//VectorNode * vn = new VectorNode(termId,termWeight);
			if(termId > maxFeaturesId[0]){
				maxFeaturesId[0] = termId;
			}
			featuresIds->insert(termId);
			dv->getNodes().push_back(VectorNode(termId,termWeight));
		}
		double norm = dv->computeNorm();
		dv->normalize(norm);
		dv->setDocId(numberOfExamples[0]);
		ret->push_back(*dv);
		dv->clear();
	}
	ss.str(""); ss.clear();

	return ret;
}

DocsLIST * DocumentClassVectorUtils::getDocumentVectors2(std::string * fileName, unsigned long lowmemChunk, unsigned long * numberOfExamples, std::set<unsigned long> * featuresIds, unsigned long * maxFeaturesId){
	DocsLIST * ret = new DocsLIST();
	std::ifstream file(fileName->c_str());
	std::string line;
	while( std::getline(file, line)){
		numberOfExamples[0] = numberOfExamples[0] + 1;
		DocumentVector * dv = parseDocumentVectorFromString(&line, featuresIds, maxFeaturesId);
		if(dv->getDocId() < 1){
			dv->setDocId(numberOfExamples[0]);
		}
		dv->normalize(dv->computeNorm());
		ret->push_back(DocumentVector(*dv));
		std::cout << "Read " << dv->getDocId() << std::endl;
		delete dv;
	}

	return ret;
}


/**
 * Parses a DocumentVector from a string
 */
DocumentVector * DocumentClassVectorUtils::parseDocumentVectorFromString(std::string * line, std::set<unsigned long> * featuresIds, unsigned long * maxFeaturesId){
	DocumentVector * ret = new DocumentVector();
	std::stringstream tokenizer(*line);
	std::string str;
	tokenizer >> str;
	unsigned int cls = Utils::stoa<unsigned int>(str);
	if(cls>0){
		ret->setPositiveExample(true);
	}
	else{
		ret->setPositiveExample(false);
	}
	while(tokenizer >> str){
		if(str == "#"){
			tokenizer >> str;
			unsigned long id = Utils::stoa<unsigned long>(str);
			ret->setDocId(id);
			break;
		}
		else{
			StringTokenizer tok = StringTokenizer(str,":");
			std::string stermId = tok.nextToken();
			unsigned long termId = Utils::stoa<unsigned long>(stermId);
			if(termId > maxFeaturesId[0]){
				maxFeaturesId[0] = termId;
			}
			featuresIds->insert(termId);
			std::string sweight = tok.nextToken();
			double weight = Utils::stoa<double>(sweight);
			ret->getNodes().push_back(VectorNode(termId,weight));
		}
	}

	return ret;
}

/**
 * Saves a ClassVector to a file
 */
void DocumentClassVectorUtils::saveClassVector(ClassVector & classVector, FILE * out){
	fprintf(out,"%lu\t",classVector.getNodes().size());
	ClsNODES::const_iterator iter;
	for(iter=classVector.getNodes().begin(); iter!=classVector.getNodes().end(); ++iter){
	//for(long i=0; i<size; i++){
		const VectorNode & node = *iter;
		//VectorNode * node = classVector->getNodes()[i];
		if(node.getTermId() < 1){
			continue;
		}
		unsigned long termId = node.getTermId();
		double termWeight = node.getTermWeight();
		fprintf(out,"%lu:%.16f\t",termId,termWeight);
	}
	std::string name = classVector.getName();
	std::replace(name.begin(),name.end(),' ','_');
	std::replace(name.begin(),name.end(),',','$');
	std::replace(name.begin(),name.end(),'-','%');
	fprintf(out,"\t# %s %lu %.16f\n",name.c_str(),(unsigned long)classVector.getSize(),classVector.getThreshold());
}

/**
 * Reads a ClassVector from a file
 */
ClassVector * DocumentClassVectorUtils::getClassVector(std::string * fileName){
	std::ifstream inData(fileName->c_str());
	std::string line;
	getline (inData,line);
	inData.close();
	return DocumentClassVectorUtils::parseClassVectorFromString(&line);
}

/**
 * Parses a ClassVector from a string
 */
ClassVector * DocumentClassVectorUtils::parseClassVectorFromString(std::string * line){
	ClassVector * ret = new ClassVector();
	std::stringstream tokenizer(*line);
	std::string str;
	tokenizer >> str;
	unsigned long length = Utils::stoa<unsigned long>(str);
	ClsNODES * nodes = new ClsNODES(length);
	//VectorNode ** nodes = new VectorNode*[length];
	//for(long i=0; i<length; i++){
		//nodes[i] = 0;
	//}
	ret->setNodes(*nodes);
	while(tokenizer >> str){
		if(str == "#"){
			std::string className;
			tokenizer >> className;
			Utils::trimString(className);
			std::replace(className.begin(),className.end(),'_',' ');
			std::replace(className.begin(),className.end(),'$',',');
			std::replace(className.begin(),className.end(),'%','-');
			ret->setName(className);
			std::string classSize;
			tokenizer >> classSize;
			Utils::trimString(classSize);
			ret->setSize(Utils::stoa<unsigned long>(classSize));
			std::string classThreshold;
			tokenizer >> classThreshold;
			Utils::trimString(classThreshold);
			ret->setThreshold(Utils::stoa<double>(classThreshold));
			break;
		}
		else{
			StringTokenizer tok = StringTokenizer(str,":");
			std::string stermId = tok.nextToken();
			unsigned long termId = Utils::stoa<unsigned long>(stermId);
			std::string sweight = tok.nextToken();
			double weight = Utils::stoa<double>(sweight);
			//VectorNode * node = new VectorNode(termId,weight);
			ret->getNodes().at(termId-1) = VectorNode(termId,weight);
			//classVector->getNodes()[termId-1] = node;
		}
	}

	return ret;
}

/**
 * Default constructor
 */
DocumentsReader::DocumentsReader(){}

/**
 * Constructor using input file name and memory chunks
 */
DocumentsReader::DocumentsReader(std::string * input, unsigned int lowmemChunk){
	this->input = input; this->lowmemChunk = lowmemChunk;
	initialize();
}

/**
 * Default destructor
 */
DocumentsReader::~DocumentsReader(){}

/**
 * Initializes a DocumentsReader class
 */
void DocumentsReader::initialize(){
	position = 0;
	maxposition = 0;
	eof = false;
	data = new char[1];
	if(lowmemChunk<=0){
		//stream = fopen64(input->c_str(),"r");
		stream = fopen(input->c_str(),"r");
		fseek(stream, 0L, SEEK_END);
		maxposition = ftell(stream);
		rewind (stream);
		delete data;
		data = new char [maxposition];
		fread(data, 1, maxposition, stream);
		fclose(stream);
		eof = true;
	}
	else{
		//stream = fopen64(input->c_str(),"r");
		stream = fopen(input->c_str(),"r");
	}
}

/**
 * Stream has more data
 */
bool DocumentsReader::hasMore(){
	if(lowmemChunk<=0){
		if(position<maxposition){
			return true;
		}
		else{
			delete data;
			return false;
		}
	}
	else{
		if(position<maxposition){
			return true;
		}
		else{
			delete data;
			if(eof){
				return false;
			}
			else{
				position = 0;
				data = new char[lowmemChunk * 1024 * 1024];
				long result = fread(data, 1, lowmemChunk * 1024 * 1024, stream);
				if(feof(stream)){
					maxposition = result;
					eof = true;
					fclose(stream);
				}
				else{
					maxposition = lowmemChunk * 1024 * 1024;
					eof = false;
				}
				return true;
			}
		}
	}
}

/**
 * Reads a single byte from the input stream
 */
char DocumentsReader::nextChar(){
	return data[position++];
}
