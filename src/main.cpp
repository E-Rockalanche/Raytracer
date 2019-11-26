
#include "debug.hpp"
#include "material_handler.hpp"
#include "pixel.hpp"
#include "scene.hpp"

#include <SDL2/SDL.h>

#include <memory>

constexpr int ScreenWidth = 512;
constexpr int ScreenHeight = 512;

RGBPixel pixel_buffer[ ScreenWidth * ScreenHeight ];

Scene scene;

SDL_Rect render_area = {
	0,
	0,
	ScreenWidth,
	ScreenHeight
};

SDL_Rect crop_area = {
	0,
	0,
	ScreenWidth,
	ScreenHeight
};

RGBPixel test_colours[] = {
	0x000000,
	0xffffff,
	0xff0000,
	0xffff00,
	0x00ff00,
	0x00ffff,
	0x0000ff,
	0xff00ff
};
size_t test_colour_index = 0;

size_t test_texture_index = 0;

void clearImage()
{
	for( size_t i = 0; i < ScreenWidth * ScreenHeight; ++i )
		pixel_buffer[ i ] = test_colours[ test_colour_index ];
}

void handleKeyboardEvent( const SDL_KeyboardEvent& event )
{
	if ( event.state != SDL_PRESSED )
		return;

	switch ( event.keysym.sym )
	{
		case SDLK_r:
		{
			// render
			scene.render( ScreenWidth, ScreenHeight, pixel_buffer );
			break;
		}

		case SDLK_c:
		{
			test_colour_index = ( test_colour_index + 1 ) % std::size( test_colours );
			clearImage();
			break;
		}

		case SDLK_m:
		{
			std::cout << "number of materials: " << MaterialManager::numMaterials() << '\n';
			for( int i = 0; i < MaterialManager::numMaterials(); ++i )
			{
				std::cout << MaterialManager::getMaterial( i ) << '\n';
			}
			break;
		}
	}
}

int main( int argc, char** argv )
{

	MaterialManager::initialize();

	if ( argc != 2 )
	{
		std::cout << "requires scene filename\n";
		return 1;
	}
	else
	{
		bool ok = scene.loadScene( argv[1] );
		if ( !ok )
		{
			std::cout << "failed to load scene\n";
			return 1;
		}
	}

	clearImage();

	dbAssert( SDL_Init( SDL_INIT_VIDEO ) >= 0 );

	auto window = SDL_CreateWindow( "raytracer",
									SDL_WINDOWPOS_UNDEFINED,
									SDL_WINDOWPOS_UNDEFINED,
									ScreenWidth,
									ScreenHeight,
									SDL_WINDOW_SHOWN );
	dbAssert( window );

	auto renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE );
	dbAssert( renderer );

	auto texture = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, ScreenWidth, ScreenHeight );
	dbAssert( texture );

	// do each frame
	bool quit = false;
	do
	{
		// handle all pending events
		SDL_Event event;
		while ( SDL_PollEvent( &event ) )
		{
			switch ( event.type )
			{
				case SDL_QUIT:
					quit = true;
					break;

				case SDL_KEYDOWN:
				case SDL_KEYUP:
					handleKeyboardEvent( event.key );
					break;
			}
		}

		SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 ); // black
		SDL_RenderClear( renderer );

		SDL_UpdateTexture( texture, nullptr, pixel_buffer, ScreenWidth * sizeof( RGBPixel ) );
		SDL_RenderCopy( renderer, texture, &crop_area, &render_area );

		SDL_RenderPresent( renderer );
	}
	while ( !quit );

	SDL_DestroyTexture( texture );
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	SDL_Quit();
	return 0;
}