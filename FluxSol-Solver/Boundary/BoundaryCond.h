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


#ifndef _BOUNDARYCOND_H_
#define _BOUNDARYCOND_H_

#include "FvGrid.h"
#include "GeometricField.h"
#include "FvField.h"


//Boundary Cond is different to PatchField type.
//BC refers to CFD patch typpe, and this sets differents patch field types (fixed value, fixed gradient)
namespace FluxSol{

class BoundaryCond
{
    protected:

    Fv_CC_Grid *grid;
    int patch;  //Which patch refers

    Patch *patchptr;
    _PatchField <Vec3D> *ufieldptr;
    _PatchField <Scalar> *pfieldptr;


    public:

    BoundaryCond(){}
    BoundaryCond(const Fv_CC_Grid *g,const int &p){this->grid=g;this->patch=p;this->patchptr=&grid->vBoundary().vPatch(p);}

    BoundaryCond(_CC_Fv_Field <Vec3D> *u, _CC_Fv_Field <Scalar>  *p, const int &pa) //Fields and patch number
    {
        //cout << "Assign grid";
        this->grid=&u->Grid();

        //cout << "Assign Patch"<<endl;
        this->patch=pa;
        this->patchptr=&grid->vBoundary().vPatch(pa);

        //cout << "Assign field ptr";

        ufieldptr=&u->Boundaryfield().PatchField(pa);
        pfieldptr=&p->Boundaryfield().PatchField(pa);

    }


    BoundaryCond(const int &p){this->patch=p;}


    virtual void AssignPatchFieldType(){}

    const int PatchId()const{return this->patch;}

    Patch * PatchPtr(){return this->patchptr;}

    _PatchField<Vec3D>  * UFieldPtr(){return this->ufieldptr;}
    _PatchField<Scalar> * pFieldPtr(){return this->pfieldptr;}

};

class WallBC:
    public BoundaryCond

{

    public:
        WallBC(){}
        WallBC(_CC_Fv_Field <Vec3D> *u, _CC_Fv_Field <Scalar>  *p, const int &pa):
            BoundaryCond(u, p, pa){}

        void AssignPatchFieldType()
        {
            this->ufieldptr->AssignType(FIXEDVALUE);
            this->pfieldptr->AssignType(FIXEDGRADIENT);
        }
};

class InletBC:
    public BoundaryCond

{

    public:
        InletBC(){}
        InletBC(_CC_Fv_Field <Vec3D> *u, _CC_Fv_Field <Scalar>  *p, const int &pa):
            BoundaryCond(u, p, pa){}

        void AssignPatchFieldType(){}
};

class VelocityInletBC:
    public BoundaryCond

{

    public:
        VelocityInletBC(){}
        VelocityInletBC(_CC_Fv_Field <Vec3D> *u, _CC_Fv_Field <Scalar>  *p, const int &pa):
            BoundaryCond(u, p, pa){}

        void AssignPatchFieldType()
        {
            this->ufieldptr->AssignType(FIXEDVALUE);
            this->pfieldptr->AssignType(FIXEDGRADIENT);
        }
};


class OutletBC:
    public BoundaryCond

{

    public:
        OutletBC(){}
        OutletBC(_CC_Fv_Field <Vec3D> *u, _CC_Fv_Field <Scalar>  *p, const int &pa):
            BoundaryCond(u, p, pa){}

        void AssignPatchFieldType(){}
};

class PressureOutletBC:
    public BoundaryCond

{

    public:
        PressureOutletBC(){}
        PressureOutletBC(_CC_Fv_Field <Vec3D> *u, _CC_Fv_Field <Scalar>  *p, const int &pa):
            BoundaryCond(u, p, pa){}

        void AssignPatchFieldType()
        {
            this->ufieldptr->AssignType(FIXEDGRADIENT);
            this->pfieldptr->AssignType(FIXEDVALUE);
        }
};


class PressureInletBC:
    public BoundaryCond

{

    public:
        PressureInletBC(){}
        PressureInletBC(_CC_Fv_Field <Vec3D> *u, _CC_Fv_Field <Scalar>  *p, const int &pa):
            BoundaryCond(u, p, pa){}

        void AssignPatchFieldType()
        {
            this->ufieldptr->AssignType(FIXEDGRADIENT);
            this->pfieldptr->AssignType(FIXEDVALUE);
        }
};

class NullFluxBC:
    public BoundaryCond

{

    public:
        NullFluxBC(){}
        NullFluxBC(_CC_Fv_Field <Vec3D> *u, _CC_Fv_Field <Scalar>  *p, const int &pa):
            BoundaryCond(u, p, pa){}

        void AssignPatchFieldType()
        {
            for (int f=0;f<this->patchptr->Num_Faces();f++)
            {
                int idface=this->patchptr->Id_Face(f);
                this->grid->Face(idface).Null_Flux_Face(true);

                this->ufieldptr->AssignType(NULL_FLUX);
                this->pfieldptr->AssignType(NULL_FLUX);
            }
        }
};

}//FluxSol

#endif // _BOUNDARYCOND_H
