#ifndef _MODEL_H
#define _MODEL_H

#include "BoundaryCond.h"
#include "FvGrid.h"
#include "SIMPLE.h"
//#include "Nastran.h"
#include "Solver.h"
#include "Input.h"
#include "Materials/Materials.h"

#include "Interpolation/CenterToVertexInterpolation.h"
#include "Interpolation/CenterToVertexInterpolation_Def.h"    //TO MODIFY


//#define _USE_32BIT_TIME_T  //MINGW BUG

#include <time.h>
#include "OutputFile.h"

#include "Div.h"

namespace FluxSol{


//Modelo con Material
//Malla, archivo Nastran, BCs
//Pero sin analisis

class Part
{

    Fv_CC_Grid mesh;

    public:
        Part();
        Part (const Fv_CC_Grid & mesh);

};

class Model
{


protected:
	int bidim_model;
	//En principio el archivo esta dentro del modelo
	//Archivo *nasfile;	//Cambiar el nombre de la clase a una que incluya el nombre Nastran
	std::vector <Part> part;
	int numparts;
	int numfields;
	int nummats;
	std::vector <Materials> mat;

	std::vector <BoundaryCond*> bcs;

    Fv_CC_Grid mesh;// COMPLETE MESH

	InputFile inputfile;

    //solution info
    int maxiter;    //Override by input file info

	//Devolucion de Miembros
	//Archivo & NasFile(){return (*nasfile);}

public:

	//Creo el
	Model(){}
	Model(const std::string );
	Model(const Fv_CC_Grid &);

    const int NumberOfParts()const{return this->numparts;}

    const Materials & Materials(const int &i)const{return this->mat[i];}

    const Fv_CC_Grid & Mesh() const{return this->mesh;}     //Complete Mesh in part?

    virtual void Solve(){};


};

class CFDModel:public Model
{

    //FIELDS FOR EACH TIME STEP
    _CC_Fv_Field <Vec3D> U; //
    _CC_Fv_Field <Scalar> p;

    _Surf_Fv_Field <Scalar>  phi; //Mass Flux

    EqnSystem <Scalar> pEqn;
    EqnSystem <Vec3D> UEqn;     //To modify: associate Eqn System with field

	public:

	void Extract_Cells_and_BoundaryFromNastran();
    void InitFields();

	//Constructores
	CFDModel(){}
//	CFDModel(const Archivo &nasfile, const Fv_CC_Grid &); //Genero un modelo a partir de un Nastran
														 	 //Que a su vez genera la malla

	CFDModel(const std::string s);
    void Solve();
    void WriteOutput();


	////////////////////////////////////
	//Funciones que devuelven miembros//
	////////////////////////////////////

	//Fv_CC_Grid & Mesh(){return (*mesh);}
//	const _CC_Fv_Field <Vec3D> &U()const{return this->U;}


};

class ThermalModel:public Model
{

    _CC_Fv_Field <Scalar> T;

};

class CFDThermalModel:public CFDModel, public ThermalModel
{



    public:
        CFDThermalModel(){}
        void Solve(){};
};

};//Fin de FluxSol
#endif
