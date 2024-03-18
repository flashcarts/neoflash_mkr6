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

#ifndef __MEM_H
#define __MEM_H

#include <nds.h>

void mem_cpy_ex(void *dst,const void *src,u32 size);

void mem_cpy8(u8 *dst,const u8 *src,u32 size);
void mem_set8(u8 *dst,u8 src,u32 size);
void mem_cpy16(u16 *dst,const u16 *src,u32 size);
void mem_set16(u16 *dst,u16 src,u32 size);
void mem_cpy32(u32 *dst,const u32 *src,u32 size);
void mem_set32(u32 *dst,u32 src,u32 size);

#endif
