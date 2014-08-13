/************************************************************************
	
	Copyright 2012-2013 Luciano Buglioni
 
	Contact: luciano.buglioni@gmail.com

	This file is a part of FluxSol

	FluxSol is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    Free CFD is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    For a copy of the GNU General Public License,
    see <http://www.gnu.org/licenses/>.

*************************************************************************/
#ifndef _SIMPLE_H
#define _SIMPLE_H

#include "./SistEcuac/SistEcuac.h"
#include "./Solver/Solver.h"
#include "./Type/Scalar.h"

namespace FluxSol{

//SIMPLE tipo de solver
class Solver_SIMPLE: public _Solver{

    //Notar que ambos sistemas de ecuaciones son clases, y el tipo de variable
    //variara
    //No puedo asociarlas a referencias porque estas deben ser inicializadas
    //una vez creadas, con lo cual creo punteros a estas
    //SE PUEDEN CREAR AL INICIAR LA CLASE SOLVER

    EqnSystem <Vec3D>  *U;   //Velocidades, siempre son referencias
    EqnSystem <Scalar> *P;   //Presiones

public:
    //El constructor por defecto Llama al constructor de la clase base!
    //Por que me conviene hacerlo asi??
    Solver_SIMPLE():_Solver(){};
    Solver_SIMPLE(const int &dim_cell);
    //Resuelve una iteracion
    //La funcion virtual la debo llamar virtual aca??

};

}

#endif
