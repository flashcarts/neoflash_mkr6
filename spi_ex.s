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

   .global spi_wait

spi_wait:
   
   ldr      r0, =0x040001A0
0:
   ldrh   r1, [ r0 ]
   tst      r1, #0x80
   bne      0b
   
   bx      lr
   
