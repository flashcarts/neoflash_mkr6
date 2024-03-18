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

#include "frameBuf.h"
#include "frameBufPal.h"

#include <stdio.h>

#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))

static u8 *m_frameBuf; //[ FRAME_BUF_WIDTH *  FRAME_BUF_HEIGHT ];

// use this static array instead of mallocing
static frameBufInvalid_t m_frameBufInvalid[ FRAME_BUF_INVALID_NUM ];

// these 2 arrays are actually used by the invalidation system
static frameBufInvalid_t *m_frameBufInvalidFree;
static frameBufInvalid_t *m_frameBufInvalidUsed;

static int m_clipLeft,m_clipRight,m_clipTop,m_clipBottom;

// this stores the colour we are drawing with
static u8 m_frameBufCol;

//static sImage m_wallpaper;

//include our ball pcx file (auto generated)
//#include "matrix_bin.h"

void resetFrameBufClip()   {
   m_clipLeft=0;
   m_clipRight=FRAME_BUF_WIDTH;
   m_clipTop=0;
   m_clipBottom=FRAME_BUF_HEIGHT;
}

void setFrameBufClip( int left, int top, int right, int bottom )   {
   if( left < 0 )
      left = 0;
   if( right > FRAME_BUF_WIDTH )
      right = FRAME_BUF_WIDTH;
   if( top < 0 )
      top = 0;
   if( bottom > FRAME_BUF_HEIGHT )
      bottom = FRAME_BUF_HEIGHT;
   m_clipLeft=left;
   m_clipRight=right;
   m_clipTop=top;
   m_clipBottom=bottom;
}

// col is a palette index, 0 to 255 (as u8 implies)
void setFrameBufCol( u8 col )   {
   m_frameBufCol = col;
}


void clearFrameBuf()   {
   u32 i;
   resetFrameBufInvalid();
   for(i=0;i<FRAME_BUF_WIDTH*FRAME_BUF_HEIGHT;i++)   {
      m_frameBuf=0; //m_wallpaper.data8+32;
   }
   invalidateFrameBuf( 0, 0, FRAME_BUF_WIDTH, FRAME_BUF_HEIGHT );
}

// this does not set an invalid region because it will be extremely
// inefficient to do so...it should be done by whoever draws the
// pixels manually

bool setFrameBufPixel( int x, int y )   {

   if( x < m_clipLeft || x >= m_clipRight )      return false;
   if( y < m_clipTop || y >= m_clipBottom )   return false;

   m_frameBuf[x+y*FRAME_BUF_WIDTH] = m_frameBufCol;

   return true;

}

void clearFrameBufEx( int left, int top, int right, int bottom )   {
   int x,y;
   for(y=top;y<bottom;y++ )   {
      for(x=left;x<right;x++)   {
         m_frameBuf[x + (y * FRAME_BUF_WIDTH)]= 0; //m_wallpaper.data8[ x + (y * FRAME_BUF_WIDTH) ]+32;
      }
   }
   invalidateFrameBuf( left, top, right, bottom );
}


void resetFrameBufInvalid()   {
   u32 i;
   for(i=0;i<FRAME_BUF_INVALID_NUM;i++)   {
      //m_frameBufInvalid.left = 0;
      //m_frameBufInvalid.right = 0;
      //m_frameBufInvalid.top = 0;
      //m_frameBufInvalid.bottom = 0;
      if( i < FRAME_BUF_INVALID_NUM - 1 )   m_frameBufInvalid.next = &m_frameBufInvalid[i+1];
      else                                                m_frameBufInvalid.next = 0;
   }
   m_frameBufInvalidFree = &m_frameBufInvalid[0];
   m_frameBufInvalidUsed = 0;
}

static bool testFrameBufInvalidIntersect( frameBufInvalid_t *a, frameBufInvalid_t *b )   {
   // if the left of b is greater than the right of a...they cannot overlap
   if( b->left > a->right )   return false;
   // repeat for other way...
   if( b->right < a->left )   return false;
   // and the next...
   if( b->top > a->bottom )   return false;
   // ...
   if( b->bottom < a->top )   return false;
   // ok they must overlap
   return true;
}

static void debugString( const char *string )   {
   //__asm( "swi 0xff" );
}

void debugFrameBufInvalid()   {
   char str[1024];
   frameBufInvalid_t *invalid;                           // used to traverse the list of used regions
   debugString( "foo" );
   for( invalid = m_frameBufInvalidUsed ; invalid ; invalid = invalid->next )   {
      sprintf( str, "%u %u %u %u %p", invalid->left, invalid->top, invalid->right, invalid->bottom, invalid->next );
      debugString( str );
   }
}

void invalidateFrameBuf( int left, int top, int right, int bottom )   {

   frameBufInvalid_t _thisInvalid;
   frameBufInvalid_t *thisInvalid = &_thisInvalid;   // pointer for convenience
   frameBufInvalid_t *invalid;                           // used to traverse the list of used regions
   //char str[265];

   //always align left and right to 2

   left = left >> 1;
   left = left << 1;

   right++;

   right = right >> 1;
   right = right << 1;

   //sprintf( str, "invalidating: %i %i %i %i", left, top, right, bottom );
//debugString( str );

   // if the left hand side of invalid region is off the right of the screen then do nothing
   if( left >= FRAME_BUF_WIDTH )   return;
   // same at the left hand side...
   if( right <= 0 )                     return;
   // same at the top...
   if( bottom <= 0 )                     return;
   // same at the bottom...
   if( top >= FRAME_BUF_HEIGHT )   return;

   // if we have reached here some part of the invalid region is still on screen so deal with it...
   // first up eliminate any off screen regions
   if( left < 0 )   left = 0;
   if( right > FRAME_BUF_WIDTH )   right = FRAME_BUF_WIDTH;
   if( top < 0 )      top = 0;
   if( bottom > FRAME_BUF_HEIGHT )   bottom = FRAME_BUF_HEIGHT;

   // now this rect is entirely on screen... so check it against the list of used invalid regions
   // if it overlaps at all then just adjust the current region in the list, otherwise
   // allocate a new region and put it at the start of the list. in the case of running out of
   // regions just free them all up and add a single region covering the entire screen


   // fill in this invalid region...

   thisInvalid->left = left;
   thisInvalid->right = right;
   thisInvalid->top = top;
   thisInvalid->bottom = bottom;

   // now traverse the list
   for( invalid = m_frameBufInvalidUsed ; invalid ; invalid = invalid->next )   {
      // great the regions overlap so fix up the original used region and we are done
      if( testFrameBufInvalidIntersect( invalid, thisInvalid ) )   {
         invalid->left = MIN( invalid->left, thisInvalid->left );
         invalid->right = MAX( invalid->right, thisInvalid->right );
         invalid->top = MIN( invalid->top, thisInvalid->top );
         invalid->bottom = MAX( invalid->bottom, thisInvalid->bottom );
         // done!
         return;
      }
   }

   // if we reached here this region does not overlap any existing regions so create an entirely new region

   // **** we are totally out of regions? this should not happen, if it does (as if!) lets not panic
   // simply free up all regions and specify a single region covering the entire screen...and in
   // the process making this entire system ineffective
   if( m_frameBufInvalidFree == 0 )   {
      // free em all up
      resetFrameBufInvalid();
      // grab the first free one
      invalid = m_frameBufInvalidFree;
      // sort our free list...although its totally pointless cos we are already screwed here
      m_frameBufInvalidFree = m_frameBufInvalidFree->next;
      // fill it in to cover the entire screen
      invalid->left = 0;
      invalid->right = FRAME_BUF_WIDTH;
      invalid->top = 0;
      invalid->bottom = FRAME_BUF_HEIGHT;
      invalid->next = 0;
      // phew...finally add it to the used list...
      m_frameBufInvalidUsed = invalid;
      // done!
      return;
   }

   // we reached here, so we have a non overlapping region...but we still have free regions left

   // grab the first free one
   invalid = m_frameBufInvalidFree;
   // sort our free list
   m_frameBufInvalidFree = m_frameBufInvalidFree->next;

   // fill it
   invalid->left = thisInvalid->left;
   invalid->right = thisInvalid->right;
   invalid->top = thisInvalid->top;
   invalid->bottom = thisInvalid->bottom;
   invalid->next = m_frameBufInvalidUsed;

   // finally add it to the used list....
   m_frameBufInvalidUsed = invalid;
   // done!
   return;


}

void resetFrameBuf()   {

   static bool firstTime = true;

   if( firstTime )   {
      m_frameBuf = (u8*)allocMem( FRAME_BUF_WIDTH * FRAME_BUF_HEIGHT );
      firstTime = false;
   }

   u32 i;
   BG2_CR = BG_BMP8_256x256 | BG_BMP_BASE(0);
   SUB_BG2_CR = BG_BMP8_256x256 | BG_BMP_BASE(0);
   BG2_XDX=0x0100;
   BG2_XDY=0;
   BG2_YDX=0;
   BG2_YDY=0x0100;
   BG2_CX=0;
   BG2_CY=0;
   SUB_BG2_XDX=0x0100;
   SUB_BG2_XDY=0;
   SUB_BG2_YDX=0;
   SUB_BG2_YDY=0x0100;
   SUB_BG2_CX=0;
   SUB_BG2_CY=0;
   videoSetMode(MODE_5_2D | DISPLAY_BG2_ACTIVE | DISPLAY_SPR_1D | DISPLAY_SPR_ACTIVE );
   videoSetModeSub(MODE_5_2D | DISPLAY_BG2_ACTIVE | DISPLAY_SPR_1D | DISPLAY_SPR_ACTIVE );
   resetFrameBufClip();
   setFrameBufCol( 0 );
   //loadPCX( (u8*)matrix_bin, &m_wallpaper );
   //if(m_wallpaper.bpp!=8)   {
   //   debugString( "Invalid BPP\n" );
   //   while(1);
   //}
   /*for(i=32;i<256;i++)   {
      setFrameBufPalCol( i, m_wallpaper.palette[i-32] );
   }*/
   flushFrameBufPal();
   clearFrameBuf();
   flushFrameBuf();

}

// buf points to either the top or bottom screen
// takes a destination frame buffer x and y, left etc is a region from our
// system memory buffer
// does no sanity checks on the values because they are all done at a higher level
// - this is a helper function for flushFrameBufInvalid()
static void _blitFrameBuf( bool sub, u32 left, u32 top, u32 right, u32 bottom )   {

   u32 width;
   u32 height;
   u16 *dst,*src;

   u32 x,y;



   if( sub )   {
      dst = (u16*)BG_BMP_RAM_SUB(0);
      src = (u16*)&m_frameBuf[FRAME_BUF_HEIGHT/2*FRAME_BUF_WIDTH];
   } else {
      dst = (u16*)BG_BMP_RAM(0);
      src = (u16*)m_frameBuf;
      //src = (u16*)&m_frameBuf[ FRAME_BUF_HEIGHT/2 * FRAME_BUF_WIDTH ];
   }

   width = right - left;
   height = bottom - top;

   // left and right will always be aligned to the nearest 2 bytes

   src = src + (left/2);
   src = src + (top*FRAME_BUF_WIDTH/2);

   dst = dst + (left/2);
   dst = dst + (top*FRAME_BUF_WIDTH/2);

   for(y=0;y<height;y++)   {
      for(x=0;x<width/2;x++)   {
         dst[x+(y*FRAME_BUF_WIDTH/2)] = src[x+(y*FRAME_BUF_WIDTH/2)];
      }
   }


}

static void flushFrameBufInvalid( frameBufInvalid_t *invalid )   {

   if( invalid->top < FRAME_BUF_HEIGHT / 2 )   {
      // the top part is on the top screen
      if( invalid->bottom <= FRAME_BUF_HEIGHT / 2 )   {
         // the entire region is on the top screen so draw it all
         _blitFrameBuf( false, invalid->left, invalid->top, invalid->right, invalid->bottom );
         // done
         return;
      }
      // ****s sake it crosses screens, draw the top screen and then
      // fix up and fall through for drawing the bottom part
      _blitFrameBuf( false, invalid->left, invalid->top, invalid->right, FRAME_BUF_HEIGHT/2 );
      // fix up
      invalid->top = FRAME_BUF_HEIGHT/2;
      // fall through
   }

   // the region was either entirely on the bottom screen, or we have drawn
   // the top screen and fixed up the rect to draw the bottom part

   // regardless of how we got here fix up so we are offset into video memory properly

   invalid->top -= FRAME_BUF_HEIGHT/2;
   invalid->bottom -= FRAME_BUF_HEIGHT/2;
   _blitFrameBuf( true, invalid->left, invalid->top, invalid->right, invalid->bottom );
   // yay done (thank **** )

}

static bool m_flush;

void flushFrameBuf()   {
   m_flush=1;
}

void _flushFrameBuf()   {

   frameBufInvalid_t *invalid;                           // used to traverse the list of used regions

   //debugFrameBufInvalid();

   if( m_flush == 0 )   return;
   m_flush = 0;

   // this is going to be fun...basically we traverse the used list drawing directly to NDS video memory
   // the fun part will be splitting regions covering 2 screens, since the entire NDS frame buffer is
   // across 2 NDS screens and is not linear in physical video memory :'(
   // oh yeah as we deal with regions add them back to the head of the free list for next frame

   // now traverse the list
   for( invalid = m_frameBufInvalidUsed ; invalid ; invalid = invalid->next )   {
      // flush this region
      flushFrameBufInvalid( invalid );
      // done so free it up
      //invalid->next = m_frameBufInvalidFree;
      //m_frameBufInvalidFree = invalid;
   }

   resetFrameBufInvalid();

}

