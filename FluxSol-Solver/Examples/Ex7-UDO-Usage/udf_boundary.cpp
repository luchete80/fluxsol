#include "udf_boundary.h"

using namespace FluxSol;

void ufield::Calculate()
{

    cout << "UDF Calculating"<<endl;
	Scalar x;
	double xveloc;

	for (int f=0;f<this->_Patch().Num_Faces();f++)
	{
		x=this->_Patch().Face(f).Center()[0]-0.5;
		xveloc=(0.25-(x.Val()*x.Val()))*100.0;
		this->value[f]=Vec3D(xveloc,0.,0.);
	}
}

