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
#include "eeprom.h"

#define MK2_SPI_FREQ 0

void flash_rd_2m(u32 addr,u8 *buf,u32 size)   {
   
   spi_open(MK2_SPI_FREQ);
   
   spi_send(0x03);
   spi_send( (addr>>16)&0xFF);
   spi_send( (addr>>8)&0xFF);
   spi_send( addr&0xFF );
   
   while(size)   {
      *buf=spi_recv();
      buf++;
      size--;
   }

   spi_close();
   
}

void flash_wr_2m(u32 addr,u8 *buf,u32 size)   {

   u32 addr_last=addr+size;
   u32 i;

   while(addr < addr_last)   {
      
      eeprom_wr_en(1);
      
      spi_open(MK2_SPI_FREQ);
      
      spi_send(0x02);
      spi_send( (addr>>16)&0xFF);
      spi_send( (addr>>8)&0xFF);
      spi_send( addr&0xFF );
      
      for(i=0; (addr<addr_last) && (i<256) ; i++, addr++)   {
         spi_send(*buf);
         buf++;
      }
      
      spi_close();
      
      eeprom_wait();
      
   }
   
   //eeprom_wr_en(0);   // not needed because wr_en latch is reset after each write
   
   spi_close();

}

void flash_erase_sector(u32 addr)
{
   eeprom_wr_en(1);
   spi_open(3);
   spi_send(0xD8);
   spi_send( (addr>>16)&0xFF);
   spi_send( (addr>>8)&0xFF);
   spi_send( addr&0xFF );
   spi_close();
   eeprom_wait();
}

void flash_erase_2m(void)   {
   
   eeprom_wr_en(1);
   
   // erase command
   spi_open(3);
   spi_send(0xC7);
   spi_close();
   
   eeprom_wait();
   
   //eeprom_wr_en(0);   // is this needed (who cares -_-) not needed (checked the data sheet)!
   
}
