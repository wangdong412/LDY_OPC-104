#if	!defined(M4_SYSTEM_OFFLINE)

/*                  M4_err_def - MOD400 error numbers */
/*                  --------------------------------- */

/*     SCCS REVISION: */
/*         @(#)M4_err_def,v	830.1.1.7 */

/*     DESCRIPTION: */
/*         This header contains all MOD400 error numbers used by SN90. */
/*         Refer to MOD400 Error messages for explanation. */


/*     system errors */

#define	M4_DATABASE_LOAD_ERRROR	0x83FA	
#define	M4_SYSTEM_OFFLINE	0x83FC	
#define	M4_SYSTEM_BACKUP	0x83FD	

/*     task termination return codes */

#define	M4_ERROR_GREATER_THAN_255	255
#define	M4_HAD_ERRORS	254
#define	M4_HAD_WARNINGS	253
#define	M4_NOT_LICENSED	252
#define	M4_BACKUP_AND_SYNCD	251

/*     communications errors */

#define	M4_PRINTER_FAULT	0x0001	
#define	M4_OFFLINE	0x0002	
#define	M4_NOPAPER	0x0004	
#define	M4_DEVICE_TIMEOUT	0x0106	 /*  timeout waiting for response */
#define	M4_HDW_ERR_STS_WRD	0x0107	 /*  parity err etc. invalid response */
#define	M4_DEVICE_UNAVAILABLE	0x010B	 /*  physical disconnect */
#define	M4_INCONSISTENT	0x010C	 /*  ie. trying to wrt when disconnected */
#define	M4_CONNECT_TIMEOUT	0x0110	 /*  timed out trying to connect */
#define	M4_GE6_TIMEOUT	0x0180	
#define	M4_HWY_TIMEOUT	0x8020	
#define	M4_HWY_ILLEGAL_PROTOCOL	0x8033	
#define	M4_RANGE	0x8130	 /*  unexpected range */
#define	M4_CHECKSUM	0x8131	 /*  received bad checksum */
#define	M4_NACKED	0x8132	 /*  received a NACK */
#define	M4_FUNCTION	0x8133	 /*  received invalid function */
#define	M4_GDA_ERROR	0x8150	 /*  GDA error */
#define	M4_GDA_WARNING	0x8151	 /*  GDA warning */
#define	M4_GDA_SERVERBLOCKED	0x8152	 /*  GDA server blocked */
#define	M4_GDA_COMMS_ERROR	0x8153	 /*  GDA communications error (only to be returned by the Network Server) */
#define	M4_GDA_COMMS_WARNING	0x8154	 /*  GDA communications warning (only to be returned by the Network Server) */
#define	M4_CDA_ERROR	0x8155	 /*  CDA error (only to be returned by the Network Server) */
#define	M4_CDA_WARNING	0x8156	 /*  CDA warning (only to be returned by the Network Server) */
#define	M4_GDA_CONNECTING	0x8157	 /*  Attempting to connect to a remote GDA server */
#define	M4_GDA_IN_GDA	0x8158	 /*  GDA attemting to call inself */
#define	M4_GDA_TPSINCOMPATIBLE	0x8159	 /*  Incompatible TPN Server build */
#define	M4_SERVER_UNAVAILABLE	0x82B5	 /*  Remote server unavailable */
#define	M4_GDA_KEEPALIVE_FAIL	0xC846	 /*  keep alive was not received */
#define	M4_GDA_LINK_FAIL	0xC847	 /*  network link failed */

/*     dat routine errors */

#define	M4_FILE_CURRENTLY_OPEN	0x0208	
#define	M4_FILE_NOT_FOUND	0x0209	
#define	M4_RECORD_BEYOND_EOF	0x020A	
#define	M4_RECORD_NOT_FOUND	0x020E	
#define	M4_CONCURRENCY	0x0213	
#define	M4_EOF_ERR	0x021F	
#define	M4_RECORD_LENGTH_ERR	0x021A	
#define	M4_DUPLICATE_KEY_ERR	0x021B	

/*     dictionary errors */

#define	M4_NO_ENTITY	0x82A0	
#define	M4_NO_ENTITY_NAME	0x82A1	
#define	M4_NO_ENTITY_NUMBER	0x82A2	
#define	M4_INV_ENTITY_TYPE	0x82A3	
#define	M4_ENTITY_NOT_FOUND	0x82A4	
#define	M4_ENTITY_EXISTS	0x82A5	
#define	M4_INV_ENTITY_NAME	0x82A6	

/*     shared heap errors */

#define	M4_HEAP_ID_INVALID	0x85F0	
#define	M4_HEAP_LOCK_INVALID	0x85F1	
#define	M4_HEAP_LOCKED	0x85F2	
#define	M4_HEAP_POINTER_INVALID	0x85F3	
#define	M4_HEAP_FREE_POINTER_INVALID	0x85F4	
#define	M4_HEAP_SIZE_DIFFERENT	0x85F5	
#define	M4_HEAP_MALLOC_TOO_BIG	0x85F6	
#define	M4_HEAP_INDEX_INVALID	0x85F7	
#define	M4_HEAP_INDEX_NOT_FOUND	0x85F8	
#define	M4_HEAP_LADDR_INVALID	0x85F9	
#define	M4_HEAP_CONTROL_INVALID	0x85FA	
#define	M4_HEAP_INDEX_ARRAY_NOT_FOUND	0x85FB	
#define	M4_HEAP_EXPAND_INVALID	0x85FC	
#define	M4_HEAP_EXPAND_LIMITED	0x85FD	
#define	M4_HEAP_RELOADED	0x85FE	

/*     semaphore errors */

#define	M4_UNAVAIL_SEM	0x0501	

/*     task requesting errors */

#define	M4_BUSY_TRB	0x0801	
#define	M4_ILLEGAL_LRN	0x0802	
#define	M4_ABORTED_LRN	0x0812	
#define	M4_DUPLICATE_LRN	0x0813	

/*     general errors ? */

#define	M4_INVALID_NO_ARGS	0x1202	
#define	M4_TOO_BIG_FOR_RECORD	0x2B09	
#define	M4_DATABASE_MOD	0x8400	
#define	M4_MUST_BE_UNIQUE	0x8249	
#define	M4_BLANK_AREA	0x9059	
#define	M4_INVALID_AREA	0x904E	
#define	M4_INV_ASSET_ACCESS	0x9032	
#define	M4_RECURSION_LIMIT	0xC84A	
#define	M4_PNTFUNC_UNSUPPORTED	0xC852	
#define	M4_INV_DATABASE_VERSION	0x90E8	

/*     point errors */

#define	M4_VALUE_CLAMPED_MIN	0x8088	
#define	M4_VALUE_CLAMPED_MAX	0x8089	
#define	M4_INV_RAISE	0x808A	
#define	M4_INV_LOWER	0x808B	
#define	M4_VALUE_BELOW_MIN	0x808c	
#define	M4_VALUE_ABOVE_MAX	0x808D	
#define	M4_VALUE_COULD_NOT_BE_CONVERTED	0x81F1	
#define	M4_PNT_ON_SCAN	0x8212	
#define	M4_PARAM_READONLY	0x8214	
#define	M4_INV_POINT	0x8231	
#define	M4_INV_PARAMETER	0x8232	
#define	M4_INV_OFFSET	0x8215	
#define	M4_INV_MODE	0x8142	
#define	M4_SOURCE_DELETED	0xC862	
#define	M4_OUT_OF_RANGE	0x821B	
#define	M4_OUT_OF_TOLERANCE	0x821C	
#define	M4_INF	0x821D	
#define	M4_NAN	0x821E	
#define	M4_BAD_VALUE	0x823E	
#define	M4_UNCERTAIN_VALUE	0xC863	      
#define	M4_POINT_NAME_EXISTS	0x82B0	
#define	M4_POINT_NUMBER_EXISTS	0x82B1	
#define	M4_PARAM_NAME_EXISTS	0x82B2	
#define	M4_PARAM_NUMBER_EXISTS	0x82B3	
#define	M4_INV_AREA	0x82B4	
#define	M4_MODIFIED_POINT	0x82B6	
#define	M4_STATE_UNKNOWN	0x82B7	
#define	M4_INV_TECHADDR_PNTEXISTS	0x82B8	
#define	M4_INV_TECHADDR	0x82B9	
#define	M4_INV_POINT_NAME	0x82BA	
#define	M4_INV_PNTENT_NAME	0xC85F	
#define	M4_BLANK_POINT_NAME	0xC860	
#define	M4_KEEP_INV_PARAM	0xC861	
#define	M4_INV_PARAM_NAME	0x82BB	
#define	M4_INV_AREA_NAME	0x82BC	
#define	M4_INV_POINT_TYPE	0x82FD	
#define	M4_INV_CONNECTION	0xC845	
#define	M4_INV_POINT_RELATION	0xC849	
#define	M4_INV_POINT_RECURSIVE_RELATION	0xC84B	
#define	M4_POINT_RELATION_EXISTS	0xC84C	
#define	M4_POINT_TOO_MANY_RELATIONS	0xC851	
#define	M4_DUPLICATES	0xC85E	
#define	M4_CTRLCNF_ENABLED	0x90E4	
#define	M4_MODECNF_ENABLED	0x90E5	
#define	M4_DIGITALPNT_SELECTED	0x90E6	
#define	M4_VALUE_CLAMPED	0x90E7	
#define	M4_SYSTEM_SUB_DISABLED	0x824E	
#define	M4_KEEP_INV_POINT	0xC864	

/*     device table error */

#define	M4_DEVICE_NOT_IMPLEM	0x8252	
#define	M4_DEVICE_NOT_IMPLEM_BYNAME	0x8253	

/*     queue handling errors */

#define	M4_QEMPTY	0x8501	
#define	M4_QINVALID_FN	0x8502	
#define	M4_QREC_TOO_SMALL	0x8503	
#define	M4_QFILE_TOO_SMALL	0x8504	
#define	M4_QINVALID_ADDR	0x8505	
#define	M4_QBEFORE_EOF	0x8506	
#define	M4_QBEYOND_EOF	0x8507	
#define	M4_QINCORRECT_PRI	0x8508	
#define	M4_QALREADY_USED	0x8509	
#define	M4_QLOCKED	0x850B	

/*     lock handling errors */

#define	FILLCK	0x8301	 /*  file locked to another task */
#define	RECLCK	0x8302	 /*  record locked to another task */
#define	DIRLCK	0x8303	 /*  directory locked */
#define	NOLCK	0x8304	 /*  locking not implemented */
#define	NORES	0x8305	 /*  file not mem resident */
#define	BADFIL	0x8306	 /*  bad file number */
#define	BADRECD	0x8307	 /*  bad record number */
#define	BADLRN	0x8308	 /*  bad lrn */
#define	NOLOAD	0x8309	 /*  directory not loaded */
#define	BADDELAY	0x830A	 /*  bad delay time (SN90) */
#define	LCK_TIMOUT	0x830B	 /*  time out before get lock (SN90) */

/*     file handling errors */

#define	M4_READ_ONLY	0x8200	 /*  read only */
#define	M4_BAD_OPEN	0x8201	 /*  open error */
#define	M4_BAD_READ	0x8202	 /*  read error */
#define	M4_BAD_WRITE	0x8203	 /*  write error */
#define	M4_BEYOND_VEC	0x8204	 /*  attempt to read outside of vector file */
#define	M4_BUF_SMALL	0x8205	 /*  buffer too small to receive data */
#define	M4_BAD_FUNC	0x8206	 /*  illegal function */
#define	M4_BAD_QUEUE	0x8207	 /*  queue full/empty on write/read of circular file */
#define	M4_BEYOND_FILE	0x8208	 /*  attempt to read outside file */
#define	M4_BAD_CLOSE	0x8209	 /*  close error */
#define	M4_BAD_FILE	0x820A	 /*  illegal file number */
#define	M4_RANGE_ERROR	0x820B	 /*  size of transfer exceeds 32K */
#define	M4_ILLEGAL_LFN	0x820C	 /*  illegal lfn */
#define	M4_NO_BACKUP	0x820D	 /*  backup access not permitted */
#define	M4_BAD_INTFLG	0x820E	 /*  illegal intflg value */
#define	M4_FILE_LOCKED	0x820F	 /*  file locked */
#define	M4_ILLEGAL_TYPE	0x8211	 /*  illegal type of data format */

/*     history collection errors */

#define	M4_ILLEGAL_VAL	0x8233	 /*  illegal number of values */
#define	M4_ILLEGAL_HST	0x8234	 /*  illegal history type */
#define	M4_VAL_NOT_FND	0x823B	 /*  value not found in history */

/*     scan errors */

#define	M4_INV_RTU	0x823C	 /*  rtu num. not specified */
#define	M4_NO_TBL_SPACE	0x823D	 /*  no table space to allocate to rtu */
#define	M4_ILLEGAL_RTU	0x8241	 /*  illegal rtu number */
#define	M4_ILLEGAL_CHN	0x8242	 /*  illegal channel number */
#define	M4_ILLEGAL_CHN_TYPE	0x8243	 /*  illegal channel type */
#define	M4_FAILED_RTU	0x8244	 /*  failed or oos rtu */
#define	M4_FAILED_CHN	0x8245	 /*  failed or oos channel */
#define	M4_NOT_CONVERTABLE	0x81F1	 /*  data not convertable */
#define	M4_LOADER_ERR	0x1601	

/*     multiserver notification errors */

#define	M4_ACKCON_FAIL	0x8F00	 /*  ack condition failed */
#define	M4_ACKPNT_FAIL	0x8F01	 /*  ack point failed */
#define	M4_ACKMOD_FAIL	0x8F02	 /*  connect failed due to incompatible ack modes */
#define	M4_ALMRSP_FAIL	0x8F03	 /*  alarm response failed */
#define	M4_ALMRSPACK_FAIL	0x8F04	 /*  alarm response failed because alarm was not acked */
#define	M4_ALMRSPRTN_FAIL	0x8F05	 /*  alarm response failed because alarm was not RTN */
#define	M4_ALMRST_FAIL	0x8F06	 /*  alarm reset failed */
#define	M4_ALMRSTACK_FAIL	0x8F07	 /*  alarm reset failed because alarm was not acked */
#define	M4_ALMRSTRSP_FAIL	0x8F08	 /*  alarm reset failed because alarm was not responded */
#define	M4_ALMRSTRTN_FAIL	0x8F09	 /*  alarm reset failed because alarm was not RTN */
#define	M4_ALM_NLE	0x8F0A	 /*  alarm no longer exists */
#define	M4_COMMENT_UNAVAILABLE	0x8F0B	 /*  user comment is unavailable */
#define	M4_COMMENT_REFRESH	0x8F0C	 /*  refresh the comment pane */
#define	M4_COMMENT_NOTSUPPORTED	0x8F0D	 /*  user comment is not supported on pre-800 remote DSA server */
#define	M4_GETCOMMENTS_FAIL	0x8F0E	 /*  get comments failed */
#define	M4_ADDCOMMENT_FAIL	0x8F0F	 /*  add comment failed */
#define	M4_PNT_ALREADYACKED	0x8F10	 /*  point already acked */
#define	M4_MSGCNF_FAIL	0x8F11	 /*  message confirm failed */
#define	M4_MSGCNFACK_FAIL	0x8F12	 /*  message confirm failed because message was not acked */
#define	M4_SHELVECON_FAIL	0x8F13	 /*  shelve condition failed */
#define	M4_ACK_ERROR	0x90E3	 /*  general acknowledgement error */

/*     direct station errors */

#define	M4_DSEVTSUMMARY_UNAVAILABLE	0x8F50	 /*  event summary is unavailable in degraded mode */
#define	M4_DSOPERATION_UNAVAILABLE	0x8F51	 /*  operation is unavailable in degraded mode */
#define	M4_DSDATA_UNAVAILABLE	0x8F52	 /*  data is unavailable in degraded mode */
#define	M4_DSEVTSUMMARY_REFRESH	0x8F53	 /*  refresh the event summary */
#define	M4_DSCFG_NOTLIC	0x8F54	 /*  Configuration failed, this exceeds the number of Console Stations licensed */
#define	M4_DSEXTCFG_NOTLIC	0x8F55	 /*  Configuration failed, this exceeds the number of Console Extension Stations licensed */
#define	M4_DSCFG_EXISTS	0x8F56	 /*  configuration failed, console station already exists */
#define	M4_DSNOTSUPPORTED	0x8F57	 /*  cannot execute on a console station */
#define	M4_DSEXTCFG_DENYDEL	0x8F58	 /*  configuration failed, console station client may not be deleted while in use */
#define	M4_DSCFG_DENYDEL	0x8F59	 /*  configuration failed, Console Station may not be deleted while in use */
#define	M4_DSCFG_NOLICENSE	0x8F68	 /*  Configuration failed, Console Stations not licensed. */
#define	M4_DSRTUSUM_OUTOFMEM	0x8F71	 /*  RTU Summary is unavailable because the server failed to allocate sufficient memory. */
#define	M4_DSCFG_DENYREN	0x8F72	 /*  configuration failed, Console Station may not be renamed while in use */
#define	M4_DSCFG_DENYCHG	0x8F73	 /*  configuration change denied, this field is auto-configured for correct operation of Console Stations */

/*     cross screen invocations errors */

#define	M4_DSDIFFCONSOLE	0x8F5A	 /*  destination is from a different console */
#define	M4_DSNOTCFG	0x8F5B	 /*  destination not configured */
#define	M4_DSINVALIDCFG	0x8F5C	 /*  invalid destination configuration */
#define	M4_DSSRCNOCONSOLE	0x8F5D	 /*  source console station is not part of a console */
#define	M4_DSDESTNOCONSOLE	0x8F5E	 /*  destination console station is not part of a console */
#define	M4_DSONLY	0x8F5F	 /*  operation is only allowed on a console station */
#define	M4_DSSRCDESTSAME	0x8F60	 /*  destination same as source */
#define	M4_DSSTNNOTRUN	0x8F61	 /*  destination console station client is not running */
#define	M4_DSRPCERR	0x8F62	 /*  unable to communicate with destination console station */
#define	M4_DSALIASNOTFOUND	0x8F63	 /*  alias not found in this console */
#define	M4_DSCSIINTERR	0x8F64	 /*  internal error */
#define	M4_DSCSISUCCESS	0x8F67	 /*  Display redirected successfully */

/*     console errors */

#define	M4_CONCFG_EXISTS	0x8F65	 /*  Configuration failed, this entry matches an existing Console */
#define	M4_CONCFG_DENYDEL	0x8F66	 /*  Configuration failed, Console may not be deleted while Console Stations configured to it */
#define	M4_CONCFG_NOLICENSE	0x8F69	 /*  Consoles require Console Station license */

/*     alarm processing errors */

#define	M4_ALM_NOT_ACK	0x9070	 /*  alarm has not been acknowledged */
#define	M4_ALM_NOT_RTN	0x9071	 /*  alarm has not returned to normal */
#define	M4_ALM_NOT_RSP	0x9072	 /*  alarm has not been responded to */


/*     Alert processing errors */

#define	M4_ALRT_NOT_ACK	0x90E1	 /*  alert has not been acknowledged */
#define	M4_ALRT_NOT_RTN	0x90E2	 /*  alert has not returned to normal */


/*     message processing errors */

#define	M4_MSG_NOT_ACK	0x9080	 /*  message has not been acknowledged */
#define	M4_MSG_NOT_CNF	0x9081	 /*  message has not been confirmed */

/*     IPS API errors */

#define	IPS_OBJECT_GET_ID_FAILED	0x9201	
#define	IPS_CHART_SET_AXIS_FAILED	0x9202	
#define	IPS_CHART_SET_DATA_FAILED	0x9203	
#define	IPS_CHART_NORMALISE_FAILED	0x9204	
#define	IPS_OBJECT_SET_ATTRIBUTE_FAILED	0x9205	

/*     Automation Server errors */

#define	M4_AS_ONE_SERVER_OBJ_ALLOWED	0x83B0	

/*     Redirection Manager errors */

#define	M4_RM_CONFIG_FAIL	0xC848	 /*  Redirection Manager configuration failed */

/*     Operator errors */

#define	M4_OPER_LVL_LOW	0x901F	 /*  operator level too low */
#define	M4_OPER_NON_EXIST	0xC842	 /*  operator not found in server */
#define	M4_OPER_BAD_RECORD	0xC843	 /*  operator record read error */
#define	M4_GET_USERNAME_ERROR	0xC844	 /*  get username error */

/*     Operator configuration errors */

#define	M4_OPER_NOT_UNIQE	0x8248	 /*  operator or windows group id is not unique */
#define	M4_OPER_NAME_INVALID_CHARS	0x8254	 /*  operator or windows group name has invalid characters */
#define	M4_OPER_DOMAIN_INVALID_CHARS	0x8255	 /*  operator or windows group domain has invalid characters */


#ifndef NAPI_CLIENT


/* Hardware Build devfmt errors */
#define	ILGADD	0x8261	 /*  Invalid syntax for an ADD statement */
#define	ILGDEV	0x8262	 /*  Invalid device type */
#define	ILGSTN	0x8263	 /*  Invalid station protocol definition */
#define	ILGLPT	0x8264	 /*  Invalid printer protocol definition */
#define	ILGCHN	0x8265	 /*  Invalid channel protocol definition */
#define	ILGTRM	0x8266	 /*  Invalid terminal protocol definition */
#define	VIRPRT	0x8267	 /*  Invalid virtual port name */
#define	SERPRT	0x8268	 /*  Invalid serial port name */
#define	PARPRT	0x8269	 /*  Invalid parallel port name */
#define	ILGSID	0x826A	 /*  Invalid system name */
#define	ILGLAN	0x826B	 /*  Invalid lan protocol */
#define	ILBAUD	0x826C	 /*  Invalid baud rate */
#define	ILSTOP	0x826D	 /*  Invalid number of stop bits */
#define	ILDATA	0x826E	 /*  Invalid number of data bits */
#define	ILPARI	0x826F	 /*  Invalid parity */
#define	ILCONN	0x8270	 /*  Invalid connect timeout */
#define	ILREAD	0x8271	 /*  Invalid read timeout */
#define	ILINTE	0x8272	 /*  Invalid inter character timeout */
#define	ILMINI	0x8273	 /*  Invalid minimum number of characters */
#define	ILNUMB	0x8274	 /*  Invalid device number */
#define	ILSUFF	0x8275	 /*  Invalid device suffix */
#define	ILCHCK	0x8276	 /*  Invalid checksum method */
#define	ILPOPT	0x8277	 /*  Invalid parallel option */
#define	ILSOPT	0x8278	 /*  Invalid serial option */
#define	I_SPACE1	0x8279	 /*  MOD400 error message */
#define	ILVOPT	0x827A	 /*  Invalid virtual option */
#define	ILLOPT	0x827B	 /*  Invalid lan option */
#define	ILIMOD	0x827C	 /*  Invalid input mode */
#define	ILDUPI	0x827D	 /*  Invalid duplex mode */
#define	ILECHI	0x827E	 /*  Invalid echo mode */
#define	ILRECON	0x827F	 /*  Invalid reconnect mode */
#define	ILGPSW	0x8280	 /*  Invalid psw definition */
#define	ILGLINK	0x8281	 /*  Invalid network link definition */
#define	ILNLOPT	0x8282	 /*  Invalid network link option */
#define	ILLTYPE	0x8283	 /*  Invalid network link type */
#define	ILRTYPE	0x8284	 /*  Invalid recovery method type */
#define	ILNPOPT	0x8285	 /*  Invalid network psw option */
#define	ILWRITE	0x8286	 /*  Invalid delay before write value */
#define	ILIDLE	0x8287	 /*  Invalid idle timeout */
#define	ILTOPT	0x8288	 /*  Invalid terminal server option */
#define	ILTSVBUFSZ	0x8289	 /*  Invalid terminal server buffer size */
#define	ILDMNPORT	0x828A	 /*  Invalid daemon port */
#define	ILDMNOPT	0x828B	 /*  Invalid daemon option */
#define	ILDMNMINSLT	0x828C	 /*  Invalid daemon minimum slots */
#define	ILDMNMAXSLT	0x828D	 /*  Invalid daemon maximum slots */
#define	ILDMNID	0x828E	 /*  Invalid daemon id */
#define	ILDMNFLG	0x828F	 /*  Invalid daemon flag */
#define	ILDMNVAL	0x8290	 /*  Invalid daemon value */
#define	ILDMNTYP	0x8291	 /*  Invalid daemon type */
#define	ILSTALL	0x8292	 /*  Invalid Stallion RS485 mode */
#define	IGNRD_REDUN_CMD	0x8293	 /*  Ignored redundant command */
#define	IL_READ_IDLE_COMB	0x8294	 /*  Invalid combination of idle and read timeouts */
#define	ILENCRTYPE	0x8296	 /*  Invalid encryption type */

/*         Control error status */

/*                                            0  ;* control not executed but ok */
#define	CTLOK	0x8220	 /*  control executed successfully */
#define	PNTNTI	0x8221	 /*  point not implemented */
#define	PNTNTA	0x8222	 /*  point not active */
#define	PARERR	0x8223	 /*  parameter error */
#define	PNTINH	0x8224	 /*  control inhibited for point */
#define	ILLRTU	0x8225	 /*  illegal rtu number */
#define	ILLCHN	0x8226	 /*  illegal channel number */
#define	RTUNTA	0x8227	 /*  rtu not active */
#define	CHNNTA	0x8228	 /*  channel not active */
#define	ILLMDV	0x8229	 /*  illegal mode for control */
#define	VALERR	0x822A	 /*  value error */
#define	CNTFUL	0x822B	 /*  control queue full */
#define	LVLNTA	0x822C	 /*  control level not adequate */
#define	TYP1A2	0x822D	 /*  only scntyp 1&2 controllable */
#define	CTLPRG	0x822E	 /*  control in progress */


#endif /* !NAPI_CLIENT */


/*     PROTOTYPES: */

/*       logical*2	hsc_IsError */
/*       logical*2	hsc_IsWarning */
/*       integer*2 hsc_GetLastError */



EXTERN_C int	hsc_IsError   (int);
EXTERN_C int	hsc_IsWarning (int);
EXTERN_C int  c_geterrno    (void);



#endif	
