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

#pragma once

typedef struct neoFlashMaker_t
{
   u16 makerCode;
   const char *makerString;
}   neoFlashMaker_t;

typedef struct neoFlashDev_t
{
   u16 makerCode;
   u16 devCode;
   const char *devString;
} neoFlashDev_t;         

typedef struct neoFlashBlockList_t
{
   u32 numBlocks;
   u32 blockSize;
} neoFlashBlockList_t;

typedef struct neoFlashChip_t
{
   u16 makerCode;
   u16 devCode;
   const neoFlashBlockList_t *blockList;
} neoFlashChip_t;

typedef struct neoFlashHandle_t {
   const neoFlashChip_t *chip;
   const char *makerString;
   const char *devString;
   unsigned int blockSize;   
   unsigned int size;   
} neoFlashHandle_t;
