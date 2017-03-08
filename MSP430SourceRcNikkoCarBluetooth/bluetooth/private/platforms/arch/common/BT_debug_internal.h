
/**
 *  @file BT_debug_internal.h
 *
 *  Internal Header File of EtherMind Debug Library
 *  for Windows (User-mode).
 */

/*
 *  Copyright (C) 2000-2010. MindTree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_DEBUG_INTERNAL_
#define _H_BT_DEBUG_INTERNAL_

/* ----------------------------------------------- Header File Inclusion */

#include "BT_common.h"
#include "BT_debug_api.h"

/* ----------------------------------------------- Internal Functions */
#ifdef HAVE_ENCRYPTED_DEBUG
void BT_debug_encrypt_init (void);
void BT_debug_do_encrypt (UCHAR *buffer, UINT32 buflen);
#endif /* HAVE_ENCRYPTED_DEBUG */

#endif /* _H_BT_DEBUG_INTERNAL_ */

