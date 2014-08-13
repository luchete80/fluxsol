#ifndef _FACEGRAD_H_
#define _FACEGRAD_H_

#include "./SistEcuac/SistEcuac.h"
#include "./Field/Field.h"

namespace FluxSol{


template <typename T>
EqnSystem<T> FaceGrad (_CC_Fv_Field <T> )
{

}

//Opciones, el operador llama a un campo y a una cara, otorgando el valor de gradiente que habra que cambiar el signo
//El operador tiene como parametro si es p o n (es muy desprolijo)
//El operador llama a un cell y calcula la ecuacion para todas las caras
//La opcion mas prolija es la 1
// LA FUNCION DEVUELVE GRADIENTE * Area de la cara
//PONER INLINE!
//EN EL CASO DEL LAPLACIANO AUMENTA Y DISMINUYE UN ORDEN
template <typename T>
Eqn<T> FaceGrad (_CC_Fv_Field <T> field, const int &idface)
{

    _FvFace f=field.Grid().Face(idface);
    //En principio es coordenado
    T ap;
	vector <T> an;
    //Ojo falta el operador UNARIO!
	an=-f.Norm_ad()/f.Dist_pn();
	ap=-an;

    Eqn<T> eqn(ap,an);
    return eqn;
}

};	//Fin namespace FluxSol

#endif
