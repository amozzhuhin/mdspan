/*
//@HEADER
// ************************************************************************
//
//                        Kokkos v. 2.0
//              Copyright (2019) Sandia Corporation
//
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact Christian R. Trott (crtrott@sandia.gov)
//
// ************************************************************************
//@HEADER
*/


#pragma once

#include "config.hpp"

#include <type_traits> // std::is_void

#ifndef MDSPAN_FORCE_INLINE_FUNCTION
#  define MDSPAN_FORCE_INLINE_FUNCTION __attribute__((always_inline))
#endif
#ifndef MDSPAN_INLINE_FUNCTION
#  define MDSPAN_INLINE_FUNCTION inline
#endif
#ifndef MDSPAN_INLINE_FUNCTION_DEFAULTED
#  define MDSPAN_INLINE_FUNCTION_DEFAULTED inline
#endif

//==============================================================================
// <editor-fold desc="Preprocessor helpers"> {{{1

#define MDSPAN_PP_COUNT(...)                                                    \
    MDSPAN_PP_COUNT_(__VA_ARGS__,                                               \
        50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,            \
        30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,            \
        10,9,8,7,6,5,4,3,2,1,)                                                  \
        /**/
        
#define MDSPAN_PP_COUNT_(                                                       \
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10,                                    \
    _11, _12, _13, _14, _15, _16, _17, _18, _19, _20,                           \
    _21, _22, _23, _24, _25, _26, _27, _28, _29, _30,                           \
    _31, _32, _33, _34, _35, _36, _37, _38, _39, _40,                           \
    _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, N, ...)  N                \
    /**/
    
#define MDSPAN_PP_STRINGIFY_IMPL(x) #x
#define MDSPAN_PP_STRINGIFY(x) MDSPAN_PP_STRINGIFY_IMPL(x)

#define MDSPAN_PP_CAT_IMPL(x, y) x ## y
#define MDSPAN_PP_CAT(x, y) MDSPAN_PP_CAT_IMPL(x, y)

#define MDSPAN_PP_EVAL(X, ...) X(__VA_ARGS__)

#define MDSPAN_PP_REMOVE_PARENS_IMPL(...) __VA_ARGS__
#define MDSPAN_PP_REMOVE_PARENS(...) MDSPAN_PP_REMOVE_PARENS_IMPL __VA_ARGS__

// </editor-fold> end Preprocessor helpers }}}1
//==============================================================================

//==============================================================================
// <editor-fold desc="Concept emulation"> {{{1

// These compatibility macros don't help with partial ordering, but they should do the trick
// for what we need to do with concepts in mdspan
#ifdef _MDSPAN_USE_CONCEPTS
#  define MDSPAN_CLOSE_ANGLE_REQUIRES(REQ) > requires REQ
#  define MDSPAN_FUNCTION_REQUIRES(PAREN_PREQUALS, FNAME, PAREN_PARAMS, QUALS, REQ) \
     MDSPAN_PP_REMOVE_PARENS(PAREN_PREQUALS) FNAME PAREN_PARAMS QUALS requires REQ \
     /**/
#else
#  define MDSPAN_CLOSE_ANGLE_REQUIRES(REQ) , int=std::enable_if_t<(REQ), int>(0)>
#  define MDSPAN_FUNCTION_REQUIRES(PAREN_PREQUALS, FNAME, PAREN_PARAMS, QUALS, REQ) \
     MDSPAN_TEMPLATE_REQUIRES( \
       class __function_requires_ignored=void, \
       (std::is_void<__function_requires_ignored>::value && REQ) \
     ) MDSPAN_PP_REMOVE_PARENS(PAREN_PREQUALS) FNAME PAREN_PARAMS QUALS \
     /**/
#endif

#define MDSPAN_TEMPLATE_REQUIRES(...) \
  MDSPAN_PP_EVAL( \
      MDSPAN_PP_CAT(MDSPAN_TEMPLATE_REQUIRES_, MDSPAN_PP_COUNT(__VA_ARGS__)), \
      __VA_ARGS__ \
  ) \
  /**/

#define MDSPAN_TEMPLATE_REQUIRES_2(TP1, REQ) \
  template<TP1 \
    MDSPAN_CLOSE_ANGLE_REQUIRES(REQ) \
    /**/
#define MDSPAN_TEMPLATE_REQUIRES_3(TP1, TP2, REQ) \
  template<TP1, TP2 \
    MDSPAN_CLOSE_ANGLE_REQUIRES(REQ) \
    /**/
#define MDSPAN_TEMPLATE_REQUIRES_4(TP1, TP2, TP3, REQ) \
  template<TP1, TP2, TP3 \
    MDSPAN_CLOSE_ANGLE_REQUIRES(REQ) \
    /**/
#define MDSPAN_TEMPLATE_REQUIRES_5(TP1, TP2, TP3, TP4, REQ) \
  template<TP1, TP2, TP3, TP4 \
    MDSPAN_CLOSE_ANGLE_REQUIRES(REQ) \
    /**/
#define MDSPAN_TEMPLATE_REQUIRES_6(TP1, TP2, TP3, TP4, TP5, REQ) \
  template<TP1, TP2, TP3, TP4, TP5 \
    MDSPAN_CLOSE_ANGLE_REQUIRES(REQ) \
    /**/

#define MDSPAN_INSTANTIATE_ONLY_IF_USED \
  MDSPAN_TEMPLATE_REQUIRES( \
    class __instantiate_only_if_used_tparam=void, \
    ( _MDSPAN_TRAIT(std::is_void, __instantiate_only_if_used_tparam) ) \
  ) \
  /**/

// </editor-fold> end Concept emulation }}}1
//==============================================================================

//==============================================================================
// <editor-fold desc="inline variables"> {{{1

#if _MDSPAN_USE_INLINE_VARIABLES
#  define _MDSPAN_INLINE_VARIABLE inline
#else
#  define _MDSPAN_INLINE_VARIABLE
#endif

// </editor-fold> end inline variables }}}1
//==============================================================================

//==============================================================================
// <editor-fold desc="Return type deduction"> {{{1

#if _MDSPAN_USE_RETURN_TYPE_DEDUCTION
#  define _MDSPAN_DEDUCE_RETURN_TYPE_SINGLE_LINE(SIGNATURE, BODY) \
    auto MDSPAN_PP_REMOVE_PARENS(SIGNATURE) { return MDSPAN_PP_REMOVE_PARENS(BODY); }
#  define _MDSPAN_DEDUCE_DECLTYPE_AUTO_RETURN_TYPE_SINGLE_LINE(SIGNATURE, BODY) \
    decltype(auto) MDSPAN_PP_REMOVE_PARENS(SIGNATURE) { return MDSPAN_PP_REMOVE_PARENS(BODY); }
#else
#  define _MDSPAN_DEDUCE_RETURN_TYPE_SINGLE_LINE(SIGNATURE, BODY) \
    auto MDSPAN_PP_REMOVE_PARENS(SIGNATURE) \
      -> typename std::remove_cv<typename std::remove_reference<decltype(BODY)>::type>::type \
    { return MDSPAN_PP_REMOVE_PARENS(BODY); }
#  define _MDSPAN_DEDUCE_DECLTYPE_AUTO_RETURN_TYPE_SINGLE_LINE(SIGNATURE, BODY) \
    auto MDSPAN_PP_REMOVE_PARENS(SIGNATURE) \
      -> decltype(BODY) \
    { return MDSPAN_PP_REMOVE_PARENS(BODY); }

#endif

// </editor-fold> end Return type deduction }}}1
//==============================================================================

//==============================================================================
// <editor-fold desc="fold expressions"> {{{1

struct __mdspan_enable_fold_comma { };

#ifdef _MDSPAN_USE_FOLD_EXPRESSIONS
#  define _MDSPAN_FOLD_AND(...) ((__VA_ARGS__) && ...)
#  define _MDSPAN_FOLD_AND_TEMPLATE(...) ((__VA_ARGS__) && ...)
#  define _MDSPAN_FOLD_OR(...) ((__VA_ARGS__) || ...)
#  define _MDSPAN_FOLD_ASSIGN_LEFT(INIT, ...) (INIT = ... = (__VA_ARGS__))
#  define _MDSPAN_FOLD_ASSIGN_RIGHT(PACK, ...) (PACK = ... = (__VA_ARGS__))
#  define _MDSPAN_FOLD_TIMES_RIGHT(PACK, ...) (PACK * ... * (__VA_ARGS__))
#  define _MDSPAN_FOLD_PLUS_RIGHT(PACK, ...) (PACK + ... + (__VA_ARGS__))
#  define _MDSPAN_FOLD_COMMA(...) ((__VA_ARGS__), ...)
#else

namespace std {

namespace __fold_compatibility_impl {

// We could probably be more clever here, but at the (small) risk of losing some compiler understanding.  For the
// few operations we need, it's not worth generalizing over the operation

#if _MDSPAN_USE_RETURN_TYPE_DEDUCTION

MDSPAN_FORCE_INLINE_FUNCTION
constexpr decltype(auto) __fold_right_and_impl() {
  return true;
}

template <class Arg, class... Args>
MDSPAN_FORCE_INLINE_FUNCTION
constexpr decltype(auto) __fold_right_and_impl(Arg&& arg, Args&&... args) {
  return ((Arg&&)arg) && __fold_compatibility_impl::__fold_right_and_impl((Args&&)args...);
}

MDSPAN_FORCE_INLINE_FUNCTION
constexpr decltype(auto) __fold_right_or_impl() {
  return false;
}

template <class Arg, class... Args>
MDSPAN_FORCE_INLINE_FUNCTION
constexpr decltype(auto) __fold_right_or_impl(Arg&& arg, Args&&... args) {
  return ((Arg&&)arg) || __fold_compatibility_impl::__fold_right_or_impl((Args&&)args...);
}

template <class Arg1>
MDSPAN_FORCE_INLINE_FUNCTION
constexpr decltype(auto) __fold_left_assign_impl(Arg1&& arg1) {
  return (Arg1&&)arg1;
}

template <class Arg1, class Arg2, class... Args>
MDSPAN_FORCE_INLINE_FUNCTION
constexpr decltype(auto) __fold_left_assign_impl(Arg1&& arg1, Arg2&& arg2, Args&&... args) {
  return __fold_compatibility_impl::__fold_left_assign_impl((((Arg1&&)arg1) = ((Arg2&&)arg2)), (Args&&)args...);
}

template <class Arg1>
MDSPAN_FORCE_INLINE_FUNCTION
constexpr decltype(auto) __fold_right_assign_impl(Arg1&& arg1) {
  return (Arg1&&)arg1;
}

template <class Arg1, class Arg2, class... Args>
MDSPAN_FORCE_INLINE_FUNCTION
constexpr decltype(auto) __fold_right_assign_impl(Arg1&& arg1, Arg2&& arg2,  Args&&... args) {
  return ((Arg1&&)arg1) = __fold_compatibility_impl::__fold_right_assign_impl((Arg2&&)arg2, (Args&&)args...);
}

template <class Arg1>
MDSPAN_FORCE_INLINE_FUNCTION
constexpr decltype(auto) __fold_right_plus_impl(Arg1&& arg1) {
  return (Arg1&&)arg1;
}

template <class Arg1, class Arg2, class... Args>
MDSPAN_FORCE_INLINE_FUNCTION
constexpr decltype(auto) __fold_right_plus_impl(Arg1&& arg1, Arg2&& arg2, Args&&... args) {
  return ((Arg1&&)arg1) + __fold_compatibility_impl::__fold_right_plus_impl((Arg2&&)arg2, (Args&&)args...);
}

template <class Arg1>
MDSPAN_FORCE_INLINE_FUNCTION
constexpr decltype(auto) __fold_right_times_impl(Arg1&& arg1) {
  return (Arg1&&)arg1;
}

template <class Arg1, class Arg2, class... Args>
MDSPAN_FORCE_INLINE_FUNCTION
constexpr decltype(auto) __fold_right_times_impl(Arg1&& arg1, Arg2&& arg2, Args&&... args) {
  return ((Arg1&&)arg1) * __fold_compatibility_impl::__fold_right_times_impl((Arg2&&)arg2, (Args&&)args...);
}

#else

//------------------------------------------------------------------------------
// <editor-fold desc="right and"> {{{2

template <class... Args>
struct __fold_right_and_impl_;
template <>
struct __fold_right_and_impl_<> {
  using __rv = bool;
  MDSPAN_FORCE_INLINE_FUNCTION
  static constexpr __rv
  __impl() noexcept {
    return true;
  }
};
template <class Arg, class... Args>
struct __fold_right_and_impl_<Arg, Args...> {
  using __next_t = __fold_right_and_impl_<Args...>;
  using __rv = decltype(std::declval<Arg>() && std::declval<typename __next_t::__rv>());
  MDSPAN_FORCE_INLINE_FUNCTION
  static constexpr __rv
  __impl(Arg&& arg, Args&&... args) noexcept {
    return ((Arg&&)arg) && __next_t::__impl((Args&&)args...);
  }
};

template <class... Args>
MDSPAN_FORCE_INLINE_FUNCTION
constexpr typename __fold_right_and_impl_<Args...>::__rv
__fold_right_and_impl(Args&&... args) {
  return __fold_right_and_impl_<Args...>::__impl((Args&&)args...);
}

// </editor-fold> end right and }}}2
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// <editor-fold desc="right or"> {{{2

template <class... Args>
struct __fold_right_or_impl_;
template <>
struct __fold_right_or_impl_<> {
  using __rv = bool;
  MDSPAN_FORCE_INLINE_FUNCTION
  static constexpr __rv
  __impl() noexcept {
    return false;
  }
};
template <class Arg, class... Args>
struct __fold_right_or_impl_<Arg, Args...> {
  using __next_t = __fold_right_or_impl_<Args...>;
  using __rv = decltype(std::declval<Arg>() || std::declval<typename __next_t::__rv>());
  MDSPAN_FORCE_INLINE_FUNCTION
  static constexpr __rv
  __impl(Arg&& arg, Args&&... args) noexcept {
    return ((Arg&&)arg) || __next_t::__impl((Args&&)args...);
  }
};

template <class... Args>
MDSPAN_FORCE_INLINE_FUNCTION
constexpr typename __fold_right_or_impl_<Args...>::__rv
__fold_right_or_impl(Args&&... args) {
  return __fold_right_or_impl_<Args...>::__impl((Args&&)args...);
}

// </editor-fold> end right or }}}2
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// <editor-fold desc="right plus"> {{{2

template <class... Args>
struct __fold_right_plus_impl_;
template <class Arg>
struct __fold_right_plus_impl_<Arg> {
  using __rv = Arg&&;
  MDSPAN_FORCE_INLINE_FUNCTION
  static constexpr __rv
  __impl(Arg&& arg) noexcept {
    return (Arg&&)arg;
  }
};
template <class Arg1, class Arg2, class... Args>
struct __fold_right_plus_impl_<Arg1, Arg2, Args...> {
  using __next_t = __fold_right_plus_impl_<Arg2, Args...>;
  using __rv = decltype(std::declval<Arg1>() + std::declval<typename __next_t::__rv>());
  MDSPAN_FORCE_INLINE_FUNCTION
  static constexpr __rv
  __impl(Arg1&& arg, Arg2&& arg2, Args&&... args) noexcept {
    return ((Arg1&&)arg) + __next_t::__impl((Arg2&&)arg2, (Args&&)args...);
  }
};

template <class... Args>
MDSPAN_FORCE_INLINE_FUNCTION
constexpr typename __fold_right_plus_impl_<Args...>::__rv
__fold_right_plus_impl(Args&&... args) {
  return __fold_right_plus_impl_<Args...>::__impl((Args&&)args...);
}

// </editor-fold> end right plus }}}2
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// <editor-fold desc="right times"> {{{2

template <class... Args>
struct __fold_right_times_impl_;
template <class Arg>
struct __fold_right_times_impl_<Arg> {
  using __rv = Arg&&;
  MDSPAN_FORCE_INLINE_FUNCTION
  static constexpr __rv
  __impl(Arg&& arg) noexcept {
    return (Arg&&)arg;
  }
};
template <class Arg1, class Arg2, class... Args>
struct __fold_right_times_impl_<Arg1, Arg2, Args...> {
  using __next_t = __fold_right_times_impl_<Arg2, Args...>;
  using __rv = decltype(std::declval<Arg1>() * std::declval<typename __next_t::__rv>());
  MDSPAN_FORCE_INLINE_FUNCTION
  static constexpr __rv
  __impl(Arg1&& arg, Arg2&& arg2, Args&&... args) noexcept {
    return ((Arg1&&)arg) * __next_t::__impl((Arg2&&)arg2, (Args&&)args...);
  }
};

template <class... Args>
MDSPAN_FORCE_INLINE_FUNCTION
constexpr typename __fold_right_times_impl_<Args...>::__rv
__fold_right_times_impl(Args&&... args) {
  return __fold_right_times_impl_<Args...>::__impl((Args&&)args...);
}

// </editor-fold> end right times }}}2
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// <editor-fold desc="right assign"> {{{2

template <class... Args>
struct __fold_right_assign_impl_;
template <class Arg>
struct __fold_right_assign_impl_<Arg> {
  using __rv = Arg&&;
  MDSPAN_FORCE_INLINE_FUNCTION
  static constexpr __rv
  __impl(Arg&& arg) noexcept {
    return (Arg&&)arg;
  }
};
template <class Arg1, class Arg2, class... Args>
struct __fold_right_assign_impl_<Arg1, Arg2, Args...> {
  using __next_t = __fold_right_assign_impl_<Arg2, Args...>;
  using __rv = decltype(std::declval<Arg1>() = std::declval<typename __next_t::__rv>());
  MDSPAN_FORCE_INLINE_FUNCTION
  static constexpr __rv
  __impl(Arg1&& arg, Arg2&& arg2, Args&&... args) noexcept {
    return ((Arg1&&)arg) = __next_t::__impl((Arg2&&)arg2, (Args&&)args...);
  }
};

template <class... Args>
MDSPAN_FORCE_INLINE_FUNCTION
constexpr typename __fold_right_assign_impl_<Args...>::__rv
__fold_right_assign_impl(Args&&... args) {
  return __fold_right_assign_impl_<Args...>::__impl((Args&&)args...);
}

// </editor-fold> end right assign }}}2
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// <editor-fold desc="left assign"> {{{2

template <class... Args>
struct __fold_left_assign_impl_;
template <class Arg>
struct __fold_left_assign_impl_<Arg> {
  using __rv = Arg&&;
  MDSPAN_FORCE_INLINE_FUNCTION
  static constexpr __rv
  __impl(Arg&& arg) noexcept {
    return (Arg&&)arg;
  }
};
template <class Arg1, class Arg2, class... Args>
struct __fold_left_assign_impl_<Arg1, Arg2, Args...> {
  using __assign_result_t = decltype(std::declval<Arg1>() = std::declval<Arg2>());
  using __next_t = __fold_left_assign_impl_<__assign_result_t, Args...>;
  using __rv = typename __next_t::__rv;
  MDSPAN_FORCE_INLINE_FUNCTION
  static constexpr __rv
  __impl(Arg1&& arg, Arg2&& arg2, Args&&... args) noexcept {
    return __next_t::__impl(((Arg1&&)arg) = (Arg2&&)arg2, (Args&&)args...);
  }
};

template <class... Args>
MDSPAN_FORCE_INLINE_FUNCTION
constexpr typename __fold_left_assign_impl_<Args...>::__rv
__fold_left_assign_impl(Args&&... args) {
  return __fold_left_assign_impl_<Args...>::__impl((Args&&)args...);
}

// </editor-fold> end left assign }}}2
//------------------------------------------------------------------------------

#endif


template <class... Args>
constexpr __mdspan_enable_fold_comma __fold_comma_impl(Args&&... args) noexcept { return { }; }

template <bool... Bs>
struct __bools;

} // __fold_compatibility_impl

} // end namespace std

#  define _MDSPAN_FOLD_AND(...) std::__fold_compatibility_impl::__fold_right_and_impl((__VA_ARGS__)...)
#  define _MDSPAN_FOLD_OR(...) std::__fold_compatibility_impl::__fold_right_or_impl((__VA_ARGS__)...)
#  define _MDSPAN_FOLD_ASSIGN_LEFT(INIT, ...) std::__fold_compatibility_impl::__fold_left_assign_impl(INIT, (__VA_ARGS__)...)
#  define _MDSPAN_FOLD_ASSIGN_RIGHT(PACK, ...) std::__fold_compatibility_impl::__fold_right_assign_impl((PACK)..., __VA_ARGS__)
#  define _MDSPAN_FOLD_TIMES_RIGHT(PACK, ...) std::__fold_compatibility_impl::__fold_right_times_impl((PACK)..., __VA_ARGS__)
#  define _MDSPAN_FOLD_PLUS_RIGHT(PACK, ...) std::__fold_compatibility_impl::__fold_right_plus_impl((PACK)..., __VA_ARGS__)
#  define _MDSPAN_FOLD_COMMA(...) std::__fold_compatibility_impl::__fold_comma_impl((__VA_ARGS__)...)

#  define _MDSPAN_FOLD_AND_TEMPLATE(...) \
  _MDSPAN_TRAIT(std::is_same, __fold_compatibility_impl::__bools<(__VA_ARGS__)..., true>, __fold_compatibility_impl::__bools<true, (__VA_ARGS__)...>)

#endif

// </editor-fold> end fold expressions }}}1
//==============================================================================

//==============================================================================
// <editor-fold desc="Variable template compatibility"> {{{1

#if _MDSPAN_USE_VARIABLE_TEMPLATES
#  define _MDSPAN_TRAIT(TRAIT, ...) TRAIT##_v<__VA_ARGS__>
#else
#  define _MDSPAN_TRAIT(TRAIT, ...) TRAIT<__VA_ARGS__>::value
#endif

// </editor-fold> end Variable template compatibility }}}1
//==============================================================================

//==============================================================================
// <editor-fold desc="Pre-C++14 constexpr"> {{{1

#if _MDSPAN_USE_CONSTEXPR_14
#  define _MDSPAN_CONSTEXPR_14 constexpr
// Workaround for a bug (I think?) in EDG frontends
#  ifdef __EDG__
#    define _MDSPAN_CONSTEXPR_14_DEFAULTED
#  else
#    define _MDSPAN_CONSTEXPR_14_DEFAULTED constexpr
#  endif
#else
#  define _MDSPAN_CONSTEXPR_14
#  define _MDSPAN_CONSTEXPR_14_DEFAULTED
#endif

// </editor-fold> end Pre-C++14 constexpr }}}1
//==============================================================================

