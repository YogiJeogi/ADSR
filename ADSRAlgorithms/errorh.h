#ifndef __ERRORH_H__
#define __ERRORH_H__

//#define _USENRERRORCLASS_ 1

// exception handling



#ifndef _USENRERRORCLASS_

#define throw(message) {printf("ERROR: %s\n     in file %s at line %d\n", message,__FILE__,__LINE__); throw(1);}

#else

struct NRerror {
	char *message;
	char *file;
	int line;
	NRerror(char *m, char *f, int l) : message(m), file(f), line(l) {}
};

#define throw(message) throw(NRerror(message,__FILE__,__LINE__));

void NRcatch(NRerror err) {
	printf("ERROR: %s\n     in file %s at line %d\n",
		err.message, err.file, err.line);
	exit(1);
}

#endif /*_USENRERRORCLASS_*/




// Floating Point Exceptions for Microsoft compilers
#ifdef _TURNONFPES_
#ifdef _MSC_VER
struct turn_on_floating_exceptions {
	turn_on_floating_exceptions() {
		int cw = _controlfp( 0, 0 );
		cw &=~(EM_INVALID | EM_OVERFLOW | EM_ZERODIVIDE );
		_controlfp( cw, MCW_EM );
	}
};
turn_on_floating_exceptions yes_turn_on_floating_exceptions;
#endif /* _MSC_VER */
#endif /* _TURNONFPES */

#endif /*__ERRORH_H__*/


// usage example:
//
//	try {
//		somebadroutine();
//	}
//	catch(NRerror s) {NRcatch(s);}
//
// (You can of course substitute any other catch body for NRcatch(s).)