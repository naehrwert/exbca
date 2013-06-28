#include <stdio.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "ana.h"
#include "tga.h"

void ana_find_midpoint_inner_border(ana_ctxt_t *actxt, int delta)
{
	int mx, my, ex, ey;
	int xu, yu, xd, yd, xr, yr, xl, yl;
	
	ex = actxt->img->nx;
	ey = actxt->img->ny;
	mx = actxt->mx;
	my = actxt->my;

	//Up.
	xu = mx;
	for(yu = my; yu < ey; yu++)
	{
		if(!tga_test_pixel(actxt->img, xu, yu, 0xFF, 0xFF, 0xFF))
			break;
		if(!tga_test_pixel(actxt->img, xu - delta, yu, 0xFF, 0xFF, 0xFF))
			break;
		if(!tga_test_pixel(actxt->img, xu + delta, yu, 0xFF, 0xFF, 0xFF))
			break;
	}

	//Down.
	xd = mx;
	for(yd = my; yd > 0; yd--)
	{
		if(!tga_test_pixel(actxt->img, xd, yd, 0xFF, 0xFF, 0xFF))
			break;
		if(!tga_test_pixel(actxt->img, xd - delta, yd, 0xFF, 0xFF, 0xFF))
			break;
		if(!tga_test_pixel(actxt->img, xd + delta, yd, 0xFF, 0xFF, 0xFF))
			break;
	}

	//Right.
	yr = my;
	for(xr = mx; xr < ex; xr++)
	{
		if(!tga_test_pixel(actxt->img, xr, yr, 0xFF, 0xFF, 0xFF))
			break;
		if(!tga_test_pixel(actxt->img, xr, yr - delta, 0xFF, 0xFF, 0xFF))
			break;
		if(!tga_test_pixel(actxt->img, xr, yr + delta, 0xFF, 0xFF, 0xFF))
			break;
	}

	//Left.
	yl = my;
	for(xl = mx; xl > 0; xl--)
	{
		if(!tga_test_pixel(actxt->img, xl, yl, 0xFF, 0xFF, 0xFF))
			break;
		if(!tga_test_pixel(actxt->img, xl, yl - delta, 0xFF, 0xFF, 0xFF))
			break;
		if(!tga_test_pixel(actxt->img, xl, yl + delta, 0xFF, 0xFF, 0xFF))
			break;
	}

	//Transform midpoint.
	actxt->mx = mx + (xr - 2 * mx + xl) / 2;
	actxt->my = my + (yu - 2 * my + yd) / 2;

	//Calculate inner radius a and b.
	actxt->ria = (xr - mx + mx - xl)/2;
	actxt->rib = (yu - my + my - yd)/2;
}

void ana_mark_bca(ana_ctxt_t *ctxt, int thr_p, int thr_m)
{
	//t \in [0,1)
	//x = ra*cos(2*pi*t)
	//y = rb*sin(2*pi*t)

	float t;
	float spp = 1 / (2 * M_PI * ((ctxt->ria + ctxt->rib) / 2)) - 0.000006; //hackityhack
	//printf("%f\n", spp);
	int match = 0, mcnt = 0, idx = 0, lidx = 0;

	FILE *fp = fopen("testdata/idx.txt", "w");

	for(t = 0; t < 1.0; t += spp, idx++)
	{
		int sx = ctxt->mx + (ctxt->ria) * cos(2 * M_PI * t);
		int sy = ctxt->my + (ctxt->rib) * sin(2 * M_PI * t);

		int ex = ctxt->mx + (ctxt->ria + 80) * cos(2 * M_PI * t);
		int ey = ctxt->my + (ctxt->rib + 80) * sin(2 * M_PI * t);

		int cnt = tga_test_line(ctxt->img, sx, sy, ex, ey, 0x00, 0x00, 0x00);

		if(cnt > thr_p)
		{
			mcnt++;
			if(match == 0)
			{
				tga_line(ctxt->img, sx, sy, ex, ey, 0x00, 0x00, 0xFF);
				match = 1;

				fprintf(fp, "%d\n", (idx - lidx) / thr_m);
				lidx = idx;
			}
			else if(mcnt > thr_m)
			{
				tga_line(ctxt->img, sx, sy, ex, ey, 0x00, 0x00, 0xFF);
				mcnt = 1;

				fprintf(fp, "%d\n", 0);
				lidx = idx;
			}
		}
		else
		{
			match = 0;
			mcnt = 0;
		}
	}

	fclose(fp);
}
