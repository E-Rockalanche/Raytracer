
#include "debug.hpp"
#include "material_handler.hpp"
#include "pixel.hpp"
#include "scene.hpp"

#include <SDL2/SDL.h>

#include <memory>

constexpr int ScreenWidth = 480;
constexpr int ScreenHeight = 320;

RGBPixel pixel_buffer[ScreenWidth * ScreenHeight];

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

void clearImage()
{
	for ( int x = 0; x < ScreenWidth; x++ )
	{
		for ( int y = 0; y < ScreenHeight; y++ )
		{
			int rgb = ( ( x / 8 + y / 8 ) % 2 ) ? 255 : 0;
			pixel_buffer[ y * ScreenWidth + x ] = RGBPixel( rgb, rgb, rgb );
		}
	}
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
			clearImage();
			scene.render( ScreenWidth, ScreenHeight, pixel_buffer );
			break;
		}
	}
}

int main( int argc, char** argv )
{

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

	MaterialManager::initialize();

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