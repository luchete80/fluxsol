#include "FluxSol.h"
#include "CDTest.h"


void CDTest()
{


	string inputFileName="Input.in";
	InputFile input(inputFileName);
	
	vector<int> equations;

	Fv_CC_Grid mesh(input.section("grid",0).get_string("file"));
	
	_CC_Fv_Field<Vec3D> U;
	_CC_Fv_Field<Scalar> phi;

	ReadFieldFromInput(input, U,mesh);
	
	
	mesh.Log("Log.txt");


	EqnSystem <Scalar> CDEqn;

	Scalar k(1.);	//Diffusion

	cout<<"Generating system"<<endl;
	
	//CDEqn = (FvImp::Laplacian(k, phi) == FvImp::Div(U));

	CDEqn = (FvImp::Laplacian(k, phi) == FvImp::Div(U));

	cout<<"Solving system"<<endl;
	Solve(CDEqn);

	
	CDEqn.Log("EqLog.txt");

	cout<<"Generating field"<<endl;
	CenterToVertexInterpolation <Scalar> interp(phi);

	Vertex_Fv_Field<Scalar> vT;


	//T.ToCellCenters(TEqn);


	cout<<"Interpolating to vertices"<<endl;
	//vT=interp.Interpolate(T);

	cout<<"Writing files"<<endl;
	//OutputFile("CellField.vtu",T);
	//OutputFile("VertexField.vtu",vT);

	//	---- The End -------
	

}