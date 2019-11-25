#ifndef FS_PATH_HPP
#define FS_PATH_HPP

#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <utility>

namespace fs
{
	namespace detail
	{
		template <typename CharT>
		inline bool is_seperator( CharT c )
		{
			return c == '/' || c == '\\';
		}
	}

	class path
	{
	public:

		using value_type = char;
		using string_type = std::basic_string<value_type>;

		#if defined( WIN32 )
			static constexpr value_type preferred_seperator = '\\';
			static constexpr value_type nonpreferred_seperator = '/';
		#else
			static constexpr value_type preferred_seperator = '/';
			static constexpr value_type nonpreferred_seperator = '\\';
		#endif

		enum format
		{
			native_format,
			generic_format,
			auto_format
		};

	public: // constructors

		path() noexcept = default;
		path( const path& ) = default;
		path( path&& ) noexcept = default;

		path( string_type&& source, format fmt = auto_format )
		{
			(void)fmt;
			m_path = std::move( source );
		}

		template <typename Source>
		path( const Source& source, format fmt = auto_format )
		{
			(void)fmt;
			m_path = source;
		}

		template <typename InputIt>
		path( InputIt first, InputIt last, format fmt = auto_format )
		{
			(void)fmt;
			m_path = string_type( first, last );
		}

	public: // assignment

		path& operator=( const path& ) = default;
		path& operator=( path&& ) = default;

		path& operator=( string_type&& source )
		{
			m_path = std::move( source );
			return *this;
		}

		template <typename Source>
		path& operator=( const Source& source )
		{
			m_path = source;
			return *this;
		}

		path& assign( string_type&& source )
		{
			m_path = std::move( source );
			return *this;
		}

		template <typename Source>
		path& assign( const Source& source )
		{
			m_path = source;
			return *this;
		}

		template <typename InputIt>
		path& assign( InputIt first, InputIt last )
		{
			m_path = string_type( first, last );
			return *this;
		}

	public: // concatenation

		path& operator/=( const path& p )
		{
			m_path += preferred_seperator;
			m_path += p.m_path;
			return *this;
		}

		template <typename Source>
		path& operator/=( const Source& source )
		{
			m_path += preferred_seperator;
			m_path += source;
			return *this;
		}

		template <typename Source>
		path& append( const Source& source )
		{
			m_path += preferred_seperator;
			m_path += source;
			return *this;
		}

		template <typename InputIt>
		path& append( InputIt first, InputIt last )
		{
			m_path += preferred_seperator;
			m_path += string_type( first, last );
			return *this;
		}

		path& operator+=( const path& p )
		{
			m_path += p.m_path;
			return *this;
		}

		path& operator+=( const string_type& str )
		{
			m_path += str;
			return *this;
		}

		path& operator+=( std::basic_string_view<value_type> str )
		{
			m_path += str;
			return *this;
		}

		path& operator+=( const value_type* ptr )
		{
			m_path += ptr;
			return *this;
		}

		template <typename Source>
		path& operator+=( const Source& source )
		{
			m_path += source;
			return *this;
		}

		path& operator+=( value_type x )
		{
			m_path += x;
			return *this;
		}

		template <typename CharT>
		path& operator+=( CharT x )
		{
			m_path += x;
			return *this;
		}

		template <typename Source>
		path& concat( const Source& source )
		{
			m_path += source;
			return *this;
		}

		template <typename InputIt>
		path& concat( InputIt first, InputIt last )
		{
			m_path += string_type( first, last );
			return *this;
		}

	public: // modifier

		void clear() noexcept { m_path.clear(); }

		path& make_preferred()
		{
			for( char& c : m_path )
			{
				if ( c == nonpreferred_seperator )
					c = preferred_seperator;
			}
			return *this;
		}

		path& remove_filename();

		path& replace_filename( const path& replacement );

		path& replace_extension( const path& replacement = path() )
		{
			if ( has_extension() )
			{
				// remove extension
			}

			if ( !replacement.empty() )
			{
				if ( replacement.m_path[ 0 ] != '.' )
					*this += '.';
				*this += replacement;
			}

			return *this;
		}

		void swap( path& other ) noexcept
		{
			m_path.swap( other.m_path );
		}

	public: // format observers

		const value_type* c_str() const noexcept { return m_path.c_str(); }
		const string_type& native() const noexcept { return m_path; }
		operator string_type() const { return m_path; }

		template <class CharT, class Traits = std::char_traits<CharT>, class Alloc = std::allocator<CharT>>
		std::basic_string<CharT, Traits, Alloc> string( const Alloc& a = Alloc() ) const;
		std::string string() const { return m_path; }
		std::wstring wstring() const;
		std::u16string u16string() const;
		std::u32string u32string() const;
		std::string u8string() const;

		template <class CharT, class Traits = std::char_traits<CharT>, class Alloc = std::allocator<CharT>>
		std::basic_string<CharT, Traits, Alloc> generic_string( const Alloc& a = Alloc() ) const;
		std::string generic_string() const { return m_path; }
		std::wstring generic_wstring() const;
		std::u16string generic_u16string() const;
		std::u32string generic_u32string() const;
		std::string generic_u8string() const;

	public: // compare

		int compare( const path& p ) const noexcept
		{
			int result = root_name().native().compare( p.root_name().native() );
			if ( result != 0 )
				return result;

			result = has_root_directory() - p.has_root_directory();
			if ( result != 0 )
				return result;

			// TODO: compare relative paths

			return 0;
		}

		int compare( const string_type& str ) const { return compare( path( str ) ); }
		int compare( std::basic_string_view<value_type> str ) const { return compare( path( str ) ); }
		int compare( const value_type* s ) const { return compare( path( s ) ); }

	public: // generation

		path lexically_normal() const;
		path lexically_relative( const path& base ) const;
		path lexically_proximate( const path& base ) const;

	public: // decomposition

		path root_name() const;
		path root_directory() const;
		path root_path() const;
		path relative_path() const;

		path parent_path() const
		{
			auto it = find_last_seperator();
			return path( m_path.cbegin(), ( it >= m_path.cbegin() ) ? it : m_path.cbegin() );
		}

		path filename() const
		{
			auto it = find_last_seperator();
			return path( it + 1, m_path.cend() );
		}

		path stem() const
		{
			auto extension_start = extension_begin();
			auto stem_start = find_last_seperator() + 1;
			if ( extension_start == stem_start )
				return path( stem_start, m_path.end() );
			else
				return path( stem_start, extension_start );
		}

		path extension() const
		{
			return path( extension_begin(), m_path.cend() );
		}

	public: // queries

		[[nodiscard]] bool empty() const noexcept { return m_path.empty(); }

		bool has_root_path() const;
		bool has_root_name() const;
		bool has_root_directory() const;
		bool has_relative_path() const;
		bool has_parent_path() const;
		bool has_filename() const;
		bool has_stem() const;
		bool has_extension() const;

		bool is_absolute() const;
		bool is_relative() const;

	public: //iteration

	private:

		string_type::const_iterator find_first_seperator() const
		{
			return std::find_if( m_path.cbegin(), m_path.cend(), []( auto c ){ return detail::is_seperator( c ); } );
		}

		string_type::const_iterator find_last_seperator() const
		{
			auto it = std::find_if( m_path.crbegin(), m_path.crend(), []( auto c ){ return detail::is_seperator( c ); } );
			return --( it.base() );
		}

		string_type::const_iterator extension_begin() const
		{
			auto it = std::find_if( m_path.crbegin(), m_path.crend(), []( auto c ){ return ( c == '.' ) || detail::is_seperator( c ); } );
			if ( it == m_path.crend() || detail::is_seperator( *it ) )
				return m_path.cend();

			return --( it.base() );
		}

	private:
		string_type m_path;
	};

	inline bool operator==( const path& lhs, const path& rhs )
	{
		return lhs.compare( rhs ) == 0;
	}

	inline bool operator!=( const path& lhs, const path& rhs )
	{
		return lhs.compare( rhs ) != 0;
	}

	inline bool operator<( const path& lhs, const path& rhs )
	{
		return lhs.compare( rhs ) < 0;
	}

	inline bool operator>( const path& lhs, const path& rhs )
	{
		return lhs.compare( rhs ) > 0;
	}

	inline bool operator<=( const path& lhs, const path& rhs )
	{
		return lhs.compare( rhs ) <= 0;
	}

	inline bool operator>=( const path& lhs, const path& rhs )
	{
		return lhs.compare( rhs ) >= 0;
	}

	inline path operator/( path lhs, const path& rhs )
	{
		return lhs /= rhs;
	}
}

inline std::istream& operator>>( std::istream& in, fs::path& p )
{
	fs::path::string_type str;
	in >> str;
	p = std::move( str );
	return in;
}

inline std::ostream& operator<<( std::ostream& out, const fs::path& p )
{
	out << p.native();
	return out;
}

#endif