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

   .global spi_open
   .global spi_close
   .global spi
   .global spi_exch

@spi_previous:
@   B   spi_previous
   
NEO_OpenSPI:
spi_open:
   ldr      r1, =0x0000A040
   orr      r1, r1, r0
   ldr      r0, =0x040001A0
   strh   r1, [ r0 ]
   bx      lr

NEO_CloseSPI:
spi_close:
   ldr      r0, =0x040001A0
   mov      r1,#0x0000
   strh   r1, [ r0 ]
   bx      lr

NEO_SPI:
spi_exch:
spi:
   ldr      r1, =0x040001A0
   strb   r0, [ r1, #2 ]
0:
   ldrb   r0, [ r1 ]
   tst      r0, #0x80
   bne      0b
   ldrb   r0, [ r1, #2 ]
   bx      lr
   
   .pool
