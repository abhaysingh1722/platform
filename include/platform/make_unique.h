/* -*- Mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */

//
// Backport of C++14's std::make_unique() to C++11
//
// Taken from llvm.org's libc++ implementation (dual licensed under
// the MIT license and the UIUC License (a BSD-like license):
// http://llvm.org/viewvc/llvm-project/libcxx/trunk/include/memory?r1=185352&r2=185351&pathrev=185352
//

#pragma once

#include <memory>

#if __cplusplus < 201402L

// Helper type traits used by make_unique below.
template<class _Tp>
struct __unique_if
{
    typedef std::unique_ptr<_Tp> __unique_single;
};

template<class _Tp>
struct __unique_if<_Tp[]>
{
    typedef std::unique_ptr<_Tp[]> __unique_array_unknown_bound;
};

template<class _Tp, size_t _Np>
struct __unique_if<_Tp[_Np]>
{
    typedef void __unique_array_known_bound;
};


/* TEMORARY global namespace version of make_unique(), this was incorrectly
 * added and there's existing code which uses it. Should be migrated to the
 * version in std - std::make_unique().
 * Once all callers of this are removed, we can delete it.
 */
template<class _Tp, class... _Args>
inline
typename __unique_if<_Tp>::__unique_single
make_unique(_Args&&... __args)
{
    return std::unique_ptr<_Tp>(new _Tp(std::forward<_Args>(__args)...));
}

template<class _Tp>
inline
typename __unique_if<_Tp>::__unique_array_unknown_bound
make_unique(size_t __n)
{
    typedef typename std::remove_extent<_Tp>::type _Up;
    return std::unique_ptr<_Tp>(new _Up[__n]());
}
// END TEMPORARY

namespace std {

template<class _Tp, class... _Args>
inline
typename __unique_if<_Tp>::__unique_single
make_unique(_Args&&... __args)
{
    return std::unique_ptr<_Tp>(new _Tp(std::forward<_Args>(__args)...));
}

template<class _Tp>
inline
typename __unique_if<_Tp>::__unique_array_unknown_bound
make_unique(size_t __n)
{
    typedef typename std::remove_extent<_Tp>::type _Up;
    return std::unique_ptr<_Tp>(new _Up[__n]());
}

}  // namespace std

#else
#warning make_unique.h not intended for C++14 upwards.
#endif  // __cplusplus < 201402L
