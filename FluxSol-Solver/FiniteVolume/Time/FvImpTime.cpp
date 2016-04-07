#include "Laplacian.h"

namespace FluxSol {


// FAST NEW LAPLACIAN
template<typename T>
EqnSystem <T> FvImp::Ddt(_CC_Fv_Field <T> &VolField)
{


    clock_t ittime_begin, ittime_end;
    double ittime_spent;


    ittime_end = clock();

    //cout << "Defining eqn..."<<endl;
	EqnSystem <T> eqnsys(VolField.Grid());

	T source;
	Scalar ap, an;  //IF FI IS A SCALAR THESE CAN BE SCALARS
	vector <int> nbr_eqn;

	//Internal field0
	//cout << "Face Number"<<VolField.Grid().Num_Faces()<<endl;
	//cout << "Cell Number"<<VolField.Grid().Num_Cells()<<endl;
	//cout << "Laplacian, sizeof intnetfluxface: "<<VolField.IntNetFluxFaces().size()<<endl;
	set <int> intfaces=VolField.IntNetFluxFaces();
	//for (std::set<int>::iterator it=VolField.IntNetFluxFaces().begin(); it!=VolField.IntNetFluxFaces().end(); ++it)

    ittime_spent = (double)(clock() - ittime_end) / CLOCKS_PER_SEC;
    ittime_end = clock();
    cout << "laplacian field gen "<<ittime_spent <<endl;


    ittime_end = clock();

    int pcellid;
    std::set<int>::iterator it;
	for (it=VolField.IntNetFluxFaces().begin(); it!=VolField.IntNetFluxFaces().end(); ++it)
    {
		//_FvFace face=VolField.Grid().Face(*it);

        //ap=-VolField.Grid().Face(*it).Norm_ad()/VolField.Grid().Face(*it).Dist_pn()*fi;

        //cout << "Look through neighbours"<<endl;
        for (int nb=0;nb<2;nb++)    //Face cells
        {
            pcellid=VolField.Grid().Face(*it).Cell(nb);

            eqnsys.Eqn(pcellid).Ap()+=ap;
            eqnsys.Eqn(pcellid).An(VolField.Grid().FaceLocalCellNeighbour(*it)[nb])-=ap;
        }

    }

    ittime_spent = (double)(clock() - ittime_end) / CLOCKS_PER_SEC;
    ittime_end = clock();
    cout << "laplacian interior faces loop "<<ittime_spent <<endl;

    ittime_end = clock();

	// BORDE - BOUNDARY
  // FASTER
	//cout << "Laplacian, look through boundary.."<<endl;
	// TO MODIFY: THIS IF MUST BE ONCE PER PATCH, NOT FOR FACE!!!!!
	// PATCH MUST BE A LIST OF NON NULL FLUX FACES
	for (int p=0;p<VolField.Grid().vBoundary().Num_Patches();p++)
	{
		//for (int f=0;f<VolField.Grid().vBoundary().vPatch(p).Num_Faces();f++)
		//{
			//int idface=VolField.Grid().vBoundary().vPatch(p).Id_Face(f);
			//_FvFace face=VolField.Grid().Face(idface);  //TO MODIFY idface or face pos??
			//cout << "BOUNDARY"<<endl;

            //if (!VolField.Grid().Face(idface).Is_Null_Flux_Face())
            //{
                //Boundary type
                //Instead of if sentence it is convenient to use inheritance
                //TO MODIFY!!! CHANGE TO INHERITED PATCHFIELD ACTION!!!!!
                if (VolField.Boundaryfield().PatchField(p).Type()==FIXEDVALUE)
                {
                    for (int f=0;f<VolField.Grid().vBoundary().vPatch(p).Num_Faces();f++)
                    {
                         int idface=VolField.Grid().vBoundary().vPatch(p).Id_Face(f);
                        //cout <<"FIXED VAL"<<endl;
//                        ap=-VolField.Grid().Face(idface).Norm_ad()/fabs(VolField.Grid().Face(idface).Dist_pf_LR(0))*fi;
                        source=VolField.Boundaryfield().PatchField(p).Val(f)*ap;
                        //cout <<"created" <<endl;
                        eqnsys.Eqn(VolField.Grid().Face(idface).Cell(0)).Ap()+=ap;
                        eqnsys.Eqn(VolField.Grid().Face(idface).Cell(0)).Source()+=source;
                    }
                }
                else if (VolField.Boundaryfield().PatchField(p).Type()==FIXEDGRADIENT)
                {
                    for (int f=0;f<VolField.Grid().vBoundary().vPatch(p).Num_Faces();f++)
                    {
                         int idface=VolField.Grid().vBoundary().vPatch(p).Id_Face(f);
                        //cout << "FIXED WIDTH"<<endl;
//                        source=VolField.Boundaryfield().PatchField(p).Val(f)*fi;
                        eqnsys.Eqn(VolField.Grid().Face(idface).Cell(0)).Source()+=source;
                    }
                }

	}
    //cout << "Returning laplacian"<<endl;
    ittime_spent = (double)(clock() - ittime_end) / CLOCKS_PER_SEC;
    ittime_end = clock();
    cout << "laplacian boundary faces loop "<<ittime_spent <<endl;

    return eqnsys;
}

} //FluxSol

#include "FvTime.inst"
