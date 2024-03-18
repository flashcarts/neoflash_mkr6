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

#include "io_mk6.h"

#include "mk6.h"

///////////////////////////////////////////////////////////////////////////////

#ifdef MK6_ASM

void mk6_tf_cmd( u8 cmd, u32 arg );
bool mk6_tf_cmd_resp( u8 *resp, u32 len );
bool mk6_tf_cmd_resp1( u8 *resp );
bool mk6_tf_read_sector_b( u8 *buf, u32 addr );

#else // MK6_ASM

static const unsigned char crcTab[256]={
   0x00, 0x09, 0x12, 0x1B, 0x24, 0x2D, 0x36, 0x3F, 0x48, 0x41, 0x5A, 0x53, 0x6C, 0x65, 0x7E, 0x77,
   0x19, 0x10, 0x0B, 0x02, 0x3D, 0x34, 0x2F, 0x26, 0x51, 0x58, 0x43, 0x4A, 0x75, 0x7C, 0x67, 0x6E,
   0x32, 0x3B, 0x20, 0x29, 0x16, 0x1F, 0x04, 0x0D, 0x7A, 0x73, 0x68, 0x61, 0x5E, 0x57, 0x4C, 0x45,
   0x2B, 0x22, 0x39, 0x30, 0x0F, 0x06, 0x1D, 0x14, 0x63, 0x6A, 0x71, 0x78, 0x47, 0x4E, 0x55, 0x5C,
   0x64, 0x6D, 0x76, 0x7F, 0x40, 0x49, 0x52, 0x5B, 0x2C, 0x25, 0x3E, 0x37, 0x08, 0x01, 0x1A, 0x13,
   0x7D, 0x74, 0x6F, 0x66, 0x59, 0x50, 0x4B, 0x42, 0x35, 0x3C, 0x27, 0x2E, 0x11, 0x18, 0x03, 0x0A,
   0x56, 0x5F, 0x44, 0x4D, 0x72, 0x7B, 0x60, 0x69, 0x1E, 0x17, 0x0C, 0x05, 0x3A, 0x33, 0x28, 0x21,
   0x4F, 0x46, 0x5D, 0x54, 0x6B, 0x62, 0x79, 0x70, 0x07, 0x0E, 0x15, 0x1C, 0x23, 0x2A, 0x31, 0x38,
   0x41, 0x48, 0x53, 0x5A, 0x65, 0x6C, 0x77, 0x7E, 0x09, 0x00, 0x1B, 0x12, 0x2D, 0x24, 0x3F, 0x36,
   0x58, 0x51, 0x4A, 0x43, 0x7C, 0x75, 0x6E, 0x67, 0x10, 0x19, 0x02, 0x0B, 0x34, 0x3D, 0x26, 0x2F,
   0x73, 0x7A, 0x61, 0x68, 0x57, 0x5E, 0x45, 0x4C, 0x3B, 0x32, 0x29, 0x20, 0x1F, 0x16, 0x0D, 0x04,
   0x6A, 0x63, 0x78, 0x71, 0x4E, 0x47, 0x5C, 0x55, 0x22, 0x2B, 0x30, 0x39, 0x06, 0x0F, 0x14, 0x1D,
   0x25, 0x2C, 0x37, 0x3E, 0x01, 0x08, 0x13, 0x1A, 0x6D, 0x64, 0x7F, 0x76, 0x49, 0x40, 0x5B, 0x52,
   0x3C, 0x35, 0x2E, 0x27, 0x18, 0x11, 0x0A, 0x03, 0x74, 0x7D, 0x66, 0x6F, 0x50, 0x59, 0x42, 0x4B,
   0x17, 0x1E, 0x05, 0x0C, 0x33, 0x3A, 0x21, 0x28, 0x5F, 0x56, 0x4D, 0x44, 0x7B, 0x72, 0x69, 0x60,
   0x0E, 0x07, 0x1C, 0x15, 0x2A, 0x23, 0x38, 0x31, 0x46, 0x4F, 0x54, 0x5D, 0x62, 0x6B, 0x70, 0x79
};


static u8 crc7 (const unsigned char *buf,unsigned int len)
{
  register unsigned int i;
  u8 crc7_accum = 0;
  register u8 crc=0;
  for (i = 0;  i < len;  ++i)   {
      crc7_accum = crcTab[(crc7_accum << 1) ^ buf];
  }
  crc ^= crc7_accum;
  return crc;
}

static void mk6_tf_cmd( u8 cmd, u32 arg )
{
   u8 p[6];
   p[0]=cmd|0x40;
   p[1]=(u8)(arg >> 24);
   p[2]=(u8)(arg >> 16);
   p[3]=(u8)(arg >> 8);
   p[4]=(u8)(arg);
   p[5]=( crc7(p,5) << 1 ) | 1;
   mk6_tf_cmd_write_byte( 0xff );
   mk6_tf_cmd_write_byte( 0xff );
   mk6_tf_cmd_write_byte( p[0] );
   mk6_tf_cmd_write_byte( p[1] );
   mk6_tf_cmd_write_byte( p[2] );
   mk6_tf_cmd_write_byte( p[3] );
   mk6_tf_cmd_write_byte( p[4] );
   mk6_tf_cmd_write_byte( p[5] );

}

static bool mk6_tf_cmd_resp( u8 *resp, u32 len )
{
    u32 i,j;
    for (i=0; i<512; i++)
   {
        if(mk6_tf_cmd_read_bits(1)==0)
      {
            *resp++ = mk6_tf_cmd_read_bits(7);
            len--;
            for(j=0; j<len; j++)
         {
               *resp++ = mk6_tf_cmd_read_byte();
          }
          return 1;
        }
    }
    return 0;
}

static bool mk6_tf_cmd_resp1( u8 *resp )   {
   return mk6_tf_cmd_resp( resp, 48/8 );
}

#endif // MK6_ASM

static bool mk6_tf_cmd_resp2( u8 *resp )   {
   return mk6_tf_cmd_resp( resp, 136/8 );
}

static bool mk6_tf_cmd_resp3( u8 *resp )   {
   return mk6_tf_cmd_resp( resp, 48/8 );
}

static bool mk6_tf_cmd_resp6( u8 *resp )   {
   return mk6_tf_cmd_resp( resp, 48/8 );
}

static void db_resp( u8 cmd, u8 *resp )   {
   iprintf( "CMD%u: %02X %02X %02X %02X %02X %02X\n", cmd, resp[0], resp[1], resp[2], resp[3], resp[4], resp[5] );
}

static void db_resp6( u8 cmd, u8 *resp )   {
   iprintf( "CMD%u: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n", cmd, resp[0], resp[1], resp[2], resp[3], resp[4], resp[5], resp[6], resp[7], resp[8], resp[9], resp[10], resp[11], resp[12], resp[13], resp[14], resp[15], resp[16] );
}

static void db_aresp( u8 cmd, u8 *resp )   {
   iprintf( "ACMD%u: %02X %02X %02X %02X %02X %02X\n", cmd, resp[0], resp[1], resp[2], resp[3], resp[4], resp[5] );
}


///////////////////////////////////////////////////////////////////////////////

bool mk6_tf_go_idle_state_cmd()
{
   mk6_tf_cmd( 0, 0 );
   return true;
}

bool mk6_tf_send_app_cmd( u16 rca )
{
   u8 resp[48/8] = { 0, 0, 0, 0, 0, 0 };

   //iprintf( "send app cmd...\n" );

   mk6_tf_cmd( 55, ((rca << 16) | 0xFFFF) );
   if( mk6_tf_cmd_resp1( resp ) && (resp[0] == 55) )
   {
      db_resp( 55, resp );
      return true;
   }
   db_resp( 55, resp );
   iprintf( "error\n" );
   return false;
}

bool mk6_tf_send_op_cond_cmd( u32 ocr, u32 *ocr_out )
{
   u8 resp[48/8] = { 0, 0, 0, 0, 0, 0 };

   if( mk6_tf_send_app_cmd(0) )
   {

      //iprintf( "send op cond...\n" );

      mk6_tf_cmd( 41, ocr );
      if( mk6_tf_cmd_resp3( resp ) && (resp[0] == 0x3f) && (resp[1] & 0x80) )
      {
         if (ocr_out)
         {
            *ocr_out = ((resp[1] << 24) | (resp[2] << 16) | (resp[3] << 8) | resp[4]);
         }
         db_aresp( 41, resp );
         return true;
      }
      db_aresp( 41, resp );
   }
   iprintf( "error\n" );
   return false;
}

bool mk6_tf_all_send_cid()   {
   u8 resp[136/8];
   mk6_tf_cmd( 2, 0xFFFFFFFF );
   {
      if( mk6_tf_cmd_resp2( resp ) && (resp[0] == 0x3f) )
      {
         db_resp6( 2, resp );
         return true;
      }
      db_resp6( 2, resp );
   }
   iprintf( "error\n" );
   return false;
}

bool mk6_tf_send_relative_addr_cmd( u16 *rca )
{
   u8 resp[48/8];
   mk6_tf_cmd( 3, 1 );
   {
      if( mk6_tf_cmd_resp6( resp ) && (resp[0] == 3) )
      {
         *rca = (resp[1]<<8) | resp[2];
         db_resp( 3, resp );
         return true;
      }
      db_resp( 3, resp );
   }
   iprintf( "error\n" );
   return false;
}

bool mk6_tf_select_cmd( u16 rca )   {
   u8 resp[48/8];
   mk6_tf_cmd( 7, (((rca>>8)&0xFF)<<24) | ((rca&0xFF)<<16) | 0xFFFF );
   {
      if( mk6_tf_cmd_resp1( resp ) && (resp[0] == 7) )   {
         db_resp( 7, resp );
         return true;
      }
      db_resp( 7, resp );
   }
   iprintf( "error\n" );
   return false;
}

bool mk6_tf_set_bus_width_cmd( u16 rca, u8 width )   {
   u8 resp[48/8];
   if( width == 4 )   width = 2;
   else if( width == 1 )   width = 0;
   else return false;
   if( mk6_tf_send_app_cmd(rca) )   {
      mk6_tf_cmd( 6, width );
      if( mk6_tf_cmd_resp1( resp ) && (resp[0] == 6) )
      {
         db_resp( 6, resp );
         return true;
      }
      db_resp( 6, resp );
   }
   iprintf( "error\n" );
   return false;
}

bool mk6_tf_init()
{

   u32 ocr;
   u16 rca;
   int i;

   for(i=0;i<50;i++)
   {
      mk6_tf_cmd_write_byte( 0xff );
   }

   mk6_tf_go_idle_state_cmd();

   for(i=0;i<1024;i++)
   {
      if( mk6_tf_send_op_cond_cmd( 0x00300000, &ocr ) )
      {
         break;
      }
   }

   if( i == 1024 )
   {
      return false;
   }

   //iprintf( "Success.\n" );
   if( mk6_tf_all_send_cid() )
   {
      if( mk6_tf_send_relative_addr_cmd( &rca ) )
      {
         if( mk6_tf_select_cmd( rca ) )
         {
            if( mk6_tf_set_bus_width_cmd( rca, 4 ) )
            {
               iprintf( "tf init great success!\n" );
               return true;
            }
         }
      }
   }
   return false;
}

bool mk6_tf_read_sector_a( u8 *buf, u32 addr )
{
   int i,j;
   u8 resp[6];
   mk6_tf_cmd( 17, addr );
   if( mk6_tf_cmd_resp1( resp ) && (resp[0] == 17) )   {
      db_resp( 17, resp );
      while( mk6_tf_data_read_4bits() != 0 )   {
         ;
      }

      for (i=0; i<512; i++)   {
         buf=0;
         for( j=0;j<2;j++)   {
            buf = buf << 4;
            buf |= mk6_tf_data_read_4bits();
         }
      }

      //mk6_tf_data_read_512bytes( buf );

      // Clock out CRC
      mk6_tf_data_read_4bits();
      mk6_tf_data_read_4bits();
      mk6_tf_data_read_4bits();
      mk6_tf_data_read_4bits();

      // Clock out end bit
      mk6_tf_data_read_4bits();
      //db_resp( 6, resp );
      iprintf( "tf read great success!\n" );
      return true;

   }
   db_resp( 17, resp );
   iprintf( "error\n" );
   return false;
}

#ifndef MK6_ASM

bool mk6_tf_read_sector_b( u8 *buf, u32 addr )
{
   int i,j;
   u8 resp[6];
   mk6_tf_cmd( 17, addr );
   if( mk6_tf_cmd_resp1( resp ) && (resp[0] == 17) )   {
      db_resp( 17, resp );
      while( mk6_tf_data_read_4bits() != 0 )   {
         ;
      }

      /*for (i=0; i<512; i++)   {
         buf=0;
         for( j=0;j<2;j++)   {
            buf = buf << 4;
            buf |= mk6_tf_data_read_4bits();
         }
      }*/

      mk6_tf_data_read_512bytes( buf );

      // Clock out CRC
      mk6_tf_data_read_4bits();
      mk6_tf_data_read_4bits();
      mk6_tf_data_read_4bits();
      mk6_tf_data_read_4bits();

      // Clock out end bit
      mk6_tf_data_read_4bits();
      //db_resp( 6, resp );
      iprintf( "tf read great success!\n" );
      return true;

   }
   db_resp( 17, resp );
   iprintf( "error\n" );
   return false;
}

#endif // MK6_ASM

bool mk6_tf_write_sector( u8 *buf, u32 addr )
{
   int i,j;
   u8 resp[6];
   u8 crcbuf[8];
   u8 crc_stat = 0;
   
   //memset(crcbuf, 0, 8); // crc in = 0
   *(u32*)&crcbuf[0]=0;
   *(u32*)&crcbuf[4]=0;
   sdCrc16(crcbuf, buf, 512); // Calculate crcs
   
   drawMessage( "write %x", addr );
   
   //return true;
   
   mk6_tf_cmd( 24, addr );
   if( mk6_tf_cmd_resp1( resp ) && (resp[0] == 24) )   {
      //drawFwFatal( "Write Sector Success\n" );
      //db_resp( 24, resp );
      
      drawMessage( "write %x cmd ok", addr );
      
   for (i = 0; i < 128; i++)   // this can probably be reduced to zero
   mk6_tf_data_write_byte( 0xff );   // write 40 P bits... should be more than enough

   mk6_tf_data_write_byte( 0xf0 );   // write 4 dummy 1s and 4 start bits ??
   
   // write out data and crc bytes
   for (i=0; i<512; i++)   {
      mk6_tf_data_write_byte( buf );         // do these need swapped?
   }
   for (i=0; i<8; i++)   {
      mk6_tf_data_write_byte( crcbuf );
   }

   mk6_tf_data_write_byte( 0xff );   // write 4 end bits
   
   //for(i=0;i<16*1024;i++) // this can probably be reduced to near zero
   //{
   //   mk6_tf_data_read_4bits();
   //}
   
   //while( mk6_tf_data_read_4bits() & 1 == 0 );
   
   return true;
   
   }
   //drawFwFatal( "Write Sector Failed\n" );
   //while(1);
   //db_resp( 24, resp );
   //iprintf( "error\n" );
   drawMessage( "write %x cmd error", addr );
   return false;
}

// Neoflash MK6 TF driver functions

bool MK6_IsInserted(void)   {
   return true;
}

bool MK6_ClearStatus (void) {
   return true;
}

bool MK6_Shutdown(void) {
   return true;
}

bool MK6_StartUp(void) {
      mk6_init(0);
   return mk6_tf_init();
}

bool MK6_WriteSectors (u32 sector, u8 numSecs, void* buffer)
{
   int i;
   int inumSecs=numSecs;
   u8 *p=(u8*)buffer;
   if( numSecs == 0 )   inumSecs = 256;
   for( i=0; i<inumSecs; i++ )   {
      if(  mk6_tf_write_sector( p, sector * 512 ) == false )   {
         return false;
      }
      sector++;
      p+=512;
   }
   return true;
}

bool MK6_ReadSectors (u32 sector, u8 numSecs, void* buffer)
{

   int inumSecs=numSecs;
   int i;
   u8 *p=(u8*)buffer;
   if( numSecs == 0 )   inumSecs = 256;
   for( i=0; i<inumSecs; i++ )      {
      if(   mk6_tf_read_sector_b( p, sector * 512 ) == false )   {
         return false;
      }
      p+=512;
      sector++;
   }
   return true;

}


IO_INTERFACE io_mk6 = {
   DEVICE_TYPE_MK6,
   FEATURE_MEDIUM_CANREAD | FEATURE_MEDIUM_CANWRITE | FEATURE_SLOT_NDS,
   (FN_MEDIUM_STARTUP)&MK6_StartUp,
   (FN_MEDIUM_ISINSERTED)&MK6_IsInserted,
   (FN_MEDIUM_READSECTORS)&MK6_ReadSectors,
   (FN_MEDIUM_WRITESECTORS)&MK6_WriteSectors,
   (FN_MEDIUM_CLEARSTATUS)&MK6_ClearStatus,
   (FN_MEDIUM_SHUTDOWN)&MK6_Shutdown
} ;


LPIO_INTERFACE MK6_GetInterface(void) {
   return &io_mk6 ;
}