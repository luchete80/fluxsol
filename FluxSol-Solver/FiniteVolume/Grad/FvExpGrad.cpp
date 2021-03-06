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
#include "FvExpGrad.h"

using namespace std;
namespace FluxSol
{
    namespace FvExp
    {
    //////////////////////
    // FUNCIONES DE GRADIENTE EXPLICITO
    ///////////////////////////////////

//    //
//    //Para que sigue OpenFoam con los tipos en la template??
//    //(const GeometricField <Type, fvs_PatchField, surfaceMesh>&)
//    template<class T>
//    _CC_Fv_Field
//    < typename outerProduct<Vec3D, T>::type >
//      Grad (const _CC_Fv_Field <T>& field)
//      {
//
//              //Field to return
//               _CC_Fv_Field < typename outerProduct<Vec3D, T>::type > r(field.Grid());
//              //Paso el flujo a las caras, a un campo de faces
//
//              //Tengo que hacer el prod "externo" entre un T a izquierda y un vector (rank=1) a derecha
//
//               //Like the open Foam functions
//               //_Surf_Fv_Field < typename outerProduct<Vec3D, T>::type > facefield=field.FaceInterpolate();
//               //Formerly was SurfaceField
//        GeomSurfaceField <T> facefi=Interpolate(field);
//
//        //cout << "interpolated to grad:" << facefi.outstr()<<endl;
//
//               bool end = false;
//               //Begin Main Loop
//               //facefi and r (which is the corrected gauss gradient of fieldnc) are changing
//               _CC_Fv_Field < typename outerProduct<Vec3D, T>::type > rant;
//
//               //FOR NON ORTHOGONAL ITERATIONS
//               //while (!end)
//               //{
//                       r=0.;
//                       //Loop through cells to calculate Gauss Gradient
//                       int c;
//                       for (c=0,r.Grid().cellit=r.Grid().BeginCell();r.Grid().cellit!=r.Grid().EndCell();r.Grid().cellit++,c++)
//                       {
//                              //External product between a T field and a vector
//                              //Grad Cell Center = 1/Vp * Sum_faces(external prod(field fi * FaceArea))
//                              //Loop trough cell faces
//                              for (int cellface=0;cellface<r.Grid().cellit->Num_Faces();cellface++)
//                              {
//                                      int f = r.Grid().cellit->Id_Face(cellface);
//                                      //This is fi_f Outer Af
//                                      //r[c]+=(facefi[f]*r.Grid().Face(f).Af());
//                                      r[c]+=facefi[f]*r.Grid().CellFaceAf_Slow(c,cellface);
//
//                              }
//                              //Divide by cell volume
//                              r[c]=r[c]/r.Grid().cellit->Vp();
//
//                       }
//                       //Average of fi and fi (fio) gradient (gradfio)
//                       //Look throug faces to obtain the facefield
//                       //fi_f = fio + gradfio & fo-f//Here is inner product between different types
//                       //With fo-f = Pf - Pfo = Pf - (Pf&ePN)ePN is the projection
//                       //Dist_pf_LR
//
////TO MODIFY: NON ORTHOGONAL CORRECTIONS
////                     for (int f=0;f<r.Grid().Num_Faces();f++)
////                     {
////                             //f-fo is  unique for each face, but it can be calculated either with P or N cells
////                             _FvFace face = r.Grid().Face(f);
////                             Vec3D fof=face.Dist_pf_LR(0)-(face.Dist_pf_LR(0)&face.e_PN())*face.e_PN();
////
////                            //Variable and gradient averages
////                             T fifo = face.Fp()*fieldnc[face.Cell(0)]+(1.0-face.Fp())*fieldnc[face.Cell(1)];
////                            typename outerProduct<Vec3D, T>::type grad_fio=(r[face.Cell(0)]*(1-face.Fp())+r[face.Cell(1)]*face.Fp());
////                            //typename outerProduct<Vec3D, T>::type grad_fio=r[face.Cell(0)];
////
////                            //facefi [f] = (fifo+(grad_fio&fof));
////                            facefi [f] = fifo;
////                            T s1,s2;
////                            s1=s2;
////
////                     }
////
////
////                     //Compare r against rant
////                     //With MaxDiff Field template function
////                     Scalar maxdif=MaxDiff(r,rant);
////                     rant = r;
//
//
//               //}//While End
//
//
//
//               return r;
//
//      }


   // GRADIENT FAST NEW CALCULATION
    //Para que sigue OpenFoam con los tipos en la template??
    //(const GeometricField <Type, fvs_PatchField, surfaceMesh>&)
    template<class T>
    _CC_Fv_Field
    < typename outerProduct<Vec3D, T>::type >
        Grad (const _CC_Fv_Field <T>& field)
        {

                //Field to return
                 _CC_Fv_Field < typename outerProduct<Vec3D, T>::type > r(field.Grid());

        GeomSurfaceField <T> facefi=Interpolate(field);

        //cout << "interpolated to grad:" << facefi.outstr()<<endl;

        double afdir[2];afdir[0]=1.;afdir[1]=-1.;
        std::set<int>::iterator it;
        std::set<int> *nff=&field.IntNetFluxFaces();

        for (int f=0;f<r.Grid().Num_Faces();f++)
        //for (it=nff->begin(); it!=nff->end(); ++it)
        {
            //for (int fc=0;fc<field.Grid().Face(*it).NumCells();fc++)
            for (int fc=0;fc<r.Grid().Face(f).NumCells();fc++)
            {
                int c=r.Grid().Face(f).Cell(fc);
                r[c]+=r.Grid().Face(f).Af()*facefi[f]*afdir[fc];

//                int c=r.Grid().Face(*it).Cell(fc);
//                r[c]+=r.Grid().Face(*it).Af()*facefi[*it]*afdir[fc];
            }
        }

                //BOUNDARY
//        int fid;    //Global face id
//        for (int p=0;p<field.Grid().vBoundary().Num_Patches();p++)
//        {
//            if (field.Boundaryfield().PatchField(p).Type()!=NULL_FLUX)
//            {
//                for (int f=0;f<r.Grid().vBoundary().vPatch(p).Num_Faces();f++)
//                {
//                        fid=field.Grid().vBoundary().vPatch(p).Id_Face(f);
//                        int c=field.Grid().Face(fid).Cell(0);
//                        r[c]+=r[c]+=field.Grid().Face(fid).Af()*facefi[fid]*afdir[0];
//                }
//            }
//        }


        int c;
        for (c=0,r.Grid().cellit=r.Grid().BeginCell();r.Grid().cellit!=r.Grid().EndCell();r.Grid().cellit++,c++)
            r[c]=r[c]/r.Grid().cellit->Vp();


                 return r;

        }
//          template<class T>
//    _CC_Fv_Field
//    < typename outerProduct<Vec3D, T>::type >
//      GradV (const _CC_Fv_Field <T>& field)
//      {
//
//              //Field to return
//               _CC_Fv_Field < typename outerProduct<Vec3D, T>::type > r(field.Grid());
//              //Paso el flujo a las caras, a un campo de faces
//
//              //Tengo que hacer el prod "externo" entre un T a izquierda y un vector (rank=1) a derecha
//
//               //Like the open Foam functions
//               //_Surf_Fv_Field < typename outerProduct<Vec3D, T>::type > facefield=field.FaceInterpolate();
//               //Formerly was SurfaceField
//        GeomSurfaceField <T> facefi=Interpolate(field);
//
//        //cout << "interpolated to grad:" << facefi.outstr()<<endl;
//
//               bool end = false;
//               //Begin Main Loop
//               //facefi and r (which is the corrected gauss gradient of fieldnc) are changing
//               _CC_Fv_Field < typename outerProduct<Vec3D, T>::type > rant;
//
//               //FOR NON ORTHOGONAL ITERATIONS
//               //while (!end)
//               //{
//                       r=0.;
//                       //Loop through cells to calculate Gauss Gradient
//                       int c;
//                       for (c=0,r.Grid().cellit=r.Grid().BeginCell();r.Grid().cellit!=r.Grid().EndCell();r.Grid().cellit++,c++)
//                       {
//                              //External product between a T field and a vector
//                              //Grad Cell Center = 1/Vp * Sum_faces(external prod(field fi * FaceArea))
//                              //Loop trough cell faces
//                              for (int cellface=0;cellface<r.Grid().cellit->Num_Faces();cellface++)
//                              {
//                                      int f = r.Grid().cellit->Id_Face(cellface);
//                                      //This is fi_f Outer Af
//                                      //r[c]+=(facefi[f]*r.Grid().Face(f).Af());
//                                      r[c]+=facefi[f]*r.Grid().CellFaceAf_Slow(c,cellface);
//
//                                      //cout << "Cell Face" <<r.Grid().CellFaceAf_Slow(c,cellface).Val().outstr()<<endl;
//
//                              }
//                              //NOT Divide by cell volume
//                              //cout << "Cell vol "<<(r.Grid().Cell(c).Vp()).outstr();
//                              //cout << "Field Val" << r[c].outstr()<< endl;
//
//                       }
//
//                       //Average of fi and fi (fio) gradient (gradfio)
//                       //Look throug faces to obtain the facefield
//                       //fi_f = fio + gradfio & fo-f//Here is inner product between different types
//                       //With fo-f = Pf - Pfo = Pf - (Pf&ePN)ePN is the projection
//                       //Dist_pf_LR
//
////TO MODIFY: NON ORTHOGONAL CORRECTIONS
////                     for (int f=0;f<r.Grid().Num_Faces();f++)
////                     {
////                             //f-fo is  unique for each face, but it can be calculated either with P or N cells
////                             _FvFace face = r.Grid().Face(f);
////                             Vec3D fof=face.Dist_pf_LR(0)-(face.Dist_pf_LR(0)&face.e_PN())*face.e_PN();
////
////                            //Variable and gradient averages
////                             T fifo = face.Fp()*fieldnc[face.Cell(0)]+(1.0-face.Fp())*fieldnc[face.Cell(1)];
////                            typename outerProduct<Vec3D, T>::type grad_fio=(r[face.Cell(0)]*(1-face.Fp())+r[face.Cell(1)]*face.Fp());
////                            //typename outerProduct<Vec3D, T>::type grad_fio=r[face.Cell(0)];
////
////                            //facefi [f] = (fifo+(grad_fio&fof));
////                            facefi [f] = fifo;
////                            T s1,s2;
////                            s1=s2;
////
////                     }
////
////
////                     //Compare r against rant
////                     //With MaxDiff Field template function
////                     Scalar maxdif=MaxDiff(r,rant);
////                     rant = r;
//
//
//               //}//While End
//
//
//
//               return r;
//
//      }

   // GRADIENT FAST NEW CALCULATION
    //Para que sigue OpenFoam con los tipos en la template??
    //(const GeometricField <Type, fvs_PatchField, surfaceMesh>&)
     template<class T>
    _CC_Fv_Field
    < typename outerProduct<Vec3D, T>::type >
        GradV (const _CC_Fv_Field <T>& field)
        {

                //Field to return
                 _CC_Fv_Field < typename outerProduct<Vec3D, T>::type > r(field.Grid());

        GeomSurfaceField <T> facefi=Interpolate(field);

        //cout << "interpolated to grad:" << facefi.outstr()<<endl;

        double afdir[2];afdir[0]=1.;afdir[1]=-1.;
        for (int f=0;f<r.Grid().Num_Faces();f++)
        {
            for (int fc=0;fc<r.Grid().Face(f).NumCells();fc++)
            {
                int c=r.Grid().Face(f).Cell(fc);
                r[c]+=r.Grid().Face(f).Af()*facefi[f]*afdir[fc];
            }
        }
                 return r;

        }

    //Face gradient
    //Explicit form of FaceGrad
    //Like Laplacian, (N-P)dist + Non-Orthogonal
    //TO MODIFY!!!!
    //CHANGE BY SnGradf
    template<class T>
    _Surf_Fv_Field
    //< typename outerProduct<Vec3D, T>::type >
    //TILL NOW IS NOT PASSED BY REF
    < T >
        SnGrad (const _CC_Fv_Field <T> &VolField)
        {
            _Surf_Fv_Field <T> r(VolField.ConstGrid());


        T source;
        Scalar ap, an;  //IF FI IS A SCALAR THESE CAN BE SCALARS
        vector <int> nbr_eqn;

        //Internal field
//        cout << "Face Number"<<VolField.Grid().Num_Faces()<<endl;
//        cout << "Cell Number"<<VolField.Grid().Num_Cells()<<endl;

//        cout << "field values "<<r.Numberofvals()<<endl;

        T grad=0.;

        set <int> intfaces=VolField.IntNetFluxFaces();
        for (std::set<int>::iterator it=intfaces.begin(); it!=intfaces.end(); ++it)
        //for (int f=0;f<VolField.Grid().Num_Faces();f++)
        {
            int f=*it;
//            //cout << "Face "<<f<<endl;
//            _FvFace face=VolField.Grid().Face(f);
//            if (!face.Is_Null_Flux_Face())
//            {
//                if (!VolField.Grid().Face(f).Boundaryface())
//                {
//                    //cout << "Not boundary face"<<endl;
//                    //nbr_eqn.push_back(VolField.Grid().Cell(c));
//                    //eqnsys.Eqn(face.Cell(0)).Coeffs(ap,an);
                    int pid=VolField.Grid().Face(f).Cell(0);
                    int nid=VolField.Grid().Face(f).Cell(1);

//                    cout << "pid nid" << pid << " " <<nid<<endl;


                    T grad=(VolField.Val(nid).Val()-VolField.Val(pid).Val())*VolField.Grid().Face(f).Norm_ad()/VolField.Grid().Face(f).Dist_pn();
                   // r.Val(f, grad);
                   //cout << "Face "<<f<<endl;
                   //cout << "grad val " << grad.Val()<<endl;
                   r[f]=grad;
//                }
//            }//End if !NullFluxFace

        }//End look trough faces
//
//        //BOUNDARY
//        //cout << "Laplacian, look through boundary.."<<endl;
//        for (int p=0;p<VolField.Grid().vBoundary().Num_Patches();p++)
//        {
//            for (int f=0;f<VolField.Grid().vBoundary().vPatch(p).Num_Faces();f++)
//            {
//                int idface=VolField.Grid().vBoundary().vPatch(p).Id_Face(f);
//                _FvFace face=VolField.Grid().Face(idface);  //TO MODIFY idface or face pos??
//                //Boundary type
//                //Instead of if sentence it is convenient to use inheritance
//                T pval= VolField.Val(face.Cell(0)).Val();
//
//                if (VolField.Boundaryfield().PatchField(p).Type()==FIXEDVALUE)
//                {
//                    //cout <<"source"<<endl;
//                    ap=-face.Norm_ad()/fabs(face.Dist_pf_LR(0));
//                    T fval =VolField.Boundaryfield().PatchField(p).Val(f);
//
//                    T grad=(fval-pval)*face.Norm_ad()/fabs(face.Dist_pf_LR(0));
//                    //cout <<"created" <<endl;
//                    //eqnsys.Eqn(face.Cell(0)).Ap()+=ap;
//                    //eqnsys.Eqn(face.Cell(0)).Source()+=source;
//                }
//                else if (VolField.Boundaryfield().PatchField(p).Type()==FIXEDGRADIENT)
//                {
//                    source=VolField.Boundaryfield().PatchField(p).Val(f);
//                    //eqnsys.Eqn(face.Cell(0)).Source()+=source;
//                }
//            }
//
//        }
        return r;
        }//Enf of Gradf

        // New function for non-Orthogonal correction

//                       //Average of fi and fi (fio) gradient (gradfio)
//                       //Look throug faces to obtain the facefield
//                       //fi_f = fio + gradfio & fo-f//Here is inner product between different types
//                         //With fo-f = Pf - Pfo = Pf - (Pf&ePN)ePN is the projection
        //FIELDS ARE
        //1 - GRAD_FI (AT CELLS)
        // 2 - GRAD_FI_FO (AT FACES, INTERPOLATED)
        // FACEFI=AT FACES
        // FI_FO = (AT FACES )


        //AT FIRST IS A GAUSS GRADIENT
        //TO MODIFY: USE ANOTHER TYPE OF GRADIENTAND CALL GRAD SChEME

//          //THIS IS THE OLD SEZAI
//        template<class T>
//        _CC_Fv_Field
//        <typename outerProduct<Vec3D, T>::type >
//        NonOrthGrad(const _CC_Fv_Field <T> & field)
//        {
//
//            _CC_Fv_Field < typename outerProduct<Vec3D, T>::type > r(field.Grid());
//
//            int cell[2];
//            std::set<int>::iterator it;
//            std::set<int> *nff=&field.IntNetFluxFaces();
//
//            //Cell center Gauss Gradient
//            _CC_Fv_Field < typename outerProduct<Vec3D, T>::type > gradp= FvExp::Grad (field);
//
//
//
//            //Field Interpolation
//            _CC_Fv_Field < typename outerProduct<Vec3D, T>::type > fi_fo(field.Grid());
//
//            //Gradient Interpolation
//            GeomSurfaceField < typename outerProduct<Vec3D, T>::type > grad_fo = Interpolate(gradp);
//
//            GeomSurfaceField <T> field_f(field);
//
//            GeomSurfaceField <T> facefi=Interpolate(field);    //INITIAL VALUES OF FACEFI
//            Vec3D fo_f;
//
//            double afdir[2];afdir[0]=1.;afdir[1]=-1.;
//
//            bool end=false;
//            int numcorr=5;
//            int corrit=0;
//
//
//            // NON ORTHOGONAL CORRECTIONS
//            // Calculate gradient at faces
//            while (!end)
//            {
//
//                fi_fo=0.;
//                for (int f=0;f<r.Grid().Num_Faces();f++)
//                {
//                    for (int fc=0;fc<r.Grid().Face(f).NumCells();fc++)
//                    {
//                        int c=r.Grid().Face(f).Cell(fc);
//                        fi_fo[c]+=r.Grid().Face(f).Af()*facefi[f]*afdir[fc];
//                    }
//                }
//
//                r=0.;
//                //Loop through Net Flux Faces (nff) via iterator
//                for (it=nff->begin(); it!=nff->end(); ++it)
//                {
//                    int f=*it;
//                    cell[0]=field.ConstGrid().Face(f).Cell(0);
//
//                    fo_f=   field.ConstGrid().Face(f).Dist_pf_LR(0)- (
//                            ( field.ConstGrid().Face(f).Dist_pf_LR(0) & (field.ConstGrid().Face(f).e_PN()) ) *
//                             field.ConstGrid().Face(f).e_PN() );
//    //                       //Dist_pf_LR
//
//                    field_f[f]=fi_fo[f]+grad_fo[f]&fo_f;        // EQN (2)
//
//                    //For each cell neighbour to face
//                    for (int c=0;c<2;c++)
//                    {
//                        cell[c]=field.ConstGrid().Face(f).Cell(c);
//                        r[ cell[c] ] += field_f [f] * field.ConstGrid().Face(f).Af();
//                    }
//
//                        int c;
//                    for (c=0,r.Grid().cellit=r.Grid().BeginCell(); r.Grid().cellit!=r.Grid().EndCell(); r.Grid().cellit++,c++)
//                        r[c]=r[c]/r.Grid().cellit->Vp();
//
//
//                    grad_fo = Interpolate(r);
//
//                }   //For net flux faces
//
//
//                if (corrit <=numcorr)   end=true;
//                corrit++;
//            }
//
//            return r;
//        } // End of NonOrthGrad


        //FROM NOW ON IS APPLIED FERZINGER-PERIC
        //CORRECTION

        template<class T>
        _CC_Fv_Field
        <typename outerProduct<Vec3D, T>::type >
        NonOrthGrad(const _CC_Fv_Field <T> & field)
        {

            _CC_Fv_Field < typename outerProduct<Vec3D, T>::type > r(field.Grid());

            int cell[2];
            std::set<int>::iterator it;
            std::set<int> *nff=&field.IntNetFluxFaces();

            //Cell center Gauss Gradient
            _CC_Fv_Field < typename outerProduct<Vec3D, T>::type > gradp= FvExp::Grad (field);



            //Field Interpolation
            //_CC_Fv_Field < typename outerProduct<Vec3D, T>::type > fi_fo(field.Grid());

            //Gradient Interpolation
            GeomSurfaceField < typename outerProduct<Vec3D, T>::type > grad_fo = Interpolate(gradp);

            GeomSurfaceField <T> field_f(field);

            //FI_FO IS A CONSTANT VALUE
            GeomSurfaceField <T> fi_fo=Interpolate(field);    //INITIAL VALUES OF FACEFI

            field_f=fi_fo;  //Initial value


            Vec3D fo_f;

            double afdir[2];afdir[0]=1.;afdir[1]=-1.;

            bool end=false;
            int numcorr=5;
            int corrit=0;


            // NON ORTHOGONAL CORRECTIONS
            // Calculate gradient at faces
            cout << "Loop Through faces"<<endl;
            while (!end)
            {

                r=0.;
                cout << "Creating gradient r"<<endl;
                 // EQN (1) GRADIENT CALC
                 // INTERNAL OR EXTERNAL??
                for (int f=0;f<r.Grid().Num_Faces();f++)
                {
                    for (int fc=0;fc<r.Grid().Face(f).NumCells();fc++)
                    {
                        int c=r.Grid().Face(f).Cell(fc);
                        r[c]+=r.Grid().Face(f).Af()*field_f[f]*afdir[fc];
                    }
                }

                int c;
                for (c=0,r.Grid().cellit=r.Grid().BeginCell(); r.Grid().cellit!=r.Grid().EndCell(); r.Grid().cellit++,c++)
                    r[c]=r[c]/r.Grid().cellit->Vp();

                grad_fo = Interpolate(r);

                //Loop through Net Flux Faces (nff) via iterator
                //ALL FACES IS NOT SENSE, BOUNDARY FACES HAVE ZERO FO_F VALUE
                cout << "Loop trough faces"<<endl;
                for (it=nff->begin(); it!=nff->end(); ++it)
                //for (int f=0;f<r.Grid().Num_Faces();f++)
                {
                    int f=*it;
                    cell[0]=field.ConstGrid().Face(f).Cell(0);

                    cout << "PF"<<field.ConstGrid().Face(f).Dist_pf_LR(0) <<endl;
//                    fo_f=   field.ConstGrid().Face(f).Dist_pf_LR(0)- (
//                            ( field.ConstGrid().Face(f).Dist_pf_LR(0) & (field.ConstGrid().Face(f).e_PN()) ) *
//                             field.ConstGrid().Face(f).e_PN() );
//    //                       //Dist_pf_LR
                    cout << "dist fo_f"<<endl;
                    cout << fo_f.outstr()<<endl;

                    field_f[f]=fi_fo[f]+(grad_fo[f]&field.ConstGrid().Face(f).fof());        // EQN (2)


                }   //For net flux faces
                cout <<"end of iteration"<<endl;

                if (corrit <=numcorr)   end=true;
                corrit++;
            }
            cout << "returning grad"<<endl;
            cout <<r.outstr()<<endl;
            return r;
        } // End of NonOrthGrad





    }//Fin de FvExp
}//Fin de namespace FluxSol

#include "FvGrad.inst"
