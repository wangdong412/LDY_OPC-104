/************************************************************************/
/*****     COPYRIGHT 1988           HONEYWELL LIMITED AUSTRALIA      ****/
/************************************************************************/


#ifndef c_almmsg_flag
#define c_almmsg_flag 

#include "src/cattbl_def"
#include "src/evtfil_def"
#include "src/lnktyp_def"
#include "src/prtque_def"
#include "src/points.h"

/*
@(#)almmsg.h,v	830.1
BEGIN_DOC
---------------------------------------------------------------------------
ALMMSG.H - define some constants for almmsg, hsc_insert_attrib fns & 
notif_send
---------------------------------------------------------------------------
DESCRIPTION:
    Define some priorities for almmsg.

    -----------------------------------------------------------------------
    NOTES - 
    -----------------------------------------------------------------------

DEFINITIONS:
*/

/*
The list of valid almmsg priorities are as follows:
    ALMMSG_JNL
    ALMMSG_LOW
    ALMMSG_HIGH
    ALMMSG_URGENT
    ALMMSG_EVENT+ALMMSG_JNL
    ALMMSG_EVENT+ALMMSG_LOW
    ALMMSG_EVENT+ALMMSG_HIGH
    ALMMSG_EVENT+ALMMSG_URGENT
*/
#define ALMMSG_ALARM  		0			/* priority offset alarming almmsg */
#define ALMMSG_EVENT  		12			/* priority offset event only almmsg */
#define ALMMSG_JNL    		PRQPRI_LOG		/* offset for a journal only priority */
#define ALMMSG_LOW    		PRQPRI_LOW		/* offset for a low priority */
#define ALMMSG_HIGH   		PRQPRI_HIGH		/* offset for a high priority */
#define ALMMSG_URGENT 		PRQPRI_URGENT		/* offset for a urgent priority */
#define ALMMSG_OP_CHANGE	PRQPRI_OP_CHANGE	/* offset for an operator change */
#define ALMMSG_REM_OP_CHANGE    PRQPRI_REM_OP_CHANGE   /* offset for a remote operator change */

/*
argument text can be broken up into the following fields:
*/
#define ALMMSG_NAME  		 0		
#define ALMMSG_ID     		(EVFALM - EVFPNT)	
#define ALMMSG_ACTION		(EVFACT - EVFPNT)
#define ALMMSG_LEVEL		(EVFLVL - EVFPNT)
#define ALMMSG_DESCR		(EVFDES - EVFPNT)
#define ALMMSG_VALUE		(EVFVAL - EVFPNT)
#define ALMMSG_UNITS		(EVFUNT - EVFPNT)
#define ALMMSG_REASON	    (EVFRSN - EVFPNT)
#define ALMMSG_NAMESZ  		EVFPNT_SZ
#define ALMMSG_IDSZ		EVFALM_SZ
#define ALMMSG_ACTIONSZ		EVFACT_SZ
#define ALMMSG_LEVELSZ		EVFLVL_SZ
#define ALMMSG_DESCRSZ		EVFDES_SZ
#define ALMMSG_VALUESZ		EVFVAL_SZ
#define ALMMSG_UNITSSZ		EVFUNT_SZ
#define ALMMSG_REASONSZ		EVFRSN_SZ

#define ALMMSG_TEXTSZ (EVFEND - EVFPNT)	/* maximum size of text */

// Link Types
#define NOTIF_LNK_VIDEO		LNK_VIDEO

// Link Flags that can be added to Link Types
// e.g NOTIF_LNK_DSA+NOTIF_LNK_VIDEO
#define	NOTIF_LNK_POPUP		LNK_POPUP_MASK
#define	NOTIF_LNK_DSA		LNK_DSA_MASK


typedef struct prtque NOTIF_STRUCT;

#include "src/eventmodes.h"

EXTERN_C int hsc_notif_send(NOTIF_STRUCT*, NOTIF_SEND_MODE);

EXTERN_C int hsc_insert_attrib(NOTIF_STRUCT*, const char*, PARvalue*, int2);
EXTERN_C int hsc_insert_attrib_byindex(NOTIF_STRUCT*, int2, PARvalue*, int2);
EXTERN_C int insert_attrib_byindex(NOTIF_STRUCT*, int2, PARvalue*, int2);

EXTERN_C PARvalue* TimetoPV(HSCTIME, PARvalue*);
EXTERN_C PARvalue* StrtoPV(const char*, PARvalue*);
EXTERN_C PARvalue* DbletoPV(double, PARvalue* pOut);
EXTERN_C PARvalue* RealtoPV(float value, PARvalue*);
EXTERN_C PARvalue* Int4toPV(int4, PARvalue*);
EXTERN_C PARvalue* Int2toPV(int2, PARvalue*);
EXTERN_C PARvalue* Int8toPV(int8, PARvalue*);
EXTERN_C PARvalue* PritoPV(int2, int2, PARvalue*);

#define	BUFFER_TOO_SMALL		0x8205
#define	BAD_VALUE			0x823E
#define	INV_CATEGORY			0x82E2
#define	CAT_ALREADY_ASSIGNED 		0x006C
#define	INV_ATTRIBUTE			0x0016
#define	VALUE_COULD_NOT_BE_CONVERTED	0x81F1
#define	ILLEGAL_TYPE 			0x8211
#define	INV_AREA	 		0x904E
#define	ATTR_NOT_IN_CAT  		0x9205


#define	HSC_ERROR	-1 	 /*  HSC general error */
#define	HSC_OK	0 	 	/*  HSC general ok */

/*
END_DOC
*/

#endif	/* end of conditional include almmsg_flag */

/************************************************************************/
/*****     COPYRIGHT 1988           HONEYWELL LIMITED AUSTRALIA      ****/
/************************************************************************/
