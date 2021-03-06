#ifndef _TMP_H
#define _TMP_H

namespace FluxSol
{

/*---------------------------------------------------------------------------*\
                             Class tmp Declaration
\*---------------------------------------------------------------------------*/

template<class T>
class tmp
{
    // Private data

        //- Flag for whether object is a temporary or a constant object
        bool isTmp_;

        //- Pointer to temporary object
        mutable T* ptr_;

        //- Const reference to constant object
        const T& ref_;


public:

    // Constructors

        //- Store object pointer
        inline explicit tmp(T* = 0);

        //- Store object const reference
        inline tmp(const T&);

        //- Construct copy and increment reference count
        inline tmp(const tmp<T>&);


    //- Destructor, delete object when reference count == 0
    inline ~tmp();


    // Member Functions

        // Access

            //- Return true if this is really a temporary object
            inline bool isTmp() const;

            //- Return true if this temporary object empty,
            //  ie, a temporary without allocation
            inline bool empty() const;

            //- Is this temporary object valid,
            //  ie, it is a reference or a temporary that has been allocated
            inline bool valid() const;

        // Edit

            //- Return tmp pointer for reuse
            inline T* ptr() const;

            //- If object pointer points to valid object:
            //  delete object and set pointer to NULL
            inline void clear() const;


    // Member operators

        //- Dereference operator
        inline T& operator()();

        //- Const dereference operator
        inline const T& operator()() const;

        //- Const cast to the underlying type reference
        inline operator const T&() const;

        //- Return object pointer
        inline T* operator->();

        //- Return const object pointer
        inline const T* operator->() const;

        //- Assignment operator
        inline void operator=(const tmp<T>&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "TmpI.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
