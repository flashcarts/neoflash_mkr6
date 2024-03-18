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

/*

   IO Status Register
   ------------------
   
   0 = input, 1 = output
   
   bit0 - "sd detect input" io - apparently in input mode this can be used to detect
                                             - sd card
   bit1 - "menu flash" io
   bit2 - "ir" io
   bit3 - "menu flash" io
   
   bit4 to 7 - the output value if corresponding bit 1 is set
   
   bit8 - psram enable
   
   bit 9 - ?
   bit 10 - ?
   bit 11 - ?
   
   bit12 - ir enable
   
   bit 13 - ?
   bit 14 - ?
   bit 15 - ?

*/




#define NEO2_ERROR   -1
#define NEO2_OK         0

#define NEO2_FLASH_UNLOCK      0
#define NEO2_FLASH_LOCK         1
#define NEO2_FLASH_LOCKDOWN   2

#define GBA_SRAM_ADDRESS      0x0A000000
#define GBA_FLASH_ADDRESS    0x08000000

#define NEO2_MAGIC_OFFS 0

#define NEO2_VOLTAGE_BYTE_OFFS               5               
#define NEO2_CONSOLE_VOLTAGE_BYTE_OFFS   6   

#define NEO2_RTC_1_SEC_4BIT_OFFS            0x4000
#define NEO2_RTC_10_SEC_3BIT_OFFS            0x4100
#define NEO2_RTC_1_MIN_4BIT_OFFS            0x4200
#define NEO2_RTC_10_MIN_3BIT_OFFS            0x4300
#define NEO2_RTC_HOUR_5BIT_OFFS               0x4400
#define NEO2_RTC_DAY_LO_4BIT_OFFS         0x4500
#define NEO2_RTC_DAY_MD_4BIT_OFFS         0x4600
#define NEO2_RTC_DAY_HI_4BIT_OFFS         0x4700

#define NEO2_GTC_1_MIN_4BIT_OFFS            0x4800
#define NEO2_GTC_10_MIN_3BIT_OFFS            0x4900
#define NEO2_GTC_HOUR_5BIT_OFFS               0x4a00
#define NEO2_GTC_DAY_LO_4BIT_OFFS         0x4b00
#define NEO2_GTC_DAY_HI_4BIT_OFFS         0x4c00

#define NEO2_CR1_16BIT_REG_OFFS               0x8040
#define NEO2_GAME_SRAM_DEV_ID_OFFS         0x8050
#define NEO2_CR_16BIT_REG_OFFS               0x8070
#define NEO2_IOSR_16BIT_REG_OFFS            0x8080
#define NEO2_GAME_FLASH_BANK_OFFS            0x8090
#define NEO2_GAME_SRAM_BANK_OFFS            0x80A0

#define NEO2_CACHE_OFFS                           0xC000         // offset of NEO2 inner cache into SRAM

// additional useful constants

#define NEO2_MAGIC_LEN   5

#define NEO2_MAGIC_0      0x34
#define NEO2_MAGIC_1      0x16
#define NEO2_MAGIC_2      0x96
#define NEO2_MAGIC_3      0x24
#define NEO2_MAGIC_4      0xf6


#define NEO2_GAME_FLASH_BANK_MASK            0x1fff
#define NEO2_GAME_SRAM_BANK_MASK            0x3f

#define NEO2_CACHE_SIZE      0x400

// this macro generates efficent code for all NEO2 ASIC commands
// the internal variable is required by the macro
static u16 _neo2_asic_command_dummy;
#define NEO2_ASIC_COMMAND( addr, data ) _neo2_asic_command_dummy = *(vu16*)( 0x08000000 + ( (addr<<16) * 2) + ( data * 2 ) )


// common NEO2 ASIC CR constants

#define NEO2_ASIC_CR_FLASH_GAME         (1<<9)
#define NEO2_ASIC_CR_CS_DELAY_EN      (1<<11)
#define NEO2_ASIC_CR_WRITE_FLASH_EN (1<<13)

// this inline function is called for all the lowest level commands

void neo2_asic_pre_command()   {
   NEO2_ASIC_COMMAND( 0xff, 0xd200 );
   NEO2_ASIC_COMMAND( 0x00, 0x1500 );
   NEO2_ASIC_COMMAND( 0x01, 0xd200 );
   NEO2_ASIC_COMMAND( 0x02, 0x1500 );
   NEO2_ASIC_COMMAND( 0xfe, 0x1500 );
}

void neo2_asic_flash_psram_write_enable()   {
   neo2_asic_pre_command();
   NEO2_ASIC_COMMAND( 0xe2, 0x1500 );
}

void neo2_asic_flash_psram_write_disable()   {
   neo2_asic_pre_command();
   NEO2_ASIC_COMMAND( 0xe2, 0xd200 );
}

void neo2_asic_fat_sram_write_enable()   {
   neo2_asic_pre_command();
   NEO2_ASIC_COMMAND( 0xe2, 0xdd00 );
}

void neo2_asic_fat_sram_write_disable()   {
   neo2_asic_pre_command();
   NEO2_ASIC_COMMAND( 0xe2, 0x3900 );
}

void neo2_asic_fat_sram_read_enable()   {
   neo2_asic_pre_command();
   NEO2_ASIC_COMMAND( 0xe2, 0x7e00 );
}

void neo2_asic_fat_sram_read_disable()   {
   neo2_asic_pre_command();
   NEO2_ASIC_COMMAND( 0xe2, 0x9400 );
}

void set_neo2_asic_game_sram_bank( u16 bank )   {
   neo2_asic_pre_command();
   NEO2_ASIC_COMMAND( 0xe0, bank );
}

void set_neo2_asic_game_flash_bank( u16 bank )   {
   neo2_asic_pre_command();
   NEO2_ASIC_COMMAND( 0xc4, bank );
}

void neo2_asic_command_e7( u16 a )   {
   neo2_asic_pre_command();
   NEO2_ASIC_COMMAND( 0xe7, a );
}

void set_neo2_asic_game_sram_device_id( u16 device_id )   {
   neo2_asic_pre_command();
   NEO2_ASIC_COMMAND( 0xe9, device_id );
}

void set_neo2_asic_iosr( u16 iosr )   {
   neo2_asic_pre_command();
   NEO2_ASIC_COMMAND( 0xda, iosr );
}

void set_neo2_asic_cr( u16 cr )   {
   neo2_asic_pre_command();
   NEO2_ASIC_COMMAND( 0x37, cr );
}

void set_neo2_asic_cr1( u16 cr )   {
   neo2_asic_pre_command();
   NEO2_ASIC_COMMAND( 0xee, cr );
}

void neo2_asic_chip_id_mode_enable()   {
   neo2_asic_pre_command();
   NEO2_ASIC_COMMAND( 0x90, 0x3500 );
}

void neo2_asic_chip_id_mode_disable()   {
   neo2_asic_pre_command();
   NEO2_ASIC_COMMAND( 0x90, 0x4900 );
}

void neo2_asic_command_9c( u16 a )   {
   neo2_asic_pre_command();
   NEO2_ASIC_COMMAND( 0x9C, a );
}

void neo2_asic_command_9d( u16 a )   {
   neo2_asic_pre_command();
   NEO2_ASIC_COMMAND( 0x9D, a );
}

void neo2_asic_command_9e( u16 a )   {
   neo2_asic_pre_command();
   NEO2_ASIC_COMMAND( 0x9E, a );
}

void neo2_asic_command_9f( u16 a )   {
   neo2_asic_pre_command();
   NEO2_ASIC_COMMAND( 0x9F, a );
}

////////////////////////////////////////////////////////////////////////////////

void neo2_chip_id_read( u8 *buf, u32 offs, u32 len )   {
   vu8 *p=(vu8*)(GBA_SRAM_ADDRESS+offs);
   while(len)   {
      *buf++=*p++;
      len--;
   }   
}

void neo2_chip_id_write( u8 *buf, u32 offs, u32 len )   {
   vu8 *p=(vu8*)(GBA_SRAM_ADDRESS+offs);
   while(len)   {
      *p++=*buf++;
      len--;
   }   
}

u8 neo2_chip_id_read_byte( u32 offs )   {
   return *(vu8*)(GBA_SRAM_ADDRESS+offs);   
}

void neo2_chip_id_write_byte( u32 offs, u8 data )   {
   *(vu8*)(GBA_SRAM_ADDRESS+offs) = data;   
}

u16 neo2_chip_id_read_16bit_reg( u32 offs )   {
   int i;
   u16 val = 0;
   for(i=0;i<16;i++)
      val |= (neo2_chip_id_read_byte( offs + i )&1) << i;
   return val;
}

////////////////////////////////////////////////////////////////////////////////

#define get_neo2_voltage()                     neo2_chip_id_read_byte(NEO2_VOLTAGE_BYTE_OFFS)
#define get_neo2_console_voltage()         neo2_chip_id_read_byte(NEO2_CONSOLE_VOLTAGE_BYTE_OFFS)

#define get_neo2_cr1()                           neo2_chip_id_read_16bit_reg(NEO2_CR1_16BIT_REG_OFFS)
#define get_neo2_cr()                              neo2_chip_id_read_16bit_reg(NEO2_CR_16BIT_REG_OFFS)
#define get_neo2_iosr()                           neo2_chip_id_read_16bit_reg(NEO2_IOSR_16BIT_REG_OFFS)
#define get_neo2_game_sram_device_id()   neo2_chip_id_read_16bit_reg(NEO2_GAME_SRAM_DEV_ID_OFFS)
#define get_neo2_game_flash_bank()         (neo2_chip_id_read_16bit_reg(NEO2_GAME_FLASH_BANK_OFFS)&NEO2_GAME_FLASH_BANK_MASK)
#define get_neo2_game_sram_bank()            (neo2_chip_id_read_16bit_reg(NEO2_GAME_SRAM_BANK_OFFS)&NEO2_GAME_SRAM_BANK_MASK)

#define open_neo2_cache()                        neo2_asic_chip_id_mode_enable()
#define close_neo2_cache()                      neo2_asic_chip_id_mode_disable()
#define read_neo2_cache(a,b,c)                neo2_chip_id_read(a,b+NEO2_CACHE_OFFS,c)
#define write_neo2_cache(a,b,c)               neo2_chip_id_write(a,b+NEO2_CACHE_OFFS,c)

#define open_neo2_rtc()                        neo2_asic_chip_id_mode_enable()
#define close_neo2_rtc()                      neo2_asic_chip_id_mode_disable()

////////////////////////////////////////////////////////////////////////////////

u8 get_neo2_rtc_secs()   {
   u8 secs;
   secs = neo2_chip_id_read_byte(NEO2_RTC_1_SEC_4BIT_OFFS)&15;
   secs += (neo2_chip_id_read_byte(NEO2_RTC_10_SEC_3BIT_OFFS)&7)*10;
   return secs;
}

u8 get_neo2_rtc_mins()   {
   u8 mins;
   mins = neo2_chip_id_read_byte(NEO2_RTC_1_MIN_4BIT_OFFS)&15;
   mins += (neo2_chip_id_read_byte(NEO2_RTC_10_MIN_3BIT_OFFS)&7)*10;
   return mins;
}

u8 get_neo2_rtc_hours()   {
   return neo2_chip_id_read_byte( NEO2_RTC_HOUR_5BIT_OFFS ) & 31;   
}

u16 get_neo2_rtc_days()   {
   return   
      ( ( ( neo2_chip_id_read_byte( NEO2_RTC_DAY_LO_4BIT_OFFS ) & 15 ) | ( neo2_chip_id_read_byte( NEO2_RTC_DAY_MD_4BIT_OFFS ) & 15 ) << 4 ) | ( neo2_chip_id_read_byte( NEO2_RTC_DAY_HI_4BIT_OFFS ) & 15 ) << 8 )   ;
}

// days = 0 - 0xFFF

void set_neo2_rtc( u16 days, u8 hours, u8 mins, u8 secs )   {
   set_neo2_asic_cr( 1<<15 );
   // not actually chip id writes, just an SRAM access
   neo2_chip_id_write_byte( (0 << 8) | ((secs%10)&15),0 );   // set 1 second bits
   neo2_chip_id_write_byte( (1 << 8) | ((secs/10)&7),0 );   // set 10 second bits
   neo2_chip_id_write_byte( (2 << 8) | ((mins%10)&15),0 );   // set 1 min bits
   neo2_chip_id_write_byte( (3 << 8) | ((mins/10)&7),0 );   // set 10 min bits
   neo2_chip_id_write_byte( (4 << 8) | ((hours)&31),0 );   // set 10 min bits
   neo2_chip_id_write_byte( (5 << 8) | ((days)&7),0 );   // set day lo bits
   neo2_chip_id_write_byte( (6 << 8) | ((days>>4)&7),0 );   // set day mid bits
   neo2_chip_id_write_byte( (7 << 8) | ((days>>8)&7),0 );   // set day hi bits
   set_neo2_asic_cr( 3 );
}

u8 get_neo2_gtc_mins()   {
   u8 mins;
   mins = neo2_chip_id_read_byte(NEO2_GTC_1_MIN_4BIT_OFFS)&15;
   mins += (neo2_chip_id_read_byte(NEO2_GTC_10_MIN_3BIT_OFFS)&7)*10;
   return mins;
}

u8 get_neo2_gtc_hours()   {
   return neo2_chip_id_read_byte( NEO2_GTC_HOUR_5BIT_OFFS ) & 31;   
}

u16 get_neo2_gtc_days()   {
   return   
      ( ( neo2_chip_id_read_byte( NEO2_GTC_DAY_LO_4BIT_OFFS ) & 15 ) | ( neo2_chip_id_read_byte( NEO2_GTC_DAY_HI_4BIT_OFFS ) & 15 ) << 4 )   ;
}

void set_neo2_gtc( u16 days, u8 hours, u8 mins )   {
   set_neo2_asic_cr( 1<<15 );
   // not actually chip id writes, just an SRAM access
   neo2_chip_id_write_byte( (8 << 8) | ((mins%10)&15),0 );   // set 1 min bits
   neo2_chip_id_write_byte( (9 << 8) | ((mins/10)&7),0 );   // set 10 min bits
   neo2_chip_id_write_byte( (10 << 8) | ((hours)&31),0 );   // set 10 min bits
   neo2_chip_id_write_byte( (11 << 8) | ((days)&7),0 );   // set day lo bits
   neo2_chip_id_write_byte( (12 << 8) | ((days>>8)&7),0 );   // set day hi bits
   set_neo2_asic_cr( 3 );
}
