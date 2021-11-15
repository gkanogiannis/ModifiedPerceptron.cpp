#include	<core/Trainer.h>
#include 	<tclap/CmdLine.h>

#include 	<iostream>
#include 	<string>
#include 	<map>

int main(int argc, char **argv){
	TCLAP::CmdLine cmd("Command description message", ' ', "1.0");

	TCLAP::ValueArg<std::string> inDataA("i","inData","Train data location(train svmf file).",true,"NULL","string");
	TCLAP::ValueArg<std::string> outModelA("o","outModel","Output data location(model file).",false,"NULL","string");
	TCLAP::ValueArg<std::string> outTxtA("x","outTxt","Text file for the results.",false,"NULL","string");
	TCLAP::ValueArg<std::string> iterA("t","iter","Number of iterations.",false,"100","int");
	TCLAP::SwitchArg verboseA("v","verbose","Verbose messages.", false);
	TCLAP::SwitchArg silentA("s","silent","No messages at all.", false);
	TCLAP::ValueArg<std::string> lowmemA("l","lowmem","Use less memory, but slower parsing.(chunks in MBs)",false,"0","int");

	cmd.add(inDataA);
	cmd.add(outModelA);
	cmd.add(outTxtA);
	cmd.add(iterA);
	cmd.add(verboseA);
	cmd.add(silentA);
	cmd.add(lowmemA);
	cmd.parse(argc,argv);

	std::string inData = inDataA.getValue();
	std::string outModel = outModelA.getValue();
	std::string outTxt = outTxtA.getValue();
	std::string iter = iterA.getValue();
	bool verbose = verboseA.getValue();
	bool silent = silentA.getValue();
	std::string lowmem = lowmemA.getValue();

	if(outModel == "NULL"){
		if(inData.find(".train.svmf",0)>0){
			outModel = inData.substr(0,inData.find(".train.svmf",0)).append(".mopecpp.model");
		}
		else if(inData.find(".svmf",0)>0){
			outModel = inData.substr(0,inData.find(".svmf",0)).append(".mopecpp.model");
		}
		else{
			outModel = inData.append(".mopecpp.model");
		}
	}
	if(outTxt == "NULL"){
		if(inData.find(".train.svmf",0)>0){
			outTxt = inData.substr(0,inData.find(".train.svmf",0)).append(".train.mopecpp.txt");
		}
		else if(inData.find(".svmf",0)>0){
			outTxt = inData.substr(0,inData.find(".svmf",0)).append(".train.mopecpp.txt");
		}
		else{
			outTxt = inData.append(".train.mopecpp.txt");
		}
	}

	std::map<std::string, std::string> options;
	options["inData"] = inData;
	options["outModel"] = outModel;
	options["outTxt"] = outTxt;
	options["iter"] = iter;
	options["verbose"] = verbose? "true":"false";
	options["silent"] = silent? "true":"false";
	options["lowmem"] = lowmem;

	Trainer * trainer = new Trainer(&options);
	trainer->startOperation();

	return 0;
}
