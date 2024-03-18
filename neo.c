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
#include <nds/arm9/video.h>
#include "mem.h"
#include "draw.h"
#include "frameBuf.h"
#include "frameBufPal.h"
#include "gba_nds_fat.h"
#include "process.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static u16 m_wait_cr=0;

static bool m_cardPulled=0;

void neoVblankIrqHandler()   {
  if(REG_IF & IRQ_VBLANK) {
     _flushFrameBuf();
      VBLANK_INTR_WAIT_FLAGS |= IRQ_VBLANK;
  }
  REG_IF = IRQ_VBLANK;
}

void neoCardLineIrqHandler()   {
   if(REG_IF & IRQ_CARD_LINE) {
     m_cardPulled=true;
  }
  REG_IF = IRQ_CARD_LINE;
}

void neoStart()   {
   m_cardPulled=false;
   REG_IME = 0;
   REG_IF = REG_IF;
   REG_IE = 0;
   IRQ_HANDLER = 0;
   m_wait_cr=WAIT_CR;
   WAIT_CR &= ~(0x8080);
   vramSetBankA(VRAM_A_MAIN_BG_0x6000000);   // 128k
   vramSetBankB(VRAM_B_MAIN_SPRITE);      // 128k
   vramSetBankC(VRAM_C_SUB_BG_0x6200000);   // 128k
   vramSetBankD(VRAM_D_SUB_SPRITE);      // 128k
   videoSetMode(0);
   videoSetModeSub(0);
   BG0_CR = 0;
   SUB_BG0_CR = 0;
   BG1_CR = 0;
   SUB_BG1_CR = 0;
   BG2_CR = 0;
   SUB_BG2_CR = 0;
   BG3_CR = 0;
   SUB_BG3_CR = 0;
   mem_set32( (u32*) 0x05000000, 0, 0x400 );
   mem_set32( (u32*) 0x6000000, 0, 128*1024 );
   mem_set32( (u32*) 0x6200000, 0, 128*1024 );
   mem_set32( (u32*) 0x07000000, 0, 0x800 );
   mem_set32( (u32*) SPRITE_GFX, 0, 128*1024 );
   mem_set32( (u32*) SPRITE_GFX_SUB, 0, 128*1024 );
   // init system palette
   resetFrameBufPal();
   setFrameBufPalCol( 0, RGB15( 0, 0, 0 ) );            // 1 - black
   setFrameBufPalCol( 1, RGB15( 31, 31, 31 ) );      // 2 - white
   setFrameBufPalCol( 2, RGB15( 15, 15, 15 ) );      // 2 - gray
   setFrameBufPalCol( 16, RGB15(12, 16, 19) );
   setFrameBufPalCol( 17, RGB15(23, 9, 0) );
   setFrameBufPalCol( 18, RGB15(31, 0, 3) );
   setFrameBufPalCol( 19, RGB15(31, 17, 31) );
   setFrameBufPalCol( 20, RGB15(31, 18, 0) );
   setFrameBufPalCol( 21, RGB15(30, 28, 0) );
   setFrameBufPalCol( 22, RGB15(21, 31, 0) );
   setFrameBufPalCol( 23, RGB15(0, 31, 0) );
   setFrameBufPalCol( 24, RGB15(0, 20, 7) );
   setFrameBufPalCol( 25, RGB15(9, 27, 17) );
   setFrameBufPalCol( 26, RGB15(6, 23, 30) );
   setFrameBufPalCol( 27, RGB15(0, 11, 30) );
   setFrameBufPalCol( 28, RGB15(0, 0, 18) );
   setFrameBufPalCol( 29, RGB15(17, 0, 26) );
   setFrameBufPalCol( 30, RGB15(26, 0, 29) );
   setFrameBufPalCol( 31, RGB15(31, 0, 18) );
   flushFrameBufPal();
   resetFrameBuf();
   flushFrameBuf();
   resetFontAnchor();
   defaultExceptionHandler();
   irqInit();
   irqSet( IRQ_VBLANK, neoVblankIrqHandler );
   irqSet( IRQ_CARD_LINE, neoCardLineIrqHandler );
   irqEnable( IRQ_VBLANK | IRQ_CARD_LINE );
   REG_IME = 1;
}

void neoExit()   {
   if(m_cardPulled)
      card_init();
   irqClear( IRQ_VBLANK );
   irqClear( IRQ_CARD_LINE );
   REG_IME = 0;
   REG_IF = REG_IF;
   REG_IE = 0;
   IRQ_HANDLER = 0;
   setExceptionHandler((VoidFunctionPointer)0);
/*
   // Can't do this here :(
   extern int __dtcm_start;
   extern int __itcm_start;
   memset((void*)__dtcm_start, 0, 16*1024); // clear DTCM
   memset((void*)__itcm_start, 0, 32*1024); // clear ITCM
*/
   videoSetMode(0);
   videoSetModeSub(0);
   BG0_CR = 0;
   SUB_BG0_CR = 0;
   BG1_CR = 0;
   SUB_BG1_CR = 0;
   BG2_CR = 0;
   SUB_BG2_CR = 0;
   BG3_CR = 0;
   SUB_BG3_CR = 0;
   mem_set32( (u32*) 0x05000000, 0, 0x400 );
   mem_set32( (u32*) 0x6000000, 0, 128*1024 );
   mem_set32( (u32*) 0x6200000, 0, 128*1024 );
   mem_set32( (u32*) 0x07000000, 0, 0x800 );
   mem_set32( (u32*) SPRITE_GFX, 0, 128*1024 );
   mem_set32( (u32*) SPRITE_GFX_SUB, 0, 128*1024 );
   vramRestoreMainBanks(0);
   DMA0_SRC=DMA0_DEST=DMA0_CR=0;
   DMA1_SRC=DMA1_DEST=DMA1_CR=0;
   DMA2_SRC=DMA2_DEST=DMA2_CR=0;
   DMA3_SRC=DMA3_DEST=DMA3_CR=0;
   WAIT_CR=m_wait_cr;
}
