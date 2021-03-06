// cmcstl2 - A concept-enabled C++ standard library
//
//  Copyright Casey Carter 2015
//  Copyright Eric Niebler 2015
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/caseycarter/cmcstl2
//
#ifndef STL2_DETAIL_CONCEPTS_FUNDAMENTAL_HPP
#define STL2_DETAIL_CONCEPTS_FUNDAMENTAL_HPP

#include <stl2/type_traits.hpp>
#include <stl2/detail/fwd.hpp>
#include <stl2/detail/concepts/compare.hpp>
#include <stl2/detail/concepts/core.hpp>
#include <stl2/detail/concepts/object.hpp>

STL2_OPEN_NAMESPACE {
	///////////////////////////////////////////////////////////////////////////
	// Scalar [Extension]
	//
	namespace ext {
		template<class T>
		META_CONCEPT Scalar =
			std::is_scalar_v<T> && Regular<T>;
	}

	///////////////////////////////////////////////////////////////////////////
	// Arithmetic [Extension]
	//
	namespace ext {
		template<class T>
		META_CONCEPT Arithmetic =
			std::is_arithmetic_v<T> && Scalar<T> && StrictTotallyOrdered<T>;
	}

	///////////////////////////////////////////////////////////////////////////
	// FloatingPoint [Extension]
	//
	namespace ext {
		template<class T>
		META_CONCEPT FloatingPoint =
			std::is_floating_point_v<T> && Arithmetic<T>;
	}

	///////////////////////////////////////////////////////////////////////////
	// Integral [concepts.lib.corelang.integral]
	//
	template<class T>
	META_CONCEPT Integral =
		std::is_integral_v<T> && ext::Arithmetic<T>;

	///////////////////////////////////////////////////////////////////////////
	// SignedIntegral [concepts.lib.corelang.signedintegral]
	//
	template<class T>
	META_CONCEPT SignedIntegral =
		Integral<T> && (T(-1) < T(0));

	///////////////////////////////////////////////////////////////////////////
	// UnsignedIntegral [concepts.lib.corelang.unsignedintegral]
	//
	template<class T>
	META_CONCEPT UnsignedIntegral =
		Integral<T> && !SignedIntegral<T>;
} STL2_CLOSE_NAMESPACE

#endif
