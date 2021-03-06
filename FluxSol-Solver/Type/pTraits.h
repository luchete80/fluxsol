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
#ifndef pTraits_H
#define pTraits_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//Esto se extrajo de OpenFoam
//Sirve para
namespace FluxSol
{

class Istream;

/*---------------------------------------------------------------------------*\
                           Class pTraits Declaration
\*---------------------------------------------------------------------------*/

template<class PrimitiveType>
class pTraits
:
    public PrimitiveType
{

public:

    // Constructors

        //- Construct from primitive
        pTraits(const PrimitiveType& p)
        :
            PrimitiveType(p)
        {}

        //- Construct from Istream
        pTraits(Istream& is)
        :
            PrimitiveType(is)
        {}
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
