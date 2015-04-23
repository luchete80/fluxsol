/************************************************************************

	Copyright 2012-2014 Luciano Buglioni - Pablo Zitelli

	Contacts:
        Luciano Buglioni: luciano.buglioni@gmail.com
        Pablo Zitelli:    zitelli.pablo@gmail.com
	This file is a part of FluxSol

	FluxSol is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    FluxSol is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    For a copy of the GNU General Public License,
    see <http://www.gnu.org/licenses/>.

*************************************************************************/

#include "Input.h"


using namespace std;

namespace FluxSol{

InputFile::InputFile(void) {
	;
}

InputFile::InputFile(string filename)
{
	setFile(filename);
	read_inputs();
}

void InputFile::setFile(string fName) {

	// Check if the input file exists
	fileName=fName;
	fstream file;
    bool found=false;
	//MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
	cout << "[I] Reading ... "<<endl;
	file.open(fileName.c_str());
	if (file.is_open()) {
		cout << "[I] Found input file " << fileName << endl;
		found=true;
	} else {
		cerr << "[E] Input file " << fileName << " could not be found!!" << endl;
	}
	// Read the whole input file to rawData
	int l=0;
	if (found)
    {
        string line;
        rawData="";
        while(getline(file, line)) {rawData += line + "\n"; l++;}
        file.close();

        // Strip all the inline or block comments (C++ style) from the rawData
        stripComments(rawData);
        // Strip all the white spaces from the rawData
        strip_white_spaces(rawData);
    }
    cout << "[I] "<<l << " lines readed ..." <<endl;
}

void InputFile::read (string sectionName, int number) {

	if (number!=-1) {
		int count=number_of_occurances(rawData,sectionName+"_");
		section(sectionName,0).count=count;
		for (int i=0;i<count;++i) {
			numberedSections[sectionName].push_back(numberedSections[sectionName][0]);
			numberedSections[sectionName][i].index=i;
			readSection(sectionName,i);
		}
	} else {
		readSection(sectionName,-1);
	}

	return;
}

void InputFile::readSection(string sectionName, int number) {
	int Rank=1;	//LB MODIFIED FROM FREE_CFD
	//MPI_Comm_rank(MPI_COMM_WORLD, &Rank);

	string fullName=sectionName;
	if (number>=0) fullName+="_"+int2str(number+1);
	// Get section data
	section(sectionName,number).is_found=extract_in_between(rawData,fullName+"{","}",section(sectionName,number).rawData,true,"};");
	// Exit if the section is required but couldn't be found
	if (section(sectionName,number).is_required && !section(sectionName,number).is_found) {
		if (Rank==0) cerr << "[E] Required input section " << fullName << " could not be found!!" << endl;
		exit(1);    //this-> readerror=true;
		cout << "Warning! section " << fullName << " could not be found!!" << endl;
	}

	// Read subsections
	map<string,Subsection>::iterator subsectionIter;
	for (subsectionIter=section(sectionName,number).subsections.begin();subsectionIter!=section(sectionName,number).subsections.end();subsectionIter++) {
		subsectionIter->second.parentIndex=number;
		readSubsection(subsectionIter->second);
	}

	// Read numbered subsections
	map<string,vector<Subsection> >::iterator nsubsectionIter;
	for (nsubsectionIter=section(sectionName,number).numberedSubsections.begin();nsubsectionIter!=section(sectionName,number).numberedSubsections.end();nsubsectionIter++) {
		// Find how many subsections there are
		int count=number_of_occurances(section(sectionName,number).rawData,nsubsectionIter->first+"_");
		// First occurance was already initialized when numbered subsection was registered
		// Copy duplicate the first entry count-1 times
		nsubsectionIter->second[0].count=count;
		for (int i=1;i<count;++i) nsubsectionIter->second.push_back(nsubsectionIter->second[0]);
		// Fill in each one of them
		for (int i=0;i<count;++i) {
			// Append number to name
			nsubsectionIter->second[i].name+="_"+int2str(i+1);
			nsubsectionIter->second[i].parentIndex=number;
			readSubsection(nsubsectionIter->second[i]);
		}

	}

	// Read variables
	section(sectionName,number).readEntries();

	if (Rank==0) cout << "[I] Read input section: " << fullName << endl;

}

void InputFile::readSubsection(Subsection &sub) {

	// Get subsection data
	sub.is_found=extract_in_between(section(sub.parentName,sub.parentIndex).rawData,sub.name+"(",");",sub.rawData,true,"};");
	// Exit if the subsection is required but couldn't be found
	if (sub.is_required && !sub.is_found) {
		int Rank=0;	//MODIFICATIONS FROM FREE_CFD
		//MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
		string path=sub.parentName;
		if (sub.parentIndex!=-1) path+="_"+int2str(sub.parentIndex);
		if (Rank==0) cerr << "[E] Required input subsection " << path << " -> " << sub.name << " could not be found!!" << endl;
		exit(1);
		cout << "Warning! subsection " << path << " -> " << sub.name << " could not be found!!" << endl;
	}
	sub.readEntries();
	return;
}

void InputBaseContainer::readEntries(void) {
	// Loop ints
	map<string,entry<int> >::iterator intIter;
	for (intIter=ints.begin();intIter!=ints.end();intIter++) {
		string varName=intIter->first;
		string varValue;
 		// Find the variable
		intIter->second.is_found=extract_in_between(rawData,varName+"=",";",varValue,true,"({;");
		if (intIter->second.is_found) {
			intIter->second.value=atoi(varValue.c_str());
		} else { // if not found
			if (intIter->second.is_required) {
				entry_not_found(varName);
			} else {
				intIter->second.value=intIter->second.default_value;
			}
		}
	} // end loop ints
	// Loop doubles
 	map<string,entry<double> >::iterator doubleIter;
 	for (doubleIter=doubles.begin();doubleIter!=doubles.end();doubleIter++) {
 		string varName=doubleIter->first;
		string varValue;
 		// Find the variable
		doubleIter->second.is_found=extract_in_between(rawData,varName+"=",";",varValue,true,"({;");
		if (doubleIter->second.is_found) {
			doubleIter->second.value=strtod(varValue.c_str(),NULL);
		} else { // if not found
			if (doubleIter->second.is_required) {
				entry_not_found(varName);
			} else {
				doubleIter->second.value=doubleIter->second.default_value;
			}
		}
		//cout << varName << "\t" << doubleIter->second.value << endl; // DEBUG

	} // end loop doubles
	// Loop strings
	map<string,entry<string> >::iterator stringIter;
	for (stringIter=strings.begin();stringIter!=strings.end();stringIter++) {
		string varName=stringIter->first;
		string varValue;
 		// Find the variable
		stringIter->second.is_found=extract_in_between(rawData,varName+"=",";",varValue,true,"({;");
		if (stringIter->second.is_found) {
			stringIter->second.value=varValue;
		} else { // if not found
			if (stringIter->second.is_required) {
				entry_not_found(varName);
			} else {
				stringIter->second.value=stringIter->second.default_value;
			}
		}
	} // end loop strings
	// Loop Vec3Ds
	map<string,entry<Vec3D> >::iterator Vec3DIter;
	for (Vec3DIter=Vec3Ds.begin();Vec3DIter!=Vec3Ds.end();Vec3DIter++) {
		string varName=Vec3DIter->first;
		string varValue;
 		// Find the variable
		Vec3DIter->second.is_found=extract_in_between(rawData,varName+"=",";",varValue,true,"({;");
		if (Vec3DIter->second.is_found) {
			// The data is read as string. Now convert it to double components of a vector
			char *pEnd;
			Vec3DIter->second.value.comp[0]=strtod(varValue.c_str()+1,&pEnd);
			Vec3DIter->second.value.comp[1]=strtod(pEnd+1,&pEnd);
			Vec3DIter->second.value.comp[2]=strtod(pEnd+1,NULL);
		} else { // if not found
			if (Vec3DIter->second.is_required) {
				entry_not_found(varName);
			} else {
				Vec3DIter->second.value=Vec3DIter->second.default_value;
			}
		}
	} // end loop Vec3Ds
	// Loop intLists
	map<string,entry<vector<int> > >::iterator intListIter;
	for (intListIter=intLists.begin();intListIter!=intLists.end();intListIter++) {
		string varName=intListIter->first;
		vector<string> exploded;
		string varValue;
 		// Find the variable
		intListIter->second.is_found=extract_in_between(rawData,varName+"=[","];",varValue,true,"({;");
		if (intListIter->second.is_found) {
			// The data is read as string with comma separated entries altogether. Now explode it.
			StringExplode(varValue,",", &exploded);
			for (int i=0;i<exploded.size();++i) intListIter->second.value.push_back(atoi(exploded[i].c_str()));
		} else { // if not found
			if (intListIter->second.is_required) {
				entry_not_found(varName);
			} else {
				intListIter->second.value=intListIter->second.default_value;
			}
		}
	} // end loop intLists
	// Loop doubleLists
	map<string,entry<vector<double> > >::iterator doubleListIter;
	for (doubleListIter=doubleLists.begin();doubleListIter!=doubleLists.end();doubleListIter++) {
		string varName=doubleListIter->first;
		vector<string> exploded;
		string varValue;
 		// Find the variable
		doubleListIter->second.is_found=extract_in_between(rawData,varName+"=[","];",varValue,true,"({;");
		if (doubleListIter->second.is_found) {
			// The data is read as string with comma separated entries altogether. Now explode it.
			StringExplode(varValue,",", &exploded);
			char *pEnd;
			for (int i=0;i<exploded.size();++i) {
				doubleListIter->second.value.push_back(strtod(exploded[i].c_str(),&pEnd));
			}
		} else { // if not found
			if (doubleListIter->second.is_required) {
				entry_not_found(varName);
			} else {
				doubleListIter->second.value=doubleListIter->second.default_value;
			}
		}
	} // end loop doubleLists
	// Loop stringLists
	map<string,entry<vector<string> > >::iterator stringListIter;
	for (stringListIter=stringLists.begin();stringListIter!=stringLists.end();stringListIter++) {
		string varName=stringListIter->first;
		string varValue;
 		// Find the variable
		stringListIter->second.is_found=extract_in_between(rawData,varName+"=[","];",varValue,true,"({;");
		if (stringListIter->second.is_found) {
			// The data is read as string with comma separated entries altogether. Now explode it.
			StringExplode(varValue,",", &stringListIter->second.value);
		} else { // if not found
			if (stringListIter->second.is_required) {
				entry_not_found(varName);
			} else {
				stringListIter->second.value=stringListIter->second.default_value;
			}
		}
	} // end loop stringLists
} // end InputBaseContainer::readEntries

void InputFile::stripComments(string &data) {
	string dummy;
	while (extract_in_between(data,"/*","*/",dummy));
	while (extract_in_between(data,"//","\n",dummy));
}

void InputFile::strip_white_spaces(string &data) {
	string whitespaces(" \t\f\v\n\r");
	size_t found=0;
	while (found!=string::npos) {
		found=data.find_first_of(whitespaces);
		if (found!=string::npos) data.erase(found,1);
	}
}

bool extract_in_between(string &data, string begin, string end, string &result,bool check_char_before, string acceptList) {
	size_t begin_pos, end_pos;
	begin_pos=0; end_pos=0;
	string pre;
	bool found=false;
	while (!found) {
		// Find the first occurance position of the beginning sequence
		begin_pos=data.find(begin,end_pos);
		if (begin_pos==string::npos) return false;
		// From where the first search left off, find the first occurance position of the ending sequence
		end_pos=data.find(end,begin_pos+begin.length());
		if (end_pos==string::npos) return false;
		// Check the character just before the beginning delimiter (if asked for)
		if (check_char_before) {
			if (begin_pos==0) {
				found=true;
			} else {
				pre=data.substr(begin_pos-1,1);
				if (pre.find_first_of(acceptList)!=string::npos) found=true;

			}
		} else {
			found=true;
		}
		if (found) {
			// Extract the what's in between
			result=data.substr(begin_pos+begin.length(),end_pos-begin_pos-begin.length());
			// Remove that chunk from the originial data
			data.replace(begin_pos,end_pos+end.length()-begin_pos,"");
		}
	}
	return true;
}

int number_of_occurances(string haystack, string needle) {
	int count=-1;
	size_t found,start;
	found=0;
	while (found!=string::npos) {
		count++;
		start=(count==0)? 0 : found+needle.length();
		found=haystack.find(needle,start);
	}
	return count;
}

void StringExplode(string str, string separator, vector<string>* results) {
    int found;
    found = str.find_first_of(separator);
    while(found != string::npos){
        if(found > 0){
            results->push_back(str.substr(0,found));
        }
        str = str.substr(found+1);
        found = str.find_first_of(separator);
    }
    if(str.length() > 0){
        results->push_back(str);
    }
}


void InputFile::read_inputs(void) {

	// These will make more sense as you read the input registration lines below
	bool required=true; bool optional=false;
	bool numbered=true; bool single=false;

//	registerSection("reference",single,optional);
//	section("reference").register_double("p",optional,0.);
//	section("reference").register_double("T",optional,0.);
//	section("reference").register_double("Mach",optional,1.);
//	read("reference");

	registerSection("grid",numbered,required);
	section("grid",0).register_string("file",optional);
	section("grid",0).register_int("dimension",optional,3);
	section("grid",0).register_string("solution_scheme",optional,"navier-stokes");                 //New, from FluxSol, if empty, solution is navier stokes
	section("grid",0).register_string("solution_scheme",required);



	//section("grid",0).register_string("equations",required);

//	section("grid",0).registerSubsection("writeoutput",single,required);
//	section("grid",0).subsection("writeoutput").register_string("format",optional,"vtk");
//	section("grid",0).subsection("writeoutput").register_int("plotfrequency",optional,1e10);
//	section("grid",0).subsection("writeoutput").register_int("restartfrequency",optional,1e10);
//	section("grid",0).subsection("writeoutput").register_int("loadfrequency",optional,1e10);
//	section("grid",0).subsection("writeoutput").register_Vec3D("momentcenter",optional,0.);
//	section("grid",0).subsection("writeoutput").register_stringList("includebcs",optional);
//	section("grid",0).subsection("writeoutput").register_stringList("variables",required);

//	section("grid",0).register_string("material",optional,"none");
//	section("grid",0).registerSubsection("material",single,optional);
//	// Default air values assigned
//	section("grid",0).subsection("material").register_double("gamma",optional,1.4);
//	section("grid",0).subsection("material").register_double("molarmass",optional,28.97);
//	section("grid",0).subsection("material").register_double("viscosity",optional,0.);
//	section("grid",0).subsection("material").register_double("thermalconductivity",optional,0.);
//	section("grid",0).subsection("material").register_double("Cp",optional,1000.);
//	section("grid",0).subsection("material").register_double("density",optional,0.);
//
//	section("grid",0).registerSubsection("IC",numbered,required);
//	section("grid",0).subsection("IC",0).register_string("region",optional,"box");
//	section("grid",0).subsection("IC",0).register_Vec3D("corner_1",optional,-1.e20);
//	section("grid",0).subsection("IC",0).register_Vec3D("corner_2",optional,1e20);
//	section("grid",0).subsection("IC",0).register_Vec3D("center",optional);
//	section("grid",0).subsection("IC",0).register_Vec3D("axisdirection",optional);
//	section("grid",0).subsection("IC",0).register_double("radius",optional);
//	section("grid",0).subsection("IC",0).register_double("height",optional);
//	section("grid",0).subsection("IC",0).register_double("p",optional);
//	section("grid",0).subsection("IC",0).register_Vec3D("V",optional);
//	section("grid",0).subsection("IC",0).register_double("T",optional);
//	section("grid",0).subsection("IC",0).register_double("rho",optional);
//	section("grid",0).subsection("IC",0).register_double("turbulenceintensity",optional,1.e-2);
//	section("grid",0).subsection("IC",0).register_double("eddyviscosityratio",optional,0.1);

    //section("grid",0).registerSubsection("patch",numbered,required);
	//section("grid",0).subsection("patch",0).register_string("type",required);
	//section("grid",0).subsection("patch",0).register_intList("list",optional);

    //std::vector<int> list=get_intLists    ("list");

    section("grid",0).registerSubsection("list",single,optional);
    section("grid",0).subsection("list").register_stringList("nodes",optional);
    section("grid",0).subsection("list").register_stringList("cells",optional);

	section("grid",0).registerSubsection("BC",numbered,required);
	section("grid",0).subsection("BC",0).register_string("patch",required);
	section("grid",0).subsection("BC",0).register_string("type",required);
	section("grid",0).subsection("BC",0).register_string("kind",optional,"none");
	section("grid",0).subsection("BC",0).register_string("region",optional,"gridfile");
	section("grid",0).subsection("BC",0).register_string("interface",optional,"none");
	section("grid",0).subsection("BC",0).register_Vec3D("corner_1",optional);
	section("grid",0).subsection("BC",0).register_Vec3D("corner_2",optional);
	section("grid",0).subsection("BC",0).register_string("pick",optional,"override");
	section("grid",0).subsection("BC",0).register_double("p",optional);
	section("grid",0).subsection("BC",0).register_double("p_total",optional);
	section("grid",0).subsection("BC",0).register_double("mdot",optional);
	section("grid",0).subsection("BC",0).register_double("qdot",optional);
	section("grid",0).subsection("BC",0).register_Vec3D("U",optional);
	section("grid",0).subsection("BC",0).register_double("T",optional);
	section("grid",0).subsection("BC",0).register_double("T_total",optional);
	section("grid",0).subsection("BC",0).register_double("rho",optional);
	section("grid",0).subsection("BC",0).register_double("turbulenceintensity",optional,1.e-2);
	section("grid",0).subsection("BC",0).register_double("eddyviscosityratio",optional,0.1);

//	section("grid",0).registerSubsection("turbulence",single,optional);
//	section("grid",0).subsection("turbulence").register_double("relativetolerance",optional,1.e-6);
//	section("grid",0).subsection("turbulence").register_double("absolutetolerance",optional,1.e-12);
//	section("grid",0).subsection("turbulence").register_int("maximumiterations",optional,10);
//	section("grid",0).subsection("turbulence").register_string("model",optional,"sst");
//	section("grid",0).subsection("turbulence").register_string("order",optional,"second");
//	section("grid",0).subsection("turbulence").register_double("klowlimit",optional,1.e-8);
//	section("grid",0).subsection("turbulence").register_double("khighlimit",optional,1.e4);
//	section("grid",0).subsection("turbulence").register_double("omegalowlimit",optional,1.);
//	section("grid",0).subsection("turbulence").register_double("viscosityratiolimit",optional,5.e4);
//	section("grid",0).subsection("turbulence").register_double("turbulentPr",optional,0.9);
//
//	section("grid",0).registerSubsection("navierstokes",single,optional);
//	section("grid",0).subsection("navierstokes").register_double("relativetolerance",optional,1.e-6);
//	section("grid",0).subsection("navierstokes").register_double("absolutetolerance",optional,1.e-12);
//	section("grid",0).subsection("navierstokes").register_int("maximumiterations",optional,10);
//	section("grid",0).subsection("navierstokes").register_string("limiter",optional,"vk");
//	section("grid",0).subsection("navierstokes").register_double("limiterthreshold",optional,1.);
//	section("grid",0).subsection("navierstokes").register_string("order",optional,"second");
//	section("grid",0).subsection("navierstokes").register_string("jacobianorder",optional,"first");
//	section("grid",0).subsection("navierstokes").register_string("convectiveflux",optional,"AUSM+up");
//
//	section("grid",0).registerSubsection("transform",numbered,optional);
//	section("grid",0).subsection("transform",0).register_string("function",optional);
//	section("grid",0).subsection("transform",0).register_Vec3D("anchor",optional,0.);
//	section("grid",0).subsection("transform",0).register_Vec3D("end",optional,0.);
//	section("grid",0).subsection("transform",0).register_Vec3D("factor",optional,0.);
//	section("grid",0).subsection("transform",0).register_Vec3D("axis",optional,0.);
//	section("grid",0).subsection("transform",0).register_double("angle",optional,0.);

	read("grid",0);

//	registerSection("timemarching",single,required);
//	section("timemarching").register_string("integrator",optional,"backwardEuler");
//	section("timemarching").register_double("stepsize",optional,1.);
//	section("timemarching").register_double("CFLmax",optional,1000.);
//	section("timemarching").register_double("CFLlocal",optional,1000.);
//	section("timemarching").registerSubsection("ramp",single,optional);
//	section("timemarching").subsection("ramp").register_double("initial",optional,1.);
//	section("timemarching").subsection("ramp").register_double("growth",optional,1.2);
//	section("timemarching").register_int("numberofsteps",required);
	//read("timemarching");

	readEntries();

	// Read the material file for each grid
	//material_input.resize(section("grid",0).count);
	//for (int gid=0;gid<material_input.size();++gid) {
		// If a material specification exists
		//if(section("grid",0).get_string("material").is_found) {
		//	// Default values are set to air properties
		//	string fileName=section("grid",0).get_string("material");
		//	fileName+=".mat";
		//	material_input[gid].setFile(fileName);
		//	material_input[gid].register_double("molar mass",optional,28.97);
		//	material_input[gid].register_double("density",optional,0.);
		//	material_input[gid].register_double("gamma",optional,1.4);
		//	material_input[gid].registerSection("equationofstate",single,optional);
		//	material_input[gid].section("equationofstate").register_string("model",optional,"idealgas");
		//	material_input[gid].read("equationofstate");
		//	material_input[gid].register_double("viscosity",optional,1.716e-5);
		//	material_input[gid].registerSection("viscosity",single,optional);
		//	material_input[gid].section("viscosity").register_string("model",optional,"sutherlands");
		//	material_input[gid].section("viscosity").register_double("referenceviscosity",optional,1.716e-5);
		//	material_input[gid].section("viscosity").register_double("referencetemperature",optional,273.15);
		//	material_input[gid].section("viscosity").register_double("sutherlandtemperature",optional,110.4);
		//	material_input[gid].read("viscosity");
		//	material_input[gid].register_double("Cp",optional,1000.);
		//	material_input[gid].registerSection("Cp",single,optional);
		//	material_input[gid].section("Cp").register_int("numberofpieces",optional);
		//	material_input[gid].section("Cp").register_string("model",optional,"shomate");
		//	material_input[gid].section("Cp").register_doubleList("coefficients",optional);
		//	material_input[gid].read("Cp");
		//	material_input[gid].register_double("thermalconductivity",optional,0.024);
		//	material_input[gid].registerSection("thermalconductivity",single,optional);
		//	material_input[gid].section("thermalconductivity").register_string("model",optional,"constantPrandtl");
		//	material_input[gid].section("thermalconductivity").register_double("Pr",optional,0.7);
		//	material_input[gid].read("thermalconductivity");
		//	material_input[gid].readEntries();
		//}
	//}

	return;
}

string InputFile::ReadEqnType() {

    string eqntype = section("grid",0).get_string("solution_scheme");

    return eqntype;
}

EqnSystem <Scalar> InputFile::ReadEqnSys(const Fv_CC_Grid &mesh) {

    string diffusion,convection_diffusion;
    EqnSystem <Scalar> eqn;

    if (ReadEqnType()=="diffusion") {

        Scalar kdiff = 1.;                 // Ahora lo defino manualmente, el proximo paso es levantar el dato del material
        _CC_Fv_Field <Scalar> phi(mesh);
        eqn = (FvImp::Laplacian(kdiff,phi)==0.);
    }
    else if (ReadEqnType()=="convection_diffusion") {

        cout << "Coming soon..." << endl;
        Scalar kdiff = 1.;                 // Ahora lo defino manualmente, el proximo paso es levantar el dato del material
        _CC_Fv_Field <Scalar> phi;
        eqn = (FvImp::Laplacian(kdiff,phi)==0.);
    }

    return eqn;

}

//TO MODIFY, TEMPLATIZE!!!!!

//NOT WORKING BECAUSE, REFERENCE IS DELETED, TO MODIFY
_CC_Fv_Field <Vec3D>
InputFile::UField()
{
    //Reading BC
    int patchfieldnumber;   //number of boundary field patches
    patchfieldnumber=section("grid",0).numberedSubsections["BC"].size();
    cout << "[I] Field Patches: "<<patchfieldnumber<<endl;
    int numpatches=GridPtr->vBoundary().Num_Patches();
    vector<int> asoc(numpatches,-1);   //Relation between Patch and bcond, size refers to patch
    vector <Vec3D> cvalues(numpatches,Vec3D(0.));   //Def cvalues
    vector <int> validpf_id;                        //Refers to global boundary condition

    int validpfnum=0;        //valid patchfield number

    for (int pf=0;pf<patchfieldnumber;pf++)
    {
        string pfname=section("grid",0).subsection("BC",pf).get_string("patch");

        for (int meshp=0;meshp<numpatches;meshp++)
        {
           if(pfname==GridPtr->vBoundary().vPatch(meshp).Name())
           {
                //cout << "Found name in patch "<<meshp<<endl;
                asoc[meshp]=validpfnum;
                validpf_id.push_back(pf);
                validpfnum++;
           }
        }
    }

    //Assign values
    for (int meshp=0;meshp<numpatches;meshp++)
    {
        //cout << "meshp: "<<meshp<<endl;
        int validpfid=asoc[meshp];
        //cout << "validpfid: "<<validpfid<<endl;
        int pf=validpf_id[validpfid];
        //cout << "global pf: "<<pf<<endl;
        cvalues[meshp]=section("grid",0).subsection("BC",pf).get_Vec3D("U");
 //       cvalues[meshp]=section("grid",0).subsection("BC",pf).get_string("U");

        //cout << "Applied "<<cvalues[meshp].outstr()<<"to patch "<<meshp<<endl;
    }

    _BoundaryField<Vec3D> bf(GridPtr->vBoundary(),cvalues);
    cout << "Creating ret field"<<endl;
    _CC_Fv_Field <Vec3D> ret(*GridPtr, bf);


    return ret;
}


//TO MODIFY, to join with previous function
_CC_Fv_Field <Scalar>
InputFile::pField()
{
    //Reading BC
    int patchfieldnumber;   //number of boundary field patches
    patchfieldnumber=section("grid",0).numberedSubsections["BC"].size();
    cout << "Field Patches: "<<patchfieldnumber<<endl;
    int numpatches=GridPtr->vBoundary().Num_Patches();
    vector<int> asoc(numpatches,-1);   //Relation between Patch and bcond, size refers to patch
    vector <Scalar> cvalues(numpatches,Scalar(0.));   //Def cvalues
    vector <int> validpf_id;                        //Refers to global boundary condition

    int validpfnum=0;        //valid patchfield number

    for (int pf=0;pf<patchfieldnumber;pf++)
    {
        string pfname=section("grid",0).subsection("BC",pf).get_string("patch");

        for (int meshp=0;meshp<numpatches;meshp++)
        {
           if(pfname==GridPtr->vBoundary().vPatch(meshp).Name())
           {
                //cout << "Found name in patch "<<meshp<<endl;
                asoc[meshp]=validpfnum;
                validpf_id.push_back(pf);
                validpfnum++;
           }
        }
    }

    //Assign values
    for (int meshp=0;meshp<numpatches;meshp++)
    {
        //cout << "meshp: "<<meshp<<endl;
        int validpfid=asoc[meshp];
        //cout << "validpfid: "<<validpfid<<endl;
        int pf=validpf_id[validpfid];
        //cout << "global pf: "<<pf<<endl;
        cvalues[meshp]=section("grid",0).subsection("BC",pf).get_double("p");
 //       cvalues[meshp]=section("grid",0).subsection("BC",pf).get_string("U");

//        cout << "Applied pressure "<<cvalues[meshp].outstr()<<"to patch "<<meshp<<endl;
    }

    _BoundaryField<Scalar> bf(GridPtr->vBoundary(),cvalues);
    cout << "Creating ret field"<<endl;
    _CC_Fv_Field <Scalar> ret(*GridPtr, bf);


    return ret;
}


std::vector <BoundaryCond*>
InputFile::ReadBCs()
{
    std::vector <BoundaryCond*> ret;
    //Reading BC
    int patchfieldnumber;   //number of boundary field patches
    patchfieldnumber=section("grid",0).numberedSubsections["BC"].size();
    cout << "Reading Patch Types: "<<patchfieldnumber<<endl;
    int numpatches=GridPtr->vBoundary().Num_Patches();
    vector<int> asoc(numpatches,-1);   //Relation between Patch and bcond, size refers to patch
    vector <Vec3D> cvalues(numpatches,Vec3D(0.));   //Def cvalues
    vector <int> validpf_id;                        //Refers to global boundary condition

    int validpfnum=0;        //valid patchfield number

    for (int pf=0;pf<patchfieldnumber;pf++)
    {
        string pfname=section("grid",0).subsection("BC",pf).get_string("patch");

        for (int meshp=0;meshp<numpatches;meshp++)
        {
           if(pfname==GridPtr->vBoundary().vPatch(meshp).Name())
           {
                //cout << "Found name in patch "<<meshp<<endl;
                asoc[meshp]=validpfnum;
                validpf_id.push_back(pf);
                validpfnum++;

                string type=section("grid",0).subsection("BC",pf).get_string("type");

                cout << "BC " << pf << "is of type " << type << endl;
                cout << "Creating BC for patch "<<meshp<<",Named: " << pfname <<endl;
                if (type=="wall" || type =="WALL")
                {
                    WallBC *bc=new WallBC(this->ufield,this->pfield,meshp);

                    ret.push_back(bc);
                }
                else if (type=="inlet" || type =="INLET")
                {
                    InletBC *bc=new InletBC(this->ufield,this->pfield,meshp);

                    ret.push_back(bc);
                }
                else if (type=="velocity-inlet" || type =="VELOCITY-INLET")
                {
                    VelocityInletBC *bc=new VelocityInletBC(this->ufield,this->pfield,meshp);

                    ret.push_back(bc);
                }
                else if (type=="pressure-outlet" || type =="PRESSURE-OUTLET")
                {
                    PressureOutletBC *bc=new PressureOutletBC(this->ufield,this->pfield,meshp);

                    ret.push_back(bc);
                }

                else if (type=="pressure-inlet" || type =="PRESSURE-OUTLET")
                {
                    PressureInletBC *bc=new PressureInletBC(this->ufield,this->pfield,meshp);

                    ret.push_back(bc);
                }
                else if (type=="null-flux" || type =="NULL-FLUX")
                {
                    NullFluxBC *bc=new NullFluxBC(this->ufield,this->pfield,meshp);

                    ret.push_back(bc);
                }
                else
                {
                    cout << "[E] Boundary condition type is not recognized, created Wall BC for this patch..." <<endl;
                    WallBC *bc=new WallBC(this->ufield,this->pfield,meshp);

                    ret.push_back(bc);
                }
           }
        }
    }

    _BoundaryField<Vec3D> bf(GridPtr->vBoundary(),cvalues);
    //cout << "Creating ret field"<<endl

    return ret;
}


}
