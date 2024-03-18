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

@
@ suggested optimisations
@
@ -consider removing inlined read from response function - speed is less
@ critical in this case and will help remove bloat use new neo2_spi_rd instead

   .global   neo2_spi_cs
neo2_spi_cs:
   ldr   r1, =(0x08000000 | (1<<24))
   orr   r0, r1, r0,LSL #12
   ldrh   r0, [r0]
   bx   lr



   .global neo2_spi_dummy
neo2_spi_dummy:
   mov   r0, #0xFF
@   b   neo2_spi_wr



   .global neo2_spi_wr
neo2_spi_wr:
   ldr   r1, =(0x08000000 | (1<<24) | (1<<19) | (1<<5))
   mov   r3, r0,LSL #(13 - 7)
   and   r3, r3, #0x2000
   orr   r3, r1, r3
   ldrh   r3, [r3]
   mov   r3, r0,LSL #(13 - 6)
   and   r3, r3, #0x2000
   orr   r3, r1, r3
   ldrh   r3, [r3]
   mov   r3, r0,LSL #(13 - 5)
   and   r3, r3, #0x2000
   orr   r3, r1, r3
   ldrh   r3, [r3]
   mov   r3, r0,LSL #(13 - 4)
   and   r3, r3, #0x2000
   orr   r3, r1, r3
   ldrh   r3, [r3]
   mov   r3, r0,LSL #(13 - 3)
   and   r3, r3, #0x2000
   orr   r3, r1, r3
   ldrh   r3, [r3]
   mov   r3, r0,LSL #(13 - 2)
   and   r3, r3, #0x2000
   orr   r3, r1, r3
   ldrh   r3, [r3]
   mov   r3, r0,LSL #(13 - 1)
   and   r3, r3, #0x2000
   orr   r3, r1, r3
   ldrh   r3, [r3]
   mov   r3, r0,LSL #(13 - 0)
   and   r3, r3, #0x2000
   orr   r3, r1, r3
   ldrh   r3, [r3]
   bx   lr



   .global neo2_spi_rd
neo2_spi_rd:
   ldr   r2,=(0x08000000 | (1<<24) | (1<<19) | (1<<13) | (1<<5))
   ldrh   r1, [r2]
   and   r1, r1, #0x0100
   mov   r0, r1,LSR #1
   ldrh   r1, [r2]
   and   r1, r1, #0x0100
   orr   r0, r0, r1,LSR #2
   ldrh   r1, [r2]
   and   r1, r1, #0x0100
   orr   r0, r0, r1,LSR #3
   ldrh   r1, [r2]
   and   r1, r1, #0x0100
   orr   r0, r0, r1,LSR #4
   ldrh   r1, [r2]
   and   r1, r1, #0x0100
   orr   r0, r0, r1,LSR #5
   ldrh   r1, [r2]
   and   r1, r1, #0x0100
   orr   r0, r0, r1,LSR #6
   ldrh   r1, [r2]
   and   r1, r1, #0x0100
   orr   r0, r0, r1,LSR #7
   ldrh   r1, [r2]
   and   r1, r1, #0x0100
   orr   r0, r0, r1,LSR #8
   bx   lr



   .global neo2_spi_mmc_cmd
neo2_spi_mmc_cmd:
   stmfd   sp!, {r4,lr}
   mov   r4, r1
   orr   r0, r0, #0x40
   bl   neo2_spi_wr
   mov   r0, r4,LSR #24
   bl   neo2_spi_wr
   mov   r0, r4,LSR #16
   bl   neo2_spi_wr
   mov   r0, r4,LSR #8
   bl   neo2_spi_wr
   mov   r0, r4
   bl   neo2_spi_wr
   ldmfd   sp!, {r4,lr}
   mov   r0, #0x95
   b   neo2_spi_wr



      .global check_neo2_spi_mmc_cmd_resp
check_neo2_spi_mmc_cmd_resp:
   stmfd   sp!, {r4-r5,lr}
   mov   r3, #4096
   ldr   r2,=(0x08000000 | (1<<24) | (1<<19) | (1<<13) | (1<<5))
0:   ldrh   r5, [r2]
   and   r5, r5, #0x0100
   mov   r4, r5,LSR #1
   ldrh   r5, [r2]
   and   r5, r5, #0x0100
   orr   r4, r4, r5,LSR #2
   ldrh   r5, [r2]
   and   r5, r5, #0x0100
   orr   r4, r4, r5,LSR #3
   ldrh   r5, [r2]
   and   r5, r5, #0x0100
   orr   r4, r4, r5,LSR #4
   ldrh   r5, [r2]
   and   r5, r5, #0x0100
   orr   r4, r4, r5,LSR #5
   ldrh   r5, [r2]
   and   r5, r5, #0x0100
   orr   r4, r4, r5,LSR #6
   ldrh   r5, [r2]
   and   r5, r5, #0x0100
   orr   r4, r4, r5,LSR #7
   ldrh   r5, [r2]
   and   r5, r5, #0x0100
   orr   r4, r4, r5,LSR #8
   and   r4, r4, r1
   cmp   r0, r4
   moveq   r0, #1
   beq   0f
   subs   r3, r3, #1
   bgt   0b
   mov   r0, #0
0:   ldmfd   sp!, {r4-r5,lr}
   bx   lr



/*.text:0200B100                 EXPORT init_neo2_spi_mmc
.text:0200B100 init_neo2_spi_mmc                       ; CODE XREF: NMMC2_StartUp+Cp
.text:0200B100                 MOV     R12, SP         ; Alternative name is '$a'
.text:0200B104                 STMFD   SP!, {R11,R12,LR,PC}
.text:0200B108                 SUB     R11, R12, #4
.text:0200B10C                 SUB     SP, SP, #0x10
.text:0200B110                 MOV     R0, #0x80
.text:0200B114                 BL      set_neo2_asic_cr
.text:0200B118                 MOV     R3, #0
.text:0200B11C                 STR     R3, [R11,#-0x10]
.text:0200B120                 B       loc_200B134
.text:0200B124 ; ---------------------------------------------------------------------------
.text:0200B124
.text:0200B124 loc_200B124                             ; CODE XREF: init_neo2_spi_mmc+3Cj
.text:0200B124                 BL      neo2_spi_dummy
.text:0200B128                 LDR     R3, [R11,#-0x10]
.text:0200B12C                 ADD     R3, R3, #1
.text:0200B130                 STR     R3, [R11,#-0x10]
.text:0200B134
.text:0200B134 loc_200B134                             ; CODE XREF: init_neo2_spi_mmc+20j
.text:0200B134                 LDR     R3, [R11,#-0x10]
.text:0200B138                 CMP     R3, #7
.text:0200B13C                 BLS     loc_200B124
.text:0200B140                 MOV     R0, #0
.text:0200B144                 MOV     R1, #0
.text:0200B148                 BL      neo2_spi_mmc_cmd
.text:0200B14C                 MOV     R0, #1
.text:0200B150                 BL      neo2_spi_cs
.text:0200B154                 MOV     R3, #0
.text:0200B158                 STR     R3, [R11,#-0x10]
.text:0200B15C                 B       loc_200B1A4
.text:0200B160 ; ---------------------------------------------------------------------------
.text:0200B160
.text:0200B160 loc_200B160                             ; CODE XREF: init_neo2_spi_mmc+ACj
.text:0200B160                 MOV     R0, #1
.text:0200B164                 MOV     R1, #0
.text:0200B168                 BL      neo2_spi_mmc_cmd
.text:0200B16C                 MOV     R0, #0
.text:0200B170                 MOV     R1, #1
.text:0200B174                 BL      check_neo2_spi_mmc_cmd_resp
.text:0200B178                 MOV     R3, R0
.text:0200B17C                 CMP     R3, #1
.text:0200B180                 BNE     loc_200B198
.text:0200B184                 MOV     R0, #1
.text:0200B188                 BL      neo2_spi_cs
.text:0200B18C                 MOV     R3, #1
.text:0200B190                 STR     R3, [R11,#-0x18]
.text:0200B194                 B       loc_200B1B8
.text:0200B198 ; ---------------------------------------------------------------------------
.text:0200B198
.text:0200B198 loc_200B198                             ; CODE XREF: init_neo2_spi_mmc+80j
.text:0200B198                 LDR     R3, [R11,#-0x10]
.text:0200B19C                 ADD     R3, R3, #1
.text:0200B1A0                 STR     R3, [R11,#-0x10]
.text:0200B1A4
.text:0200B1A4 loc_200B1A4                             ; CODE XREF: init_neo2_spi_mmc+5Cj
.text:0200B1A4                 LDR     R3, [R11,#-0x10]
.text:0200B1A8                 CMP     R3, #7
.text:0200B1AC                 BLS     loc_200B160
.text:0200B1B0                 MOV     R3, #0
.text:0200B1B4                 STR     R3, [R11,#-0x18]
.text:0200B1B8
.text:0200B1B8 loc_200B1B8                             ; CODE XREF: init_neo2_spi_mmc+94j
.text:0200B1B8                 LDR     R3, [R11,#-0x18]
.text:0200B1BC                 MOV     R0, R3
.text:0200B1C0                 SUB     SP, R11, #0xC
.text:0200B1C4                 LDMFD   SP, {R11,SP,LR}
.text:0200B1C8                 BX      LR
.text:0200B1C8 ; End of function init_neo2_spi_mmc*/



   .global read_neo2_spi_mmc
read_neo2_spi_mmc:
   stmfd   sp!, {r4,lr}
   mov   r4, r0

   mov   r0, #17
   bl   neo2_spi_mmc_cmd

   mov   r0, #0x00
   mov   r1, #0xFF
   bl   check_neo2_spi_mmc_cmd_resp
   cmp   r0, #0
   beq   1f

   mov   r0, #0xFE
   mov   r1, #0xFF
   bl   check_neo2_spi_mmc_cmd_resp
   cmp   r0, #0
   beq   1f

   mov   r3, #512
   ldr   r2,=(0x08000000 | (1<<24) | (1<<19) | (1<<13) | (1<<5))
0:   ldrh   r1, [r2]
   and   r1, r1, #0x0100
   mov   r0, r1,LSR #1
   ldrh   r1, [r2]
   and   r1, r1, #0x0100
   orr   r0, r0, r1,LSR #2
   ldrh   r1, [r2]
   and   r1, r1, #0x0100
   orr   r0, r0, r1,LSR #3
   ldrh   r1, [r2]
   and   r1, r1, #0x0100
   orr   r0, r0, r1,LSR #4
   ldrh   r1, [r2]
   and   r1, r1, #0x0100
   orr   r0, r0, r1,LSR #5
   ldrh   r1, [r2]
   and   r1, r1, #0x0100
   orr   r0, r0, r1,LSR #6
   ldrh   r1, [r2]
   and   r1, r1, #0x0100
   orr   r0, r0, r1,LSR #7
   ldrh   r1, [r2]
   and   r1, r1, #0x0100
   orr   r0, r0, r1,LSR #8
   strb   r0, [r4],#1
   subs   r3, r3, #1
   bgt   0b

   bl   neo2_spi_dummy
   bl   neo2_spi_dummy

@   mov   r0, #1
@   bl   neo2_spi_cs
   
   mov   r0, #1

1:   ldmfd   sp!, {r4,lr}
   bx   lr

   .pool



/*.text:0200B2AC ; ªªªªªªªªªªªªªªª S U B R O U T I N E ªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªªª
.text:0200B2AC
.text:0200B2AC
.text:0200B2AC                 EXPORT write_neo2_spi_mmc
.text:0200B2AC write_neo2_spi_mmc                      ; CODE XREF: NMMC2_WriteSectors+60p
.text:0200B2AC                 MOV     R12, SP         ; Alternative name is '$a'
.text:0200B2B0                 STMFD   SP!, {R11,R12,LR,PC}
.text:0200B2B4                 SUB     R11, R12, #4
.text:0200B2B8                 SUB     SP, SP, #0x20
.text:0200B2BC                 STR     R0, [R11,#-0x18]
.text:0200B2C0                 STR     R1, [R11,#-0x1C]
.text:0200B2C4                 LDR     R2, [R11,#-0x18]
.text:0200B2C8                 STR     R2, [R11,#-0x20]
.text:0200B2CC                 MOV     R0, #0x18
.text:0200B2D0                 LDR     R1, [R11,#-0x1C]
.text:0200B2D4                 BL      neo2_spi_mmc_cmd
.text:0200B2D8                 MOV     R0, #0
.text:0200B2DC                 MOV     R1, #0xFF
.text:0200B2E0                 BL      check_neo2_spi_mmc_cmd_resp
.text:0200B2E4                 MOV     R3, R0
.text:0200B2E8                 CMP     R3, #0
.text:0200B2EC                 BNE     loc_200B2FC
.text:0200B2F0                 MOV     R3, #0
.text:0200B2F4                 STR     R3, [R11,#-0x28]
.text:0200B2F8                 B       loc_200B39C
.text:0200B2FC ; ---------------------------------------------------------------------------
.text:0200B2FC
.text:0200B2FC loc_200B2FC                             ; CODE XREF: write_neo2_spi_mmc+40j
.text:0200B2FC                 MOV     R0, #0xFE
.text:0200B300                 BL      neo2_spi_wr
.text:0200B304                 MOV     R2, #0
.text:0200B308                 STR     R2, [R11,#-0x24]
.text:0200B30C                 B       loc_200B334
.text:0200B310 ; ---------------------------------------------------------------------------
.text:0200B310
.text:0200B310 loc_200B310                             ; CODE XREF: write_neo2_spi_mmc+94j
.text:0200B310                 LDR     R3, [R11,#-0x24]
.text:0200B314                 LDR     R2, [R11,#-0x20]
.text:0200B318                 ADD     R3, R2, R3
.text:0200B31C                 LDRB    R3, [R3]
.text:0200B320                 MOV     R0, R3
.text:0200B324                 BL      neo2_spi_wr
.text:0200B328                 LDR     R3, [R11,#-0x24]
.text:0200B32C                 ADD     R3, R3, #1
.text:0200B330                 STR     R3, [R11,#-0x24]
.text:0200B334
.text:0200B334 loc_200B334                             ; CODE XREF: write_neo2_spi_mmc+60j
.text:0200B334                 LDR     R3, =0x1FF
.text:0200B338                 LDR     R2, [R11,#-0x24]
.text:0200B33C                 CMP     R2, R3
.text:0200B340                 BLS     loc_200B310
.text:0200B344                 BL      neo2_spi_dummy
.text:0200B348                 BL      neo2_spi_dummy
.text:0200B34C                 SUB     R3, R11, #0xD
.text:0200B350                 MOV     R0, R3
.text:0200B354                 BL      neo2_spi_rd
.text:0200B358                 LDRB    R3, [R11,#-0xD]
.text:0200B35C                 AND     R3, R3, #0xF
.text:0200B360                 CMP     R3, #5
.text:0200B364                 BEQ     loc_200B374
.text:0200B368                 MOV     R3, #0
.text:0200B36C                 STR     R3, [R11,#-0x28]
.text:0200B370                 B       loc_200B39C
.text:0200B374 ; ---------------------------------------------------------------------------
.text:0200B374
.text:0200B374 loc_200B374                             ; CODE XREF: write_neo2_spi_mmc+B8j
.text:0200B374                                         ; write_neo2_spi_mmc+DCj
.text:0200B374                 SUB     R3, R11, #0xD
.text:0200B378                 MOV     R0, R3
.text:0200B37C                 BL      neo2_spi_rd
.text:0200B380                 LDRB    R3, [R11,#-0xD]
.text:0200B384                 CMP     R3, #0
.text:0200B388                 BEQ     loc_200B374
.text:0200B38C                 MOV     R0, #1
.text:0200B390                 BL      neo2_spi_cs
.text:0200B394                 MOV     R2, #1
.text:0200B398                 STR     R2, [R11,#-0x28]
.text:0200B39C
.text:0200B39C loc_200B39C                             ; CODE XREF: write_neo2_spi_mmc+4Cj
.text:0200B39C                                         ; write_neo2_spi_mmc+C4j
.text:0200B39C                 LDR     R3, [R11,#-0x28]
.text:0200B3A0                 MOV     R0, R3
.text:0200B3A4                 SUB     SP, R11, #0xC
.text:0200B3A8                 LDMFD   SP, {R11,SP,LR}
.text:0200B3AC                 BX      LR
.text:0200B3AC ; End of function write_neo2_spi_mmc*/
