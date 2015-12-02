// cmcstl2 - A concept-enabled C++ standard library
//
//  Copyright Casey Carter 2015
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/caseycarter/cmcstl2
//
#ifndef STL2_DETAIL_ITERATOR_INSERT_ITERATORS_HPP
#define STL2_DETAIL_ITERATOR_INSERT_ITERATORS_HPP

#include <cstddef>
#include <stl2/memory.hpp>
#include <stl2/detail/fwd.hpp>
#include <stl2/detail/raw_ptr.hpp>
#include <stl2/detail/iterator/basic_iterator.hpp>
#include <stl2/detail/iterator/concepts.hpp>

STL2_OPEN_NAMESPACE {
  namespace detail {
    template <MemberValueType Container>
    class insert_cursor_base {
    public:
      using container_type = Container;
      using iterator_category = output_iterator_tag;

      constexpr insert_cursor_base() noexcept = default;

      STL2_CONSTEXPR_EXT explicit
      insert_cursor_base(Container& x) noexcept
      : container_{std::addressof(x)} {}

    protected:
      raw_ptr<Container> container_{};
    };

    template <class T, class C>
    concept bool BackInsertableInto =
      requires (T&& t, C& c) {
        c.push_back((T&&)t);
      };

    template <MemberValueType Container>
    class back_insert_cursor : public insert_cursor_base<Container> {
      using base_t = insert_cursor_base<Container>;
    public:
      back_insert_cursor() = default;
      using base_t::base_t;

    private:
      friend cursor_access;
      template <BackInsertableInto<Container> T>
      void write(T&& t) {
        base_t::container_->push_back(__stl2::forward<T>(t));
      }
    };
  }

  ///////////////////////////////////////////////////////////////////////////
  // back_insert_iterator [back.insert.iterator]
  //
  template <detail::MemberValueType Container>
  using back_insert_iterator =
    basic_iterator<detail::back_insert_cursor<Container>>;

  template <detail::MemberValueType Container>
  auto back_inserter(Container& c) noexcept {
    return back_insert_iterator<Container>{c};
  }

  namespace detail {
    template <class T, class C>
    concept bool FrontInsertableInto =
      requires (T&& t, C& c) {
        c.push_front((T&&)t);
      };

    template <MemberValueType Container>
    class front_insert_cursor : public insert_cursor_base<Container> {
      using base_t = insert_cursor_base<Container>;
    public:
      front_insert_cursor() = default;
      using base_t::base_t;

    private:
      friend cursor_access;
      template <FrontInsertableInto<Container> T>
      void write(T&& t) {
        base_t::container_->push_front(__stl2::forward<T>(t));
      }
    };
  }

  ///////////////////////////////////////////////////////////////////////////
  // front_insert_iterator [front.insert.iterator]
  //
  template <detail::MemberValueType Container>
  using front_insert_iterator =
    basic_iterator<detail::front_insert_cursor<Container>>;

  template <detail::MemberValueType Container>
  auto front_inserter(Container& x) noexcept {
    return front_insert_iterator<Container>{x};
  }

  namespace detail {
    template <class T, class C>
    concept bool InsertableInto =
      requires (T&& t, C& c, typename C::iterator i) {
        {  c.insert(i, (T&&)t) } -> typename C::iterator;
      };

    template <MemberValueType Container>
      requires requires { typename Container::iterator; }
    class insert_cursor : public insert_cursor_base<Container> {
      using base_t = insert_cursor_base<Container>;
      using I = typename Container::iterator;
    public:

      insert_cursor() = default;
      STL2_CONSTEXPR_EXT explicit insert_cursor(Container& x, I i)
        noexcept(is_nothrow_move_constructible<I>::value) :
        base_t{x}, iter_{__stl2::move(i)} {}

    private:
      I iter_{};

      template <InsertableInto<Container> T>
      void write(T&& t) {
        iter_ = base_t::container_->insert(iter_, __stl2::forward<T>(t));
        ++iter_;
      }
    };
  }

  ///////////////////////////////////////////////////////////////////////////
  // insert_iterator [insert.iterator]
  //
  template <detail::MemberValueType Container,
            class I = typename Container::iterator>
  using insert_iterator =
    basic_iterator<detail::insert_cursor<Container>>;

  template <detail::MemberValueType Container,
            class I = typename Container::iterator>
  auto inserter(Container& x, I i)
  STL2_NOEXCEPT_RETURN(
    insert_iterator<Container>{x, __stl2::move(i)}
  )
} STL2_CLOSE_NAMESPACE

#endif
