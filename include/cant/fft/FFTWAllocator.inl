//
// Created by binabik on 10/09/2020.
//

#ifndef CANTINA_TILDE_FFTWALLOCATOR_INL
#define CANTINA_TILDE_FFTWALLOCATOR_INL

#pragma once

#include <cant/common/CantinaException.hpp>

#include <cant/common/macro.hpp>
namespace cant
{
    template <typename T>
    typename fftw3f_allocator<T>::pointer
    fftw3f_allocator<T>::
    allocate(fftw3f_allocator::size_type n, const void *)
    {
        pointer p  = fftwf_alloc_real(n);
        if (!p)
        {
            throw CANTINA_EXCEPTION("Could not allocate memory.");
        }
        return p;
    }

    template <typename T>
    void
    fftw3f_allocator<T>::
    deallocate(pointer p, size_type)
    {
        fftwf_free(p);
    }

    template <typename T, typename U>
    CANT_CONSTEXPR
    bool
    operator==(const fftw3f_allocator<T>& a, const fftw3f_allocator<U>& other) CANT_NOEXCEPT
    {
        return std::is_same_v<T, U>;
    }

    template <typename T, typename U>
    CANT_CONSTEXPR
    bool
    operator!=(const fftw3f_allocator<T>& a, const fftw3f_allocator<U>& other) CANT_NOEXCEPT
    {
        return !(a == other);
    }

    template <typename T>
    template <typename U>
    fftw3f_allocator<T>::
    fftw3f_allocator(const fftw3f_allocator<U>& a) CANT_NOEXCEPT
    {
    }
}
#include <cant/common/undef_macro.hpp>

#endif //CANTINA_TILDE_FFTWALLOCATOR_INL
