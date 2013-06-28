#ifndef _ANA_H_
#define _ANA_H_

#include "tga.h"

/*! Analysis context. */
typedef struct _ana_ctxt
{
	/*! TGA context. */
	tga_ctxt_t *img;
	/*! Midpoint X. */
	int mx;
	/*! Midpoint Y. */
	int my;
	/*! Inner radius. */
	int ria;
	int rib;
} ana_ctxt_t;

void ana_find_midpoint_inner_border(ana_ctxt_t *actxt, int delta);
void ana_mark_bca(ana_ctxt_t *ctxt, int thr_p, int thr_m);

#endif
