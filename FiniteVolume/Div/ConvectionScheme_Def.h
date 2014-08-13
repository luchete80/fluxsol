#include "ConvectionScheme.h"

using namespace FluxSol;

//Must be defined one function with a field and a flux
//Since flux may be not related to the field
template typename <T>
EqnSystem <T> FirstOrderUpwindScheme<T>::Div(const _CC_Fv_Field<T> &)
{

	Eqn <T> eqn;            //Ecuacion para cada una de las cells
	//No hace falta construirla porque la devuelve el VolField
	//EqnSystem <T> eqnsys(VolField.GridPtr->Num_Cells());   //Como no le doy parametros inicia todo en cero, salvo las dimensiones
	//EqnSystem <T> < typename innerProduct < Vec3D, T> ::type > eqnsys(VolField.Grid());
	EqnSystem <typename innerProduct < Vec3D, T> ::type> eqnsys(VolField.ConstGrid());

	//Interpolate face fluxes and then upwind 
	CenterToFaceInterpolation <T> interp(VolField);
	//Flux, inner producto
	//Can be a vector, or a scalar
	//Sf is a vector
	SurfaceField <typename innerProduct < Vec3D, Vec3D> ::type> FluxField(VolField.Numberofvals());
	//_CC_Fv_Field <typename innerProduct < Vec3D, Vec3D> ::type> Prueba(VolField.Numberofvals(),0.);
	FluxField = VolField.GridPtr->Sf() & interp.Interpolate();


	//A continuacion se muestra una forma de calculo de divergencia
	//Sum_f (Sf * (ro * U)f * fi(f) )
	//En OpenFoam ro*U es fi
	//Esta forma de calculo es para cuando las partes de la cara no son iguales
	//--------------
	//RECORRO CELLS
	const std::vector<Cell_CC>::iterator cellit;
	vector <Scalar> flux;	//Flujo en la celda y en las vecina
	const int numcells = VolField.GridPtr->Num_Cells();
	for (int cid = 0; cid<numcells; cid++)
	{
		//const int &icell, std::vector <Scalar> flux
		//Tengo que pasar a vector el campo de flujo
		const int nfaces = VolField.GridPtr->Cell(cid).Num_Faces();


		//Calculate Face Flux
		//eqn=VolField.ToFaces(cid,flux); //Puedo hacer una funcion que no itere por elemento pero esta la memoria

	}
	//    for (cellit=;icell<VolField.GridPtr->num_cells;icell++)
	//    {
	//
	//    }

	return eqnsys;
}