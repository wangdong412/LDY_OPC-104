#if	!defined(DT_CHAR)


/*	Data types */

#define	DT_CHAR	1
#define	DT_INT2	2
#define	DT_INT4	3
#define	DT_REAL	4
#define	DT_DBLE	5
#define	DT_HIST	6
#define	DT_VAR	7
#define	DT_ENUM	8
#define	DT_DATE_TIME	9
#define	DT_STATUS	10
#define	DT_SRCADDR	11
#define	DT_DSTADDR	12
#define	DT_SERVAR	13
#define	DT_POINTREF	14
#define	DT_INT8	15
#define	DT_TIME	16
#define	DT_DELTATIME	17
#define	DT_TIMEOFDAY	18
#define	DT_ALARMHANDLE	19
#define	DT_POINTREF2	20

/*	Data formats */

#define	DF_CHAR	1
#define	DF_NUM	2
#define	DF_POINT	3
#define	DF_PARAM	4
#define	DF_ENG	5
#define	DF_PCT	6
#define	DF_ENUM	7
#define	DF_MODE	8
#define	DF_BIT	9
#define	DF_STATE	10
#define	DF_PNTTYPE	11
#define	DF_TIME	12
#define	DF_DATE	13
#define	DF_DATE_TIME	14
#define	DF_GETVAL	15
#define	DF_DATE_TIME_UST	16

/*	Data access methods */

#define	DA_DATAIO	1
#define	DA_PRMDEF	2
#define	DA_PARAM	3
#define	DA_CDA	4
#define	DA_EVENT	5
#define	DA_QUE	6
#define	DA_HISTORY	7
#define	DA_CONSTANT	8
#define	DA_HIST_DTM	9
#define	DA_VARIABLE	10
#define	DA_POINT	11
#define	DA_FIELDBUS_MODE	12
#define	DA_ALMCOUNT	13
#define	DA_MSGCOUNT	14
#define	DA_PNTALM	15
#define	DA_RELATIVES	16
#define	DA_PNTBLK	17
#define	DA_FUNCTION	18
#define	DA_ALTCOUNT	19

/*	Point types */

#define	STA	1
#define	ANA	2
#define	ACC	3
#define	ACS	4
#define	CON	5
#define	ASH	6
#define	CLH	7
#define	SYS	8
#define	CDA	101
#define	RDA	102
#define	PSA	103

/*	Point subtypes */

#define	SYS_MODEL	1001
#define	SYS_SERVER	1002
#define	SYS_CONTROLLERS	1003
#define	SYS_CHANNEL	1004
#define	SYS_CONTROLLER	1005
#define	SYS_PRINTERS	1006
#define	SYS_PRINTER	1007
#define	SYS_SYSINTERFACES	1008
#define	SYS_SYSINTERFACE	1009
#define	SYS_STATIONS	1010
#define	SYS_FLEXSTATIONS	1011
#define	SYS_CONSSTATIONS	1012
#define	SYS_FLEXSTATION	1013
#define	SYS_CONSSTATION	1014
#define	SYS_CONSOLES	1015
#define	SYS_CONSOLE	1016
#define	SYS_CSTNCLIENT	1017
#define	SYS_UNASSIGNEDITEMS	1018
#define	SYS_COMPUTERS	1019
#define	SYS_COMPUTER	1020
#define	SYS_DOMAIN	1021
#define	SYS_ORGUNIT	1022
#define	SYS_NETEQUIP	1023
#define	SYS_SWITCH	1024
#define	SYS_FTE	1025
#define	SYS_FTECOMMUNITY	1026
#define	SYS_FTEDEVICE	1027
#define	SYS_9PSWITCH	1028
#define	SYS_UNKNOWNCONTROLLERS	1029
#define	PSA_SYSTEM_HIDDEN	1001

/*	Point parameters */

#define	param_PV	1
#define	PV	1
#define	param_MD	2
#define	MD	2
#define	param_OP	3
#define	OP	3
#define	param_EULO	16
#define	EULO	16
#define	param_EUHI	17
#define	EUHI	17
#define	param_NumberOfParents	30
#define	param_ACODE	31
#define	ACODE	31
#define	param_NAME	36
#define	NAME	36
#define	param_DESC	37
#define	DESC	37
#define	param_PVDAY	38
#define	PVDAY	38
#define	param_PVSEC	39
#define	PVSEC	39
#define	param_HBASE	40
#define	HBASE	40
#define	param_H1M	41
#define	H1M	41
#define	param_H6M	42
#define	H6M	42
#define	param_H1H	43
#define	H1H	43
#define	param_H8H	44
#define	H8H	44
#define	param_H24H	45
#define	H24H	45
#define	param_H5SF	46
#define	H5SF	46
#define	param_H1HE	47
#define	H1HE	47
#define	param_H8HE	48
#define	H8HE	48
#define	param_H24HE	49
#define	H24HE	49
#define	param_SCNSTS	56
#define	SCNSTS	56
#define	param_ALMSTS	57
#define	ALMSTS	57
#define	param_CNTLEV	58
#define	CNTLEV	58
#define	param_ALMINH	59
#define	ALMINH	59
#define	param_PVEROR	60
#define	PVEROR	60
#define	param_MDEROR	61
#define	MDEROR	61
#define	param_OPEROR	62
#define	OPEROR	62
#define	param_UNITS	68
#define	UNITS	68
#define	param_UNITS1	69
#define	UNITS1	69
#define	param_UNITS2	70
#define	UNITS2	70
#define	param_UNITS3	71
#define	UNITS3	71
#define	param_UNITS4	72
#define	UNITS4	72
#define	param_UNITS5	73
#define	UNITS5	73
#define	param_UNITS6	74
#define	UNITS6	74
#define	param_UNITS7	75
#define	UNITS7	75
#define	param_TARGET0	76
#define	TARGET0	76
#define	param_TARGET1	77
#define	TARGET1	77
#define	param_TARGET2	78
#define	TARGET2	78
#define	param_TARGET3	79
#define	TARGET3	79
#define	param_ENTZNE	80
#define	ENTZNE	80
#define	param_PVALG	85
#define	PVALG	85
#define	param_PVBLK	86
#define	PVBLK	86
#define	param_ACALG	87
#define	ACALG	87
#define	param_ACBLK	88
#define	ACBLK	88
#define	param_UNACKN	89
#define	UNACKN	89
#define	param_CNTPRI	91
#define	CNTPRI	91
#define	param_AL1PRI	92
#define	AL1PRI	92
#define	param_AL2PRI	93
#define	AL2PRI	93
#define	param_AL3PRI	94
#define	AL3PRI	94
#define	param_AL4PRI	95
#define	AL4PRI	95
#define	param_AL5PRI	96
#define	AL5PRI	96
#define	param_AL6PRI	97
#define	AL6PRI	97
#define	param_AL7PRI	98
#define	AL7PRI	98
#define	param_AL8PRI	99
#define	AL8PRI	99
#define	param_ALMSUB	100
#define	ALMSUB	100
#define	param_CNTSUB	101
#define	CNTSUB	101
#define	param_AL1SUB	102
#define	AL1SUB	102
#define	param_AL2SUB	103
#define	AL2SUB	103
#define	param_AL3SUB	104
#define	AL3SUB	104
#define	param_AL4SUB	105
#define	AL4SUB	105
#define	param_AL5SUB	106
#define	AL5SUB	106
#define	param_AL6SUB	107
#define	AL6SUB	107
#define	param_AL7SUB	108
#define	AL7SUB	108
#define	param_AL8SUB	109
#define	AL8SUB	109
#define	param_AssociatedTrend	110
#define	param_MAPALM	113
#define	param_ALRMSG	114
#define	param_REALARM	116
#define	param_EXTPVCHANGE	117
#define	param_EXTMDCHANGE	118
#define	param_EXTOPCHANGE	119
#define	param_EXTSPCHANGE	120
#define	param_AL1ARMED	121
#define	param_AL2ARMED	122
#define	param_AL3ARMED	123
#define	param_AL4ARMED	124
#define	param_AL5ARMED	125
#define	param_AL6ARMED	126
#define	param_AL7ARMED	127
#define	param_AL8ARMED	128
#define	param_IPWIDTH	129
#define	param_OPWIDTH	130
#define	param_NORMALMODE	131
#define	param_PULSEWIDTH	132
#define	param_OPREVERSE	133
#define	param_CTRLCONFIRM	134
#define	param_CTRLTIMEOUT	135
#define	param_ALMDEADBAND	137
#define	param_PvSource	140
#define	param_MdSource	141
#define	param_OpSource	142
#define	param_AckDestination	148
#define	param_MdDestination	149
#define	param_OpDestination	150
#define	param_PvPeriod	156
#define	param_MdPeriod	157
#define	param_OpPeriod	158
#define	param_PvController	164
#define	param_MdController	165
#define	param_OpController	166
#define	param_CreateTime	172
#define	param_AlarmPriority	181
#define	param_AlarmSubPriority	182
#define	param_SrvConNum	183
#define	param_SrvConLinkNum	184
#define	param_ItemName	185
#define	param_FullItemName	186
#define	param_DefaultName	187
#define	param_LocationFullItemName	188
#define	param_LocationTagName	189
#define	param_PointDetailDisplayDefault	200
#define	param_GroupDetailDisplayDefault	201
#define	param_Assignable	211
#define	param_OwnerTagName	212
#define	param_DisablePVAlarmJounaling	213
#define	param_Guid	234
#define	param_AssociatedDisplay	237
#define	param_InstructionDisplay	238
#define	param_PointType	291
#define	param_PointSubType	292
#define	param_PVALGEX	301
#define	param_ACALGEX	302
#define	param_INALM	303
#define	param_INALARM0	304
#define	param_INALARM1	305
#define	param_INALARM2	306
#define	param_INALARM3	307
#define	param_INALARM4	308
#define	param_INALARM5	309
#define	param_INALARM6	310
#define	param_INALARM7	311
#define	param_MDDISABL	319
#define	param_TotalUrgentRTNUnackAlarms	326
#define	param_TotalUrgentActiveUnackAlarms	327
#define	param_TotalUrgentActiveAckAlarms	328
#define	param_TotalUrgentRTNAckAlarms	329
#define	param_TotalUrgentRTNAlarms	330
#define	param_TotalUrgentUnackAlarms	331
#define	param_TotalUrgentActiveAlarms	332
#define	param_TotalUrgentAckAlarms	333
#define	param_TotalUrgentAlarms	334
#define	param_TotalHighRTNUnackAlarms	335
#define	param_TotalHighActiveUnackAlarms	336
#define	param_TotalHighActiveAckAlarms	337
#define	param_TotalHighRTNAckAlarms	338
#define	param_TotalHighRTNAlarms	339
#define	param_TotalHighUnackAlarms	340
#define	param_TotalHighActiveAlarms	341
#define	param_TotalHighAckAlarms	342
#define	param_TotalHighAlarms	343
#define	param_TotalLowRTNUnackAlarms	344
#define	param_TotalLowActiveUnackAlarms	345
#define	param_TotalLowActiveAckAlarms	346
#define	param_TotalLowRTNAckAlarms	347
#define	param_TotalLowRTNAlarms	348
#define	param_TotalLowUnackAlarms	349
#define	param_TotalLowActiveAlarms	350
#define	param_TotalLowAckAlarms	351
#define	param_TotalLowAlarms	352
#define	param_TotalRTNUnackAlarms	353
#define	param_TotalActiveUnackAlarms	354
#define	param_TotalActiveAckAlarms	355
#define	param_TotalRTNAckAlarms	356
#define	param_TotalRTNAlarms	357
#define	param_TotalUnackAlarms	358
#define	param_TotalActiveAlarms	359
#define	param_TotalAckAlarms	360
#define	param_TotalAlarms	361
#define	param_TotalUnackInfoMessages	362
#define	param_TotalUnackConfMessages	363
#define	param_TotalAckConfMessages	364
#define	param_TotalAckInfoMessages	365
#define	param_TotalInfoMessages	366
#define	param_TotalUnackMessages	367
#define	param_TotalConfMessages	368
#define	param_TotalAckMessages	369
#define	param_TotalMessages	370
#define	param_Parents	371
#define	param_Children	372
#define	param_TotalDisabledAlarms	373
#define	param_TotalDisabledAlerts	374
#define	param_TotalRTNAlerts	375
#define	param_TotalRTNUnackAlerts	376
#define	param_TotalRTNAckAlerts	377
#define	param_TotalActiveAlerts	378
#define	param_TotalActiveUnackAlerts	379
#define	param_TotalActiveAckAlerts	380
#define	param_TotalAckAlerts	381
#define	param_TotalUnackAlerts	382
#define	param_TotalAlerts	383
#define	param_ItemNumber	432
#define	param_SIGINH	462
#define	param_TotalShelvedAlarms	466
#define	param_TotalShelvedAlerts	467
#define	param_SP	4
#define	SP	4
#define	param_A1	5
#define	A1	5
#define	param_A2	6
#define	A2	6
#define	param_A3	7
#define	A3	7
#define	param_A4	8
#define	A4	8
#define	param_AL1	18
#define	AL1	18
#define	param_AL2	19
#define	AL2	19
#define	param_AL3	20
#define	AL3	20
#define	param_AL4	21
#define	AL4	21
#define	param_AT1	22
#define	AT1	22
#define	param_AT2	23
#define	AT2	23
#define	param_AT3	24
#define	AT3	24
#define	param_AT4	25
#define	AT4	25
#define	param_SPLO	32
#define	SPLO	32
#define	param_SPHI	33
#define	SPHI	33
#define	param_OPLO	34
#define	OPLO	34
#define	param_OPHI	35
#define	OPHI	35
#define	param_SPEROR	63
#define	SPEROR	63
#define	param_A1EROR	64
#define	A1EROR	64
#define	param_A2EROR	65
#define	A2EROR	65
#define	param_A3EROR	66
#define	A3EROR	66
#define	param_A4EROR	67
#define	A4EROR	67
#define	param_ALMPRI	90
#define	ALMPRI	90
#define	param_DFTDEADBAND	136
#define	param_CNTDEADBAND	138
#define	param_PVCLAMP	139
#define	param_SpSource	143
#define	param_A1Source	144
#define	param_A2Source	145
#define	param_A3Source	146
#define	param_A4Source	147
#define	param_SpDestination	151
#define	param_A1Destination	152
#define	param_A2Destination	153
#define	param_A3Destination	154
#define	param_A4Destination	155
#define	param_SpPeriod	159
#define	param_A1Period	160
#define	param_A2Period	161
#define	param_A3Period	162
#define	param_A4Period	163
#define	param_SpController	167
#define	param_A1Controller	168
#define	param_A2Controller	169
#define	param_A3Controller	170
#define	param_A4Controller	171
#define	param_AL5	173
#define	AL5	173
#define	param_AL6	174
#define	AL6	174
#define	param_AL7	175
#define	AL7	175
#define	param_AL8	176
#define	AL8	176
#define	param_AT5	177
#define	AT5	177
#define	param_AT6	178
#define	AT6	178
#define	param_AT7	179
#define	AT7	179
#define	param_AT8	180
#define	AT8	180
#define	param_USRALM1	296
#define	param_USRALM2	297
#define	param_USRALM3	298
#define	param_USRALM4	299
#define	param_RATEALM	304
#define	param_DVLOALM	305
#define	param_DVHIALM	306
#define	param_XTLOALM	307
#define	param_XTHIALM	308
#define	param_PVLOALM	309
#define	param_PVHIALM	310
#define	param_PVLOLOALM	311
#define	param_PVHIHIALM	312
#define	param_RSLOALM	313
#define	param_RSHIALM	314
#define	param_EXTA1CHANGE	315
#define	param_EXTA2CHANGE	316
#define	param_EXTA3CHANGE	317
#define	param_EXTA4CHANGE	318
#define	param_A1ASSOCP	320
#define	param_A2ASSOCP	321
#define	param_A3ASSOCP	322
#define	param_A4ASSOCP	323
#define	param_ROLOVR	26
#define	ROLOVR	26
#define	param_MF	27
#define	MF	27
#define	param_SF	28
#define	SF	28
#define	param_RAWPV	29
#define	RAWPV	29
#define	param_EXTZNE	81
#define	EXTZNE	81
#define	param_LSTACD	82
#define	LSTACD	82
#define	param_LSTACS	83
#define	LSTACS	83
#define	param_LSTACT	84
#define	LSTACT	84
#define	param_TIMEATT	324
#define	param_CREDENTIAL	325
#define	param_FlexStationAssignment	190
#define	param_FlexStationPermission	191
#define	param_ConsoleStationAssignment	192
#define	param_ConsoleStationPermission	193
#define	param_ConsoleAssignment	194
#define	param_ConsolePermission	195
#define	param_OperatorAssignment	196
#define	param_OperatorPermission	197
#define	param_ProfileListAssignment	198
#define	param_ProfileListPermission	199
#define	param_FlexStationPropagatedAssignment	214
#define	param_FlexStationPropagatedPermission	215
#define	param_ConsoleStationPropagatedAssignment	216
#define	param_ConsoleStationPropagatedPermission	217
#define	param_ConsolePropagatedAssignment	218
#define	param_ConsolePropagatedPermission	219
#define	param_OperatorPropagatedAssignment	220
#define	param_OperatorPropagatedPermission	221
#define	param_ProfileListPropagatedAssignment	222
#define	param_ProfileListPropagatedPermission	223
#define	param_FlexStationAssignmentMatchesDescendents	224
#define	param_FlexStationPermissionMatchesDescendents	225
#define	param_ConsoleStationAssignmentMatchesDescendents	226
#define	param_ConsoleStationPermissionMatchesDescendents	227
#define	param_ConsoleAssignmentMatchesDescendents	228
#define	param_ConsolePermissionMatchesDescendents	229
#define	param_OperatorAssignmentMatchesDescendents	230
#define	param_OperatorPermissionMatchesDescendents	231
#define	param_ProfileListAssignmentMatchesDescendents	232
#define	param_ProfileListPermissionMatchesDescendents	233
#define	param_AlarmPropagatedDisabled	235
#define	param_AlarmDisabledMatchesDescendents	236
#define	param_FlexStationPermAllowWrite	239
#define	param_FlexStationPermAllowPropagatedWrite	240
#define	param_ConsoleStationPermAllowWrite	241
#define	param_ConsoleStationPermAllowPropagatedWrite	242
#define	param_ConsolePermAllowWrite	243
#define	param_ConsolePermAllowPropagatedWrite	244
#define	param_OperatorPermAllowWrite	245
#define	param_OperatorPermAllowPropagatedWrite	246
#define	param_ProfileListPermAllowWrite	247
#define	param_ProfileListPermAllowPropagatedWrite	248
#define	param_StationEffectiveSecurityPermissions	290
#define	param_SignaturePropagatedDisabled	463
#define	param_SignatureDisabledMatchesDescendents	464
#define	param_AssociatedAssetTagName	209
#define	param_AssociatedAssetFullItemName	210
#define	param_Status	384
#define	param_PrimaryStatus	414
#define	param_BackupStatus	415
#define	param_BackupSyncStatus	416
#define	param_Link1Status	417
#define	param_Link2Status	418
#define	param_LastSyncTime	419
#define	param_LastLostSyncTime	420
#define	param_LastSyncDate	421
#define	param_LastLostSyncDate	422
#define	param_DataStatus	425
#define	param_NotificationStatus	426
#define	param_DSALink0State	433
#define	param_DSALink1State	434
#define	param_DSALink2State	435
#define	param_DSALink3State	436
#define	param_DSALink0FullName	437
#define	param_DSALink1FullName	438
#define	param_DSALink2FullName	439
#define	param_DSALink3FullName	440
#define	param_DSACurrentPrimaryServer	441
#define	param_IsRedundantPair	465
#define	param_LnkATotalRequests	385
#define	param_LnkBTotalRequests	386
#define	param_LnkATotalErrors	387
#define	param_LnkBTotalErrors	388
#define	param_LnkAPercentErrors	389
#define	param_LnkBPercentErrors	390
#define	param_BarFailLimit	391
#define	param_BarMarginalLimit	392
#define	param_LnkABarCount	393
#define	param_LnkBBarCount	394
#define	param_LinkAEnabled	442
#define	param_LinkBEnabled	443
#define	param_LinkAImplemented	444
#define	param_LinkBImplemented	445
#define	param_ConnectionType	446
#define	param_LinkAStatus	448
#define	param_LinkBStatus	449
#define	param_Enabled	395
#define	param_FontSize	396
#define	param_DetailStatus	431
#define	param_DataConnectionStatus	423
#define	param_NotificationConnectionStatus	424
#define	param_DataServerState	427
#define	param_NotificationServerState	428
#define	param_DataLink0State	451
#define	param_DataLink1State	452
#define	param_DataLink0Suffix	453
#define	param_DataLink1Suffix	454
#define	param_NotificationLink0State	455
#define	param_DataNetworkBaseName	456
#define	param_NotificationNetworkBaseName	457
#define	param_NotificationEnabled	458
#define	param_Type	459
#define	param_StationType	397
#define	param_EnableLowAlarms	398
#define	param_EnableHighAlarms	399
#define	param_EnableUrgentAlarms	400
#define	param_EnableFailAlarms	401
#define	param_EnableSignOffAlarms	402
#define	param_IPAddress	403
#define	param_IPName	404
#define	param_Operator	405
#define	param_NetworkName	430
#define	param_StationStatus	447
#define	param_DatacastStatus	408
#define	param_FileReplicationStatus	409
#define	param_PointReplicationStatus	410
#define	param_FileRedirectionStatus	411
#define	param_PointRedirectionStatus	412
#define	param_NotificationRedirectionStatus	413
#define	param_NodeStatus	429
#define	param_CDACommsDataConnection	460
#define	param_CDACommsNotificationConnection	461
#define	param_NumberOfStations	406
#define	param_OperatorSignOn	407
#define	param_ConsoleStatus	450
#define	param_QSTABL	112
#define	param_CEENUM	115
#define	param_PointDetailDisplayError	202
#define	param_GroupDetailDisplayError	208
#define	param_ReasonSetNumber	203
#define	param_PrimarySignatureMeaning	204
#define	param_SecondarySignerSecLevel	205
#define	param_SecondarySignatureMeaning	206
#define	param_ServicesAvailable	207

#endif	