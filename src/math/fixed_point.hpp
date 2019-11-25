#ifndef FIXED_POINT_HPP
#define FIXED_POINT_HPP

#include <cstdint>
#include <functional>
#include <limits>
#include <stdexcept>
#include <type_traits>

namespace math
{

template <size_t INT_BITS, size_t FRAC_BITS>
class fixed_point;

class fixed_point_overflow : public std::runtime_error
{
	fixed_point_overflow() : std::runtime_error( "fixed point overflow" ) {}
};

namespace detail
{
	template <size_t SIZE>
	struct type_from_size
	{
		static constexpr bool is_specialized = false;
		using base_type = void;
		using next_type = void;
	};

	template <>
	struct type_from_size<64>
	{
		static constexpr bool is_specialized = true;
		using base_type = int64_t;
		using next_type = type_from_size<64>; // no next type
	};

	template <>
	struct type_from_size<32>
	{
		static constexpr bool is_specialized = true;
		using base_type = int32_t;
		using next_type = type_from_size<64>;
	};

	template <>
	struct type_from_size<16>
	{
		static constexpr bool is_specialized = true;
		using base_type = int16_t;
		using next_type = type_from_size<32>;
	};

	template <>
	struct type_from_size<8>
	{
		static constexpr bool is_specialized = true;
		using base_type = int8_t;
		using next_type = type_from_size<16>;
	};

	template <typename TO, typename FROM>
	constexpr
	std::enable_if_t<std::is_integral_v<TO> && std::is_integral_v<FROM>, TO>
	narrow_cast( const FROM& from )
	{
		static_assert( sizeof( TO ) < sizeof( FROM ) );

		TO result = static_cast<TO>( from );

		if ( static_cast<FROM>( result ) != from )
			throw std::runtime_error( "failed to narrow cast" );

		return result;
	}

	template <size_t I, size_t F>
	fixed_point<I, F> add( const fixed_point<I, F>& lhs, const fixed_point<I, F>& rhs )
	{
		auto result = fixed_point<I, F>::from_base( lhs.base() + rhs.base() );
		fixed_point<I, F> zero( 0 );

		bool lhs_sign = lhs < zero;
		bool rhs_sign = rhs < zero;
		bool result_sign = result < zero;

		if ( ( lhs_sign == rhs_sign ) && ( lhs_sign != result_sign ) )
			throw std::runtime_error( "fixed point addition overflow" );

		return result;
	}

	template <size_t I, size_t F>
	std::enable_if_t<not std::is_same_v<typename type_from_size<I + F>::base_type, typename type_from_size<I + F>::next_type>, fixed_point<I, F>>
	multiply( const fixed_point<I, F>& lhs, const fixed_point<I, F>& rhs )
	{
		using base_type = typename fixed_point<I, F>::base_type;
		using next_type = typename fixed_point<I, F>::next_type;

		fixed_point<I, F> zero( 0 );

		next_type intermediate = static_cast<next_type>( lhs.base() ) * static_cast<next_type>( rhs.base() );

		auto result = fixed_point<I, F>::from_base( narrow_cast<base_type>( intermediate / fixed_point<I, F>::one ) );

		bool same_sign = ( ( lhs < zero ) == ( rhs < zero ) );
		if ( same_sign == ( result < zero ) )
			throw std::runtime_error( "fixed point multiplication trap");

		return result;
	}

	// safer but slower multiply for largest base type
	template <size_t I, size_t F>
	std::enable_if_t<std::is_same_v<typename type_from_size<I + F>::base_type, typename type_from_size<I + F>::next_type>, fixed_point<I, F>>
	multiply( const fixed_point<I, F>& lhs, const fixed_point<I, F>& rhs )
	{
		using base_type = typename fixed_point<I, F>::base_type;
		base_type one = fixed_point<I, F>::one;

		base_type int_mask = fixed_point<I, F>::integer_mask;
		base_type frac_mask = fixed_point<I, F>::fractional_mask;

		base_type left_high = ( lhs.base() & int_mask ) / one;
		base_type right_high = ( rhs.base() & int_mask ) / one;
		base_type left_low = lhs.base() & frac_mask;
		base_type right_low = rhs.base() & frac_mask;

		base_type lh_rh = left_high * right_high;
		base_type lh_rl = left_high * right_low;
		base_type ll_rh = left_low * right_high;
		base_type ll_rl = left_low * right_low;

		auto result = fixed_point<I, F>::from_base( ( lh_rh * one ) + lh_rl + ll_rh + ( ll_rl / one ) );

		bool same_sign = ( lhs < 0 == rhs < 0 );
		if ( same_sign == result < 0 )
			throw std::runtime_error( "fixed point multiplication trap");

		return result;
	}

	template <size_t I, size_t F>
	std::enable_if_t<not std::is_same_v<typename type_from_size<I + F>::base_type, typename type_from_size<I + F>::next_type>, fixed_point<I, F>>
	divide( const fixed_point<I, F>& lhs, const fixed_point<I, F>& rhs )
	{
		using base_type = typename fixed_point<I, F>::base_type;
		using next_type = typename fixed_point<I, F>::next_type;

		if ( rhs == fixed_point<I, F>( 0 ) )
			throw std::runtime_error( "fixed point divide by zero" );

		next_type intermediate = static_cast<next_type>( lhs.base() ) * fixed_point<I, F>::one;
		base_type result = narrow_cast<base_type>( intermediate / rhs.base() );
		return fixed_point<I, F>::from_base( result );
	}

	// TODO: implement divide for largest base type
}

template <size_t INT_BITS, size_t FRAC_BITS>
class fixed_point
{
public:
	static_assert( INT_BITS > 0, "need at least one integer bit for sign" );
	static_assert( FRAC_BITS > 0, "use an integral type instead" );

	static constexpr size_t integer_bits = INT_BITS;
	static constexpr size_t fractional_bits = FRAC_BITS;
	static constexpr size_t total_bits = integer_bits + fractional_bits;

	static_assert( detail::type_from_size<total_bits>::is_specialized, "number of bits is unsupported" );

	using base_type = typename detail::type_from_size<total_bits>::base_type;
	using next_type = typename detail::type_from_size<total_bits>::next_type::base_type;

	static constexpr base_type one = ( 1 << fractional_bits );

	static constexpr base_type integer_mask = base_type( -1 ) << fractional_bits;
	static constexpr base_type fractional_mask = base_type( -1 ) & ~integer_mask;
	static constexpr base_type sign_bit = 1 << ( total_bits - 1 );

private: // helper constructor

	struct no_scale{};
	constexpr fixed_point( base_type data, no_scale ) : m_value( data ) {}

public: // constructors

	fixed_point() noexcept = default;

	constexpr fixed_point( const fixed_point& other ) : m_value( other.m_value ) {}

	template <size_t I, size_t F>
	constexpr fixed_point( const fixed_point<I, F>& other )
	{
		assign( other );
	}

	template <typename INT>
	constexpr fixed_point( std::enable_if_t<std::is_integral_v<INT> && not std::is_same_v<INT, bool>, INT> n ) noexcept
		: m_value( n << fractional_bits ) {}

	template <typename FLOAT>
	constexpr fixed_point( std::enable_if_t<std::is_floating_point_v<FLOAT>, FLOAT> n ) noexcept
		: m_value( static_cast<base_type>( n * one ) ) {}

	constexpr fixed_point( bool b ) noexcept
		: m_value( b ? one : 0 ) {}

	static constexpr fixed_point from_base( base_type data )
	{
		return fixed_point( data, no_scale{} );
	}

public: // assignment

	constexpr fixed_point& operator=( const fixed_point& other )
	{
		m_value = other.m_value;
		return *this;
	}

	template <size_t I, size_t F>
	constexpr fixed_point& operator=( const fixed_point<I, F>& other )
	{
		assign( other );
		return *this;
	}

	template <typename INT>
	constexpr
	std::enable_if_t<std::is_integral_v<INT> && not std::is_same_v<INT, bool>, fixed_point&>
	operator=( INT n ) noexcept
	{
		m_value = n << fractional_bits;
		return *this;
	}

	template <typename FLOAT>
	constexpr
	std::enable_if_t<std::is_floating_point_v<FLOAT>, fixed_point&>
	operator=( FLOAT n ) noexcept
	{
		m_value = static_cast<base_type>( n * one );
		return *this;
	}

	constexpr fixed_point& operator=( bool b ) noexcept
	{
		m_value = b ? one : 0;
		return *this;
	}

public: //conversion

	template <typename INT>
	constexpr
	std::enable_if_t<std::is_integral_v<INT> && not std::is_same_v<INT, bool>, INT>
	to() const
	{
		return m_value / one;
	}

	template <typename FLOAT>
	constexpr
	std::enable_if_t<std::is_floating_point_v<FLOAT>, FLOAT>
	to() const
	{
		return static_cast<FLOAT>( m_value ) / one;
	}

	template <typename T>
	explicit operator T() const { return to<T>(); }

	explicit operator bool() const { return m_value > 0; }

public:

	base_type base() const { return m_value; }

public: // unary operators

	fixed_point operator-() const { return fixed_point::from_base( -m_value ); }
	fixed_point operator+() const { return fixed_point::from_base( +m_value ); }

	fixed_point& operator++()
	{
		m_value += one;
		return *this;
	}

	fixed_point& operator--()
	{
		m_value -= one;
		return *this;
	}

	fixed_point operator++( int )
	{
		fixed_point copy( *this );
		m_value += one;
		return copy;
	}

	fixed_point operator--( int )
	{
		fixed_point copy( *this );
		m_value -= one;
		return copy;
	}

public: // math operators

	fixed_point& operator+=( const fixed_point& other )
	{
		*this = detail::add( *this, other );
		return *this;
	}

	fixed_point& operator-=( const fixed_point& other )
	{
		*this = detail::add( *this, -other );
		return *this;
	}

	fixed_point& operator*=( const fixed_point& other )
	{
		*this = detail::multiply( *this, other );
		return *this;
	}

	fixed_point& operator/=( const fixed_point& other )
	{
		*this = detail::divide( *this, other );
		return *this;
	}

public: // comparison

	template <typename T>
	bool operator==( const T& other ) const { return m_value == static_cast<fixed_point>( other ).m_value; }
	template <typename T>
	bool operator!=( const T& other ) const { return m_value != static_cast<fixed_point>( other ).m_value; }
	template <typename T>
	bool operator<( const T& other ) const { return m_value < static_cast<fixed_point>( other ).m_value; }
	template <typename T>
	bool operator>( const T& other ) const { return m_value > static_cast<fixed_point>( other ).m_value; }
	template <typename T>
	bool operator<=( const T& other ) const { return m_value <= static_cast<fixed_point>( other ).m_value; }
	template <typename T>
	bool operator>=( const T& other ) const { return m_value >= static_cast<fixed_point>( other ).m_value; }

private:

	template <size_t I, size_t F>
	void assign( const fixed_point<I, F>& other )
	{
		m_value = other.m_value * ( one / fixed_point<I, F>::one );

		if ( integer_bits < I && ( m_value / one ) != ( other.m_value / fixed_point<I, F>::one ) )
			throw std::runtime_error( "failed narrow fixed point conversion" );
	}

private:

	base_type m_value;
};

template <size_t I, size_t F>
fixed_point<I, F> operator+( const fixed_point<I, F>& lhs, const fixed_point<I, F>& rhs )
{
	return detail::add( lhs, rhs );
}

template <size_t I, size_t F>
constexpr  fixed_point<I, F> operator-( const fixed_point<I, F>& lhs, const fixed_point<I, F>& rhs )
{
	return detail::add( lhs, -rhs );
}

template <size_t I, size_t F>
fixed_point<I, F> operator*( const fixed_point<I, F>& lhs, const fixed_point<I, F>& rhs )
{
	return detail::multiply( lhs, rhs );
}

template <size_t I, size_t F>
fixed_point<I, F> operator/( const fixed_point<I, F>& lhs, const fixed_point<I, F>& rhs )
{
	return detail::divide( lhs, rhs );
}

} // namespace math

namespace std
{
	template <size_t I, size_t F>
	struct hash<math::fixed_point<I, F>>
	{
		using type_info = math::detail::type_from_size<I + F>;
		using base_type = typename type_info::base_type;

		size_t operator()( const math::fixed_point<I, F>& value ) const noexcept
		{
			return std::hash<base_type>{}( value.base() );
		}
	};

	template <size_t I, size_t F>
	struct numeric_limits<math::fixed_point<I, F>>
	{
		using T = math::fixed_point<I, F>;
		using base_type = typename T::base_type;

		static constexpr bool is_specialized = true;
		static constexpr T min() { return T::from_base( 1 ); }
		static constexpr T max() { return T::from_base( std::numeric_limits<base_type>::max() ); }
		static constexpr T lowest() { return T::from_base( std::numeric_limits<base_type>::min() ); }
		static constexpr int digits = static_cast<int>( I + F );
		static constexpr bool is_signed = std::numeric_limits<base_type>::is_signed;
		static constexpr bool is_integer = ( F == 0 );
		static constexpr bool is_exact = true;
		static constexpr int radix = 2;
		static constexpr T epsilon() { return min(); }
		static constexpr T round_error() { return min(); }
		static constexpr int min_exponent = 1 - F;
		static constexpr int max_exponent = I - 1;
		static constexpr bool has_infinity = false;
		static constexpr bool has_quiet_NaN = false;
		static constexpr bool has_signaling_NaN = false;
		static constexpr bool is_bounded = true;
		static constexpr bool is_modulo = true;
		static constexpr bool traps = true;
		static constexpr std::float_round_style round_style = std::round_toward_neg_infinity;
	};

	template <size_t I, size_t F>
	bool signbit( const math::fixed_point<I, F>& value )
	{
		return value.base() & math::fixed_point<I, F>::sign_bit;
	}

	template <size_t I, size_t F>
	math::fixed_point<I, F> abs( const math::fixed_point<I, F>& value )
	{
		return std::signbit( value ) ? -value : value;
	}

	template <size_t I, size_t F>
	math::fixed_point<I, F> floor( const math::fixed_point<I, F>& value )
	{
		return math::fixed_point<I, F>::from_base( value.base() & math::fixed_point<I, F>::integer_mask );
	}

	template <size_t I, size_t F>
	math::fixed_point<I, F> ceil( const math::fixed_point<I, F>& value )
	{
		using base_type = typename math::fixed_point<I, F>::base_type;

		base_type low = value.base() & math::fixed_point<I, F>::fractional_mask;
		base_type high = value.base() & math::fixed_point<I, F>::integer_mask;
		base_type result = high + ( ( low == 0 ) ? 0 : 1 );

		return math::fixed_point<I, F>::from_base( result );
	}

	template <size_t I, size_t F>
	math::fixed_point<I, F> round( const math::fixed_point<I, F>& value )
	{
		using base_type = typename math::fixed_point<I, F>::base_type;

		base_type low = value.base() & math::fixed_point<I, F>::fractional_mask;
		base_type high = value.base() & math::fixed_point<I, F>::integer_mask;
		base_type result = high + ( ( low & ( 1 << ( F - 1 ) ) ) ? 0 : 1 );

		return math::fixed_point<I, F>::from_base( result );
	}

	template <size_t I, size_t F>
	math::fixed_point<I, F> trunc( const math::fixed_point<I, F>& value )
	{
		using base_type = typename math::fixed_point<I, F>::base_type;

		base_type high = value.base() & math::fixed_point<I, F>::integer_mask;
		base_type low = value.base() & math::fixed_point<I, F>::fractional_mask;
		base_type result = high + ( ( low != 0 && std::signbit( value ) ) ? 1 : 0 );

		return math::fixed_point<I, F>::from_base( result );
	}

	template <size_t I, size_t F>
	math::fixed_point<I, F> copysign( const math::fixed_point<I, F>& x, const math::fixed_point<I, F>& y )
	{
		return std::signbit( x ) == std::signbit( y ) ? x : -x;
	}
}

#endif