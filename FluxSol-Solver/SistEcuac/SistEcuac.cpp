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
#include "SistEcuac.h"

#include <time.h>

//In templates definitions must be included namespace instead of..
namespace FluxSol
{

template <typename T>
int Eqn<T>::Width()
{
	int min=1e10;
	int max=0;
	for (int i=0;i<this->neighbour_id.size();i++)
	{
		if (neighbour_id[i]<min)
			min=neighbour_id[i];
		if (neighbour_id[i]>max)
			max=neighbour_id[i];
	}
	if (this->id<min)	min=id;
	if (this->id>max)	max=id;
	return (max-min+1);
}

//Min Id of neighbour coefficient
template <typename T>
int Eqn<T>::MinNeigbourId()
{
	int min=1e10;
	for (int i=0;i<this->neighbour_id.size();i++)
	{
		if (neighbour_id[i]<min)
			min=neighbour_id[i];
	}
	if (this->id<min)	min=id;
	return min;
}

template <typename T>
void EqnSystem<T>::Insert (const FluxSol::Eqn <T> &ec)
{
	eqn.push_back(ec);
}



//Both ap must refer to the same id
//roght coeffs pass negative and left term pass as positive to rhs
//If neighbours refer to differents id, neighbour ids will be added
//THIS FUNCTION ASSUMES THAT COEFFS ON BOTH EQNS ARE NOT REPEATED
//TO MODIFY!! CHANGE TO Reference Return, replacing ret for this->
//template <typename T>
//Eqn<T> & Eqn<T>::operator==(const Eqn<T> &right)
//{
//	Eqn<T> ret=*this;    //To equal neighbour id
//
//	vector <int> tempright;
//	//vector <T> temprval;
//    vector <Scalar> temprval;
//
//	tempright = right.neighbour_id;
//    temprval = right.an;
//	vector<int> ids;	//checked neighbours ids
//
//
//
////    for (int nleft = 0; nleft < this->an.size(); nleft++)
////    {
////        bool found =false;
////        for (int nr=0;nr<ret.neighbour_id.size();nr++)
////            if(an[nleft]==ret.neighbour_id[nr])
////                found==true;
////
////    }
//
//	//Che if both central coeff are the same
//	if (id == right.id)
//	{
//	    //ret=Eqn<T>(pid,nids);
//		//Must chek if both neighbour ids are the same and in that case
//		ret.ap = this->ap - right.ap;
//
//		bool lfound;
////        cout <<"left an size" <<this->an.size()<<endl;
////        cout <<"right an size" <<right.an.size()<<endl;
//		//left eqn could have a bigger stencil
//		//At first assign left an size to ret eqn
//		ret.an.assign(this->an.size(),0.);
//		for (int nleft = 0; nleft < this->an.size(); nleft++)
//		{
//			lfound = false;
//			int id;
//			int nright = 0;
//			while (!lfound && nright < tempright.size())
//			{
//
//				if (this->neighbour_id[nleft] == tempright[nright])
//				{
//					lfound = true;
//					ids.push_back(nleft);
//				}
//				nright++;
//			}
//			nright--;
//
//			//
////			cout << "ret an size"<<ret.an.size()<<endl;
//			if (!lfound)
//				ret.an[nleft] = this->an[nleft];
//			else
//			{
//				ret.an[nleft] = this->an[nleft] - temprval[nright];
//				tempright.erase(tempright.begin() + nright);
//				temprval.erase(temprval.begin() + nright);
//			}
//
//		}//for nleft
//
//
//		//Comparing right vectors
//		bool rfound;
//		for (int nright = 0; nright < tempright.size(); nright++)
//		{
//			//if ids[]
//			rfound = false;
//			int nleft = 0;
//			while (!rfound && nleft < this->an.size())
//			{
//				if (this->neighbour_id[nleft] == tempright[nright])
//				{
//					rfound = true;
//					ids.push_back(nleft);
//				}
//				nleft++;
//			}
//			nleft--;
//
//			if (!rfound)
//            {
//				ret.an.push_back(-(temprval[nright]));
//                ret.neighbour_id.push_back(nright);
//            }
//			else
//				ret.an.push_back( this->an[nleft] - temprval[nright]);
//		}
//
//		//ret.An(this->Ap() - right.Ap());
//		//for (int nid=0;nid<ids.size();nid++)
//                //this->neighbour_id.push_back(ids[nid]);
//
//		ret.source = this->source - right.source;
//
//        //cout << "Left Source"<<endl;
//		//cout << this->source.outstr()<<endl;
//
//        //cout << "Right Source"<<endl;
//		//cout << right.source.outstr()<<endl;
//
//        //cout << "Return Source"<<endl;
//		//cout << ret.source.outstr()<<endl;
//
//
//		*this=ret;
//		//return ret;
//	}
//
//    //return *this;
//    return ret;
//
//}

//FAST EQN SYSTEM OPERATOR==, THIS SUPPOSE THAT ID's ARE THE SAME
//
template <typename T>
EqnSystem <T> & EqnSystem<T>::operator=(const EqnSystem <T> &right)
	{
        clock_t ittime_begin, ittime_end;
        double ittime_spent;
        ittime_end = clock();

	    this->eqn.clear();
	    this->GridPtr=right.GridPtr;
	    this->eqn.resize(right.EqnV().size());

	    this->nbr_eqn.resize(right.nbr_eqn.size());
        for (int e=0;e<right.EqnV().size();e++)
        {
            //this->eqn.push_back(right.Eqn(e));
            this->eqn[e]=right.Eqn(e);
        }

//        for (int f=0;f<right.first_nonzero_column.size();f++)
//            this->first_nonzero_column.push_back(right.first_nonzero_column[f]);
//
//        for (int f=0;f<right.nbr_eqn.size();f++)
//            this->nbr_eqn.push_back(right.nbr_eqn[f]);

        this->dimension=right.dimension;

        ittime_spent = (double)(clock() - ittime_end) / CLOCKS_PER_SEC;
        ittime_end = clock();
        cout << "eqnsys operator ="<<ittime_spent <<endl;

        return *this;
	}

template <typename T>
Eqn<T> & Eqn<T>::operator==(const Eqn<T> &right)
{


    //ret=Eqn<T>(pid,nids);
    //Must chek if both neighbour ids are the same and in that case
    this->ap -= right.ap;

//        cout <<"left an size" <<this->an.size()<<endl;
//        cout <<"right an size" <<right.an.size()<<endl;
    //left eqn could have a bigger stencil
    //At first assign left an size to ret eqn

    for (int n=0;n<this->an.size();n++)
    {
        this->an[n]-=right.an[n];
    }

    this->source -= right.source;

    return *this;

}

template <typename T>
Eqn<T> Eqn<T>::operator-()
{
    Eqn<T> eq;
    Scalar _ap;
    _ap=-this->ap;
    vector <Scalar> _an;
    _an.assign(this->num_neighbours,0.);
    for (int n=0;n<this->num_neighbours;n++)
        //_an.push_back(-this->an[n]);
        _an[n]=-this->an[n];
    T _source;
    _source=-this->source;
    //cout << ""
    //return Eqn<T>(_ap,_an,_source);
    return Eqn<T>(_ap,_an,_source);
}

// TO MODIFY: THESE + and - operator do not construct new object
//Assuming same meshes
template <typename T>
Eqn<T> &Eqn<T>::operator-(const Eqn<T> &right)
{

    *this==right;
    return *this;
}

//Supposing that stencils are the same
template <typename T>
Eqn<T> & Eqn<T>::operator+(const Eqn<T> &right)
{

//    //T _ap=-this->ap;
//    vector <T> _an;
//    //BUG, TO MODIFY, CHECK NEIGHBOURS
//    for (int n=0;n<this->num_neighbours;n++)
//        _an.push_back(this->an[n]);
//
//    T _source=this->source+right.source;
//    return Eqn<T>(_ap,_an,_source);

    *this==-right;
    return *this;
}


//template <typename T>
//Eqn<T> & Eqn<T>::operator-()
//{
//	Eqn<T> ret=*this;    //To equal neighbour id
//}

//template <typename T>
//void EqnSystem<T>::Insert (const Eqn<T> &ec, std::vector<int> & nbreq)
//{
//	Insert(ec);
//	nbr_eqn=nbreq;
//}


// OPERATORS
template <typename T>
EqnSystem <T> & EqnSystem <T>::operator==(const double &d)
{
	Scalar source(d);

    clock_t ittime_begin, ittime_end;
    double ittime_spent;


    ittime_end = clock();


	//This value is added to each source term
	for (int e=0;e<eqn.size();e++)
	{
		Eqn(e)+=d;	//Overloaded Eqn operator, add to source term
	}


    ittime_spent = (double)(clock() - ittime_end) / CLOCKS_PER_SEC;
    ittime_end = clock();
    cout << "eqnsys operator == "<<ittime_spent <<endl;

	return *this;
}


template <typename T>
void EqnSystem<T>::Log(std::string str)
{
	ofstream file(str.c_str());
	file<<"Equation System Info"<<endl;
	int e=0;
	for (int e=0; e<eqn.size();e++)
	{
		file<<"Eqn "<<e<<endl;
		eqn[e].Log(file);
	}
}

template <typename T>
EqnSystem <T> EqnSystem <T>::operator+ (const EqnSystem <T> &right)
{

clock_t ittime_begin, ittime_end;
double ittime_spent;


ittime_end = clock();

//cout << "Eqn sizes"<< this->Num_Eqn()<< " "<<right.EqnV().size()<<endl;
int num_eqn=this->eqn.size();
    //EqnSystem<T> ret(num_eqn);
    EqnSystem<T> ret=*this;
    if (EqnV().size() == right.EqnV().size())
    {
        //Check if both eqns have same size
        for (int e = 0; e<EqnV().size(); e++)
        {
            ret.eqn[e] = (this->eqn[e] + right.Eqn(e));

        }
    }
    else
    {
        cout << "Eqn Systems have different size. Operator == fails"<<endl;
    }

    ittime_spent = (double)(clock() - ittime_end) / CLOCKS_PER_SEC;
    ittime_end = clock();
    cout << "eqnsys operator -  "<<ittime_spent <<endl;


    return ret;

}
//
//template <typename T>
//EqnSystem<T>::EqnSystem(const Fv_CC_Grid &FvG)//:grid(Fv_CC_Grid(FvG))
//{
//    this->GridPtr=&FvG;
//	dimension=int(pTraits<T>::rank);
//
//	for (int c=0;c<FvG.Num_Cells();c++)
//    {
//		vector <T> an;
//		vector <Scalar> an;
//		Scalar init_an;
//        cout << "Cell "<<endl;
//		//Insert empty Values
//		T source;
//		Scalar ap;
//		vector <int> nbr_id;
//		cout << "Neighbours number" <<endl;
//		_Cell cc(FvGrid.Cell(c));
//		cout << "Assigned"<<endl;
//		cout << FvGrid.Cell(c).Num_Neighbours()<<endl;
//		an.assign(FvG.Cell(c).Num_Neighbours(),init_an),
//		nbr_id.assign(FvG.Cell(c).Num_Neighbours(),0);
//
//        cout << "Neighbours"<<endl;
//		for (int n=0;n<FvG.Cell(c).Num_Neighbours();n++)
//			nbr_id[n]=FvG.Cell(c).Neighbour(n);
//
//		FluxSol::Eqn <T> eq(c,ap,an,source,nbr_id);
//		eqn.push_back(eq);
//
//	}//End for cells
//
//}

template <typename T>
EqnSystem<T>::EqnSystem(const Fv_CC_Grid &FvG)
{
    cout << "Creating EqnSys"<<endl;
    this->GridPtr=&FvG;
	dimension=int(pTraits<T>::rank);
    eqn=std::vector < FluxSol::Eqn <T> >(FvG.Num_Cells());

	for (int c=0;c<FvG.Num_Cells();c++)
    {
		//vector <T> an;
		vector <Scalar> an;
		Scalar init_an;
        //cout << "Cell "<<endl;
		//Insert empty Values
		T source;
		Scalar ap;
		vector <int> nbr_id;
		//cout << "Neighbours number" <<endl;
		//_Cell cc(FvGrid.Cell(c));
		//cout << "Assigned"<<endl;
		//cout << FvG.Cell(c).Num_Neighbours()<<endl;
		eqn[c].id=c;
		eqn[c].ap=ap;eqn[c].source=source;eqn[c].x=0.;
		eqn[c].an=vector<Scalar>(FvG.Cell(c).Num_Neighbours());
		eqn[c].neighbour_id=vector<int>(FvG.Cell(c).Num_Neighbours());
		eqn[c].num_neighbours=FvG.Cell(c).Num_Neighbours();

//		vector<int> nid(FvG.Cell(c).Num_Neighbours());
//		vector<Scalar> van(FvG.Cell(c).Num_Neighbours());


        //cout << "Neighbours"<<endl;
		for (int n=0;n<FvG.Cell(c).Num_Neighbours();n++)
        {
            eqn[c].an[n]=0.;eqn[c].neighbour_id[n]=FvG.Cell(c).Neighbour(n);
            //nid[n]=FvG.Cell(c).Neighbour(n);
        }
		//FluxSol::Eqn<T>eq(c,ap,van,source,nid);
		//eqn[c]=eq;

	}//End for cells

}

//Binary members
//Arguments Can not be defined as constants
//Passing right coeffs to left and left source to sight
template <typename T>
EqnSystem <T> & EqnSystem <T>::operator==(const EqnSystem <T> &right)
{
//    cout << "Eqn sizes"<< EqnV().size() << " "<<right.EqnV().size()<<endl;
	if (EqnV().size() == right.EqnV().size())
	{
		//Check if both eqns have same size
		for (int e = 0; e<EqnV().size(); e++)
		{
//			cout << "operator =="<< this-> eqn[e].NeighboursIds().size()<<endl;
			this->eqn[e] = (this->eqn[e] == right.Eqn(e));
//			cout << "operator =="<< this-> eqn[e].NeighboursIds().size()<<endl;

//
//            this->eqn[e].ap -= right.eqn[e].ap;
//
//            for (int n=0;n<this->eqn[e].an.size();n++)
//            {
//                this->eqn[e].an[n]-=right.eqn[e].an[n];
//            }
//
//            this->eqn[e].source -= right.eqn[e].source;

		}
	}
	else
    {
        cout << "Eqn Systems have different size. Operator == fails"<<endl;
    }

	return *this;
}



template <typename T>
EqnSystem <T> & EqnSystem <T>::operator==(const _CC_Fv_Field<T> &field)
{
    for (int c=0;c<this->EqnV().size();c++)
    {
        T val=field.Val(c).Val();
        //this->Eqn(c).Source(val);
        this->eqn[c]+=val;
    }
	return *this;
}
//
//template <typename T>
//EqnSystem<T>::EqnSystem(_CC_Fv_Field<T> &fvfield):field(fvfield)
//{
//	Fv_CC_Grid &FvGrid=this->field.Grid();
//
//	dimension=int(pTraits<T>::rank);
//
//	for (int c=0;c<FvGrid.Num_Cells();c++)
//    {
//		vector <T> an;
//		T init_an;
//
//		//Insert empty Values
//		T ap,source;
//		vector <int> nbr_id;
//		an.assign(FvGrid.Cell(c).Num_Neighbours(),init_an),
//		nbr_id.assign(FvGrid.Cell(c).Num_Neighbours(),0);
//
//		for (int n=0;n<FvGrid.Cell(c).Num_Neighbours();n++)
//			nbr_id[n]=FvGrid.Cell(c).Neighbour(n);
//
//		FluxSol::Eqn <T> eq(c,ap,an,source,nbr_id);
//		eqn.push_back(eq);
//
//	}//End for cells
//
//}

template <typename T>
const vector<double> EqnSystem<T>::GlobalRes() const
{

    int dim = (int)pow(3.0,pTraits<T>::rank);
     vector <double> ret(dim);
    vector <double> temp(dim), num(dim), den(dim);

    T tempv, numv, denv,apx;
    T retv;

//    for (int e=0;e<this->EqnV().size();e++)
//    {
//        for (int d=0;d<dim;d++)
//            temp[d]= (this->Eqn(e).Ap().Comp()[0]*this->Eqn(e).X().Comp()[d]) - this->Eqn(e).Source().Comp()[d];
//
//        for (int i=0;i<this->Eqn(e).NeighboursIds().size();i++)
//        {
//            int nid=this->GridPtr->Cell(e).Neighbour(i);
//            //Converting to double
//            for (int d=0;d<dim;d++)
//            {
//                temp[d]+= ( this->Eqn(e).An(i).Comp()[0]*this->Eqn(nid).X().Comp()[d] );
//            }
//        }
//
//        for (int d=0;d<dim;d++)
//        {
//            num[d]+=fabs(temp[d]);
//            den[d]+=fabs(this->Eqn(e).Ap().Comp()[0]*Eqn(e).X().Comp()[d]);
//        }
//        //cout << "Cell residual: "<<temp[0]<< " "<<temp[1]<<" "<<temp[2]<<endl;
//
//    }
//
//        for (int d=0;d<dim;d++) cout << "Den: "<<den[d]<<endl;
//        //for (int d=0;d<dim;d++) ret[d]=num[d]/den[d];


    for (int e=0;e<this->EqnV().size();e++)
    {
            tempv=0.;
            apx=( this->Eqn(e).Ap()*this->Eqn(e).X() );
            tempv= apx  - this->Eqn(e).Source();

            //temp[d]= (this->Eqn(e).Ap().Comp()[0]*this->Eqn(e).X().Comp()[d]) - this->Eqn(e).Source().Comp()[d];
        denv=Sum_Mag(denv,apx);

        for (int i=0;i<this->Eqn(e).NeighboursIds().size();i++)
        {
            int nid=this->GridPtr->Cell(e).Neighbour(i);
            //Converting to double
            //for (int d=0;d<dim;d++)
            //{
                //temp[d]+= ( this->Eqn(e).An(i).Comp()[0]*this->Eqn(nid).X().Comp()[d] );
            //}
            tempv+=( this->Eqn(e).An(i)*this->Eqn(nid).X() );
        }

//        for (int d=0;d<dim;d++)
//        {
//            //num[d]+=fabs(temp[d]);
//            //nums+=temps.Norm();
//            //den[d]+=fabs(this->Eqn(e).Ap().Comp()[0]*Eqn(e).X().Comp()[d]);
//        }
        //cout <<"residuals cell "<< e << ": "<< numv.outstr()<<endl;
        numv=Sum_Mag(numv,tempv);
        //cout << "Cell residual: "<<temp[0]<< " "<<temp[1]<<" "<<temp[2]<<endl;

    }
    retv=numv;
    //retv=numv/denv;
        for (int d=0;d<dim;d++)
            ret[d]=retv.Comp()[d];

    return ret;
}

}

#include "SistEcuac.inst"

