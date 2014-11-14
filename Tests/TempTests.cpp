#include "../FluxSol.h"
#include "TempTests.h"


//Former temp test
int TempTest2()
{

	Fv_CC_Grid malla("cylinder.cgns");
	//malla.ReadCGNS();

	malla.Log("Log.txt");

	_CC_Fv_Field <Scalar> T(malla);

	//Boundary conditions
	Scalar wallvalue=0.;
	Scalar topvalue=1.;
	for (int p=0;p<3;p++)
	T.Boundaryfield()._PatchField(p).AssignValue(wallvalue);
	T.Boundaryfield()._PatchField(3).AssignValue(topvalue);

	EqnSystem <Scalar> TEqn;
	//Construir aca con la malla
	Scalar k(1.);	//Difusion, puede ser un escalar
	cout<<"Generating system"<<endl;
	TEqn=(FvImp::Laplacian(k,T)==0.);
	cout<<"Solving system"<<endl;
	Solve(TEqn);
	TEqn.Log("EqLog.txt");

	cout<<"Generating field"<<endl;
	CenterToVertexInterpolation interp(malla);

	Vertex_Fv_Field<Scalar> vT;


	T.ToCellCenters(TEqn);

	cout<<"Interpolating to vertices"<<endl;
	vT=interp.Interpolate(T);

	cout<<"Writing files"<<endl;
	OutputFile("CellField.vtu",T);
	OutputFile("VertexField.vtu",vT);
	return 0;
}


//Last Test
void DefTempTest()
{
	string inputFileName="Input.in";
	InputFile input(inputFileName);

	vector<int> equations;

	Fv_CC_Grid mesh(input.section("grid",0).get_string("file"));

	_CC_Fv_Field<Scalar> T;

	ReadFieldFromInput(input, T,mesh);
	mesh.Log("Log.txt");


	EqnSystem <Scalar> TEqn;

	Scalar k(1.);	//Diffusion

	cout<<"Generating system"<<endl;

	TEqn=(FvImp::Laplacian(k,T)==0.);


	cout<<"Solving system"<<endl;
	Solve(TEqn);


	TEqn.Log("EqLog.txt");

	cout<<"Generating field"<<endl;
	CenterToVertexInterpolation interp(mesh);

	Vertex_Fv_Field<Scalar> vT;


	T.ToCellCenters(TEqn);
	//_CC_Fv_Field<Vec3D> gradT = FvExp::Grad(T);

	//_CC_Fv_Field <Vec3D> gradT=FvExp::Grad(T);

	cout<<"Interpolating to vertices"<<endl;
	vT=interp.Interpolate(T);

	cout<<"Writing files"<<endl;
	OutputFile("CellField.vtu",T);
	OutputFile("VertexField.vtu",vT);

	//	---- The End -------
}
