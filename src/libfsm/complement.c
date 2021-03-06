/*
 * Copyright 2008-2017 Katherine Flavel
 *
 * See LICENCE for the full copyright terms.
 */

#include <assert.h>
#include <limits.h>
#include <stddef.h>

#include <fsm/fsm.h>
#include <fsm/pred.h>
#include <fsm/walk.h>
#include <fsm/bool.h>

#include "internal.h"

int
fsm_complement(struct fsm *fsm)
{
	size_t i;

	assert(fsm != NULL);

	if (!fsm_all(fsm, fsm_iscomplete)) {
		if (!fsm_complete(fsm, fsm_isany)) {
			fsm_free(fsm);
			return 0;
		}
	}

	/*
	 * For a Complete DFA, the complement of the language matched is given by
	 * marking all the previous end states as non-end, and all the previous
	 * non-end states as end.
	 */

	for (i = 0; i < fsm->statecount; i++) {
		fsm_setend(fsm, i, !fsm_isend(fsm, i));
	}

	if (fsm_trim(fsm, FSM_TRIM_START_REACHABLE, NULL) < 0) {
		return 0;
	}

	return 1;
}

