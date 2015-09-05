#include <stl2/detail/iterator/ostreambuf_iterator.hpp>
#include <stl2/iterator.hpp>
#include <stl2/type_traits.hpp>
#include <sstream>
#include "../simple_test.hpp"

using namespace __stl2;

namespace {
  template <InputIterator I, Sentinel<I> S, WeakOutputIterator<ValueType<I>> O>
  tagged_pair<tag::in(I), tag::out(O)>
  constexpr copy(I first, S last, O out) {
    for (; first != last; ++first, void(), ++out) {
      *out = *first;
    }
    return {first, out};
  }

  template <InputRange R, class I = IteratorType<R>, WeakOutputIterator<ValueType<I>> O>
  constexpr tagged_pair<tag::in(safe_iterator_t<R>), tag::out(O)>
  copy(R&& range, O out) {
    return ::copy(__stl2::begin(range), __stl2::end(range), __stl2::move(out));
  }
}

int main() {
  using I = ostreambuf_iterator<char>;
  static_assert(models::OutputIterator<I, char>);
  static_assert(models::Sentinel<default_sentinel, I>);
  using C = CommonType<I, default_sentinel>;
  static_assert(is_same<C, I>());

  {
    static const char hw[] = "Hello, world!";
    auto hw_range = ext::make_range(__stl2::begin(hw), __stl2::end(hw) - 1);
    std::ostringstream os;
    auto r = ::copy(hw_range, I{os});
    CHECK(r.out() != default_sentinel{});
    ::check_equal(os.str(), hw_range);
  }

  return ::test_result();
}
