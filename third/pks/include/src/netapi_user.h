/************************************************************************/
/*****     COPYRIGHT 1998 - 2001    HONEYWELL LIMITED AUSTRALIA      ****/
/************************************************************************/

/* Conditionally print out a trace of header file activity */
#ifdef _HEADER_FILE_TRACE_FLAG
#pragma message("Entering Header file: netapi_user.h,v")
#endif

/* Check to see if we've finished processing this header file before */
#ifndef NETAPI_USER_ENDFLAG

/* Check to see if we've partially processed this header file already */
#ifdef NETAPI_USER_STARTFLAG
#pragma message("ERROR: Partially processed: netapi_user.h,v")
#else
#define NETAPI_USER_STARTFLAG

/*
@(#)netapi_user.h,v	830.1
BEGIN_DOC
---------------------------------------------------------------------------
NETAPI_USER.H - Contains all macros and function prototypes to be exposed
                to end users.
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
#include "src/netapi_types.h"

/***********************************************
** Raw defines
***********************************************/

#define __loadds

/************************************
** *** Miscellaneous Functions *** **
************************************/
EXTERN_C int __loadds CALLBACK hsc_cnads_startup _DECLARE((void));
EXTERN_C int __loadds CALLBACK hsc_cnads_cleanup _DECLARE((void));
EXTERN_C int __loadds CALLBACK rrqstsk _DECLARE((char *, int, rrqstsk_data *));
EXTERN_C int __loadds CALLBACK rrqtskb _DECLARE((char *, int, rrqtskb_data *));
EXTERN_C int __loadds CALLBACK hsc_DATE_to_hsctime _DECLARE((DATE, HSCTIME*));
EXTERN_C int __loadds CALLBACK hsc_hsctime_to_DATE _DECLARE((HSCTIME, DATE*));

/******************************
** *** History Functions *** **
******************************/
EXTERN_C int __loadds CALLBACK rgethst_date _DECLARE((char *, int, rgethst_date_data *));
EXTERN_C int __loadds CALLBACK rgethst_ofst _DECLARE((char *, int, rgethst_ofst_data *));

EXTERN_C int __loadds CALLBACK rhsc_param_hist_dates
                _DECLARE((char *, int, rgethstpar_date_data *));
EXTERN_C int __loadds CALLBACK rhsc_param_hist_offsets
                _DECLARE((char *, int, rgethstpar_ofst_data *));

EXTERN_C int __loadds CALLBACK rhsc_param_hist_dates_vb_ansi
                _DECLARE((char *, int, SAFEARRAY **));
EXTERN_C int __loadds CALLBACK rhsc_param_hist_offsets_vb_ansi
                _DECLARE((char *, int, SAFEARRAY **));

EXTERN_C int __loadds CALLBACK rhsc_param_hist_date_bynames
                _DECLARE((char *, int, HIST_BYNAME_DATA *));

EXTERN_C int __loadds CALLBACK rhsc_param_hist_date_bynames_vb_ansi
                _DECLARE((char *szHostname, int cRequests, SAFEARRAY **ppsa));

EXTERN_C int __loadds CALLBACK rhsc_param_hist_offset_bynames
                _DECLARE((char *, int, HIST_BYNAME_DATA *));

EXTERN_C int __loadds CALLBACK rhsc_param_hist_offset_bynames_vb_ansi
                _DECLARE((char *szHostname, int cRequests, SAFEARRAY **ppsa));

EXTERN_C int __loadds CALLBACK rgethstpar_date
                _DECLARE((char *, int, rgethstpar_date_data *));
EXTERN_C int __loadds CALLBACK rgethstpar_ofst
                _DECLARE((char *, int, rgethstpar_ofst_data *));

EXTERN_C int __loadds CALLBACK hsc_bad_value
                _DECLARE((float));

/************************************************************
** *** Point, Parameter and Error Resolution Functions *** **
************************************************************/
EXTERN_C int __loadds CALLBACK rhsc_point_numbers 
                _DECLARE((char *, int, POINT_NUMBER_DATA *));

EXTERN_C int __loadds CALLBACK rhsc_point_numbers_vb_ansi
                _DECLARE((char *, int, SAFEARRAY **));

EXTERN_C int __loadds CALLBACK rgetpnt 
                _DECLARE((char *, int, struct rgetpnt_data_str *));

EXTERN_C short __loadds CALLBACK rgetpnt_vb_ansi
                _DECLARE((char *, short, SAFEARRAY **));

EXTERN_C int __loadds CALLBACK rhsc_param_numbers 
                _DECLARE((char *, int, PARAM_NUMBER_DATA *));

EXTERN_C int __loadds CALLBACK rhsc_param_numbers_vb_ansi
                _DECLARE((char *, int, SAFEARRAY **));

EXTERN_C void __loadds CALLBACK hsc_napierrstr
                _DECLARE((UINT err, char *texterr));


/**************************************************
** *** Get and Put Parameter Value Functions *** **
**************************************************/
EXTERN_C int __loadds CALLBACK rgetval_ascii
                _DECLARE((char *, int, struct rgetval_ascii_data_str *));

EXTERN_C short __loadds CALLBACK rgetval_ascii_vb_ansi
                _DECLARE((char *, short, SAFEARRAY **));
EXTERN_C int __loadds CALLBACK rgetval_hist
                _DECLARE((char *, int, struct rgetval_hist_data_str *));

EXTERN_C int __loadds CALLBACK rgetval_numb
                _DECLARE((char *, int, struct rgetval_numb_data_str *));

EXTERN_C int __loadds CALLBACK rputval_numb
                _DECLARE((char *, int, rputval_numb_data *));

EXTERN_C int __loadds CALLBACK rputval_ascii
                _DECLARE((char *, int, rputval_ascii_data *));

EXTERN_C short __loadds CALLBACK rputval_ascii_vb_ansi
                _DECLARE((char *szHostname, short cRequests, SAFEARRAY **ppsa));
EXTERN_C int __loadds CALLBACK rputval_hist
                _DECLARE((char *, int, rputval_hist_data *));

EXTERN_C int __loadds CALLBACK rhsc_param_values
                _DECLARE((char *, int, int, PARAM_VALUE_DATA *));

EXTERN_C int __loadds CALLBACK rhsc_param_values_vb_ansi
                _DECLARE((char *, int, int, SAFEARRAY **));

EXTERN_C int __loadds CALLBACK rhsc_param_value_puts
                _DECLARE((char *, int, PARAM_VALUE_DATA *));

EXTERN_C int __loadds CALLBACK rhsc_param_value_puts_vb_ansi
                _DECLARE((char *, int, SAFEARRAY **));
EXTERN_C int __loadds CALLBACK rhsc_param_value_puts_sec
                _DECLARE((char *, int, PARAM_VALUE_DATA *, unsigned short));

EXTERN_C int __loadds CALLBACK rhsc_param_value_puts_sec_vb_ansi
                _DECLARE((char *, int, SAFEARRAY **, unsigned short));

EXTERN_C int __loadds CALLBACK rhsc_param_value_bynames
                _DECLARE((char *, int, int, PARAM_BYNAME_DATA *));

EXTERN_C int __loadds CALLBACK rhsc_param_value_bynames_vb_ansi
                _DECLARE((char *, int, int, SAFEARRAY **));

EXTERN_C int __loadds CALLBACK rhsc_param_value_put_bynames
                _DECLARE((char *, int, PARAM_BYNAME_DATA *));

EXTERN_C int __loadds CALLBACK rhsc_param_value_put_bynames_vb_ansi
                _DECLARE((char *, int, SAFEARRAY **));
EXTERN_C int __loadds CALLBACK rhsc_param_value_put_sec_bynames
                _DECLARE((char *, int, PARAM_BYNAME_DATA *, unsigned short));

EXTERN_C int __loadds CALLBACK rhsc_param_value_put_sec_bynames_vb_ansi
                _DECLARE((char *, int, SAFEARRAY **, unsigned short));


/*************************************************
** *** Get and Put User File Data Functions *** **
*************************************************/
EXTERN_C int __loadds CALLBACK rgetdat
                _DECLARE((char *, int, rgetdat_data *));

EXTERN_C short __loadds CALLBACK rgetdat_str_vb_ansi
                _DECLARE((char *szHostname, short cRequests, SAFEARRAY **ppsa));

EXTERN_C int __loadds CALLBACK rputdat
                _DECLARE((char *, int, rgetdat_data *));

EXTERN_C short __loadds CALLBACK rputdat_str_vb_ansi
                _DECLARE((char *szHostname, short cRequests, SAFEARRAY **ppsa));

/****************************************************************
** *** Notification, Alarm, Event Functions
****************************************************************/
EXTERN_C int __loadds CALLBACK rhsc_notifications
                _DECLARE((char *, int, NOTIFICATION_DATA *));

EXTERN_C int __loadds CALLBACK rhsc_notifications_vb_ansi
                _DECLARE((char *szHostname, int cRequests, SAFEARRAY **ppsa));


#define NETAPI_USER_ENDFLAG
#endif               /* end of if not *_startflag */
#endif               /* end of if not *_endflag */

/* Conditionally print out a trace of header file activity */
#ifdef _HEADER_FILE_TRACE_FLAG
#pragma message("Leaving Header file: netapi_user.h,v")
#endif

/************************************************************************/
/*****     COPYRIGHT 1998 - 2001    HONEYWELL LIMITED AUSTRALIA      ****/
/************************************************************************/
