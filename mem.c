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

void *allocMem( u32 size )   {
   return malloc( size );
}

void freeMem( void *ptr )   {
   if( ptr )   free( ptr );
}

void mem_cpy8(u8 *dst,u8 *src,u32 size)   {
   
   while(size)   {
   
      *dst=*src;
      dst++; src++;
      size-=1;
   
   }

}

void mem_set8(u8 *dst,u8 src,u32 size)   {
   
   while(size)   {
   
      *dst=src;
      dst++;
      size-=1;
   
   }

}

void mem_cpy16(u16 *dst,u16 *src,u32 size)   {
   
   while(size)   {
   
      *dst=*src;
      dst++; src++;
      size-=2;
   
   }

}

void mem_set16(u16 *dst,u16 src,u32 size)   {
   
   while(size)   {
   
      *dst=src;
      dst++;
      size-=2;
   
   }

}

void mem_cpy32(u32 *dst,u32 *src,u32 size)   {
   
   while(size)   {
   
      *dst=*src;
      dst++; src++;
      size-=4;
   
   }

}

void mem_set32(u32 *dst,u32 src,u32 size)   {
   
   while(size)   {
   
      *dst=src;
      dst++;
      size-=4;
   
   }

}

void mem_cpy_ex(void *dst,const void *src,u32 size)   {

   u32 d=(u32)dst,s=(u32)src;
   u32 align;

   if( ((d|s|size)&3) == 0 )      align=4;
   else if( ((d|s|size)&1) == 0 )   align=2;
   else            align=1;

   if(align==1)      mem_cpy8(dst,(u8*)src,size);
   else if(align==2)   mem_cpy16(dst,(u16*)src,size);
   else         mem_cpy32(dst,(u32*)src,size);

}
