/************************************************************************/
/*****     COPYRIGHT 1989 - 2002    HONEYWELL LIMITED AUSTRALIA      ****/
/************************************************************************/

/* Conditionally print out a trace of header file activity */
#ifdef _HEADER_FILE_TRACE_FLAG
#pragma message("Entering Header file: netapi_flags.h,v")
#endif

/* Check to see if we've finished processing this header file before */
#ifndef NETAPI_FLAGS_ENDFLAG

/* Check to see if we've partially processed this header file already */
#ifdef NETAPI_FLAGS_STARTFLAG
#pragma message("ERROR: Partially processed: netapi_flags.h,v")
#else
#define NETAPI_FLAGS_STARTFLAG

/*
@(#)netapi_flags.h,v	830.1.1.1
BEGIN_DOC
---------------------------------------------------------------------------
NETAPI_FLAGS.H - Network API file containing flags, constants, error
                 values and raw defines.
---------------------------------------------------------------------------
SUMMARY:

DESCRIPTION:

    -----------------------------------------------------------------------
    NOTES -
    -----------------------------------------------------------------------

DEFINITIONS:

STRUCTURE:

END_DOC
*/

/***********************************************
** INCLUDE FILES
***********************************************/
#include "src/defs.h"
#include "src/almmsg.h"
#include "src/cntque_def"

/***********************************************
** Constants
***********************************************/

/* Maximum point and paramater name and value string lengths.
   The following constants (defined in defs.h) should be used instead
   of the NIF_MAX_POINT_NAME_LEN and NIF_MAX_ASCII_PARAM_LEN constants:
     MAX_POINT_NAME_LEN          - The maximum length of a point name
     MAX_PARAM_NAME_LEN          - The maximum length of a parameter name
     MAX_PARAM_VALUE_STRING_LEN  - The maximum string length that can be stored in a parameter value
*/
#define     NIF_MAX_POINT_NAME_LEN      MAX_POINT_NAME_LEN  /* maintained for backwards compatability only - use MAX_POINT_NAME_LEN in any new code */
#define     NIF_MAX_ASCII_PARAM_LEN     MAX_PARAM_NAME_LEN  /* maintained for backwards compatability only - use MAX_PARAM_NAME_LEN in any new code */

/* Nif specific constants */
#define     NIF_MAX_HISTORY_POINTS      20
#define     NIF_MAX_HISTORY_VALUES      2000
#define     NIF_MAX_NEW_HISTORY_POINTS  1100
#define     NIF_MAX_ARCHIVE_PATH_LEN    100
#define     NIF_MAX_GETDAT_STR_LEN      2048

/* define two data types to simplify puts of enum values on the client */
#define DT_ENUM_ORD	20	/* enum where only ordinal value supplied */
#define DT_ENUM_STR	21	/* enum where only C char string supplied */

/* common message layout */
#define	NADS_REQUEST			0x1

/* define socket address family for NADS */
#define AF_NADS 		AF_INET		/* currently internet family */

/* define the maximum number of NADS network interfaces */
#define MAX_NADS_INTERFACES	5

/* define maximum datagram size for current transport */
#define MAX_DGRAM_SIZE 4096

/* NADS stream pointer information */
#define NADS_STREAM_UNINITIALISED   -1

/* define nads host type information */
#define NADS_HOST_UNINITIALISED   -1
#define NADS_SINGLE_HOST           0
#define NADS_REDUN_HOST            1
#define NADS_SINGLE_HOST_DUAL_LAN  2
#define NADS_REDUN_HOST_DUAL_LAN   3

/* define nads current host link information */
#define NADS_DUAL_HOST        1
#define NADS_DUAL_LAN         2
#define NADS_DUAL_LNK         1
#define NADS_LAN_LNK          2

/* define nads host-link resolution status (by gethostbyname) */
#define NADS_HOST_LNK_UNRESOLVED   -1
#define NADS_HOST_LNK_RESOLVED      0

/* define max possible links & types supported etc */
#define NADS_MAX_HOST_LINKS         4
#define NADS_MAX_HOST_TYPES         4
#define NADS_UNKNOWN_HOST_TYPE     -1
#define NADS_UNDEFINED_LINK        -1

/* define default wait period (seconds and microseconds) and retries for datagram transport */
#define NADS_DEFAULT_WAIT_PERIOD_S	5
#define NADS_DEFAULT_WAIT_PERIOD_US	0
#define NADS_DEFAULT_RETRIES		4

/* define maximum length of a host name under NADS */
#define NADS_HOST_NAME_LEN	30

/* specify NADS Stream options */
#define NADSOPT_BROADCAST	0x0001

/* define various receive modes for cd_rcv() */
#define MATCH_HOST			0x1
#define MATCH_PORT			0x2
#define MATCH_TRANS_ID			0x4
#define RECEIVE_MATCHED_REPLY		(MATCH_HOST|MATCH_PORT|MATCH_TRANS_ID)
#define RECEIVE_BROADCAST_REPLY		(MATCH_PORT|MATCH_TRANS_ID)
#define RECEIVE_ANY_REPLY		0

/* define current NADS version */
#define	NADS_VERS1	1

/* define type flags for rgetdat and rputdat functions */
#define RGETDAT_TYPE_INT2		2
#define RGETDAT_TYPE_BITS		3
#define RGETDAT_TYPE_INT4		4
#define RGETDAT_TYPE_REAL4		5
#define RGETDAT_TYPE_REAL8		6
#define RGETDAT_TYPE_STR		7

/* History types for rgethst and rgetval_hst */
#define HST_1MIN	1
#define HST_6MIN	2
#define HST_1HOUR	3
#define HST_8HOUR	4
#define HST_24HOUR	5
#define HST_5SECF	6
#define HST_1HOURE	7
#define HST_8HOURE	8
#define HST_24HOURE	9

/* Subscription period for hsc_param_value* */
#define NADS_ONE_SHOT   0
#define NADS_READ_CACHE  0
#define NADS_READ_DEVICE -1

/* Define flag and constant for enum to ord converting */
#define NADS_ENUM_AS_ORD_FLAG 0x1
#define NIF_GENERAL_FLAGS_DEFAULT 0

/* Notification types and other constants */
#define NTFN_ALARM_JNL     ALMMSG_JNL
#define NTFN_ALARM_LOW     ALMMSG_LOW
#define NTFN_ALARM_HIGH    ALMMSG_HIGH
#define NTFN_ALARM_URGENT  ALMMSG_URGENT
#define NTFN_EVENT          PRQPRI_REM_OP_CHANGE
#define NTFN_NAMESZ     ALMMSG_NAMESZ
#define NTFN_EVENTSZ    ALMMSG_IDSZ
#define NTFN_ACTIONSZ   ALMMSG_ACTIONSZ
#define NTFN_LEVELSZ    ALMMSG_LEVELSZ
#define NTFN_DESCRSZ    ALMMSG_DESCRSZ
#define NTFN_VALUESZ    ALMMSG_VALUESZ
#define NTFN_UNITSSZ    ALMMSG_UNITSSZ
#define NTFN_TEXTSZ     ALMMSG_TEXTSZ

static char* EventStrings[] = 	{
		// should be an alarm type, an event type from
		// one of below, blank or user defined
		"CHANGE",  // local operator change
		"ACHANGE", // application (non-Station) change
		"LOGIN",   // operator login
		"ALOGIN",  // application (non-Station) login
		"WDT",	   // watch dog timer event
		"FAILED"   // operation failed
				};

static char* ActionStrings[] = 	{
		// should be blank (new alarm), an event type from
		// one of below or user defined
		"OK", 	// alarm returned to normal
		"ACK", 	// alarm acknowledged
		"CNF" 	// message confirmed
				};

static char* LevelStrings[]  = 	{
		// should be an alarm level, one of the
		// following strings indicating where the event
		// was generated from, blank or user defined
		"CB", 	  // Control Builder
		"MSEDE",  // Microsoft Excel Data Exchange
		"NAPI",   // Network API application
		"API"	  // API application
				};

/* Time to wait before attempt to lock database will fail (rgetdat/rputdat) */
/* value is in milliseconds */
#define NADS_LOCK_FAIL_WAIT 50

/* Define some standard return values */
#define HSC_C_ERROR	-1
#define HSC_C_SUCCESS	0

/* Define VB codes */
#define UNICODE_STRINGS 1
#define ANSI_STRINGS    0

/* Bad parameter code */
#define NADS_BAD_PARAMETER_FLAG 65535  /* Used in n_convert(). Send to pre-sabre server */
                                       /* if sabre gives us a bad parameter (0) */

#define NIF_SERVER_NAME         "hsc_nif"
#define NIF_SERVER_PROTOCOL     "udp"
#define NIF_SERVER_PORT			50000
#define NIF_SERVER_PROG_NO		1

/***********************************************
** Function code mappings
***********************************************/

#define NIF_TIMES_FUNC_NO		1
#define NIF_PROFILE_ON_FUNC_NO		8
#define NIF_PROFILE_OFF_FUNC_NO		9
#define NIF_GETPNT_FUNC_NO		10
#define NIF_GETVAL_NUMB_FUNC_NO		11
#define NIF_GETVAL_ASCII_FUNC_NO	12
#define NIF_GETVAL_HIST_FUNC_NO		13
#define NIF_PUTVAL_NUMB_FUNC_NO		14
#define NIF_PUTVAL_ASCII_FUNC_NO	15
#define NIF_PUTVAL_HIST_FUNC_NO		16
#define NIF_GETHST_DATE_FUNC_NO		17
#define NIF_GETHST_OFST_FUNC_NO		18
#define NIF_GETDAT_FUNC_NO		20
#define NIF_PUTDAT_FUNC_NO		21
#define NIF_PARAM_HIST_DATES_FUNC_NO	22
#define NIF_PARAM_HIST_OFFSETS_FUNC_NO	23
#define NIF_RQSTSK_FUNC_NO		24
#define NIF_RQTSKB_FUNC_NO		25
#define NIF_POINT_NUMBERS_FUNC_NO	30
#define NIF_PARAM_NUMBERS_FUNC_NO	31
#define NIF_PARAM_VALUES_FUNC_NO	32
#define NIF_PARAM_VALUE_PUTS_FUNC_NO	33
#define NIF_PARAM_VALUE_BYNAMES_FUNC_NO	34
#define NIF_PARAM_VALUE_PUT_BYNAMES_FUNC_NO	35
#define NIF_PARAM_HIST_OFFSET_BYNAMES_FUNC_NO	36
#define NIF_PARAM_HIST_DATE_BYNAMES_FUNC_NO	37
#define NIF_NOTIF_FUNC_NO		38
#define NIF_PARAM_VALUE_PUTS_SEC_FUNC_NO	39
#define NIF_PARAM_VALUE_PUT_SEC_BYNAMES_FUNC_NO	40


/***********************************************
** Error Numbers
***********************************************/

#define	NADS_TX_BUFFER_FULL	0x8381	/* message being built to large */
#define	NADS_RX_BUFFER_EMPTY	0x8382  /* no more data in received message */
#define	NADS_NO_SUCH_VERS	0x8383  /* requested version not supported */
#define	NADS_NO_SUCH_FUNC	0x8384	/* requested function not supported */
#define	NADS_WRONG_PROGRAM	0x8385	/* wrong program number */
#define	NADS_RCV_TIMEOUT	0x8386	/* receive timeout */
#define	NADS_INCOMPLETE_HEADER	0x8387	/* runt request or reply received */
#define	NADS_HOST_MISMATCH	0x8388	/* host address mismatch */
#define	NADS_PORT_MISMATCH	0x8389	/* port address mismatch */
#define	NADS_TRANS_ID_MISMATCH	0x838A	/* transaction id mismatch */
#define	NADS_PROFILE_ERROR	0x838B	/* multiple servers are running or
					   server does not support profile */
#define	NADS_BAD_POINT_PAR	0x838C	/* BAD point parameter value */
#define	NADS_NO_DLL		    0x838D	/* No Network DLL could be found */
#define NADS_INVALID_PROT	0x838E	/* Unknown Network protocol specified */
#define NADS_TX_ER	        0x838F	/* Failed to transmit message */
#define NADS_INIT_ER		0x8390	/* Could not initialise the system correctly */
#define NADS_SOCK_ER		0x8391	/* Socket count not be opened */
#define NADS_HOST_ER		0x8392	/* Server name not recognised */
#define NADS_RX_ERROR		0x8393	/* Failed to receive message */
#define NADS_CLOSE_ERR		0x8394	/* Could not close socket correctly */
#define NADS_GLOBAL_LOCK_FAIL	0x8395	/* Failed to access internal memory */
#define NADS_GLOBAL_ALLOC_FAIL	0x8396	/* Failed to allocated space in memory */
#define NADS_INVALID_STATUS	0x8397	/* Invalid Status received from Server */
#define NADS_HOST_NOT_PRIMARY   0x8398 /* Host in dual redundant backup mode */
#define NADS_HOST_NOT_IMPLEMENTED 0x8399 /* host link not implemented */
#define NADS_PARTIAL_FUNC_FAIL  0x839A /* List-based func returned at least
					 one status in error */
#define NADS_INVALID_LIST_SIZE  0x839B /* List size passed into func is <= 0 */
#define	NADS_REQ_COUNT_MISMATCH	0x839C	/* Request count mismatch */
#define	NADS_VAR_TYPE_MISMATCH	0x839D	/* Mismatch between VARIANT and
					   PARvalue types */
#define NADS_HOST_NOT_RUNNING   0x839E /* Host not running (aborted) */
#define NADS_ARRAY_OVERFLOW     0x839F /* Array is not large enough for data */
#define NADS_ARRAY_DIM_ERROR    0x83A0 /* Safearray is incorrectly dimensioned */
#define NADS_ARRAY_INVALID_ELEMENT_SIZE 0x83A1 /* Safearray elements incorrect size */
#define NADS_INVALID_DATABASE_ADDR 0x83A2 /* Attempt to write to an invalid address */

/***********************************************
** Message layout
***********************************************/

/* common message layout */
#define	TRANS_ID_OFF			0
#define FLAGS_OFF			(TRANS_ID_OFF + OCTETS_PER_SHORT)
#define FUNC_OFF			(FLAGS_OFF + OCTETS_PER_SHORT)

/* additional request message layout */
#define	PROG_OFF			(FUNC_OFF + OCTETS_PER_SHORT)
#define	VERS_OFF			(PROG_OFF + OCTETS_PER_SHORT)
#define	REQ_RES_1			(VERS_OFF + OCTETS_PER_SHORT)
#define	REQ_COUNT_OFF			REQ_RES_1    /* # of reqsts in packet */
#define	REQ_RES_2			(REQ_RES_1 + OCTETS_PER_SHORT)
#define	REQ_FLAGS1			REQ_RES_2    // general flags. Bit 0 = change enums to int4s on a get
#define	REQ_RES_3			(REQ_RES_2 + OCTETS_PER_SHORT)
#define REQ_DATA_OFF			(REQ_RES_3 + OCTETS_PER_SHORT)
#define REQ_HDR_SIZE			REQ_DATA_OFF

/* additional reply message layout */
#define	REP_STS_OFF			(FUNC_OFF + OCTETS_PER_SHORT)
#define	REP_RES_1			(REP_STS_OFF + OCTETS_PER_SHORT)
#define	REP_RES_2			(REP_RES_1 + OCTETS_PER_SHORT)
#define	REP_RES_3			(REP_RES_2 + OCTETS_PER_SHORT)
#define REP_DATA_OFF			(REP_RES_3 + OCTETS_PER_SHORT)
#define REP_HDR_SIZE			REP_DATA_OFF

/* define constants for header information byte offsets into stream buffer */
#define OCTETS_PER_RAW      sizeof(n_raw)
#define OCTETS_PER_CHAR     sizeof(n_char)
#define OCTETS_PER_SHORT    sizeof(n_short)
#define OCTETS_PER_LONG     sizeof(n_long)
#define OCTETS_PER_FLOAT    sizeof(n_float)
#define OCTETS_PER_DOUBLE   sizeof(n_double)
#define OCTETS_PER_INT8     sizeof(n_int8)

#define NETAPI_FLAGS_ENDFLAG
#endif               /* end of if not *_startflag */
#endif               /* end of if not *_endflag */

/* Conditionally print out a trace of header file activity */
#ifdef _HEADER_FILE_TRACE_FLAG
#pragma message("Leaving Header file: netapi_flags.h,v")
#endif

/************************************************************************/
/*****     COPYRIGHT 1989 - 2002    HONEYWELL LIMITED AUSTRALIA      ****/
/************************************************************************/
