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

#ifndef __CARD_H
#define __CARD_H

void cardTransfer(u32,void*,u32,void*);


int card_init(void);
int card_size(void);
void card_read(u32 src, u8 *dst, int size);

#endif
