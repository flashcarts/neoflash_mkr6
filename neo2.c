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

#include "neoFlash.h"


#define MAKER_CODE_ST         0x20
#define MAKER_CODE_INTEL   0x89

#define DEV_CODE_M36DR432A   0xA0               // top
#define DEV_CODE_M36DR432B   0xA1               // bottom   

#define DEV_CODE_RD28F1602C3BD70   0x88C3       // bottom

static const neoFlashMaker_t neoFlashMakerList[] =
{
   {
      MAKER_CODE_ST,
      "ST"
   },
   {
      MAKER_CODE_INTEL,
      "Intel"
   },
   // add additional makers here
   {
      0,
      0
   }
};

static const neoFlashBlockList_t m35dr432aBlockList[] =
{
   {
      63,
      32 * 1024 * 2
   },
   {
      8,
      4 * 1024 * 2
   },
   {
      0,
      0
   }
};

static const neoFlashBlockList_t m35dr432bBlockList[] =
{
   {
      8,
      4 * 1024 * 2
   },
   {
      63,
      32 * 1024 * 2
   },
   {
      0,
      0   
   }
};

static const neoFlashBlockList_t rd28f1602c3bd70BlockList[] =
{
   {
      8,
      4 * 1024 * 2
   },
   {
      31,
      32 * 1024 * 2
   },
   {
      0,
      0
   }
};

static const neoFlashDev_t neoFlashDevList[] =
{
   {
      MAKER_CODE_ST,
      DEV_CODE_M36DR432A,
      "M36DR432A"
   },
/*   {
      MAKER_CODE_ST,
      DEV_CODE_M36DR432B,
      "M36DR432B"
   }, */
   {
      MAKER_CODE_INTEL,
      DEV_CODE_RD28F1602C3BD70,
      "RD28F1602C3BD70"
   },
   // add additional devices here
   {
      0,
      0,
      0
   }
};

static const neoFlashChip_t neoFlashChipList[] =
{
   {
      MAKER_CODE_ST,
      DEV_CODE_M36DR432A,
      m35dr432aBlockList
   },
/*   {
      MAKER_CODE_ST,
      DEV_CODE_M36DR432B,
      m35dr432bBlockList
   }, */
   // add additional chips here
   {
      MAKER_CODE_INTEL,
      DEV_CODE_RD28F1602C3BD70,
      rd28f1602c3bd70BlockList
   },
   {
      0,
      0,
      0
   }
};
////////////////////////////////////////////////////////////////////////////////

void neoFlashReadMakerAndDevCodes( u16 *makerCode, u16 *devCode )
{
   // setup all the **** to read the maker and device codes
   
   // flash and psram write enable
   
   neo2_asic_flash_psram_write_enable();

   // auto select
   
   //m36dr432AutoSelect();
   
   m36dr432CfiQuery();
   
   // read em
   
   *makerCode = *(vu16*)(0x08000000);
   *devCode = *(vu16*)(0x08000002);
   
   // flash and psram write disable

   neo2_asic_flash_psram_write_disable();
   
   /*clearFrameBuf();
   drawFwTitle();
   drawFwToolbar();
   drawMessage("%X %X", *makerCode, *devCode );
   flushFrameBuf();
   _flushFrameBuf();
   while(1);*/
   
   // return
   
}

void neoFlashReadMakerAndDevCodesIntel( u16 *makerCode, u16 *devCode )
{   
   neo2_asic_flash_psram_write_enable();
   
   intel28f1602CfiQuery();
   
   *makerCode = *(vu16*)(0x08000000);
   *devCode = *(vu16*)(0x08000002);
   neo2_asic_flash_psram_write_disable();
   
   clearFrameBuf();
   drawFwTitle();
   drawFwToolbar();
   drawMessage("%X %X", *makerCode, *devCode );
   flushFrameBuf();
   _flushFrameBuf();
   while(1);
   
}



////////////////////////////////////////////////////////////////////////////////

const neoFlashChip_t *neoFlashChip( u16 makerCode, u16 devCode )   
{
   u32 i=0;
   const neoFlashChip_t *chip = neoFlashChipList;
   while( chip.makerCode != 0 && chip.devCode != 0 && chip.blockList != 0 )   
   {
      if( chip.makerCode == makerCode && chip.devCode == devCode )
      {
            return chip;
      }
      i++;
   }
   return 0;
}

const char *neoFlashMakerString( u16 makerCode )   
{
   u32 i=0;
   const neoFlashMaker_t *maker = neoFlashMakerList;
   while( maker.makerCode != 0 && maker.makerString != 0 )   
   {
      if( maker.makerCode == makerCode )
      {
            return maker.makerString;
      }
      i++;
   }
   return 0;
}

const char *neoFlashDevString( u16 makerCode, u16 devCode )   
{
   u32 i=0;
   const neoFlashDev_t *dev = neoFlashDevList;
   while( dev.makerCode != 0 && dev.devCode != 0 && dev.devString )   
   {
      if( dev.makerCode == makerCode && dev.devCode == devCode )
      {
            return dev.devString;
      }
      i++;
   }
   return 0;
}

unsigned int neoFlashBlockSize( neoFlashChip_t *chip )   
{
   
   if( !chip )
   {
      return 0;
   }
   
   u32 i=0;
   u32 blockSize=0;
   neoFlashBlockList_t *blockList = chip->blockList;
   
   while( blockList.numBlocks != 0 && blockList.blockSize != 0 )   
   {
      if( blockList.blockSize > blockSize )
      {
         blockSize = blockList.blockSize;
      }
      i++;
   }
   
   return blockSize;
   
}

unsigned int neoFlashSize( neoFlashChip_t *chip )   
{
   
   if( !chip )
   {
      return 0;
   }
   
   u32 i=0;
   u32 size=0;
   neoFlashBlockList_t *blockList = chip->blockList;
   
   while( blockList.numBlocks != 0 && blockList.blockSize != 0 )   
   {
      size += blockList.numBlocks * blockList.blockSize;
      i++;
   }
   
   return size;
   
}


////////////////////////////////////////////////////////////////////////////////

bool neoFlashOpen( neoFlashHandle_t *handle )   
{
      
   u16 makerCode;
   u16 devCode;
   
   const neoFlashChip_t *chip;
   
   neoFlashReadMakerAndDevCodes( &makerCode, &devCode );
   
   chip = &neoFlashChipList[0]; //neoFlashChip( makerCode, devCode );
   
   if( chip )   
   {
      handle->chip = chip;
      handle->makerString = neoFlashMakerString( makerCode );
      handle->devString = neoFlashDevString( makerCode, devCode );
      handle->blockSize = neoFlashBlockSize( chip );
      handle->size = neoFlashSize( chip );
      return (bool)chip;
   }
   
   /*neoFlashReadMakerAndDevCodesIntel( &makerCode, &devCode );
   
   chip = neoFlashChip( makerCode, devCode );
   
   if( chip )   
   {
      handle->chip = chip;
      handle->makerString = neoFlashMakerString( makerCode );
      handle->devString = neoFlashDevString( makerCode, devCode );
      handle->blockSize = neoFlashBlockSize( chip );
      handle->size = neoFlashSize( chip );
      return (bool)chip;
   }*/
   
   
   
   return 0;
      
}

bool neoFlashRead( neoFlashHandle_t *handle, u8 *buf, u32 addr, u32 size )
{
   if( handle->chip->makerCode == MAKER_CODE_ST )
   {
      m36dr432ReadResetMemoryArray();
   }
   else if( handle->chip->makerCode == MAKER_CODE_INTEL )
   {
      intel28f1602ReadArray();
   }
   if( handle )   {
      while( size )   {
         *buf=*(vu8*)(0x08000000+addr);
         addr++;
         size--;
      }
      return true;
   }
   return false;
}

bool neoFlashProgram( neoFlashHandle_t *handle, u32 addr, const void *buf, u32 size )   
{
   const u16 *wp=buf;
   u32 i;
   
   if( handle )   {
      
      neo2_asic_flash_psram_write_enable();
      if( handle->chip->makerCode == MAKER_CODE_ST )
      {
         m36dr432BlockUnprotect( addr );
         m32dr432BlockErase( addr );
         m36dr432EnterBypassMode();
         while( size )   {
            m36dr432ProgramInBypassMode( addr, *wp );
            size-=2;
            addr+=2;
            wp++;
         }
         m36dr432ExitBypassMode();
         m36dr432ReadResetMemoryArray();
      }
      else if( handle->chip->makerCode == MAKER_CODE_INTEL )
      {
         intel28f1602BlockUnlock( addr );
         intel28f1602BlockErase( addr );
         while( size )
         {
            intel28f1602WordProgram( addr, *wp );
            size+=2;
            addr+=2;
            wp++;
         }
         intel28f1602ReadArray();
      }
      neo2_asic_flash_psram_write_disable();
      
      return true;   
   }
   return false;
}

void neoFlashClose( neoFlashHandle_t *handle )
{
   return;
}
