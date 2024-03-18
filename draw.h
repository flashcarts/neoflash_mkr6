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

// draw.h

#pragma once



void drawButton( int left, int top, int right, int bottom, const char *string );
void drawButtonA(const char *string );
void drawButtonB(const char *string );
//void drawMessage( const char *string );

u32 getFontHeight();
void resetFontAnchor();
void setFontAnchor( int x, int y );

void drawRect( int left, int top, int right, int bottom );
void drawRectFill( int left, int top, int right, int bottom );
void setLineThickness( u32 thickness );
u32 getLineThickness();
u32 _drawFontStringW( int x,int y,const u16 *str );
u32 drawFontString(int x,int y,const char *str,...);
void drawButton( int left, int top, int right, int bottom, const char *string );
void drawButtonA(const char *string );
void drawButtonB(const char *string );
void _drawMessage( const char *string );
void drawMessage(const char *str,...);
void drawErrorMessage( const char *string );
