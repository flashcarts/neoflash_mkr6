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

#include "frameBuf.h"
#include "draw.h"


void drawButton( int left, int top, int right, int bottom, const char *string )   {
   int width = right - left;
   int height = bottom - top;
   setFrameBufCol( 1 );
   setLineThickness( 2 );
   clearFrameBufEx( left, top, right, bottom);
   drawRect( left, top, right, bottom);
   setFontAnchor(0,0);
   drawFontString( left+(width/2), top+(height/2), string );
   resetFontAnchor();
}

void drawButtonA(const char *string )   {
   drawButton( FRAME_BUF_WIDTH/4*3-32, FRAME_BUF_HEIGHT/4*3-16, FRAME_BUF_WIDTH/4*3+32, FRAME_BUF_HEIGHT/4*3+16, string );
}

void drawButtonB(const char *string )   {
   drawButton( FRAME_BUF_WIDTH/4-32, FRAME_BUF_HEIGHT/4*3-16, FRAME_BUF_WIDTH/4+32, FRAME_BUF_HEIGHT/4*3+16, string );
}

void _drawMessage( const char *string )   {
   drawButton( FRAME_BUF_WIDTH/2-96, FRAME_BUF_HEIGHT/4*3-16-48, FRAME_BUF_WIDTH/2+96, FRAME_BUF_HEIGHT/4*3+16-48, string );
}

void drawMessage(const char *str,...)   {
   char buf[2048];
   va_list arg;
   va_start(arg,str);
   vsprintf(buf,str,arg);
   _drawMessage(buf);
   va_end(arg);
}

void drawErrorMessage( const char *string )   {
   setFrameBufCol( 1 );
   setLineThickness( 2 );
   clearFrameBufEx( FRAME_BUF_WIDTH/2-48, FRAME_BUF_HEIGHT/4*3-48, FRAME_BUF_WIDTH/2+48, FRAME_BUF_HEIGHT/4*3+24 );

   drawButton( FRAME_BUF_WIDTH/2-32, FRAME_BUF_HEIGHT/4*3-16, FRAME_BUF_WIDTH/2+32, FRAME_BUF_HEIGHT/4*3+16, "A: OK" );
}
