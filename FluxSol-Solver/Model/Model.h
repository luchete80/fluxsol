#ifndef _MODEL_H
#define _MODEL_H

#include "BoundaryCond.h"
#include "FvGrid.h"
#include "SIMPLE.h"
//#include "Nastran.h"
#include "Solver.h"
#include "PETSC_Solver.h"
#include "PETSC_GAMGSolver.h"
#include "Input.h"
#include "Materials/Materials.h"

#include "Interpolation/CenterToVertexInterpolation.h"
#include "Interpolation/CenterToVertexInterpolation_Def.h"    //TO MODIFY


//#define _USE_32BIT_TIME_T  //MINGW BUG

#include <time.h>
#include "OutputFile.h"

#include "Div.h"
#include "UDO.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <numeric>
//#include <chrono>
#include <string>
#include <cstring>

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

	stringstream itlog,reslog;

    Fv_CC_Grid mesh;// COMPLETE MESH

	InputFile inputfile;

    //solution info
    int maxiter;    //Override by input file info
    UDOLib udolib;

	//Devolucion de Miembros
	//Archivo & NasFile(){return (*nasfile);}
    int iternumber;

public:

	//Creo el
	Model(){}
	Model(const std::string );
	Model(const Fv_CC_Grid &);

    const int NumberOfParts()const{return this->numparts;}

    const Materials & Materials(const int &i)const{return this->mat[i];}

    const Fv_CC_Grid & Mesh() const{return this->mesh;}     //Complete Mesh in part?
    virtual void SolveIter(){}

    virtual void Solve(){};

    const string ItLog()const {return reslog.str();}


};

class CFDModel:public Model
{


    protected:


    clock_t ittime_begin, ittime_end, ittime_temp;
    clock_t begin, end;

    double time_spent;
    double ittime_spent;

    //FIELDS FOR EACH TIME STEP
    _CC_Fv_Field <Vec3D> U; //
    _CC_Fv_Field <Scalar> p;

    GeomSurfaceField <Vec3D> meshSf;

    EqnSystem <Scalar> pEqn;
    EqnSystem <Vec3D> UEqn;     //To modify: associate Eqn System with field
    _Surf_Fv_Field <Scalar>  phi;


    //TEMPORARY FIELDS, TO MODIFY
    _BoundaryField<Vec3D>  bf =U.Boundaryfield();   //Temp pbf
    _BoundaryField<Scalar>pbf =p.Boundaryfield();   //Temp bf
    _CC_Fv_Field <Scalar> AUr;
    //TO FIX: MAKE THIS WORK
    _Surf_Fv_Field <Scalar> AUrf_;
    _CC_Fv_Field<Scalar> pcorr;
    _Surf_Fv_Field <Vec3D> Gradpf_;



    Scalar alpha_p,alpha_u;

    Scalar k,rho;

    vector <double> ures;


    void GetNShowTimeSpent(const string &str){
        ittime_spent = (double)(clock() - ittime_temp) / CLOCKS_PER_SEC;
        cout << str<< " Time Elapsed: " << ittime_spent<<endl;
        ittime_temp =clock();
        }

	public:

	void Extract_Cells_and_BoundaryFromNastran();
	void SolveIter();
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

// ORIGINAL
// PROFILER
//#define PROFILE(BLOCK, ROUTNAME) ProfilerRun([&](){do {BLOCK;} while(0);}, \
//        ROUTNAME, __FILE__, __LINE__);
//
//template <typename T>
//void ProfilerRun (T&&  func, const std::string& routine_name = "unknown",
//                  const char* file = "unknown", unsigned line = 0)
//{
//    using std::chrono::duration_cast;
//    using std::chrono::microseconds;
//    using std::chrono::steady_clock;
//    using std::cerr;
//    using std::endl;
//
//    steady_clock::time_point t_begin = steady_clock::now();
//
//    // Call the function
//    func();
//
//    steady_clock::time_point t_end = steady_clock::now();
//    cerr << "[" << std::setw (20)
//         << (std::strrchr (file, '/') ?
//             std::strrchr (file, '/') + 1 : file)
//         << ":" << std::setw (5) << line << "]   "
//         << std::setw (10) << std::setprecision (6) << std::fixed
//         << static_cast<float> (duration_cast<microseconds>
//                                (t_end - t_begin).count()) / 1e6
//         << "s  --> " << routine_name << endl;
//
//    cerr.unsetf (std::ios_base::floatfield);
//}


// PROFILER
#define PROFILE(BLOCK, ROUTNAME) ProfilerRun([&](){do {BLOCK;} while(0);}, \
        ROUTNAME, __FILE__, __LINE__);

template <typename T>
void ProfilerRun (T  **func, const std::string& routine_name = "unknown",
                  const char* file = "unknown", unsigned line = 0)
{
    using std::cerr;
    using std::endl;

    clock_t t_begin = clock();

    // Call the function
    func();

    clock_t t_end = clock();
    cerr << "[" << std::setw (20)
         << (std::strrchr (file, '/') ?
             std::strrchr (file, '/') + 1 : file)
         << ":" << std::setw (5) << line << "]   "
         << std::setw (10) << std::setprecision (6) << std::fixed
//         << static_cast<float> (duration_cast<microseconds>
         <<(double)(t_end - t_begin) / CLOCKS_PER_SEC
         //<< (t_end - t_begin).count() / 1e6
         << "s  --> " << routine_name << endl;

    cerr.unsetf (std::ios_base::floatfield);
}


// USAGE
//    PROFILE (
//    {
//        for (unsigned int k = 0; k < N; ++k)
//            bigarray.push_back (k);
//    }, "reserve + push_back");

};//Fin de FluxSol
#endif
