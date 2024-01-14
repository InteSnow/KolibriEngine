#ifndef KOS32

#include <GL/gl.h>
#include "wingl.h"
#include "zgl.h"

typedef struct {
  GLContext *gl_context;
  int dx,dy;
  int x,y;
  HDC hDC;
  HDC hBufDC;
  HBITMAP hBufBM;
} TinyGLContext;


static void* framebuffer;

WINGLContext winglCreateContext(HDC hDC)
{
  TinyGLContext *ctx;

  ctx=gl_malloc(sizeof(TinyGLContext));
  if (!ctx)
    return NULL;
  ctx->gl_context=NULL;
  ctx->hDC = hDC;
  return (WINGLContext) ctx;
}

void winglDestroyContext(WINGLContext ctx)
{
  TinyGLContext *tctx = (TinyGLContext *) ctx;
  DeleteObject(tctx->hBufBM);
  DeleteDC(tctx->hBufDC);
  if (tctx->gl_context != NULL) {
    glClose();
  }
  gl_free(ctx);
}

/* resize the glx viewport : we try to use the xsize and ysize
   given. We return the effective size which is guaranted to be smaller */

static int gl_resize_viewport(GLContext *c,int *xsize_ptr,int *ysize_ptr)
{
  TinyGLContext *ctx;
  int xsize,ysize;
   
  ctx=(TinyGLContext *)c->opaque;

  xsize=*xsize_ptr;
  ysize=*ysize_ptr;

  /* we ensure that xsize and ysize are multiples of 2 for the zbuffer. 
     TODO: find a better solution */
  xsize&=~3;
  ysize&=~3;

  if (xsize == 0 || ysize == 0) return -1;

  *xsize_ptr=xsize-1;
  *ysize_ptr=ysize-1;
  ctx->dx = xsize;
  ctx->dy = ysize;
    
  /* resize the Z buffer */
  ZB_resize(c->zb,framebuffer,xsize,ysize);
  return 0;
}
#include <stdio.h>
/* we assume here that drawable is a window */
int winglMakeCurrent(WINGLContext ctx) 
{
  TinyGLContext *tctx = (TinyGLContext *) ctx;
  RECT r;
  GetClientRect(WindowFromDC(tctx->hDC), &r);
  int win_x0 = 0, win_y0 = 0;
  int win_x = r.right-r.left, win_y = r.bottom - r.top;
  win_x &= ~3;
  win_y &= ~3;
  int mode;
  ZBuffer *zb;
  if (tctx->hBufDC) {
    DeleteDC(tctx->hBufDC);
    DeleteObject(tctx->hBufBM);
  }

  BITMAPINFO bmi = {0};
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth = win_x;
  bmi.bmiHeader.biHeight = -win_y;
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 24;
  bmi.bmiHeader.biCompression = BI_RGB;

  tctx->hBufDC = CreateCompatibleDC(tctx->hDC);
  tctx->hBufBM = CreateDIBSection(tctx->hDC, &bmi, DIB_RGB_COLORS, &framebuffer, 0, 0);
  BITMAP bm = {};
  GetObject(tctx->hBufBM, sizeof(BITMAP), &bm);
  //SetBitmapBits(tctx->hBufBM, 3*tctx->dx*tctx->dy, tctx->gl_context->zb->pbuf);
  //tctx->hBufBM = CreateDIBSection(tctx->hDC, &bmi, DIB_RGB_COLORS, NULL, 0, 0);
  
  SelectObject(tctx->hBufDC, tctx->hBufBM);

  if (tctx->gl_context == NULL) {
    /* create the TinyGL context */
    tctx->x = win_x0;
	  tctx->y = win_y0;
	  tctx->dx = win_x;
	  tctx->dy = win_y;

    /* currently, we only support 24 bit rendering */
    mode = ZB_MODE_RGB24;
    zb=ZB_open(win_x,win_y,mode,0,NULL,NULL,framebuffer);      
    if (zb == NULL) {
      fprintf(stderr, "Error while initializing Z buffer\n");
      exit(1);
    }
    
    /* initialisation of the TinyGL interpreter */
    glInit(zb); 

    tctx->gl_context=gl_get_context();
      
    tctx->gl_context->opaque=(void *) ctx;
    tctx->gl_context->gl_resize_viewport=gl_resize_viewport;

    /* set the viewport : we force a call to gl_resize_viewport */
    tctx->gl_context->viewport.xsize=-1;
    tctx->gl_context->viewport.ysize=-1;
      
    glViewport(0, 0, win_x, win_y);
    
    // tctx->hBufDC = CreateCompatibleDC(tctx->hDC);
    // tctx->hBufBM = CreateCompatibleBitmap(tctx->hDC, tctx->dx, tctx->dy);
    // SetBitmapBits(tctx->hBufBM, tctx->dx*tctx->dy, (const void*)(tctx->gl_context->zb->pbuf));
    // SelectObject(tctx->hBufDC, tctx->hBufBM);
  }

  return 1;
}

void winglSwapBuffers()
{
  GLContext *gl_context;
  TinyGLContext *ctx;
    
  /* retrieve the current TinyGLContext */
  gl_context=gl_get_context();
  ctx=(TinyGLContext *)gl_context->opaque;

  SetBitmapBits(ctx->hBufBM, 3*ctx->dx*ctx->dy, ctx->gl_context->zb->pbuf);
  BitBlt(ctx->hDC,
    ctx->x, ctx->y,
    ctx->dx, ctx->dy,
    ctx->hBufDC,
    0, 0,
    SRCCOPY
  );

	// __asm__ __volatile__("int $0x40"::"a"(7),
  //                                     "b"((char *)gl_context->zb->pbuf),
	// 							      "c"((ctx->dx<<16)|ctx->dy),
	// 								  "d"((ctx->x<<16)|ctx->y));
}

#endif