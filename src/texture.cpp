
#include "math/vec4.hpp"
#include "texture.hpp"

#include <stdexcept>
#include <iostream>
#include <cmath>

#define clamp(value, low, high) std::max(low, std::min(high, value))

Texture::Texture( unsigned char* data, int width, int height, int channels )
	: width( width ), height( height ), channels( channels ), data( data )
{
	if ( data == NULL )
	{
		throw std::runtime_error( "texture data is NULL" );
	}
	if ( width == 0 )
	{
		throw std::runtime_error( "texture width is 0" );
	}
	if ( height == 0 )
	{
		throw std::runtime_error( "texture height is 0" );
	}
	if ( channels <= 0 )
	{
		throw std::runtime_error( "texture channels is less than 1" );
	}
	else if ( channels > 4 )
	{
		throw std::runtime_error( "texture channels is greater than 4" );
	}
}

math::vec4 Texture::samplePixel( int x, int y ) const
{
	if ( x < 0 || y < 0 || x >= width || y >= height )
	{
		throw std::runtime_error( "texture sample position out of bounds" );
	}
	unsigned char* p = data + ( x + ( height - y - 1 ) * width ) * channels;
	float r, g, b, a;
	switch ( channels )
	{
		case 1:
			r = g = b = p[0];
			a = 255.0;
			break;
		case 2:
			r = p[0];
			g = p[1];
			b = a = 255.0;
			break;
		case 3:
			r = p[0];
			g = p[1];
			b = p[2];
			a = 255.0;
			break;
		case 4:
			r = p[0];
			g = p[1];
			b = p[2];
			a = p[3];
			break;
		default:
			throw std::runtime_error( "no support for more than 4 colour channels" );
	}
	return math::vec4( r, g, b, a ) / 255.0;
}

math::vec4 Texture::sampleColour( float x, float y, Texture::SampleMode sample_mode, Texture::EdgeMode edge_mode ) const
{
	math::vec4 colour;

	switch ( edge_mode )
	{
		default:
		case REPEAT:
			while ( x >= 1 )
			{
				x -= 1;
			}
			while ( x < 0 )
			{
				x += 1;
			}
			while ( y >= 1 )
			{
				y -= 1;
			}
			while ( y < 0 )
			{
				y += 1;
			}
			break;

		case CLAMP:
			x = clamp( x, 0.0f, 1.0f );
			y = clamp( y, 0.0f, 1.0f );
			break;
	}

	switch ( sample_mode )
	{
		default:
		case LINEAR:
		{
			int xl, xr, yt, yb;

			xl = x * ( width - 1 ) - 0.5;
			yt = y * ( height - 1 ) - 0.5;

			xl = clamp( xl, 0, width - 1 );
			xr = clamp( xl + 1, 0, width - 1 );
			yt = clamp( yt, 0, height - 1 );
			yb = clamp( yt + 1, 0, height - 1 );

			float wt = x * ( width - 1 ) - xl;
			float ht = y * ( height - 1 ) - yt;

			math::vec4 col_tl, col_tr, col_bl, col_br;
			col_tl = samplePixel( xl, yt );
			col_tr = samplePixel( xr, yt );
			col_bl = samplePixel( xl, yb );
			col_br = samplePixel( xr, yb );

			math::vec4 col_top = ( 1 - wt ) * col_tl + wt * col_tr;
			math::vec4 col_bottom = ( 1 - wt ) * col_bl + wt * col_br;
			colour = ( 1 - ht ) * col_top + ht * col_bottom;
		}
		break;

		case NEAREST:
		{
			int px = clamp( ( int )( x * width ), 0, width - 1 );
			int py = clamp( ( int )( y * height ), 0, height - 1 );
			colour = samplePixel( px, py );
		}
		break;
	}

	return colour;
}