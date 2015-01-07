

//#include "error.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class T>
inline FluxSol::tmp<T>::tmp(T* tPtr)
:
    isTmp_(true),
    ptr_(tPtr),
    ref_(*tPtr)
{}


template<class T>
inline FluxSol::tmp<T>::tmp(const T& tRef)
:
    isTmp_(false),
    ptr_(0),
    ref_(tRef)
{}


template<class T>
inline FluxSol::tmp<T>::tmp(const tmp<T>& t)
:
    isTmp_(t.isTmp_),
    ptr_(t.ptr_),
    ref_(t.ref_)
{
    if (isTmp_)
    {
        if (ptr_)
        {
            ptr_->operator++();
        }
        else
        {
//            FatalErrorIn("FluxSol::tmp<T>::tmp(const tmp<T>&)")
//                << "attempted copy of a deallocated temporary"
//                << abort(FatalError);
        }
    }
}


template<class T>
inline FluxSol::tmp<T>::~tmp()
{
//    if (isTmp_ && ptr_)
//    {
//        if (ptr_->okToDelete())
//        {
//            delete ptr_;
//            ptr_ = 0;
//        }
//        else
//        {
//            ptr_->operator--();
//        }
//    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T>
inline bool FluxSol::tmp<T>::isTmp() const
{
    return isTmp_;
}


template<class T>
inline bool FluxSol::tmp<T>::empty() const
{
    return (isTmp_ && !ptr_);
}


template<class T>
inline bool FluxSol::tmp<T>::valid() const
{
    return (!isTmp_ || (isTmp_ && ptr_));
}


template<class T>
inline T* FluxSol::tmp<T>::ptr() const
{
    if (isTmp_)
    {
         if (!ptr_)
         {
//             FatalErrorIn("FluxSol::tmp<T>::ptr() const")
//                 << "temporary deallocated"
//                 << abort(FatalError);
         }

         T* ptr = ptr_;
         ptr_ = 0;

         ptr->resetRefCount();

         return ptr;
    }
    else
    {
        return new T(ref_);
    }
}


template<class T>
inline void FluxSol::tmp<T>::clear() const
{
    if (isTmp_ && ptr_)  // skip this bit:  && ptr_->okToDelete())
    {
        delete ptr_;
        ptr_ = 0;
    }
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T>
inline T& FluxSol::tmp<T>::operator()()
{
    if (isTmp_)
    {
        if (!ptr_)
        {
//            FatalErrorIn("T& FluxSol::tmp<T>::operator()()")
//                << "temporary deallocated"
//                << abort(FatalError);
        }

        return *ptr_;
    }
    else
    {
        // Note: const is cast away!
        // Perhaps there should be two refs, one for const and one for non const
        // and if the ref is actually const then you cannot return it here.
        //
        // Another possibility would be to store a const ref and a flag to say
        // whether the tmp was constructed with a const or a non-const argument.
        //
        // eg, enum refType { POINTER = 0, REF = 1, CONSTREF = 2 };
        return const_cast<T&>(ref_);
    }
}


template<class T>
inline const T& FluxSol::tmp<T>::operator()() const
{
    if (isTmp_)
    {
        if (!ptr_)
        {
//            FatalErrorIn("const T& FluxSol::tmp<T>::operator()() const")
//                << "temporary deallocated"
//                << abort(FatalError);
        }

        return *ptr_;
    }
    else
    {
        return ref_;
    }
}


template<class T>
inline FluxSol::tmp<T>::operator const T&() const
{
    return operator()();
}


template<class T>
inline T* FluxSol::tmp<T>::operator->()
{
    if (isTmp_)
    {
         if (!ptr_)
         {
//             FatalErrorIn("FluxSol::tmp<T>::operator->()")
//                 << "temporary deallocated"
//                 << abort(FatalError);
         }

         return ptr_;
    }
    else
    {
        return &const_cast<T&>(ref_);
    }
}


template<class T>
inline const T* FluxSol::tmp<T>::operator->() const
{
    return const_cast<tmp<T>&>(*this).operator->();
}


template<class T>
inline void FluxSol::tmp<T>::operator=(const tmp<T>& t)
{
    if (isTmp_ && ptr_)
    {
        if (ptr_->okToDelete())
        {
            delete ptr_;
            ptr_ = 0;
        }
        else
        {
            ptr_->operator--();
        }
    }

    if (t.isTmp_)
    {
        isTmp_ = true;
        ptr_ = t.ptr_;

        if (ptr_)
        {
            ptr_->operator++();
        }
        else
        {
//            FatalErrorIn("FluxSol::tmp<T>::operator=(const tmp<T>&)")
//                << "attempted copy of a deallocated temporary"
//                << abort(FatalError);
        }
    }
    else
    {
//        FatalErrorIn("FluxSol::tmp<T>::operator=(const tmp<T>&)")
//            << "attempted to assign to a const reference to constant object"
//            << abort(FatalError);
    }
}



// ************************************************************************* //
