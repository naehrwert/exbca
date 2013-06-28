#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tga.h"

int tga_read(const char *file, tga_ctxt_t *ctxt)
{
	FILE *fp;
	tga_header_t tgah;
	int i, j, k;
	
	if((fp = fopen(file, "rb")) == NULL)
		return 0;

	fread(&tgah, sizeof(tga_header_t), 1, fp);

	//Only accept RGB uncompressed 24 bits images.
	if(!(tgah.img_type == 2 && tgah.bpp == 24))
	{
		fclose(fp);
		return 0;
	}

	//Allocate y lines.
	for(k = 0; k < 3; k++)
		ctxt->p[k] = (unsigned char **)malloc(sizeof(unsigned char *) * tgah.img_h);
	for(j = 0; j < tgah.img_h; j++)
	{
		//Allocate x lines.
		for(k = 0; k < 3; k++)
			ctxt->p[k][j] = (unsigned char *)malloc(sizeof(unsigned char) * tgah.img_w);
		for(i = 0; i < tgah.img_w; i++)
		{
			//Read RGB.
			unsigned int p = 0;
			for(k = 0; k < 3; k++)
			{
				unsigned char c;
				fread(&c, sizeof(unsigned char), 1, fp);
				ctxt->p[k][j][i] = c;
			}
		}
	}

	fclose(fp);

	ctxt->nx = tgah.img_w;
	ctxt->ny = tgah.img_h;

	return 1;
}

int tga_write(const char *file, tga_ctxt_t *ctxt)
{
	FILE *fp;
	tga_header_t tgah;
	int i, j, k;

	if((fp = fopen(file, "wb")) == NULL)
		return 0;

	memset(&tgah, 0, sizeof(tgah));
	tgah.img_type = 2; //RGB uncompressed.
	tgah.img_w = ctxt->nx;
	tgah.img_h = ctxt->ny;
	tgah.bpp = 24;

	fwrite(&tgah, sizeof(tga_header_t), 1, fp);

	for(j = 0; j < tgah.img_h; j++)
	{
		for(i = 0; i < tgah.img_w; i++)
		{
			for(k = 0; k < 3; k++)
				fwrite(&ctxt->p[k][j][i], sizeof(unsigned char), 1, fp);
		}
	}

	fclose(fp);

	return 1;
}

void tga_set_pixel(tga_ctxt_t *ctxt, int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
	ctxt->p[0][y][x] = b;
	ctxt->p[1][y][x] = g;
	ctxt->p[2][y][x] = r;
}

int tga_test_pixel(tga_ctxt_t *ctxt, int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
	if(ctxt->p[0][y][x] == b && 
		ctxt->p[1][y][x] == g &&
		ctxt->p[2][y][x] == r)
		return 1;
	return 0;
}

void tga_line(tga_ctxt_t *ctxt, int xs, int ys, int xe, int ye, unsigned char r, unsigned char g, unsigned char b)
{
	int dx = abs(xe - xs), sx = xs < xe ? 1 : -1;
	int dy = -abs(ye - ys), sy = ys < ye ? 1 : -1;
	int err = dx + dy, e2;
	
	while(1)
	{
		tga_set_pixel(ctxt, xs, ys, r, g, b);

		if(xs == xe && ys == ye)
			break;
		
		e2 = 2 * err;
		
		if(e2 > dy)
		{
			err += dy;
			xs += sx;
		}
		
		if(e2 < dx)
		{
			err += dx;
			ys += sy;
		}
	}
}

void tga_circle(tga_ctxt_t *ctxt, int x, int y, int ra, int rb, unsigned char r, unsigned char g, unsigned char b)
{
	int dx = 0, dy = rb;
	long long int a2 = ra * ra, b2 = rb * rb;
	long long int err = b2 - (2 * rb - 1) * a2, e2;

	do
	{
		tga_set_pixel(ctxt, x + dx, y + dy, r, g, b);
		tga_set_pixel(ctxt, x - dx, y + dy, r, g, b);
		tga_set_pixel(ctxt, x - dx, y - dy, r, g, b);
		tga_set_pixel(ctxt, x + dx, y - dy, r, g, b);

		e2 = 2 * err;
		
		if(e2 < (2 * dx + 1) * b2)
		{
			dx++;
			err += (2 * dx + 1) * b2;
		}

		if(e2 > -(2 * dy - 1) * a2)
		{
			dy--;
			err -= (2 * dy - 1) * a2;
		}
	} while(dy >= 0);

	while(dx++ < ra)
	{
		tga_set_pixel(ctxt, x + dx, y, r, g, b);
		tga_set_pixel(ctxt, x - dx, y, r, g, b);
	}
}

int tga_test_line(tga_ctxt_t *ctxt, int xs, int ys, int xe, int ye, unsigned char r, unsigned char g, unsigned char b)
{
	int dx = abs(xe - xs), sx = xs < xe ? 1 : -1;
	int dy = -abs(ye - ys), sy = ys < ye ? 1 : -1;
	int err = dx + dy, e2;
	int cnt = 0;
	
	while(1)
	{
		if(tga_test_pixel(ctxt, xs, ys, r, g, b))
			cnt++;

		if(xs == xe && ys == ye)
			break;
		
		e2 = 2 * err;
		
		if(e2 > dy)
		{
			err += dy;
			xs += sx;
		}
		
		if(e2 < dx)
		{
			err += dx;
			ys += sy;
		}
	}

	return cnt;
}
