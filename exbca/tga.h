#ifndef _TGA_H_
#define _TGA_H_

/*! TGA header. */
#pragma pack(push, 1)
typedef struct _tga_header
{
	unsigned char imgid_len;
	unsigned char pal_type;
	unsigned char img_type;
	unsigned short pal_start;
	unsigned short pal_len;
	unsigned char pal_ent_len;
	unsigned short origin_x;
	unsigned short origin_y;
	unsigned short img_w;
	unsigned short img_h;
	unsigned char bpp;
	unsigned char img_attr;
} tga_header_t;
#pragma pack(pop)

/* TGA context. */
typedef struct _tga_ctxt
{
	/*! X dimension. */
	int nx;
	/*! Y dimension. */
	int ny;
	/*! Pixels. */
	unsigned char **p[3];
} tga_ctxt_t;

int tga_read(const char *file, tga_ctxt_t *ctxt);
int tga_write(const char *file, tga_ctxt_t *ctxt);
void tga_set_pixel(tga_ctxt_t *ctxt, int x, int y, unsigned char r, unsigned char g, unsigned char b);
int tga_test_pixel(tga_ctxt_t *ctxt, int x, int y, unsigned char r, unsigned char g, unsigned char b);
void tga_line(tga_ctxt_t *ctxt, int xs, int ys, int xe, int ye, unsigned char r, unsigned char g, unsigned char b);
void tga_circle(tga_ctxt_t *ctxt, int x, int y, int ra, int rb, unsigned char r, unsigned char g, unsigned char b);
int tga_test_line(tga_ctxt_t *ctxt, int xs, int ys, int xe, int ye, unsigned char r, unsigned char g, unsigned char b);


#endif
