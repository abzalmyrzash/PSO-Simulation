#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include <stdexcept>


namespace utils
{
	inline float hireTimeInSeconds()
	{
		float t = SDL_GetTicks();
		t *= 0.001;
		return t;
	}

	template<typename ... Args>
	std::string format( const std::string& format, Args ... args )
	{
	    int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
	    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
	    auto size = static_cast<size_t>( size_s );
	    std::unique_ptr<char[]> buf( new char[ size ] );
	    std::snprintf( buf.get(), size, format.c_str(), args ... );
	    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
	}

	void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius);
}