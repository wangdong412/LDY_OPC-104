/************************************************************************/
/*****     COPYRIGHT 1996 - 1998    HONEYWELL LIMITED AUSTRALIA      ****/
/************************************************************************/

/* Conditionally print out a trace of header file activity */
#ifdef _HEADER_FILE_TRACE_FLAG
#pragma message("Header file: hscnapi.h,v")
#endif

/* Check to see if we've finished processing this header file before */
#ifndef HSCNAPI_ENDFLAG

/* Check to see if we've partially processed this header file already */
#ifdef HSCNAPI_STARTFLAG
#pragma message("ERROR: Partially processed: hscnapi.h,v")
#else
#define HSCNAPI_STARTFLAG

/*
@(#)hscnapi.h,v	830.1
BEGIN_DOC
---------------------------------------------------------------------------
hscnapi.h - Top level header file for users for Network API. Includes
            necessary types and and prototypes for network api functions.
---------------------------------------------------------------------------
SUMMARY:

DESCRIPTION:

    -----------------------------------------------------------------------
    NOTES - 
    -----------------------------------------------------------------------

DEFINITIONS:
*/

#define NAPI_DLL
#define GHT_DLL

#include "src/params.h"
#include "src/netapi_user.h"
#include "M4_err.h"

/*
END_DOC
*/


#define HSCNAPI_ENDFLAG
#endif               /* end of if not *_startflag */
#endif               /* end of if not *_endflag */

/************************************************************************/
/*****     COPYRIGHT 1996 - 1998    HONEYWELL LIMITED AUSTRALIA      ****/
/************************************************************************/
