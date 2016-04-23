
#include "StatThermalModel.h"

using namespace std;
using namespace FluxSol;


void StatThermalModel::SolveIter()
{
    //till down here this is like examples 6 and 15
    /////////////////////////////////
    //TEMPERATURE
    /////////////////////////////////
    //Flux for transport eqn is cT=h
    //d(rho c T)/dt + Div(rho c T U) - Div . (k Grad (cT) ) = ScT
    //tauij=mu(dui/dxj+duj/dxi)
//
//    cout << "Phi values: "<<endl;
//    cout << this->phi.outstr()<<endl;
//    //h=cp*T;
//    TEqn=( FvImp::Div(phi, h)-FvImp::Laplacian(k,h) );
//    TEqn==0.;
//    Solve(TEqn);
//    h=TEqn.Field();
//    cout << "Enthalpy value: "<<endl;
//    for (int e=0;e<TEqn.Num_Eqn();e++)cout <<TEqn.Eqn(e).Source().outstr()<<endl;
//    cout << TEqn.outstr()<<endl;
}
