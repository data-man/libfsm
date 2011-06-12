/* $Id$ */

#include <assert.h>
#include <stdlib.h>

#include <fsm/fsm.h>

#include "internal.h"
#include "set.h"

struct fsm_state *
fsm_collateends(struct fsm *fsm)
{
	struct fsm_state *new;
	struct fsm_state *s;
	int endcount;

	assert(fsm != NULL);

	/* TODO: centralise */
	endcount = 0;
	for (s = fsm->sl; s != NULL; s = s->next) {
		endcount += !!fsm_isend(fsm, s);
	}

	switch (endcount) {
	case 0:
		return NULL;

	case 1:
		for (s = fsm->sl; !fsm_isend(fsm, s); s = s->next) {
			assert(s->next != NULL);
		}
		return s;

	default:
		new = fsm_addstate(fsm);
		if (new == NULL) {
			return NULL;
		}

		for (s = fsm->sl; s != NULL; s = s->next) {
			struct colour_set *c;

			if (!fsm_isend(fsm, s)) {
				continue;
			}

			for (c = s->cl; c != NULL; c = c->next) {
				if (!set_addcolour(fsm, &s->cl, c->colour)) {
					/* TODO: free new */
					return NULL;
				}
			}

			if (!fsm_addedge_epsilon(fsm, s, new)) {
				/* TODO: free new */
				return NULL;
			}

			fsm_removeends(fsm, s);
		}

		return new;
	}
}

