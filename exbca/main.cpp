#include <stdio.h>

#include "tga.h"
#include "ana.h"

void _dbg_draw_inner_border(ana_ctxt_t *ctxt)
{
	tga_circle(ctxt->img, ctxt->mx, ctxt->my, ctxt->ria, ctxt->rib, 0xFF, 0x00, 0xFF);
	tga_line(ctxt->img, ctxt->mx, ctxt->my, ctxt->mx + ctxt->ria, ctxt->my, 0xFF, 0x00, 0xFF);
	tga_line(ctxt->img, ctxt->mx, ctxt->my, ctxt->mx - ctxt->ria, ctxt->my, 0xFF, 0x00, 0xFF);
	tga_line(ctxt->img, ctxt->mx, ctxt->my, ctxt->mx, ctxt->my + ctxt->rib, 0xFF, 0x00, 0xFF);
	tga_line(ctxt->img, ctxt->mx, ctxt->my, ctxt->mx, ctxt->my - ctxt->rib, 0xFF, 0x00, 0xFF);
}

int main(int argc, char **argv)
{
	tga_ctxt_t tga_ctxt;
	ana_ctxt_t ana_ctxt;
	
	printf("[+] Loading image...");
	tga_read("testdata/test.tga", &tga_ctxt);
	printf("done\n");

	printf("[+] Finding inner border and midpoint...");
	ana_ctxt.img = &tga_ctxt;
	ana_ctxt.mx = tga_ctxt.nx/2;
	ana_ctxt.my = tga_ctxt.ny/2;
	ana_find_midpoint_inner_border(&ana_ctxt, 1);
	ana_find_midpoint_inner_border(&ana_ctxt, 2);
	ana_find_midpoint_inner_border(&ana_ctxt, 3);
	ana_find_midpoint_inner_border(&ana_ctxt, 4);
	printf("done\n");

	_dbg_draw_inner_border(&ana_ctxt);

	printf("[+] Marking BCA cuts...");
	ana_mark_bca(&ana_ctxt, 15, 4);
	printf("done\n");

	printf("[+] Saving image...");
	tga_write("testdata/output.tga", &tga_ctxt);
	printf("done\n");

	getchar();
}
