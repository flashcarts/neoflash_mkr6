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



@ -----------------------------------------------
@ cardWriteCommand
@
@ Write nds card command for send
@ Input:
@  r0: Pointer to command buffer
@
@ -----------------------------------------------
cardWriteCommand:
   mov   r2, #0xC0
   ldr   r1, =0x040001A0
   strb   r2, [r1,#1]

   add   r1, r1, #0x0F
   mov   r2, #7

0:   ldrb   r3, [r0],#1
   strb   r3, [r1],#-1
   subs   r2, r2, #1
   bge   0b

   bx   lr



@ -----------------------------------------------
@ cardTransfer
@
@ Write nds card command for send
@ Input:
@  r0: Flags
@  r1: Destination
@  r2: Length
@  r3: Command Buffer
@
@ -----------------------------------------------
cardTransfer:
   stmfd   sp!, {r0-r2,r4-r6,lr}
   ldmfd   sp!, {r4-r6}

   mov   r0, r3
   bl   cardWriteCommand

   mov   r2, #0x04100000
   ldr   r3, =0x040001A0
   str   r4, [r3,#4]

0:   ldr   r0, [r3,#4]
   tst   r0, #0x00800000
   beq   1f
   ldr   r1, [r2,#0x10]
   cmp   r6, #0
   strgt   r1, [r5],#4
   subgt   r6, r6, #1
1:   tst   r0, #0x80000000
   bne   0b

   ldmfd   sp!, {r4-r6,lr}
   bx   lr

   .pool
