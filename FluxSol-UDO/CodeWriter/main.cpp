#include "FileSearch.h"

#include "PatchField.inst"

int main()
{

	vector <UDO *> vudo;
	map <UDOIds, UDO*> test;


    FileSearcher fs(".");
    //test = fs.GetUDOMapsFromFile("boundary.cpp");


    fs.WriteUDOLibFile();
    //UDO* udo=new _PatchField<Scalar>();
	//vudo.push_back(udo);



}
