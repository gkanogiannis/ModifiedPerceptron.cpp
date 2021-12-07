/*
 *
 * MoPe_test
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

#include 	<core/Tester.h>
#include 	<core/Trainer.h>
#include 	<tclap/CmdLine.h>

#include 	<iostream>
#include 	<string>
#include 	<map>

int main(int argc, char **argv){
	TCLAP::CmdLine cmd("Command description message", ' ', "1.0");

	TCLAP::ValueArg<std::string> inDataA("i","inData","Test data location(test svmf file).",true,"NULL","string");
	TCLAP::ValueArg<std::string> inModelA("m","inModel","Train data location(train model file).",true,"NULL","string");
	TCLAP::ValueArg<std::string> outPredA("o","outPred","Output data location(predictions file).",false,"NULL","string");
	TCLAP::ValueArg<std::string> outTxtA("x","outTxt","Text file for the results.",false,"NULL","string");
	TCLAP::SwitchArg verboseA("v","verbose","Verbose messages.", false);
	TCLAP::ValueArg<std::string> lowmemA("l","lowmem","Use less memory, but slower parsing.(chunks in MBs)",false,"0","int");

	cmd.add(inDataA);
	cmd.add(inModelA);
	cmd.add(outPredA);
	cmd.add(outTxtA);
	cmd.add(verboseA);
	cmd.add(lowmemA);
	cmd.parse(argc,argv);

	std::string inData = inDataA.getValue();
	std::string inModel = inModelA.getValue();
	std::string outPred = outPredA.getValue();
	std::string outTxt = outTxtA.getValue();
	bool verbose = verboseA.getValue();
	std::string lowmem = lowmemA.getValue();

	if(outPred == "NULL"){
		if(inData.find(".test.svmf",0)>0){
			outPred = inData.substr(0,inData.find(".test.svmf",0)).append(".mopecpp.pred");
		}
		else if(inData.find(".svmf",0)>0){
			outPred = inData.substr(0,inData.find(".svmf",0)).append(".mopecpp.pred");
		}
		else{
			outPred = inData.append(".mopecpp.pred");
		}
	}
	if(outTxt == "NULL"){
		if(inData.find(".test.svmf",0)>0){
			outTxt = inData.substr(0,inData.find(".test.svmf",0)).append(".test.mopecpp.txt");
		}
		else if(inData.find(".svmf",0)>0){
			outTxt = inData.substr(0,inData.find(".svmf",0)).append(".test.mopecpp.txt");
		}
		else{
			outTxt = inData.append(".test.mopecpp.txt");
		}
	}

	std::map<std::string, std::string> options;
	options["inData"] = inData;
	options["inModel"] = inModel;
	options["outPred"] = outPred;
	options["outTxt"] = outTxt;
	options["verbose"] = verbose? "true":"false";
	options["lowmem"] = lowmem;

	Tester * evaluator = new Tester(&options);
	evaluator->startOperation();

	return 0;
}
