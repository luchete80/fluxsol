#include "FluxSol.h"

//Former temp test
// TEMPERATURE EXAMPLE 2
// READS FROM A FILE
void DefTempTest();

int main()
{

    cout << "Reading square.cgns ..."<<endl;
	Fv_CC_Grid malla("square.cgns");
	//malla.ReadCGNS();

	malla.Log("Log.txt");

	_CC_Fv_Field <Scalar> T(malla);

	//Boundary conditions
	Scalar wallvalue=0.;
	Scalar topvalue=1.;
	for (int p=0;p<3;p++)
	T.Boundaryfield().PatchField(p).AssignValue(wallvalue);
	T.Boundaryfield().PatchField(3).AssignValue(topvalue);

	EqnSystem <Scalar> TEqn;
	//Construir aca con la malla
	Scalar k(1.);	//Difusion, puede ser un escalar
	cout<<"Generating system"<<endl;
	TEqn=(FvImp::Laplacian(k,T)==0.);
	cout<<"Solving system"<<endl;
	Solve(TEqn);
	TEqn.Log("EqLog.txt");

	cout<<"Generating field"<<endl;
//	CenterToVertexInterpolation interp(malla);

	Vertex_Fv_Field<Scalar> vT;


	T=TEqn.Field();

	cout<<"Interpolating to vertices"<<endl;
//	vT=interp.Interpolate(T);

	cout<<"Writing files"<<endl;
	OutputFile("CellField.vtu",T);
//	OutputFile("VertexField.vtu",vT);

	cout << "End. Now Reading input.in"<<endl;
	DefTempTest();
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
//	CenterToVertexInterpolation interp(mesh);

	Vertex_Fv_Field<Scalar> vT;


	T=TEqn.Field();
	//_CC_Fv_Field<Vec3D> gradT = FvExp::Grad(T);

	//_CC_Fv_Field <Vec3D> gradT=FvExp::Grad(T);

	cout<<"Interpolating to vertices"<<endl;
	//vT=interp.Interpolate(T);

	cout<<"Writing files"<<endl;
	OutputFile("CellField-2.vtu",T);
	//OutputFile("VertexField.vtu",vT);

	//	---- The End -------
}
