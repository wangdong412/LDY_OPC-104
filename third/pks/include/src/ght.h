/************************************************************************/
/*****     COPYRIGHT 1997 - 1998    HONEYWELL LIMITED AUSTRALIA      ****/
/************************************************************************/

/* Conditionally print out a trace of header file activity */
#ifdef _HEADER_FILE_TRACE_FLAG
#pragma message("Entering Header file: ght.h,v")
#endif

/* Check to see if we've finished processing this header file before */
#ifndef GHT_ENDFLAG

/* Check to see if we've partially processed this header file already */
#ifdef GHT_STARTFLAG
#pragma message("ERROR: Partially processed: ght.h,v")
#else
#define GHT_STARTFLAG

/*
@(#)ght.h,v	830.1
BEGIN_DOC
---------------------------------------------------------------------------
GHT.H - defines all specific types & function prototypes for the GHT (Global 
        Host Table)
---------------------------------------------------------------------------
SUMMARY:
    This is the header file for all the routines provided for manipulation of 
    the Global Host Table (rgnhGHT) and any GHT constants.
    The following functionality is allowed:
        * GHTIsCreated(void) - returns true if GHT has been created.
        * GHTCreate(nTransport) - create and initialise the GHT structure.
        * GHTDestroy(void) - destroy the GHT structure

        * GHTFindHostEntry(szHostname, *psa, nOptions, pnIndex) - 
            Tries to find the hostname in the GHT. If it is not found,
            create a new host entry in the GHT provided it is not full.
            Returns an index into the GHT to this host entry. This index
            must be stored by the user. GHTFindHostEntry is the only routine
            that can create a new entry in the GHT.

        * GHTInitHostEntry(nIndex) -
            Initialise a particular host entry in the GHT by setting the
            host type to GHT_HOST_NOT_INITIALISED.
        
        * GHTReadHostInfo(int iIndex,int *pfInitialised,int *pfTransport,
                          int *pnHostType,int *pcNumLnks) - 
            Reads all useful information host information for a particular
            entry in the GHT.

        * GHTReadCurrentLinkInfo(iIndex,szLnkHostname,psa,pfLnkStatus) - 
            Reads all useful link information regarding the current link 
            for a particular entry in the GHT.

        * GHTReadHostName(nIndex, pszHostname) - 
            Read the hostname for a particular entry in the GHT.

        * GHTReadHostType(nIndex, pnHostType) -
            Read the host type for a particular entry in the GHT.
                             -1 = Not initialised.
                              0 = Single Host, Single LAN
                              1 = Redund Host, Single LAN
                              2 = Single Host, Dual LAN
                              3 = Redund Host, Dual LAN 

        * GHTReadNumLnks(nIndex, pnNumLnks) -
            Read the number of host links applicable for a particular
            entry in the GHT.

        * GHTUpdateCurrentLnk(iIndex, *psa, fError) -
            If a connect to a link fails for a particular host system, call
            this routine to update the Current Link value. At the moment,
            this simply choses the next link in the list but the error has
            been included in the interface so that in the future, a smarter
            mechanism can be implement for determining the next current link.

        * GHTReadCurrentRaddr(nIndex, psa) -
            Reads the socket address structure for the current link of the
            specified entry in the GHT.

        * GHTReadCurrentLnkStatus(nIndex, pfLnkStatus) -
            Reads the status for the current host lnk of the specified entry
            in the GHT.
        * GHTWriteCurrentLnkStatus(nIndex, fLnkStatus) - 
            This is the only write functionality that the user may invoke. 
            The initial link status will be set by the GHTFindHostEntry routine. 
            But this function may be used to update the value of the current
            host link's status as appropriate.

END_DOC
*/

/**********************************************************
** INCLUDE FILES
**********************************************************/
#include "src/defs.h"
#include "src/netapi_flags.h"
#include "src/environ.h"
#include <errno.h>
#include <stdlib.h>

#ifdef NT
#include <memory.h>
#include <winsock.h>
#else
#include <netdb.h>		/* enable use of gethostbyname */
#include <netinet/in.h>
#include <sys/socket.h>
EXTERN_C char	*memcpy();
#endif



/* Define constants */
#define GHT_MAX_HOSTS       20
#define GHT_UDP             0
#define GHT_TCP             1
#define GHT_MAX_HOST_LINKS  4
#define GHT_HOST_NAME_LEN	30
#define GHT_HOST_BASENAME_LEN	28
#define GHT_HOST_TYPE_UNKNOWN -1
#define GHT_HOST_INITIALISED 1
#define GHT_HOST_NOT_INITIALISED 0

/* Define error codes */
#define GHT_NOT_CREATED		0x8800
#define GHT_ALREADY_CREATED	0x8801
#define GHT_INDEX_OUT_OF_RANGE	0x8802
#define GHT_INVALID_HOSTNAME	0x8803
#define GHT_INVALID_NUM_LNKS	0x8804
#define GHT_INVALID_LNK_NUM	0x8805
#define GHT_INVALID_LNK_STATUS	0x8806
#define GHT_UNINITIALISED_HOST_ENTRY	0x8807
#define GHT_HOST_TABLE_FULL	0x8808


/* define control structure for serving host information */
struct net_host
{
    char host_name[GHT_HOST_NAME_LEN+1];/* host name as requested by user */

    int  initialised;   /* initialised flag
                            0 = not initialised
                            1 = initialised     */
    int  transport;     /* Transport Protocol
                            0 = UDP
                            1 = TCP         */

    int  host_type;	    /* host type
                            -1 = host type unknown
                            bit 0 = 0 single host, 1 = Dual host
                            bit 1 = 0 single LAN,  1 = Dual Lan 
                     ie.    0 = Single Host, Single Lan
                            1 = Dual Host  , Single Lan
                            2 = Single Host, Dual Lan
                            3 = Dual Host  , Dual Lan */


    int  num_lnks;      /* total no. of links found for the host */

    int  current_lnk;   /* current host link
                                bit 0 = 0 host A,1 host B
                                bit 1 = 0 Lan A ,1 Lan B */

    char lnk_name[GHT_MAX_HOST_LINKS][GHT_HOST_NAME_LEN+1];
                        /* The hostname extension for each link     */
                        /* By appending these extension to the      */
                        /* hostname, the link hostname is generated */

    struct sockaddr_in	raddr[GHT_MAX_HOST_LINKS];
                        /* generic address store for 4 possible links */

    int  lnk_sts[GHT_MAX_HOST_LINKS];		
                        /* status of 4 possible links
                                0 = Good
                                1 = Backup
                                2 = Not running
                                3 = Failed  
                                4 = Unimplemented */
};


/* Function prototypes */
EXTERN_C int GHTIsCreated _DECLARE((void));
EXTERN_C int GHTCreate _DECLARE((int nTransport));
EXTERN_C int GHTDestroy _DECLARE((void));
EXTERN_C int GHTFindHostEntry _DECLARE((char *szHostname,struct sockaddr_in *psa,int nOptions,int *pnIndex));
EXTERN_C int GHTInitHostEntry _DECLARE((int nIndex));
EXTERN_C int GHTReadHostInfo _DECLARE((int iIndex,int *pfInitialised,int *pfTransport,int *pnHostType,int *pcNumLnks));
EXTERN_C int GHTReadCurrentLinkInfo _DECLARE((int iIndex,char *szLnkHostname,struct sockaddr_in *psa,int *pfLnkStatus));
EXTERN_C int GHTReadHostName _DECLARE((int nIndex,char **pszHostname));
EXTERN_C int GHTReadHostType _DECLARE((int nIndex,int *pnHostType));
EXTERN_C int GHTReadNumLnks _DECLARE((int nIndex,int *pnNumLnks));
EXTERN_C int GHTUpdateCurrentLnk _DECLARE((int nIndex,struct sockaddr_in *psa,int fError));
EXTERN_C int GHTReadCurrentRaddr _DECLARE((int nIndex,struct sockaddr_in *psa));
EXTERN_C int GHTReadCurrentLnkStatus _DECLARE((int nIndex,int *pfLnkStatus));
EXTERN_C int GHTWriteCurrentLnkStatus _DECLARE((int nIndex,int fLnkStatus));
EXTERN_C int GHTFreeString _DECLARE((char *szString));

#define GHT_ENDFLAG
#endif               /* end of if not *_startflag */
#endif               /* end of if not *_endflag */

/* Conditionally print out a trace of header file activity */
#ifdef _HEADER_FILE_TRACE_FLAG
#pragma message("Entering Header file: ght.h,v")
#endif

/************************************************************************/
/*****     COPYRIGHT 1997 - 1998    HONEYWELL LIMITED AUSTRALIA      ****/
/************************************************************************/
