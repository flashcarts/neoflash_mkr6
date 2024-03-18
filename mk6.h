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

#define MK6_ASM


void mk6_tf_data_read_512bytes( u8 *buf );

void mk6_spi_command( const u8 *cmd, u32 len );
void mk6_card_command( const u8 *cmd, u32 cmdLen, u8 *out, u32 outLen, u8 *in, u32 inLen );

void mk6_disable();
void mk6_enable();

void mk6_init( int encrypted );

void mk6_nor_write_half( u32 addr, u16 data );
u16 mk6_nor_read_half( u32 addr );

#define MK6_LOCKING_OP_LOCK            0x01
#define MK6_LOCKING_OP_UNLOCK         0xD0
#define MK6_LOCKING_OP_LOCKDOWN      0x2F

void mk6_nor_read_array( u32 addr );

void mk6_nor_read_config( u32 addr );

void mk6_nor_locking_op( u32 addr, u32 op );

void mk6_nor_erase( u32 addr );

void mk6_nor_program_word( u32 addr, u32 data );

//static void db( const char *s );

void mk6_nor_program( u32 addr, u16 *buf, u32 size );

////////////////////////////////////////////////////////////////////////////////

void mk6_sram_write_half( u32 addr, u16 data );
u16 mk6_sram_read_half( u32 addr );

////////////////////////////////////////////////////////////////////////////////

void mk6_tf_cmd_write_byte( u8 data );
void mk6_tf_cmd_write( const u8 *buf, u32 len );

u8 mk6_tf_cmd_read_bits( u8 bits );
u8 mk6_tf_cmd_read_byte();

void mk6_tf_cmd_read( u8 *buf, u32 len );

void mk6_tf_data_write_byte( u8 data );

void mk6_tf_data_write( const u8 *buf, u32 len );

u8 mk6_tf_data_read_byte();

void mk6_tf_data_read( u8 *buf, u32 len );

u8 mk6_tf_data_read_4bits();
