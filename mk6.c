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

#include "spi.h"
#include "mk6.h"
//#include <nds/arm9/card.h>

#include <stdio.h>



static int mEncrypted = 0;

void mk6_spi_command( const u8 *cmd, u32 len )
{
   u32 i;
   for(i=0;i<len;i++)
   {
      spi_open(0);
      spi( cmd );
      spi_close();
   }
}

void mk6_card_command( const u8 *cmd, u32 cmdLen, u8 *out, u32 outLen, u8 *in, u32 inLen )
{

   u32 i;
   u8 cardCmd[8] = {0,0,0,0,0,0,0,0};
   u8 buf[1024];
   u32 len=0;
   u32 flags;

   for(i=0;i<cmdLen;i++)
   {
      cardCmd[7 - i] = cmd;
   }

   for( /* i is initialized in previous for loop */ ;i<cmdLen+outLen;i++)
   {
      cardCmd[7 - i] = out[i-cmdLen];
   }


                              // 1<<31 = this is the card start bit and must be set
      flags = (1<<31) | (1<<29);   // (0<<27) = 6.7mhz
                              // (1<<27) = 4.2mhz

   if( inLen == 0 )
   {
      len = 0;
      flags |= 0;
   }
   else if( inLen <= 4 )
   {
      len = 4;
      flags |= (7<<24);
   }
   else if( inLen <= 512 )
   {
      len = 512;
      flags |= (1<<24);
   }
   else if( inLen <= 1024 )
   {
      len = 1024;
      flags |= (2<<24);
   }

   if( mEncrypted )
   {
      flags |= 0x00406000;
   }

   //flags |= 0x001808F8; // Set Latency 1 and 2 (~250us)

   //flags |= 0x00180140; // fifty

   //flags &= 0x003F1FFF; // zero


   cardPolledTransfer( flags, (uint32*)buf, (len / sizeof(u32)), cardCmd );

   for(i=0;i<inLen;i++)
   {
      if( in )
      {
         in=buf;
      }
   }

}

////////////////////////////////////////////////////////////////////////////////

//#ifdef MK6_V1

void mk6_disable()
{
   static const u8 c[ 4 ] = { 0xfe, 0xfd, 0xfb, 0xf5 };
   mk6_spi_command( c, 4 );
}

void mk6_enable()
{
   static const u8 c[ 4 ] = { 0xfe, 0xfd, 0xfb, 0xf7 };
   mk6_spi_command( c, 4 );
}

//#else
/*
void mk6_disable()
{
   static const u8 c[ 2 ] = { 0xfb, 0xf5 };
   mk6_spi_command( c, 2 );
}

void mk6_enable()
{
   static const u8 c[ 2 ] = { 0xfb, 0xf7 };
   mk6_spi_command( c, 2 );
}
*/
//#endif

void mk6_init( int encrypted )
{
   mEncrypted = encrypted;
   if( !encrypted )
   {
      CARD_1B0 = 0;
      CARD_1B4 = 0;
      CARD_1B8 = 0;
      CARD_1BA = 0;
   }
   mk6_enable();
}



////////////////////////////////////////////////////////////////////////////////

void mk6_nor_write_half( u32 addr, u16 data )
{
   static u8 c[ 8 ] = { 0xf3, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

   addr = addr / 2;

   c[2] = addr & 0xff;
   c[3] = ( addr >> 8 ) & 0xff;
   c[4] = ( addr >> 16 ) & 0xff;
   c[5] = ( addr >> 24 ) & 0xff;

   c[6] = data & 0xff;
   c[7] = ( data >> 8 ) & 0xff;

   mk6_card_command( c, 8, 0, 0, 0, 0 );
}


u16 mk6_nor_read_half( u32 addr )
{
   static u8 c[ 6 ] = { 0xf1, 0x01, 0x00, 0x00, 0x00, 0x00 };
   u16 data;

   addr = addr / 2;

   c[2] = addr & 0xff;
   c[3] = ( addr >> 8 ) & 0xff;
   c[4] = ( addr >> 16 ) & 0xff;
   c[5] = ( addr >> 24 ) & 0xff;

   mk6_card_command( c, 6, 0, 0, (u8*)&data, 2 );

   return data;
}

////////////////////////////////////////

#define MK6_NOR_TIMEOUT                  1024

#define MK6_LOCKING_OP_LOCK            0x01
#define MK6_LOCKING_OP_UNLOCK         0xD0
#define MK6_LOCKING_OP_LOCKDOWN      0x2F

void mk6_nor_read_array( u32 addr )
{
   mk6_nor_write_half( addr, 0xFF );
}

void mk6_nor_read_config( u32 addr )
{
   mk6_nor_write_half( addr, 0x90 );
}

void mk6_nor_locking_op( u32 addr, u32 op )
{
   mk6_nor_write_half( addr, 0x60 );
   mk6_nor_write_half( addr, op );
}

void mk6_nor_erase( u32 addr )
{
   u16 status;
   mk6_nor_write_half( addr, 0x20 );
   mk6_nor_write_half( addr, 0xD0 );
   while( 1 )
   {
      status = mk6_nor_read_half( addr );
      if( (status>>7) & 1 )
      {
         break;
      }
   }
}

void mk6_nor_program_word( u32 addr, u32 data )
{
   u16 status;
   mk6_nor_write_half( addr, 0x40 );
   mk6_nor_write_half( addr, data );
   while( 1 )
   {
      status = mk6_nor_read_half( addr );
      if( (status>>7) & 1 )
      {
         break;
      }
   }
}

static void db( const char *s )
{
      //swiWaitForVBlank();
      //clearFrameBuf();
      //drawFwTitle();
      //drawFwToolbar();
      //drawMessage( "%s", s );
      //flushFrameBuf();
      return;
      while( 1 )   {
         scanKeys();
         if( keysDown() & 1 )            {
            break;
         }
      }
}

void mk6_nor_program( u32 addr, u16 *buf, u32 size )
{
   u32 i;
//   u32 update;

//   char msg[1024];

//   db("erasing...");

   mk6_nor_locking_op( addr, MK6_LOCKING_OP_UNLOCK );
   mk6_nor_erase( addr );
   mk6_nor_read_array( addr );
   for(i=0;i<size;i+=2)
   {
      mk6_nor_locking_op( addr + i, MK6_LOCKING_OP_UNLOCK );
      mk6_nor_read_array( addr + i );
      if( mk6_nor_read_half( addr + i ) != 0xffff )
      {
         mk6_nor_erase( addr + i );
         mk6_nor_read_array( addr + i );
         if( mk6_nor_read_half( addr + i ) != 0xffff )
         {
            //sprintf( msg, "erase error at %x / %x", addr + i, size );
            //db(msg);
            while(1);
         }
      }
   }

//   db("flashing...");

   for(i=0;i<size;i+=2)
   {

      mk6_nor_program_word( addr + i, buf[ i/2 ] );
      mk6_nor_read_array( addr + i );
      if( mk6_nor_read_half( addr + i ) != buf[ i/2 ] )
      {
      //   sprintf( msg, "flash error at %x / %x", addr + i, size );
   //      db(msg);
         while(1);
      }



   }
}

////////////////////////////////////////////////////////////////////////////////

void mk6_sram_write_half( u32 addr, u16 data )
{
   static u8 c[ 8 ] = { 0xf3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

   addr = addr / 2;

   c[2] = addr & 0xff;
   c[3] = ( addr >> 8 ) & 0xff;
   c[4] = ( addr >> 16 ) & 0xff;
   c[5] = ( addr >> 24 ) & 0xff;

   c[6] = data & 0xff;
   c[7] = ( data >> 8 ) & 0xff;

   mk6_card_command( c, 8, 0, 0, 0, 0 );
}

u16 mk6_sram_read_half( u32 addr )
{
   static u8 c[ 8 ] = { 0xf1, 0x00, 0x00, 0x00, 0x00, 0x00 };
   u16 data;

   addr = addr / 2;

   c[2] = addr & 0xff;
   c[3] = ( addr >> 8 ) & 0xff;
   c[4] = ( addr >> 16 ) & 0xff;
   c[5] = ( addr >> 24 ) & 0xff;

   mk6_card_command( c, 6, 0, 0, (u8*)&data, 2 );

   return data;
}

////////////////////////////////////////////////////////////////////////////////

#ifdef MK6_ASM

void mk6_tf_cmd_write_byte( u8 data );
u8 mk6_tf_cmd_read_bits( u8 bits );
//u8 mk6_tf_data_read_bits( u8 bits );
u8 mk6_tf_data_read_4bits();
void mk6_tf_data_read_512bytes( u8 *buf );
//u8 mk6_tf_data_read_byte();
//void mk6_tf_data_read( u8 *buf, u32 len );

#else // MK6_ASM

void mk6_tf_cmd_write_byte( u8 data )
{
   //iprintf("send cmd byte %x\n", data);         // print here to be sure

   // this code works, can test exactly using button A

   //u8 buf[4];
   static u8 c[ 3 ] = { 0xd7, 0x57, 0x00 };
   c[ 2 ] = data;
   mk6_card_command( c, 3, 0, 0, 0, 2 );
}

// bits should be 1 to 8 please!

u8 mk6_tf_cmd_read_bits( u8 bits )
{
   u8 dat = 0;
   u8 size;
   int i;
   //static u8 c[ 2 ] = { 0xd7, 0x65 };
   u8 c[2];
   u8 buf[8];
   c[0] = 0xd7;
   while (bits) {
      size = (bits > 4) ? 4: bits;
      c[1] = 0x65 + size;
      mk6_card_command( c, 2, 0, 0, buf, size );
      for(i=0;i<size;i++)
      {
         dat <<= 1;
         dat |= (0x1 & buf);
      }
      bits -= size;
   }
   return dat;
}

/*u8 mk6_tf_data_read_bits( u8 bits )
{
   int i;
   u8 buf[2];
   u8 dat = 0;
   static u8 c[ 2 ] = { 0xd7, 0x40 };
   mk6_card_command( c, 2, 0, 0, buf, 2 );
   for(i=0;i<2;i++)
   {
      dat <<= 4;
      dat |= (0xF & buf);
   }
   return dat;
}*/

u8 mk6_tf_data_read_4bits()
{
   static const u8 c[ 3 ] = { 0xd7, 0x46, 0x00 };
   u8 dat;
   mk6_card_command( c, 3, 0, 0, &dat, 1 );
   return dat&15;
}

void mk6_tf_data_read_512bytes( u8 *buf )
{
   // X2
   /*
   static const u8 c[ 3 ] = { 0xd7, 0x4E, 0x01 };
   mk6_card_command( c, 3, 0, 0, buf, 512 );
   */

   // X1
   static const u8 c[ 3 ] = { 0xd7, 0x4E, 0x00 };
   u8 ibuf[1024];
   int i;
   mk6_card_command( c, 3, 0, 0, ibuf, 1024 );
   for(i=0;i<512;i++)
   {
      buf=(ibuf[(i*2)]&15)<<4;
      buf|=(ibuf[(i*2)+1]&15);
   }
}

/*u8 mk6_tf_data_read_byte()
{
   int i;
   u8 buf[2];
   u8 dat = 0;
   static u8 c[ 2 ] = { 0xd7, 0x40 };
   mk6_card_command( c, 2, 0, 0, buf, 2 );
   for(i=0;i<2;i++)
   {
      dat <<= 4;
      dat |= (0xF & buf);
   }
   return dat;
}*/

/*void mk6_tf_data_read( u8 *buf, u32 len )
{
   u32 i;
   for(i=0;i<len;i++)
   {
      buf = mk6_tf_data_read_byte();
   }
}*/

#endif // MK6_ASM

u8 mk6_tf_cmd_read_byte()
{
      return mk6_tf_cmd_read_bits(8);
}

void mk6_tf_cmd_write( const u8 *buf, u32 len )
{
   u32 i;
   for(i=0;i<len;i++)
   {
      mk6_tf_cmd_write_byte( buf );
   }
}

void mk6_tf_cmd_read( u8 *buf, u32 len )
{
   u32 i;
   for(i=0;i<len;i++)
   {
      buf = mk6_tf_cmd_read_byte();
   }
}


void mk6_tf_data_write_nybble( u8 data )
{
   static u8 c[ 3 ] = { 0xd7, 0x38, 0x00 };
   c[ 2 ] = (data&15)<<4;
   mk6_card_command( c, 3, 0, 0, 0, 0 );   
}

void mk6_tf_data_write_byte( u8 data )
{
   mk6_tf_data_write_nybble( data >> 4 );
   mk6_tf_data_write_nybble( data );
}

void mk6_tf_data_write( const u8 *buf, u32 len )
{
   u32 i;
   for(i=0;i<len;i++)
   {
      mk6_tf_data_write_byte( buf );
   }
}
