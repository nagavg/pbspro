/*
 * Copyright (C) 1994-2016 Altair Engineering, Inc.
 * For more information, contact Altair at www.altair.com.
 *  
 * This file is part of the PBS Professional ("PBS Pro") software.
 * 
 * Open Source License Information:
 *  
 * PBS Pro is free software. You can redistribute it and/or modify it under the
 * terms of the GNU Affero General Public License as published by the Free 
 * Software Foundation, either version 3 of the License, or (at your option) any 
 * later version.
 *  
 * PBS Pro is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
 *  
 * You should have received a copy of the GNU Affero General Public License along 
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *  
 * Commercial License Information: 
 * 
 * The PBS Pro software is licensed under the terms of the GNU Affero General 
 * Public License agreement ("AGPL"), except where a separate commercial license 
 * agreement for PBS Pro version 14 or later has been executed in writing with Altair.
 *  
 * Altair’s dual-license business model allows companies, individuals, and 
 * organizations to create proprietary derivative works of PBS Pro and distribute 
 * them - whether embedded or bundled with other software - under a commercial 
 * license agreement.
 * 
 * Use of Altair’s trademarks, including but not limited to "PBS™", 
 * "PBS Professional®", and "PBS Pro™" and Altair’s logos is subject to Altair's 
 * trademark licensing policies.
 *
 */

/*	pbs_stathook.c
 *
 *	Return the status of a hook.
 */

#include <pbs_config.h>   /* the master config generated by configure */

#include "libpbs.h"
#include "pbs_ecl.h"


/**
 * @brief
 *	Return the status of a hook.
 *
 * @param[in] c - communication handle
 * @param[in] id - object name
 * @param[in] attrib - pointer to attrl structure(list)
 * @param[in] extend - extend string for req
 *
 * @return	structure handle
 * @retval	pointer to attr list	success
 * @retval	NULL			error
 *
 */
struct batch_status *
pbs_stathook(int c, char *id, struct attrl *attrib, char *extend)
{
	struct batch_status *ret = NULL;
	int                  rc;
	int		    hook_obj;

	if (extend != NULL) {
		if (strcmp(extend, PBS_HOOK) == 0) {
			hook_obj = MGR_OBJ_PBS_HOOK;
		} else if (strcmp(extend, SITE_HOOK) == 0) {
			hook_obj = MGR_OBJ_SITE_HOOK;
		} else {
			return NULL;	/* bad extend value */
		}
	} else {
		hook_obj = MGR_OBJ_SITE_HOOK;
	}

	/* initialize the thread context data, if not already initialized */
	if (pbs_client_thread_init_thread_context() != 0)
		return NULL;

	/* first verify the attributes, if verification is enabled */
	rc = pbs_verify_attributes(c, PBS_BATCH_StatusHook,
		hook_obj, MGR_CMD_NONE, (struct attropl *) attrib);
	if (rc)
		return NULL;

	if (pbs_client_thread_lock_connection(c) != 0)
		return NULL;

	ret = PBSD_status(c, PBS_BATCH_StatusHook, id, attrib, extend);

	/* unlock the thread lock and update the thread context data */
	if (pbs_client_thread_unlock_connection(c) != 0)
		return NULL;

	return ret;
}