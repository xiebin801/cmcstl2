#ifndef STL2_DETAIL_CONCEPTS_ALGORITHM_HPP
#define STL2_DETAIL_CONCEPTS_ALGORITHM_HPP

#include <stl2/functional.hpp>
#include <stl2/iterator.hpp>
#include <stl2/detail/fwd.hpp>
#include <stl2/detail/concepts/callable.hpp>

///////////////////////////////////////////////////////////////////////////
// Common algorithm requirements [commmonalgoreq]
//
STL2_OPEN_NAMESPACE {
  ///////////////////////////////////////////////////////////////////////////
  // IndirectlyComparable [indirectlycomparable.commmonalgoreq]
  //
  template <class I1, class I2,
            class R = equal_to<>, class P1 = identity, class P2 = identity>
  concept bool IndirectlyComparable() {
    return IndirectCallableRelation<R, Projected<I1, P1>, Projected<I2, P2>>();
  }

  namespace models {
    template <class, class, class, class, class>
    constexpr bool IndirectlyComparable = false;
    __stl2::IndirectlyComparable{I1, I2, R, P1, P2}
    constexpr bool IndirectlyComparable<I1, I2, R, P1, P2> = true;
  }

  ///////////////////////////////////////////////////////////////////////////
  // Permutable [permutable.commmonalgoreq]
  //
  template <class I>
  concept bool Permutable() {
    return ForwardIterator<I>() &&
      Movable<ValueType<I>>() &&
      IndirectlyMovable<I, I>();
  }

  namespace models {
    template <class>
    constexpr bool Permutable = false;
    __stl2::Permutable{I}
    constexpr bool Permutable<I> = true;
  }

  ///////////////////////////////////////////////////////////////////////////
  // Mergeable [mergeable.commmonalgoreq]
  //
  template <class I1, class I2, class Out,
            class R = less<>, class P1 = identity, class P2 = identity>
  concept bool Mergeable() {
    return InputIterator<I1>() &&
      InputIterator<I2>() &&
      WeaklyIncrementable<Out>() &&
      IndirectlyCopyable<I1, Out>() &&
      IndirectlyCopyable<I2, Out>() &&
      IndirectCallableStrictWeakOrder<R, Projected<I1, P1>, Projected<I2, P2>>();
  }

  namespace models {
    template <class, class, class, class, class, class>
    constexpr bool Mergeable = false;
    __stl2::Mergeable{I1, I2, Out, R, P1, P2}
    constexpr bool Mergeable<I1, I2, Out, R, P1, P2> = true;
  }

  ///////////////////////////////////////////////////////////////////////////
  // MergeMovable [mergemovable.commmonalgoreq]
  //
  template <class I1, class I2, class Out,
            class R = less<>, class P1 = identity, class P2 = identity>
  concept bool MergeMovable() {
    return InputIterator<I1>() &&
      InputIterator<I2>() &&
      WeaklyIncrementable<Out>() &&
      IndirectlyMovable<I1, Out>() &&
      IndirectlyMovable<I2, Out>() &&
      IndirectCallableStrictWeakOrder<R, Projected<I1, P1>, Projected<I2, P2>>();
  }

  namespace models {
    template <class, class, class, class, class, class>
    constexpr bool MergeMovable = false;
    __stl2::MergeMovable{I1, I2, Out, R, P1, P2}
    constexpr bool MergeMovable<I1, I2, Out, R, P1, P2> = true;
  }

  ///////////////////////////////////////////////////////////////////////////
  // Sortable [sortable.commmonalgoreq]
  //
  template <class I, class R = less<>, class P = identity>
  concept bool Sortable() {
    return ForwardIterator<I>() &&
      Permutable<I>() &&
      IndirectCallableStrictWeakOrder<R, Projected<I, P>>();
  }

  namespace models {
    template <class, class, class>
    constexpr bool Sortable = false;
    __stl2::Sortable{I, R, P}
    constexpr bool Sortable<I, R, P> = true;
  }
} STL2_CLOSE_NAMESPACE

#endif
