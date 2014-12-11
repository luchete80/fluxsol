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




	//boundary type
	enum _PatchFieldType { FIXEDVALUE, FIXEDGRADIENT };
		//IN FORMER FLUXSOL VERSIONS THIS CLASS WERE CALLED FVPATCHFIELD; NOW IS GENERIC
	template<typename T>
	class _PatchField :
		//public _Surf_Fv_Field<T>
		//IN FIRST FLUXSOL VERSIONS THIS CLASS INHERITED FROM SURF_FV_FIELD
		public _Field<T>//Originally it was an GeomField
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
		void AssignType(const _PatchFieldType &t) {this->type=t;}

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
        void AssignPatchFieldTypes(const _PatchFieldType &t)
		{
		    for (int pf=0;pf<this->patchfield.size();pf++)
                this->PatchField(pf).AssignType(t);
		}
		void Add_PatchField(_PatchField<T> &field)  { patchfield.push_back(field); }
        ~_BoundaryField(){}
	};



    /// HERE ALL THE OPERATORS
    //THIS AS BOUNDARY AND ALL THAT
    template<typename T>
    class GeomField:
    public _Field<T>
	{


	protected:
        //THIS IS A NEW FEATURE; A POINTER TO A BASE CLASS. THIS IS EXACTLY LIKE SURF_FV_FIELD WITHOUT OPERATORS
		_Grid *GridPtr;       //Se podria probar con un puntero general
		// en la clase base
		//Conviene colocar referencia o puntero??
		_BoundaryField <T> BoundaryField;	//En las caras

		GeomField<T> *temp;

	public:
		GeomField(const int &numval, const double &value = 0.) :_Field<T>(numval, value){}
		GeomField(){};         //Constructor
		virtual void SizeFromGrid(const _Grid *gPtr){};
		void AssignGrid(const _Grid *gPtr){this->GridPtr=gPtr;}
		void AssignPatchFieldTypes(const _PatchFieldType &t)
		{
		    this->BoundaryField.AssignPatchFieldTypes(t);
		}

        //GeomField(const GeomField<T> &field):_Field<T>(field.Numberofvals()){this->GridPtr=&field.Grid();}
		//NON VIRTUAL!
		virtual _Grid & Grid(){ return *GridPtr; }
		const _Grid  & ConstGrid()const{ return this->GridPtr; }
		//Conveccion, interpolacion o reconstruccion, de solo una celda
		//THIS IS OLD. FIELD DOES NOT RETURN AN EQ SYSTEM BECAUSE REFER TO EQSYS AND THIS TO FVGRID
		//AT THE SAME TIME FVGRID REFERS TO GEOMFIELD AND THIS TO FIELD, AND FIELD TO EQSYS
		//const Eqn <T> ToFaces(const int &icell, const std::vector <Scalar> flux) const;

		_BoundaryField <T> &  Boundaryfield(){ return BoundaryField; }


        GeomField <T>  operator+ (const GeomField <T> &right)
        {
            //GeomField<T> *ret=new GeomField<T>(this->Numberofvals());
            GeomField<T> ret(this->Numberofvals());
            ret.GridPtr=&this->Grid();
            T val;
            //Sizes must be equal and rank must be large than zero?
            for (int c = 0; c < this->Numberofvals(); c++)
            {
                val = this->Val(c) + right.Val(c);
                ret.Val(c,val);
            }

            return ret;
        }

        GeomField <T>  operator- (const GeomField <T> &right)
        {
            //GeomField<T> *ret=new GeomField<T>(this->Numberofvals());
            //TO MODIFY: USE COPY CONSTRUCTOR OR OPERATOR=
            //POINTER MUST NOT BE PASSED HERE
            GeomField<T> ret(this->Numberofvals());
            //THIS IS WRONG
            ret.GridPtr=this->GridPtr;

            //GeomField<T> ret(this->Grid());
            T val;
            //Sizes must be equal and rank must be large than zero?
            for (int c = 0; c < this->Numberofvals(); c++)
            {
                val = this->Val(c) - right.Val(c);
                ret.Val(c,val);
            }

            return ret;
        }

                //UNARY
		GeomField<T> operator-()
		{
		    //GeomField<T> ret(*this);
            GeomField<T> ret(this->Numberofvals());
            ret.GridPtr=this->GridPtr;

			for (int v = 0; v<this->value.size(); v++)
			{
				ret.value[v] = -this->value[v];
			}
			return ret;
		}

        GeomField<typename innerProduct < T, T> ::type> operator&(const GeomField<T> &right)
        {
                //temp=new GeomField<typename innerProduct < T, T> ::type>(this->Numberofvals());
                GeomField<typename innerProduct < T, T> ::type> ret(this->Numberofvals());
                ret.AssignGrid(&right.Grid());
                typename innerProduct < T, T> ::type val;
                //Sizes must be equal and rank must be large than zero?
//                cout << "Number of vals" <<this->Numberofvals()<<endl;
                for (int c = 0; c < this->Numberofvals(); c++)
                {
                    val = this->Val(c) & right.Val(c);
                    ret.Val(c,val);
                }

                return ret;

        }

        friend const GeomField <T>  operator/ (const T &left, const GeomField <T> &right)
        {
                    //GeomField<T> *ret=new GeomField<T>(this->Numberofvals());
            //TO MODIFY: USE COPY CONSTRUCTOR OR OPERATOR=
            //POINTER MUST NOT BE PASSED HERE
            GeomField<T> ret(right.Numberofvals());
            //THIS IS WRONG
            ret.AssignGrid(&right.Grid());

            //GeomField<T> ret(this->Grid());
            T val;
            //Sizes must be equal and rank must be large than zero?
            for (int c = 0; c < right.Numberofvals(); c++)
            {
                val = left / right.Val(c);
                cout << "val" << val.outstr()<<endl;
                ret.Val(c,val);
            }

            return ret;
        }

        //~GeomField(){};

	};

	//Binary
//    template <typename T>
//    const GeomField <T>  operator/ (const T &left, const GeomField <T> &right)
//        {
//            //GeomField<T> *ret=new GeomField<T>(this->Numberofvals());
//            //TO MODIFY: USE COPY CONSTRUCTOR OR OPERATOR=
//            //POINTER MUST NOT BE PASSED HERE
//            GeomField<T> ret(right.Numberofvals());
//            //THIS IS WRONG
//            ret.GridPtr=right.GridPtr();
//
//            //GeomField<T> ret(this->Grid());
//            T val;
//            //Sizes must be equal and rank must be large than zero?
//            for (int c = 0; c < right.Numberofvals(); c++)
//            {
//                val = left / right.Val(c);
//                ret.Val(c,val);
//            }
//
//            return ret;
//        }


		template<typename T>
	class GeomSurfaceField :
	    //public _Field<T>
	    public GeomField<T>
	{

	protected:

	public:
		GeomSurfaceField(const int &numval, const double &value = 0.) :GeomField<T>(numval, value){}
		GeomSurfaceField(const GeomField<T> &field):GeomField<T>(field){}
		GeomSurfaceField(){};         //Constructor

        //CenterToInterpolation crashes if it is not specified
        GeomSurfaceField <T> & operator=(const GeomField<T> &field)
		{
            this->value.clear();
            T v;
            this->value.assign(field.Numberofvals(),v);
            this->GridPtr=&field.Grid();
            for (int c=0;c<field.Numberofvals();c++)
                this->value[c]=field.Val(c);
            //TO MODIFY
            //this->Boundaryfield()=field->BoundaryField
            return *this;

		}

        GeomSurfaceField <T> & operator=(const T &val)
		{

            for (int c=0;c<this->Numberofvals();c++)
                this->value[c]=val;
            //TO MODIFY
            //this->Boundaryfield()=field->BoundaryField
            return *this;

		}

        //TO MODIFY, WITH LIST OF INTERNAL
//        GeomSurfaceField <T> & AssignBoundaryVal(const T &val)
//		{
//            for (int c=0;c<this->Numberofvals();c++)
//            if(this->)
//                this->value[c]=val;
//            //TO MODIFY
//            //this->Boundaryfield()=field->BoundaryField
//            return *this;
//
//		}

        GeomSurfaceField <T> & operator=(const double &val)
		{

            for (int c=0;c<this->Numberofvals();c++)
                this->value[c]=val;
            //TO MODIFY
            //this->Boundaryfield()=field->BoundaryField
            return *this;

		}
		virtual ~GeomSurfaceField(){};
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


	public:

		//EVALUATE IF IT IS CONST
		//Fv_CC_Grid  *GridPtr;       //Se podria probar con un puntero general

		GeomVolField(const _Grid &grid);

        virtual void SizeFromGrid(const _Grid *gPtr)
        {
            this->GridPtr=gPtr;
            this->value.clear();
            //this->value.assign(field.Numberofvals(),v);

        }

            //COMPONENT TO COMPONENT PRODUCT
        //template <typename T>
        friend GeomField<T> operator* (const GeomField<Scalar> &left,const GeomField<T> &right)
        {
            GeomField<T> ret(left.Numberofvals());
           // ret.GridPtr=&left.Grid();
            T val;
            //Sizes must be equal and rank must be large than zero?
            for (int c = 0; c < left.Numberofvals(); c++)
            {
                val = left.Val(c) * right.Val(c);
                ret.Val(c,val);
            }
            return ret;
        }

		//_CC_Fv_Field (InputFile &inputfile);

		GeomVolField(){};

		//void ToCellCenters(EqnSystem <T> &eqnsys);


	};


//
//    template<typename T>
//	GeomField<T> operator* (const GeomField<Scalar> &left,const GeomField<T> &right)
//	{
//	GeomField<T> ret(left.Numberofvals());
//	T val;
//	//Sizes must be equal and rank must be large than zero?
//	for (int c = 0; c < left.Numberofvals(); c++)
//	{
//		val = left.Val(c) * right.Val(c);
//		ret.Val(c,val);
//	}
//	return ret;
//	}




}//FluxSol

#endif
