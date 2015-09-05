#ifndef STL2_DETAIL_CONCEPTS_FUNCTION_HPP
#define STL2_DETAIL_CONCEPTS_FUNCTION_HPP

#include <stl2/type_traits.hpp>
#include <stl2/detail/fwd.hpp>
#include <stl2/detail/concepts/compare.hpp>
#include <stl2/detail/concepts/core.hpp>
#include <stl2/detail/concepts/object.hpp>

////////////////////////////////////////////
// Function Concepts [concepts.lib.function]
//
STL2_OPEN_NAMESPACE {
  ///////////////////////////////////////////////////////////////////////////
  // Function [concepts.lib.functions.function]
  //
  template <class F, class...Args>
  using ResultType =
    result_of_t<F(Args...)>;

  template <class F, class...Args>
  concept bool Function() {
    return CopyConstructible<F>() &&
      requires (F& f, Args&&...args) {
        // not equality preserving
        STL2_EXACT_TYPE_CONSTRAINT(f((Args&&)args...),
                                   ResultType<F, Args...>);
      };
  }

  namespace models {
    template <class, class...>
    constexpr bool Function = false;
    __stl2::Function{F, ...Args}
    constexpr bool Function<F, Args...> = true;
  }

  ///////////////////////////////////////////////////////////////////////////
  // RegularFunction [concepts.lib.functions.regularfunction]
  //
  template <class F, class...Args>
  concept bool RegularFunction() {
    return Function<F, Args...>();
  }

  namespace models {
    template <class, class...>
    constexpr bool RegularFunction = false;
    __stl2::RegularFunction{F, ...Args}
    constexpr bool RegularFunction<F, Args...> = true;
  }

  ///////////////////////////////////////////////////////////////////////////
  // Predicate [concepts.lib.functions.predicate]
  //
  template <class F, class...Args>
  concept bool Predicate() {
    return RegularFunction<F, Args...>() &&
      Boolean<ResultType<F, Args...>>();
  }

  namespace models {
    template <class, class...>
    constexpr bool Predicate = false;
    __stl2::Predicate{F, ...Args}
    constexpr bool Predicate<F, Args...> = true;
  }

  ///////////////////////////////////////////////////////////////////////////
  // WeakRelation
  // Extension: Equivalent to Relation, but does not require common_type to
  //            be specialized.
  //
  namespace ext {
    template <class R, class T>
    concept bool WeakRelation() {
      return Predicate<R, T, T>();
    }

    template <class R, class T, class U>
    concept bool WeakRelation() {
      return WeakRelation<R, T>() &&
        WeakRelation<R, U>() &&
        Predicate<R, T, U>() &&
        Predicate<R, U, T>();
    }
  }

  namespace models {
    template <class R, class T, class U = T>
    constexpr bool WeakRelation = false;
    __stl2::ext::WeakRelation{R, T}
    constexpr bool WeakRelation<R, T, T> = true;
    __stl2::ext::WeakRelation{R, T, U}
    constexpr bool WeakRelation<R, T, U> = true;
  }

  ///////////////////////////////////////////////////////////////////////////
  // Relation [concepts.lib.functions.relation]
  //
  template <class R, class T>
  concept bool Relation() {
    return ext::WeakRelation<R, T>();
  }

  template <class R, class T, class U>
  concept bool Relation() {
    return ext::WeakRelation<R, T, U>() &&
      Common<T, U>() &&
      Predicate<R, CommonType<T, U>>();
  }

  namespace models {
    template <class R, class T, class U = T>
    constexpr bool Relation = false;
    __stl2::Relation{R, T}
    constexpr bool Relation<R, T, T> = true;
    __stl2::Relation{R, T, U}
    constexpr bool Relation<R, T, U> = true;
  }

  ///////////////////////////////////////////////////////////////////////////
  // StrictWeakOrder [concepts.lib.functions.strictweakorder]
  //
  template <class R, class T>
  concept bool StrictWeakOrder() {
    return Relation<R, T>();
  }

  template <class R, class T, class U>
  concept bool StrictWeakOrder() {
    return Relation<R, T, U>();
  }

  namespace models {
    template <class R, class T, class U = T>
    constexpr bool StrictWeakOrder = false;
    __stl2::StrictWeakOrder{R, T}
    constexpr bool StrictWeakOrder<R, T, T> = true;
    __stl2::StrictWeakOrder{R, T, U}
    constexpr bool StrictWeakOrder<R, T, U> = true;
  }
} STL2_CLOSE_NAMESPACE

#endif
