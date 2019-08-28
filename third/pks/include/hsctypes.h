

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Mon Sep 22 23:03:33 2008
 */
/* Compiler settings for \r830\develop\work\pcdm\std\include\src\..\hsctypes.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __hsctypes_h__
#define __hsctypes_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "propidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

/* interface __MIDL_itf_hsctypes_0000 */
/* [local] */ 

#ifndef hsctypes_flag
#define hsctypes_flag
typedef short int2;

typedef unsigned short uint2;

typedef long int4;

typedef unsigned long uint4;

typedef unsigned char uchar;

typedef __int64 int8;

typedef unsigned __int64 uint8;

#ifdef	NT
typedef unsigned short ushort;

typedef unsigned long ulong;

#endif
typedef FILETIME HSCTIME;

typedef int2 PNTRELTYP;

typedef int2 PNTRELDIR;

typedef unsigned short PPTNUM;

typedef unsigned short PRMNUM;

typedef unsigned short PNTNUM;

typedef short PNTTYP;

typedef int GINDEX;

typedef long CREATETIME;

#define	NOCREATETIME	( -1 )

union minus_zero_tmp_var
    {
    float fval;
    long lval;
    } ;
typedef struct tagGDAERR
    {
    DWORD Code;
    DWORD Qualifier;
    } 	GDAERR;

typedef struct tagGDASERVAR
    {
    DWORD cSize;
    /* [size_is] */ unsigned char *pData;
    } 	GDASERVAR;

typedef struct tagGDASECURITY
    {
    int2 StationLevel;
    int2 ControlLevel;
    /* [string] */ unsigned char *pOperID;
    /* [string] */ unsigned char *pOperName;
    } 	GDASECURITY;

typedef struct tagENUMITEM
    {
    /* [string] */ unsigned char *pText;
    int Value;
    } 	ENUMITEM;

typedef struct tagITEMHANDLE
    {
    PNTNUM Point;
    PRMNUM Param;
    } 	ITEMHANDLE;

typedef struct tagGDAALARMHANDLE
    {
    DWORD EECode;
    DWORD BBIndex;
    } 	GDAALARMHANDLE;

typedef struct tagPOINTREF
    {
    /* [string] */ unsigned char *pAlias;
    /* [string] */ unsigned char *pPointName;
    /* [string] */ unsigned char *pPointDescription;
    /* [string] */ unsigned char *pPointArea;
    CREATETIME PointCreateTime;
    } 	POINTREF;

typedef struct tagPOINTREFLIST
    {
    DWORD cPoints;
    /* [size_is] */ POINTREF *Point;
    } 	POINTREFLIST;

typedef struct tagPOINTREF2
    {
    POINTREF PointRef;
    /* [string] */ unsigned char *pPointLocation;
    /* [string] */ unsigned char *pPointItemName;
    /* [string] */ unsigned char *pPointOwner;
    /* [string] */ unsigned char *pPointGuid;
    short PointType;
    short PointSubtype;
    } 	POINTREF2;

typedef struct tagPOINTREF2LIST
    {
    DWORD cPoints;
    /* [size_is] */ POINTREF2 *Point;
    } 	POINTREF2LIST;

typedef PROPVARIANT GDAVARIANT;

typedef struct tagAREAPTLIST
    {
    /* [string] */ unsigned char *szareaname;
    int numpts;
    /* [string][size_is][size_is] */ unsigned char **pszptnames;
    } 	AREAPTLIST;

typedef /* [public] */ 
enum __MIDL___MIDL_itf_hsctypes_0000_0001
    {	AREA_CHANGE	= 1,
	CON_DEL	= 2,
	CON_MAN_REFRESH	= 3
    } 	TCEVENTCODE;

typedef /* [switch_type] */ union tag_GDAVALUEUNION
    {
    /* [case()][string] */ unsigned char *pText;
    /* [case()] */ short int2;
    /* [case()] */ int int4;
    /* [case()] */ int8 int8;
    /* [case()] */ float real;
    /* [case()] */ double dble;
    /* [case()] */ ENUMITEM en;
    /* [case()] */ GDAERR Status;
    /* [case()] */ GDASERVAR servar;
    /* [case()] */ POINTREFLIST PointList;
    /* [case()] */ POINTREF2LIST PointList2;
    /* [case()] */ GDAVARIANT *pVar;
    /* [case()] */ HSCTIME time;
    /* [case()] */ GDAALARMHANDLE AlarmHandle;
    /* [default] */  /* Empty union arm */ 
    } 	GDAVALUEUNION;

typedef struct tag_GDAITEM
    {
    /* [switch_is] */ GDAVALUEUNION Value;
    short sValueType;
    uchar ucQuality;
    } 	GDAITEM;

typedef /* [public] */ struct __MIDL___MIDL_itf_hsctypes_0000_0002
    {
    unsigned char cEvent[ 4 ];
    } 	GDAHANDLE;

typedef /* [public] */ struct __MIDL___MIDL_itf_hsctypes_0000_0003
    {
    uint4 dwLrn;
    uint4 dwProcessId;
    uint4 dwThreadId;
    uint4 dwTransId;
    /* [string] */ unsigned char *szName;
    } 	GDACLIENT;

typedef int4 SRVCAVALPRM;

#define	CONTROLLER_MASK	( 0x1 )

#define	BACNET_MASK	( 0x2 )

#define	ALLPOINTS_MASK	( 0x4 )

#define	FIREPOINT_MASK	( 0x8 )

#define	PSDEFAULT	( "$$PSDEFAULT$$" )

#define	PSDEFAULT_SZ	( 14 )

#define	PSROOT	( "$$PSROOT$$" )

#define	PSROOT_SZ	( 11 )

#define	PSSYSINT	( "$$PSSYSINT$$" )

#define	PSSYSINT_SZ	( 13 )

#endif


extern RPC_IF_HANDLE __MIDL_itf_hsctypes_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_hsctypes_0000_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


