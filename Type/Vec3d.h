/************************************************************************

	Copyright 2012-2013 Luciano Buglioni

	Contact: luciano.buglioni@gmail.com

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
#ifndef _VEC3D_H
#define _VEC3D_H

#include <iostream>
#include <vector>
#include "Scalar.h"
#include "Tensor.h"
#include "Products.h"    //Para incluir tipo de Rango
#include <iostream>     // std::cout, std::right, std::endl
#include <iomanip>      // std::setw
#include <sstream>     // std::cout, std::right, std::endl

using namespace std;

//La clase se coloca
// CAMBIAR CARACTERISTICAS DE VEC3D
namespace FluxSol{

//Ambos son del tipo tensor!!
class Vec3D :public Tensor{

private:


public:
	double comp[3];

    enum
    {
        rank = 1 // Rank of Vector is 1, esto sirve para las templates
    };
	//Constructores
	//Vec3D(const double &v){comp[0]=comp[1]=comp[2]=v;};
	Vec3D(double x=0., double y=0., double z=0.);   //Argunentos por defecto

	//Funcion tipica usada en los templates de operaciones
	Vec3D Val();

	//Operadores
	//Is exclusive of vectors
	double dot(const Vec3D &right);
	Vec3D cross(const Vec3D &right);
	Vec3D & operator= (const Vec3D &);
	Vec3D & operator= (const Scalar &);
	Vec3D & operator= (const double &);
	Vec3D & operator= (const std::vector<double> &);
	Vec3D & operator*= (const double &);


	Vec3D &operator/= (const double &);
	Vec3D &operator+= (const double &);
	Vec3D &operator+= (const Vec3D &);
	Vec3D &operator-= (const double &);
	Vec3D &operator-= (const Vec3D &);


	const Vec3D operator* (const double &)const;
	const Vec3D operator/ (const double &)const;
	const Vec3D operator- (const double &)const;
	const Vec3D operator+ (const double &)const;

	const Vec3D operator* (const Vec3D &)const;
	const Vec3D operator/ (const Vec3D &)const;
	const Vec3D operator- (const Vec3D &)const;
	const Vec3D operator+ (const Vec3D &)const;

	//Unary
	//DOES NOT MODIFY THE OBJECT
    const Vec3D & operator+=(const Scalar &s)
	{
        for (int i=0;i<3;i++)   this->comp[i]+=s.Val();
        return *this;
	}
	const Vec3D operator-()
	{
	    Vec3D ret;
        for (int i=0;i<3;i++)   ret[i]=-comp[i];
        return ret;
	}

	const Scalar operator&(const Vec3D &right) const;
	const string outstr()const
	{
        string cad;
        std::ostringstream strs;
        strs << comp[0] << " " << comp[1] << " " << comp[2] << " " << "\n";
        cad+= strs.str();
        return cad;
	}

	//template<>
	//typename innerProduct<Vec3D, Vec3D> ::type
	//operator& (const Vec3D &right);

	bool operator== (const Vec3D &);
	bool operator!= (const Vec3D &);
	double &operator[] (int i);
	const Scalar Norm()const;							//This must be moved to operations

	const Vec3D &normalize(void);	//Versor
	const Vec3D  VecNorm(void) const;	//Versor


    //Componentes
    inline double & x(){return comp[0];}
    inline double & y(){return comp[1];}
    inline double & z(){return comp[2];}


	const vector<double> Comp()const ;	//Common to all templates

	//Operaciones de Log``
	std::string Imprimir_Coord();


    virtual ~Vec3D(){}; //Destructor virtual
	void Log(ofstream &f){f<<comp[0]<<";"<<comp[1]<<";"<<comp[2]<<";";}

};

Vec3D  operator^ (const Vec3D &left, const Vec3D &right);

//Funciones aparte
//Puede ser vertice o vec3D
//Vec3D  Prom(const vector <Vec3D> &vecs);
//Para que acepte solo vectores de clase derivadas
//Pero la clase base podria no ser template
template <typename T>
//typename enable_if<is_base_and_derived<BaseClass, T>, unsigned>::type
T  Prom(const vector<T>& vecs)
{
    T vec(0,0,0);
    for (int i=0;i<vecs.size();i++)
        vec+=vecs[i];
    vec/=vecs.size();
    return vec;
}


template<>
class pTraits<Vec3D>
{

    public:
    enum
    {
        //Que dimension es??
        dim = 3,
        rank = 1,
        nComponents = 3
    };

//- Component type
    typedef Vec3D cmptType;
};


//Especializacion de plantilla
template<>
class typeOfRank<1>
{
public:

typedef Vec3D type;
};

const Vec3D operator* (const double &left, const Vec3D &right);
//Inner Prod

//Pending for allocating in Operations.h/cpp
const Vec3D operator* (const Scalar &sc, const Vec3D &vector);
const Vec3D operator* (const Vec3D &vector, const Scalar &sc);

Vec3D operator/ (Vec3D &vector, Scalar &sc);
double fabs(const Vec3D& vec);

}//Fin de namespace FluxSol

//Operaciones que no son de clase pero que incluyen a otras clases



//Funciones inline
#include "Vec3dI.h"

#endif
