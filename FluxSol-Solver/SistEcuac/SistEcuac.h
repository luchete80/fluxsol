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
#ifndef _SIST_EC_H
#define _SIST_EC_H

#include <vector>
#include <string>
#include <fstream>
//Is important to note that must not be recursively
#include "FvGrid.h"
#include "FvField.h"

#include <time.h>

using namespace std;
//EL SISTEMA DE ECUACIONES PODRIA TENER UNA MATRIZ Y UN VECTOR
namespace FluxSol{


template <typename T>
class Eqn;
template <typename T>
class EqnSystem;

template <typename T>
class Eqn{

    int id;										//eqn Id
//	T ap;                                       //Central coefficient
    Scalar ap;
//	std::vector <T> an;                         //Neighbours
    std::vector <Scalar> an;

	//Neighbours
	//Tis information is duplicated, is in mesh and in system eqn
	//This can be improved by referring the mesh with a reference in eqnsystem
	int num_neighbours;                         //Number of neighbours
    vector<int> neighbour_id;                         //Id de cada neighbours
	int width;									//width of equations according to neighbours ids


    T x,source;                                 //source and unknown

public:

	Eqn(){ap=0.;num_neighbours=0;x=0.;source=0.;};
//    Eqn(const T &p, const std::vector <T>& n){ap=p;an=n;x=0.;source=0.;};    //Constructor definiendo ap y an
    Eqn(const T &p, const std::vector <Scalar>& n){ap=p;an=n;x=0.;source=0.;};    //Constructor definiendo ap y an

//	Eqn(const T &p, const std::vector <T>& n, const T &s)
	Eqn(const Scalar &p, const std::vector <Scalar>& n, const T &s)
	{ap=p;an=n;x=0.;source=s;}    //Constructor definiendo ap y an

//	Eqn(const T &p, const std::vector <T>& n, const T &s, std::vector<int> nbrid)
	Eqn(const Scalar &p, const std::vector <Scalar>& n, const T &s, std::vector<int> nbrid)
	{ap=p;an=n;x=0.;source=s;neighbour_id=nbrid;num_neighbours=neighbour_id.size();}    //Constructor definiendo ap y an

//	Eqn(const int &ids,const T &p, const std::vector <T>& n, const T &s, std::vector<int> nbrid)
	Eqn(const int &ids,const Scalar &p, const std::vector <Scalar>& n, const T &s, std::vector<int> nbrid)
	{id=ids;ap=p;an=n;x=0.;source=s;neighbour_id=nbrid;num_neighbours=neighbour_id.size();}    //Constructor definiendo ap y an
    void Coeffs(const T &iap, const std::vector<T> &ian){ap=iap;an=ian;}

    Scalar& Ap(){return ap;};  //Coeficiente central
	Scalar& An(const int &i){return an[i];}
	T& Source(){return source;}
	const T& X()const{return x;}
	//void Ap(const double &);
	void Ap(const Scalar &iap){ap=iap;}
	void An(const int &i, const Scalar &val){an[i]=val;}
	void Source(const T &v){source=v;}
	void X(const T &v){x=v;}
	int & Neighbour(const int &i){return this->neighbour_id[i];}
	int MinNeigbourId();
	void Relax();
	const std::vector <int> & NeighboursIds()const{return neighbour_id;}

    Eqn<T> & operator-(const Eqn<T> &right);
    std::vector <T>& An(){return an;};


	//OPERATORS
	//Is more general in case of vectors
	Eqn<T> & operator +=(const double &d){this->source+=d; return *this;};

	Eqn<T> & operator +=(const T &d){this->source+=d; return *this;};

    void SetValueCondition(const T &val)
    {
        *this=Eqn<T>();
        this->ap=1.;
        this->source=val;
    }

	Eqn<T> operator-()
	{
	    Eqn<T> eq;
	    T _ap=-this->ap;
	    vector <T> _an;
	    for (int n=0;n<this->num_neighbours;n++)
            _an.push_back(-this->an[n]);

        return Eqn<T>(_ap,_an);
    }

	//Binary operators
	//Eqn<T> & operator +(const Eqn<T> &right);
	//Eqn<T> & operator -(const Eqn<T> &right);
	Eqn<T> & operator ==(const Eqn<T> &right) ;

	int Width();
	int & Num_Neighbours(){return num_neighbours;}
	const int & Id(){return id;}


	//Log
	void Log(ofstream &f);

    friend EqnSystem<T>;

};

//Can derive a Class With Finite Volume information
//FvEqnSystem
template <typename T>
class EqnSystem{   //Es un vector de ecuaciones


    //OJO CON EL ESPACIO, NO LO TOMA SI NO DEJO EL ESPACIO
    //ASI no funciona: vector <_Ecuacion<T>>
	//const int dimension;
	int dimension;
    std::vector <Eqn <T> > eqn;
    //std::vector <T> prevsol;            //Previous solutions
    std::vector <unsigned int> first_nonzero_column;    //Primer columna con valor distinto de cero
	std::vector <int> nbr_eqn;							//To which neighbour eqn belongs each coefficient an
	//Fv_CC_Grid &grid;                                      //Esto sirve para sistemas esparsos
    Fv_CC_Grid *GridPtr;    //OpenFOAM Style, TO MODIFY;
	//_CC_Fv_Field <T> &field;										//Can be any tipe of field

	Scalar alpha,invalpha;                                       //Under Relaxation term

	_CC_Fv_Field   <T> field;              //Field referring to Eqn System results

	_CC_Fv_Field <T> *initfield;             //Initial field val

    public:

	EqnSystem(const int &n, Eqn<T> &eqq)
	{

        this->eqn.clear();

        for (int e_=0;e_<n;e_++)
        {

            //this->eqn.push_back(eqq);
        }
    }
    //Constructores
    EqnSystem():dimension(int(pTraits<T>::rank)){
        //cout << "Eqn Sys created w/o dim"<<endl;
        }
    //~EqnSystem(){
//    	    delete GridPtr;
//    	    }
    EqnSystem(std::vector<Eqn<T> > ec);


	EqnSystem(const Fv_CC_Grid &);

	//EqnSystem(_CC_Fv_Field <T> &fvfield);


    //Ensamble
    void Ensamblar(const int &pos, const Eqn<T> ec);
    void Insertar (const int &pos, const Eqn<T> ec);
    void Insert (const Eqn<T> &ec);
	void Insert (const Eqn<T> &ec, std::vector <int> &);

	void Log(std::string str);
	vector< Eqn<T> > & EqnV(){ return eqn; }
	Eqn<T> & Eqn(const int &i){return eqn[i];}

    void SetRelaxCoeff(const Scalar &a){this->alpha=a;this->invalpha=1.-a;}
	void Relax()
	{
	    for (int e=0;e<this->EqnV().size();e++)
        {
            this->eqn[e].Ap(this->eqn[e].Ap()/this->alpha);
            //this->eqn[e].ap=this->eqn[e].ap*this->invalpha;
            //T xcorr;
            //xcorr=this->eqn[e].X()+(1.-this->alpha)*this->prevsol[e]/this->alpha;
            //this->eqn[e].X(xcorr);
        }
    }

	//Operators
	const int & Dim() const {return dimension;}
	const int Num_Eqn()const {
	    return eqn.size();
    }
//	void operator=(const EqnSystem <T> &right)
//	{
//
//        cout << "TEMP"<<endl;
//	}

	EqnSystem <T> & operator==(const double &);

	//Binary operators, U
	EqnSystem <T> & operator==(const EqnSystem <T> &right);

//MUST TO EQUAL ALL
	EqnSystem <T> & operator=(const EqnSystem <T> &right)
	{
        clock_t ittime_begin, ittime_end;
        double ittime_spent;
        ittime_end = clock();

	    this->eqn.clear();
	    this->GridPtr=right.GridPtr;
	    this->eqn.resize(right.EqnV().size());

	    this->nbr_eqn.resize(right.nbr_eqn.size());
        for (int e=0;e<right.EqnV().size();e++)
        {
            //this->eqn.push_back(right.Eqn(e));
            this->eqn[e]=right.Eqn(e);
        }

//        for (int f=0;f<right.first_nonzero_column.size();f++)
//            this->first_nonzero_column.push_back(right.first_nonzero_column[f]);
//
//        for (int f=0;f<right.nbr_eqn.size();f++)
//            this->nbr_eqn.push_back(right.nbr_eqn[f]);

        this->dimension=right.dimension;

        ittime_spent = (double)(clock() - ittime_end) / CLOCKS_PER_SEC;
        ittime_end = clock();
        cout << "eqnsys operator ="<<ittime_spent <<endl;

        return *this;
	}


	EqnSystem <T> operator- (const EqnSystem <T> &right)
	{

    clock_t ittime_begin, ittime_end;
    double ittime_spent;


    ittime_end = clock();

    //cout << "Eqn sizes"<< this->Num_Eqn()<< " "<<right.EqnV().size()<<endl;
    int num_eqn=this->eqn.size();
        //EqnSystem<T> ret(num_eqn);
        EqnSystem<T> ret=*this;
        if (EqnV().size() == right.EqnV().size())
        {
            //Check if both eqns have same size
            for (int e = 0; e<EqnV().size(); e++)
            {
    //			cout << "operator =="<< this-> eqn[e].NeighboursIds().size()<<endl;
                ret.eqn[e] = (this->eqn[e] - right.Eqn(e));
    //			cout << "operator =="<< this-> eqn[e].NeighboursIds().size()<<endl;
            }
        }
        else
        {
            cout << "Eqn Systems have different size. Operator == fails"<<endl;
        }

        //cout << "Returning..."<<endl;

        ittime_spent = (double)(clock() - ittime_end) / CLOCKS_PER_SEC;
        ittime_end = clock();
        cout << "eqnsys operator -  "<<ittime_spent <<endl;


        return ret;

	}

	EqnSystem <T> & operator==(const _CC_Fv_Field<T> &field);

	//Output to field
	//void ToCellCenterField();
    const _CC_Fv_Field<T> Field()const
    {
        _CC_Fv_Field<T> field(this->Grid());
        //vector <T*> temp(this->Num_Eqn());
        //Can compare number of cells vs number of eqn
        for (int e=0;e<this->Num_Eqn();e++)
        {
            field.Val(e,this->Eqn(e).X());
            //temp[e]=&this->eqn[e].source;
        }
        //_CC_Fv_Field<T> field(&this->Grid(),temp);
        return field;
    }


	//template <typename T>
	//operator=(const EqnSystem <T> &r){}

	const Fv_CC_Grid  & Grid()const{ return *GridPtr; }

	const _CC_Fv_Field<Scalar> A()const
	{
	    vector <Scalar*> temp(this->Num_Eqn());
	    _CC_Fv_Field<Scalar> field(this->Grid());
        for (int e=0;e<this->Num_Eqn();e++)
        {
//            temp[e]=&this->eqn[e].ap;
            field.Val(e,this->Eqn(e).Ap());
        }
//        _CC_Fv_Field<Scalar> field(this->Grid(),temp);
        return field;
	}

	//_CC_Fv_Field <T> InitialField(){}
	void InitField(const _CC_Fv_Field <T> &field)
	{
        initfield=&field;
	}

	const _CC_Fv_Field <T> & InitField()const{return *initfield;}

    const string outstr()const
	{
        string ret;
        string cad;
        std::ostringstream strs;
        //strs << std::setprecision(6) << std::scientific << val ;//<< "\n";
        strs<< "Eqn System Width"<<endl;

        for (int e=0;e<this->eqn.size();e++)
            strs << this->eqn[e].Width()<<endl;
        cad+= strs.str();
        return cad;

	}


	const vector<double> GlobalRes()const;        //Globally Scaled Residual, like in Fluent

};//EqnSYstem


//Puedo generar una matriz a partir de un sistema de ecuaciones con un typedef
//typedef

//ACA PUEDO PONER TODOS LOS OPERADORES PARA RESOLVER!!!

//FUNCIONES

// CONSTRUCTORES
//
//


template <typename T>
void Eqn <T>::Log(ofstream &f)
{
	f<<"ap : ";
	ap.Log(f);
	f<<endl;
	f<<"an : ";
	for (int n=0;n<an.size();n++)	{an[n].Log(f);f<<" ";}
	f<<endl;
	f<<"source: ";
	this->source.Log(f);
	f<<endl;
}



}

//This is needed

#endif
