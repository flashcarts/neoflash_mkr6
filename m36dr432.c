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
// 32 Mbit (2Mb x16, Dual Bank, Page) Flash Memory
// and 4 Mbit (256K x16) SRAM, Multiple Memory Product

// ELECTRONIC SIGNATURE
// û Manufacturer Code: 20h
// û Top Device Code, M36DR432A: 00A0h
// û Bottom Device Code, M36DR432B: 00A1h

//Table 4. Bank Size and Sectorization

//Bank Size Parameter Blocks Main Blocks
//Bank A 4 Mbit 8 blocks of 4 KWord 7 blocks of 32 KWord
//Bank B 28 Mbit - 56 blocks of 32 KWord

#define ADDR( addr )                         (0x08000000|(addr))
#define READ_WORD( addr )                *(vu16*)( ADDR( addr ) )
#define WRITE_WORD( addr, data )      READ_WORD( addr ) = data

void intel28f1602ReadArray()
{
   WRITE_WORD( 0, 0xFF );
   // two coded cycles?
   READ_WORD( 0 );
   READ_WORD( 0 );
}

void intel28f1602ReadIdentifier()
{
   WRITE_WORD( 0, 0x90 );
}

void intel28f1602ReadStatusRegister( u32 addr )
{
   WRITE_WORD( 0, 0x70 );
}

void intel28f1602CfiQuery()
{
   WRITE_WORD( 0, 0x98 );
}

void intel28f1602BlockUnlock( u32 addr )
{
      WRITE_WORD( 0, 0x60 );
      WRITE_WORD( 0, 0xD0 );
}

void intel28f1602WordProgram( u32 addr, u16 data )
{
   WRITE_WORD( 0, 0x40 );
   WRITE_WORD( addr, data );
   while( ( READ_WORD( addr ) & 0x80 ) == 0x00 )
   {
      // do nothing
   }
   //while( ( READ_WORD( addr ) & (1<<2) ) != 0x00 )
   //{
      // do nothing
   //}
   
}

void intel28f1602BlockErase( u32 addr )
{
   WRITE_WORD( addr, 0xB0 );
   intel28f1602ReadStatusRegister( addr );
   while( ( READ_WORD( addr ) & 0x80 ) == 0x00 )
   {
      // do nothing
   }
   //while( ( READ_WORD( addr ) & 0x40 ) != 0x00 )
   //{
      // do nothing
   //}
   
}




void m36dr432ReadResetMemoryArray()   {
   WRITE_WORD( 0, 0xF0 );   // any addr allowed
   // two coded cycles ?
   READ_WORD( 0 );
   READ_WORD( 0 );
}

void m36dr432AutoSelect()   {
   WRITE_WORD( 0x555*2, 0xAA );   
   WRITE_WORD( 0x2AA*2, 0x55 );
   WRITE_WORD( 0x555*2, 0x90 );
}

void m36dr432CfiQuery()   {
   WRITE_WORD( 0x55*2, 0x98 );
}

void m36dr432Program( u32 addr, u16 data )   {
   WRITE_WORD( 0x555*2, 0xAA );
   WRITE_WORD( 0x2AA*2, 0x55 );
   WRITE_WORD( 0x555*2, 0xA0 );
   WRITE_WORD( addr, data );
   while( ( READ_WORD(addr) & 0x80 ) == ( data & 0x80 ) )   {
      // do nothing
      // todo: check for program error bit
   }
   // todo: check for program error bit
}

void m36dr432EnterBypassMode()   {
   WRITE_WORD( 0x555*2, 0xAA );
   WRITE_WORD( 0x2AA*2, 0x55 );
   WRITE_WORD( 0x555*2, 0x20 );
}

void m36dr432ExitBypassMode()   {
   WRITE_WORD( 0, 0x90 );   // any addr allowed
   WRITE_WORD( 0, 0x00 );   // any addr allowed
}

void m36dr432ProgramInBypassMode( u32 addr, u16 data )   {
   WRITE_WORD( 0, 0xA0 );   // any addr allowed
   WRITE_WORD( addr, data );
   while( ( data ^ READ_WORD(addr) ) & 0x80 )   {
      // do nothing
      // todo: check for program error bit
   }
   // todo: check for program error bit
}

void m36dr432BlockProtect( u32 addr )   {
   WRITE_WORD( 0x555*2, 0xAA );
   WRITE_WORD( 0x2AA*2, 0x55 );
   WRITE_WORD( 0x555*2, 0x60 );
   WRITE_WORD( addr, 0x01 );
}

void m36dr432BlockUnprotect( u32 addr )   {
   WRITE_WORD( 0x555*2, 0xAA );
   WRITE_WORD( 0x2AA*2, 0x55 );
   WRITE_WORD( 0x555*2, 0x60 );
   WRITE_WORD( addr, 0xD0 );
}

void m36dr432BlockLock( u32 addr )   {
   WRITE_WORD( 0x555*2, 0xAA );
   WRITE_WORD( 0x2AA*2, 0x55 );
   WRITE_WORD( 0x555*2, 0x60 );
   WRITE_WORD( addr, 0x2F );
}

bool m32dr432BlockErase( u32 addr )   {
   WRITE_WORD( 0x555*2, 0xAA );
   WRITE_WORD( 0x2AA*2, 0x55 );
   WRITE_WORD( 0x555*2, 0x80 );
   WRITE_WORD( 0x555*2, 0xAA );
   WRITE_WORD( 0x2AA*2, 0x55 );
   WRITE_WORD( addr, 0x30 );
   while( ( READ_WORD(addr) & 0x80 ) == 0 )   {
      // do nothing
      // todo: check for erase error bit
   }
   // todo: check for erase error bit
   return true;
}

void m32dr432BankErase( u32 addr )   {
   addr = addr/2;
   WRITE_WORD( 0x555*2, 0xAA );
   WRITE_WORD( 0x2AA*2, 0x55 );
   WRITE_WORD( 0x555*2, 0x80 );
   WRITE_WORD( 0x555*2, 0xAA );
   WRITE_WORD( 0x2AA*2, 0x55 );
   WRITE_WORD( addr, 0x10 );
   while( ( READ_WORD(addr) & 0x80 ) == 0 )   {
      // do nothing
      // todo: check for erase error bit
   }
   // todo: check for erase error bit
   return true;
}

