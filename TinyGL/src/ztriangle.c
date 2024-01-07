#include <stdlib.h>
#include "zbuffer.h"
#include "zmath.h"

#define ZCMP(z,zpix) ((z) >= (zpix))

void ZB_fillTriangleFlat(ZBuffer *zb,
			 ZBufferPoint *p0,ZBufferPoint *p1,ZBufferPoint *p2)
{
#if TGL_FEATURE_RENDER_BITS == 24
    unsigned char colorR, colorG, colorB;
#else
    int color;
#endif

#define INTERP_Z

#if TGL_FEATURE_RENDER_BITS == 24 

#define DRAW_INIT()				\
{						\
  colorR=p2->b>>8; \
  colorG=p2->g>>8; \
  colorB=p2->r>>8; \
}

#define PUT_PIXEL(_a)		\
{						\
    zz=z >> ZB_POINT_Z_FRAC_BITS;		\
    if (ZCMP(zz,pz[_a])) {				\
      pp[3 * _a]=colorR;\
      pp[3 * _a + 1]=colorG;\
      pp[3 * _a + 2]=colorB;\
      pz[_a]=zz;				\
    }\
    z+=dzdx;					\
}

#else

#define DRAW_INIT()				\
{						\
  color=RGB_TO_PIXEL(p2->b,p2->g,p2->r);	\
}
  
#define PUT_PIXEL(_a)				\
{						\
    zz=z >> ZB_POINT_Z_FRAC_BITS;		\
    if (ZCMP(zz,pz[_a])) {				\
      pp[_a]=color;				\
      pz[_a]=zz;				\
    }						\
    z+=dzdx;					\
}
#endif /* TGL_FEATURE_RENDER_BITS == 24 */

#include "ztriangle.h"
}

/*
 * Smooth filled triangle.
 * The code below is very tricky :)
 */

void ZB_fillTriangleSmooth(ZBuffer *zb,
			   ZBufferPoint *p0,ZBufferPoint *p1,ZBufferPoint *p2)
{
#if TGL_FEATURE_RENDER_BITS == 16
        int _drgbdx;
#endif

#define INTERP_Z
#define INTERP_RGB

#define SAR_RND_TO_ZERO(v,n) (v / (1<<n))

#if TGL_FEATURE_RENDER_BITS == 24

#define DRAW_INIT() 				\
{						\
}

#define PUT_PIXEL(_a)				\
{						\
    zz=z >> ZB_POINT_Z_FRAC_BITS;		\
    if (ZCMP(zz,pz[_a])) {				\
      pp[3 * _a]=or1 >> 8;\
      pp[3 * _a + 1]=og1 >> 8;\
      pp[3 * _a + 2]=ob1 >> 8;\
      pz[_a]=zz;				\
    }\
    z+=dzdx;					\
    og1+=dgdx;					\
    or1+=drdx;					\
    ob1+=dbdx;					\
}

#elif TGL_FEATURE_RENDER_BITS == 16

#define DRAW_INIT() 				\
{						\
  _drgbdx=(SAR_RND_TO_ZERO(drdx,6) << 22) & 0xFFC00000;		\
  _drgbdx|=SAR_RND_TO_ZERO(dgdx,5) & 0x000007FF;		\
  _drgbdx|=(SAR_RND_TO_ZERO(dbdx,7) << 12) & 0x001FF000; 	\
}


#define PUT_PIXEL(_a)				\
{						\
    zz=z >> ZB_POINT_Z_FRAC_BITS;		\
    if (ZCMP(zz,pz[_a])) {				\
      tmp=rgb & 0xF81F07E0;			\
      pp[_a]=tmp | (tmp >> 16);			\
      pz[_a]=zz;				\
    }						\
    z+=dzdx;					\
    rgb=(rgb+drgbdx) & ( ~ 0x00200800);		\
}

#define DRAW_LINE()							   \
{									   \
  register unsigned short *pz;					   \
  register PIXEL *pp;					   \
  register unsigned int tmp,z,zz,rgb,drgbdx;				   \
  register int n;							   \
  n=(x2 >> 16) - x1;							   \
  pp=pp1+x1;								   \
  pz=pz1+x1;								   \
  z=z1;									   \
  rgb=(r1 << 16) & 0xFFC00000;						   \
  rgb|=(g1 >> 5) & 0x000007FF;						   \
  rgb|=(b1 << 5) & 0x001FF000;						   \
  drgbdx=_drgbdx;							   \
  while (n>=3) {							   \
    PUT_PIXEL(0);							   \
    PUT_PIXEL(1);							   \
    PUT_PIXEL(2);							   \
    PUT_PIXEL(3);							   \
    pz+=4;								   \
    pp+=4;								   \
    n-=4;								   \
  }									   \
  while (n>=0) {							   \
    PUT_PIXEL(0);							   \
    pz+=1;								   \
    pp+=1;								   \
    n-=1;								   \
  }									   \
}

#else

#define DRAW_INIT() 				\
{						\
}

#define PUT_PIXEL(_a)				\
{						\
    zz=z >> ZB_POINT_Z_FRAC_BITS;		\
    if (ZCMP(zz,pz[_a])) {				\
      pp[_a] = RGB_TO_PIXEL(or1, og1, ob1);\
      pz[_a]=zz;				\
    }\
    z+=dzdx;					\
    og1+=dgdx;					\
    or1+=drdx;					\
    ob1+=dbdx;					\
}

#endif /* TGL_FEATURE_RENDER_BITS */

#include "ztriangle.h"
}

void ZB_setTexture(ZBuffer *zb,PIXEL *texture, PIXEL *alpha, int slot)
{
    zb->current_textures[slot]=texture;
    zb->alphaMaps[slot]=alpha;
}

void ZB_fillTriangleMapping(ZBuffer *zb,
			    ZBufferPoint *p0,ZBufferPoint *p1,ZBufferPoint *p2)
{
    PIXEL *texture;

#define INTERP_Z
#define INTERP_ST

#define DRAW_INIT()				\
{						\
  texture=zb->current_textures[0];			\
}

#if TGL_FEATURE_RENDER_BITS == 24

#define PUT_PIXEL(_a)				\
{						\
   unsigned char *ptr;\
   zz=z >> ZB_POINT_Z_FRAC_BITS;		\
     if (ZCMP(zz,pz[_a])) {				\
       ptr = texture + (((t & 0x3FC00000) | s) >> 14) * 3; \
       pp[3 * _a]= ptr[0];\
       pp[3 * _a + 1]= ptr[1];\
       pp[3 * _a + 2]= ptr[2];\
       pz[_a]=zz;				\
    }						\
    z+=dzdx;					\
    s+=dsdx;					\
    t+=dtdx;					\
}

#else

#define PUT_PIXEL(_a)				\
{						\
   zz=z >> ZB_POINT_Z_FRAC_BITS;		\
     if (ZCMP(zz,pz[_a])) {				\
       pp[_a]=texture[((t & 0x3FC00000) | s) >> 14];	\
       pz[_a]=zz;				\
    }						\
    z+=dzdx;					\
    s+=dsdx;					\
    t+=dtdx;					\
}

#endif

#include "ztriangle.h"
}

/*
 * Texture mapping with perspective correction.
 * We use the gradient method to make less divisions.
 * TODO: pipeline the division
 */
#if 1

static void shader() {

}
#include <stdio.h>
void ZB_fillTriangleMappingPerspective(ZBuffer *zb,
                            ZBufferPoint *p0,ZBufferPoint *p1,ZBufferPoint *p2)
{
    PIXEL *diffuse, *alpha;
    float fdzdx,fndzdx,ndszdx,ndtzdx;

#define INTERP_Z
#define INTERP_RGB
#define INTERP_STZ

#define NB_INTERP 8
#define SAR_RND_TO_ZERO(v,n) (v / (1<<n))

#define DRAW_INIT()				\
{						\
  diffuse=zb->current_textures[0];\
  alpha=zb->alphaMaps[0];\
  fdzdx=(float)dzdx;\
  fndzdx=NB_INTERP * fdzdx;\
  ndszdx=NB_INTERP * dszdx;\
  ndtzdx=NB_INTERP * dtzdx;\
}


#if TGL_FEATURE_RENDER_BITS == 24
      //  dptr = diffuse + (((t & 0x3FC00000) | (s & 0x003FC000)) >> 14) * 3;\
      //  a = alpha[((t & 0x3FC00000) | (s & 0x003FC000)) >> 14];
//0xFA73
#define PUT_PIXEL(_a)				\
{						\
  unsigned char *dptr, a=255;\
  zz=z >> ZB_POINT_Z_FRAC_BITS;		\
    if (!zb->depthTest || ZCMP(zz,pz[_a])) {				\
      unsigned long long si = ((s+0xFFFF) & 0xFF0000) >> 16,\
      ti = ((t+0xFFFF) & 0xFF0000) >> 16;\
      float fa;\
      dptr = diffuse + (ti*zb->texW + si) * 3;\
      if (alpha) a = alpha[ti*zb->texW + si];\
      switch (zb->fragmentShader) {\
      case 2:\
        fa = 1.0f-gl_Max(gl_V2_Len(\
                    gl_Max(gl_Abs((float)si-(zb->quadW-1)/2) - (zb->quadW-1)/2+zb->radius, 0),\
                    gl_Max(gl_Abs((float)ti-(zb->quadH-1)/2) - (zb->quadH-1)/2+zb->radius, 0))-zb->radius, 0);\
        if (fa < 0) fa = 0.0f;\
        a *= fa;\
        pp[3 * _a + 0]= a*(ob1 >> 8)/0xFF + (0xFF-a)*pp[3*_a+0]/0xFF;\
        pp[3 * _a + 1]= a*(og1 >> 8)/0xFF + (0xFF-a)*pp[3*_a+1]/0xFF;\
        pp[3 * _a + 2]= a*(or1 >> 8)/0xFF + (0xFF-a)*pp[3*_a+2]/0xFF;\
      break;\
      case 1:\
        if (ti >= zb->texH-1 || si >= zb->texW-1) {\
          a = 0;\
        }\
        pp[3 * _a + 0]= a*(ob1 >> 8)/0xFF + (0xFF-a)*pp[3*_a+0]/0xFF;\
        pp[3 * _a + 1]= a*(og1 >> 8)/0xFF + (0xFF-a)*pp[3*_a+1]/0xFF;\
        pp[3 * _a + 2]= a*(or1 >> 8)/0xFF + (0xFF-a)*pp[3*_a+2]/0xFF;\
      break;\
      default:\
        pp[3 * _a + 0]= dptr[2]*(ob1 >> 8)/0xFF;\
        pp[3 * _a + 1]= dptr[1]*(og1 >> 8)/0xFF;\
        pp[3 * _a + 2]= dptr[0]*(or1 >> 8)/0xFF;\
      }\
      pz[_a]=zz;				\
  }						\
  z+=dzdx;					\
  s+=dsdx;					\
  t+=dtdx;					\
  og1+=dgdx;					\
  or1+=drdx;					\
  ob1+=dbdx;					\
}

#else

#define PUT_PIXEL(_a)				\
{						\
   zz=z >> ZB_POINT_Z_FRAC_BITS;		\
     if (ZCMP(zz,pz[_a])) {				\
       pp[_a]=*(PIXEL *)((char *)diffuse+ \
               (((t & 0x3FC00000) | (s & 0x003FC000)) >> (17 - PSZSH)));\
       pz[_a]=zz;				\
    }						\
    z+=dzdx;					\
    s+=dsdx;					\
    t+=dtdx;					\
}

#endif

#define DRAW_LINE()				\
{						\
  register unsigned short *pz;		\
  register PIXEL *pp;		\
  register long long s,t,z,zz;	\
  register unsigned int or1,og1,ob1; \
  register int n,dsdx,dtdx;		\
  float sz,tz,fz,zinv; \
  n=(x2>>16)-x1;                             \
  fz=(float)z1;\
  zinv=1.0 / fz;\
  pp=(PIXEL *)((char *)pp1 + x1 * PSZB); \
  pz=pz1+x1;					\
  z=z1;						\
  or1 = r1; \
  og1 = g1; \
  ob1 = b1; \
  sz=sz1;\
  tz=tz1;\
  while (n>=(NB_INTERP-1)) {						   \
    {\
      float ss,tt;\
      ss=(sz * zinv);\
      tt=(tz * zinv);\
      s=(int) ss;\
      t=(int) tt;\
      dsdx= (int)( (dszdx - ss*fdzdx)*zinv );\
      dtdx= (int)( (dtzdx - tt*fdzdx)*zinv );\
      fz+=fndzdx;\
      zinv=1.0 / fz;\
    }\
    PUT_PIXEL(0);							   \
    PUT_PIXEL(1);							   \
    PUT_PIXEL(2);							   \
    PUT_PIXEL(3);							   \
    PUT_PIXEL(4);							   \
    PUT_PIXEL(5);							   \
    PUT_PIXEL(6);							   \
    PUT_PIXEL(7);							   \
    pz+=NB_INTERP;							   \
    pp=(PIXEL *)((char *)pp + NB_INTERP * PSZB);\
    n-=NB_INTERP;							   \
    sz+=ndszdx;\
    tz+=ndtzdx;\
  }									   \
    {\
      float ss,tt;\
      ss=(sz * zinv);\
      tt=(tz * zinv);\
      s=(int) ss;\
      t=(int) tt;\
      dsdx= (int)( (dszdx - ss*fdzdx)*zinv );\
      dtdx= (int)( (dtzdx - tt*fdzdx)*zinv );\
    }\
  while (n>=0) {							   \
    PUT_PIXEL(0);							   \
    pz+=1;								   \
    pp=(PIXEL *)((char *)pp + PSZB);\
    n-=1;								   \
  }									   \
}
  
#include "ztriangle.h"
}

#endif

#if 0

/* slow but exact version (only there for reference, incorrect for 24
   bits) */

void ZB_fillTriangleMappingPerspective(ZBuffer *zb,
                            ZBufferPoint *p0,ZBufferPoint *p1,ZBufferPoint *p2)
{
    PIXEL *texture;

#define INTERP_Z
#define INTERP_STZ

#define DRAW_INIT()				\
{						\
  texture=zb->current_texture;			\
}

#define PUT_PIXEL(_a)				\
{						\
   float zinv; \
   int s,t; \
   zz=z >> ZB_POINT_Z_FRAC_BITS;		\
     if (ZCMP(zz,pz[_a])) {				\
       zinv= 1.0 / (float) z; \
       s= (int) (sz * zinv); \
       t= (int) (tz * zinv); \
       pp[_a]=texture[((t & 0x3FC00000) | s) >> 14];	\
       pz[_a]=zz;				\
    }						\
    z+=dzdx;					\
    sz+=dszdx;					\
    tz+=dtzdx;					\
}

#include "ztriangle.h"
}


#endif
