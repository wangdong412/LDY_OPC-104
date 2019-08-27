/************************************************************************/
/*****     COPYRIGHT 2008           HONEYWELL INTERNATIONAL SARL     ****/
/************************************************************************/

#ifndef PROTOTYPES_FLAG
#define PROTOTYPES_FLAG
/*
@(#)prototypes.h,v	830.4.1.20
BEGIN_DOC
---------------------------------------------------------------------------
prototypes.h - CLIB function prototypes
---------------------------------------------------------------------------
DESCRIPTION:
	This header file contains prototypes for all functions defined
	under CLIB.

    -----------------------------------------------------------------------
    NOTES - The _DECLARE macro is defined to optionally omit argument types
	    where necessary.
    -----------------------------------------------------------------------
*/

#ifndef _DECLARE
#ifdef NT
#define _DECLARE(formal_parameters) formal_parameters
#else
#define _DECLARE(formal_parameters) ()
#endif
#endif /* _DECLARE */

// definitions

#ifndef EXTERN_C
#ifdef __cplusplus
    #define EXTERN_C    extern "C"
#else
    #define EXTERN_C    extern
#endif
#endif

#ifndef STDCALL
#ifdef NT
    #define STDCALL    _stdcall
#else
    #define STDCALL
#endif
#endif

#ifndef __cplusplus
#ifndef inline
#define inline __inline
#endif
#endif

#ifndef THREAD
#ifdef NT
    #define THREAD	__declspec(thread)
#else
    #define THREAD
#endif
#endif

#include "src/mzero.h"
#include "src/trbtbl_def"
#include "src/evtfil_def"
#include "src/notifhook_fun"
#include "src/dictionary.h"
#include "src/operid_def"


// inline functions

// inline function to protect the initialization of a data structure
inline int hsc_Protected_Initialize(volatile long* lpLock, int CALLBACK InitializationRoutine(void))
{
    if (*lpLock==0)		/* do faster cached memory check first */
    {
	if (InterlockedCompareExchange(lpLock,1,0)==0)
	{
	    if (InitializationRoutine())
	    {
		InterlockedExchange(lpLock,0);
		return -1;
	    }
	    InterlockedExchange(lpLock,2);
	}
    }
    while (*lpLock==1)		/* wait for initialization to complete */
	Sleep(1);
    if (*lpLock!=2)		/* if initialization failed, then error */
    {
	errno = 39;		/* ENOLCK (not using symbol because caller may not have errno.h )*/
	return -1;
    }
    return 0;
}

// inline function to protect the initialization of a data structure.
// Allows pointer to argument to be passed to the initialization routine.
inline int hsc_Protected_Initialize_Ex(volatile long* lpLock, int CALLBACK InitializationRoutine(void*), void* ArgList)
{
    if (*lpLock==0)		/* do faster cached memory check first */
    {
	if (InterlockedCompareExchange(lpLock,1,0)==0)
	{
	    if (InitializationRoutine(ArgList))
	    {
		InterlockedExchange(lpLock,0);
		return -1;
	    }
	    InterlockedExchange(lpLock,2);
	}
    }
    while (*lpLock==1)		/* wait for initialization to complete */
	Sleep(1);
    if (*lpLock!=2)		/* if initialization failed, then error */
    {
	errno = 39;		/* ENOLCK (not using symbol because caller may not have errno.h )*/
	return -1;
    }
    return 0;
}

// inline function to protect the termination of a data structure
inline int hsc_Protected_Terminate(volatile long* lpLock, int CALLBACK TerminationRoutine(void))
{
    if (*lpLock==2)		/* do faster cached memory check first */
    {
	    if (InterlockedCompareExchange(lpLock,1,2)==2)
	    {
	        if (TerminationRoutine())
	        {
    	        InterlockedExchange(lpLock,2);
   	    	    return -1;
	        }
   		    InterlockedExchange(lpLock,0);
	    }
    }
    while (*lpLock==1)		/* wait for termination to complete */
    	Sleep(1);
    if (*lpLock!=0)		/* if termination failed, then error */
    {
	    errno = 39;		/* ENOLCK (not using symbol because caller may not have errno.h )*/
	    return -1;
    }
    return 0;
}

// inline function to protect the termination of a data structure.
// Allows pointer to argument to be passed to the termination routine.
inline int hsc_Protected_Terminate_Ex(volatile long* lpLock, int CALLBACK TerminationRoutine(void*), void* ArgList)
{
    if (*lpLock==2)		/* do faster cached memory check first */
    {
	    if (InterlockedCompareExchange(lpLock,1,2)==2)
	    {
	        if (TerminationRoutine(ArgList))
	        {
    	        InterlockedExchange(lpLock,2);
   	    	    return -1;
	        }
   		    InterlockedExchange(lpLock,0);
	    }
    }
    while (*lpLock==1)		/* wait for termination to complete */
    	Sleep(1);
    if (*lpLock!=0)		/* if termination failed, then error */
    {
	    errno = 39;		/* ENOLCK (not using symbol because caller may not have errno.h )*/
	    return -1;
    }
    return 0;
}

#define MINIMISE_BASE_PRIORITY	0x0100	// Add this to our server priority values to
					// indicate the process|thread priority pair
					// should minimise the process priority instead
					// or maximising it

// function protoypes

static int		addkey _DECLARE((int, struct keydesc*, int));
EXTERN_C void		addtime _DECLARE((HSCTIME*, HSCTIME*));
EXTERN_C void		addtm  _DECLARE((HSCTIME*, long, int));
EXTERN_C void		addtm2 _DECLARE((HSCTIME*, long, long, int));
EXTERN_C void		AdjustToInterval _DECLARE((HSCTIME*, int4));
EXTERN_C void		alpha _DECLARE((char*, short));
EXTERN_C void		arshft _DECLARE((short*, short*, short*, short*));
EXTERN_C short		asc2date _DECLARE((char*, short));
EXTERN_C float		asc2time _DECLARE((char*, short));
EXTERN_C void		asctm  _DECLARE((HSCTIME*, char*, short));
EXTERN_C void		asctmraw  _DECLARE((HSCTIME*, char*, short));
EXTERN_C void		asctmz _DECLARE((HSCTIME*, short, char*, short));
EXTERN_C void		asctmd _DECLARE((HSCTIME*, char*, short));
EXTERN_C void		asctmt _DECLARE((HSCTIME*, char*, short));
EXTERN_C short		asctod _DECLARE((char*, short, short*, short*));
EXTERN_C short		asctodh _DECLARE((char*, short, short*, long*));
EXTERN_C short		asctof _DECLARE((char*, short, short*, short*, short*));
EXTERN_C short		asctoh _DECLARE((char*, short, short*, short*));
EXTERN_C short		asctoi _DECLARE((char*, short, short*, short*));
EXTERN_C short		asctoll _DECLARE((char*, short, short*, short*));
EXTERN_C short		asctoo _DECLARE((char*, short, short*, short*));
EXTERN_C short		asctop _DECLARE((char*, short, short*, short*, short*));
EXTERN_C int		ascv _DECLARE((char*, int, int, union values*, int, int));
EXTERN_C void		ascvrt _DECLARE((char*, short, short*, short*, short*, short*, short*));
static int		aud_chk _DECLARE((int));
EXTERN_C short		avail_lrn _DECLARE((void));
EXTERN_C void		binren _DECLARE((short*, short*, short*, short*));
EXTERN_C void		binwrn _DECLARE((short*, short*, short*, short*));
EXTERN_C void		bldpath _DECLARE((char*, char*, char*, char*, char*));
EXTERN_C struct dir*	bldtree _DECLARE((char*));
EXTERN_C void		brkds _DECLARE((void));
EXTERN_C short		brken _DECLARE((void));

#ifdef NT
EXTERN_C int		brkintr _DECLARE((unsigned long));
#else
EXTERN_C void		brkintr _DECLARE((void));
#endif

EXTERN_C int		bsortcmp _DECLARE((short*, short*));
EXTERN_C int		bsortdir _DECLARE((char*, struct directives*));
EXTERN_C int		bsortin _DECLARE((char*, char*, unsigned long, int*));
EXTERN_C int		bsortout _DECLARE((char*, char*, int, int));
EXTERN_C long		btestx _DECLARE((unsigned short*, unsigned short*));
EXTERN_C int2 STDCALL ACTIONW _DECLARE((int2*, int2*, int4*, int4*));
EXTERN_C void		CALLBACK c_almmsg_alarm _DECLARE((char*, int));
EXTERN_C void		CALLBACK c_almmsg_event _DECLARE((char*));
EXTERN_C void		CALLBACK c_almmsg_event_stn _DECLARE((char*,int2));
EXTERN_C void		CALLBACK c_almmsg_alarm_area _DECLARE((char*, int, char*));
EXTERN_C void		CALLBACK c_almmsg_event_area _DECLARE((char*, char*));
EXTERN_C void		CALLBACK c_almmsg_alarm_area_holder _DECLARE((char*, int, char*, int4));
EXTERN_C void		CALLBACK c_almmsg_event_area_holder _DECLARE((char*, char*, int4));
EXTERN_C char*		CALLBACK c_almmsg_format _DECLARE((char*, char*, char*, char*, char*, char*));
EXTERN_C char*		CALLBACK c_almmsg_format2 _DECLARE((char*, char*, char *, char*, char*, char*, char*));
EXTERN_C char*		CALLBACK c_almmsg_format2_malloc _DECLARE((char*, char*, char *, char*, char*, char*, char*));
EXTERN_C void		CALLBACK c_almmsg_format2_almevt _DECLARE((struct almevtfil *,char*, char*, char *, char*, char*, char*, char*, char*));
EXTERN_C int		CALLBACK c_bad _DECLARE((uint2));
EXTERN_C int		CALLBACK c_badpar _DECLARE((uint2, uint2));
EXTERN_C void		CALLBACK c_chrint _DECLARE((char*, int, int2*, int));
EXTERN_C int		CALLBACK c_copy _DECLARE((char*, char*, int));
EXTERN_C void		CALLBACK c_crttsk _DECLARE((char*, int, int, int, int2*));
EXTERN_C void STDCALL	DATAIO  _DECLARE((int2*,int2*,int2*,int2*,int2*,int2*,int2*,int2*,int2*));
EXTERN_C int4 STDCALL   DATAIO4 _DECLARE((int4*,int4*,int4*,int4*,int4*,int2*,int4*,int4*));
EXTERN_C void STDCALL	DATBIO  _DECLARE((int2*,int2*,int2*,int2*,int2*,int2*,int2*,int2*,int2*));
EXTERN_C int4 STDCALL   DATBIO4 _DECLARE((int4*,int4*,int4*,int4*,int4*,int2*,int4*,int4*));
EXTERN_C int4 STDCALL   DATWIO4 _DECLARE((int4*,int4*,int4*,int4*,int4*,int4*,int4*,int4*,int4*));
EXTERN_C int		CALLBACK c_dataio_close _DECLARE((int));
EXTERN_C int		CALLBACK c_dataio_dequeue _DECLARE((int, int, int2*, int));
EXTERN_C int		CALLBACK c_dataio_open _DECLARE((int));
EXTERN_C int		CALLBACK c_dataio_queue _DECLARE((int, int, int2*, int));
EXTERN_C int		CALLBACK c_dataio_reaabs _DECLARE((int, int, int, int2*, int));
EXTERN_C int		CALLBACK c_dataio_read _DECLARE((int, int, int, int2*, int));
EXTERN_C int		CALLBACK c_dataio_reaabs_blk _DECLARE((int, int, int, int, int2*, int));
EXTERN_C int		CALLBACK c_dataio_read_blk _DECLARE((int, int, int, int, int2*, int));
EXTERN_C int		CALLBACK c_dataio_read_newest _DECLARE((int, int*, int, int2*, int));
EXTERN_C int		CALLBACK c_dataio_read_oldest _DECLARE((int, int*, int, int2*, int));
EXTERN_C int		CALLBACK c_dataio_size _DECLARE((int, int*, int*));
EXTERN_C int		CALLBACK c_dataio_write _DECLARE((int, int, int, int2*, int));
EXTERN_C int		CALLBACK c_dataio_write_blk _DECLARE((int, int, int, int, int2*, int));
EXTERN_C int		CALLBACK c_dataio_write_newest _DECLARE((int, int*, int, int2*, int));
EXTERN_C int2		CALLBACK c_datclo _DECLARE((int));
EXTERN_C int2		CALLBACK c_datopn _DECLARE((int*, char*, int));
EXTERN_C int		CALLBACK c_deltsk _DECLARE((int));
EXTERN_C int		CALLBACK c_devget_by_lrn _DECLARE((int, struct devtbl*));
EXTERN_C int		CALLBACK c_devget_by_name _DECLARE((char*, struct devtbl*));
EXTERN_C int		CALLBACK c_devput_by_lrn _DECLARE((int, struct devtbl*));
EXTERN_C int		CALLBACK c_devput_by_name _DECLARE((char*, struct devtbl*));
EXTERN_C int2		c_dsphtml( int2 , char * );
EXTERN_C int		CALLBACK c_ex _DECLARE((char*));
EXTERN_C int		CALLBACK c_ex_nowait _DECLARE((char*));
EXTERN_C int		CALLBACK c_ex_full _DECLARE((char*, int));
EXTERN_C int		CALLBACK c_gbload _DECLARE((void));
EXTERN_C int		CALLBACK c_gbunload _DECLARE((void));
EXTERN_C int		CALLBACK c_gdbcnt _DECLARE((int2 *, int2 *, int2 *, int2 *, int2 *, double *));
EXTERN_C int2		CALLBACK c_getapp _DECLARE((char*, int, struct apptbl*));
EXTERN_C int		CALLBACK c_getch _DECLARE((uint2, uint2, char*, int));
EXTERN_C uint2		CALLBACK c_getht _DECLARE((int, int2, float, int, int, uint2, char*, float*, int2*));
EXTERN_C int		CALLBACK c_geti2 _DECLARE((uint2, uint2, int2*));
EXTERN_C int		CALLBACK c_geti4 _DECLARE((uint2, uint2, int4*));
EXTERN_C int		CALLBACK c_getrl _DECLARE((uint2, uint2, float*));
EXTERN_C int		CALLBACK c_getdat _DECLARE((int, int, int, int, int, int, int2*, int));
EXTERN_C int		CALLBACK c_gethst_date _DECLARE((int, int, double, int, uint2*, int, char*, float*));
EXTERN_C int		CALLBACK c_gethst_ofst _DECLARE((int, int, int, uint2*, int, char *, float *));
EXTERN_C int		CALLBACK c_gethstpar_date _DECLARE((int, int, double, int, uint2*, uint2*, int, char*, float*));
EXTERN_C int		CALLBACK c_gethstpar_ofst _DECLARE((int, int, int, uint2*, uint2*, int, char*, float*));
EXTERN_C int		CALLBACK c_getlrn _DECLARE((void));
EXTERN_C void		CALLBACK c_getlst _DECLARE((int2, float*, int2*));
EXTERN_C int		CALLBACK c_getpar _DECLARE((int, char*, int));
EXTERN_C int		CALLBACK c_getpnt _DECLARE((char*, uint2*));
EXTERN_C int2		CALLBACK c_getprm _DECLARE((int, int2*, int2*,int));
EXTERN_C int		CALLBACK c_getreq _DECLARE((int2 *));
EXTERN_C int		CALLBACK c_getval_asci _DECLARE((uint2, int, char*, int));
EXTERN_C int		CALLBACK c_getval_hist _DECLARE((uint2, int, int, float*));
EXTERN_C int		CALLBACK c_getval_numb _DECLARE((uint2, int, float*));
EXTERN_C void		CALLBACK c_givlst _DECLARE((int2, float*, int2*));
EXTERN_C int		CALLBACK c_gtoj _DECLARE((int, int, int));
EXTERN_C void		CALLBACK c_intchr _DECLARE((int2*, int, char*, int));
EXTERN_C void		CALLBACK c_jtog _DECLARE((int, int*, int*, int*));
EXTERN_C int		c_ll_cleanup _DECLARE((void));
EXTERN_C void		c_ll_getbypar _DECLARE((short*, short*, char*, short, char*, short));
EXTERN_C char*		c_ll_getstr _DECLARE((int, int, char*));
EXTERN_C int		CALLBACK c_lock_dir _DECLARE((int, int));
EXTERN_C int		CALLBACK c_lock_file _DECLARE((int, int, int));
EXTERN_C int		CALLBACK c_lock_rec _DECLARE((int, int, int, int));
EXTERN_C void		c_logmsg _DECLARE((char*, char*, char*, ...));
EXTERN_C int2		CALLBACK c_mkdir _DECLARE((char *path, int2, int));
EXTERN_C int		CALLBACK c_mmio _DECLARE((int, int, int, int, int, int, int2*, int));
EXTERN_C int		CALLBACK c_mmio_close _DECLARE((int));
EXTERN_C int		CALLBACK c_mmio_getoid _DECLARE((int, int, char*, int*));
EXTERN_C int		CALLBACK c_mmio_open _DECLARE((int));
EXTERN_C int		CALLBACK c_mmio_oprread _DECLARE((int, int, int2*, int));
EXTERN_C int		CALLBACK c_mmio_read _DECLARE((int, int, int, int, int, int2*, int));
EXTERN_C int		CALLBACK c_mmio_write _DECLARE((int, int2*, int));
EXTERN_C int		CALLBACK c_mmio2 _DECLARE((int, int, int, int, int, int, int, int2*, int));
EXTERN_C int		CALLBACK c_mmio2_read _DECLARE((int, int, int, int, int, int, int2*, int));
EXTERN_C int		CALLBACK c_mmio2_write _DECLARE((int, int, int2*, int));
EXTERN_C uint2		CALLBACK c_morerq _DECLARE((void));
EXTERN_C int		CALLBACK c_mzero _DECLARE((float *));
EXTERN_C int		CALLBACK c_oprerr _DECLARE((int, int));
EXTERN_C int		CALLBACK c_oprstr_info _DECLARE((int, char*));
EXTERN_C int		CALLBACK c_oprstr_message _DECLARE((int, char*));
EXTERN_C int		CALLBACK c_oprstr_prompt _DECLARE((int, char*, int));
EXTERN_C int		CALLBACK c_oprstr_prompth _DECLARE((int, char*, int));
EXTERN_C int		CALLBACK c_oprstr_promptyn _DECLARE((int, char*, int));
EXTERN_C char*		CALLBACK c_oprstr_response _DECLARE((int, struct prm*));
EXTERN_C int		CALLBACK c_pps _DECLARE((uint2, int, int*));
EXTERN_C int		CALLBACK c_ppsw _DECLARE((uint2, int, int*));
EXTERN_C int		CALLBACK c_ppv _DECLARE((uint2, int, double));
EXTERN_C int		CALLBACK c_ppvw _DECLARE((uint2, int, double));
EXTERN_C int		CALLBACK c_prsend_crt _DECLARE((int, char *));
EXTERN_C int		CALLBACK c_prsend_printer _DECLARE((int, char *));
EXTERN_C int		CALLBACK c_putch _DECLARE((uint2, uint2, char*));
EXTERN_C int		CALLBACK c_puthst_date _DECLARE((int, int, double, int, uint2*, uint2*, int, char*, float*));
EXTERN_C int		CALLBACK c_puthst_ofst _DECLARE((int, int, int, uint2*, uint2*, int, char*, float*));
EXTERN_C int		CALLBACK c_puti2 _DECLARE((uint2, uint2, int2));
EXTERN_C int		CALLBACK c_puti4 _DECLARE((uint2, uint2, int4));
EXTERN_C int		CALLBACK c_putrl _DECLARE((uint2, uint2, double));
EXTERN_C int		CALLBACK c_putdat _DECLARE((int, int, int, int, int, int, int2*, int));
EXTERN_C int		CALLBACK c_putval_asci _DECLARE((uint2, int, char*));
EXTERN_C int		CALLBACK c_putval_hist _DECLARE((uint2, int, int, double));
EXTERN_C int		CALLBACK c_putval_numb _DECLARE((uint2, int, double));
EXTERN_C int		c_putval_numb_ext _DECLARE((uint2, int, double, int));
EXTERN_C int		CALLBACK c_rm _DECLARE((char*, int));
EXTERN_C uint2		CALLBACK c_rqcl _DECLARE((int2*, int, int));
EXTERN_C void		CALLBACK c_rqstsk _DECLARE((int, int, uint2*, int2, int2*, int));
EXTERN_C int2           CALLBACK c_rqtskb _DECLARE((int));
EXTERN_C int2           CALLBACK c_rqtskb_prm _DECLARE((int, int2*));
EXTERN_C int		CALLBACK c_sps _DECLARE((uint2, int));
EXTERN_C int		CALLBACK c_spsw _DECLARE((uint2, int));
EXTERN_C int		CALLBACK c_spv _DECLARE((uint2, int, double));
EXTERN_C int		CALLBACK c_spvw _DECLARE((uint2, int, double));
EXTERN_C int		CALLBACK c_stcupd _DECLARE((int, int));
EXTERN_C int		CALLBACK c_tmstrt_single _DECLARE((int, int, int));
EXTERN_C int		CALLBACK c_tmstrt_cycle _DECLARE((int, int, int));
EXTERN_C void		CALLBACK c_tmstop _DECLARE((int));
EXTERN_C void		CALLBACK c_trm04 _DECLARE((int2));
EXTERN_C void		CALLBACK c_trmtsk _DECLARE((int2));
EXTERN_C int		CALLBACK c_tstskb _DECLARE((int));
EXTERN_C int		CALLBACK c_unlock_dir _DECLARE((int));
EXTERN_C int		CALLBACK c_unlock_file _DECLARE((int, int));
EXTERN_C int		CALLBACK c_unlock_rec _DECLARE((int, int, int));
EXTERN_C void		CALLBACK c_upper _DECLARE((char*));
EXTERN_C int		CALLBACK c_wdon _DECLARE((int));
EXTERN_C int		CALLBACK c_wdstrt _DECLARE((int, int));
EXTERN_C int2		CALLBACK c_wttskb _DECLARE((int));
EXTERN_C int		CALLBACK isdbmodified _DECLARE((void));
static int		check _DECLARE((int));
EXTERN_C int		check_arg _DECLARE((char*));
EXTERN_C int		checktapedrv();
EXTERN_C int		chn_lock _DECLARE((int, int*));
EXTERN_C void		chn_unlock _DECLARE((int, int));
EXTERN_C void		chrint	_DECLARE((char*, short, short*, short*));
EXTERN_C void		cl  _DECLARE((short*, short*));
EXTERN_C void		cl4 _DECLARE((short*, long*));
EXTERN_C void		ClearScreen _DECLARE((void));
EXTERN_C int		close_ok _DECLARE((int*));

#ifdef NT
EXTERN_C void		closedir _DECLARE((HANDLE));
#endif

EXTERN_C int		cmdxmt _DECLARE((char*));
EXTERN_C void		cmpres _DECLARE((short*, short*, short*, short*));
EXTERN_C int		cmr _DECLARE((char*, int));
EXTERN_C int		cnt_tmout _DECLARE((unsigned int, char*, int));
EXTERN_C void		comerr _DECLARE((short*, short*, short*, short*));
EXTERN_C void		comerupd _DECLARE((struct rtutbl*, int, int, int, int));
EXTERN_C short		compar _DECLARE((short*, short*));
EXTERN_C int		hsc_compare   _DECLARE((char*,int,char*,int));
EXTERN_C int		hsc_compare_i _DECLARE((char*,int,char*,int));
EXTERN_C void		compress _DECLARE((short*, short*, short*, short*, short));
static int		confirm _DECLARE((struct dictinfo*, struct mkeydesc*, char));
EXTERN_C int		convpath _DECLARE((char*));
static long		count _DECLARE((int));
EXTERN_C uint2		crc _DECLARE((int, unsigned char *, int));
EXTERN_C unsigned short	crc16 _DECLARE((unsigned char*, int, unsigned short));
EXTERN_C short		crgtsk _DECLARE((char*, short, short*, short*, char*, short, short*));
EXTERN_C int		crgtskxmt _DECLARE((char*, short, short, short, char*, short));
EXTERN_C short		crtasn _DECLARE((short*, short*));
EXTERN_C short		crtasnperm _DECLARE((short*, short*));
EXTERN_C short		crtasnperm_ex _DECLARE((short*, short*, short*));
EXTERN_C short		crtamp _DECLARE((short*, short*));
EXTERN_C short		crtiot _DECLARE((short*, short*));
EXTERN_C void		crttsk _DECLARE((char*, short, short*, short*, short*, short*, short*));
EXTERN_C void		ctofstr _DECLARE((char*, char*, int));
EXTERN_C int		cvpath _DECLARE((char*, short));
EXTERN_C short		cwd _DECLARE((char*, short, short*));
EXTERN_C int		d_sock _DECLARE((unsigned short, int));
EXTERN_C int		d_sock_range _DECLARE((unsigned short, unsigned short, int));
EXTERN_C int		dat_block_read _DECLARE((long, long, long, char*, unsigned long));
EXTERN_C int		dat_block_write _DECLARE((long, long, long, char*, unsigned long));
EXTERN_C int		dat_file_open _DECLARE((short));
EXTERN_C void		datclo _DECLARE((short*, short*));
EXTERN_C long		datclo4 _DECLARE((long*));
EXTERN_C void		datcre _DECLARE((short*, char*, short, short*, short*));
EXTERN_C long		datcre4 _DECLARE((long*, char*, short, long*));
EXTERN_C short		datcreate _DECLARE((char*, short, char*, short));
EXTERN_C void		datdel _DECLARE((short*, char*, short, short*));
EXTERN_C long		datdel4 _DECLARE((char*, short));
EXTERN_C void		date _DECLARE((long*));
EXTERN_C void		dateof _DECLARE((short*, short*));
EXTERN_C FILETIME	DateToFiletime _DECLARE((DATE));
EXTERN_C void		DateToHsctime _DECLARE((DATE*,HSCTIME*));
EXTERN_C void		datget _DECLARE((short*, char*, short, short*, short*));
EXTERN_C long		datget4 _DECLARE((long*, char*, short, long*));
EXTERN_C void		datini _DECLARE((void));
EXTERN_C void		datopn _DECLARE((short*, char*, short, unsigned short*, short*));
EXTERN_C int4		datopn4 _DECLARE((long*, char*, short, unsigned long*));
EXTERN_C void		datopr _DECLARE((short*, char*, short, short*, short*));
EXTERN_C long		datopr4 _DECLARE((long*, char*, short, long*));
EXTERN_C void		datrea _DECLARE((short*, long*, short*, short*));
EXTERN_C long		datrea4 _DECLARE((long*, long*, short*));
EXTERN_C void		datren _DECLARE((short*, short*, short*, short*));
EXTERN_C long		datren2 _DECLARE((long*, long*, short*, BOOL));
EXTERN_C long		datren4 _DECLARE((long*, long*, short*));
EXTERN_C void		datrew _DECLARE((short*, short*));
EXTERN_C long		datrew4 _DECLARE((long*));
EXTERN_C void		datrmv _DECLARE((short*, short*));
EXTERN_C long		datrmv4 _DECLARE((long*));
EXTERN_C void		datrnm _DECLARE((char*, short, char*, short, short*));
EXTERN_C void		datwri _DECLARE((short*, long*, short*, short*));
EXTERN_C long		datwri4 _DECLARE((long*, long*, short*));
EXTERN_C void		datwrn _DECLARE((short*, short*, short*, short*));
EXTERN_C void		datwrt _DECLARE((short*, long*, short*, short*));
EXTERN_C long		datwrt4 _DECLARE((long*, long*, short*));
EXTERN_C long		datxmt _DECLARE((long*, long*, long*, long*, short*, long*, long*));
EXTERN_C long		datxmtex _DECLARE((long*, long*, long*, long*, short*, long*, long*, long*));
EXTERN_C int		db _DECLARE((void));
EXTERN_C short		db_lock _DECLARE((short*, short*, short*, short*));
EXTERN_C short		db_lockint		_DECLARE((short*));
EXTERN_C short		db_lockt _DECLARE((short*, short*, short*));
EXTERN_C short		db_unlock _DECLARE((short*, short*, short*));
EXTERN_C int		dbloaded _DECLARE((void));
EXTERN_C short		dbghr _DECLARE((short*, short*, short*));
EXTERN_C int		defaultrelease _DECLARE((void));
EXTERN_C short		deffnd _DECLARE((unsigned short*, short*, short*));
EXTERN_C int		delcchr _DECLARE((char*, int, char, char));
EXTERN_C int		delchr _DECLARE((char*, int, char));
static int		deldefn _DECLARE((int));
static int		deleted _DECLARE((long));
static int		delfree _DECLARE((long));
EXTERN_C void		DeltaFiletimeToHsctime _DECLARE((FILETIME*, HSCTIME*));
EXTERN_C void		DeltaHsctimeToFiletime _DECLARE((HSCTIME*, FILETIME*));
EXTERN_C void		DeltaHsctimeToRawtm _DECLARE((HSCTIME*, int4*));
EXTERN_C void		DeltaRawtmToHsctime _DECLARE((int4*, HSCTIME*));
EXTERN_C long		deltm _DECLARE((HSCTIME*, HSCTIME*));
EXTERN_C LONGLONG	deltm2 _DECLARE((HSCTIME*, HSCTIME*, int2));
EXTERN_C short		cmptm _DECLARE((HSCTIME*, HSCTIME*));
EXTERN_C struct dir*	deltree _DECLARE((struct dir*));
EXTERN_C short		deltsk _DECLARE((short*));
EXTERN_C short		devfmt _DECLARE((char*, short, short*));
EXTERN_C struct devtbl*	devget_lnk _DECLARE((int));
static int		dexpand _DECLARE((void));
EXTERN_C long		dget _DECLARE((short*));
EXTERN_C int		dhlww _DECLARE((short*, short*, short*, short*, short*));
EXTERN_C int		dhsf _DECLARE((short*, short*, short*, short*, short*));
EXTERN_C long		dir_downgrade _DECLARE((long*, unsigned short*, long*));
EXTERN_C long		dir_read _DECLARE((long*, long*, long*, long*, short*, long*));
EXTERN_C long		dir_size _DECLARE((short*));
EXTERN_C long		dir_type _DECLARE((short*,long*));
EXTERN_C long		dir_upgrade _DECLARE((unsigned short*, long*, long*, long*, long*));
EXTERN_C long		dir_write _DECLARE((long*, long*, long*, long*, long*));
EXTERN_C int		dl_psdt _DECLARE((void));
EXTERN_C short		dlgtsk _DECLARE((char*, short, short*));
EXTERN_C int		dlgtskxmt _DECLARE((char*, short, short));
EXTERN_C int		dll_init _DECLARE((void*, int, int));
EXTERN_C int		dllmain _DECLARE((void*, int, int));
static int		do_aud _DECLARE((char*, char*, long));
EXTERN_C int		do_open _DECLARE((char*, int, int, short, short*, int*));
EXTERN_C short		dps6now _DECLARE((char*, short));
EXTERN_C void		dput _DECLARE((short*, long*));
EXTERN_C void		dsanod _DECLARE((char*, short));
EXTERN_C void		dspalm _DECLARE((short*, short*, short*, long*, short*, short*, char*, short, short*, long*));
EXTERN_C short		dspatr _DECLARE((short*, short*, short*));
EXTERN_C void		dspchk _DECLARE((short*, short*, short*, long*, short*, char*, short, short*));
EXTERN_C void		dspind _DECLARE((short*, short*, short*, long*, short*, float*, float*, float*, float*, float*));
EXTERN_C short		dsply_lrn _DECLARE((short*));
EXTERN_C void		dspmsg _DECLARE((short*, short*, short*, long*, short*, short*, char*, short, short*));
EXTERN_C int		dspnamcmp _DECLARE((char*, int, char*, int));
EXTERN_C int		dspnamncmp _DECLARE((char*, int, char*, int, int));
EXTERN_C void		dsppag _DECLARE((short*,short*,char*,short));
EXTERN_C int		dsppath _DECLARE((void));
EXTERN_C void		dsppb _DECLARE((short*, short*, short*, long*, short*, char*, short));
EXTERN_C void		dspscl _DECLARE((short*, short*, short*, long*, short*, float*, float*, float*, short*));
EXTERN_C void		dsptxt _DECLARE((short*, short*, short*, long*, short*, char*, short));
EXTERN_C int4		dsupdate _DECLARE((int4, int4, int4, int4, int2 *));
EXTERN_C short		dtoasc _DECLARE((char*, short, short*, short*));
EXTERN_C short		dumpen _DECLARE((void));
EXTERN_C short		dvclrn _DECLARE((char*, short));
EXTERN_C short		eauout _DECLARE((char*, short));
EXTERN_C int		eouout _DECLARE((int, char*, short));
EXTERN_C void		epsnrm _DECLARE((short*, short*));
EXTERN_C short		equal _DECLARE((char*, short, char*, short));
EXTERN_C short		equal_i _DECLARE((char*, short, char*, short));
EXTERN_C void		errmsg _DECLARE((int, int,...));
EXTERN_C short		errout _DECLARE((char*, short));
EXTERN_C short		evarchrcv _DECLARE((int,int,char*));
EXTERN_C short		evarchxmt _DECLARE((int,int,char*));
EXTERN_C void		ex _DECLARE((short*, char*, short, short*, short*));
static int		extract _DECLARE((char*, char*, char*, int));
EXTERN_C void		favg _DECLARE((float*, float*, float*));
EXTERN_C float		fclamp _DECLARE((float*, float*));
EXTERN_C float		fcpuse _DECLARE((short*));
EXTERN_C void		fdec _DECLARE((float*));
EXTERN_C float		fget _DECLARE((short*));
EXTERN_C DATE		FiletimeToDate _DECLARE((FILETIME));
EXTERN_C void		FiletimeToHsctime _DECLARE((FILETIME*, HSCTIME*));
EXTERN_C short		filinf _DECLARE((char*, short, char*, short));
EXTERN_C void		fill _DECLARE((char*, short, char*, short, short*));
EXTERN_C short		filrea _DECLARE((short*));
EXTERN_C short		filwrt _DECLARE((short*, short*));
EXTERN_C short		filxmt _DECLARE((short*, int2));
EXTERN_C void		finc _DECLARE((float*));
EXTERN_C long		find_eol_rm_esc _DECLARE((char*, int, char*, short*));
EXTERN_C struct rtu_pad_data* find_pad _DECLARE((struct rtu_pad_data**, int));
EXTERN_C struct rtu_hxr_data* find_rtu _DECLARE((struct rtu_hxr_data**, int));
EXTERN_C short		findarea _DECLARE((char*, short));
EXTERN_C int		findudcpar _DECLARE((int*, int*, int*, int*, int*));
static int		fixkey _DECLARE((void));
EXTERN_C void		flush_chn _DECLARE((int));
EXTERN_C int		forktsk _DECLARE((char*, int, int, char**, int));
EXTERN_C void		fput _DECLARE((short*, float*));
EXTERN_C int		freestorage _DECLARE((char**, unsigned int*));
static int		frkey _DECLARE((char**, char**, int, int));
EXTERN_C void		fromieee _DECLARE((short*, short*));
EXTERN_C void		fromlong		_DECLARE((short*, short*));
EXTERN_C short		fssize _DECLARE((char*, short, float*, float*));
EXTERN_C short		ftoasc _DECLARE((char*, short, short*, short*, short*));
EXTERN_C char*		ftocstr _DECLARE((char*, int, char*, int));
EXTERN_C void           ftofstr _DECLARE((char *, int, char *, int));
EXTERN_C int		g_par _DECLARE((int, int));
EXTERN_C int		gbaccess _DECLARE((void*,void*));
EXTERN_C char*		gbattach _DECLARE((int, char*, int));
EXTERN_C int		gbattach_nt _DECLARE((int));
EXTERN_C int		gbunattach_nt _DECLARE((void));
EXTERN_C int		gbld _DECLARE((void));
EXTERN_C void		gbload _DECLARE((short*));
EXTERN_C void		gbunload _DECLARE((short*));
EXTERN_C void		gbunload_nt _DECLARE((short*));
EXTERN_C void		gbload_nt _DECLARE((short*, int));
EXTERN_C int		get_defsem_semid _DECLARE((void));
EXTERN_C int		get_defsem_semno _DECLARE((short));
EXTERN_C int		get_file_no _DECLARE((char*));
EXTERN_C int		get_free_semnum _DECLARE((void));
EXTERN_C unsigned int	get_hrl_sz _DECLARE((short*, unsigned int, unsigned int*));
EXTERN_C int		get_lic_size _DECLARE((int, int, int*));
EXTERN_C int		get_rec_len _DECLARE((char*, int, unsigned long*));
static struct keydesc*	getblank _DECLARE((void));
EXTERN_C void		getbyt _DECLARE((short*, short*, short*));
EXTERN_C void		getcom _DECLARE((short*, long*, short*, short*));
EXTERN_C short		getdat  _DECLARE((short*, short*, short*, short*, short*, unsigned short*, unsigned short*, short*));
EXTERN_C long		getdat4 _DECLARE((long*, long*, long*, long*, long*, long*, unsigned short*, long*));
EXTERN_C int		getdbkey _DECLARE((void));
EXTERN_C void		getfld _DECLARE((char*, short, char*, short, short*));
EXTERN_C void           getfld2 _DECLARE((char*, short, char*, short, short*, short*));

#ifdef NT
EXTERN_C short		getgid _DECLARE((void));
#endif

EXTERN_C struct hostent* gethost _DECLARE((char*));
EXTERN_C void		gethst _DECLARE((short*, short*, float*, short*, short*, short*, char*, short, long*, short*));
EXTERN_C void		gethstpar _DECLARE((short*, short*, float*, short*, short*, short*, short*, char*, short, long*, short*));
EXTERN_C void		getht _DECLARE((short*, short*, float*, short*, short*, short*, char*, short, long*, short*, short*));
EXTERN_C short		getlrn _DECLARE((void));
EXTERN_C void		getFullPriority _DECLARE((int4, int4*, int4*));
EXTERN_C void		GetLocDateTime _DECLARE((HSCTIME* time, int2 bias, char* szDateTime, int2 strlen, int2 includeTime, int2 includeUS, int2 includeMS));
EXTERN_C void		GetLocDateTimeEx _DECLARE((HSCTIME* time, int2 bias, char* szDateTime, int2 strlen, int2 datetimeFunction, int2 includeMS, int2 includeUS, int2 showAMPM, int2* datetimeSeparator));
EXTERN_C void		GetMilliSecsFromHsctime _DECLARE((HSCTIME*, int2*));
EXTERN_C void		GetMicroSecsFromHsctime _DECLARE((HSCTIME*, int4*));
EXTERN_C void		GetDatePartFromHsctime _DECLARE((HSCTIME* pHscTime, HSCTIME* pHscDate));
EXTERN_C void		GetTimePartFromHsctime _DECLARE((HSCTIME* pHscTime, HSCTIME* pHscDate));
EXTERN_C int2 STDCALL	GETPAG _DECLARE((int4*, int4*, int4*, int4*, int4*, char*, int2));
EXTERN_C short		getprm _DECLARE((short*, short*, short*, short*));
EXTERN_C short		getreq _DECLARE((short*));
EXTERN_C short		getrid _DECLARE((short*));
EXTERN_C struct rtu_hxr_data* getrtu_hxr _DECLARE((int, int, int));
EXTERN_C struct rtu_pad_data* getrtu_pad _DECLARE((int, int));
EXTERN_C int2		GetServerAPIerrno _DECLARE((void));
EXTERN_C int		getstorage _DECLARE((char**, unsigned int*, unsigned int));
EXTERN_C void		gettm  _DECLARE((HSCTIME*));
EXTERN_C int		gettmz _DECLARE((HSCTIME*));
EXTERN_C short		getval _DECLARE((short*, uint2*, uint2*, float*, ...));
EXTERN_C void		getvol _DECLARE((char*, char*, short*, long*, long*));
EXTERN_C char*		ghr_rd_rec _DECLARE((int, int, unsigned int*));
EXTERN_C int		ghr_rd_rtu _DECLARE((struct rtu_hxr_data*));
EXTERN_C short		gifil _DECLARE((char*, short, short*, short*));
EXTERN_C int4		gpmain _DECLARE((int4*, int4*, int4*, int4*, int2*, int4*, int4*, int4*, int4*, int4*));
EXTERN_C int4 STDCALL	GPMAIN _DECLARE((int4*, int4*, int4*, int4*, int2*, int4*, int4*, int4*, int4*, int4*));
EXTERN_C void		gregor _DECLARE((long*, short*));
EXTERN_C int 		hsc_AccessConfigLogin _DECLARE((char *, char *));
EXTERN_C HRESULT        hsc_add_comment _DECLARE((short, long, int, __int64, char*, char*, char*, BSTR, BOOL, __int64*, int*, BSTR*, long*, short*));
EXTERN_C int2		hsc_AdjustZone _DECLARE((int2 *, int2 *));
EXTERN_C int		hsc_area_desc _DECLARE((int, char*, int));
EXTERN_C int		hsc_area_name _DECLARE((int, char*, int));
EXTERN_C int		hsc_area_number _DECLARE((char*));
EXTERN_C int		hsc_asset_to_area_number _DECLARE((char*));
EXTERN_C int		hsc_callup_display _DECLARE((int2, char*));
EXTERN_C int2       hsc_alarm2point _DECLARE((int4 *));
EXTERN_C PNTNUM		hsc_event2point _DECLARE((struct almevtfil *));
EXTERN_C int		hsc_nav_event_assoc_display _DECLARE((int2, struct almevtfil *));
EXTERN_C int		hsc_nav_event_cardholder_detail _DECLARE((int2, struct almevtfil *));
EXTERN_C HRESULT STDCALL HSC_CoInitialize _DECLARE((void*));
EXTERN_C HRESULT STDCALL HSC_CoInitializeEx _DECLARE((void*, DWORD));
EXTERN_C void    STDCALL HSC_CoUninitialize _DECLARE((void));
EXTERN_C HRESULT        hsc_connect_to_eventsdc _DECLARE((int));
EXTERN_C int		hsc_nav_event_detail_display _DECLARE((int2, struct almevtfil *));
EXTERN_C int		hsc_nav_point_detail_display _DECLARE((int2, PNTNUM));
EXTERN_C int		hsc_nav_tag_detail_display _DECLARE((int2, char*, int));
EXTERN_C int		hsc_nav_point_assoc_display _DECLARE((int2, PNTNUM));
EXTERN_C int            hsc_delete_rm _DECLARE((int));
EXTERN_C int            hsc_config_rm _DECLARE((int, int, int));
EXTERN_C int 	        hsc_convert_to_valid_area _DECLARE((int area));
EXTERN_C int 	        hsc_ErrorToQuality _DECLARE((int code));
EXTERN_C int 	        hsc_GdaErrorToQuality _DECLARE((GDAERR code));
EXTERN_C int		hsc_FormProgIdList(int iType, char* szServerName, int timeout, enumlist** ppList);
EXTERN_C int		hsc_ResolveOPCProgID(WCHAR *wszProgId, WCHAR *wszServerName, LPCLSID pClsid);
EXTERN_C int		hsc_OPCenum_OnOPCServerNameChange(int2 recnum, int2 wordnum);
EXTERN_C int2           hsc_GetAcrnym _DECLARE(( struct Location *, char *));
EXTERN_C int2           hsc_GetCrtUser _DECLARE(( int , char * ));
EXTERN_C HRESULT        hsc_get_comments _DECLARE((short, long, int, __int64, __int64*, int*, BSTR*, long*, short*));
EXTERN_C char*		hsc_getwsaerrtext _DECLARE((unsigned int));
EXTERN_C int		hsc_history2 _DECLARE((int, int, HSCTIME*, int, int, PNTNUM*, PRMNUM*, char*, HSCTIME*, int2*, float*, int4*));
EXTERN_C int2       hsc_InsertSourceEntityName _DECLARE((int2 * pEvtBuffer, PNTNUM pointNumber));
EXTERN_C int2       hsc_InsertLocation _DECLARE((int2 * pEvtBuffer, PNTNUM pointNumber, PNTNUM parentNumber));
EXTERN_C int2		hsc_isLocalHost _DECLARE((int2));
EXTERN_C int		hsc_IsUrl _DECLARE((char*));
EXTERN_C int		hsc_IsHmiUrl _DECLARE((char*));
EXTERN_C int		hsc_IsName _DECLARE((char*, int));
EXTERN_C int            hsc_IsLocationWritable _DECLARE((short, short, short, short, short, short));
EXTERN_C int		hsc_isValidName _DECLARE((char*, int));
EXTERN_C int		hsc_isValidPointName _DECLARE((char*, int));
EXTERN_C int		hsc_makeValidPointName(char* pName, char cRep, int NameLen);
EXTERN_C int		hsc_isValidParamName _DECLARE((uint2, char*, int));
EXTERN_C int		hsc_isValidEntityName _DECLARE((char*, int, uint2));
EXTERN_C int		hsc_procexists _DECLARE((char*));
EXTERN_C BOOL STDCALL	hsc_redirectlog_init _DECLARE(());
EXTERN_C void STDCALL	hsc_redirectlog_close _DECLARE(());
EXTERN_C void		hsc_refresh_crts _DECLARE((int2*));
EXTERN_C int		hsc_release _DECLARE((int));
EXTERN_C int2 	        hsc_get_system_area_number _DECLARE(());
EXTERN_C int            hsc_q_getnextitemsize _DECLARE((int, int*));
EXTERN_C int            hsc_q_getnumitems _DECLARE((int, int*));
EXTERN_C int            hsc_q_getusage _DECLARE((int));
EXTERN_C int            hsc_q_init _DECLARE((int, int, int));
EXTERN_C int            hsc_q_open _DECLARE((int, int, int));
EXTERN_C int            hsc_q_openex _DECLARE((int, int, int, int));
EXTERN_C int            hsc_q_read _DECLARE((int, char*, int));
EXTERN_C void           hsc_q_remove _DECLARE((int));
EXTERN_C int            hsc_q_reset _DECLARE((int, int, int));
EXTERN_C int            hsc_q_write _DECLARE((int, char*, int));
EXTERN_C void		hsc_SetSecurityLevel4CRT _DECLARE((int2, int2, int2));
EXTERN_C BOOL		hsc_Server_LoadParanoidEnv _DECLARE((void));
EXTERN_C int 		hsc_Set_CMS_Access _DECLARE((void));
EXTERN_C void		hsc_set_locale _DECLARE((void));
EXTERN_C BOOL		hsc_SetPrivilege _DECLARE((HANDLE, char*, BOOL));
EXTERN_C int2		hsc_sort_areas _DECLARE((void));
EXTERN_C void       hsc_Sound_Audible_By_Area _DECLARE((int2, int2, int2, HSCTIME));
EXTERN_C void       hsc_Silence_Audible_By_Crt _DECLARE((int));
EXTERN_C void       hsc_Silence_Audible_All _DECLARE((void));
EXTERN_C void       hsc_Silence_Audible_All_From_Below _DECLARE((void));
EXTERN_C void       hsc_Silence_Audible_All_From_AckSync _DECLARE((HSCTIME));
EXTERN_C void       hsc_Silence_Audible_All_By_Area _DECLARE((int2));
EXTERN_C void       hsc_silence_crt _DECLARE((int2));
EXTERN_C int		hsc_task_ident _DECLARE((int,int,char*,int));
EXTERN_C int		hsc_my_task_ident _DECLARE((char*,int));
EXTERN_C void 		hsc_timer_begin _DECLARE((uint4*));
EXTERN_C int 		hsc_timer_end _DECLARE((uint4*, uint4));
EXTERN_C int		hsc_uselog _DECLARE((void));
EXTERN_C int2		hsc_min_since_midnight _DECLARE((void));
EXTERN_C int		hsc_validateRaiseStep _DECLARE((void));
EXTERN_C void		hsc_FormatUnixErrMsg _DECLARE((int errcode, char *out_msg, int bufsize));
EXTERN_C void		HsctimeToDate _DECLARE((HSCTIME*,DATE*));
EXTERN_C void		HsctimeToDateEx _DECLARE((HSCTIME*, int2, DATE*));
EXTERN_C void		HsctimeToFiletime _DECLARE((HSCTIME*, FILETIME*));
EXTERN_C void		HsctimeToLocalHsctime _DECLARE((HSCTIME*, HSCTIME*));
EXTERN_C void		HsctimeToRawtm _DECLARE((HSCTIME*, int4*));
EXTERN_C void       HsctimeToSeconds _DECLARE((HSCTIME*, double*));
EXTERN_C void       HsctimeToInt8 _DECLARE((HSCTIME*, int8*));
EXTERN_C void       hstavg _DECLARE((short*, short*, long*, long*));
EXTERN_C void		hstclr _DECLARE((short*, long*));
EXTERN_C short		hstxmt _DECLARE((short*, short*, long*));
EXTERN_C short		htoasc _DECLARE((char*, short, short*, short*));
EXTERN_C int		hwi_chkp _DECLARE((struct rtu_hxr_data*));
EXTERN_C int2		i2add _DECLARE((short*, short));
EXTERN_C int2		i2dec _DECLARE((short*));
EXTERN_C int2		i2inc _DECLARE((short*));
EXTERN_C int2		i2set _DECLARE((short*, short));
EXTERN_C int4		i4add _DECLARE((long*, long));
EXTERN_C int4		i4inc _DECLARE((long*));
EXTERN_C int4		i4dec _DECLARE((long*));
EXTERN_C int4		i4set _DECLARE((long*, long));
EXTERN_C int		i6add _DECLARE((long*, long*));
EXTERN_C int		i6sub _DECLARE((long*, long*));
EXTERN_C void		ialpha _DECLARE((short*, short*));
EXTERN_C unsigned long	iandx _DECLARE((unsigned short*, unsigned short*));
EXTERN_C long		ibclrx _DECLARE((unsigned short*, unsigned short*));
EXTERN_C unsigned long	ibits _DECLARE((unsigned long*, unsigned long*, unsigned long*));
EXTERN_C short		ibitsx _DECLARE((unsigned short*, unsigned short*, unsigned short*));
EXTERN_C long		ibsetx _DECLARE((unsigned short*, unsigned short*));
EXTERN_C long		idbl _DECLARE((short*));
EXTERN_C long		ieorx _DECLARE((short*, short*));
EXTERN_C short		iequal _DECLARE((short*, short*, short*));
static int		iexpand _DECLARE((long));
EXTERN_C void		ifill _DECLARE((short*, short*, short*));
static int		inhere _DECLARE((char*));
EXTERN_C double		infdouble _DECLARE((void));
EXTERN_C float		inffloat _DECLARE((void));
EXTERN_C int		init_scan _DECLARE((struct rtu_hxr_data*,int));
EXTERN_C int		inschr _DECLARE((char*, int, char*, int, char, int));
EXTERN_C void       Int8ToHsctime _DECLARE((int8*, HSCTIME*));
EXTERN_C void       intchr _DECLARE((char*, short, short*, short*));
EXTERN_C void		interupt _DECLARE((int));
EXTERN_C short		intflg _DECLARE((int));
EXTERN_C void		inttm _DECLARE((HSCTIME*, short*, float*));
EXTERN_C void           inttm2 _DECLARE((HSCTIME*,int2*,float*,HSCTIME*));
EXTERN_C void		inttm1m _DECLARE((HSCTIME*, short*, uint4*));
EXTERN_C void		inttm2m _DECLARE((HSCTIME*, short*, uint4*, HSCTIME*));
EXTERN_C int2		inttoip _DECLARE((int4, char*));
EXTERN_C int		ioctl _DECLARE((int, int, ...));
EXTERN_C long		iorx _DECLARE((short*, short*));
EXTERN_C int		CALLBACK IPS_chart_normalise _DECLARE((float *, int *, double, double, int));
EXTERN_C int		CALLBACK IPS_chart_set_data _DECLARE((int, int, int, int, int, int *));
EXTERN_C int		CALLBACK IPS_chart_set_real_x_axis _DECLARE((int, int, int, double, double));
EXTERN_C int		CALLBACK IPS_chart_set_real_y_axis _DECLARE((int, int, int, double, double));
EXTERN_C int		CALLBACK IPS_get_object_id _DECLARE((int, int, char *, int *));
EXTERN_C int		CALLBACK IPS_object_set_attribute _DECLARE((int, int, int, char *, char *));
EXTERN_C short		ipsio _DECLARE((short*, short*, short*, short*, short*, short*, short*, short*));
EXTERN_C int		ipsread _DECLARE((int, int, char*, int, unsigned int));
EXTERN_C int		ipsreadpb _DECLARE((int, int, char*, int, unsigned int));
EXTERN_C int		ipswrite _DECLARE((int, int, char*, int));
EXTERN_C int2       iptoint _DECLARE((char*, int2, int2*, int4*));
EXTERN_C unsigned short	irot _DECLARE((unsigned short*, short*));
EXTERN_C unsigned long	irot4 _DECLARE((unsigned long*, short*));
EXTERN_C int		isblank _DECLARE((char *str, int2 len));
EXTERN_C unsigned short	ishftx _DECLARE((unsigned short*, short*));
EXTERN_C int		isinfdouble _DECLARE((double*));
EXTERN_C int		isinffloat _DECLARE((float*));
EXTERN_C int		isnandouble _DECLARE((double*));
EXTERN_C int		isnanfloat _DECLARE((float*));
EXTERN_C int		ispntalarm _DECLARE((int));
EXTERN_C int		isScanTask _DECLARE((int));
EXTERN_C short		itoasc _DECLARE((char*, short, short*, short*));
EXTERN_C char*		itos _DECLARE((int));
EXTERN_C void		iupper _DECLARE((short*, short*));
EXTERN_C void		jbclr  _DECLARE((unsigned short*, int));
EXTERN_C unsigned short	jbits  _DECLARE((unsigned short*, int, int));
EXTERN_C unsigned long	j4bits _DECLARE((unsigned short*, int, int));
EXTERN_C void		jbset  _DECLARE((unsigned short*, int));
EXTERN_C int            jbtest _DECLARE((unsigned short*, int));
EXTERN_C unsigned short	jbyte _DECLARE((unsigned short*, short*));
EXTERN_C int		mvjbits _DECLARE((unsigned short*, int, int, unsigned short*, int));
EXTERN_C short		julian _DECLARE((short*, long*, ...));
EXTERN_C void		jultm _DECLARE((HSCTIME*, short*, float*));
EXTERN_C DATE		JulianToDate _DECLARE((int, float));
EXTERN_C short		kbdio _DECLARE((short*, short*, short*, short*, short*, short*));
EXTERN_C short		kbdsv_lrn _DECLARE((short*));
EXTERN_C short		keepalive _DECLARE((void));
static int		keycomp _DECLARE((char*, char*, int, int));
EXTERN_C void		keydel _DECLARE((char*, short, char*, short, char*, short, short*));
EXTERN_C void		keyint		_DECLARE((void));
EXTERN_C int		keyopn _DECLARE((char*, short, char*, short, char*));
EXTERN_C void		keyrea _DECLARE((char*, short, char*, short, char*, short, short*));
EXTERN_C int		keystore _DECLARE((char*, char*, int, struct keydesc*));
EXTERN_C void		keywrt _DECLARE((char*, short, char*, short, char*, short, short*));
EXTERN_C void		kilintr _DECLARE((void));
EXTERN_C void		killintr _DECLARE((void));
EXTERN_C void		killtsk _DECLARE((struct trbtbl*));
EXTERN_C void		killx _DECLARE((short*));
EXTERN_C short		kiltsk _DECLARE((short*));
static int		lastkey _DECLARE((char*));
EXTERN_C int		ldchar _DECLARE((char*, int, char*));
EXTERN_C double		lddbl _DECLARE((char*));
EXTERN_C float		ldfloat _DECLARE((char*));
EXTERN_C int		ldint _DECLARE((unsigned char*));
EXTERN_C long		ldlong _DECLARE((unsigned char*));
EXTERN_C int		ldmint _DECLARE((char*));
EXTERN_C long		ldmlong _DECLARE((char*));
EXTERN_C int		lfn2file _DECLARE((int));
EXTERN_C short		lfnok _DECLARE((short*));
EXTERN_C void		lftjst _DECLARE((char*, short, short*));
EXTERN_C int2		licencechk _DECLARE((short, unsigned short*));
EXTERN_C int2		licread _DECLARE((char*, unsigned char*, unsigned char*));
EXTERN_C int2		licwrite _DECLARE((char*, unsigned char*, unsigned char*));
EXTERN_C void		linrpt _DECLARE((short*, short*, short*, short*, short*, short*, short*));
EXTERN_C int		linkname _DECLARE((int, char*, int));
EXTERN_C short		lltoasc _DECLARE((char*, short, short*, short*));
EXTERN_C short		lnknok _DECLARE((void));
EXTERN_C void		LocalHsctimeToHsctime _DECLARE((HSCTIME*, HSCTIME*));
EXTERN_C int		localname _DECLARE((char*, int2));
#ifdef NT
EXTERN_C void		LogMessage _DECLARE((int, int, int, char *,char *, char *,int		,char *, ...));
EXTERN_C void		LogDebugMsg _DECLARE((int, int, int, char *,char *, char *,int		,char *, ...));
#endif
EXTERN_C char*		lower _DECLARE((char*));
EXTERN_C void		lowercase _DECLARE((char*));
EXTERN_C unsigned char	lrc8 _DECLARE((unsigned char*, int));
EXTERN_C short		lrnok _DECLARE((short*));
EXTERN_C BOOL		MatchAccessRights _DECLARE((DWORD, int));
EXTERN_C BOOL		MatchDataType _DECLARE((VARTYPE, int, int));
EXTERN_C BOOL		MatchStringFilter _DECLARE((LPCWSTR, WCHAR *));
static int		makekey _DECLARE((char*, char*));
static int		man_up _DECLARE((char*));
EXTERN_C int4		max_def_utbnum _DECLARE((short*));
EXTERN_C int4		max_def_utbsiz _DECLARE((short*));
EXTERN_C int4		max_def_utbtyp _DECLARE((short*));
EXTERN_C int4		max_lic_acc _DECLARE((short*));
EXTERN_C int4		max_lic_acs _DECLARE((short*));
EXTERN_C int4		max_lic_alg _DECLARE((short*));
EXTERN_C int4		max_lic_alm _DECLARE((short*));
EXTERN_C int4		max_lic_ana _DECLARE((short*));
EXTERN_C short		max_lic_arc _DECLARE((short*));
EXTERN_C int4		max_lic_chn _DECLARE((short*));
EXTERN_C int4		max_lic_crd _DECLARE((short*));
EXTERN_C int4		max_lic_dly _DECLARE((short*));
EXTERN_C int4		max_lic_dsp _DECLARE((short*));
EXTERN_C int4		max_lic_evt _DECLARE((short*));
EXTERN_C int4		max_lic_ext _DECLARE((short*));
EXTERN_C int4		max_lic_fst _DECLARE((short*));
EXTERN_C int4		max_lic_ggl _DECLARE((short*));
EXTERN_C int4		max_lic_grp _DECLARE((short*));
EXTERN_C int4		max_lic_hst _DECLARE((short*, short*));
EXTERN_C int4		max_lic_lacgrp _DECLARE((short*));
EXTERN_C int4		max_lic_lpt _DECLARE((short*));
EXTERN_C int4		max_lic_msg _DECLARE((short*));
EXTERN_C int4		max_lic_opr _DECLARE((short*));
EXTERN_C long		max_lic_pnt _DECLARE((short*));
EXTERN_C int4		max_lic_rcp _DECLARE((short*));
EXTERN_C int4		max_lic_rpt _DECLARE((short*));
EXTERN_C int4		max_lic_rsp _DECLARE((short*));
EXTERN_C int4		max_lic_rtu _DECLARE((short*));
EXTERN_C int4		max_lic_sdl _DECLARE((short*));
EXTERN_C int4		max_lic_sha _DECLARE((short*));
EXTERN_C int4		max_lic_slglvl _DECLARE((short*));
EXTERN_C int4		max_lic_slgrpt _DECLARE((short*));
EXTERN_C int4		max_lic_slgspc _DECLARE((short*));
EXTERN_C int4		max_lic_slgtpd _DECLARE((short*));
EXTERN_C int4		max_lic_slgzon _DECLARE((short*));
EXTERN_C int4		max_lic_soe _DECLARE((short*));
EXTERN_C int4		max_lic_sta _DECLARE((short*));
EXTERN_C int4		max_lic_std _DECLARE((short*));
EXTERN_C int4		max_station _DECLARE((void));
EXTERN_C int4		max_lic_tnd _DECLARE((short*));
EXTERN_C int4		max_lic_ulplvl _DECLARE((short*));
EXTERN_C int4		max_lic_ulpspc _DECLARE((short*));
EXTERN_C int4		max_lic_ulptpd _DECLARE((short*));
EXTERN_C int4		max_lic_utbl _DECLARE((short*));
EXTERN_C int		memadr _DECLARE((short*));
EXTERN_C short		mmout _DECLARE((short*, char*, short, char*, short, short*, short*, short*));
EXTERN_C unsigned char	mod256 _DECLARE((unsigned char*, int));
EXTERN_C long		modx _DECLARE((short*, short*));
EXTERN_C short		morerq _DECLARE((void));
EXTERN_C void		movbyt _DECLARE((short*, short*, short*, short*, short*));
EXTERN_C void		move _DECLARE((short*, short*, short*));
EXTERN_C long		mpool _DECLARE((short));
EXTERN_C int		msgget _DECLARE((int, int));
EXTERN_C int		msgrcv _DECLARE((int, struct message*, int, int, int));
EXTERN_C int		msgsnd _DECLARE((int, struct message*, int, int));
EXTERN_C long		msiday _DECLARE((HSCTIME*));
EXTERN_C void		mvbitx _DECLARE((unsigned short*, unsigned short*, unsigned short*, unsigned short*, unsigned short*));
EXTERN_C short		name2len _DECLARE((char*));
EXTERN_C short		name2lfn _DECLARE((char*));
static int		namelen _DECLARE((char*));
EXTERN_C double		nandouble _DECLARE((void));
EXTERN_C float		nanfloat _DECLARE((void));
EXTERN_C int		netcmp _DECLARE((void*,int,void*));
EXTERN_C int		netexp _DECLARE((void*,int,void*));
static int		newroot _DECLARE((char*, long, int, int));
EXTERN_C int		nips _DECLARE((struct thrd_prm*));
EXTERN_C void		no_floppy _DECLARE((void));
EXTERN_C int		none _DECLARE((void));
EXTERN_C int		not_end_of_text _DECLARE((char*, int));
EXTERN_C void		notransfer _DECLARE((void));
EXTERN_C void		nterr _DECLARE((int, int, char*, char*, char*, ...));
EXTERN_C int		numeric_subpag _DECLARE((int, int));
EXTERN_C short		nuout _DECLARE((short*, char*, short));
EXTERN_C short		nxpath _DECLARE((char*, short, char*, short));
EXTERN_C short		nxtfld _DECLARE((char*, short, short*));
EXTERN_C int2 		NxtPnt _DECLARE((int2, int2, uint2 *));
EXTERN_C int STDCALL	OFFSET _DECLARE((void*, void*));
EXTERN_C void		on_alarm _DECLARE((void));
EXTERN_C void		on_pipe _DECLARE((void));
EXTERN_C unsigned char	onescomp _DECLARE((unsigned char*, int));
EXTERN_C int		open_dat_file _DECLARE((char*, long*, char*, short, unsigned long*, int));

#ifdef NT
EXTERN_C HANDLE		opendir _DECLARE((char*));
EXTERN_C struct _iobuf*	open_input_file _DECLARE((char*));
#else
EXTERN_C FILE*		open_input_file _DECLARE((char*));
#endif

EXTERN_C void		open_log _DECLARE((void));
EXTERN_C int		open_output_file _DECLARE((char*, struct dictinfo**, struct keydesc**));
EXTERN_C int		open_raw _DECLARE((int, int*));

EXTERN_C short		otoasc _DECLARE((char*, short, short*, short*));
EXTERN_C int		pcsi_io _DECLARE((int, int, int, unsigned char*, int));
EXTERN_C void		pcsiio _DECLARE((short*, short*, short*, short*, short*));
EXTERN_C int		p_close _DECLARE((int, int));
EXTERN_C int		p_flush _DECLARE((int));
EXTERN_C void*		p_getevt _DECLARE((int, int));
EXTERN_C int		p_open _DECLARE((int, int));
EXTERN_C int		p_read _DECLARE((int, int, char*, int, int));
EXTERN_C int		p_write _DECLARE((int, int, char*, int));
EXTERN_C int        peername _DECLARE((char*, int));
EXTERN_C double		pget _DECLARE((short*));
EXTERN_C void		pput _DECLARE((short*, double*));
EXTERN_C char*		prline _DECLARE((unsigned char*, int));
EXTERN_C char*		prline2 _DECLARE((unsigned char*, int));
EXTERN_C char*		prline4 _DECLARE((unsigned char*, int));
EXTERN_C char*		prlinex _DECLARE((unsigned char*, int, int));
EXTERN_C short		prmfnd _DECLARE((char*, short, short*));
EXTERN_C void		prof_togle _DECLARE((void));
EXTERN_C void	    	PrtFldWidths _DECLARE((int,int2*,int2*,int2*,int2*,int2*,int2*,int2*));
EXTERN_C short		prvfld _DECLARE((char*, short, short*));
EXTERN_C short		pswio _DECLARE((short*));
EXTERN_C short		ptoasc _DECLARE((char*, short, short*, short*, short*));
EXTERN_C void		putbyt _DECLARE((short*, short*, short*));
EXTERN_C void		putcom _DECLARE((short*, long*, short*, short*));
EXTERN_C short		putdat _DECLARE((short*, short*, short*, short*, short*, unsigned short*, unsigned short*, short*));
EXTERN_C long		putdat4 _DECLARE((long*, long*, long*, long*, long*, long*, unsigned short*, long*));
EXTERN_C long       putdatw4 _DECLARE((long*, long*, long*, long*, long*, long*, unsigned short*, long*, long*));
EXTERN_C void		puthst _DECLARE((short*, short*, float*, short*, short*, short*, short*, char*, short, long*, short*));
EXTERN_C short		putval _DECLARE((short*, uint2*, uint2*, float*, ...));
EXTERN_C short		putvl _DECLARE((short*, int, uint2*, uint2*, float*, char*, short, short, short));
EXTERN_C void		q_close _DECLARE((struct unix_queue*));
EXTERN_C char*		q_name _DECLARE((char*, char*));
EXTERN_C struct unix_queue*       q_open _DECLARE((char*));
EXTERN_C int		q_reset _DECLARE((char*, int, int));
EXTERN_C int		q_seg_wrt _DECLARE((struct unix_queue*, char*, int, int));
EXTERN_C int		r12_rd _DECLARE((unsigned int, int, unsigned char*, int, int));
EXTERN_C int		r12_wrt _DECLARE((int, int, unsigned char*, int));
EXTERN_C void		rawtm _DECLARE((int4*));
EXTERN_C void		RawtmToHsctime _DECLARE((int4*, HSCTIME*));
static int		rd_aud _DECLARE((int));
EXTERN_C void		rdpipe _DECLARE((void*));
EXTERN_C int		read_atd _DECLARE((int, int, char*, int, int));
EXTERN_C int		read_atdp _DECLARE((int, int, char*, int, int));
EXTERN_C int		read_exact _DECLARE((int, char*, int));
EXTERN_C int		read_exact_silent _DECLARE((int, char*, int));
EXTERN_C int		read_exact_silent_timeout _DECLARE((int, char*, int, int));
EXTERN_C int		read_n _DECLARE((int, int, char*, int));
EXTERN_C int		read_pcsi _DECLARE((int, int, char*, int));
EXTERN_C void		real_psw _DECLARE((int, short*, int));
EXTERN_C int		read_r12 _DECLARE((int, int, char*, int));
EXTERN_C int		read_tmout _DECLARE((unsigned int, int (*read_routine)(void), int, int, char*, int));

#ifdef NT
EXTERN_C struct dirent*	readdir _DECLARE((HANDLE));
EXTERN_C int		read_input_line _DECLARE((struct _iobuf*, char*, int));
#else
EXTERN_C int		read_input_line _DECLARE((FILE*, char*, int));
#endif

EXTERN_C short		relsem _DECLARE((short*));
EXTERN_C short		ressem _DECLARE((short*, short*, short*));
EXTERN_C void		revrng _DECLARE((short*, float*, float*, float*, float*, float*));
EXTERN_C void		rgtfnd _DECLARE((char*, short, short*));
EXTERN_C void		rgtjst _DECLARE((char*, short, short*));
EXTERN_C short		rltml _DECLARE((short*));
EXTERN_C short		root_fstyp _DECLARE((void));
EXTERN_C void	    	RptFldWidths _DECLARE((int2*,int2*,int2*,int2*,int2*));
EXTERN_C short		rqcl _DECLARE((short*, short*, short*));
EXTERN_C void		rqcl_wake _DECLARE((void));
EXTERN_C short		rqgtsk _DECLARE((char*, short, short*));
EXTERN_C void		rqstsk _DECLARE((short*, short*, unsigned short*, ...));
EXTERN_C short		rqtskb _DECLARE((short*, short*,...));
EXTERN_C int		rqtskb2 _DECLARE((int, void*, int));
EXTERN_C int		rqtskb_ex _DECLARE((int, void*, int, int));
EXTERN_C short		rwflxmt _DECLARE((char*, short));
static int		save _DECLARE((void));
EXTERN_C short		savval _DECLARE((short*, uint2*, uint2*, float*, char*, short));
EXTERN_C int		scriptcmd _DECLARE((int, int));
static int		searchkey _DECLARE((char*));
EXTERN_C void		SecondsToHsctime _DECLARE((double*, HSCTIME*));
EXTERN_C void		sed _DECLARE((char*, short, char*, short, char*, short));
EXTERN_C void		sem_tmout _DECLARE((int));
EXTERN_C int		semgt _DECLARE((int, int));
EXTERN_C void		semlck _DECLARE((int, int));
EXTERN_C char*		semnam _DECLARE((char*, int, int));
EXTERN_C int		semop1 _DECLARE((int, int, int, int));
EXTERN_C void		semset _DECLARE((int, int, int));
EXTERN_C void		semunlck _DECLARE((int, int));
EXTERN_C int 		set_debug _DECLARE((int type));
EXTERN_C int 		set_file_secur _DECLARE(( SECURITY_DESCRIPTOR** psd));
EXTERN_C int 		set_sa(SECURITY_ATTRIBUTES** psa);
EXTERN_C int 		set_secur _DECLARE((HANDLE h, int type));
EXTERN_C int            set_secur_byname _DECLARE((HANDLE h, int type, char* groups,int access));
EXTERN_C int            set_secur_bysid(HANDLE h, int type, PSID group,int access);
EXTERN_C int 		set_secur_ea_byname _DECLARE((char* groupName,char* objectName,int type,int accessMask,int inheritance,int accessMode));
EXTERN_C int 		set_svc_secur _DECLARE((HANDLE h));
EXTERN_C void		setobjcol _DECLARE((short*, short*, short*, short*));
EXTERN_C void		setobjlvl _DECLARE((short*, short*, short*, float*, float*, float*, float*, float*));
EXTERN_C void		setobjtxt _DECLARE((short*, short*, short*, char*, short));
EXTERN_C void		setobjval _DECLARE((short*, short*, short*, short*));
EXTERN_C int		setraw _DECLARE((int, struct devtbl*));
EXTERN_C short		settm _DECLARE((long*));
EXTERN_C int		setuxtm _DECLARE((struct timeval*));
EXTERN_C void		sig_exit _DECLARE((void));
EXTERN_C void		sig_onsrv _DECLARE((void));
EXTERN_C void		sig_onclt _DECLARE((void));
EXTERN_C int		snd_u_fl _DECLARE((char*, int, int2));
EXTERN_C void		snt_nips _DECLARE((struct thrd_prm*));
static int		splitadd _DECLARE((char*, long, int));
static int		splithigh _DECLARE((char*, long, int));
EXTERN_C void		splithost _DECLARE((char*, char*, char*));
static int		splitlow _DECLARE((char*, long, int));
EXTERN_C void		splitpath _DECLARE((char*, char*, char*));
EXTERN_C void		sptsk _DECLARE((short*, char*, short, short*, short*, short*, char*, short, char*, short, char*, short));
EXTERN_C void		srch _DECLARE((char*, short, char*, short, short*));
EXTERN_C short		srchar _DECLARE((char*, short, char*, short, short*, short*));
EXTERN_C short		ssne _DECLARE((float*, float*));
EXTERN_C void		stats_sleep _DECLARE((int4*, float*, float*));
EXTERN_C void		stats_wakeup _DECLARE((void));
EXTERN_C int		stchar _DECLARE((char*, char*, int));
EXTERN_C int		stdbl _DECLARE((double, char*));
EXTERN_C int		stfloat _DECLARE((double, char*));
EXTERN_C int		stint _DECLARE((short, unsigned char*));
EXTERN_C int		stlong		_DECLARE((long, unsigned char*));
EXTERN_C int		stmint _DECLARE((int, char*));
EXTERN_C int		stmlong		_DECLARE((long, char*));
EXTERN_C short		stn_num _DECLARE((short*));
static void		str_insert _DECLARE((char*, char*, char*));
EXTERN_C void		strip_space _DECLARE((char*, int2));
EXTERN_C void		strjstfy _DECLARE((char*, int));
EXTERN_C int		strrep _DECLARE((char*, int, char*, char*));
static int		subsearch _DECLARE((char*));
EXTERN_C void		subtime _DECLARE((HSCTIME*, HSCTIME*));
EXTERN_C void		subtm  _DECLARE((HSCTIME*, long, int));
EXTERN_C void		subtm2 _DECLARE((HSCTIME*, long, long, int));
EXTERN_C int		svc_crgtsk _DECLARE((int,char*,struct error_buf*));
EXTERN_C int		svc_dataio _DECLARE((int,char*,struct error_buf*));
EXTERN_C int		svc_dbseq  _DECLARE((int,char*,struct error_buf*));
EXTERN_C int		svc_deltag _DECLARE((int,char*,struct error_buf*));
EXTERN_C int		svc_dlgtsk _DECLARE((int,char*,struct error_buf*));
EXTERN_C int		svc_evarch _DECLARE((int,char*,struct error_buf*));
EXTERN_C int		svc_execute _DECLARE((int,char*,struct error_buf*));
EXTERN_C int		svc_filwrt _DECLARE((int,char**,struct error_buf*));
EXTERN_C int		svc_heap _DECLARE((int,char*,struct error_buf*));
EXTERN_C int		svc_history _DECLARE((int,char**,struct error_buf*));
EXTERN_C int		svc_mkdir _DECLARE((int,char*,struct error_buf*));
EXTERN_C int		svc_puttag _DECLARE((int,char*,struct error_buf*));
EXTERN_C int		svc_rwflw _DECLARE((int,char*,struct error_buf*));
EXTERN_C int		svc_scriptcmd _DECLARE((int,char*,struct error_buf*));
EXTERN_C int		svc_trans _DECLARE((int,struct d_trans*,struct error_buf*));

#ifdef NT
			/* swab is a standard runtime library function */
#else
EXTERN_C void		swab _DECLARE((char*, char*, int*));
#endif

EXTERN_C int		swb_cpy _DECLARE((char*, char*, unsigned int));
EXTERN_C void		tabexp _DECLARE((char*, int));
EXTERN_C int		task_busy _DECLARE((struct trbtbl*));
EXTERN_C short		tdccnt _DECLARE((float*, double*, short*, short*));
EXTERN_C short		tdceng _DECLARE((float*, double*, short*, short*));
EXTERN_C int		textchr _DECLARE((char*));
EXTERN_C void		textclr _DECLARE((void));
EXTERN_C int		textget _DECLARE((int, int, int, unsigned char*, int, unsigned char*, int));
EXTERN_C void		textins _DECLARE((void));
EXTERN_C int		textput _DECLARE((int, int, unsigned char*, int, unsigned char*, int));
EXTERN_C void		toieee _DECLARE((short*, short*));
static int		tokey _DECLARE((char**, char**, int, int));
EXTERN_C void		tolong _DECLARE((short*, short*));
EXTERN_C void		trace _DECLARE((int, int, char*, int));
static long		transno _DECLARE((void));
EXTERN_C int		trap _DECLARE((int));
EXTERN_C void		trapen _DECLARE((int*,short*));

#ifdef	NT
EXTERN_C void		trm04 _DECLARE((short*, ...));
#else
EXTERN_C void		trm04 _DECLARE((short*, short*));
#endif

EXTERN_C void		trmtsk _DECLARE((short*, short*));
EXTERN_C void		trptsk _DECLARE((void));
EXTERN_C void       TruncateHsctime(HSCTIME* pHscTime, HSCTIME* pHscTimeTruncated, int2 units);
EXTERN_C void       tskclean _DECLARE((void));
EXTERN_C short		tstskb _DECLARE((short*));
EXTERN_C unsigned char	twoscomp _DECLARE((unsigned char*, int));
static int		typlen _DECLARE((int));
EXTERN_C void		unixbuf _DECLARE((int, int, char*, char*, char*, ...));
EXTERN_C void		unixerr _DECLARE((int, int, char*, char*, char*, ...));
EXTERN_C void		unixmsg _DECLARE((int, int, char*, char*, char*, ...));
EXTERN_C void		unixstack _DECLARE((int, int, char*, char*, char*, ...));
EXTERN_C void		unixstack_ex _DECLARE((int, int, char*, char*, CONTEXT*, char*, ...));
EXTERN_C void		update_links _DECLARE((struct link_data*,int,int,int));
EXTERN_C void		upper _DECLARE((char*, short));
EXTERN_C short		usrout _DECLARE((char*, short));
EXTERN_C short		vewreq _DECLARE((short*));
EXTERN_C void		virtual_psw _DECLARE((int, short*, int));
EXTERN_C short		vld _DECLARE((short*));
EXTERN_C short		vms_euout _DECLARE((int, char*, short));
EXTERN_C void		vmserr _DECLARE((int, int, char*, char*, char*, ...));
EXTERN_C void		vrfy _DECLARE((char*, short, char*, short, short*));
EXTERN_C void		vxprrm _DECLARE((short*, short*));
EXTERN_C short STDCALL	WAITN _DECLARE((short*, short*));
EXTERN_C short		waitr _DECLARE((short*));
EXTERN_C int		waitx _DECLARE((int,int));
EXTERN_C void STDCALL	WAITX _DECLARE((short*,short*,short*));
EXTERN_C char*		where _DECLARE((char*));
static int		wr_aud _DECLARE((int));
EXTERN_C int		write_output_line _DECLARE((int, char*, struct dictinfo*));
EXTERN_C int		write_n _DECLARE((int, int, char*, int));
EXTERN_C int		wrt_exact _DECLARE((int, char*, int));
EXTERN_C void		wrtlog _DECLARE((int));
EXTERN_C short		wttskb _DECLARE((short*));
EXTERN_C void		xoff _DECLARE((void));
EXTERN_C void		xon _DECLARE((void));
EXTERN_C char		xor _DECLARE((unsigned char*, int));
EXTERN_C char		xorchk _DECLARE((unsigned char*, int, unsigned short));
EXTERN_C short		xpath _DECLARE((char*, short, char*, short));


//
// Operator Security Related calls
//
EXTERN_C int		hsc_sec_get_ClientHelper(void**);
EXTERN_C int		hsc_sec_release_ClientHelper(void*);
EXTERN_C int		hsc_sec_ValidateCredentials(char*, char*, char*);
EXTERN_C int		hsc_sec_AuthenticateCredentials(void*, unsigned int, char*, char*, char*);
EXTERN_C int		hsc_sec_ChangePassword(void*, unsigned int, char*, char*, char*, char*);
EXTERN_C int		hsc_operatorUpdateFullName _DECLARE((int));
EXTERN_C int2		hsc_oper_allowed_on_area _DECLARE((int2, int2, int2*));
EXTERN_C int		hsc_addOperatorTag _DECLARE((int));
EXTERN_C int		hsc_delOperatorTag _DECLARE((int));
EXTERN_C int		hsc_checkWinOperatorExists _DECLARE((char*, char*));
EXTERN_C int		hsc_getWinOperatorName _DECLARE((char*, char*, char*));
EXTERN_C void		hsc_constructOperatorIDa _DECLARE((char*, char*, char*));
EXTERN_C void		hsc_constructOperatorIDb _DECLARE((char*, char*, char*));
EXTERN_C int		hsc_getActorName _DECLARE((char*, int, int));
EXTERN_C int		hsc_getCRTOperatorNumber _DECLARE((int));
EXTERN_C int		hsc_getCRTOperatorID _DECLARE((char*, int, int));
EXTERN_C int            hsc_getLocalUserName _DECLARE((char*, int, int));
EXTERN_C int            hsc_getLocalUserNameCrt _DECLARE((char*, int, int));
EXTERN_C int            hsc_getLocalUserNameString _DECLARE((char*, int, char*));
EXTERN_C int		hsc_getOperatorID _DECLARE((char*, int, int));
EXTERN_C int		hsc_getOperatorFullName _DECLARE((char*, int, int));
EXTERN_C int		hsc_IsOperidRecordAGroup _DECLARE((struct operid));
EXTERN_C int		hsc_IsOperidRecordAValidGroup _DECLARE((struct operid));
EXTERN_C int		hsc_DoesOperidAllowGroups _DECLARE((int));
EXTERN_C int		CanLoginNow _DECLARE((struct operid));
EXTERN_C int		hsc_GetGroups4Operator _DECLARE((int2, char *, int **, int *));
EXTERN_C void		hsc_FreeGroups _DECLARE((int *));
EXTERN_C int		hsc_GetSOR4Operator(int nOperidRec, int *pnSecLvl, int *pnContLvl, int2 *poidpin,
						int2 *poidsmp[OIDSTASZ], int2 *poidcsmp[OIDCSTASZ], int2 *poidapm[MAPSZ*2],
						int2 *poidasg[MAPSZ], int2 *poidcsg[CMDTBLNM][MAPSZ]);
EXTERN_C int		hsc_GetSOR4Operator2 _DECLARE((int, int *, int *, int2 *, int2 *, int2 *, int2 *, int2 *, int2 *));
EXTERN_C int		hsc_GetSOR4Groups(int cGroups, int *rgOperidRecNo, int *pnSecLvl, int *pnContLvl,
					int2 *poidpin, int2 *poidsmp[OIDSTASZ], int2 *poidcsmp[OIDCSTASZ],
					int2 *poidapm[MAPSZ*2], int2 *poidasg[MAPSZ], int2 *poidcsg[CMDTBLNM][MAPSZ]);
EXTERN_C int		hsc_GetSecLvl4Operator _DECLARE((int, int *));
EXTERN_C int		hsc_GetSecLvl4Groups _DECLARE((int, int *, int *));
EXTERN_C int		hsc_GetContLvl4Operator _DECLARE((int, int *));
EXTERN_C int		hsc_GetContLvl4Groups _DECLARE((int, int *, int *));
EXTERN_C int		hsc_GetPrintInhibits4Operator _DECLARE((int, int *));
EXTERN_C int		hsc_GetPrintInhibits4Groups _DECLARE((int, int *, int *));
EXTERN_C int		hsc_GetIdleTimeout4Operator _DECLARE((int , int *));
EXTERN_C int		hsc_GetIdleTimeout4Groups _DECLARE((int , int *, int*));
EXTERN_C int		hsc_GetStartupPageBuffer4Operator _DECLARE((int , int2**));
EXTERN_C int		hsc_GetStartupPageBuffer4Groups _DECLARE((int , int *, int2**));
EXTERN_C int		hsc_FreeStartupPageBuffer _DECLARE((int2 **));
EXTERN_C int		hsc_GetFullName4User _DECLARE((char *, char **));
EXTERN_C int		hsc_FreeFullName _DECLARE((char **));
EXTERN_C int		hsc_UpdateCrttblGroup _DECLARE((int));
EXTERN_C int        hsc_GetMaxNumberOfGroups _DECLARE((void));
EXTERN_C BOOL       hsc_IsMemberOfHA();
EXTERN_C BOOL       hsc_IsMemberOfGroup(char *, char *);

EXTERN_C int        getloginuseratlevel_emb _DECLARE((int2  *, int2));

EXTERN_C void		hsc_event_fn(uint2, uint2, uint4);

EXTERN_C void		auditmsg _DECLARE((long, long, char*, char*, char*, ...));
EXTERN_C int		hsc_SetParanoid _DECLARE((int, int));
EXTERN_C int		hsc_auditout _DECLARE((char*, int));


#endif	/* end of conditional include PROTOTYPES_FLAG */

/************************************************************************/
/*****     COPYRIGHT 2008           HONEYWELL INTERNATIONAL SARL     ****/
/************************************************************************/
