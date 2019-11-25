#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <cstdio>
#include <cstdlib>

#define dbAbort() std::abort();

#define dbPrintFileLine()	\
	std::printf( "\t%s %i\n", __FILE__, __LINE__ );

#define dbPrintAssertion( condition ) {	\
	std::printf( "assertion failed: %s\n", #condition );	\
	dbPrintFileLine();	\
}

#define dbAssert( condition )	\
	if ( !( condition ) ) { dbPrintAssertion( condition ); dbAbort(); }

#define dbAssertMessage( condition, ... )	\
	if ( !( condition ) ) {	\
		dbPrintAssertion( condition );	\
		std::printf( "\n\t" );	\
		std::printf( __VA_ARGS__ );	\
		dbAbort();	\
	}

#define dbLog( ... ) std::printf( __VA_ARGS__ );
#define dbLogError( ... ) { std::printf( "Error: "); std::printf( __VA_ARGS__ ); dbPrintFileLine(); }
#define dbLogWarning( ... ) { std::printf( "Warning: "); std::printf( __VA_ARGS__ ); dbPrintFileLine(); }

#endif