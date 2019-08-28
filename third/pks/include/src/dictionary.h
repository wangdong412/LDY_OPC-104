/************************************************************************/
/*****     COPYRIGHT 1996-2006      HONEYWELL LIMITED AUSTRALIA      ****/
/************************************************************************/

#ifndef dictionary_flag
#define dictionary_flag

#include "src/shheap.h"

/*
@(#)dictionary.h,v	830.1.1.1
BEGIN_DOC
---------------------------------------------------------------------------
dictionary.h - dictionary database definitions
---------------------------------------------------------------------------
SUMMARY:
    This file contains a description of the shared memory heap
    used for holding the dictionary

DESCRIPTION:
    The first malloc in the heap is an array item structures, all of which
    will be classes, such as point. Each item contains a class or entity
    structure. A class would point to another array of item structures
    for the members of that class. Names are inline in the header.
    There is a count of how many null terminated names are contained in the
    block.

    +-------+        + ------+
    | array |------->| child |
    | of    |        | array |
    | items |        | of    |
    |       |        | items |     +-------+
    |       |        |       |---->| child |---> etc
    |       |        +-------+     | array |
    |       |                      | of    |
    +-------+                      | items |
				   +-------+

    -----------------------------------------------------------------------
    NOTES -
    -----------------------------------------------------------------------

DEFINITIONS:
*/

/********************************** Attribute name and value association list */

typedef struct
    {
    int		value;
    char*	text;
    } enumlist;

/********************************** Dictionary structure types */

typedef enum
    {
    HSC_CLASS = 1,
    HSC_ENTITY
    } DICTtype;
#ifdef dict_parse_flag
enumlist DICTtypes[] =
    {
    HSC_CLASS	,"CLASS",
    HSC_ENTITY	,"ENTITY",
    0		,(char*)NULL
    };
#else
extern enumlist DICTtypes[];
#endif

/********************************** Name access methods */

typedef enum
    {
    NAM_STATIC = 1,
    NAM_LOOKUP
    } NAMEmethod;
#ifdef dict_parse_flag
enumlist NAMEmethods[] =
    {
    NAM_STATIC	,"STATIC_N",
    NAM_LOOKUP	,"LOOKUP_N",
    0		,(char*)NULL
    };
#else
extern enumlist NAMEmethods[];
#endif

/********************************** Name method parameters */

typedef struct
    {
    int		file;
    int		record;
    int		word;
    short	width;
    } NAMElookup;

/********************************** Name structure */

#define MAX_NAME_LENGTH		256

typedef struct
    {
    NAMEmethod	    method;
    union
	{
	NAMElookup  lookup;
	} details;
    char	    string[4];
    // 1st byte is how many names
    // each name is a null terminated string
    } DICTname;

/********************************** Native data types */

#undef DT_CHAR		/* avoid definitions from "parameters" */
#undef DT_INT2
#undef DT_INT4
#undef DT_INT8
#undef DT_REAL
#undef DT_DBLE
#undef DT_HIST
#undef DT_VAR
#undef DT_ENUM
#undef DT_DATE_TIME
#undef DT_STATUS
#undef DT_SRCADDR
#undef DT_DSTADDR
#undef DT_POINTREF
#undef DT_SERVAR
#undef DT_TIME
#undef DT_DELTATIME
#undef DT_TIMEOFDAY
#undef DT_ALARMHANDLE
#undef DT_POINTREF2

typedef enum
    {
    DT_CHAR=1,	/* character string		*/
    DT_INT2,	/* 1 to 16 bit short integer	*/
    DT_INT4,	/* 32 bit integer		*/
    DT_REAL,	/* short float			*/
    DT_DBLE,	/* long float			*/
    DT_HIST,	/* history (-0 => large float)  */
    DT_VAR,	/* variant			*/
    DT_ENUM,	/* enumeration			*/
    DT_DATE_TIME,/* timestamp (int2 day,real sec)*/
    DT_STATUS,	/* status of a parameter value	*/
    DT_SRCADDR,	/* source address		*/
    DT_DSTADDR,	/* destination address		*/
    DT_SERVAR,	/* serialized variant		*/
    DT_POINTREF, /* used when creating remote container points	*/
    DT_INT8,	 /* 64 bit integer		*/
    DT_TIME,	 /* date and time (HSCTIME format)	*/
    DT_DELTATIME,/* delta time (HSCTIME format)		*/
    DT_TIMEOFDAY, /* time of day (HSCTIME format)	*/
    DT_ALARMHANDLE, /* handle for alarm grouping	*/
    DT_POINTREF2 /* used when creating remote container points	*/
    } DATAtype;
#ifdef dict_parse_flag
enumlist DATAtypes[] =
    {
    DT_CHAR	,"CHAR",
    DT_INT2	,"INT2",
    DT_INT4	,"INT4",
    DT_REAL	,"REAL",
    DT_DBLE	,"DBLE",
    DT_HIST	,"HIST",
    DT_VAR	,"VAR",
    DT_ENUM	,"ENUM",
    DT_DATE_TIME,"DATE_TIME",
    DT_STATUS	,"STATUS",
    DT_SRCADDR	,"SRCADDR",
    DT_DSTADDR	,"DSTADDR",
    DT_SERVAR	,"SERVAR",
    DT_POINTREF	,"POINTREF",
    DT_INT8	,"INT8",
    DT_TIME	,"TIME",
    DT_DELTATIME,"DELTATIME",
    DT_TIMEOFDAY,"TIMEOFDAY",
    DT_ALARMHANDLE,"ALARMHANDLE",
    DT_POINTREF2,"POINTREF2",
    0		,(char*)NULL
    };
#else
extern enumlist DATAtypes[];
#endif

/********************************** Data format methods */

#undef DF_CHAR		/* avoid definitions from "parameters" */
#undef DF_NUM
#undef DF_POINT
#undef DF_PARAM
#undef DF_ENG
#undef DF_PCT
#undef DF_ENUM
#undef DF_MODE
#undef DF_BIT
#undef DF_STATE
#undef DF_PNTTYPE
#undef DF_TIME
#undef DF_DATE
#undef DF_DATE_TIME
#undef DF_GETVAL
#undef DF_DATE_TIME_UST

typedef enum
    {
    DF_CHAR = 1,    /* character	    */
    DF_NUM,	    /* numeric		    */
    DF_POINT,	    /* point name	    */
    DF_PARAM,	    /* parameter name	    */
    DF_ENG,	    /* engineering units    */
    DF_PCT,	    /* percent		    */
    DF_ENUM,	    /* enumerated	    */
    DF_MODE,	    /* enumerated mode	    */
    DF_BIT,	    /* TRUE/FALSE	    */
    DF_STATE,	    /* state descriptor	    */
    DF_PNTTYPE,	    /* point type	    */
    DF_TIME,	    /* time		    */
    DF_DATE,	    /* date		    */
    DF_DATE_TIME,   /* time stamp	    */
    DF_GETVAL,	    /* format as pnt-param  */
    DF_DATE_TIME_UST /* time stamp in UST   */
    } DATAformat;
#ifdef dict_parse_flag
enumlist DATAformats[] =
    {
    DF_CHAR		,"CHAR",
    DF_NUM		,"NUM",
    DF_POINT		,"POINT",
    DF_PARAM		,"PARAM",
    DF_ENG		,"ENG",
    DF_PCT		,"PCT",
    DF_ENUM		,"ENUM",
    DF_MODE		,"MODE",
    DF_BIT		,"BIT",
    DF_STATE		,"STATE",
    DF_PNTTYPE		,"PNTTYPE",
    DF_TIME		,"TIME",
    DF_DATE		,"DATE",
    DF_DATE_TIME	,"DATE_TIME",
    DF_GETVAL		,"GETVAL",
    DF_DATE_TIME_UST	,"DATE_TIME_UST",
    0		,(char*)NULL
    };
#else
extern enumlist DATAformats[];
#endif

/********************************** Format method parameters */

typedef	struct
    {
    int		file;
    int		record;
    int		word;
    int		length;
    } DFenum;

typedef	struct
    {
    int		parameter;
    } DFstate;

typedef	struct
    {
    int		parameter;
    DATAtype	type;
    int		length;
    } DFgetval;

/********************************** Format structure */

#define	DFF_LEFT	0x0001	/* left justified */
#define DFF_RIGHT	0x0002	/* right justified */
#define DFF_ACROSS	0x0010	/* acronyms across record */

#ifdef dict_parse_flag
enumlist FMTflags[] =
    {
    DFF_LEFT	,"LEFT",
    DFF_RIGHT	,"RIGHT",
    DFF_ACROSS	,"ACROSS",
    0		,(char*)NULL
    };
#else
extern enumlist FMTflags[];
#endif

typedef struct
    {
    DATAformat	    method;
    union
	{
	DFenum	    fenum;
	DFstate	    state;
	DFgetval    getval;
	} details;
    int		    flag;
    } DICTformat;

/********************************** Data conversion methods */

typedef enum
    {
    DC_PO2 = 1,	    /* (n+1)th power of 2 minus 1   */
    DC_SCALE	    /* type conversion		    */
    } DATAconversion;
#ifdef dict_parse_flag
enumlist DATAconversions[] =
    {
    DC_PO2	,"PO2",
    DC_SCALE	,"SCALE",
    0		,(char*)NULL
    };
#else
extern enumlist DATAconversions[];
#endif

/********************************** Conversion method parameters */

typedef	struct
    {
    double	bias;
    double	scale;
    } DCscaled;

/********************************** Conversion structure */

typedef struct
    {
    DATAconversion  method;
    union
	{
	DCscaled    scaled;
	} details;
    } DICTconversion;

/********************************** Data access methods	*/

#undef DA_DATAIO
#undef DA_PRMDEF
#undef DA_PARAM
#undef DA_CDA
#undef DA_EVENT
#undef DA_QUE
#undef DA_HISTORY
#undef DA_CONSTANT
#undef DA_HSTDTM
#undef DA_VARIABLE
#undef DA_POINT
#undef DA_FIELDBUS_MODE
#undef DA_ALMCOUNT
#undef DA_MSGCOUNT
#undef DA_PNTALM
#undef DA_RELATIVES
#undef DA_PNTBLK
#undef DA_FUNCTION
#undef DA_ALTCOUNT

typedef enum
    {
    DA_DATAIO = 1,
    DA_PRMDEF,
    DA_PARAM,
    DA_CDA,
    DA_EVENT,
    DA_QUE,
    DA_HISTORY,
    DA_CONSTANT,
    DA_HSTDTM,
    DA_VARIABLE,
    DA_POINT,
    DA_FIELDBUS_MODE,
    DA_ALMCOUNT,
    DA_MSGCOUNT,
    DA_PNTALM,
    DA_RELATIVES,
    DA_PNTBLK,
    DA_FUNCTION,
    DA_ALTCOUNT
    } DATAaccess;
#ifdef dict_parse_flag
enumlist DATAaccesses[] =
    {
    DA_DATAIO		,"DATAIO",
    DA_PRMDEF		,"PRMDEF",
    DA_PARAM		,"PARAM",
    DA_CDA		,"CDA",
    DA_EVENT		,"EVENT",
    DA_QUE		,"QUE",
    DA_HISTORY		,"HISTORY",
    DA_CONSTANT 	,"CONSTANT",
    DA_HSTDTM		,"HIST_DTM",
    DA_VARIABLE		,"VARIABLE",
    DA_POINT		,"POINT",
    DA_FIELDBUS_MODE	,"FIELDBUS_MODE",
    DA_ALMCOUNT		,"ALMCOUNT",
    DA_MSGCOUNT		,"MSGCOUNT",
    DA_PNTALM		,"PNTALM",
    DA_RELATIVES	,"RELATIVES",
    DA_PNTBLK		,"PNTBLK",
    DA_FUNCTION		,"FUNCTION",
    DA_ALTCOUNT		,"ALTCOUNT",
    0			,(char*)NULL
    };
#else
extern enumlist DATAaccesses[];
#endif

/********************************** Access method parameters */

typedef	struct
    {
    int		file;
    int		record;
    int		word;
    short	bit;
    short	width;
    } DAdataio;

typedef	struct
    {
    int		point;
    int		definition;
    int		offset;
    } DAprmdef;

typedef	struct
    {
    int		point;
    int		param;
    int		offset;
    } DAparam;

typedef	struct
    {
    int		period;		/* period of subscription */
    uint4	lastupd;	/* time of last update */
    uint4	lastrdacs;	/* time of last read access (for average calc) */
    uint4	average;	/* average access period (determines auto-subscribe) */
    uint4	lastacs;	/* time of last access to value block */
    } DAcda;

typedef	struct
    {
    int		word;
    short	offset;
    short	width;
    } DAevent;

typedef	struct
    {
    int		file;
    int		header;
    } DAque;

typedef	struct
    {
    int		file;
    int		point;
    int		param;
    } DAhistory;

typedef	struct
    {
    GINDX	gPoint;
    } DApoint;

typedef	struct
    {
    int		actual;		/* actual mode parameter number */
    int		target;		/* target mode parameter number */
    } DAfieldbusmode;

typedef	struct
    {
    int		priority;
    int		active;
    int		acknowledged;
    } DAalmcount;

typedef struct
    {
    int		confirmable;
    int		acknowledged;
    } DAmsgcount;

typedef	struct
    {
    int		active;
    int		acknowledged;
    } DAaltcount;

typedef struct
    {
    int		word;
    int		bit;
    int		width;
    } DApntalm;

typedef struct
    {
    int		type;
    } DArelatives;

typedef struct
    {
    int		word;
    int		bit;
    int		width;
    } DApntblk;

typedef struct
    {
    int		ordinal;	/* ordinal of function entry in jump table */
    int		param1;		/* parameter to pass the function */
    int		param2;		/* parameter to pass the function */
    int		param3;		/* parameter to pass the function */
    } DAfunction;

/********************************** Access structure */

typedef struct
    {
    DATAaccess		method;
    union
	{
	DAdataio	dataio;
	DAprmdef	prmdef;
	DAparam		param;
	DAcda		cda;
	DAevent		event;
	DAque		queue;
	DAhistory	history;
	DApoint		point;
	DAfieldbusmode	fieldbusmode;
	DAalmcount	almcount;
	DAmsgcount	msgcount;
	DApntalm	pntalm;
	DArelatives	relatives;
	DApntblk	pntblk;
	DAfunction	function;
	DAaltcount	altcount;
	} details;
    short		repeat;
    short		pad;
    } DICTaccess;

/********************************** Value */

typedef struct
    {
    DATAtype  	    type;
    DICTformat	    format;
    DICTaccess 	    access;
    DICTconversion  conversion;
    GDAERR	    quality;
    union
	{
	short	int2;
	long	int4;
	int8	int8;
	float	real;
	double	dble;
	uint2	param;
	GINDX	gtext;	/* pointer to character array */
	struct
	    {
	    GINDX gvar;	/* pointer to variant */
	    DATAtype dt;	/* size of variant*/
	    } var;
	GINDX	genum;	/* pointer to 'enumlist' */
	struct
	    {
	    short   day;    /* note:			*/
	    float   sec;    /* this is 32 bit aligned	*/
	    } datetime;
	HSCTIME time;
	} value;
    uint4   BBIndex;	/* basic block index */
    uint2   EECode;	/* execution environment code */
    uint2   _spare1;	/* spare - included to preserve alignment */
    } DICTvalue;

/********************************** ODBC structure */

#define	FL_PRM	0x0001		/* primary key */
#define FL_IDX	0x0002		/* non-unique index key */

#ifdef dict_parse_flag
enumlist ODBCflags[] =
    {
    FL_PRM	,"PRMKY",
    FL_IDX	,"INDEX",
    0		,(char*)NULL
    };
#else
extern enumlist ODBCflags[];
#endif

typedef struct
    {
    int		    flag;
    int		    table;	/* column is key to this table */
    } DICTodbc;

/********************************** Class (table) definition structure */

typedef struct
    {
    GINDX	child;			/* childrenn */
    int		area;			/* position/parameter of area code 	*/
    short	enable_wrd;		/* word offset of record enable flag	*/
    short	enable_bit;		/* bit  offset of record enable flag	*/
    } DICTclass;

/********************************** Entity (column) definition structure */

typedef struct				/* as stored in the dictionary */
    {
    DICTodbc		odbc;
    DICTvalue		value;
    GINDX		limit_min;
    GINDX		limit_max;
    GINDX		range_min;
    GINDX		range_max;
    } DICTentity;

typedef struct				/* as used by fatDICTitem */
    {
    DICTodbc		odbc;
    DICTvalue		value;
    DICTvalue		limit_min;
    DICTvalue		limit_max;
    DICTvalue		range_min;
    DICTvalue		range_max;
    } fatDICTentity;

/********************************** Item header */

#define FL_RD	0x0001          /* read access */
#define FL_WRT	0x0002          /* write access */
#define FL_RW	(FL_RD|FL_WRT)	/* read & write access */
#define FL_OVR	0x0004          /* parameter overload allowed */
#define FL_CDA	0x0008          /* parameter must be overload by CDA method */
#define FL_NDS	0x0010          /* parameter is not valid on direct station */
#define FL_CWD	0x0020          /* clusterwide, this parameter should be replicated to console stations */
#define FL_LOC	0x0040          /* parameter must be overload by a LOCAL method */


#ifdef dict_parse_flag
enumlist HDRflags[] =
    {
     FL_RD          ,"FL_RD",
     FL_WRT         ,"FL_WRT",
     FL_RW          ,"FL_RW",
     FL_OVR         ,"FL_OVR",
    (FL_CDA|FL_OVR)	,"FL_CDA",
    (FL_NDS|FL_OVR)	,"FL_NDS",
    (FL_CWD|FL_RW)	,"FL_CWD",
    (FL_LOC|FL_OVR)	,"FL_LOC",
    0			,(char*)NULL
    };
#else
extern enumlist HDRflags[];
#endif

typedef struct
    {
    int			size;	// size of header structure (bytes)
    DICTtype		type;
    int			number;
    int			flag;
    union {
	DICTclass	classy;
	DICTentity	entity;
	} ce;
    DICTname		name;	// variable length
    } DICTitem;

typedef struct			// as used by the parser and create
    {
    int			size;	// size of header structure (bytes)
    DICTtype		type;
    int			number;
    int			flag;
    union {
	DICTclass	classy;
	fatDICTentity	entity;
	} ce;
    DICTname		name;	// variable length
    } fatDICTitem;

/**********************************/

/*
PROTOTYPES:
*/

EXTERN_C DICTitem*	hsc_dict_get		();
EXTERN_C DICTitem*	hsc_dict_add_by_name	(DICTitem*,char**,fatDICTitem*,char*);
EXTERN_C DICTitem*	hsc_dict_find_by_name	(DICTitem*,char**);
EXTERN_C DICTitem*	hsc_dict_find_by_number	(DICTitem*,int*);
EXTERN_C int		hsc_dict_list		(DICTitem*,DICTtype,enumlist**);
EXTERN_C void		hsc_dict_list_unique	(char*,DICTitem*,enumlist*,int*,int*);
EXTERN_C void		hsc_dict_list_unique_last(char*,DICTitem*,enumlist*,int*,int*);
EXTERN_C void		hsc_dict_list_unique_position(char*,DICTitem*,enumlist*,int*,int*,int);
EXTERN_C void		hsc_dict_walk		(DICTitem*,char*,void(*routine)(char*,...),void*,void*,void*);
EXTERN_C void		hsc_dict_dump		(DICTitem*,char*);
EXTERN_C int		hsc_dict_parse		(char*,fatDICTitem*,char*,char**,char*);
EXTERN_C char*		hsc_dict_nextToken	(char*);

/*
END_DOC
*/

#endif	/* end of conditional include dictionary_flag */

/************************************************************************/
/*****     COPYRIGHT 1996-2006      HONEYWELL LIMITED AUSTRALIA      ****/
/************************************************************************/
