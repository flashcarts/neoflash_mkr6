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

#ifndef __FLASH_H
#define __FLASH_H

void flash_rd_2m(u32 addr,u8 *buf,u32 size);
void flash_wr_2m(u32 addr,u8 *buf,u32 size);
void flash_erase_2m(void);

#endif
