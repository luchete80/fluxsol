#ifndef _INTERPOLATION_H_
#define _INTERPOLATION_H_


#include "../Type/Scalar.h"
#include "../Field/Field.h"
#include "../FiniteVolume/Mesh/FvGrid.h"
#include "../Type/Vec3d.h"

namespace FluxSol
{

template <typename T>
class Interpolation
{

	protected:
		//Reference to the volfield
		const _CC_Fv_Field<T> &field; 
		const Fv_CC_Grid &grid;

		_CC_Fv_Field<T> nullfield;
		const Fv_CC_Grid nullgrid;
	
	public:
		//Must be initialized in initialization list
		Interpolation():field(nullfield),grid(nullgrid){};
		Interpolation(const _CC_Fv_Field<T> &f) :field(f),grid(field.ConstGrid()){};
		
		//Must be called by a const
		const _CC_Fv_Field<T> & Field() const { return field; };

};


}; //FluxSol

#endif
