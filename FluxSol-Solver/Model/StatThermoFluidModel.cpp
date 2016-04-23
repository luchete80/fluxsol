
#include "StatThermoFluidModel.h"

using namespace std;
using namespace FluxSol;


void StatThermoFluidModel::SolveIter()
{
    CFDModel::SolveIter();
    StatThermalModel::SolveIter();
}
