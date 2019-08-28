/************************************************************************/
/*****     COPYRIGHT 1988 - 2002    HONEYWELL LIMITED AUSTRALIA      ****/
/************************************************************************/
#ifndef environ_flag
#define environ_flag
/*
@(#)environ.h,v	830.4.1.1
BEGIN_DOC
---------------------------------------------------------------------------
ENVIRON.H - Environment variables
---------------------------------------------------------------------------
DESCRIPTION:
    This file lists all the environment variables used by SN90.

NOTES:
    $D/g_par_def is generated from this file and contains all the
		G_??? symbols for use in unixmsg() and g_par()

GLOBAL:
    These variables are used throughout the SN90 system.

    SN90 (do not use)	holds the root directory for sn90 (/usr/users/sn90)
    system_root_dir	holds the root directory (/usr/users/sn90/) for >>
    user_root_dir	holds the root directory (/usr/users/sn90/) for ^>
    root_dir		holds the root directory (/usr/users/sn90/) for >
    message_lib		holds the pathname of the message library for errmsg.c
    database_key nn	nn is the first key used in attaching the shared memory
    dump_core		if set then gbload will set SIG_DFL catchers

PARANOID:
    These variables allow in depth debugging and error reporting to be
    turned on and off with a running system. The integer value of a variable
    is its corresponding debugging level (the higher the number the more
    messages that come out) The standard values for these levels are
    defined by the identifiers L_ERROR, L_RANGE, L_PARAM ...
*/

#include "src/g_par_def"

// Define EXTERN_C
#ifndef EXTERN_C
#ifdef __cplusplus
    #define EXTERN_C    extern "C"
#else
    #define EXTERN_C    extern
#endif
#endif
// Define DLLIMPORT and DLLEXPORT
#ifndef DLLEXPORT
    #define DLLEXPORT __declspec( dllexport )
#endif
#ifndef DLLIMPORT
    #define DLLIMPORT __declspec( dllimport )
#endif

/* UNIX PARANOID STRUCTURE */
/* this structure is used by unixerr and unixmsg to remember the values */
/* of the paranoid environment variables */
#define MAXSIZEOFPARANOIDNAME	15
struct par
{
    long nLevel;                          // the current level of debugging for this variable
    char szName[MAXSIZEOFPARANOIDNAME+1];    // the environment variables name
};

#include "src/par_desc.h"

/* SN90 PARANOID INITIALISATION */
/* this initialises all the debugging levels to default so that on the first */
/* pass the environment variable is checked */
#if	defined(unixerr_flag)
	struct par* unixpar = NULL;
#elif	defined(unixpar_import)
	EXTERN_C DLLIMPORT struct par* unixpar;
#else
	EXTERN_C struct par* unixpar;
#endif  // unixerr_flag

EXTERN_C struct par * CreateUnixPar(long pid);
EXTERN_C struct par * GetUnixPar(long pid);
EXTERN_C struct par * GetUnixPar2(long pid, HANDLE *pMMFHandle);
EXTERN_C long ReleaseUnixPar(struct par * psp, HANDLE *pMMFHandle);
EXTERN_C long DoesUnixParExist(long pid);
EXTERN_C long GetParanoidDescriptionsArray(struct pardesc** prgspdDescription);

#ifdef dynpardll

/* NOTE the Paranoid Name can be no more than 15 characters */
    struct par unixpardll[] =
	{
	L_INVALID, "TASK",          // Task requesting and handling
	L_INVALID, "QUEUE",         // Queue handling
	L_INVALID, "SHM",           // Shared memory
	L_INVALID, "DAT",           // Data I/O routines
	L_INVALID, "ASCV",          // ASCII conversion routines and entry points
	L_INVALID, "BYTE",          // Byte manipulation routines
	L_INVALID, "GHR",           // Get hardware reference (ie. ipcghr, tdcghr)
	L_INVALID, "HIST",          // History routines
	L_INVALID, "LOCK",          // Lock routines
	L_INVALID, "IPSIO",         // Integrated Personal Station I/O
	L_INVALID, "KEY",           // Keyed files
	L_INVALID, "CMPRES",        // Compress routine for displays
	L_INVALID, "IPCIO",         // IPC PLC I/O
	L_INVALID, "MODBUS",        // Modbus protocol handling (ie. IPC)
	L_INVALID, "TIME",          // System time handling
	L_INVALID, "DEVIO",         // Set raw I/O attributes
	L_INVALID, "ERR",           // Communications errors and statistics
	L_INVALID, "ARITH",         // Real arithmetic (ie. fclamp, mzero, ssne)
	L_INVALID, "BIT",           // Bit manipulations
	L_INVALID, "SEM",           // Semaphore handling
	L_INVALID, "TDC",           // TDC routines
	L_INVALID, "GPMAIN",        // Database I/O and statistics
	L_INVALID, "ALM",           // Alarm counts (ie. alarm increment, alarm decrement)
	L_INVALID, "CRT",           // CRT area assignment
	L_INVALID, "PRINT",         // Printer handling
	L_INVALID, "NIF_MSGQ",      // Message queue interface routines
	L_INVALID, "NIF_TCP",       // TCP network interface routines
	L_INVALID, "NIF_AIF",       // NIF application interface routines
	L_INVALID, "NIF_SYS",       // General system call errors
	L_INVALID, "NIF_GEN",       // General NIF routines
	L_INVALID, "NIF_MSG",       // Message management routines
	L_INVALID, "TRACE",         // Trace messages
	L_INVALID, "APPLIC",        // Application programs
	L_INVALID, "R12IO",         // R1200 and R1200c I/O
	L_INVALID, "LNK",           // Link routines for dual redundant systems
	L_INVALID, "MODIO",         // MODICON I/O routines
	L_INVALID, "ABRIO",         // Allen Bradley I/O routines
	L_INVALID, "KNGIO",         // King Fisher  I/O routines
	L_INVALID, "PSWIO",         // Peripheral switch I/O routine
	L_INVALID, "CL",            // TDC CL compiler interface routines
	L_INVALID, "CHKSUM",        // Checksum generation routines
	L_INVALID, "IO",            // General I/O routines
	L_INVALID, "ASEAIO",        // Asea I/O routines
	L_INVALID, "ULPIO",         // MicroLPM I/O routines
	L_INVALID, "STRING",        // String manipulation routines
	L_INVALID, "BBLNK",         // Bristol Babcock link layer routines
	L_INVALID, "BBNET",         // Bristol Babcock network layer routines
	L_INVALID, "BBTRN",         // Bristol Babcock transport layer routines
	L_INVALID, "BBRDB",         // Bristol Babcock RDB routines
	L_INVALID, "BBRBE",         // Bristol Babcock RBE routines
	L_INVALID, "BBALM",         // Bristol Babcock alarm routines
	L_INVALID, "BBSCN",         // Bristol Babcock scan task routines
	L_INVALID, "DSPLY",         // Display task and routines
	L_INVALID, "BBTNS",         // Bristol Babcock transaction routines
	L_INVALID, "BBCFG",         // Bristol Babcock configurator routines
	L_INVALID, "DUAL_PSW",      // Dual Redundant PSW software
	L_INVALID, "DUAL_LINK",     // Dual Redundant link manager software
	L_INVALID, "DUAL_SYNC",     // Dual Redundant time synchronisation software
	L_INVALID, "DUAL_TIME",     // Dual Redundant time server software
	L_INVALID, "DUAL_QUEUE",    // Dual Redundant queue I/F software
	L_INVALID, "DUAL_ONLINE",   // Dual Redundant online backup software
	L_INVALID, "DUAL_OFFLINE",  // Dual Redundant offline backup software
	L_INVALID, "SCAN",          // Scan task software
	L_INVALID, "ECLIO",         // Excel I/O routines
	L_INVALID, "SQLRPT",        // SQL report program
	L_INVALID, "LCN",           // LCN scan task
	L_INVALID, "LICENCE",       // Licence management
	L_INVALID, "REPORT",        // Report software
	L_INVALID, "I18N",          // Internationalisation subsystem
	L_INVALID, "CONFIG",        // Configuration subsystem
	L_INVALID, "MA5IO",         // MA500 I/O routines
	L_INVALID, "WINPRT",        // Win32 printing routines
	L_INVALID, "CONFD",         // Configuration daemon
	L_INVALID, "CONFD_SELECT",  // Configuration daemon select subsystem
	L_INVALID, "APPIO",         // Applicom I/O routines
	L_INVALID, "SQDIO",         // Square D I/O routines
	L_INVALID, "RAD6500",       // Rad6500 scan task
	L_INVALID, "RADDAEMON",     // Rad6500 scan daemon
	L_INVALID, "DEFFILE",       // Get definition file routines
	L_INVALID, "S90CFG",        // Series 9000 cfgsum routines
	L_INVALID, "SYSCFG",        // System config (OS or SCAN/XSM/XFi)
	L_INVALID, "CCTV",          // CCTV scan tasks
	L_INVALID, "LIGHTS",        // Lighting control option
	L_INVALID, "G90IO",         // GE Fanuc series 90 I/O
	L_INVALID, "POINTS",        // Point database access
	L_INVALID, "DICT",          // Dictionary access
	L_INVALID, "XDIALUP",       // XLNET dialup program
	L_INVALID, "ODBC",          // ODBC interface
	L_INVALID, "NOTCLI",        // Notifications and alarms client
	L_INVALID, "EVARCH",        // Extended event archiving
	L_INVALID, "ALARM",         // Low level alarm manipulation
	L_INVALID, "CDA",           // Control data access routines
	L_INVALID, "LOGGER",        // Logger
	L_INVALID, "UMAXIO",        // UMAX I/O routines
	L_INVALID, "SPPREC",        // S9000 SPP and recipe paranoids
	L_INVALID, "COPYPROT",      // Copy protection
	L_INVALID, "UDCCFG",        // UDC cfgsum routines
	L_INVALID, "UDCDEF",        // UDC definition file routines
	L_INVALID, "MESSAGE",       // Message directory access
	L_INVALID, "FBUSIO",        // Fieldbus routines
	L_INVALID, "BACNET",        // BACnet server
	L_INVALID, "DFSIO",         // FS90
	L_INVALID, "OREPORT",       // Open database reporting
	L_INVALID, "OPC",           // OPC interface
	L_INVALID, "DFSDMN",        // FS90 daemon paranoids
	L_INVALID, "ZONE",          // Zone enforcement
	L_INVALID, "SHIFTMNGR",     // Shift manager
	L_INVALID, "ACSCFG",        // Access Configurator
	L_INVALID, "SMS7IO",        // Siemens S7 interface
	L_INVALID, "MMUIO",         // Moore mycro 352 I/O routines
	L_INVALID, "ALM_NOTIF",     // Alarm notification paranoids
	L_INVALID, "FSCIO",         // FSC routines
	L_INVALID, "SOE",           // FSCSOE routines
	L_INVALID, "XPC5",          // XFI - XLNET general
	L_INVALID, "XL5IO",         // XFI - I/O routines
	L_INVALID, "G80IO",         // GEM 80 I/O routines
	L_INVALID, "DIALUP",        // Dialup daemon routines
	L_INVALID, "DMNIO",         // Daemon I/O routines
	L_INVALID, "IVR",           // XFI - IVR routines
	L_INVALID, "PGAP",          // Perimeter global anti-passback routines
	L_INVALID, "H78IO",         // 7800 series burner controller paranoid
	L_INVALID, "OPCIO",         // OPC client routines
	L_INVALID, "GDA",           // Generic data access routines
	L_INVALID, "GDALOCK",       // GDA locking routines
	L_INVALID, "GDALOCAL",      // GDA local access routines
	L_INVALID, "GDAREM",        // GDA remote access routines
	L_INVALID, "GDANOTSRV",     // GDA notification server
	L_INVALID, "GDANOTCLI",     // GDA notification client
	L_INVALID, "UL",            // UL listed software
	L_INVALID, "BALYIO",        // Bailey scan task
	L_INVALID, "ALMPGR",        // Alarm pager
	L_INVALID, "MSLINKD",       // Multiple server link daemon
	L_INVALID, "XLSSCN",        // XLS scan task
	L_INVALID, "XLSDMN",        // XLS daemon
	L_INVALID, "I9KSCN",        // Intelliguard 9000 scan task
	L_INVALID, "I9KDMN",        // Intelliguard 9000 daemon
	L_INVALID, "I9KCFG",        // Intelliguard 9000 configuration task
	L_INVALID, "CMS",           // Card Management System
	L_INVALID, "MSVRDF",        // Multiserver report data factory server
	L_INVALID, "BACSCN",        // BACnet scan task paranoid
	L_INVALID, "APACIO",        // Moore APACS routines
	L_INVALID, "CONSVR",        // Server scripting event connection server
	L_INVALID, "AUTSVR",        // Server scripting automation server
	L_INVALID, "SCPPKG",        // Server scripting script package
	L_INVALID, "CHIP",          // SE chip paranoid
	L_INVALID, "BSIIO",         // Bristol Babcock open BSI interface
	L_INVALID, "POINTSERVER",   // PointServer development
	L_INVALID, "DNPSCN",        // DNP3 top level scantask routines
	L_INVALID, "DNPSCNIO",      // DNP3 I/O level scantask routines
	L_INVALID, "DNPDMN",        // DNP3 top level daemon routines
	L_INVALID, "DNPUSR",        // DNP3 user layer daemon routines
	L_INVALID, "DNPAL",         // DNP3 application layer daemon routines
	L_INVALID, "DNPTL",         // DNP3 transport layer daemon routines
	L_INVALID, "DNPDLL",        // DNP3 data link layer daemon routines
	L_INVALID, "DNPIO",         // DNP3 device I/O layer daemon routines
	L_INVALID, "TCMIO",         // Tecom scan task
	L_INVALID, "SQLREP",        // SQLRep CMS replication task
	L_INVALID, "SCPTSK",        // Server scripting script manager task
	L_INVALID, "SCPMNGR",       // Server scripting script manager
	L_INVALID, "SCPPERSIST",    // Server scripting persistance
	L_INVALID, "SCPEDIT",       // Server scripting editor
	L_INVALID, "TBLCURSOR",     // Table Cursor
	L_INVALID, "RSI",           // RSI Hand Reader
	L_INVALID, "GDASERVER",     // GDASERVER
	L_INVALID, "GDAOPCIO",      // GDA OPC client interface
	L_INVALID, "FILREP",        // File Replication
	L_INVALID, "HSTBCK",        // History Backfill
	L_INVALID, "NOTIFHOOKAPI",  // Notification Hook API
	L_INVALID, "DSREDIRECT",    // DSRedSrv and DSRedCli Redirection
	L_INVALID, "DSDCSRV",       // DS Network Components server side
	L_INVALID, "DSDCCLI",       // DS Network Components client side
	L_INVALID, "DSFILEREP",     // DSFileRepSrv, DSFileRepCli and DSAPI File Replication
	L_INVALID, "DSPNTREP",      // DSPntRepSrv, DSPntRepCli and DSAPI Point Replication
	L_INVALID, "DSACKSYNC",     // DSAckSync Alarm and Message Acknowledgement Synching
	L_INVALID, "DSCOMMON",      // DS Common Libraries
	L_INVALID, "ENERGY",        // Energy Manager
	L_INVALID, "BCMTOOLS",      // BizCalc Manager Tools
	L_INVALID, "BCMAUTOVAL",    // BizCalc Manager Auto Validator
	L_INVALID, "BCMPROCESSOR",  // BizCalc Manager Processor
	L_INVALID, "BCMSCHEDULER",  // BizCalc Manager Scheduler
	L_INVALID, "BCMSEQUENCER",  // BizCalc Manager Sequencer
	L_INVALID, "BCMEVENTPROC",  // BizCalc Manager Event Processor
	L_INVALID, "BCMMONITOR",    // BizCalc Manager Monitor
	L_INVALID, "BCMGENERAL",    // BizCalc Manager General
	L_INVALID, "SQLDB",         // SQL Database
	L_INVALID, "BCMALGORITHMS", // BizCalc Manager Algorithms
	L_INVALID, "ALMEVTATTRIBUTE", // alarm/event attributes
	L_INVALID, "NOTIFTRACE",	// trace alarm/events in notcli/gdanotci
	L_INVALID, "SECURITY",      // Used by login, and security library routines
	L_INVALID, "WTMGENERAL",    // Well Test Manager Common & Client
	L_INVALID, "WTMDATA",       // Well Test Manager Data Access
	L_INVALID, "WTMDEVICE",     // Well Test Manager Device Access
	L_INVALID, "WTMPROCESSOR",  // Well Test Manager Processor
	L_INVALID, "WTMSCHEDULER",  // Well Test Manager Scheduler
	L_INVALID, "HTS",           // History Tag Synchronization - General
	L_INVALID, "OPCDA",         // OPC DA interface
	L_INVALID, "OPCAE",         // OPC AE interface
	L_INVALID, "OPCHDA",        // OPC HDA interface
	L_INVALID, "GS",            // Global Schedules
	L_INVALID, "EAFGENERAL",    // Experion Application Framework - General
	L_INVALID, "EAFCONFIG",     // Experion Application Framework - Configuration
	L_INVALID, "EAFDESKTOP",    // Experion Application Framework - Desktop
	L_INVALID, "EAFDIAGNOSTICS",// Experion Application Framework - Diagnostics
	L_INVALID, "EAFEVENTS",     // Experion Application Framework - Events
	L_INVALID, "EAFRPDC",       // Experion Application Framework - RPDC
	L_INVALID, "EAFSECURITY",   // Experion Application Framework - Security
	L_INVALID, "EAFSTORAGE",    // Experion Application Framework - Storage
	L_INVALID, "GDAUSAGE",      // GDA usage statistics
	L_INVALID, "HTSTAG",		// History Tag Synchronization - Tag Output
	L_INVALID, "PNTAUDIT",		// Point audit messages
	L_INVALID, "SYSAUDIT",		// System audit messages
	L_INVALID, "TAGAUDIT",		// Tag audit messages
	L_INVALID, "PNTRELAUDIT",		// Point relation audit messages
	L_INVALID, "AUDIBLE",		// Annuniciation and silencing paranoids
	L_INVALID, "GCL",		// CDA-FTW 
	L_INVALID, "CDADATA",		// CDA-FTW 
	L_INVALID, "CDANOTIF",		// CDA-FTW 
	L_INVALID, "CDAPLATFORM",	// CDA-FTW 
	L_INVALID, "ER",		// CDA-FTW 
	L_INVALID, "SR",		// CDA-FTW 
	L_INVALID, "CHKPOINT",		// CDA-FTW 
	L_INVALID, "BOOTP",		// CDA-FTW 
	L_INVALID, "CTRLLIB",		// CDA-FTW 
	L_INVALID, "EMDB",		// CDA-FTW 
	L_INVALID, "BROWSER",		// CDA-FTW 
	L_INVALID, "NDM",		// CDA-FTW 
	L_INVALID, "DTL",		// CDA-FTW 
	L_INVALID, "ICOA",		// CDA-FTW 
	L_INVALID, "RCOA",		// CDA-FTW 
	L_INVALID, "OPCG",		// CDA-FTW 
	L_INVALID, "FUNCTRACE",		// Function tracing messages
	L_INVALID, "TIMER",		// Code performance timer and profiling messages
	// This next line is important for NT. Put new paranoids before it
	L_INVALID, ""
	};
/* NOTE the Paranoid Name can be no more than 15 characters */

#endif  // dynpardll

/*
END_DOC
*/

#endif	/* end of conditional include environ_flag */

/************************************************************************/
/*****     COPYRIGHT 1988 - 2002    HONEYWELL LIMITED AUSTRALIA      ****/
/************************************************************************/
