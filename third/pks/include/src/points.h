/************************************************************************/
/*****     COPYRIGHT 1996-2007      HONEYWELL LIMITED AUSTRALIA      ****/
/************************************************************************/

#ifndef points_flag
#define points_flag

#include "src/shheap.h"
#include "src/dictionary.h"
#include "src/dat000_def"
#include "src/ext000_def"
#include "src/cnt000_def"
#include "src/des000_def"
#include "src/points_def"
#include "parameters"

/*
@(#)points.h,v	830.5.1.18
BEGIN_DOC
---------------------------------------------------------------------------
POINTS.H - point record database definitions
---------------------------------------------------------------------------
SUMMARY:
    This file contains a description of the shared memory heap
    used for holding the point database.

DESCRIPTION:
    The first malloc in the heap contains the point index array.
    Entry 0 holds the next point number to use.
    Entry 1 holds the number of points.
    Entry 2 onwards are pointers to point instances, and if NULL,
    means that the point is not implemented.

	------------
        | next     |
     -->| size = n |
	| point  1 |	-----------------
	| point  2 |--->| parameters	|
	| point  3 |	| type		|
	| .....  . |	| ...		|
	| .....  . |	| DAT		|
	| .....  . |	| EXT		|
	| .....  . |	| CNT		|
	| point  n |	| DES		|
	-----------	-----------------

    The point consists of a parameter block for additional
    parameters, a point type (STA,ANA,etc), some other fields,
    and then the old four part point record.

    The parameter block contans one or more variable length structures.
    Variable length, because it contains the parameter names which vary
    in length. The names will be null terminated and padded so that
    the next parameter starts on a 4 byte boundary.
    The structure is optimised for GDA, containing the bare minimum
    to be able to describe a remote parameter. The value pointer
    points to a DICTvalue structure. For GDA, this is in the GDA heap
    and only exists while the parameter is being accessed. For other
    methods, the vaue structure is held in the point heap and is
    permanent. The min/max pointer points to an optional block of four
    DICTvalue structures, one each for the limit min, limit max,
    range min, and range max.

    -----------------------------------------------------------------------
    NOTES -

    WARNING:
	Definitions in points_def are dependent on the following structure
 	definitions. ENSURE YOU MODIFY POINTS_DEF AS WELL.

    -----------------------------------------------------------------------

DEFINITIONS:
*/

// do not use _NEXT_POINT or _NUMBER_OF_POINTS unless inside points_lib
// in an intimate scenario. Never as a loop variable/condition.
#define _NEXT_POINT		((int *)points)[-1]
#define _NUMBER_OF_POINTS	((int *)points)[ 0]

#define PARAM_VAL     0		/* Items of a parameter */
#define PARAM_LMT_MIN 1
#define PARAM_LMT_MAX 2
#define PARAM_RNG_MIN 3
#define PARAM_RNG_MAX 4

	/* parameter numbers
	 * fixed parameter numbers are allocated
	 * 	in dictionary.src as numbers 1 to 1000
	 * point subtype parameyter numbers (child class) are allocated
	 * 	in dictionary.src as numbers 1001 to 2000
	 * flexibble parameters are allocated
	 * 	in param_create.c as numbers 32767 to 2001 */
#define PARAM_NUM_MIN          1
#define PARAM_NUM_MAX      32767
#define PARAM_NUM_MIN_FLEX  2001
#define PARAM_NUM_MAX_FLEX 32767

#define PPTNUM_POINT_DETAIL_DISPLAY	0	/* PointDetailDisplay */
#define PPTNUM_GROUP_DETAIL_DISPLAY	1	/* GroupDetailDisplay */
#define PPTNUM_ASSOCIATED_DISPLAY	2	/* AssociatedDisplay */
#define PPTNUM_INSTRUCTION_DISPLAY	3	/* InstructionDisplay */
#define PPTNUM_UNITS			4	/* Units */
#define PPTNUM_DESCRIPTION		5	/* Description */

#define POINT_LOCK_TIME 10000

#define PARAM_MAX_VARIANT_SIZE 32765 /* maximum size of variant data */

#define ONE_SHOT	0	/* one shot acquire (subscribe period) */
#define HSC_READ_CACHE	0	/* read data from cache (subscribe period) */
#define HSC_READ_DEVICE	-1	/* read data from device (subscribe period) */

#define PARAM_TYPE_CONV 0	/* convert the parameter type to a standard HSC type*/
#define PARAM_TYPE_RAW  1	/* don't convert the parameter type*/

#define NULL_PRIORITY -1	/* Use when calling putvl and not wanting to specify a command or residual priority */

#define MAX_HIERARCHY_RECURSION 1000	/* maximum level of recursion before we bail out */
					/* (note: not meant to be a restriction but an unexpected error) */

#define MAX_LIC_COLLECTION_POINTS   5000    /* maximum number of collection points that can be built */
#define PNT_HIERARCHY_MAX_PARENTS   500	    /* maximum number of parents that can be added to a point */

typedef int2 PNTRELDIR;		/* point relationship direction (see points_def for available values) */

typedef int2 PNTRELTYP;		    /* point relationship type */
#define PNTRELTYP_INVALID	0
#define PNTRELTYP_CONTAINMENT	1
#define PNTRELTYP_REFERENCE	2

/* flags for modifying behavior of point relations' update functions like hsc_point_update_relatives() */
#define PNTREL_ALREADY_CHECKED		1
#define PNTREL_UPDATE_ALMCOUNTS		2
#define PNTREL_DONT_UPDATE_TIMESTAMP 4
#define PNTREL_POINT_IS_DELETED		8

#define PNTALM_AGGREGATE_OFFSET	0   /* offset of alarm count parameters that will return aggregate counts */
#define PNTALM_LEAF_OFFSET	2   /* offset of alarm count parameters that will return leaf counts */

#define PNTALM_STNIDX_FULLCOUNT	-1  /* station index for full alarm counts without regard to SOR */

#define BBDOCMASK 0xFFFF		/*Dependent object code in BBIndex*/

// flags for hsc_point_create_ex
#define PNTCREATE_KEEP_LOCKED   1   // Keep the point locked after the hsc_point_create_ex function returns. Caller must unlock the point.

/*
STRUCTURE:
*/

typedef uint2 PNTNUM;		/*  0 for invalid point		*/
typedef  int2 PNTTYP;		/* -1 for invalid type		*/
typedef uint2 PRMNUM;		/*  0 for invalid parameter	*/
typedef  int2 PRMTYP;		/* -1 for invalid type		*/
typedef uint2 PPTNUM;		/* property number		*/

#define PNTNUM_INVALID  0
#define PNTTYP_INVALID -1
#define PNTTYP_NONE	0	/* used when we don't have a remote type*/
#define PRMNUM_INVALID  0
#define PRMTYP_INVALID -1
#define PNTSUBTYP_NONE  0

/************************************************************************************/
/* WARNING: Any modifications to the following structures may also require updates  */
/* to be made to the constants in points_def.                                       */
/************************************************************************************/

/*
  Layout of the alarm count structure
  Note that offsets are words not bytes.

    ---------------------------------
  1 !      UrgentRTNUnackAlms       !
    ---------------------------------
  2 !      UrgentActiveUnackAlms    !
    ---------------------------------
  3 !      UrgentActiveAckAlms      !
    ---------------------------------
  4 !      HighRTNUnackAlms         !
    ---------------------------------
  5 !      HighActiveUnackAlms      !
    ---------------------------------
  6 !      HighActiveAckAlms        !
    ---------------------------------
  7 !      LowRTNUnackAlms          !
    ---------------------------------
  8 !      LowActiveUnackAlms       !
    ---------------------------------
  9 !      LowActiveAckAlms         !
    ---------------------------------
 10 !      TotalUnackInfoMsgs       !
    ---------------------------------
 11 !      TotalUnackConfMsgs       !
    ---------------------------------
 12 !      TotalAckConfMsgs         !
    ---------------------------------
 13 !      Station                  !
    ---------------------------------
 14 !      AlarmState               !
    ---------------------------------
 15 !      LastReadTime             !
    !                               !
 16 !                               !
    ---------------------------------
 17 !      DisabledAlms             !
    ---------------------------------
 18 !      TotalRTNUnackAlts        !
    ---------------------------------
 19 !      TotalActiveUnackAlts     !
    ---------------------------------
 20 !      TotalActiveAckAlts       !
    ---------------------------------
 21 !      DisabledAlts             !
    ---------------------------------
 22 !      ShelvedAlms              !
    ---------------------------------
 23 !      ShelvedAlts              !
    ---------------------------------
 24 !      SpareWord1               !
    ---------------------------------

*/

typedef struct
    {
    uint2 UrgentRTNUnackAlms;	    /* Count of urgent priority RTN unack alarms    */
    uint2 UrgentActiveUnackAlms;    /* Count of urgent priority active unack alarms */
    uint2 UrgentActiveAckAlms;	    /* Count of urgent priority active ack alarms   */
    uint2 HighRTNUnackAlms;	    /* Count of high priority RTN unack alarms	    */
    uint2 HighActiveUnackAlms;	    /* Count of high priority active unack alarms   */
    uint2 HighActiveAckAlms;	    /* Count of high priority active ack alarms	    */
    uint2 LowRTNUnackAlms;	    /* Count of low priority RTN unack alarms	    */
    uint2 LowActiveUnackAlms;	    /* Count of low priority active unack alarms    */
    uint2 LowActiveAckAlms;	    /* Count of low priority active ack alarms	    */
    uint2 TotalUnackInfoMsgs;	    /* Count of unack information messages	    */
    uint2 TotalUnackConfMsgs;	    /* Count of unack confirmable messages	    */
    uint2 TotalAckConfMsgs;	    /* Count of ack confirmable messages	    */
    int2 Station;		    /* Station number counts apply to		    */
    uint2 AlarmState;		    /* Current alarm state (see points_def)	    */
    uint4 LastReadTime;		    /* Last read time				    */
    uint2 DisabledAlms;		    /* Count of all disabled alarms		    */
    uint2 TotalRTNUnackAlts;	    /* Count of RTN unack alerts		    */
    uint2 TotalActiveUnackAlts;	    /* Count of active unack alerts		    */
    uint2 TotalActiveAckAlts;	    /* Count of active ack alerts		    */
    uint2 DisabledAlts;		    /* Count of all disabled alerts		    */
    uint2 ShelvedAlms;		    /* Count of all shelved alarms		    */
    uint2 ShelvedAlts;		    /* Count of all shelved alerts		    */
    uint2 SpareWord1;		    /* Spare word to align structure		    */
    } PNTalmcount;

/*
  Layout of the point structures.
  Note that offsets are words not bytes.

    ---------------------------------
  1 !          parameters           !
    !                               !
  2 !                               !
    ---------------------------------
  3 !          type                 !
    ---------------------------------
  4 !          size                 !
    ---------------------------------
  5 !          parents              !
    !                               !
  6 !                               !
    ---------------------------------
  7 !          children             !
    !                               !
  8 !                               !
    ---------------------------------
  9 !          almcounts            !
    !                               !
 10 !                               !
    ---------------------------------
 11 !          BBArray              !
    !                               !
 12 !                               !
    ---------------------------------
 13 !          fullalmcount         !
    !          (48 bytes)           !
 36 !                               !
    ---------------------------------
 37 !          guid                 !
    !          (16 bytes)           !
 44 !                               !
    ---------------------------------
 45 !          subtype              !
    ---------------------------------
 46 !          itemnumber           !
    ---------------------------------
 47 !          flags                !
    ---------------------------------
 48 !          owner                !
    ---------------------------------
 49 !          remotetype           !
    ---------------------------------
 50 !          spare1               !
    ---------------------------------
 51 !          spare2               !
    ---------------------------------
 52 !          spare3               !
     ---------------------------------
 53 !          entityname           !
    !          (40 chars)           !
 72 !                               !
    ---------------------------------
    !          dat                  !
    !                               !
    !                               !
    ---------------------------------
    !          ext                  !
    !                               !
    !                               !
    ---------------------------------
    !          cnt                  !
    !                               !
    !                               !
    ---------------------------------
    !          des                  !
    !                               !
    !                               !
    ---------------------------------
*/

typedef struct
    {
    GINDX	parameters;
    uint2	type;
    uint2	size;
    GINDX	parents;
    GINDX	children;
    GINDX	almcounts;
    GINDX	BBArray;
    PNTalmcount fullalmcount;
    uint2	guid[PNT_GUID_SZ/2];
    uint2	subtype;
    uint2	itemnumber;
    uint2	flags;
    PNTNUM	owner;
    uint2	remotetype;
    uint2	spare1;
    uint2	spare2;
    uint2	spare3;
    uint2	entityname[PNT_ENTITYNAME_SZ/2];
    STA_DAT	dat;
    STA_EXT	ext;
    STA_CNT	cnt;
    STA_DES	des;
    } STApoint;

typedef struct
    {
    GINDX	parameters;
    uint2	type;
    uint2	size;
    GINDX	parents;
    GINDX	children;
    GINDX	almcounts;
    GINDX	BBArray;
    PNTalmcount fullalmcount;
    uint2	guid[PNT_GUID_SZ/2];
    uint2	subtype;
    uint2	itemnumber;
    uint2	flags;
    PNTNUM	owner;
    uint2	remotetype;
    uint2	spare1;
    uint2	spare2;
    uint2	spare3;
    uint2	entityname[PNT_ENTITYNAME_SZ/2];
    ANA_DAT	dat;
    ANA_EXT	ext;
    ANA_CNT	cnt;
    ANA_DES	des;
    } ANApoint;

typedef struct
    {
    GINDX	parameters;
    uint2	type;
    uint2	size;
    GINDX	parents;
    GINDX	children;
    GINDX	almcounts;
    GINDX	BBArray;
    PNTalmcount fullalmcount;
    uint2	guid[PNT_GUID_SZ/2];
    uint2	subtype;
    uint2	itemnumber;
    uint2	flags;
    PNTNUM	owner;
    uint2	remotetype;
    uint2	spare1;
    uint2	spare2;
    uint2	spare3;
    uint2	entityname[PNT_ENTITYNAME_SZ/2];
    ACC_DAT	dat;
    ACC_EXT	ext;
    ACC_CNT	cnt;
    ACC_DES	des;
    } ACCpoint;

typedef struct
    {
    GINDX	parameters;
    uint2	type;
    uint2	size;
    GINDX	parents;
    GINDX	children;
    GINDX	almcounts;
    GINDX	BBArray;
    PNTalmcount fullalmcount;
    uint2	guid[PNT_GUID_SZ/2];
    uint2	subtype;
    uint2	itemnumber;
    uint2	flags;
    PNTNUM	owner;
    uint2	remotetype;
    uint2	spare1;
    uint2	spare2;
    uint2	spare3;
    uint2	entityname[PNT_ENTITYNAME_SZ/2];
    ACS_DAT	dat;
    ACS_EXT	ext;
    ACS_CNT	cnt;
    ACS_DES	des;
    } ACSpoint;

typedef struct
    {
    GINDX	parameters;
    uint2	type;
    uint2	size;
    GINDX	parents;
    GINDX	children;
    GINDX	almcounts;
    GINDX	BBArray;
    PNTalmcount fullalmcount;
    uint2	guid[PNT_GUID_SZ/2];
    uint2	subtype;
    uint2	itemnumber;
    uint2	flags;
    PNTNUM	owner;
    uint2	remotetype;
    uint2	spare1;
    uint2	spare2;
    uint2	spare3;
    uint2	entityname[PNT_ENTITYNAME_SZ/2];
    CDA_DAT	dat;
    CDA_EXT	ext;
    CDA_CNT	cnt;
    CDA_DES	des;
    } CDApoint;

typedef struct
    {
    GINDX	parameters;
    uint2	type;
    uint2	size;
    GINDX	parents;
    GINDX	children;
    GINDX	almcounts;
    GINDX	BBArray;
    PNTalmcount fullalmcount;
    uint2	guid[PNT_GUID_SZ/2];
    uint2	subtype;
    uint2	itemnumber;
    uint2	flags;
    PNTNUM	owner;
    uint2	remotetype;
    uint2	spare1;
    uint2	spare2;
    uint2	spare3;
    uint2	entityname[PNT_ENTITYNAME_SZ/2];
    ASH_DAT	dat;
    ASH_EXT	ext;
    ASH_CNT	cnt;
    ASH_DES	des;
    } ASHpoint;

typedef struct
    {
    uint2	size;	/* size of structure (chars) */
    uint2	number;	/* parameter number */
    uint2	flags;	/* flags */
#define		PARAM_FLAG_DELETE		0x0001	/* parameter deleted */
#define		PARAM_FLAG_BITSTRING	0x0002  /* assumed type is bitstring */
#define		PARAM_FLAG_VARIANT_MASK	0x0002  /* mask for valid variant bits */
    uint2	method;	/* data access method */
    uint2	type;	/* data type */
    uint2	vt_type;/* variant data type */
    GINDX	gValue;	/* handle to value structure */
    GINDX	gLimits;/* handle to value limits structures */
    char	name[4];/* parameter name (null terminated) */
    } PNTparam;

typedef struct			/*BB alarm info */
    {
    uint4	BBIndex;	/* basic block index */
    uint2	EECode;		/* execution environment code */
    uint2	low;		/* Active U count for this BB,EE */
    uint2	high;		/* Active H count for this BB,EE */
    uint2	urgent;		/* Active L count for this BB,EE */
    uint2	unack;		/* unacked count for this BB,EE */
    uint2	BBpri;		/* alarm priority for this BB,EE */

/*              5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 */
/*             --------------------------------- */
/*             |         |I|A|U|  pri  | subpri| */
/*	       --------------------------------- */
/*              U - Unacked flag */
#define		BB_UNACK_FLAG	PNTALM_UNACKN_MASK	/* unacked alarms on this BB */
#define		BB_UNACK_BIT	PNTALM_UNACKN_BIT
/*              A - Acked flag */
#define		BB_ACKED_FLAG	PNTALM_ACKED_MASK	/* acked alarms on this BB */
#define		BB_ACKED_BIT	PNTALM_ACKED_BIT
/*              I - In alarm flag */
#define		BB_INALARM_FLAG	PNTALM_INALM_MASK	/* BB in alarm*/
#define		BB_INALARM_BIT	PNTALM_INALM_BIT
/*		Priority */
#define		BB_PRI_MASK	    PNTALM_ALMPRI_MASK
#define		BB_PRI_OFFSET	PNTALM_ALMPRI_BIT
#define		BB_PRI_WIDTH	PNTALM_ALMPRI_SZ
/*		Subpriority */
#define		BB_SUBPRI_MASK	    PNTALM_ALMSUBPRI_MASK
#define		BB_SUBPRI_OFFSET    PNTALM_ALMSUBPRI_BIT
#define		BB_SUBPRI_WIDTH	    PNTALM_ALMSUBPRI_SZ
    } BBalarminfo;

typedef union		/* This structure is used to pass values into and */
    {			/* out of hsc_param_value() */
    GDAVARIANT	var;
    char	text[MAX_PARAM_VALUE_STRING_LEN+1];
    short	int2;
    long	int4;
    int8	int8;
    float	real;
    double	dble;
    struct {
	   long	ord;
	   char	text[MAX_PARAM_VALUE_STRING_LEN+1];
	   } en;
    struct {
	ULONG	cSize;	/* size of serialized variant */
	BYTE	*pData;	/* pointer to serialized variant */
	} servar;
    HSCTIME time;
    } PARvalue;

typedef struct		/* This structure is used internally by the */
    {			/* parameter access routines */
    int		fParam;		/* flags for the parameter */
    int		hPoint;		/* which heap the point is in */
    int		hParam;		/* which heap the parameter is in */
    int		hValue;		/* which heap the value is in */
    int		hItem;		/* which heap the item is in */
    STApoint*	pPoint;		/* point record */
    PNTparam*	pParam;		/* parameter entry */
    DICTvalue*	pValue;		/* parameter value */
    DICTvalue*	pItem;		/* parameter item */
    } PARhandle;

typedef struct		/* This structure is used by  */
    {			/* point_count routines */
    int		Type;		/* point type */
    int		Count;		/* point count for this type */
    } PNTcount;

/*
PROTOTYPES:
*/
EXTERN_C int2		hsc_ispointavailable_history(uint2*);
EXTERN_C int2		hsc_ispointavailable_netapi(uint2*);
EXTERN_C int2		hsc_ispointavailable_odbcdrv(uint2*);
EXTERN_C int2		hsc_ispointavailable_reports(uint2*);
EXTERN_C int2		hsc_ispointavailable_srvscripting(uint2*);
EXTERN_C BOOL 		hsc_ispoint_local	(PNTNUM);
EXTERN_C int CALLBACK	hsc_notification (struct timeb,int,int,char*,char*,char*,char*,char*,char*,char*);
EXTERN_C GINDX*		hsc_points_get		(PNTNUM);
EXTERN_C void 		hsc_points_flush	();
EXTERN_C void 		hsc_point_flush		(PNTNUM);
EXTERN_C STApoint*	hsc_point_access	(PNTNUM);
EXTERN_C int	 	hsc_point_count		(PNTTYP,int2);
EXTERN_C int		hsc_point_count_by_area	(int);
EXTERN_C int		hsc_point_count_by_areaconn(int,int);
EXTERN_C int            hsc_point_count_by_conn (int iConnection);
EXTERN_C int		hsc_point_count_by_type	(PNTcount**);
EXTERN_C PNTNUM 	hsc_number_of_points	();
EXTERN_C PNTNUM 	hsc_point_find_free	();
EXTERN_C PNTNUM 	hsc_point_number	(char*);
EXTERN_C PNTNUM 	hsc_point_number_ex	(char*,int);
EXTERN_C int 		hsc_point_number_ex2	(int,int,char**,PNTNUM*,GDAERR*);
EXTERN_C PNTNUM		hsc_point_number_search	(char*,PNTNUM);
EXTERN_C PNTTYP 	hsc_point_type		(PNTNUM);
EXTERN_C PNTTYP 	hsc_point_type_ex	(STApoint *);
EXTERN_C uint2      hsc_point_type_to_acr (PNTTYP pointtype);
EXTERN_C PNTTYP		hsc_point_remote_type	(PNTNUM);
EXTERN_C PNTTYP		hsc_point_remote_type_ex(STApoint *);
EXTERN_C int2		hsc_point_subtype	(PNTNUM);
EXTERN_C int2       hsc_point_subtype_ex(STApoint *);
EXTERN_C uint2      hsc_point_itemnumber    (PNTNUM);
EXTERN_C uint2      hsc_point_itemnumber_ex (STApoint *);
EXTERN_C int 		hsc_point_name		(PNTNUM,char*,int);
EXTERN_C int		hsc_point_name_change	(PNTNUM, char*);
EXTERN_C int 		hsc_point_area		(PNTNUM);
EXTERN_C int 		hsc_point_area_ex	(STApoint *);
EXTERN_C int        hsc_point_area_change   (PNTNUM, int2);
EXTERN_C int        hsc_point_area_change_ex   (PNTNUM, STApoint *, int2);
EXTERN_C int		hsc_point_areachg_by_conn(int,int2);
EXTERN_C int		hsc_point_guid_change	(PNTNUM, GUID);
EXTERN_C int		hsc_point_desc_change	(PNTNUM, char *);
EXTERN_C int		hsc_point_connection (PNTNUM);
EXTERN_C int		hsc_point_connection_ex (STApoint *);
EXTERN_C int2		hsc_point_pointservertype (PNTNUM);
EXTERN_C int2		hsc_point_pointservertype_ex (STApoint *);
EXTERN_C int		hsc_point_size		(PNTNUM,uint2*,uint2*,uint2*,uint2*);
EXTERN_C int 		hsc_point_read		(PNTNUM,uint2*,uint2*,uint2*,uint2*);
EXTERN_C int 		hsc_point_read2		(PNTNUM,uint2*,uint2*,uint2*,uint2*,PNTTYP*);
EXTERN_C int 		hsc_point_write		(PNTNUM,uint2*,uint2*,uint2*,uint2*);
EXTERN_C PNTNUM		hsc_point_create	(char*,PNTTYP,int2,char*,CREATETIME*,PNTNUM,int2,PNTNUM*,int2,char*,PNTTYP,PNTNUM,GUID*);
EXTERN_C PNTNUM		hsc_point_create_ex	(char*,PNTTYP,int2,char*,CREATETIME*,PNTNUM,int2,PNTNUM*,int2,char*,PNTTYP,PNTNUM,GUID*,BOOL,int);
EXTERN_C int 		hsc_point_delete	(PNTNUM);
EXTERN_C int 		hsc_point_delete_duplicate(PNTNUM);
EXTERN_C int		hsc_point_delete_ex (PNTNUM,BOOL,BOOL,BOOL);
EXTERN_C int		hsc_point_delete_filedata (PNTNUM*,int);
EXTERN_C int		hsc_point_delete_by_areaconn(int,int);
EXTERN_C int            hsc_point_delete_by_conn(int iConneciton);
EXTERN_C int 		hsc_point_delete_external();
EXTERN_C int		hsc_point_delete_lock_query(PNTNUM point, PNTNUM **ppPointsToLock, int *piNumPointsToLock);
EXTERN_C void	 	hsc_point_clear		(PNTNUM);
EXTERN_C void	 	hsc_point_clear_all	();
EXTERN_C void	 	hsc_point_age		(PNTNUM);
EXTERN_C void	 	hsc_point_age_all	();
EXTERN_C void	 	hsc_point_resubscribe	(PNTNUM*,uint4);
EXTERN_C void	 	hsc_point_resubscribe_all();
EXTERN_C void	 	hsc_point_unsubscribe	(PNTNUM*,uint4);
EXTERN_C void	 	hsc_point_unsubscribe_all();
EXTERN_C void	 	hsc_point_validate	(PNTNUM);
EXTERN_C void	 	hsc_point_validate_all ();
EXTERN_C void		hsc_point_validate_all_ex (BOOL);

EXTERN_C int 		hsc_point_alarm_display	(PNTNUM,char*,int);
EXTERN_C int 		hsc_point_assoc_display	(PNTNUM,char*,int);
EXTERN_C int 		hsc_point_detail_display(PNTNUM,char*,int);
EXTERN_C int 		hsc_point_group_display	(PNTNUM,char*,int);

/* do not use hsc_pnttyp_list, use hsc_pnttyp_list_create instead */
EXTERN_C int 		hsc_pnttyp_list_create	(enumlist**);
EXTERN_C int 		hsc_pnttyp_list		(enumlist*,int); /* obsolete */
EXTERN_C int 		hsc_pnttyp_number	(char*);
EXTERN_C int 		hsc_pnttyp_name		(int,char*,int);

EXTERN_C int 		hsc_pntsubtyp_list_create (int,enumlist**);
EXTERN_C int 		hsc_pntsubtyp_number	(int,char*);
EXTERN_C int 		hsc_pntsubtyp_name	(int,int,char*,int);

EXTERN_C PNTparam*	hsc_param_access	(STApoint*,PRMNUM);
EXTERN_C PNTparam*	hsc_param_block_check	(STApoint*,PNTparam*,PRMNUM);
EXTERN_C PNTparam*	hsc_param_find_first	(STApoint*);
EXTERN_C PNTparam*	hsc_param_find_next	(STApoint*,PNTparam**,PNTparam*,PRMNUM);
EXTERN_C PNTparam*	hsc_param_find_by_name	(PNTNUM,char*);
EXTERN_C PNTparam*	hsc_param_find_by_number(PNTNUM,PRMNUM);
EXTERN_C PNTparam*	hsc_param_add		(PNTNUM,PRMNUM,char*,fatDICTitem*,char*);
EXTERN_C DICTitem*	hsc_class_param_find_by_name  (STApoint*,char*);
EXTERN_C DICTitem*	hsc_class_param_find_by_number(STApoint*,PRMNUM);

EXTERN_C int	 	hsc_param_count		();
EXTERN_C PARhandle	hsc_param_handle	(PNTNUM,PRMNUM,int);
EXTERN_C PRMNUM 	hsc_param_create_alt	(PNTNUM,char*,int);
EXTERN_C PRMNUM 	hsc_param_create	(PNTNUM,PRMNUM,char*,fatDICTitem*,char*);
EXTERN_C int		hsc_param_delete	(PNTNUM,PRMNUM);
EXTERN_C int		hsc_param_delete_marked	(PNTNUM);
EXTERN_C int		hsc_param_mark_delete	(PNTNUM,PRMNUM);
EXTERN_C int		hsc_param_unmark_delete	(PNTNUM,PRMNUM);
/* do not use hsc_param_list or hsc_param_list_create, use hsc_param_list_create_limit instead */
EXTERN_C int 		hsc_param_list_create_limit	(PNTNUM,int,enumlist**);
EXTERN_C int 		hsc_param_list_create	(PNTNUM,enumlist**);
EXTERN_C int 		hsc_param_list		(PNTNUM,enumlist*,int); /* obsolete */
EXTERN_C int		hsc_param_list_filter	(enumlist*,int,enumlist**);
EXTERN_C int		hsc_param_list_local	(PNTNUM,PNTTYP,int,enumlist **);
EXTERN_C int		hsc_param_list_merge	(int,int,enumlist *,int,enumlist *,int,enumlist **);
EXTERN_C int        hsc_param_method    (PNTNUM,PRMNUM);
EXTERN_C PRMNUM 	hsc_param_number	(PNTNUM,char*);
EXTERN_C PRMNUM 	hsc_param_number_ex	(PNTNUM*,char*);
EXTERN_C int 		hsc_param_number_ex2	(int,char**,PNTNUM*,PRMNUM*,GDAERR*);
EXTERN_C int 		hsc_param_number_ex3	(int,int,char**,PNTNUM*,PRMNUM*,GDAERR*);
EXTERN_C PRMNUM 	hsc_param_number_local	(PNTNUM*,char*);
EXTERN_C int 		hsc_param_name		(PNTNUM,PRMNUM,char*,int);
EXTERN_C int 		hsc_param_name_next	(PNTNUM,PRMNUM,char*,int);
EXTERN_C int 		hsc_param_source	(PNTNUM,PRMNUM,char*,int);
EXTERN_C int 		hsc_param_type		(PNTNUM,PRMNUM);
EXTERN_C int 		hsc_param_type_ex	(PNTNUM,PRMNUM,int);
EXTERN_C int 		hsc_param_format	(PNTNUM,PRMNUM);
EXTERN_C int		hsc_param_id_item	(PNTNUM,PRMNUM,int,PNTNUM*,PRMNUM*);
EXTERN_C int		hsc_param_id_limits	(PNTNUM,PRMNUM,PNTNUM*,PRMNUM*,PNTNUM*,PRMNUM*);
EXTERN_C int		hsc_param_id_range	(PNTNUM,PRMNUM,PNTNUM*,PRMNUM*,PNTNUM*,PRMNUM*);
EXTERN_C int		hsc_param_limits	(PNTNUM,PRMNUM,double*,double*);
EXTERN_C int		hsc_param_limits_local	(PNTNUM,PRMNUM,double*,double*);
EXTERN_C int		hsc_param_limits_ex	(int4,int,PNTNUM*,PRMNUM*,double*,double*,GDAERR*);
EXTERN_C int		hsc_param_proc		(PNTNUM,PRMNUM,int2,int2,double,double,int2,int2*,double*);
EXTERN_C int		hsc_param_range		(PNTNUM,PRMNUM,double*,double*);
EXTERN_C int		hsc_param_units		(int, PNTNUM*, PRMNUM*, char ***,GDAERR**,int4);
EXTERN_C int		hsc_param_range_local	(PNTNUM,PRMNUM,double*,double*);
EXTERN_C int		hsc_param_range_ex	(int4,int,PNTNUM*,PRMNUM*,double*,double*,GDAERR*);
EXTERN_C int		hsc_param_raise		(PNTNUM,PRMNUM,int,double,PARvalue*,uint2*);
EXTERN_C int 		hsc_param_value		(PNTNUM,PRMNUM,int,PARvalue*,uint2*);
EXTERN_C int 		hsc_param_value_sec	(PNTNUM,PRMNUM,int,PARvalue*,uint2*,GDASECURITY*);
EXTERN_C int 		hsc_param_values	(int,int,PNTNUM*,PRMNUM*,int*,PARvalue*,uint2*,int*);
EXTERN_C int 		hsc_param_values_get	(int,PNTNUM*,PRMNUM*,int*,PARvalue*,uint2*,GDAERR*);
EXTERN_C int 		hsc_param_values_put	(int,PNTNUM*,PRMNUM*,int*,PARvalue*,uint2*,GDAERR*,GDASECURITY*);
EXTERN_C int 		hsc_param_values_put_ex	(int,int,PNTNUM*,PRMNUM*,int*,PARvalue*,uint2*,GDAERR*,GDASECURITY*);
EXTERN_C int 		hsc_param_value_put	(PNTNUM,PRMNUM,int,PARvalue*,uint2*);
EXTERN_C int 		hsc_param_value_put_priority	(PNTNUM,PRMNUM,int,PARvalue*,uint2*,int,int);
EXTERN_C int 		hsc_param_value_save	(PNTNUM,PRMNUM,int,PARvalue*,uint2*);
EXTERN_C int 		hsc_param_value_put_sec	(PNTNUM,PRMNUM,int,PARvalue*,uint2*,GDASECURITY*);
EXTERN_C int 		hsc_param_value_of_type	(PNTNUM,PRMNUM,int,PARvalue*,uint2*);
EXTERN_C int 		hsc_param_values_get_of_type(int,uint2,PNTNUM*,PRMNUM*,int*,uint2*,GDAERR*);
EXTERN_C int 		hsc_param_value_of_type_unmodified_error(PNTNUM,PRMNUM,int,PARvalue*,uint2*,GDAERR*);
EXTERN_C int 		hsc_param_limit_min	(PNTNUM,PRMNUM,int,PARvalue*,uint2*);
EXTERN_C int 		hsc_param_limit_max	(PNTNUM,PRMNUM,int,PARvalue*,uint2*);
EXTERN_C int 		hsc_param_range_min	(PNTNUM,PRMNUM,int,PARvalue*,uint2*);
EXTERN_C int 		hsc_param_range_max	(PNTNUM,PRMNUM,int,PARvalue*,uint2*);
/* do not use hsc_param_enum_list, use hsc_param_enum_list_create instead */
EXTERN_C int 		hsc_param_enum_list_create (PNTNUM,PRMNUM,enumlist**);	/* obsolete */
EXTERN_C int 		hsc_param_enum_list	(PNTNUM,PRMNUM,enumlist*,int);		/* obsolete */
EXTERN_C char* 		hsc_param_enum_string	(PNTNUM,PRMNUM,int4);
EXTERN_C int 		hsc_param_enum_ordinal	(PNTNUM,PRMNUM,char*);
EXTERN_C int	 	hsc_param_acquire	(int,PNTNUM*,PRMNUM*,int);
EXTERN_C int	 	hsc_param_subscribe	(int,PNTNUM*,PRMNUM*,int);
EXTERN_C int	 	hsc_param_resubscribe	(int,PNTNUM*,PRMNUM*,int);
EXTERN_C int	 	hsc_param_unsubscribe	(int,PNTNUM*,PRMNUM*,int);
EXTERN_C int	 	hsc_param_trigger	(int,PNTNUM*,PRMNUM*,int);
EXTERN_C void		_hsc_param_delete_	(STApoint*,PNTparam*);
EXTERN_C int	 	hsc_param_flush		(PNTNUM,PRMNUM);
EXTERN_C int 		hsc_param_flags		(PNTNUM,PRMNUM);
EXTERN_C int		hsc_param_add_rangelimits   (PNTNUM, PRMNUM, DICTvalue, DICTvalue, DICTvalue, DICTvalue);
EXTERN_C BOOL		hsc_param_is_instance_param(PNTNUM, PRMNUM);

EXTERN_C int 		hsc_enumlist_destroy	(enumlist**);
EXTERN_C int		hsc_enumlist_copy	(enumlist*,enumlist*,int);

EXTERN_C int		hsc_stringlist_midl_destroy	(int,char***);

EXTERN_C uint2*         hsc_decode_prmdef_ex	(STApoint* pPoint, uint2 prmdef);  // internal use only

EXTERN_C int		hsc_geti2	(PNTNUM     point, uint2 prmdef, uint2 *value);
EXTERN_C int            hsc_geti2_ex	(STApoint *pPoint, uint2 prmdef, uint2 *value);
EXTERN_C int		hsc_geti4	(PNTNUM     point, uint2 prmdef, int4  *value);
EXTERN_C int		hsc_geti4_ex	(STApoint *pPoint, uint2 prmdef, int4  *value);
EXTERN_C int		hsc_getrl	(PNTNUM     point, uint2 prmdef, float *value);
EXTERN_C int		hsc_getrl_ex	(STApoint *pPoint, uint2 prmdef, float *value);
EXTERN_C int		hsc_getr8	(PNTNUM     point, uint2 prmdef, double*value);
EXTERN_C int		hsc_getr8_ex	(STApoint *pPoint, uint2 prmdef, double*value);
EXTERN_C int		hsc_getch	(PNTNUM     point, uint2 prmdef, char  *value, int sizeofvalue);
EXTERN_C int		hsc_getch_ex	(STApoint *pPoint, uint2 prmdef, char  *value, int sizeofvalue);

EXTERN_C int		hsc_puti2	(PNTNUM     point, uint2 prmdef, uint2 value);
EXTERN_C int            hsc_puti2_ex	(STApoint *pPoint, uint2 prmdef, uint2 value);
EXTERN_C int		hsc_puti4	(PNTNUM     point, uint2 prmdef, int4  value);
EXTERN_C int		hsc_puti4_ex	(STApoint *pPoint, uint2 prmdef, int4  value);
EXTERN_C int		hsc_putrl	(PNTNUM     point, uint2 prmdef, float value);
EXTERN_C int		hsc_putrl_ex	(STApoint *pPoint, uint2 prmdef, float value);
EXTERN_C int		hsc_putr8	(PNTNUM     point, uint2 prmdef, double value);
EXTERN_C int		hsc_putr8_ex	(STApoint *pPoint, uint2 prmdef, double value);
EXTERN_C int		hsc_putch	(PNTNUM     point, uint2 prmdef, char*  value, int sizeofvalue);
EXTERN_C int		hsc_putch_ex	(STApoint *pPoint, uint2 prmdef, char*  value, int sizeofvalue);

EXTERN_C int		hsc_geti2_struct	(PNTNUM     point, uint2 location, uint2 *value);
EXTERN_C int		hsc_geti2_struct_ex	(STApoint *pPoint, uint2 location, uint2 *value);
EXTERN_C int		hsc_getbits_struct	(PNTNUM     point, uint2 location, uint2 *value, uint2 bitmask);
EXTERN_C int		hsc_getbits_struct_ex	(STApoint *pPoint, uint2 location, uint2 *value, uint2 bitmask);
EXTERN_C int		hsc_getch_struct	(PNTNUM     point, uint2 location, uint2 length, char *value, int sizeofvalue);
EXTERN_C int		hsc_getch_struct_ex	(STApoint *pPoint, uint2 location, uint2 length, char *value, int sizeofvalue);
EXTERN_C int		hsc_getbytes_struct	(PNTNUM     point, uint2 location, uint2 length, BYTE *value);
EXTERN_C int		hsc_getbytes_struct_ex	(STApoint *pPoint, uint2 location, uint2 length, BYTE *value);

EXTERN_C int		hsc_puti2_struct	(PNTNUM     point, uint2 location, uint2 value);
EXTERN_C int		hsc_puti2_struct_ex	(STApoint *pPoint, uint2 location, uint2 value);
EXTERN_C int		hsc_putbits_struct	(PNTNUM     point, uint2 location, uint2 value, uint2 bitmask);
EXTERN_C int		hsc_putbits_struct_ex	(STApoint *pPoint, uint2 location, uint2 value, uint2 bitmask);
EXTERN_C int		hsc_putch_struct	(PNTNUM     point, uint2 location, uint2 length, char *value, int sizeofvalue);
EXTERN_C int		hsc_putch_struct_ex	(STApoint *pPoint, uint2 location, uint2 length, char *value, int sizeofvalue);
EXTERN_C int		hsc_putbytes_struct	(PNTNUM     point, uint2 location, uint2 length, BYTE *value);
EXTERN_C int		hsc_putbytes_struct_ex	(STApoint *pPoint, uint2 location, uint2 length, BYTE *value);

EXTERN_C int		getlst(int,float*,int2*);
EXTERN_C int		givlst(int,float*,int2*);

EXTERN_C int		hsc_tagtyp_find (int);
EXTERN_C int4 STDCALL	POINTTRA (int4*,int4*,int4*,PNTNUM*,PRMNUM*);
EXTERN_C int4 STDCALL	REATAGTYP(int4*,int4*,int2*,int2*,int4*);
EXTERN_C int		hsc_prmdef2param (int4,int4,PNTNUM,int2);
EXTERN_C int		hsc_param2name (int4,int4,PNTNUM,PRMNUM,char*,int);

EXTERN_C void		hsc_rtn_points(uint2,uint2,uint2,uint2);
EXTERN_C void		hsc_rtn_point(PNTNUM);

EXTERN_C int		PARvalueConvert(PARvalue*,DATAtype,PARvalue*,DATAtype);
EXTERN_C int		GDAITEMConvert(GDAITEM*,DATAtype,GDAITEM*);
EXTERN_C void		CtoFPARvalue(PARvalue*,DATAtype,int2*,PARvalue*);
EXTERN_C void		FtoCPARvalue(PARvalue*,DATAtype,int,PARvalue*);
EXTERN_C int		GDAtoPARvalue(GDAITEM*,PARvalue*,uint2*);
EXTERN_C GDAITEM*	PARtoGDAvalue(PARvalue*,DATAtype);
EXTERN_C PARvalue*	ClearPARvalue(PARvalue*,DATAtype);
EXTERN_C PARvalue*	InitializePARvalue(PARvalue*);
// Implementation for point server is not EXTERN_C
#ifdef POINT_SERVER
GDAITEM*	DestroyGDAvalue(GDAITEM*);
GDAITEM*	ClearGDAvalue(GDAITEM*);
GDAITEM*	InitializeGDAvalue(GDAITEM*);
GDAITEM*	CopyGDAvalue(GDAITEM*,GDAITEM*);
#else
EXTERN_C GDAITEM*	DestroyGDAvalue(GDAITEM*);
EXTERN_C GDAITEM*	ClearGDAvalue(GDAITEM*);
EXTERN_C GDAITEM*	InitializeGDAvalue(GDAITEM*);
EXTERN_C GDAITEM*	CopyGDAvalue(GDAITEM*,GDAITEM*);
#endif
EXTERN_C int		PARvaluetoDble(PARvalue*,DATAtype,double*);
EXTERN_C int		PARvaluetoReal(PARvalue*,DATAtype,float*);
EXTERN_C int		PARvaluetoInt4(PARvalue*,DATAtype,int4*);
EXTERN_C int		PARvaluetoInt2(PARvalue*,DATAtype,int2*);
EXTERN_C HRESULT	SerializeVariantData(GDAVARIANT,char**,int*);
EXTERN_C HRESULT	UnSerializeVariantData(GDAVARIANT*,char*);
EXTERN_C int		hsc_variant_to_PARvalue(GDAVARIANT*,PARvalue*,uint2*,int);
EXTERN_C int		hsc_param_type_convert(GDAVARIANT*);
EXTERN_C int		hsc_param_is_variant(PNTNUM point, PRMNUM param);
EXTERN_C int		hsc_param_is_variant_bitstring(GDAVARIANT *);
EXTERN_C int		hsc_param_is_variant_exposed(PNTNUM point, PRMNUM param, int offset);
EXTERN_C int		hsc_param_variant_type(PNTNUM, PRMNUM, VARTYPE*);
EXTERN_C int		hsc_param_variant_set_type(PNTNUM, PRMNUM, VARTYPE, uint2);
EXTERN_C int		hsc_param_is_array (PNTNUM,PRMNUM);

EXTERN_C int		hsc_security_set(GDASECURITY*);
EXTERN_C int            hsc_security_set_ex(int,GDASECURITY*);
EXTERN_C void 		hsc_security_destroy(GDASECURITY*);
EXTERN_C int		hsc_variant_to_gdaitem(VARIANT *pVar, GDAITEM *pItem);

EXTERN_C BOOL 		hsc_ispoint_replicable(PNTNUM);
EXTERN_C BOOL 		hsc_ispoint_replicable_ex(STApoint *);
EXTERN_C BOOL 		hsc_istype_replicable(PNTTYP);
EXTERN_C BOOL 		hsc_isparam_replicable(PNTNUM, PRMNUM, PNTTYP, uint2);

/*BB alarming functions*/
EXTERN_C int2		hsc_BBAlarmUpdate(struct almdir*,BOOL,BOOL,BOOL);
EXTERN_C int2		hsc_getAlarmState(PNTNUM,PRMNUM,uint2*,uint2*,uint2*,uint2,uint4,int);
EXTERN_C int2		hsc_setAlarmState(PNTNUM,uint2,uint2,uint2,uint4,BOOL,BOOL);
EXTERN_C int2		hsc_get_EE_and_BB(PNTNUM,PRMNUM,uint2*,uint4*);
EXTERN_C void		hsc_point_BBArray_clear_all();

/* aggregate alarming functions */
EXTERN_C int		hsc_almcount_alarm	(int, struct almdir*, struct almevtfil*);
EXTERN_C int		hsc_almcount_alarmchg	(struct almdir*, struct almevtfil*, uint2, uint2);
EXTERN_C int		hsc_almcount_message	(int, struct msgdir*, struct almevtfil*);
EXTERN_C int		hsc_almcount_messagechg	(struct msgdir*, struct almevtfil*, uint2);
EXTERN_C int		hsc_almcount_clear	(PNTNUM);
EXTERN_C int		hsc_almcount_refresh	(PNTNUM);
EXTERN_C int		hsc_almcount_clean_point(PNTNUM, int);

EXTERN_C int		hsc_almcount_find(PNTNUM, int, int, PNTalmcount*);
EXTERN_C int		hsc_almcount_clean_station(int);
EXTERN_C int		hsc_almcount_validate_settings();
EXTERN_C int		hsc_almcount_recalculate();

/* Enterprise Model functions */
EXTERN_C void		hsc_em_FreePointList(PNTNUM *pPointList);
EXTERN_C int		hsc_em_GetRootAssets(int *pCount, PNTNUM **ppPointList);
EXTERN_C int		hsc_em_GetRootAlarmGroups(int *pCount, PNTNUM **ppPointList);
EXTERN_C int		hsc_em_GetModelPoints(int *pCount, PNTNUM **ppPointList);
EXTERN_C PNTNUM		hsc_em_GetSysPoint(uint2, uint2);
EXTERN_C int		hsc_em_GetServerTagName(char *szAlias, char *szTagName, int TagNameLen);
EXTERN_C int		hsc_em_GetSysPntTagName(uint2, uint2, char*, int);
EXTERN_C int		hsc_em_GetSysPntDescription(uint2, uint2, char*, int);
EXTERN_C int		hsc_em_GetSysPntEntityName(uint2, uint2, char*, int);
EXTERN_C int		hsc_em_GetRootEntities(int *pCount, PNTNUM **ppPointList);
EXTERN_C int		hsc_em_GetDSASystemName(char *name, int namelen);
EXTERN_C int		hsc_em_SetDSASystemName(char *name);
EXTERN_C void		hsc_em_GetLastPointChangeTime(HSCTIME *pTime);
EXTERN_C void		hsc_em_GetLastAssetLoadTime(HSCTIME *pTime);
EXTERN_C void		hsc_em_GetLastAlarmGroupLoadTime(HSCTIME *pTime);
EXTERN_C void		hsc_em_GetLastSystemConfigLoadTime(HSCTIME *pTime);
EXTERN_C int		hsc_em_GetModelName(uint2 model, char *name, int namelen);
EXTERN_C int		hsc_em_SetModelName(uint2 model, char *name);
EXTERN_C PNTNUM		hsc_em_ResolveGUIDtoPoint(GUID *pGUID);
EXTERN_C int		hsc_em_ResolveGUIDtoTag(GUID *pGUID,struct tagfl *pTagRec);
EXTERN_C int		hsc_em_ResolveName(char*,int,struct tagfl*);
EXTERN_C int		hsc_em_ResolveFullName(char*,struct tagfl*);
EXTERN_C int		hsc_em_ResolveTagName(char*,int,struct tagfl*);
EXTERN_C int		hsc_em_ResolveTagNameEx(char*,int,PNTNUM,struct tagfl*);
EXTERN_C int		hsc_em_FindTagNameMatch(char *szName, struct tagfl *pRecord);
EXTERN_C int		hsc_em_FindNameMatch(char *szName, struct tagfl *pRecord);
EXTERN_C int		hsc_em_GetSimpleName(char *szName, char *szSimpleName, int iSimpleNameLen);
EXTERN_C int		hsc_em_TagNameSplit(char *szTagName, char *szOwner, int iOwnerLen, char *szBaseName, int iBaseNameLen);
EXTERN_C PNTNUM		hsc_em_GetDefaultOwnerEx(PNTTYP type, int2 subtype, int2 gdacon);
EXTERN_C PNTNUM		hsc_em_GetDefaultOwner(int gdacon);
EXTERN_C PNTNUM		hsc_em_GetOwnerPoint(char *szOwner);
EXTERN_C int		hsc_em_GetOwnerString(PNTNUM owner, char *szOwner, int iStrLen);
EXTERN_C BOOL		hsc_em_CheckTagNameClash(char *szTagName, PNTNUM owner, int iTagType, int iTagNum, int *piClashingTagNum);
EXTERN_C BOOL		hsc_em_CheckGUIDClash(GUID *pGUID, int iTagType, int iTagNum, int *piClashingTagType, int *piClashingTagNum);
EXTERN_C BOOL		hsc_em_CheckEntityNameClash(char *szName, PNTNUM parent, int iTagType, int iTagNum, int *piClashingTagType, int *piClashingTagNum);
EXTERN_C int		hsc_GUIDFromString(char *szGUID, GUID *pGUID);
EXTERN_C int		hsc_StringFromGUID(GUID *pGUID,	char *pszGUID);
EXTERN_C int		hsc_em_SetAlarmDisabled(PNTNUM, int2, BOOL);
EXTERN_C BOOL		hsc_em_GetAlarmDisabledMatch(PNTNUM);
EXTERN_C int		hsc_em_SetSignatureDisabled(PNTNUM, int2, BOOL);
EXTERN_C BOOL		hsc_em_GetSignatureDisabledMatch(PNTNUM);

EXTERN_C int		hsc_em_FlattenUnassignedItems();

EXTERN_C int		hsc_point_name_ex(PNTNUM point, char *name, int namelen, int flags);
EXTERN_C int		hsc_point_entityname(PNTNUM point, char *name, int namelen);
EXTERN_C int		hsc_point_fullname(PNTNUM point, char *name, int namelen);
EXTERN_C int		hsc_point_location_tagname(PNTNUM point, char *name, int namelen);
EXTERN_C int		hsc_point_location_fullname(PNTNUM point, char *name, int namelen);
EXTERN_C int		hsc_point_entityname_change(PNTNUM point, char *name);
EXTERN_C int		hsc_point_owner_change(PNTNUM point, PNTNUM owner);
EXTERN_C int		hsc_point_rename(PNTNUM point);
EXTERN_C int		hsc_point_guid(PNTNUM point, GUID *pGUID);

EXTERN_C void		hsc_point_freeRelTypList(PNTRELTYP *pRelTypList);
EXTERN_C void		hsc_point_freeRelDirList(PNTRELDIR *pRelDirList);

EXTERN_C int		hsc_point_update_relatives_ex(PNTNUM point, PNTRELTYP reltyp, PNTRELDIR reldir, PNTNUM *pRelatives, int iNumRelatives, DWORD dwFlags, PNTNUM **ppPointsToLock, int *piNumPointsToLock);
EXTERN_C int		hsc_point_update_relatives(PNTNUM point, PNTRELTYP reltyp, PNTRELDIR reldir, PNTNUM *pRelatives, int iNumRelatives, BOOL checked, BOOL updatecounts, PNTNUM **ppPointsToLock, int *piNumPointsToLock);
EXTERN_C int		hsc_point_update_repl_relatives(PNTNUM point, PNTNUM *pRelatives, PNTRELTYP *pRelTypes, PNTRELDIR *pRelDirs, int iNumRelatives, PNTNUM **ppPointsToLock, int *piNumPointsToLock);
EXTERN_C int		hsc_point_add_containment_relation(PNTNUM child, PNTNUM parent);
EXTERN_C int		hsc_point_add_reference_relation(PNTNUM refitem, PNTNUM referer);
EXTERN_C int		hsc_point_add_relation(PNTNUM, PNTNUM, BOOL, BOOL);
EXTERN_C int		hsc_point_add_relations(PNTNUM, PNTRELDIR, int, PNTNUM*, BOOL, BOOL);
EXTERN_C int		hsc_point_add_relations_ex(PNTNUM, PNTRELDIR, int, PNTNUM*, DWORD);
EXTERN_C int            hsc_point_mod_parent(PNTNUM child, PNTNUM oldparent, PNTNUM newparent);

EXTERN_C int		hsc_point_check_proposed_relation(PNTNUM, PNTNUM, PNTNUM);
EXTERN_C int		hsc_point_check_proposed_relations(PNTNUM, PNTRELDIR, int, PNTNUM*);
EXTERN_C int		hsc_point_check_num_proposed_relations(PNTRELDIR reldir, int count);
EXTERN_C int		hsc_point_check_num_proposed_relations_ex(PNTNUM point, PNTRELDIR reldir, int iNumPropsedRelatives, PNTNUM *pProposedRelatives);
EXTERN_C int		hsc_point_check_existing_relations(PNTNUM point);
EXTERN_C int		hsc_point_check_num_existing_relations(PNTNUM point);
EXTERN_C int		hsc_check_cyclic_relations();
EXTERN_C int		hsc_point_check_cyclic_relations(PNTNUM point);
EXTERN_C int		hsc_point_check_links();
EXTERN_C int		hsc_point_check_parentlinks(PNTNUM point);
EXTERN_C int		hsc_point_check_childlinks(PNTNUM point);
EXTERN_C int		hsc_point_check_entityname(PNTNUM point);

EXTERN_C int		hsc_point_del_invalid_parents(PNTNUM point);
EXTERN_C int		hsc_point_del_invalid_children(PNTNUM point);
EXTERN_C int		hsc_point_del_containment_relation(PNTNUM child, PNTNUM parent, BOOL);
EXTERN_C int		hsc_point_del_reference_relation(PNTNUM refitem, PNTNUM referer, BOOL);
EXTERN_C int		hsc_point_del_relation(PNTNUM, PNTNUM, BOOL, BOOL);
EXTERN_C int		hsc_point_del_relations_ex(PNTNUM, PNTRELDIR, int, PNTNUM*, DWORD);
EXTERN_C int		hsc_point_del_relations(PNTNUM, PNTRELDIR, int, PNTNUM*, BOOL, BOOL);
EXTERN_C int		hsc_point_del_all_relations(PNTNUM, BOOL, BOOL);

EXTERN_C int		hsc_point_get_parents(PNTNUM, int*, PNTNUM**);
EXTERN_C int		hsc_point_get_parent(PNTNUM, int, PNTNUM*);
EXTERN_C int		hsc_point_get_num_parents(PNTNUM, int*);
EXTERN_C int            hsc_point_get_ancestors(PNTNUM point, int *piNumAncestors, PNTNUM **ppAncestors);
EXTERN_C int		hsc_point_get_children(PNTNUM, int*, PNTNUM**);
EXTERN_C int		hsc_point_get_child(PNTNUM, int, PNTNUM*);
EXTERN_C int		hsc_point_get_num_children(PNTNUM, int*);

EXTERN_C int		hsc_point_get_containment_parents(PNTNUM child, int *piNumParents, PNTNUM **ppParents);
EXTERN_C int		hsc_point_get_containment_parent(PNTNUM child, int offset, PNTNUM *pParent);
EXTERN_C int		hsc_point_get_num_containment_parents(PNTNUM child, int *piNumParents);
EXTERN_C int		hsc_point_get_containment_children(PNTNUM parent, int *piNumChildren, PNTNUM **ppChildren);
EXTERN_C int		hsc_point_get_containment_child(PNTNUM parent, int offset, PNTNUM *pChild);
EXTERN_C int		hsc_point_get_containment_child_byname(PNTNUM point, char *szChildName, PNTNUM *pChild);
EXTERN_C int		hsc_point_get_num_containment_children(PNTNUM parent, int *piNumChildren);
EXTERN_C int		hsc_point_get_containment_ancestors(PNTNUM point, int *piNumAncestors, PNTNUM **ppAncestors);
EXTERN_C int		hsc_point_get_containment_ancestor(PNTNUM point, int offset, PNTNUM *pAncestor);
EXTERN_C int		hsc_point_get_num_containment_ancestors(PNTNUM point, int *piNumAncestors);
EXTERN_C int		hsc_point_get_containment_descendents(PNTNUM point, int *piNumDescendents, PNTNUM **ppDescendents);
EXTERN_C int		hsc_point_get_containment_descendent(PNTNUM point, int offset, PNTNUM *pDescendent);
EXTERN_C int		hsc_point_get_num_containment_descendents(PNTNUM point, int *piNumDescendents);

EXTERN_C int		hsc_point_get_references(PNTNUM point, int *piNumRefItems, PNTNUM **ppRefItems);
EXTERN_C int		hsc_point_get_reference(PNTNUM point, int offset, PNTNUM *pRefItem);
EXTERN_C int		hsc_point_get_num_references(PNTNUM point, int *piNumRefItems);
EXTERN_C int		hsc_point_get_referers(PNTNUM point, int *piNumRefItems, PNTNUM **ppRefItems);
EXTERN_C int		hsc_point_get_referer(PNTNUM point, int offset, PNTNUM *pRefItem);
EXTERN_C int		hsc_point_get_num_referers(PNTNUM point, int *piNumRefItems);

EXTERN_C int		hsc_asset_get_parents(PNTNUM point, int *piNumParents, PNTNUM **ppParents);
EXTERN_C int		hsc_asset_get_parent(PNTNUM point, int offset, PNTNUM *pParent);
EXTERN_C int		hsc_asset_get_num_parents(PNTNUM point, int *piNumParents);
EXTERN_C int		hsc_asset_get_children(PNTNUM point, int *piNumChildren, PNTNUM **ppChildren);
EXTERN_C int		hsc_asset_get_child(PNTNUM point, int offset, PNTNUM *pChild);
EXTERN_C int		hsc_asset_get_num_children(PNTNUM point, int *piNumChildren);
EXTERN_C int		hsc_asset_get_ancestors(PNTNUM point, int *piNumAncestors, PNTNUM **ppAncestors);
EXTERN_C int		hsc_asset_get_ancestor(PNTNUM point, int offset, PNTNUM *pAncestor);
EXTERN_C int		hsc_asset_get_num_ancestors(PNTNUM point, int *piNumAncestors);
EXTERN_C int		hsc_asset_get_descendents(PNTNUM point, int *piNumDescendents, PNTNUM **ppDescendents);
EXTERN_C int		hsc_asset_get_descendent(PNTNUM point, int offset, PNTNUM *pDescendent);
EXTERN_C int		hsc_asset_get_num_descendents(PNTNUM point, int *piNumDescendents);

EXTERN_C int		hsc_point_get_relatives(PNTNUM point, PNTRELTYP reltyp, PNTRELDIR reldir, PNTNUM **ppRelatives, int *piNumRelatives);
EXTERN_C int		hsc_point_get_repl_relatives(PNTNUM point, PNTNUM **ppRelatives, PNTRELTYP **ppRelTypes, PNTRELDIR **ppRelDirs, int *piNumRelatives);
EXTERN_C int2		hsc_point_get_relation_type(PNTNUM child, PNTNUM parent);
EXTERN_C int2		hsc_point_get_relation_type_ex(STApoint *pChild, STApoint *pParent);
EXTERN_C int		hsc_point_get_primary_parent(PNTNUM child, PNTNUM *pParent);
EXTERN_C int        hsc_point_get_descendents(PNTNUM point, int *piNumDescendents, PNTNUM **ppDescendents);
EXTERN_C int		hsc_point_get_areaenabled_descendents(PNTNUM point, int *piNumDescendents, PNTNUM **ppDescendents);
EXTERN_C int		hsc_point_get_areascoped_descendents(PNTNUM point, int *piNumDescendents, PNTNUM **ppDescendents);
EXTERN_C BOOL		hsc_point_relation_is_replicable(PNTNUM child, PNTNUM parent);
EXTERN_C BOOL		hsc_point_relation_is_replicable_ex(STApoint *pChild, STApoint *pParent);
EXTERN_C BOOL		hsc_point_is_descendent(PNTNUM point1, PNTNUM point2, int depth);
EXTERN_C BOOL		hsc_point_is_ancestor(PNTNUM point1, PNTNUM point2, int depth);
EXTERN_C int		hsc_point_sort_relations(PNTNUM point);
EXTERN_C int		hsc_point_rebuild_children();
EXTERN_C PNTNUM		hsc_point_owner(PNTNUM point);
EXTERN_C int            hsc_point_get_owned_points(PNTNUM owner, PNTNUM **ppOwnedPoints, int *piNumOwnedPoints);

EXTERN_C int		hsc_point_get_assoc_asset(PNTNUM point, PNTNUM *asset);
EXTERN_C int        hsc_point_set_assoc_asset(PNTNUM point, PNTNUM asset);
EXTERN_C int        hsc_point_set_assoc_asset_ex(PNTNUM point, STApoint *pPoint, PNTNUM asset, STApoint *pAsset);
EXTERN_C int		hsc_point_get_parent_asset(PNTNUM point, PNTNUM *parent);
EXTERN_C int		hsc_point_set_parent_asset(PNTNUM point, PNTNUM parent);
EXTERN_C int		hsc_point_set_area_enabled(PNTNUM point, BOOL areaenabled);
EXTERN_C int		hsc_point_set_area_enabled_ex(PNTNUM point, BOOL areaenabled, int area);
EXTERN_C int		hsc_point_create_area(PNTNUM point, int area);
EXTERN_C BOOL		hsc_point_is_area_enabled(PNTNUM point);
EXTERN_C BOOL		hsc_point_is_area_enabled_ex(STApoint *pPoint);
EXTERN_C int		hsc_clear_area(int iAreaNum);
EXTERN_C int		hsc_clear_areas(int2* areaNums, int aCount);
EXTERN_C BOOL		hsc_point_has_duplicates(PNTNUM point);
EXTERN_C BOOL		hsc_point_has_assigned_descendents(PNTNUM point, int2 crtnum);
EXTERN_C BOOL		hsc_point_is_unassigned_item(PNTNUM point);
EXTERN_C int		hsc_point_gettaginfo(PNTNUM point, struct tagfl *pTagRec);
EXTERN_C int		hsc_point_updatetag(PNTNUM point);
EXTERN_C int		hsc_update_childpoint_areas(PNTNUM point, int2 area);
EXTERN_C BOOL		hsc_has_assoc_asset(PNTNUM point);


EXTERN_C int		hsc_create_model_point(uint2 model, int area);
EXTERN_C int		hsc_create_unassigned_items_point(int area);
EXTERN_C int		hsc_create_collection_point(uint2 collection);
EXTERN_C int		hsc_update_rawitem_point(int gdarec, char *szNewAlias, char *szNewSrvAlias, BOOL bRenameClashes);
EXTERN_C int		hsc_update_cstn_localsrv_point(char *szName);
EXTERN_C int		hsc_update_server_point(int, char*,BOOL);
EXTERN_C int		hsc_update_sysinterface_point(int, char*);
EXTERN_C int		hsc_update_gda_point(int, char*,BOOL);
EXTERN_C int		hsc_get_gda_point(int);
EXTERN_C int		hsc_check_clusterserver_pntnum();
EXTERN_C int		hsc_update_channel_point(int, char*);
EXTERN_C int		hsc_check_channel_point(int, char*);
EXTERN_C int		hsc_update_controller_point(int, char*);
EXTERN_C int		hsc_check_controller_point(int, char*, int);
EXTERN_C int		hsc_update_flexstation_point(int, char*);
EXTERN_C int		hsc_update_consolestation_point(int, char*);
EXTERN_C int		hsc_update_printer_point(int, char*);
EXTERN_C int		hsc_update_console_point(int, char*);
EXTERN_C int		hsc_update_cstn_client_point(int, int);
EXTERN_C int		hsc_update_cdaconstatus_point();
EXTERN_C int		hsc_rename_sysinterface_point(PNTNUM point);

EXTERN_C PNTNUM		hsc_get_areaenabled_point(int2);
EXTERN_C PNTNUM		hsc_get_channel_point(char *szName);
EXTERN_C PNTNUM		hsc_get_controller_point(char *szName);
EXTERN_C PNTNUM		hsc_get_localsrv_point();
EXTERN_C PNTNUM		hsc_get_localsys_point();
EXTERN_C PNTNUM		hsc_get_unassigned_items_point();
EXTERN_C int		hsc_point_move_to_unassigned_items(PNTNUM point);
EXTERN_C int		hsc_point_move_to_unassigned_items_ex(PNTNUM point, PNTNUM **ppPointsToLock, int *piNumPointsToLock);
EXTERN_C int		hsc_point_move_children_to_unassigned_items(PNTNUM point, BOOL bUnsubscribeFlag, BOOL bUpdateCount, BOOL bDelAlmMsg);
EXTERN_C int		hsc_point_move_children_to_unassigned_items_ex(PNTNUM point, BOOL bUnsubscribeFlag, BOOL bUpdateCount, BOOL bDelAlmMsg, PNTNUM **ppPointsToLock, int *piNumPointsToLock);
EXTERN_C PNTNUM		hsc_getpointnumber(int2, int2);
EXTERN_C int		hsc_getcorrespondingrecord(PNTNUM nPoint, int2* pFileNum, int4* pRecNum);
EXTERN_C int		hsc_getSYSPointRecord(STApoint* pPoint);
EXTERN_C BOOL       hsc_point_is_system_entity(PNTNUM point);
EXTERN_C BOOL       hsc_point_is_system_entity_ex(STApoint* pPoint);
EXTERN_C BOOL       hsc_point_is_network_entity(PNTNUM point);
EXTERN_C BOOL       hsc_point_is_network_entity_ex(STApoint* pPoint);
EXTERN_C int		hsc_ispoint_real(PNTNUM PointNumber);
EXTERN_C BOOL		hsc_ispoint_root(PNTNUM point);
EXTERN_C BOOL		hsc_ispoint_owner(PNTNUM point);
EXTERN_C BOOL		hsc_ispoint_localsrv(PNTNUM point);
EXTERN_C BOOL		hsc_ispoint_localsys(PNTNUM point);
EXTERN_C BOOL		hsc_ispoint_rawitem(PNTNUM point);
EXTERN_C BOOL		hsc_is_container (STApoint* pPoint);

EXTERN_C int		hsc_build_system_points(void);
EXTERN_C int		hsc_build_model_points(void);
EXTERN_C int		hsc_build_model_points_ex(int nAssetModelAreaNum, int nAlarmGroupModelAreaNum, int nSystemModelAreaNum, int nUnassignedItemsAreaNum);
EXTERN_C int		hsc_build_syscollection_points(void);
EXTERN_C int		hsc_build_server_points(void);

EXTERN_C int		hsc_check_point_parent(PNTNUM point);
EXTERN_C int		hsc_check_points_parent(void);
EXTERN_C int		hsc_check_area_table(void);
EXTERN_C int		hsc_check_emh(void);
EXTERN_C int		hsc_check_owners(void);
EXTERN_C int		hsc_check_and_correct_owner(PNTNUM point);
EXTERN_C BOOL		hsc_update_createtime_by_area(int AreaNum);
EXTERN_C int		hsc_entmodel_CheckInstall(void);

EXTERN_C int		hsc_migrate_system_area(void);
EXTERN_C int		hsc_migrate_createlocalgdaentry_skeleton(void);
EXTERN_C int		hsc_migrate_pre810_points(void);
EXTERN_C int		hsc_migrate_areas_to_assets(BOOL);
EXTERN_C int		hsc_migrate_area_to_asset(char*, int2);

EXTERN_C int		hsc_security_set_permissions(int2 filenum, int2 recnum, int2 areanum, int2 permissions);
EXTERN_C int		hsc_security_set_permissions_ex(int2 area, int2 permission);
EXTERN_C int		hsc_security_set_permissions_byarray(int2* areas, int aCount, int2 permission);
EXTERN_C int		hsc_security_get_permissions(int2 filenum, int2 recnum, int2 areanum, int2 *permissions);
EXTERN_C int		hsc_security_copy_permissions(int2 srcarea, int2 destarea);
EXTERN_C int		hsc_security_copy_permissions_ex(int2 filenum, int2 recnum, int2 srcarea, int2 destarea);
EXTERN_C int		hsc_security_copy_subordinates(int2 filenum, int2 recnum, int2 areanum);
EXTERN_C int		hsc_security_assign_all(int2 filenum, int2 recnum, int2 permission);
EXTERN_C BOOL		hsc_security_permissions_match(int2 filenum, int2 recnum, int2 areanum1, int2 areanum2, BOOL bOnlyCheckAssignment);
EXTERN_C BOOL		hsc_security_descendent_permissions_match(int2 filenum, int2 recnum, PNTNUM point, BOOL bOnlyCheckAssignment);
EXTERN_C int            hsc_security_handle_change(int2 filenum, int2 recnum);
EXTERN_C BOOL           hsc_security_allow_change(GDASECURITY *pSecurity, int2 filenum, int2 recnum, int2 areanum, BOOL bPropagated);

/*
MACROS:
*/

#define PRMDEF_WORD(x)	 ( (x    ) & 0x3F)
#define PRMDEF_FILE(x)	 ( (x>> 6) & 0x03)
#define PRMDEF_BIT(x)	 ( (x>> 8) & 0x0F)
#define PRMDEF_WIDTH(x)	 (((x>>12) & 0x0F) + 1)
#define PRMDEF_LENGTH(x) ( (x>> 8) & 0xFF)
#define PRMDEF_SET(x)	 (((1<<PRMDEF_WIDTH(x))-1)<<PRMDEF_BIT(x))
#define PRMDEF_CLR(x)	 (~PRMDEF_SET(x))
#define PRMDEF_BY_BIT(x,i)	(x + (((i*PRMDEF_WIDTH(x))%16)<<8) \
				   + (((i*PRMDEF_WIDTH(x))/16)   ))
#define PRMDEF_BY_INT2(x,i)	 (x + i)
#define PRMDEF_BY_REAL(x,i)	 (x + i*2)

#define IS_SYSTEM_ENTITY(remtype, subtype) ((remtype == SYS) || (((remtype == CDA) || (remtype == PSA)) && (subtype != 0)))
#define IS_ROOT_ENTITY(remtype, subtype) ((remtype == SYS) && (subtype == SYS_MODEL))
#define IS_OWNER_POINT(type, subtype) ((type == SYS) && (subtype == SYS_SERVER))

/*
END_DOC
*/

#endif	/* end of conditional include points_flag */

/************************************************************************/
/*****     COPYRIGHT 1996-2007      HONEYWELL LIMITED AUSTRALIA      ****/
/************************************************************************/


