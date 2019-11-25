#ifndef PIXEL_HPP
#define PIXEL_HPP

#include <cstdint>

class RGBPixel
{
public:
	RGBPixel() = default;
	constexpr RGBPixel( uint32_t rgb )
		: r( rgb >> 16 ), g( rgb >> 8 ), b( rgb )
	{}
	constexpr RGBPixel( uint8_t red, uint8_t green, uint8_t blue )
		: r( red ), g( green ), b( blue )
	{}
	constexpr RGBPixel( const RGBPixel& ) = default;

	uint8_t* data() { return (uint8_t*)(char*)this; }
	const uint8_t* data() const { return (const uint8_t*)(const char*)this; }

	uint8_t& operator[]( size_t index ) { return data()[ index ]; }
	uint8_t operator[]( size_t index ) const { return data()[ index ]; }

	uint8_t r;
	uint8_t g;
	uint8_t b;
};

class RGBAPixel
{
public:
	RGBAPixel() = default;
	constexpr RGBAPixel( uint32_t abgr_ ) : abgr( abgr_ ) {}
	constexpr RGBAPixel( uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255 )
		: r( red ), g( green ), b( blue ), a( alpha )
	{}
	constexpr RGBAPixel( const RGBAPixel& other ) : abgr( other.abgr ) {}
	constexpr RGBAPixel( const RGBPixel& other, uint8_t alpha = 255 )
		: r( other.r ), g( other.g ), b( other.b ), a( alpha )
	{}

	uint8_t* data() { return (uint8_t*)(char*)this; }
	const uint8_t* data() const { return (const uint8_t*)(const char*)this; }

	uint8_t& operator[]( size_t index ) { return data()[ index ]; }
	uint8_t operator[]( size_t index ) const { return data()[ index ]; }

	union
	{
		struct
		{
			uint8_t r;
			uint8_t g;
			uint8_t b;
			uint8_t a;
		};

		//assuming little endian
		uint32_t abgr;
	};
};

#endif