#include "Laplacian.h"

namespace FluxSol {


// To Modify, this can call a finite volume scheme
//111  return fv::ddtScheme<Type>::New
//112  (
//113  vf.mesh(),
//114  vf.mesh().ddtScheme
//115  (
//116  "ddt("
//117  + alpha.name() + ','
//118  + rho.name() + ','
//119  + vf.name() + ')'
//120  )
//121  )().fvmDdt(alpha, rho, vf);
//122 }


//template<class Type>
//tmp<fvMatrix<Type> >
//EulerDdtScheme<Type>::fvmDdt
//(
//    const GeometricField<Type, fvPatchField, volMesh>& vf
//)
//{
//    tmp<fvMatrix<Type> > tfvm
//    (
//        new fvMatrix<Type>
//        (
//            vf,
//            vf.dimensions()*dimVol/dimTime
//        )
//    );
//
//    fvMatrix<Type>& fvm = tfvm();
//
//    scalar rDeltaT = 1.0/mesh().time().deltaTValue();
//
//    fvm.diag() = rDeltaT*mesh().Vsc();
//
//    if (mesh().moving())
//    {
//        fvm.source() = rDeltaT*vf.oldTime().internalField()*mesh().Vsc0();
//    }
//    else
//    {
//        fvm.source() = rDeltaT*vf.oldTime().internalField()*mesh().Vsc();
//    }
//
//    return tfvm;
//}

//43  //- Return the top-level database
//244  const Time& time() const
//245  {
//246  return polyMesh::time();
//247  }

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

    cout << "Obtaining Delta t"<<endl;
    Scalar dt=VolField.Grid()._Time().Deltat();
    int c;
    for (c=0,VolField.Grid().cellit=VolField.Grid().BeginCell(); VolField.Grid().cellit!=VolField.Grid().EndCell(); VolField.Grid().cellit++,c++)
    {
        cout << "cell "<<c<<endl;
        ap=VolField.Grid().cellit->Vp()/dt;
        cout << "Obtaining previous field value"<<endl;
        eqnsys.Eqn(c).Source()=ap*VolField.PrevVal(c);
        cout << "Obtaining ap"<<endl;
        eqnsys.Eqn(c).Ap()=ap;

    }
    // BOUNDARY??

    //cout << "Returning laplacian"<<endl;
    ittime_spent = (double)(clock() - ittime_end) / CLOCKS_PER_SEC;
    ittime_end = clock();
    cout << "laplacian boundary faces loop "<<ittime_spent <<endl;

    return eqnsys;
}

} //FluxSol

#include "FvTime.inst"
