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
#ifndef _GEOMETRICFIELD_H
#define _GEOMETRICFIELD_H

#include "Field.h"
#include "Products.h"
#include "Grid.h"
#include "Boundary.h"
//A general FvField can inherit fields below

namespace FluxSol
{
	template<typename T>
	class SurfaceField: public _Field<T>
	{

		protected:

		public:
			SurfaceField(){}
			SurfaceField(const int &i):_Field<T>(i, 0.0){}
			//SurfaceField<typename innerProduct < T, T> ::type>  operator &(const SurfaceField<T> &right) const;

	};


	//IF OPERATOR IN THOSE FIELDS ARE DECLARED AS MEMBER FUNCTIONS, IS AN ERROR WITH INNERPROD<TYPE>
	template <typename T>
	SurfaceField<typename innerProduct < T, T> ::type>  operator&(const SurfaceField<T> &left, const SurfaceField<T> &right);


    ///
    template<typename T>
    class GeomField:
    public _Field<T>
	{


	protected:
        //THIS IS A NEW FEATURE; A POINTER TO A BASE CLASS. THIS IS EXACTLY LIKE SURF_FV_FIELD WITHOUT OPERATORS
		_Grid *GridPtr;       //Se podria probar con un puntero general
		// en la clase base

	public:
		GeomField(const int &numval, const double &value = 0.) /*:SurfaceField<T>(numval, value)*/{}
		GeomField(){};         //Constructor
		//virtual GeomField  & Grid(){ return *GridPtr; }
		virtual const _Grid  & ConstGrid()const{ return *GridPtr; }

	};

		template<typename T>
	class GeomSurfaceField :
	    //public _Field<T>
	    public SurfaceField<T>
	{

    _Grid *GridPtr;

	protected:

	public:
		GeomSurfaceField(const int &numval, const double &value = 0.) /*:SurfaceField<T>(numval, value)*/{}
		GeomSurfaceField(){};         //Constructor
		//Adding boundary face
// TO CHECK; INHERIT OR NOT
//		GeomSurfaceField(const Fv_CC_Grid &grid)
//		{
//			GridPtr = &grid;
//			//LO HAGO PARA TODOS LOS FACES
//			for (int f = 0; f<GridPtr->Num_Faces(); f++)
//			{
//				if (GridPtr->Face(f).Boundaryface() && !GridPtr->Face(f).Is_Null_Flux_Face())
//				{
//					T val;
//					this->value.push_back(val);
//					this->idgrid_domain.push_back(f);
//				}
//			}
//
//		}
//
//		//Devuelve el valor en una Surface
//		T& GridIdSurFaceVal(const int &i)		//El valor de una
//		{
//			//Tengo que ver cual es la face i del field
//			for (int fid = 0; fid<this->Numberofvals(); fid++)
//			{
//				if (i == this->Idgrid_domain(fid))
//					return this->value[fid];
//			}
//		}

	};


	//boundary type
	enum _PatchFieldType { FIXEDVALUE, FIXEDGRADIENT };
		//IN FORMER FLUXSOL VERSIONS THIS CLASS WERE CALLED FVPATCHFIELD; NOW IS GENERIC
	template<typename T>
	class _PatchField :
		//public _Surf_Fv_Field<T>
		//IN FIRST FLUXSOL VERSIONS THIS CLASS INHERITED FROM SURF_FV_FIELD
		public GeomSurfaceField<T>
	{
	protected:
		int patchid;
		_PatchFieldType type;


	public:
		_PatchField(){};
		explicit _PatchField(const Patch &);
		int & PatchId(){ return patchid; };
		//This must be in parent class
		void AssignValue(const T &val);

		_PatchFieldType& Type(){ return type; }


	};


		//Instead a FieldField as in OpenFoam has a vector in Fields
	//This will be soon a field
	template<typename T>
	class _BoundaryField
	{
	protected:
		//Can be Fixed Value and Fixed Gradient
		std::vector < _PatchField < T > > patchfield;

	public:
		//_BoundaryField(const _CC_Fv_Field < T > &);
		_BoundaryField(){}
		_BoundaryField(const Boundary &bound)
		{
			Boundary b = bound;
			//Generate a _PatchField for every patch in boundary
			for (int np = 0; np<b.Num_Patches(); np++)
			{
				_PatchField < T > pf(b.vPatch(np));
				patchfield.push_back(pf);
			}

		}

		_PatchField < T > & PatchField(const int &i){ return this->patchfield[i]; }
		void Add_PatchField(_PatchField<T> &field)  { patchfield.push_back(field); }

	};

		//CAMPO VOLUMETRICO DE VOLUMENES FINITOS
	//CONVIENE QUE DERIVE
	// La T es Scalar, vector o tensor
	template<typename T>
	class GeomVolField :
	    public GeomField<T>{

		//Busco en los nodos de GridPtr
		//El vector value refiere a cada uno de los node

		//

		//SET DE DIMENSIONES

	protected:
		//Conviene colocar referencia o puntero??
		_BoundaryField <T> BoundaryField;	//En las caras


	public:

		//EVALUATE IF IT IS CONST
		//Fv_CC_Grid  *GridPtr;       //Se podria probar con un puntero general

		GeomVolField(const _Grid &grid);

		//_CC_Fv_Field (InputFile &inputfile);

		GeomVolField(){};

		//Conveccion, interpolacion o reconstruccion, de solo una celda
		//THIS IS OLD. FIELD DOES NOT RETURN AN EQ SYSTEM BECAUSE REFER TO EQSYS AND THIS TO FVGRID
		//AT THE SAME TIME FVGRID REFERS TO GEOMFIELD AND THIS TO FIELD, AND FIELD TO EQSYS
		//const Eqn <T> ToFaces(const int &icell, const std::vector <Scalar> flux) const;

		_BoundaryField <T> &  Boundaryfield(){ return BoundaryField; }

		//void ToCellCenters(EqnSystem <T> &eqnsys);


	};

}//FluxSol

#endif
