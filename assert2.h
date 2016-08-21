#ifndef Assert2Included
#define Assert2Included

#include <assert.h>		/* for assert */
#include <stdio.h>		/* for fprintf */

/* like assert, but also prints a message */
#define assert2(_c_, _msg_) ((_c_) ? 0 : (fprintf(stderr, "\nAssertion Failed: %s\n", _msg_), fflush(stderr), assert(_c_)))

#endif // included
