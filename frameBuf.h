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

#pragma once

#include <nds.h>
#include <nds/arm9/video.h>

#define FRAME_BUF_WIDTH      (SCREEN_WIDTH)
#define FRAME_BUF_HEIGHT   (SCREEN_HEIGHT*2)

#define FRAME_BUF_INVALID_NUM   256

typedef struct _frameBufInvalid_t {
   u32 top,bottom,left,right;
   struct _frameBufInvalid_t *next;
} frameBufInvalid_t;

void resetFrameBufInvalid();
void invalidateFrameBuf( int left, int top, int right, int bottom );
void resetFrameBuf();
void flushFrameBuf();
void _flushFrameBuf();

void setFrameBufCol( u8 col );
void clearFrameBuf();

void resetFrameBufClip();
void setFrameBufClip( int left, int top, int right, int bottom );

void clearFrameBufEx( int left, int top, int right, int bottom );

bool setFrameBufPixel( int x, int y );
