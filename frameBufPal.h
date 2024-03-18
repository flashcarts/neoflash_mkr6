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

#define FRAME_BUF_PAL_NUM_COLS   256

void resetFrameBufPal();
void setFrameBufPalCol( u8 id, u16 col );
void flushFrameBufPal();
