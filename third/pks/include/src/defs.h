/************************************************************************/
/*****     COPYRIGHT 1988-2004      HONEYWELL LIMITED AUSTRALIA      ****/
/************************************************************************/

#ifndef defs_flag
#define defs_flag

/*
@(#)defs.h,v	830.1.1.3
BEGIN_DOC
---------------------------------------------------------------------------
DEFS.H - user's system header file
---------------------------------------------------------------------------

DESCRIPTION:
    This is the ultimate in headers, and should be used for all system wide
    #defines and declarations.

    It defines
	UNIX system constants
	SN90 system constants and option flags
	wierd data types
	commonly used macros

*/

#include <stdio.h>
#include "system"
#include "src/lfns"
#include "src/platform"

/*
------------------------------------------------------------------------
Specific operating system name, choose from:
    UNIX_VAT			Microport's System V/AT
    UNIX_V386			Microport's System V/386
    UNIX_SUPERTEAM_V_2		Honeywell's Superteam Unix V version VA80
    VMS_VAX			Digital's VMS on a VAX
    HP_UX			Hewlett Packard Unix version 8.0 (BSD)
------------------------------------------------------------------------
*/
#ifdef  VMS
#define VMS_VAX
#elif   defined(HP_UX)
#else
#define UNIX_V386
#endif


/*
------------------------------------------------------------------------
The rest of the definitions are self guided and DON'T need to be changed
from system to system.
------------------------------------------------------------------------
Generic operating system version and release:
    UNIX_SYSTEM_V_RELEASE_2	AT&T system V release 2 standard
    UNIX_SYSTEM_V_RELEASE_3	AT&T system V release 3 standard
    VMS_VERSION_5		Digital's VMS version 5
Dependancies on specific operating systems
    LFINDNULL			value returned by lfind(3)
    INT_EQ_SHORT		defined if int2==short (int2!=int)
    SWAP_BYTES			defined if int2's are stored lsb first
    SWAP_WORDS			defined if int4's are stored lsw first
------------------------------------------------------------------------
*/
#ifdef	UNIX_VAT		/* Microport's System V/AT */
#define	UNIX_SYSTEM_V_RELEASE_2	/* AT&T system V release 2 standard */
#define	LFINDNULL	-1	/* bug in microports unix */
#define SWAP_BYTES 	TRUE	/* switch on byte swapping routines */
#define SWAP_WORDS 	TRUE	/* switch on word swapping routines */
#endif

#ifdef 	UNIX_V386		/* Microport's System V/386 */
#define	UNIX_SYSTEM_V_RELEASE_3	/* AT&T system V release 3 standard */
#define	LFINDNULL	NULL	/* correct definition for unix */
#define INT_EQ_SHORT	TRUE	/* an int2==short */
#define SWAP_BYTES 	TRUE	/* switch on byte swapping routines */
#define SWAP_WORDS 	TRUE	/* switch on word swapping routines */
#endif

#ifdef 	UNIX_SUPERTEAM_V_2	/* Honeywell's Superteam Unix V version VA80 */
#define	UNIX_SYSTEM_V_RELEASE_2	/* AT&T system V release 2 standard */
#define	LFINDNULL	NULL	/* correct definition for unix */
#define INT_EQ_SHORT	TRUE	/* an int2==short */
#define B19200		B9600	/* define 19.2kb as 9600 for release 2 */
#define B38400		B9600	/* define 38.4kb as 9600 for release 2 */
#endif

#ifdef 	VMS_VAX			/* Digital's VMS */
#define	VMS_VERSION_5		/* Digital's VMS version 5 */
#define	LFINDNULL	NULL	/* correct definition for unix */
#define INT_EQ_SHORT	TRUE	/* an int2==short */
#define SWAP_BYTES 	TRUE	/* switch on byte swapping routines */
#define SWAP_WORDS 	TRUE	/* switch on word swapping routines */
#endif

#ifdef 	HP_UX			/* Hewlett packard's UNIX supports-> */
#define	UNIX_SYSTEM_V_RELEASE_3	/* AT&T system V release 3 standard */
#define	LFINDNULL	NULL	/* correct definition for unix */
#define INT_EQ_SHORT	TRUE	/* an int2==short */
#endif

#ifdef NT
#define	LFINDNULL	NULL	/* correct definition for unix */
#define INT_EQ_SHORT	TRUE	/* an int2==short */
#define SWAP_BYTES 	TRUE	/* switch on byte swapping routines */
#define SWAP_WORDS 	TRUE	/* switch on word swapping routines */
#define O_NDELAY	0x00	/* non-blocking I/O */
#endif

/*
------------------------------------------------------------------------
Dependancies on generic operating systems
    SIG_ERR			value returned by signal(2)
    noshare			VMS C declaration of non-sharable data
------------------------------------------------------------------------
*/
#ifdef	UNIX_SYSTEM_V_RELEASE_2
#define	SIG_ERR		(int(*)())-1
#define noshare
#endif

#ifdef	UNIX_SYSTEM_V_RELEASE_3
#define noshare
#endif

#ifdef	VMS_VERSION_5
#define sigset(x,y)  signal(x,y)
#ifndef SIG_ERR
/* This definition causes a problem on ALPHA, its already defined */
#define	SIG_ERR		(int(*)())-1
#endif
#endif

/*
------------------------------------------------------------------------
Defines macros to rename C routines callable from both C & FORTRAN
    eg datopn => datopn$ from C
                 DATOPN  from FORTRAN
------------------------------------------------------------------------
*/

#ifdef  VMS_VERSION_5

#include	<descrip.h>		/* VMS FORTRAN chr descriptor */

#define		abrclo		abrclo$
#define		abropn		abropn$
#define		abrrea		abrrea$
#define		abracq		abracq$
#define		abrcnt		abrcnt$
#define		abrcntnw	abrcntnw$
#define		abrpio		abrpio$
#define		abrelemsz	abrelemsz$
#define		addtm		addtm$
#define		asc2date	asc2date$
#define		asc2time	asc2time$
#define		almdec		almdec$
#define		alminc		alminc$
#define		alm_resp	alm_resp$
#define		alpha		alpha$
#define		arshft		arshft$
#define		asctm		asctm$
#define		asctof		asctof$
#define		asctod 		asctod$
#define		asctodh		asctodh$
#define		asctodo		asctodo$
#define		asctop 		asctop$
#define		asctoh 		asctoh$
#define		asctoi 		asctoi$
#define		asctoll		asctoll$
#define		asctoo 		asctoo$
#define		ascvrt		ascvrt$
#define		avail_lrn       avail_lrn$
#define		bbtsnrt		bbtsnrt$
#define		bblocrtu	bblocrtu$
#define		bcd2bin		bcd2bin$
#define		bin2bcd		bin2bcd$
#define		brken		brken$
#define		brkds		brkds$
#define		btestx 		btestx$
#define		chrint		chrint$
#define		cl		cl$
#define		ClearScreen   	ClearScreen$
#define		cfg_draw   	cfg_draw$
#define		comerr		comerr$
#define		compar		compar$
#define		crgtsk		crgtsk$
#define		crtasn		crtasn$
#define		crttsk		crttsk$
#define		cvpath		cvpath$
#define		datclo		datclo$
#define		datcre		datcre$
#define		datcreate	datcreate$
#define		datdel		datdel$
#define		datget		datget$
#define		datopn		datopn$
#define		datopr		datopr$
#define		datrea		datrea$
#define		datren		datren$
#define		datrew		datrew$
#define		datrmv		datrmv$
#define		datwri		datwri$
#define		datwrn		datwrn$
#define		datwrt		datwrt$
#define		datxmt		datxmt$
#define		db_lock  	db_lock$
#define		db_lockt  	db_lockt$
#define		db_lockint  	db_lockint$
#define		db_unlock	db_unlock$
#define		ddt_init	ddt_init$
#define		ddt_updrec	ddt_updrec$
#define		deffnd		deffnd$
#define		delpnt		delpnt$
#define		deltag		deltag$
#define		deltm		deltm$
#define		deltsk		deltsk$
#define		devfmt		devfmt$
#define		dfdldi		dfdldi$
#define		dfdldt		dfdldt$
#define		dfdsti		dfdsti$
#define		dfdstt		dfdstt$
#define		dget		dget$
#define		dirrea		dirrea$
#define		dlgtsk		dlgtsk$
#define		dput  		dput$
#define		dsply_lrn       dsply_lrn$
#define		dtoasc		dtoasc$
#define		dhtoasc		dhtoasc$
#define		dotoasc		dotoasc$
#define         dvclrn          dvclrn$
#define		equal		equal$
#define		equal_i		equal_i$
#define		errmsg		errmsg$
#define		errout		errout$
#define		ex		ex$
#define		favg		favg$
#define		fclamp		fclamp$
#define		fdec		fdec$
#define		fget		fget$
#define		filinf		filinf$
#define		filrea		filrea$
#define		filwrt		filwrt$
#define		filxmt		filxmt$
#define		finc		finc$
#define		findarea	findarea$
#define		fput		fput$
#define		fromieee	fromieee$
#define		fromlong	fromlong$
#define		fssize		fssize$
#define		ftoasc		ftoasc$
#define		g_par		g_par$
#define		ge6io		ge6io$
#define		getbyt		getbyt$
#define		getcom		getcom$
#define		getdat		getdat$
#define		getfld		getfld$
#define		gethstpar	gethstpar$
#define		gethst		gethst$
#define		gethst		gethst$
#define		gethst		gethst$
#define		gethst		gethst$
#define		gethst		gethst$
#define		getht		getht$
#define		getlst		getlst$
#define		getlrn		getlrn$
#define		getntag		getntag$
#define		getpnt		getpnt$
#define		getprm		getprm$
#define		getreq		getreq$
#define		gettag		gettag$
#define		getval		getval$
#define		gpmain		gpmain$
#define		gregor		gregor$
#define		givlst		givlst$
#define		hsc_s90cbrtu	hsc_s90cbrtu$
#define		hshcre		hshcre$
#define		hshrea		hshrea$
#define		hshwrt		hshwrt$
#define		hstavg		hstavg$
#define		hstclr		hstclr$
#define		hstxmt		hstxmt$
#define		htoasc		htoasc$
#define		i2dec		i2dec$
#define		i2inc		i2inc$
#define 	i6add		i6add$
#define 	i8add		i8add$
#define 	i8sub		i8sub$
#define 	i8mul		i8mul$
#define 	i8div		i8div$
#define 	i8shl		i8shl$
#define		ialpha		ialpha$
#define		iandx 		iandx$
#define		ibclrx		ibclrx$
#define		ibitsx		ibitsx$
#define		ibsetx		ibsetx$
#define		ieee_from_mc	ieee_from_mc$
#define		ieee_to_mc	ieee_to_mc$
#define		ieorx		ieorx$
#define		intchr		intchr$
#define		inttm		inttm$
#define		iorx		iorx$
#define		irot		irot$
#define		irot4		irot4$
#define		ipcio		ipcio$
#define		ipsio		ipsio$
#define		ishftx		ishftx$
#define		itoasc		itoasc$
#define		iupper		iupper$
#define		jbclr		jbclr$
#define		jbits		jbits$
#define		jbits4		jbits4$
#define		jbset		jbset$
#define		mvjbits		mvjbits$
#define		julian		julian$
#define		jultm		jultm$
#define		kbdsv_lrn       kbdsv_lrn$
#define		keyrea		keyrea$
#define		keydel		keydel$
#define		keywrt		keywrt$
#define		kiltsk		kiltsk$
#define		lcsio 		lcsio$
#define		lftjst		lftjst$
#define		licencechk	licencechk$
#define		lltoasc		lltoasc$
#define		lnknok		lnknok$
#define		lnktrn		lnktrn$
#define		max_lic_pnt	max_lic_pnt$
#define		max_lic_sta	max_lic_sta$
#define		max_lic_ana	max_lic_ana$
#define		max_lic_acc	max_lic_acc$
#define		max_lic_acs	max_lic_acs$
#define		max_lic_crd	max_lic_crd$
#define		max_lic_std	max_lic_std$
#define		max_lic_ext	max_lic_ext$
#define		max_lic_fst	max_lic_fst$
#define		max_lic_hst	max_lic_hst$
#define		max_lic_lpt	max_lic_lpt$
#define		max_lic_chn	max_lic_chn$
#define		max_lic_rtu	max_lic_rtu$
#define		max_lic_alg	max_lic_alg$
#define		max_lic_tnd	max_lic_tnd$
#define		max_lic_grp	max_lic_grp$
#define		max_lic_rpt	max_lic_rpt$
#define		max_lic_sdl	max_lic_sdl$
#define		max_lic_opr	max_lic_opr$
#define		max_lic_rcp	max_lic_rcp$
#define		max_lic_alm	max_lic_alm$
#define		max_lic_msg	max_lic_msg$
#define		max_lic_soe	max_lic_soe$
#define		max_lic_evt	max_lic_evt$
#define		max_lic_dly	max_lic_dly$
#define		max_lic_ggl	max_lic_ggl$
#define		max_lic_dsp	max_lic_dsp$
#define		max_lic_sha	max_lic_sha$
#define		max_lic_rsp	max_lic_rsp$
#define		max_lic_arc	max_lic_arc$
#define		max_lic_ulplvl	max_lic_ulplvl$
#define		max_lic_ulpspc	max_lic_ulpspc$
#define		max_lic_ulptpd	max_lic_ulptpd$
#define		max_lic_slglvl	max_lic_slglvl$
#define		max_lic_slgspc	max_lic_slgspc$
#define		max_lic_slgtpd	max_lic_slgtpd$
#define		max_lic_slgrpt	max_lic_slgrpt$
#define		max_lic_slgzon	max_lic_slgzon$
#define		max_lic_lacgrp	max_lic_lacgrp$
#define		max_lic_utbl	max_lic_utbl$
#define		max_def_utbtyp	max_def_utbtyp$
#define		max_def_utbsiz	max_def_utbsiz$
#define		max_def_utbnum	max_def_utbnum$
#define		mfccnv		mfccnv$
#define		modio		modio$
#define		morerq		morerq$
#define		move		move$
#define		movbyt		movbyt$
#define		msiday		msiday$
#define		mvbitx		mvbitx$
#define		mzero		mzero$
#define		notransfer	notransfer$
#define		nuout		nuout$
#define		nxtfld		nxtfld$
#define		otoasc		otoasc$
#define		pad_init	pad_init$
#define		pad_trymsd	pad_trymsd$
#define		pad_trynam	pad_trynam$
#define		pad_updmsd	pad_updmsd$
#define		pad_updtim	pad_updtim$
#define		pget		pget$
#define		pput		pput$
#define		prmfnd		prmfnd$
#define		ptoasc		ptoasc$
#define		putbyt		putbyt$
#define		putcom		putcom$
#define		putdat		putdat$
#define		putht		putht$
#define		puthst		puthst$
#define		putpnt		putpnt$
#define		putval		putval$
#define		puttag		puttag$
#define		q_reset		q_reset$
#define		qald		qald$
#define		qbsc		qbsc$
#define		qcnt		qcnt$
#define		qdaq		qdaq$
#define		qprt		qprt$
#define		qlock		qlock$
#define		qunlock		qunlock$
#define		rawtm		rawtm$
#define		rbe_ack		rbe_ack$
#define		rbe_act		rbe_act$
#define		rbe_dmd		rbe_dmd$
#define		rbe_init	rbe_init$
#define		rbe_rspact	rbe_rspact$
#define		rbe_rsperr	rbe_rsperr$
#define		rbe_rspini	rbe_rspini$
#define		rbe_rsprpt	rbe_rsprpt$
#define		rbe_rspsts	rbe_rspsts$
#define		rbe_sts		rbe_sts$
#define		rdb_fn00	rdb_fn00$
#define		rdb_fn04	rdb_fn04$
#define		rdb_fn14	rdb_fn14$
#define		rdb_fn80	rdb_fn80$
#define		rdb_fn84	rdb_fn84$
#define		rdb_fn94	rdb_fn94$
#define		rdb_resp	rdb_resp$
#define		relsem		relsem$
#define		ressem		ressem$
#define		rgtfnd		rgtfnd$
#define		rgtjst		rgtjst$
#define		rqcl		rqcl$
#define		rqgtsk		rqgtsk$
#define		rqstsk		rqstsk$
#define		rqtskb		rqtskb$
#define		rwflxmt		rwflxmt$
#define		s90io		s90io$
#define		savval		savval$
#define		sig_onsrv	sig_onsrv$
#define		settm		settm$
#define		spath		spath$
#define		sptsk		sptsk$
#define		SQLsubref	SQLsubref$
#define  	sqdacq		sqdacq$
#define  	sqdcnt		sqdcnt$
#define  	sqdpac		sqdpac$
#define  	sqdpio		sqdpio$
#define  	sqdopn		sqdopn$
#define  	sqdclo		sqdclo$
#define  	sqdtim		sqdtim$
#define		srch  		srch$
#define		srchar		srchar$
#define		ssne		ssne$
#define		stn_num         stn_num$
#define		subtm		subtm$
#define		swab		swab$
#define		tagrea		tagrea$
#define		tbldmp		tbldmp$
#define		toieee		toieee$
#define		tolong		tolong$
#define		trcsyn		trcsyn$
#define		trm04		trm04$
#define		trmtsk		trmtsk$
#define		tstskb		tstskb$
#define		udcio      	udcio$
#define		upper      	upper$
#define		vewreq		vewreq$
#define		vld		vld$
#define		vrfy  		vrfy$
#define		wttskb		wttskb$
#define		xpath		xpath$

#endif

#ifdef _WIN32
#ifndef WIN32
#define WIN32
#endif
#ifndef NT
#define NT
#endif

#ifndef _X86_
#define _X86_ 1
#endif
#endif /* _WIN32 */

#ifdef NT
#define WSA_VER MAKEWORD(2,2)
#ifdef WIN16
#include <windows.h>
#define _stdcall
#define __stdcall
#else
#include <windef.h>
#include <winnt.h>
#include <excpt.h>
#include <winbase.h>
#include <io.h>
#include <stddef.h>
#include <process.h>
#endif

#define pause()		SuspendThread(GetCurrentThread())
#define sigset(x,y)	signal(x,y)
#define sleep(t)	Sleep(1000*(t))
#define swab		_swab

typedef int key_t;

#endif

/*
------------------------------------------------------------------------
Compiler flags, comment out to turn off:
------------------------------------------------------------------------
*/
#define PARANOID 	    		TRUE	/* switch on heavy checking */

/* #define SOFTWARE_PROTECTION_IMPLEMENTED TRUE /* perform dongle checks */
/* #define SOFTWARE_PROTECTION_WRITE_TRB TRUE   /* clear trb on failed check */
/* #define NO_DISPLAY_COMPRESS TRUE */ 	/* dont try to compress displays */

/*
------------------------------------------------------------------------
define a few of our own types
------------------------------------------------------------------------
*/

#include "hsctypes.h"

#define INT4_SZ		2	/* number of int2's per fortran int4 */
#define REAL_SZ		2	/* number of int2's per fortran real */
#define DBLE_SZ		4	/* number of int2's per fortran double */

#ifndef FAR
#define FAR
#endif
#ifndef CALLBACK
#define CALLBACK
#endif

/* standard simple definitions */
#ifndef TRUE
/* This is normally defined as 1, the redefinition causes a problem on ALPHA */
#define	TRUE		-1
#endif
#define FALSE		0
#define VALID   	1
#define INVALID 	0

#ifdef NT
#define PATH_SEPARATOR 	'\\'
#else
#define PATH_SEPARATOR 	'/'
#endif

/* system range definitions */
#define MAXINT8		((int8)(((uint8)-1) >> 1))
#define MAXINT4		((int4)(((uint4)-1) >> 1))
#define MAXUINT4	((uint4)-1))
#define MAXINT2		((int2)(((uint2)-1) >> 1))
#define MAXUINT2	((uint2)(-1))
#define MININT8		((int8)(~MAXINT8))
#define MININT4		((int4)(~MAXINT4))
#define MINUINT4	((uint4)(0))
#define MININT2		((int2)(~MAXINT2))
#define MINUINT2	((uint2)(0))

/* project range definitions */
#define MAX_NUMBER_LFNS 	NUM_LFNS	/* number of sn90 lfns */
#define MIN_FILE		1
#define MIN_LFN 		RUNLFN
#define MAX_LRN 		(300+NUM_CRTS)
#define MIN_LRN 		-1
#define MAX_ASCII_REC_SIZE	132	/* bytes */
#define DATA_LFN 		RUNLFN
#define DATA_REC_SIZE 		DISC_SSZ	/* bytes */
#define DATA_FILE_REL_PATH	"../data/data"
#define DEVICE_FILE_REL_PATH	"../data/device"
#define LOGFILE     		"/../data/log"
#define BACKUP_LFN 		BUPLFN
#define SPACES_PER_TAB		8

/* channel lrns */
#define LPT1_LRN		  6	/* 15 printers  */
#define DSDC_LRN		 21	/* Direct Station Network Components LRN */
#define CHN1_LRN		 51	/* 99 channels  */
#define RIPSD_LRN		300	/* Comms LRN used for Ripsd tracing */
#define CRT1_LRN		301	/* ?? stations  */

/* a few simple macros */

#ifndef min		/* defined for NT already */
#define min(a,b)  (((a)>(b))? (b) : (a))
#define max(a,b)  (((a)>(b))? (a) : (b))
#endif

#define __HILOBYTE
#define hibyte(int2ptr)	 	 ((*(int2ptr) >> 8) & 0x00ff)
#define lobyte(int2ptr)  	 (*(int2ptr) & 0x00ff)

#define sthibyte(int2ptr,byte)   *(int2ptr) = (*(int2ptr) & 0x00ff) \
						| ((byte)<<8)

#define stlobyte(int2ptr,byte)   *(int2ptr) = (*(int2ptr) & 0xff00) \
						| ((byte) & 0xff)

#define ldint4(int2ptr)		 (*(int4 *)(int2ptr))
#define stint4(int2ptr,int4val)	 *(int4 *)(int2ptr) = (int4val)

#define ldint8(int2ptr)		 (*(int8 *)(int2ptr))
#define stint8(int2ptr,int8val)	 *(int8 *)(int2ptr) = (int8val)

#define ldreal(int2ptr)		 (*(float *)(int2ptr))
#define streal(int2ptr,realval)	 *(float *)(int2ptr) = (realval)

#define lddble(int2ptr)		 (*(double *)(int2ptr))
#define stdble(int2ptr,dbleval)	 *(double *)(int2ptr) = (dbleval)

#include "src/prototypes.h"
#include "src/points.h"
#include "src/dictionary.h"

// Macro for setting the locale so that references to the
// g_hsc_LocaleIsSet global variable are not littered throughout
// the code in the system. See clib\cstr\setlocal.c for more
// information.
#define HSC_SET_LOCALE	hsc_set_locale();

// Error macros
#define CHECK_RETVAL(x) if ((x) == -1) return -1;

/*
END_DOC
*/

#endif		/* end of defs_flag if */
/************************************************************************/
/*****     COPYRIGHT 1988-2004      HONEYWELL LIMITED AUSTRALIA      ****/
/************************************************************************/
