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

#include <nds.h>

#include "Video.h"


u16 *OBJPtr( u32 type, u32 index )   {
   u32 ptr;

   switch( type )   {
      case OBJ_TYPE_A:   ptr=0x07000000;   break;
      case OBJ_TYPE_B:   ptr=0x07000400;   break;
      default:         /*type error*/   return OBJ_ERROR;
   }
   if( index > 255 )      /*index error*/ return OBJ_ERROR;
   
   ptr+=index*8;
   return (u16*)(ptr);
}

void OBJClear( u16 *obj )   {
   obj[0]=obj[1]=obj[2]=0;
}

void OBJDisable( u16 *obj )   {
   obj[0]=(2<<8);
}

void OBJPos( u16 *obj, int x, int y )   {
   
   u16 i;

   if( obj == 0 )   return;

   x=x&511;
   y=y&255;

   i=obj[0];
   i=i&~255;
   i=i|y;
   obj[0]=i;

   i=obj[1];
   i=i&~511;
   i=i|x;
   obj[1]=i;

}

void OBJSize( u16 *obj, u32 size )   {
   
   u32 *ptr=(u32*)obj;
   u32 i;

   if( obj == 0 )   return;

   i=ptr[0];
   i=i&~( ( 3<<14 ) | (3<<30) );
   i=i|size;
   ptr[0]=i;

}

void OBJPal( u16 *obj, u32 index )   {
   
   u16 i;

   if( obj == 0 )   return;

   index=index&15;

   i=obj[2];
   i=i&~(15<<12);
   i=i|(index<<12);
   obj[2]=i;

}
void OBJTile( u16 *obj, u32 index )   {
   
   u16 i;

   if( obj == 0 )   return;

   index=index&255;

   i=obj[2];
   i=i&~255;
   i=i|index;
   obj[2]=i;

}


u16 *PalPtr( u32 type, u32 index )   {

   u32 ptr;
   
   switch( type )   {
      case PAL_TYPE_BG_A:      ptr=0x05000000;   break;
      case PAL_TYPE_BG_B:      ptr=0x05000400;   break;
      case PAL_TYPE_OBJ_A:   ptr=0x05000200;   break;   
      case PAL_TYPE_OBJ_B:   ptr=0x05000600;   break;
      default:            /*type error*/   return PAL_ERROR;
   }
   if( index > 15 )            /*index error*/   return PAL_ERROR;
   
   ptr+=index*0x20;
   return (u16*)(ptr);
   
}

// copies 32 bytes from src to dst, useful for loading 16 colour nds palettes

u32 PalCopy( u16 *dst, const u16 *src )   {

   if( dst==0 || src==0 )   return PAL_ERROR;

   dmaCopy(src, dst, 32);

   return PAL_SUCCESS;
   
}

// try using this and remove equivelent from arm9_main

u32 PalCopyGrayScale( u16 *dst, const u16 *src )   {

   u32 i;
   u32 r,g,b;
   u32 a;
   u32 o;

   if( dst==0 || src==0 )   return PAL_ERROR;

   for(i=0;i<16;i++)   {

      r= (src)&31;
      g= (src >> 5) & 31;
      b= (src >> 10) & 31;

      a=r+g+b;
      a=a/3;
      a=a&31;

      o=a | (a<<5) | (a<<10) ;

      dst=o;

   }

   return PAL_SUCCESS;

}

u16 *TilePtr( u32 type, u32 index )   {
   
   u32 ptr;

   switch( type )   {
      case TILE_TYPE_BG_A:   ptr=0x6000000;   break;
      case TILE_TYPE_BG_B:   ptr=0x6200000;   break;
      case TILE_TYPE_OBJ_A:   ptr=0x6400000;   break;   
      case TILE_TYPE_OBJ_B:   ptr=0x6600000;   break;
      default:            /*type error*/   return TILE_ERROR;
   }
   if( index > 65535 )         /*index error*/   return TILE_ERROR;
   
   ptr+=index*0x20;
   return (u16*)(ptr);

}

u32 TileCopy( u16 *dst, const u16 *src, u32 count )   {

   if( dst==0 || src==0 || count==0 )   return TILE_ERROR;

   dmaCopy(src, dst, 32*count);

   return TILE_SUCCESS;
   
}
