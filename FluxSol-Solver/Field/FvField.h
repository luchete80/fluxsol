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
#ifndef _FVFIELD_H
#define _FVFIELD_H

#include "Field.h"
#include "GeometricField.h"
//A general FvField can inherit fields below
#include "FvGrid.h"

namespace FluxSol
{

	template <typename T>
	class Vertex_Fv_Field :public _Field<T>
	{
		Fv_CC_Grid *GridPtr;

	public:
		Vertex_Fv_Field(){};
		Vertex_Fv_Field(const Fv_CC_Grid &grid);

		//Later it will be inherited
		const Fv_CC_Grid  & Grid()const{ return *GridPtr; }

	};

	/////////////////////////////
	///// CAMPOS GEOMETRICO /////
	/////////////////////////////
	// IMPORTANTE REFERENCIA A LA MALLA
	//Tienen asociadas mallas, siempre se llama igual *GridPtr
	//y puede ser distinto tipo de malla
	//CONVIENE PONER FvField como padre

	//Campo de superficie
	//TO MODIFY, INHERITS FRoM SURFACEField TO??
	template<typename T>
	class _Surf_Fv_Field :
	    //public _Field<T>
	    public GeomSurfaceField<T>
	{


	protected:

	public:
		_Surf_Fv_Field(const int &numval, const double &value = 0.) /*:SurfaceField<T>(numval, value)*/{}
		_Surf_Fv_Field(){};         //Constructor
		Fv_CC_Grid  & Grid(){ Fv_CC_Grid *grid=this->GridPtr; return *grid;}
		//Adding boundary face
		_Surf_Fv_Field(const Fv_CC_Grid &grid)
		{
			this->GridPtr = &grid;
			//LO HAGO PARA TODOS LOS FACES
			this->numberofvals=grid.Num_Faces();
			this->value=vector<T>(this->GridPtr->Num_Faces());
            this->idgrid_domain=vector<int>(this->GridPtr->Num_Faces());
			for (int f = 0; f<this->GridPtr->Num_Faces(); f++)
			{
			    //This is to consider, by now are created all
				//if (grid.Face(f).Boundaryface() && !grid.Face(f).Is_Null_Flux_Face())
				//{
					//T val;
					//this->value.push_back(val);
					//this->idgrid_domain.push_back(f);
					this->idgrid_domain[f]=f;
				//}
			}

		}

		//Devuelve el valor en una Surface
		T& GridIdSurFaceVal(const int &i)		//El valor de una
		{
			//Tengo que ver cual es la face i del field
			for (int fid = 0; fid<this->Numberofvals(); fid++)
			{
				if (i == this->Idgrid_domain(fid))
					return this->value[fid];
			}
		}

        //TO MODIFY
		_Surf_Fv_Field <T> & operator=(const GeomField<T> &field)
		{
            this->value.clear();
            T v;
            this->numberofvals=field.Numberofvals();
            this->value.assign(field.Numberofvals(),v);
            this->GridPtr=&field.Grid();
            for (int c=0;c<field.Numberofvals();c++)
                this->value[c]=field.Val(c);
            //TO MODIFY
            //this->Boundaryfield()=field->BoundaryField
            return *this;
		}

        //TO MODIFY, GeomField has this
		_Surf_Fv_Field <T> & operator=(const double &val)
		{
            for (int c=0;c<this->Numberofvals();c++)
                this->value[c]=val;
		}
//        _Surf_Fv_Field <T> & operator=(const GeomField<T> field)
//        {
//
//        }

		_Surf_Fv_Field<T> operator||(const _Surf_Fv_Field<T> &right)
		{
		    _Surf_Fv_Field<T> ret(*this);
			for (int v = 0; v<this->value.size(); v++)
			{
				ret.value[v] = this->value[v]||right.value[v];
			}
			ret;

		}


	};



	//Diriclet Field
	template <typename T>
	class _Fv_FixedValue_Patch_Field :public _PatchField<T>
	{


	public:
		explicit _Fv_FixedValue_Patch_Field(const Patch &p) :_PatchField<T>(p){ this->type = FIXEDVALUE; };
	};

	//Newmann Field
	template <typename T>
	class _Fv_FixedGradient_Patch_Field :public _PatchField<T>
	{


	public:
		explicit _Fv_FixedGradient_Patch_Field(const Patch &p) :_PatchField<T>(p){ this->type = FIXEDGRADIENT; };
	};


	//CAMPO VOLUMETRICO DE VOLUMENES FINITOS
	//CONVIENE QUE DERIVE
	// La T es Scalar, vector o tensor
	template<typename T>
	class _CC_Fv_Field :
	    //public _Field<T>
	    public GeomVolField<T>
	    {

		//Busco en los nodos de GridPtr
		//El vector value refiere a cada uno de los node

		//

		//SET DE DIMENSIONES

	protected:

	    set<int> int_netflux_faces;
        void Create_IntNetFluxFaces();
	public:

		//EVALUATE IF IT IS CONST
		//Fv_CC_Grid  *GridPtr;       //Se podria probar con un puntero general

		_CC_Fv_Field(const Fv_CC_Grid *grid);
		_CC_Fv_Field(const Fv_CC_Grid *grid, const vector <T*> vp);
		_CC_Fv_Field(const Fv_CC_Grid &grid);
		_CC_Fv_Field(const Fv_CC_Grid &grid, const vector<T *>);
		_CC_Fv_Field(const Fv_CC_Grid &grid, const _BoundaryField <T> &bfield);

        //IS NECCESARY TO OVERRIDE THIS FUNCTION!?!!?!?
		Fv_CC_Grid  & Grid(){ Fv_CC_Grid *gptr=this->GridPtr; return *gptr; }
        const Fv_CC_Grid & ConstGrid()const { Fv_CC_Grid *gptr=this->GridPtr; return *gptr; }

                //TO MODIFY, CHECKMESH
        void AssignBoundaryField(const _BoundaryField <T> &bfield)
        {
            this->BoundaryField=bfield;
        }

        const set<int> & IntNetFluxFaces()const{return this->int_netflux_faces;}
		//_CC_Fv_Field (InputFile &inputfile);

		_CC_Fv_Field(){};

		_CC_Fv_Field(GeomField<T> field)
		{
            operator=(field);
		}

		//_CC_Fv_Field & operator=(const GeomField<T> &field){}
		_CC_Fv_Field <T> & operator=(const GeomField<T> &field)
		{
            this->value.clear();
            T v;
            this->value.assign(field.Numberofvals(),v);
            this->GridPtr=&field.Grid();
            this->StorePrevValues();
            for (int c=0;c<field.Numberofvals();c++)
                this->value[c]=field.Val(c);


            //TO MODIFY
            //this->Boundaryfield()=field->BoundaryField
            return *this;
		}

//		_CC_Fv_Field <T> & operator=(const GeomField<T> field)
//		{
//		    this=&field;
//
//		    return *this;
//		}
		//Conveccion, interpolacion o reconstruccion, de solo una celda
		//THIS IS OLD. FIELD DOES NOT RETURN AN EQ SYSTEM BECAUSE REFER TO EQSYS AND THIS TO FVGRID
		//AT THE SAME TIME FVGRID REFERS TO GEOMFIELD AND THIS TO FIELD, AND FIELD TO EQSYS
		//const Eqn <T> ToFaces(const int &icell, const std::vector <Scalar> flux) const;

		//void ToCellCenters(EqnSystem <T> &eqnsys);


		_CC_Fv_Field<T> & operator=(const double &val)
		{
		    //cout << "Numvals"<<this->value.size()<<endl;
			for (int v = 0; v<this->value.size(); v++)
			{
				this->value[v] = val;
			}
			return *this;
		}

		_CC_Fv_Field<T> & operator=(const Vec3D &val)
		{
			for (int v = 0; v<this->value.size(); v++)
			{
				this->value[v] = val;
			}
			return *this;
		}


	};


	////////////////////////////
	// FUNCIONES DE TEMPLATES //
	////////////////////////////
	//Reconstruccion a las caras
	//Esta funcion es llamada por la divergencia
	//Devuelve una ecuacion que es como un stencil pero tiene a la vez la componente
	//de source
	//Esta funcion devuelve Sum n (-min[mf,0])
	//O lo que es lo mismo Si m<0 fi f=fi p si no fif=fiN
	//template < typename T >
	//const Eqn <T> _CC_Fv_Field<T>::ToFaces(const int &icell, std::vector <Scalar> flux) const
	//{
	//	vector <T> an;      //coeficientes de los vecinos
	//	T ap(0.);          //Lo inicio vacio, podria tb igualarlo a 0
	//	//Recorro las caras del cell
	//	//En el caso de upwind o CDS simple la molecula es siempre la misma

	//	for (int iface = 0; iface<GridPtr->Cell(icell).Num_Faces(); iface++)
	//	{
	//		T coef(0.);
	//		an.push_back(coef); //Agrego un vecino mas
	//		//
	//		if (flux[iface].Val()>0.)   //Flujo saliente, va a ap
	//			ap += flux[iface].Val();       //Este oprador suma un valor a derecha constante

	//		else if (flux[iface].Val()<0.)  //Flujo entrante, va al vecino
	//			an[iface] += flux[iface].Val();

	//	}
	//	//Creo la ecuacion con el coeficiente central y los vecinos
	//	//Como no especifico la fuente, esta es nula
	//	Eqn <T> eqn(ap, an);
	//	return eqn;


	//	//Field Output
	//	void Write_vtk();
	//}

	template <typename T>
	const Scalar MaxDiff(const _Field<T> &field1, const _Field<T> &field2);
	//#include "Field_Def.h"

	//Inner Product Field
//	//INHERITS FROM FIELD
//	template<typename T>
//	SurfaceField<typename innerProduct < T, T> ::type> operator &(const SurfaceField<T> &left,const SurfaceField<T> &right)
//	{
//	SurfaceField<typename innerProduct < T, T> ::type> ret(left.Numberofvals());
//	typename innerProduct < T, T> ::type val;
//	//Sizes must be equal and rank must be large than zero?
//	for (int c = 0; c < left.Numberofvals(); c++)
//	{
//		val = left.Val(c) & right.Val(c);
//		ret.Val(c,val);
//	}
//
//	return ret;
//    }
//
//    //TO MODIFY, SINTESIS
//    template<typename T>
//	_Surf_Fv_Field<typename innerProduct < T, T> ::type> operator &(const _Surf_Fv_Field<T> &left,const SurfaceField<T> &right)
//	{
//	_Surf_Fv_Field<typename innerProduct < T, T> ::type> ret(left.Numberofvals());
//	typename innerProduct < T, T> ::type val;
//	//Sizes must be equal and rank must be large than zero?
//	for (int c = 0; c < left.Numberofvals(); c++)
//	{
//		val = left.Val(c) & right.Val(c);
//		ret.Val(c,val);
//	}
//
//	return ret;
//    }


	template<typename T>
	_CC_Fv_Field<T> operator* (const Scalar &left,const _CC_Fv_Field<T> &right)
	{
	_CC_Fv_Field<T> ret(right.Grid());
	T val;
	//Sizes must be equal and rank must be large than zero?
	for (int c = 0; c < right.Numberofvals(); c++)
	{
		val = left.Val() * right.Val(c);
		ret.Val(c,val);
	}
	return ret;
    }

	template<typename T>
	_Surf_Fv_Field<T> operator* (const _Surf_Fv_Field<Scalar> &left,const _Surf_Fv_Field<T> &right)
	{
	//_Surf_Fv_Field<T> ret(left.Numberofvals()); THIS NOT WORKS, CREATE WITH GRID
	_Surf_Fv_Field<T> ret(right.Grid());
	T val;
	//Sizes must be equal and rank must be large than zero?
	for (int c = 0; c < left.Numberofvals(); c++)
	{
		val = left.Val(c) * right.Val(c);
		ret.Val(c,val);
	}

	return ret;
    }

}//FluxSol

#endif
