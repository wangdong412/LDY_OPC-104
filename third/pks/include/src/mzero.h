/************************************************************************/
/*****     COPYRIGHT 2006           HONEYWELL LIMITED AUSTRALIA      ****/
/************************************************************************/

#ifndef mzero_flag
#define mzero_flag

#include "src/defs.h"

/*
BEGIN_DOC
------------------------------------------------------------------------------
MZERO	    - Test 32 bit float value for equal to -0.0
MZERO32	    - Test 32 bit float value for equal to -0.0
MZERO64	    - Test 64 bit float value for equal to -0.0
SET_MZERO   - Set 32 bit float value to -0.0
SET_MZERO32 - Set 32 bit float value to -0.0
SET_MZERO64 - Set 64 bit float value to -0.0
CLR_MZERO   - Set 32 bit float value to 0.0 if it was -0.0
CLR_MZERO32 - Set 32 bit float value to 0.0 if it was -0.0
CLR_MZERO64 - Set 64 bit float value to 0.0 if it was -0.0
------------------------------------------------------------------------------
DESCRIPTION:
	This header provides inline tests for -0.0.

  	MINUS_ZERO is tested as a long integer because floating point
	units vary in behaviour. MINUS_ZERO is represented as all zeros
	except for the sign bit which is set to 1.

	These routines use the definition of MINUS_ZERO from PLATFORM.

RETURN VALUES:
	TRUE if  -0.0
	FALSE otherwise

RELATED FUNCTIONS:
	mzero.c

END_DOC
*/

// ------ test ----------------------------------------------------------------

static inline int mzero (float* pVal)
{
    register long* p = (long*)pVal;
    if (p[0] == MINUS_ZERO)	
	return TRUE;
    return FALSE;
}

static inline int mzero32 (float* pVal)
{
    register long* p = (long*)pVal;
    if (p[0] == MINUS_ZERO)	
	return TRUE;
    return FALSE;
}

static inline int mzero64 (double* pVal)
{
    register long* p = (long*)pVal;
#if SWAP_BYTES	    /* little endian */
    if (p[0] == 0 && p[1] == MINUS_ZERO)	
	return TRUE;
#else		    /* big endian */
    if (p[0] == MINUS_ZERO && p[1] == 0)	
	return TRUE;
#endif
    return FALSE;
}


// ------ set -----------------------------------------------------------------

static inline void set_mzero (float* pVal)
{
    register long* p = (long*)pVal;
    p[0] = MINUS_ZERO;
}

static inline void set_mzero32 (float* pVal)
{
    register long* p = (long*)pVal;
    p[0] = MINUS_ZERO;
}

static inline void set_mzero64 (double* pVal)
{
    register long* p = (long*)pVal;
#if SWAP_BYTES	    /* little endian */
    p[0] = 0;
    p[1] = MINUS_ZERO;
#else		    /* big endian */
    p[0] = MINUS_ZERO;
    p[1] = 0;
#endif
}


// ------ clear ---------------------------------------------------------------

static inline void clr_mzero (float* pVal)
{
    register long* p = (long*)pVal;
    if (p[0] == MINUS_ZERO)	
	p[0] = 0;
}

static inline void clr_mzero32 (float* pVal)
{
    register long* p = (long*)pVal;
    if (p[0] == MINUS_ZERO)	
	p[0] = 0;
}

static inline void clr_mzero64 (double* pVal)
{
    register long* p = (long*)pVal;
#if SWAP_BYTES	    /* little endian */
    if (p[0] == 0 && p[1] == MINUS_ZERO)
	p[1] = 0;
#else		    /* big endian */
    if (p[0] == MINUS_ZERO && p[1] == 0)	
	p[0] = 0;
#endif
}

#endif		/* end of mzero_flag if */

/************************************************************************/
/*****     COPYRIGHT 2006           HONEYWELL LIMITED AUSTRALIA      ****/
/************************************************************************/
