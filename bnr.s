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

   .global   bnr_game_str
   .global bnr_maker_str
   .global   bnr_game_str_ex
   .global bnr_maker_str_ex

@ bnr_game_string
@ fixme: add language support

@ description:
@ gets the game title string from a banner in r1 and stores it in string r0

@ args:
@ r1=banner
@ r0=dst string
@ return:
@ nothing

bnr_game_str:
   add    r1,r1,#0x240
bnr_game_str_loop:
   ldrb   r2,[r1],#2
   cmp      r2,#0x0A
   cmpne   r2,#0
   strneb   r2,[r0],#1
   bne   bnr_game_str_loop
   mov      r2,#0
   strb   r2,[r0]
   bx      lr
   
@ the same as bnr_game_str but for 16bit chars

bnr_game_str_ex:
   add    r1,r1,#0x240
bnr_game_str_ex_loop:
   ldrh   r2,[r1],#2
   cmp      r2,#0x0A
   cmpne   r2,#0
   strneh   r2,[r0],#2
   bne   bnr_game_str_ex_loop
   mov      r2,#0
   strh   r2,[r0]
   bx      lr
   

@ bnr_maker_string
@ fixme: add language support

@ description:
@ gets the game maker string from a banner in r1 and stores it in string r0

@ args:
@ r1=banner
@ r0=dst string
@ return:
@ nothing

bnr_maker_str:
   add    r1,r1,#0x240
bnr_maker_str_loop1:
   ldrb   r2,[r1],#2
   cmp      r2,#0x0A
   cmpne   r2,#0
   bne      bnr_maker_str_loop1
bnr_maker_str_loop2:
   ldrb   r2,[r1],#2
   cmp      r2,#0x00
   strneb   r2,[r0],#1
   bne   bnr_maker_str_loop2
   mov      r2,#0
   strb   r2,[r0]
   bx      lr
   
bnr_maker_str_ex:
   add    r1,r1,#0x240
bnr_maker_str_ex_loop1:
   ldrh   r2,[r1],#2
   cmp      r2,#0x0A
   cmpne   r2,#0
   bne      bnr_maker_str_ex_loop1
bnr_maker_str_ex_loop2:
   ldrh   r2,[r1],#2
   cmp      r2,#0x00
   strneh   r2,[r0],#2
   bne   bnr_maker_str_ex_loop2
   mov      r2,#0
   strh   r2,[r0]
   bx      lr
