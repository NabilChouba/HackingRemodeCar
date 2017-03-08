
/**
 *  @file db_gen.h
 *
 *  This is a generated file.
 *  Created On : Mon Sep 07 19:42:17 2009
 */

/* 
 *  Copyright (C) 2000-2010. MindTree Ltd.
 *  All rights reserved.
 */

#ifndef _H_DB_GEN_
#define _H_DB_GEN_

/* ---------------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_dbase_api.h"

/* --------------------------------------- Global Definitions/Constants */
/* Number of UUIDs present in the SDDB */
#define DB_MAX_UUIDS                        5

/* Number of SDP Records in the SDDB */
#define DB_MAX_RECORDS                      2

#ifdef BT_EIR
/* Number of Service Class UUIDs */
#define SERVICE_CLASS_UUID_INDICES_MAX      2
#endif /* BT_EIR */

/* Service Record Indices */
#define DB_RECORD_SDP                       0

#define DB_RECORD_SPP                       1

#endif /* _H_DB_GEN_ */
