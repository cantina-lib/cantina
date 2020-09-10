//
// Created by binabik on 10/09/2020.
//

#ifndef CANTINA_TILDE_FFTWALLOCATOR_HPP
#define CANTINA_TILDE_FFTWALLOCATOR_HPP

#pragma once

extern "C"
{
#include <fftw3.h>
}

#include <cant/common/memory.hpp>

#include <cant/common/macro.hpp>
namespace cant
{
    template <typename T>
    class fftw3f_allocator
    {
    public:
        /** -- internal structs -- **/
        // required by std::allocator_traits
        typedef T value_type;
        typedef size_u size_type;
        typedef T* pointer;
        typedef const T* const_pointer;

        /** -- methods -- **/
        pointer allocate(size_type n, const void* hint=nullptr);
        void deallocate(pointer p, CANT_MAYBEUNUSED size_type n);

        fftw3f_allocator() CANT_NOEXCEPT = default;
        template <class U>
        CANT_EXPLICIT
        fftw3f_allocator(const fftw3f_allocator<U>& a) CANT_NOEXCEPT;
    };

    template <typename T, typename U>
    CANT_CONSTEXPR bool operator==(const fftw3f_allocator<T>&, const fftw3f_allocator<U>&) CANT_NOEXCEPT;

    template <typename T, typename U>
    CANT_CONSTEXPR bool operator!=(const fftw3f_allocator<T>&, const fftw3f_allocator<U>&) CANT_NOEXCEPT;

    using FFTW3FBlock = std::vector<sample_f, fftw3f_allocator<sample_f>>;
}
#include <cant/common/undef_macro.hpp>

#include "FFTWAllocator.inl"

#endif //CANTINA_TILDE_FFTWALLOCATOR_HPP
