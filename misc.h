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

#ifndef __MISC_H
#define __MISC_H

typedef struct {
   char game_title[12];
   char game_code[4];
   char maker_code[2];
   u8 unit_code;
   u8 device_code;
   u8 card_size;

   u8 reserved0[9];

   u8 rom_version;
   u8 boot_info;

   u32 arm9_src;
   u32 arm9_entry;
   u32 arm9_dst;
   u32 arm9_size;

   u32 arm7_src;
   u32 arm7_entry;
   u32 arm7_dst;
   u32 arm7_size;

   u32 fnt_src;
   u32 fnt_size;

   u32 fat_src;
   u32 fat_size;

   u32 arm9_overlay_src;
   u32 arm9_overlay_size;
   u32 arm7_overlay_src;
   u32 arm7_overlay_size;

   u32 rom_control0;
   u32 rom_control1;
   u32 banner_src;
   u16 secure_crc16;
   u16 rom_control2;

   u32 arm9_autoload;
   u32 arm7_autoload;

   u32 reserved1[2];

   u32 rom_size;
   u32 header_size;

   u8 reserved2[56];

   u8 logo[156];

   u16 logo_crc16;
   u16 header_crc16;
} NDS_HEADER;

typedef struct {
   struct {
      u32 CardID;               //0x00  (0x00000FC2)
      u32 SecureCardID;         //0x04  (0x00000FC2 [Card ID in encrypted mode?])
      u16 ROMHeader_CRC16;      //0x08
      u16 SecureAreaCRC16;      //0x0A
      u16 ROMHeader_Invalid;      //0x0C  (1 if ROM header is invalid [Bad header crc16 or logo crc16])
      u16 Unk_E;               //0x0E  (1 if decrypting Secure Area failed?)
      u16 Unk_10;               //0x10  (0x0000)
      u16 Unk_12;               //0x12  (1 if decrypting Secure Area failed?)
      u16 Unk_14;               //0x14  (1 if decrypting Secure Area failed?)
      u16 RTC_Reset;            //0x16  (1 if RTC was reset)
      u8 RTC_data[8];            //0x18  (Indicates when system started up)
   } bootCheckInfo;
   u32 Unk_20;                  //0x20  (pointer somewhere in main RAM)
   u32 Unk_24;                  //0x24  (pointer somewhere in WRAM)
   u16 Unk_28;                  //0x28  (0x0000)
   u16 Unk_2A;                  //0x2A  (0x0000)
   u16 Unk_2C;                  //0x2C  (0x0000)
   u16 Unk_2E;                  //0x2E  (0x0000)
   u8 FW_Header[32];            //0x30  (Firmware header)   // u16 Unk_44;  //0x44
   u16 ARM7BIOS_CRC16;            //0x50  (0x5835)
} RED;


#endif
