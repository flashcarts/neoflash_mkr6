//
//
//     Welcome to join our develope team 
//      contact us now for more details
//
//    Mail&MSN: neoflash_team@hotmail.com
//           mIRC : #neo @ EFnet
//
//         http://www.neoflash.com     
//                                     
//

#include <stdarg.h>
#include <stdio.h>

#include <nds.h>

#include "frameBufPal.h"
#include "frameBuf.h"

#define BG_VWF_IMB_W   3200
#define BG_VWF_IMB_H   16
#define BG_VWF_SPC_W   3
#define BG_VWF_SPC_CH   0

typedef struct BG_VWF_DAT {
  u16 ch;
  u16 w;
  u32 x;
} BG_VWF_DAT;

#include "bg_vwf_bin.h"

BG_VWF_DAT m_bg_vwf_dat[] = {
  { '1', 4, 0, },
  { '2', 6, 4, },
  { '3', 6, 10, },
  { '4', 6, 16, },
  { '5', 6, 22, },
  { '6', 6, 28, },
  { '7', 6, 34, },
  { '8', 6, 40, },
  { '9', 6, 46, },
  { '0', 6, 52, },  //10

  { 'a', 6, 58, },
  { 'b', 6, 64, },
  { 'c', 6, 70, },
  { 'd', 6, 76, },
  { 'e', 6, 82, },
  { 'f', 5, 88, },
  { 'g', 6, 93, },
  { 'h', 6, 99, },
  { 'i', 2, 105, },
  { 'j', 4, 107, },
  { 'k', 5, 111, },
  { 'l', 3, 116, },
  { 'm', 8, 119, },
  { 'n', 6, 127, },
  { 'o', 6, 133, },
  { 'p', 6, 139, },
  { 'q', 6, 145, },
  { 'r', 5, 151, },
  { 's', 5, 156, },
  { 't', 5, 161, },
  { 'u', 6, 166, },
  { 'v', 6, 172, },
  { 'w', 6, 178, },
  { 'x', 6, 184, },
  { 'y', 6, 190, },
  { 'z', 6, 196, },  //26

  { 'A', 6, 202, },
  { 'B', 6, 208, },
  { 'C', 6, 214, },
  { 'D', 6, 220, },
  { 'E', 5, 226, },
  { 'F', 5, 231, },
  { 'G', 6, 236, },
  { 'H', 6, 242, },
  { 'I', 2, 248, },
  { 'J', 5, 250, },
  { 'K', 6, 255, },
  { 'L', 5, 261, },
  { 'M', 6, 266, },
  { 'N', 6, 272, },
  { 'O', 6, 278, },
  { 'P', 6, 284, },
  { 'Q', 6, 290, },
  { 'R', 6, 296, },
  { 'S', 5, 302, },
  { 'T', 6, 307, },
  { 'U', 6, 313, },
  { 'V', 6, 319, },
  { 'W', 6, 325, },
  { 'X', 6, 331, },
  { 'Y', 6, 337, },
  { 'Z', 5, 343, },  //26

  { '[', 4, 348, },
  { ']', 4, 352, },
  { '(', 4, 356, },
  { ')', 4, 360, },
  { '<', 4, 364, },
  { '>', 4, 368, },
  { '{', 5, 372, },
  { '}', 5, 377, },
  { '_', 6, 382, },
  { '+', 6, 388, },
  { '-', 6, 394, },
  { '+', 6, 400, },
  { '*', 8, 406, },
  { '/', 4, 414, },
  { '?', 5, 418, },
  { '.', 2, 423, },
  { ',', 3, 426, },
  { ';', 3, 429, },
  { ':', 2, 432, },
  { '`', 3, 434, },
  { '!', 2, 437, },
  { '@', 8, 439, },
  { '#', 8, 447, },
  { '$', 6, 455, },
  { '%', 8, 461, },
  { '^', 4, 469, },
  { '&', 7, 473, },
  { '~', 6, 480, },  //28

  { 0x00E0, 6, 487, },
  { 0x00E1, 6, 493, },
  { 0x00E2, 6, 499, },
  { 0x00E3, 6, 505, },
  { 0x00E8, 6, 511, },
  { 0x00E9, 6, 517, },
  { 0x00EA, 6, 523, },
  { 0x00EB, 6, 529, },
  { 0x00EC, 3, 535, },
  { 0x00ED, 3, 538, },
  { 0x00EE, 4, 541, },
  { 0x00EF, 4, 545, },
  { 0x00F2, 6, 549, },
  { 0x00F3, 6, 555, },
  { 0x00F4, 6, 561, },
  { 0x00F6, 6, 567, },
  { 0x0153, 8, 573, },
  { 0x00F9, 6, 581, },
  { 0x00FA, 6, 587, },
  { 0x00FB, 6, 593, },
  { 0x00FC, 6, 599, },
  { 0x00E7, 5, 605, },
  { 0x00F1, 6, 610, },
  { 0x00DF, 6, 616, },
  { 0x00C0, 6, 622, },
  { 0x00C1, 6, 628, },
  { 0x00C2, 6, 634, },
  { 0x00C4, 6, 640, },
  { 0x00C8, 5, 646, },
  { 0x00C9, 5, 651, },
  { 0x00CA, 5, 656, },
  { 0x00CB, 5, 661, },
  { 0x00CC, 3, 666, },
  { 0x00CD, 3, 669, },
  { 0x00CE, 4, 672, },
  { 0x00CF, 4, 676, },
  { 0x00D2, 6, 680, },
  { 0x00D3, 6, 686, },
  { 0x00D4, 6, 692, },
  { 0x00D6, 6, 698, },
  { 0x0152, 8, 704, },
  { 0x00D9, 6, 712, },
  { 0x00DA, 6, 718, },
  { 0x00DB, 6, 724, },
  { 0x00DC, 6, 730, },
  { 0x00C7, 5, 738, },
  { 0x00D1, 6, 741, },
  { 0x00A1, 2, 747, },
  { 0x00BF, 5, 749, },
  { 0x20AC, 7, 754, },
  { 0x00A2, 7, 761, },
  { 0x00A3, 6, 768, },  //52


  { 0x2033, 6, 774, },
  { 0x0027, 2, 780, },
  { 0xFF5E,10, 782, },
  { 0x00D7, 6, 792, },
  { 0x00F7, 6, 798, },
  { 0x2192,10, 804, },
  { 0x2190,10, 814, },
  { 0x2191, 6, 824, },
  { 0x2193, 6, 830, },
  { 0x300C, 5, 836, },
  { 0x300D, 5, 841, },
  { 0x201C, 5, 846, },
  { 0x201D, 5, 846, },
  { 0x2022, 5, 851, },
  { 0x203B,10, 856, },
  { 0x3012,10, 866, },
  { 0x266D, 6, 876, },
  { 0x266A, 7, 882, },
  { 0x00B1, 6, 889, },
  { 0x005C, 4, 895, },
  { 0x00B0, 4, 899, },
  { 0xFF5C, 2, 903, },
  { 0xFF0F,10, 905, },
  { 0xFF3C,10, 915, },
  { 0x221E,10, 925, },
  { 0x2234, 9, 935, },
  { 0x2026, 6, 944, },
  { 0x2122,10, 950, },
  { 0x00A9,10, 960, },
  { 0x00AE,10, 970, },
  { 0xE008,10, 980, },
  { 0xE009,10, 990, },
  { 0xE00A,10, 1000, },
  { 0xE00B,10, 1010, },
  { 0xE00C,10, 1020, },
  { 0xE00D,10, 1030, },
  { 0xE00E,10, 1040, },
  { 0xE00F, 8, 1050, },
  { 0xE012,10, 1058, },
  { 0xE013, 6, 1068, },  //39

  { 0xE007,10, 1074, },
  { 0xE000,10, 1084, },
  { 0xE001,10, 1094, },
  { 0xE002,10, 1104, },
  { 0xE003,10, 1114, },
  { 0xE004,10, 1124, },
  { 0xE005,10, 1134, },
  { 0xE006,10, 1144, },
  { 0xE015,10, 1154, },
  { 0xE016,10, 1164, },
  { 0xE017,10, 1174, },
  { 0xE018,10, 1184, },
  { 0xE010,10, 1194, },
  { 0xE02D,10, 1204, },
  { 0x2606,10, 1214, },
  { 0x25CB,10, 1224, },
  { 0x25C7,10, 1234, },
  { 0x25A1,10, 1244, },
  { 0x25B3,10, 1254, },
  { 0x25BD,10, 1264, },
  { 0x25CE,10, 1274, },
  { 0xE019,10, 1284, },
  { 0xE01A,10, 1294, },
  { 0xE01B,10, 1304, },
  { 0xE01C,10, 1314, },
  { 0x2605,10, 1324, },
  { 0x25CF,10, 1334, },
  { 0x25C6,10, 1344, },
  { 0x25A0,10, 1354, },
  { 0x25B2,10, 1364, },
  { 0x25BC,10, 1374, },
  { 0xE028,10, 1384, },  //32

  { 0x3042,10, 1395, },
  { 0x3044,10, 1405, },
  { 0x3046, 9, 1415, },
  { 0x3048,10, 1424, },
  { 0x304A,11, 1434, },
  { 0x304B,10, 1445, },
  { 0x304D,10, 1455, },
  { 0x304F, 8, 1465, },
  { 0x3051,10, 1473, },
  { 0x3053,10, 1483, },
  { 0x3055, 9, 1493, },
  { 0x3057, 8, 1502, },
  { 0x3059,10, 1510, },
  { 0x305B,10, 1520, },
  { 0x305D,10, 1530, },
  { 0x305F, 9, 1540, },
  { 0x3061, 9, 1549, },
  { 0x3064,10, 1558, },
  { 0x3066,10, 1568, },
  { 0x3068, 9, 1578, },
  { 0x306A,10, 1587, },
  { 0x306B,10, 1597, },
  { 0x306C,10, 1607, },
  { 0x306D,10, 1617, },
  { 0x306E,10, 1627, },
  { 0x306F,10, 1637, },
  { 0x3072,10, 1647, },
  { 0x3075,10, 1657, },
  { 0x3078,10, 1667, },
  { 0x307B,10, 1677, },
  { 0x307E,10, 1687, },
  { 0x307F,10, 1697, },
  { 0x3080,10, 1707, },
  { 0x3081,10, 1717, },
  { 0x3082, 9, 1727, },
  { 0x3084,10, 1736, },
  { 0x3086,10, 1746, },
  { 0x3088, 9, 1756, },
  { 0x3089, 9, 1765, },
  { 0x308A, 8, 1774, },
  { 0x308B,10, 1782, },
  { 0x308C,10, 1792, },
  { 0x308D,10, 1802, },
  { 0x308F,10, 1812, },
  { 0x3092,10, 1822, },
  { 0x3093,10, 1832, },

  { 0x304C,12, 1842, },
  { 0x304E,13, 1854, },
  { 0x3050,12, 1867, },
  { 0x3052,13, 1879, },
  { 0x3054,12, 1892, },
  { 0x3056,12, 1904, },
  { 0x3058,10, 1916, },
  { 0x305A,12, 1926, },
  { 0x305C,12, 1938, },
  { 0x305E,12, 1950, },
  { 0x3060,13, 1962, },
  { 0x3062,12, 1975, },
  { 0x3065,12, 1987, },
  { 0x3067,12, 1999, },
  { 0x3069,12, 2011, },
  { 0x3070,13, 2023, },
  { 0x3073,13, 2036, },
  { 0x3076,12, 2049, },
  { 0x3079,12, 2061, },
  { 0x307C,14, 2073, },
  { 0x3094,12, 2087, },
  { 0x3071,14, 2099, },
  { 0x3074,13, 2113, },
  { 0x3077,13, 2126, },
  { 0x307A,12, 2139, },
  { 0x307D,14, 2151, },  //72

  { 0x30A2, 9, 2165, },
  { 0x30A4, 8, 2174, },
  { 0x30A6, 9, 2182, },
  { 0x30A8,10, 2191, },
  { 0x30AA,10, 2201, },
  { 0x30AB,10, 2211, },
  { 0x30AD, 9, 2221, },
  { 0x30AF, 9, 2230, },
  { 0x30B1,10, 2239, },
  { 0x30B3, 9, 2249, },
  { 0x30B5,10, 2258, },
  { 0x30B7, 9, 2268, },
  { 0x30B9,11, 2277, },
  { 0x30BB,10, 2288, },
  { 0x30BD,10, 2298, },
  { 0x30BF,10, 2308, },
  { 0x30C1,10, 2318, },
  { 0x30C4,10, 2328, },
  { 0x30C6,10, 2338, },
  { 0x30C8, 7, 2348, },
  { 0x30CA,10 ,2355, },
  { 0x30CB,10, 2365, },
  { 0x30CC, 9, 2375, },
  { 0x30CD,10, 2384, },
  { 0x30CE, 8, 2394, },
  { 0x30CF,10, 2402, },
  { 0x30D2, 9, 2412, },
  { 0x30D5, 9, 2421, },
  { 0x30D8,10, 2430, },
  { 0x30DB,10, 2440, },
  { 0x30DE,10, 2450, },
  { 0x30DF, 9, 2460, },
  { 0x30E0,10, 2469, },
  { 0x30E1, 9, 2479, },
  { 0x30E2,10, 2488, },
  { 0x30E4,10, 2498, },
  { 0x30E6,10, 2508, },
  { 0x30E8, 8, 2518, },
  { 0x30E9, 9, 2526, },
  { 0x30EA, 9, 2535, },
  { 0x30EB,10, 2544, },
  { 0x30EC, 8, 2554, },
  { 0x30ED, 9, 2562, },
  { 0x30EF, 9, 2571, },
  { 0x30F2, 9, 2580, },
  { 0x30F3,10, 2589, },
  { 0x30AC,13, 2599, },
  { 0x30AE,13, 2612, },
  { 0x30B0,13, 2625, },
  { 0x30B2,13, 2638, },
  { 0x30B4,12, 2651, },

  { 0x30B6,13, 2663, },
  { 0x30B8,12, 2676, },
  { 0x30BA,13, 2688, },
  { 0x30BC,13, 2701, },
  { 0x30BE,13, 2714, },

  { 0x30C0,13, 2727, },
  { 0x30C2,13, 2740, },
  { 0x30C5,13, 2753, },
  { 0x30C7,13, 2766, },
  { 0x30C9, 8, 2779, },

  { 0x30D0,13, 2787, },
  { 0x30D3,12, 2800, },
  { 0x30D6,13, 2812, },
  { 0x30D9,11, 2825, },
  { 0x30DC,13, 2836, },

  { 0x30F4,13, 2849, },

  { 0x30D1,13, 2862, },
  { 0x30D4,12, 2875, },
  { 0x30D7,13, 2887, },
  { 0x30DA,10, 2900, },
  { 0x30DD,12, 2910, },  //70

  { 0x3041, 9, 2922, },
  { 0x3043, 7, 2931, },
  { 0x3045, 7, 2938, },
  { 0x3047, 8, 2945, },
  { 0x3049,11, 2953, },
  { 0x3083, 9, 2964, },
  { 0x3085, 9, 2973, },
  { 0x3087, 9, 2982, },
  { 0x308E, 9, 2991, },
  { 0x30A1, 8, 3000, },
  { 0x30A3, 7, 3008, },
  { 0x30A5, 8, 3015, },
  { 0x30A7, 8, 3023, },
  { 0x30A9, 8, 3031, },
  { 0x30E3, 9, 3039, },
  { 0x30E5, 9, 3048, },
  { 0x30E7, 7, 3057, },
  { 0x30EE, 7, 3064, },
  { 0xFF01, 2, 3071, },
  { 0xFF1F, 7, 3073, },
  { 0x30FC, 8, 3080, },
  { 0x3001, 3, 3088, },
  { 0x3002, 5, 3091, },
  { 0x3063, 8, 3096, },
  { 0x30C3, 9, 3104, },
};

u32 getFontHeight(void)   {
   return BG_VWF_IMB_H;
}

static int m_fontAnchorX;
static int m_fontAnchorY;

void resetFontAnchor()   {
   m_fontAnchorX=-1;
   m_fontAnchorY=-1;
}

void setFontAnchor( int x, int y )   {
   m_fontAnchorX=x;
   m_fontAnchorY=y;
}

static const BG_VWF_DAT *getFontData( u16 ch )   {
   u32 i;
   for (i=0; i<sizeof(m_bg_vwf_dat)/sizeof(BG_VWF_DAT); i++) {
    if (m_bg_vwf_dat.ch == ch) return &m_bg_vwf_dat;
  }
  return 0;
}

void drawRectFill( int left, int top, int right, int bottom )   {
   int x,y;
   for(y=top;y<bottom;y++ )   {
      for(x=left;x<right;x++)   {
         setFrameBufPixel( x, y );
      }
   }
   invalidateFrameBuf( left, top, right, bottom );
}

static u32 m_thickness = 1;

void setLineThickness( u32 thickness )   {
   m_thickness = thickness;
}

u32 getLineThickness()   {
   return m_thickness;
}

void drawRect( int left, int top, int right, int bottom )   {
   drawRectFill( left, top, left+m_thickness, bottom );
   drawRectFill( right-m_thickness, top, right, bottom );
   drawRectFill( left+m_thickness, top, right-m_thickness, top+m_thickness );
   drawRectFill( left+m_thickness, bottom-m_thickness, right-m_thickness, bottom );
}

// returns char width
static u32 drawFontChar(int x,int y,u16 ch)   {
   u32 ix,iy;
   u32 w;
   const BG_VWF_DAT *bg_vwf_dat;
   u8 px[2];
   if(ch==' ')   return BG_VWF_SPC_W;
   bg_vwf_dat=getFontData(ch);
   if(bg_vwf_dat==0)   return BG_VWF_SPC_W;
   u8 *imb=(u8*)bg_vwf_bin;
   imb+=bg_vwf_dat->x/2;
   w=bg_vwf_dat->w;
   if(bg_vwf_dat->x&1)   {
      w++;
      for(iy=0;iy<BG_VWF_IMB_H;iy++)   {
         for(ix=0;ix<w;ix+=2)   {
            px[0]=(imb[ix/2]>>4)&15;
            px[1]=imb[ix/2]&15;
            if(px[0]&&ix>0)         setFrameBufPixel(ix+x-1,iy+y);
            if(px[1]&&(ix+1<w))   setFrameBufPixel(ix+x,iy+y);
         }
         imb+=BG_VWF_IMB_W/2;
      }
      w--;
   }   else {
      for(iy=0;iy<BG_VWF_IMB_H;iy++)   {
         for(ix=0;ix<w;ix+=2)   {
            px[0]=(imb[ix/2]>>4)&15;
            px[1]=imb[ix/2]&15;
            if(px[0])                  setFrameBufPixel(ix+x,iy+y);
            if(px[1]&&(ix+1<w))   setFrameBufPixel(ix+x+1,iy+y);
         }
         imb+=BG_VWF_IMB_W/2;
      }
   }
   return w;
   
}

u32 getFontStringLength(const char *str)   {
   u32 len=0;
   const BG_VWF_DAT *bg_vwf_dat;
   while(*str)   {
      if(*str==' ')   {
         len+=BG_VWF_SPC_W;
      } else {
         bg_vwf_dat=getFontData(*str);
         if(bg_vwf_dat)   {
            len+=bg_vwf_dat->w;
            len+=BG_VWF_SPC_CH;
         } else {
            len+=BG_VWF_SPC_W;
         }
      }
      str++;
   }
   return len;
}

// returns string width
static u32 _drawFontString(int x,int y,const char *str)   {
   u32 w=0;
   u32 tot_w=0;
   int _x=x;
   
   if( m_fontAnchorX == 0 )
      x=x-getFontStringLength(str)/2;
   else if( m_fontAnchorX > 0 )
      x=x-getFontStringLength(str);
   
   if( m_fontAnchorY == 0 )
      y=y-getFontHeight()/2;
   else if( m_fontAnchorX > 0 )
      y=y-getFontHeight();
   
   
   while(*str)   {
      w=drawFontChar(x,y,*str);
      w+=BG_VWF_SPC_CH;
      tot_w+=w;
      x+=w;
      str++;
   }
   invalidateFrameBuf( _x, y, _x + tot_w, y + getFontHeight() );
   return tot_w;
}

u32 drawFontString(int x,int y,const char *str,...)   {
   char buf[2048];
   u32 w;
   va_list arg;
   va_start(arg,str);
   vsprintf(buf,str,arg);
   w=_drawFontString(x,y,buf);
   va_end(arg);
   return w;
}

u32 getFontStringLengthW(const u16 *str)   {
   u32 len=0;
   const BG_VWF_DAT *bg_vwf_dat;
   while(*str)   {
      if(*str==' ')   {
         len+=BG_VWF_SPC_W;
      } else {
         bg_vwf_dat=getFontData(*str);
         if(bg_vwf_dat)   {
            len+=bg_vwf_dat->w;
            len+=BG_VWF_SPC_CH;
         } else {
            len+=BG_VWF_SPC_W;
         }
      }
      str++;
   }
   return len;
}

// returns string width
u32 _drawFontStringW(int x,int y,const u16 *str)   {
   u32 w=0;
   u32 tot_w=0;
   int _x=x;
   
   if( m_fontAnchorX == 0 )
      x=x-getFontStringLengthW(str)/2;
   else if( m_fontAnchorX > 0 )
      x=x-getFontStringLengthW(str);
   
   if( m_fontAnchorY == 0 )
      y=y-getFontHeight()/2;
   else if( m_fontAnchorX > 0 )
      y=y-getFontHeight();
   
   
   while(*str)   {
      w=drawFontChar(x,y,*str);
      w+=BG_VWF_SPC_CH;
      tot_w+=w;
      x+=w;
      str++;
   }
   invalidateFrameBuf( _x, y, _x + tot_w, y + getFontHeight() );
   return tot_w;
}
