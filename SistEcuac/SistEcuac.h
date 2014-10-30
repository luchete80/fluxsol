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
#include "./FiniteVolume/Mesh/FvGrid.h"
#include "FvField.h"

using namespace std;
//EL SISTEMA DE ECUACIONES PODRIA TENER UNA MATRIZ Y UN VECTOR
namespace FluxSol{

template <typename T>
class Eqn{

    int id;										//eqn Id
	T ap;                                       //Central coefficient
	std::vector <T> an;                         //Neighbours

	//Neighbours
	//Tis information is duplicated, is in mesh and in system eqn
	//This can be improved by referring the mesh with a reference in eqnsystem
	int num_neighbours;                         //Number of neighbours
    vector<int> neighbour_id;                         //Id de cada neighbours
	int width;									//width of equations according to neighbours ids


    T x,source;                                 //source and unknown

public:

	Eqn(){ap=0.;num_neighbours=0;x=0.;source=0.;};
    Eqn(const T &p, const std::vector <T>& n){ap=p;an=n;x=0.;source=0.;};    //Constructor definiendo ap y an

	Eqn(const T &p, const std::vector <T>& n, const T &s)
	{ap=p;an=n;x=0.;source=s;}    //Constructor definiendo ap y an

	Eqn(const T &p, const std::vector <T>& n, const T &s, std::vector<int> nbrid)
	{ap=p;an=n;x=0.;source=s;neighbour_id=nbrid;num_neighbours=neighbour_id.size();}    //Constructor definiendo ap y an

	Eqn(const int &ids,const T &p, const std::vector <T>& n, const T &s, std::vector<int> nbrid)
	{id=ids;ap=p;an=n;x=0.;source=s;neighbour_id=nbrid;num_neighbours=neighbour_id.size();}    //Constructor definiendo ap y an
    void Coeffs(const T &iap, const std::vector<T> &ian){ap=iap;an=ian;}

    T& Ap(){return ap;};  //Coeficiente central
	T& An(const int &i){return an[i];}
	T& Source(){return source;}
	const T& X()const{return x;}
	//void Ap(const double &);
	void Ap(const T &iap){ap=iap;}
	void An(const int &i, const T &val){an[i]=val;}
	void Source(const T &v){source=v;}
	void X(const T &v){x=v;}
	int & Neighbour(const int &i){return this->neighbour_id[i];}
	int MinNeigbourId();
	const std::vector <int> & NeighboursIds()const{return neighbour_id;}

    Eqn<T> & operator-(const Eqn<T> &right);
    std::vector <T>& An(){return an;};


	//OPERATORS
	//Is more general in case of vectors
	Eqn<T>  operator +=(const double &d){source+=d; return *this;};

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
};

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

//Can derive a Class With Finite Volume information
//FvEqnSystem
template <typename T>
class EqnSystem{   //Es un vector de ecuaciones


    //OJO CON EL ESPACIO, NO LO TOMA SI NO DEJO EL ESPACIO
    //ASI no funciona: vector <_Ecuacion<T>>
	//const int dimension;
	int dimension;
    std::vector <Eqn <T> > eqn;
    std::vector <unsigned int> first_nonzero_column;    //Primer columna con valor distinto de cero
	std::vector <int> nbr_eqn;							//To which neighbour eqn belongs each coefficient an
	//Fv_CC_Grid &grid;                                      //Esto sirve para sistemas esparsos
    Fv_CC_Grid *GridPtr;    //OpenFOAM Style, TO MODIFY;
	//_CC_Fv_Field <T> &field;										//Can be any tipe of field

    public:

    //Constructores
    EqnSystem():dimension(int(pTraits<T>::rank)){
        //cout << "Eqn Sys created w/o dim"<<endl;
        }
    ~EqnSystem(){
//    	    delete GridPtr;
    	    }
    EqnSystem(std::vector<Eqn<T> > ec);
	//EqnSystem(const int);
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

	//Operators
	const int & Dim(){return dimension;}
	const int Num_Eqn(){return eqn.size();}
//	void operator=(const EqnSystem <T> &right)
//	{
//
//        cout << "TEMP"<<endl;
//	}

	EqnSystem <T> & operator==(const double &);

	//Binary operators, U
	EqnSystem <T> & operator==(const EqnSystem <T> &right);
	EqnSystem <T> & operator- (const EqnSystem <T> &right);

	EqnSystem <T> & operator==(const _CC_Fv_Field<T> &field);

	//Output to field
	//void ToCellCenterField();
    const _CC_Fv_Field<T> Field()const
    {
        _CC_Fv_Field<T> field(this->Grid());
        //Can compare number of cells vs number of eqn
        for (int e=0;e<this->Num_Eqn();e++)
        {
            field.Val(e,this->Eqn(e).X());
        }
        return field;
    }
	//template <typename T>
	//operator=(const EqnSystem <T> &r){}

	const Fv_CC_Grid  & Grid()const{ return *GridPtr; }

};


//Puedo generar una matriz a partir de un sistema de ecuaciones con un typedef
//typedef

//ACA PUEDO PONER TODOS LOS OPERADORES PARA RESOLVER!!!

//FUNCIONES

// CONSTRUCTORES
//
//
}

//This is needed

#endif
