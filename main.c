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

#include "Video.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

//#include "neo2_spi_mmc_drv_bin.h"
#include "mk6_tf_drv_bin.h"

#include "bnr.h"

#include "neoFlash.h"

#include "config_settings.h"

u32 ndsImageOffset;

#define BROWSER_FILE_MAX   256

unsigned int m_numFiles;
char m_fileName[BROWSER_FILE_MAX][256];
u8 *m_banner[BROWSER_FILE_MAX];
char m_fileType[BROWSER_FILE_MAX];
char m_path[1024];

int m_state=0;

char m_menuFlashFileName[256]="Hello World";

#include "misc.h"
#include "card.h"

#define NEO2_FLASH_BUF_SIZE   (64*1024)

// keeping this smaller will make cancelling more responsive
// (does making it bigger help dumping speed at all? i dont think
// much)
#define NEO2_DUMP_BUF_SIZE 0x8000 //(1024*1024/8)

// bad globals!
// used to read data from the NDS card in to it before writing
// to SD/MMC

u8 flash_read_buffer[NEO2_FLASH_BUF_SIZE] __attribute__((aligned(4)));

u8 read_buffer[NEO2_DUMP_BUF_SIZE]  __attribute__((aligned(4)));

#ifdef NEO_VERIFY

// used to read back data into for verifying
u8 verify_buffer[NEO2_DUMP_BUF_SIZE]  __attribute__((aligned(4)));

#endif

static u8 banner[2112];

void drawFwTitle();
void drawFwToolbar();

void *allocMem( unsigned int size );
void freeMem( void *p );

void neoStart();
void neoExit();

void startLoader();
void startLoaderDriver( u32 user1, u32 user2, u32 user3, u32 user4, u32 userPtr, const void *driverBin, u32 driverBinSize );

void OBJSize( u16 *obj, u32 size );

int menuFlashProcess02( unsigned int msg, unsigned int arg )   {

   static NDS_HEADER nds_header __attribute__((aligned(4)));
   static FAT_FILE *f;            // file handle
   static u32 i,j;
   static u32 len;
   static bool dumpDone;
   static int pcentLast = -1;
   static u32 addr;
   //u8 buf[512];

   if( msg == 1 )   {

      f=FAT_fopen( m_menuFlashFileName, "rb" );
      if((int)f==-1)   {
         return 0;
      }

      len = FAT_GetFileSize();

      clearFrameBuf();
      drawFwTitle();
      drawFwToolbar();
      drawMessage( "Flashing %s...", m_menuFlashFileName );
      //drawButtonB("B: Cancel");
      flushFrameBuf();
      dumpDone=false;
      i=0;

      return 1;

   }

   if( msg == 2 && arg == KEY_A && dumpDone == true )   {

      set_neo2_asic_cr( 1<<5 );
      neoExit();

   //   *(vu32*)(0x0237FFF8) = 0xDEADBEEF;
   /*__asm("mov   r0, #0");
   __asm("ldr   r1, =0x00002078");
   __asm("mcr   p15, 0, r1, c1, c0, 0");
   __asm("mcr   p15, 0, r0, c7, c5, 0");
   __asm("mcr   p15, 0, r0, c7, c6, 0");
   __asm("mcr   p15, 0, r0, c7, c10, 4");
   __asm("ldr   r1, =0x0080000A");
   __asm("mcr   p15, 0, r1, c9, c1, 0");
   __asm("mcr   p15, 0, r0, c6, c0, 0");
   __asm("mcr   p15, 0, r0, c6, c1, 0");
   __asm("mcr   p15, 0, r0, c6, c2, 0");
   __asm("mcr   p15, 0, r0, c6, c3, 0");
   __asm("mcr   p15, 0, r0, c6, c4, 0");
   __asm("mcr   p15, 0, r0, c6, c5, 0");
   __asm("mcr   p15, 0, r0, c6, c6, 0");
   __asm("mcr   p15, 0, r0, c6, c7, 0");
   __asm("mcr   p15, 0, r0, c3, c0, 0");
   __asm("mcr   p15, 0, r0, c2, c0, 0");
   __asm("mcr   p15, 0, r0, c2, c0, 1");
   __asm("mcr   p15, 0, r0, c5, c0, 3");
   __asm("mcr   p15, 0, r0, c5, c0, 2");
   __asm("ldr   r1, =0x00012078");
   __asm("mcr   p15, 0, r1, c1, c0, 0");

   __asm("mrs     r0, cpsr");
   __asm("orr     r1, r0, #0x80");
   __asm("msr     cpsr_c, r1");
   __asm("and     r0, r0, #0x80");*/

   __asm("ldr      r0, =0xE51FF004");
   __asm("ldr      r1, =0x027FFE20");
   __asm("str      r0, [ r1 ]");
   __asm("str      r1, [ r1, #4 ]");
   __asm("bx      r1");


      while(1);
      exitProcess();
      return 1;
   }

   // cancelling flashing is probably never going to be a good idea -_-

   /*if( msg == 2 && arg == KEY_B && dumpDone == false )   {
      FAT_fclose(f);
      FAT_remove ( filename );

      dumpDone = true;
      clearFrameBuf();
      drawFwTitle();
      drawFwToolbar();
      drawMessage( "Dumping %s cancelled",filename );
      drawButtonA("A: OK");

      u16 str[512];

      setFrameBufCol( 1 );
      setLineThickness( 2 );
      drawRect( 4, (SCREEN_HEIGHT/2)-24, SCREEN_WIDTH-4, (SCREEN_HEIGHT/2)+24);

      setFontAnchor( 0, 0 );

      bnr_game_str_ex((u16*)str,banner);
      _drawFontStringW( (SCREEN_WIDTH/2) + 12 ,(SCREEN_HEIGHT/2)-8, str );
      bnr_maker_str_ex((u16*)str,banner);
      _drawFontStringW( (SCREEN_WIDTH/2) + 12 ,(SCREEN_HEIGHT/2)+8, str );

         mem_cpy16( &SPRITE_PALETTE[0], (u16*)&banner[544], 32);
         mem_cpy16( &SPRITE_GFX[0], (u16*)&banner[32], 512 );
         OBJClear( &OAM[ 0 ] );
         OBJSize( &OAM[ 0 ], OBJ_SIZE_32_32 );
         OBJPos( &OAM[ 0 ], 8, (SCREEN_HEIGHT/2)-16 );
         OBJPal( &OAM[ 0 ], 0 );
         OBJTile( &OAM[ 0 ], 0 );



         flushFrameBuf();

      return 1;
   }*/

   if( msg == 0 && !dumpDone )   {

      if( i < len )   {


         u32 pcent = i/(len/100);
         if( pcent != pcentLast )   {

            swiWaitForVBlank();
            clearFrameBufEx( FRAME_BUF_WIDTH/2-52, FRAME_BUF_HEIGHT/4-12-16+32+16, FRAME_BUF_WIDTH/2+52, FRAME_BUF_HEIGHT/4+12+32+16);
            setFrameBufCol( 2 );
            drawRectFill( FRAME_BUF_WIDTH/2-50, FRAME_BUF_HEIGHT/4-12+32+16, FRAME_BUF_WIDTH/2-50+pcent, FRAME_BUF_HEIGHT/4+12+32+16);
            setFrameBufCol( 1 );
            drawRect( FRAME_BUF_WIDTH/2-52, FRAME_BUF_HEIGHT/4-12+32+16, FRAME_BUF_WIDTH/2+52, FRAME_BUF_HEIGHT/4+12+32+16);
            setFontAnchor(0,0);
            drawFontString( FRAME_BUF_WIDTH/2, FRAME_BUF_HEIGHT/4+32+16, "%u%%", pcent );
            resetFontAnchor();

            // draw banner here!

            u16 str[512];

            setFrameBufCol( 1 );
            setLineThickness( 2 );
            drawRect( 4, (SCREEN_HEIGHT/2)-24, SCREEN_WIDTH-4, (SCREEN_HEIGHT/2)+24);

            setFontAnchor( 0, 0 );

            bnr_game_str_ex((u16*)str,banner);
            _drawFontStringW( (SCREEN_WIDTH/2) + 12 ,(SCREEN_HEIGHT/2)-8, str );
            bnr_maker_str_ex((u16*)str,banner);
            _drawFontStringW( (SCREEN_WIDTH/2) + 12 ,(SCREEN_HEIGHT/2)+8, str );

            mem_cpy16( &SPRITE_PALETTE[0], (u16*)&banner[544], 32);
            mem_cpy16( &SPRITE_GFX[0], (u16*)&banner[32], 512 );
            OBJClear( &OAM[ 0 ] );
            OBJSize( &OAM[ 0 ], OBJ_SIZE_32_32 );
            OBJPos( &OAM[ 0 ], 8, (SCREEN_HEIGHT/2)-16 );
            OBJPal( &OAM[ 0 ], 0 );
            OBJTile( &OAM[ 0 ], 0 );



            flushFrameBuf();
         }
         // calculate the size of the next block to read, either NEO2_DUMP_BUF_SIZE
         // or the number of bytes remaining to dump, which ever is smaller
         j = (len - i);
         j = ((j < NEO2_FLASH_BUF_SIZE) ? j : NEO2_FLASH_BUF_SIZE);


         FAT_fread(flash_read_buffer,1,j,f);

            neoFlashHandle_t handle;

            set_neo2_asic_cr( 1<<13 );

            if( neoFlashOpen( &handle ) )   {
               neoFlashProgram( &handle, i+0x40000, flash_read_buffer, j );
            } else {
               clearFrameBuf();
               drawFwTitle();
               drawFwToolbar();
               drawMessage("Flash Error!");
               flushFrameBuf();
               _flushFrameBuf();
               while(1);
            }

            set_neo2_asic_cr( 1<<7 | 1<<10 );

#ifdef NEO_VERIFY

         /*FAT_fseek( f, i, SEEK_SET );
         if( j!=FAT_fread(verify_buffer,1,j,f))   {
            FAT_fclose(f);
            FAT_remove (filename);
            return 0;
         }

         if( memcmp( read_buffer, verify_buffer, j ) )   {
            clearFrameBuf();
            drawFwTitle();
            drawFwToolbar();
            drawMessage("Dump Verify Error!");
            flushFrameBuf();
            _flushFrameBuf();
            while(1);
         }*/

#endif

         i += NEO2_FLASH_BUF_SIZE;
         pcentLast = pcent;
      } else {
         FAT_fclose(f);
         dumpDone = true;
         clearFrameBuf();
         drawFwTitle();
         drawFwToolbar();
         drawMessage( "%s flashed", m_menuFlashFileName );
         drawButtonA("A: OK");
            u16 str[512];

      setFrameBufCol( 1 );
      setLineThickness( 2 );
      drawRect( 4, (SCREEN_HEIGHT/2)-24, SCREEN_WIDTH-4, (SCREEN_HEIGHT/2)+24);

      setFontAnchor( 0, 0 );

      bnr_game_str_ex((u16*)str,banner);
      _drawFontStringW( (SCREEN_WIDTH/2) + 12 ,(SCREEN_HEIGHT/2)-8, str );
      bnr_maker_str_ex((u16*)str,banner);
      _drawFontStringW( (SCREEN_WIDTH/2) + 12 ,(SCREEN_HEIGHT/2)+8, str );

         mem_cpy16( &SPRITE_PALETTE[0], (u16*)&banner[544], 32);
            mem_cpy16( &SPRITE_GFX[0], (u16*)&banner[32], 512 );
            OBJClear( &OAM[ 0 ] );
            OBJSize( &OAM[ 0 ], OBJ_SIZE_32_32 );
            OBJPos( &OAM[ 0 ], 8, (SCREEN_HEIGHT/2)-16 );
            OBJPal( &OAM[ 0 ], 0 );
            OBJTile( &OAM[ 0 ], 0 );

         flushFrameBuf();
      }
      return 1;
   }

   return 0;
}

int menuFlashProcess01( unsigned int msg, unsigned int arg )   {

   NDS_HEADER nds_header __attribute__((aligned(4)));
   int i;
   static bool cardReady;
   static FAT_FILE *f;

   if( msg == 1 )   {

            for(i=0;i<128;i++)   {
               OAM[i*4]      = ATTR0_DISABLED;
               OAM_SUB[i*4]      = ATTR0_DISABLED;
            }
         cardReady = false;

         /*if( cardReady )   {
            switchProcess( menuFlashProcess02, 0 );
            return 1;
         }

         clearFrameBuf();
         drawFwTitle();
         drawFwToolbar();
         drawMessage("Please wait...");
         flushFrameBuf();*/
         // artificial wait...
         /*for(i=0;i<10;i++)
            swiWaitForVBlank();
         // force wait control register incase its not set properly elsewhere

         WAIT_CR &= ~0x8880;

         // initialise the NDS card and read in the header
         if (card_init() == 0){
            cardReady=false;
            clearFrameBuf();
            drawFwTitle();
            drawFwToolbar();
            drawMessage( "NDS Card Error" );
            drawButtonA("A: Retry");
            drawButtonB("B: Cancel");
            return 1;
         }*/

         //f = FAT_fopen( m_menuFlashFileName, "rb" );
         if( 1) { //f )   {

         //FAT_fread( &nds_header, sizeof( NDS_HEADER ), 1, f );      // read in this nds header

            // do a sanity check, if this passes there is a reasonable chance this is a nds image

            //if ((nds_header.arm9_dst >= 0x02000000) && (nds_header.arm9_dst < 0x02800000) && (nds_header.arm7_dst >= 0x02000000) && (nds_header.arm7_dst < 0x02800000)) {
               /*if(nds_header.banner_src) {
                     FAT_fseek(f,nds_header.banner_src,SEEK_SET);
                     FAT_fread(banner,2112,1,f);
                     FAT_fclose(f);
               } else {
                     FAT_fclose(f);
                     menuFlashProcess01( 1, 0 );
                     return 1;
               }*/
            //} else {
            //   FAT_fclose(f);
            //   menuFlashProcess01( 1, 0 );
            //   return 1;
            //}

            cardReady = true;
            clearFrameBuf();
            drawFwTitle();
            drawFwToolbar();
            drawMessage( "Flash %s", m_menuFlashFileName );
            drawButtonA("A: OK");
            drawButtonB("B: Cancel");

               // draw banner here!

            u16 str[512];

            setFrameBufCol( 1 );
            setLineThickness( 2 );
            drawRect( 4, (SCREEN_HEIGHT/2)-24, SCREEN_WIDTH-4, (SCREEN_HEIGHT/2)+24);

            setFontAnchor( 0, 0 );

            bnr_game_str_ex((u16*)str,banner);
            _drawFontStringW( (SCREEN_WIDTH/2) + 12 ,(SCREEN_HEIGHT/2)-8, str );
            bnr_maker_str_ex((u16*)str,banner);
            _drawFontStringW( (SCREEN_WIDTH/2) + 12 ,(SCREEN_HEIGHT/2)+8, str );

            flushFrameBuf();

            mem_cpy16( &SPRITE_PALETTE[0], (u16*)&banner[544], 32);
            mem_cpy16( &SPRITE_GFX[0], (u16*)&banner[32], 512 );
            OBJClear( &OAM[ 0 ] );
            OBJSize( &OAM[ 0 ], OBJ_SIZE_32_32 );
            OBJPos( &OAM[ 0 ], 8, (SCREEN_HEIGHT/2)-16 );
            OBJPal( &OAM[ 0 ], 0 );
            OBJTile( &OAM[ 0 ], 0 );

            return 1;

         }
         else   {
            menuFlashProcess01( 1, 0 );
         }
         return 1;
   }

   if( msg == 2 && arg == KEY_B )   {
      exitProcess();
      return 1;
   }

   if( msg == 2 && arg == KEY_A )   {
      switchProcess( menuFlashProcess02, 0 );
      return 1;
   }



   return 0;
}


int dumperProcess02( unsigned int msg, unsigned int arg )   {

   static NDS_HEADER nds_header __attribute__((aligned(4)));
   static char filename[256];
   static FAT_FILE *f;            // file handle
   static u32 i,j;
   static u32 len;
   static bool dumpDone;
   static int pcentLast = -1;
   //u8 buf[512];

   if( msg == 1 )   {
      card_read(0,(u8*)&nds_header,sizeof(NDS_HEADER));
      sprintf( filename, "%.4s%.2s.NDS", nds_header.game_code, nds_header.maker_code );
      clearFrameBuf();
      drawFwTitle();
      drawFwToolbar();
      drawMessage( "Dumping %s...",filename );
      drawButtonB("B: Cancel");
      flushFrameBuf();
      dumpDone=false;
      i=0;
      len=((nds_header.rom_size+0x03FF)&~0x01FF);
      if( FAT_FileExists( filename ) != FT_NONE )
         FAT_remove( filename );

      f=FAT_fopen(filename,"w+");
      if((int)f==-1)   {
         return 0;
      }
      // --TODO-- handle error
      return 1;
   }

   if( msg == 2 && arg == KEY_A && dumpDone == true )   {
      exitProcess();
      return 1;
   }

   if( msg == 2 && arg == KEY_B && dumpDone == false )   {
      FAT_fclose(f);
      FAT_remove ( filename );

      dumpDone = true;
      clearFrameBuf();
      drawFwTitle();
      drawFwToolbar();
      drawMessage( "Dumping %s cancelled",filename );
      drawButtonA("A: OK");

      u16 str[512];

      setFrameBufCol( 1 );
      setLineThickness( 2 );
      drawRect( 4, (SCREEN_HEIGHT/2)-24, SCREEN_WIDTH-4, (SCREEN_HEIGHT/2)+24);

      setFontAnchor( 0, 0 );

      bnr_game_str_ex((u16*)str,banner);
      _drawFontStringW( (SCREEN_WIDTH/2) + 12 ,(SCREEN_HEIGHT/2)-8, str );
      bnr_maker_str_ex((u16*)str,banner);
      _drawFontStringW( (SCREEN_WIDTH/2) + 12 ,(SCREEN_HEIGHT/2)+8, str );

         mem_cpy16( &SPRITE_PALETTE[0], (u16*)&banner[544], 32);
            mem_cpy16( &SPRITE_GFX[0], (u16*)&banner[32], 512 );
            OBJClear( &OAM[ 0 ] );
            OBJSize( &OAM[ 0 ], OBJ_SIZE_32_32 );
            OBJPos( &OAM[ 0 ], 8, (SCREEN_HEIGHT/2)-16 );
            OBJPal( &OAM[ 0 ], 0 );
            OBJTile( &OAM[ 0 ], 0 );



         flushFrameBuf();

      return 1;
   }

   if( msg == 0 && !dumpDone )   {

      if( i < len )   {


         u32 pcent = i/(len/100);
         if( pcent != pcentLast )   {
            swiWaitForVBlank();
            clearFrameBufEx( FRAME_BUF_WIDTH/2-52, FRAME_BUF_HEIGHT/4-12-16+32+16, FRAME_BUF_WIDTH/2+52, FRAME_BUF_HEIGHT/4+12+32+16);
            setFrameBufCol( 2 );
            drawRectFill( FRAME_BUF_WIDTH/2-50, FRAME_BUF_HEIGHT/4-12+32+16, FRAME_BUF_WIDTH/2-50+pcent, FRAME_BUF_HEIGHT/4+12+32+16);
            setFrameBufCol( 1 );
            drawRect( FRAME_BUF_WIDTH/2-52, FRAME_BUF_HEIGHT/4-12+32+16, FRAME_BUF_WIDTH/2+52, FRAME_BUF_HEIGHT/4+12+32+16);
            setFontAnchor(0,0);
            drawFontString( FRAME_BUF_WIDTH/2, FRAME_BUF_HEIGHT/4+32+16, "%u%%", pcent );
            resetFontAnchor();


               // draw banner here!

            u16 str[512];

            setFrameBufCol( 1 );
            setLineThickness( 2 );
            drawRect( 4, (SCREEN_HEIGHT/2)-24, SCREEN_WIDTH-4, (SCREEN_HEIGHT/2)+24);

            setFontAnchor( 0, 0 );

            bnr_game_str_ex((u16*)str,banner);
            _drawFontStringW( (SCREEN_WIDTH/2) + 12 ,(SCREEN_HEIGHT/2)-8, str );
            bnr_maker_str_ex((u16*)str,banner);
            _drawFontStringW( (SCREEN_WIDTH/2) + 12 ,(SCREEN_HEIGHT/2)+8, str );

            mem_cpy16( &SPRITE_PALETTE[0], (u16*)&banner[544], 32);
            mem_cpy16( &SPRITE_GFX[0], (u16*)&banner[32], 512 );
            OBJClear( &OAM[ 0 ] );
            OBJSize( &OAM[ 0 ], OBJ_SIZE_32_32 );
            OBJPos( &OAM[ 0 ], 8, (SCREEN_HEIGHT/2)-16 );
            OBJPal( &OAM[ 0 ], 0 );
            OBJTile( &OAM[ 0 ], 0 );



            flushFrameBuf();
         }
         // calculate the size of the next block to read, either NEO2_DUMP_BUF_SIZE
         // or the number of bytes remaining to dump, which ever is smaller
         j = (len - i);
         j = ((j < NEO2_DUMP_BUF_SIZE) ? j : NEO2_DUMP_BUF_SIZE);

         // read the next block, --TODO-- consider some error checking here?
         card_read(i,read_buffer,j);

         // write the block to the file, if the write fails for any reason
         // close and delete the file we are dumping to and return zero so
         // the caller knows the dump failed
         if (j!=FAT_fwrite(read_buffer,1,j,f)) {
            FAT_fclose(f);
            FAT_remove (filename);
            clearFrameBuf();
            drawFwTitle();
            drawFwToolbar();
            drawMessage("Write Error!");
            flushFrameBuf();
            _flushFrameBuf();
            return 0;
         }

#ifdef NEO_VERIFY

         FAT_fseek( f, i, SEEK_SET );
         if( j!=FAT_fread(verify_buffer,1,j,f))   {
            FAT_fclose(f);
            FAT_remove (filename);
            return 0;
         }

         if( memcmp( read_buffer, verify_buffer, j ) )   {
            clearFrameBuf();
            drawFwTitle();
            drawFwToolbar();
            drawMessage("Dump Verify Error!");
            flushFrameBuf();
            _flushFrameBuf();
            while(1);
         }

#endif

         i += NEO2_DUMP_BUF_SIZE;
         pcentLast = pcent;

      } else {

         FAT_fclose(f);
         dumpDone = true;
         clearFrameBuf();
         drawFwTitle();
         drawFwToolbar();
         drawMessage( "%s dumped", filename );
         drawButtonA("A: OK");
            u16 str[512];

      setFrameBufCol( 1 );
      setLineThickness( 2 );
      drawRect( 4, (SCREEN_HEIGHT/2)-24, SCREEN_WIDTH-4, (SCREEN_HEIGHT/2)+24);

      setFontAnchor( 0, 0 );

      bnr_game_str_ex((u16*)str,banner);
      _drawFontStringW( (SCREEN_WIDTH/2) + 12 ,(SCREEN_HEIGHT/2)-8, str );
      bnr_maker_str_ex((u16*)str,banner);
      _drawFontStringW( (SCREEN_WIDTH/2) + 12 ,(SCREEN_HEIGHT/2)+8, str );

         mem_cpy16( &SPRITE_PALETTE[0], (u16*)&banner[544], 32);
            mem_cpy16( &SPRITE_GFX[0], (u16*)&banner[32], 512 );
            OBJClear( &OAM[ 0 ] );
            OBJSize( &OAM[ 0 ], OBJ_SIZE_32_32 );
            OBJPos( &OAM[ 0 ], 8, (SCREEN_HEIGHT/2)-16 );
            OBJPal( &OAM[ 0 ], 0 );
            OBJTile( &OAM[ 0 ], 0 );

         flushFrameBuf();
      }
      return 1;
   }

   return 0;
}

int dumperProcess01( unsigned int msg, unsigned int arg )   {

   NDS_HEADER nds_header __attribute__((aligned(4)));
   int i;
   static bool cardReady;


   if( msg == 1 )   {
         for(i=0;i<128;i++)   {
               OAM[i*4]      = ATTR0_DISABLED;
               OAM_SUB[i*4]      = ATTR0_DISABLED;
            }
         cardReady = false;
         clearFrameBuf();
         drawFwTitle();
         drawFwToolbar();
         drawMessage("Insert NDS card");
         drawButtonA("A: OK");
         drawButtonB("B: Cancel");
         return 1;
   }

   if( msg == 2 && arg == KEY_A )   {

         if( cardReady )   {
            switchProcess( dumperProcess02, 0 );
            return 1;
         }

         clearFrameBuf();
         drawFwTitle();
         drawFwToolbar();
         drawMessage("Please wait...");
         flushFrameBuf();
         // artificial wait...
         for(i=0;i<10;i++)
            swiWaitForVBlank();
         // force wait control register incase its not set properly elsewhere
         WAIT_CR &= ~0x8880;
         // initialise the NDS card and read in the header
         if (card_init() == 0){
            cardReady=false;
            clearFrameBuf();
            drawFwTitle();
            drawFwToolbar();
            drawMessage( "NDS Card Error" );
            drawButtonA("A: Retry");
            drawButtonB("B: Cancel");
            return 1;
         }
         card_read(0,(u8*)&nds_header,sizeof(NDS_HEADER));
         if ((nds_header.arm9_dst >= 0x02000000) && (nds_header.arm9_dst < 0x02800000) && (nds_header.arm7_dst >= 0x02000000) && (nds_header.arm7_dst < 0x02800000)) {
            if(!nds_header.banner_src) {
                  dumperProcess01( 1, 0 );
                  return 1;
            }
            card_read(nds_header.banner_src,banner,2112);

            cardReady=true;
            clearFrameBuf();
            drawFwTitle();
            drawFwToolbar();
            drawMessage( "Dump %.4s%.2s.NDS", nds_header.game_code, nds_header.maker_code );
            drawButtonA("A: OK");
            drawButtonB("B: Cancel");

            // draw banner here!

            u16 str[512];

            setFrameBufCol( 1 );
            setLineThickness( 2 );
            drawRect( 4, (SCREEN_HEIGHT/2)-24, SCREEN_WIDTH-4, (SCREEN_HEIGHT/2)+24);

            setFontAnchor( 0, 0 );

            bnr_game_str_ex((u16*)str,banner);
            _drawFontStringW( (SCREEN_WIDTH/2) + 12 ,(SCREEN_HEIGHT/2)-8, str );
            bnr_maker_str_ex((u16*)str,banner);
            _drawFontStringW( (SCREEN_WIDTH/2) + 12 ,(SCREEN_HEIGHT/2)+8, str );

            flushFrameBuf();

            mem_cpy16( &SPRITE_PALETTE[0], (u16*)&banner[544], 32);
            mem_cpy16( &SPRITE_GFX[0], (u16*)&banner[32], 512 );
            OBJClear( &OAM[ 0 ] );
            OBJSize( &OAM[ 0 ], OBJ_SIZE_32_32 );
            OBJPos( &OAM[ 0 ], 8, (SCREEN_HEIGHT/2)-16 );
            OBJPal( &OAM[ 0 ], 0 );
            OBJTile( &OAM[ 0 ], 0 );

            return 1;
         }
         else   {
            dumperProcess01( 1, 0 );
         }
         return 1;
   }

   if( msg == 2 && arg == KEY_B )   {
      exitProcess();
      return 1;
   }



   return 0;
}

void refreshBrowserDir()   {

//   REG_IME=0;

   m_numFiles=0;
   int type;
   char lfn[256];
   int i;
   FAT_FILE *f;
   NDS_HEADER header;

   static bool firstTime=true;                  // this is used to set all banner pointers to NULL only the first time this is called

   if( firstTime == true )   {                     // if it is the first time
      for(i=0;i<BROWSER_FILE_MAX;i++)   {      // set all banners to NULL
         m_banner=0;
      }
      firstTime = false;                           // set first time flag to false so it is never called again
   } else {                                             // if this is not the first time
      for(i=0;i<BROWSER_FILE_MAX;i++)   {      // free any used banners and set all to NULL
         if( m_banner )
            freeMem( m_banner );
         m_banner=0;
      }
   }

   // next get the file or dirnames and their type (either dir or file)

   type = FAT_FindFirstFileLFN(lfn);
   while (type != FT_NONE)   {
      m_fileType[ m_numFiles ] = type;
      strcpy( m_fileName[ m_numFiles ], lfn );
      m_numFiles++;
      type = FAT_FindNextFileLFN(lfn);
   }

   for(i=0;i<m_numFiles;i++)   {
      if( m_fileType[ i ] == FT_DIR )   {
         // do nothing else for a dir
      } else if ( m_fileType[ i ] == FT_FILE )   {
         f=FAT_fopen( m_fileName[ i ], "rb" );                           // open the file
         if( f )   {                                                                     // if file opened ok

            memset( &header, 0, sizeof( NDS_HEADER ) );               // clear the header buffer
            FAT_fread( &header, sizeof( NDS_HEADER ), 1, f );      // read in this nds header

            // do a sanity check, if this passes there is a reasonable chance this is a nds image

            if ( ( (header.arm9_dst >= 0x02000000) && (header.arm9_dst < 0x02800000) && (header.arm7_dst >= 0x02000000) && (header.arm7_dst < 0x03810000) ) ) {
               if(header.banner_src) {
                  m_banner = allocMem( 2112 );
                  if( m_banner )   {
                     FAT_fseek(f,header.banner_src,SEEK_SET);
                     FAT_fread(m_banner,2112,1,f);
                  }
               }
               else {
                  m_banner = allocMem( 2112 );
                  if( m_banner )   {
                     memset( m_banner, 0, 2112 );
                  }
               }
            }
            else {
               u32 neo_offset = ((u32*)&header)[3];
               FAT_fseek( f, neo_offset, SEEK_SET );
               FAT_fread( &header, sizeof( NDS_HEADER ), 1, f );      // read in this nds header
               if ( ( (header.arm9_dst >= 0x02000000) && (header.arm9_dst < 0x02800000) && (header.arm7_dst >= 0x02000000) && (header.arm7_dst < 0x03810000) ) ) {
                  if(header.banner_src) {
                     m_banner = allocMem( 2112 );
                     if( m_banner )   {
                        FAT_fseek(f,header.banner_src+neo_offset,SEEK_SET);
                        FAT_fread(m_banner,2112,1,f);
                     }
                  }
               }
            }


            FAT_fclose( f );                                                      // close the file

         }
      }
   }


//   REG_IME=1;


}

bool changeBrowserDir( const char *path )   {
//      REG_IME=0;
      if( FAT_chdir( path ) == false )   {
//         REG_IME=1;
         return false;
      }
      strcpy(m_path,path);
      refreshBrowserDir();
//      REG_IME=1;
      return true;
}

void drawFwTitle()   {
      resetFrameBufClip();
      setFrameBufCol( 1 );
      setFontAnchor( 0, -1 );
      drawFontString( FRAME_BUF_WIDTH/2, 0,"NEO2 Menu V1.5 - %s %s", __DATE__, __TIME__ );
      drawFontString( FRAME_BUF_WIDTH/2, getFontHeight(), "www.neoflash.com" );
      resetFontAnchor();
      drawRectFill( 0, getFontHeight()*2, FRAME_BUF_WIDTH, getFontHeight()*2+2 );
}

void drawFwToolbar()   {
   setFrameBufCol( 1 );
   drawRect( 0, FRAME_BUF_HEIGHT-(getFontHeight()*2)-2, FRAME_BUF_WIDTH, FRAME_BUF_HEIGHT-(getFontHeight()*2) );
}

void drawFwToolbarBrowser()
{
   resetFrameBufClip();
   setFrameBufCol( 1 );
   drawRect( 0, FRAME_BUF_HEIGHT-(getFontHeight()*2)-2, FRAME_BUF_WIDTH, FRAME_BUF_HEIGHT-(getFontHeight()*2) );
   setFrameBufCol( 1 );
   setFontAnchor( 0, -1 );
   drawFontString( FRAME_BUF_WIDTH/2, FRAME_BUF_HEIGHT - (getFontHeight()*2),"A: OK  B: Back" );
   drawFontString( FRAME_BUF_WIDTH/2, FRAME_BUF_HEIGHT - getFontHeight(), "X: Dump NDS card" );
   resetFontAnchor();
}


void drawFwFatal(char *str)   {
   clearFrameBuf();
   drawFwTitle();
   drawFwToolbar();
   drawMessage(str);
   flushFrameBuf();
   _flushFrameBuf();
   while(1);
}





extern int filesysSecPerClus, filesysData;

u32 FAT_NextCluster(u32 cluster);
#define   CLUSTER_EOF   0x0FFFFFFF
#define CLUSTER_FREE   0x0000

int FileFragmented() {
   int clust;
   int next_clust;

   if ((clust = FAT_GetFileCluster()) == CLUSTER_FREE) return 0; // No file data, no fragmentation! :)
   while ((next_clust = FAT_NextCluster(clust)) != CLUSTER_EOF) {
      if ((++clust) != next_clust) return 1;
   }

   return 0;
}


u8 save_buffer[256*1024];

int browserProcess( unsigned int msg, unsigned int arg )   {

   static int soff;
   static int sidx;
   char buf[512];
   char filename[256];
   NDS_HEADER *nds_header;

   NDS_HEADER nds_vheader;
   FAT_FILE *f;

   u32 i;
   u32 size;

   int x,y;

   u32 pcent;

   if( msg == 1 )   {
      if( changeBrowserDir( "/" ) == false )
         return 0;
      soff=0;
      sidx=0;
      browserProcess( 0x1000, 0 );
      return 1;
   }

   if( msg == 2 && arg == KEY_A )   {
      if( sidx < 0 || sidx > 255 )
         ; // do nothing
      else if( sidx < m_numFiles )   {

         // if its a dir go up one
         if( m_fileType[ sidx ] == FT_DIR )   {
            if( changeBrowserDir( m_fileName[ sidx ] ) == true )   {
               sidx=0;
               soff=0;
               browserProcess( 0x1000, 0 );
            }
         } else {

            if( m_banner[ sidx ] )   {

                        for(i=0;i<128;i++)   {
                           OAM[i*4]      = ATTR0_DISABLED;
                           OAM_SUB[i*4]      = ATTR0_DISABLED;
                        }

                  f = FAT_fopen( m_fileName[ sidx ], "r" );
                  if( f == 0 )
                  {
                        clearFrameBuf();
                        drawFwTitle();
                        drawFwToolbar();
                        drawMessage("R Open Error!");
                        flushFrameBuf();
                        _flushFrameBuf();
                        while(1);
                  }

                  FAT_fseek( f, 4, SEEK_SET );

                  u8 magic[8];

                  FAT_fread( magic, 8, 1, f );

                  FAT_fclose( f) ;

                  if( memcmp( magic, "N30FL4SH", 8 ) == 0 )
                  {
                     strcpy( m_menuFlashFileName, m_fileName[ sidx ] );
                     memcpy( banner, m_banner[sidx], 2112 );
                     startProcess( menuFlashProcess01, 0 );
                     return 1;
                  }

#define read_neo2_cache(a,b,c)                neo2_chip_id_read(a,b+NEO2_CACHE_OFFS,c)
#define write_neo2_cache(a,b,c)               neo2_chip_id_write(a,b+NEO2_CACHE_OFFS,c)
#define NEO2_CACHE_OFFS                           0x0000   // changed to zero for **** sram hack since inner cache wont work
                                                                        // please dont make me cry
//goto start_game_only;
               set_neo2_asic_cr( 3 );
               //neo2_asic_chip_id_mode_enable(); // inner cache really isnt working, lets just disable this
                                                                   // and try writing to logical SRAM instead
               char neomagic[4];
               
#define MK6_INTERNAL_OFFS   ((1024*1024*2)-2048)
               
#ifdef MK6_BUILD

               mk6_nor_read_buf( neomagic,MK6_INTERNAL_OFFS+ 0, 4 );
               
#else

               read_neo2_cache( neomagic, 0, 4 );
#endif

               if( strcmp( neomagic, "NEO" ) == 0 )
               {
                  // we have an old game save
                  // so read it in
#ifdef MK6_BUILD
                  mk6_nor_read_buf( filename, MK6_INTERNAL_OFFS+4, 256 );
#else
                  read_neo2_cache( filename, 4, 256 );
                  // back to sd mode
                  set_neo2_asic_cr( 1<<7 | 1<<10 );
#endif
               }
               else
               {
                  // no old game save
                  // back to sd mode
                  //   neo2_asic_chip_id_mode_disable();   // inner cache :'(
                  set_neo2_asic_cr( 1<<7 | 1<<10 );
                  goto attempt_load;
               }

               char donotdrinkandcodeplease[256]; // lol

               strcpy( donotdrinkandcodeplease, filename );

               i = strlen( donotdrinkandcodeplease );
               while ( donotdrinkandcodeplease[--i] != '.');
               strcpy( &donotdrinkandcodeplease, ".NDS" );

               if( strcmpi( donotdrinkandcodeplease, m_fileName[sidx] ) == 0 )
               {
                  goto start_game_only;
               }




            //////////////////////////////////////////////////////////////////////

            // backup last save
            // FIXME: This should only be done if the file associated with the save exists!

            //   strcpy( filename, m_fileName[ sidx ] );
               i = strlen( filename );
               while ( filename[--i] != '.');
               strcpy( &filename, ".SAV" );


            //////////////////////////////////////////////////////////////////////////////
            // saving message

               clearFrameBuf();
               drawFwTitle();
               drawFwToolbar();

               drawMessage("Saving %s...", filename);

               u16 str[512];

               setFrameBufCol( 1 );
               setLineThickness( 2 );
               drawRect( 4, (SCREEN_HEIGHT/2)-24, SCREEN_WIDTH-4, (SCREEN_HEIGHT/2)+24);

               setFontAnchor( 0, 0 );

               bnr_game_str_ex((u16*)str,m_banner[sidx]);
               _drawFontStringW( (SCREEN_WIDTH/2) + 12 ,(SCREEN_HEIGHT/2)-8, str );
               bnr_maker_str_ex((u16*)str,m_banner[sidx]);
               _drawFontStringW( (SCREEN_WIDTH/2) + 12 ,(SCREEN_HEIGHT/2)+8, str );

               flushFrameBuf();

               i=0;
               mem_cpy16( &SPRITE_PALETTE[16*i], (u16*)&m_banner[sidx][544], 32);
               mem_cpy16( &SPRITE_GFX[16*16*i], (u16*)&m_banner[sidx][32], 512 );
               OBJClear( &OAM[ i*4 ] );
               OBJSize( &OAM[ i*4 ], OBJ_SIZE_32_32 );
               OBJPos( &OAM[ i*4 ], 8, (SCREEN_HEIGHT/2)-16 );
               OBJPal( &OAM[ i*4 ], i );
               OBJTile( &OAM[ i*4 ], i*16 );


#define SAVE_BUF_SIZE   1024 * 32
               //////////////////////////////////////////////////////////////////////
               // back it up
//#ifdef SAVE_MAN_WRITE   
               f = FAT_fopen( filename, "w" );
               if( f == 0 )
               {
                  drawFwFatal( "File Error WO!\n" );
                  // code never reaches here
               }



               for(i=0;i<256*1024;i+=SAVE_BUF_SIZE)   {

                  mk6_disable();
                  flash_rd_2m( i, save_buffer, SAVE_BUF_SIZE );
                  mk6_enable();
                  
               
                  
                  FAT_fwrite( save_buffer, SAVE_BUF_SIZE, 1, f );



                  pcent = i/(256*1024/100);
                  clearFrameBufEx( FRAME_BUF_WIDTH/2-52, FRAME_BUF_HEIGHT/4-12-16+32+16, FRAME_BUF_WIDTH/2+52, FRAME_BUF_HEIGHT/4+12+32+16);
                  setFrameBufCol( 2 );
                  drawRectFill( FRAME_BUF_WIDTH/2-50, FRAME_BUF_HEIGHT/4-12+32+16, FRAME_BUF_WIDTH/2-50+pcent, FRAME_BUF_HEIGHT/4+12+32+16);
                  setFrameBufCol( 1 );
                  drawRect( FRAME_BUF_WIDTH/2-52, FRAME_BUF_HEIGHT/4-12+32+16, FRAME_BUF_WIDTH/2+52, FRAME_BUF_HEIGHT/4+12+32+16);
                  setFontAnchor(0,0);
                  drawFontString( FRAME_BUF_WIDTH/2, FRAME_BUF_HEIGHT/4+32+16, "%u%%", pcent );
                  resetFontAnchor();

                  setFrameBufCol( 1 );
                  setLineThickness( 2 );
                  drawRect( 4, (SCREEN_HEIGHT/2)-24, SCREEN_WIDTH-4, (SCREEN_HEIGHT/2)+24);

                  flushFrameBuf();

               }
               FAT_fclose(f);
//#endif

clearFrameBuf();
               drawFwTitle();
               drawFwToolbar();

               drawMessage(filename);
               drawButtonA("A: OK");


               setFrameBufCol( 1 );
               setLineThickness( 2 );
               drawRect( 4, (SCREEN_HEIGHT/2)-24, SCREEN_WIDTH-4, (SCREEN_HEIGHT/2)+24);

               setFontAnchor( 0, 0 );

               bnr_game_str_ex((u16*)str,m_banner[sidx]);
               _drawFontStringW( (SCREEN_WIDTH/2) + 12 ,(SCREEN_HEIGHT/2)-8, str );
               bnr_maker_str_ex((u16*)str,m_banner[sidx]);
               _drawFontStringW( (SCREEN_WIDTH/2) + 12 ,(SCREEN_HEIGHT/2)+8, str );

               flushFrameBuf();

               i=0;
               mem_cpy16( &SPRITE_PALETTE[16*i], (u16*)&m_banner[sidx][544], 32);
               mem_cpy16( &SPRITE_GFX[16*16*i], (u16*)&m_banner[sidx][32], 512 );
               OBJClear( &OAM[ i*4 ] );
               OBJSize( &OAM[ i*4 ], OBJ_SIZE_32_32 );
               OBJPos( &OAM[ i*4 ], 8, (SCREEN_HEIGHT/2)-16 );
               OBJPal( &OAM[ i*4 ], i );
               OBJTile( &OAM[ i*4 ], i*16 );


               while(1)   {
                  scanKeys();
                  if( keysDown() & KEY_A )         break;
                  swiWaitForVBlank();

               }

attempt_load:

               // FIXME: This should write the full filename without changing the extension!
               // Not all ROMs will be named .NDS!
               strcpy( filename, m_fileName[ sidx ] );
               i = strlen( filename );
               while ( filename[--i] != '.');
               strcpy( &filename, ".SAV" );

#ifdef MK6_BUILD

   char newbuf[256+4];
   memcpy( newbuf, "NEO", 4 );
   memcpy( newbuf+4, filename );
   mk6_nor_program( MK6_INTERNAL_OFFS+ 0, newbuf, 256+4 );


#else


               set_neo2_asic_cr( 3 );
               char neomagic1[4]="NEO";
               write_neo2_cache( neomagic1, 0, 4 );
               write_neo2_cache( filename, 4, 256 );
               read_neo2_cache( filename, 4, 256 );
               set_neo2_asic_cr( 1<<7 | 1<<10 );
#endif
                  //////////////////////////////////////////////////////////////////////////////
               // saving message

               clearFrameBuf();
               drawFwTitle();
               drawFwToolbar();
               
               mk6_disable();

               drawMessage("Erasing NDS save...");


               setFrameBufCol( 1 );
               setLineThickness( 2 );
               drawRect( 4, (SCREEN_HEIGHT/2)-24, SCREEN_WIDTH-4, (SCREEN_HEIGHT/2)+24);

               setFontAnchor( 0, 0 );

               bnr_game_str_ex((u16*)str,m_banner[sidx]);
               _drawFontStringW( (SCREEN_WIDTH/2) + 12 ,(SCREEN_HEIGHT/2)-8, str );
               bnr_maker_str_ex((u16*)str,m_banner[sidx]);
               _drawFontStringW( (SCREEN_WIDTH/2) + 12 ,(SCREEN_HEIGHT/2)+8, str );

               flushFrameBuf();

               i=0;
               mem_cpy16( &SPRITE_PALETTE[16*i], (u16*)&m_banner[sidx][544], 32);
               mem_cpy16( &SPRITE_GFX[16*16*i], (u16*)&m_banner[sidx][32], 512 );
               OBJClear( &OAM[ i*4 ] );
               OBJSize( &OAM[ i*4 ], OBJ_SIZE_32_32 );
               OBJPos( &OAM[ i*4 ], 8, (SCREEN_HEIGHT/2)-16 );
               OBJPal( &OAM[ i*4 ], i );
               OBJTile( &OAM[ i*4 ], i*16 );

               for(i=0;i<256*1024;i+=64*1024)
               {
                  flash_erase_sector(i);

                  pcent = i/(256*1024/100);
                  clearFrameBufEx( FRAME_BUF_WIDTH/2-52, FRAME_BUF_HEIGHT/4-12-16+32+16, FRAME_BUF_WIDTH/2+52, FRAME_BUF_HEIGHT/4+12+32+16);
                  setFrameBufCol( 2 );
                  drawRectFill( FRAME_BUF_WIDTH/2-50, FRAME_BUF_HEIGHT/4-12+32+16, FRAME_BUF_WIDTH/2-50+pcent, FRAME_BUF_HEIGHT/4+12+32+16);
                  setFrameBufCol( 1 );
                  drawRect( FRAME_BUF_WIDTH/2-52, FRAME_BUF_HEIGHT/4-12+32+16, FRAME_BUF_WIDTH/2+52, FRAME_BUF_HEIGHT/4+12+32+16);
                  setFontAnchor(0,0);
                  drawFontString( FRAME_BUF_WIDTH/2, FRAME_BUF_HEIGHT/4+32+16, "%u%%", pcent );
                  resetFontAnchor();

                  setFrameBufCol( 1 );
                  setLineThickness( 2 );
                  drawRect( 4, (SCREEN_HEIGHT/2)-24, SCREEN_WIDTH-4, (SCREEN_HEIGHT/2)+24);

                  flushFrameBuf();

               }
               //flash_erase_2m();
               mk6_enable();

               if( FAT_FileExists( filename ) != FT_NONE )
               {

                  clearFrameBuf();
                  drawFwTitle();
                  drawFwToolbar();

                  drawMessage("Loading %s...", filename);

                  u16 str[512];

                  setFrameBufCol( 1 );
                  setLineThickness( 2 );
                  drawRect( 4, (SCREEN_HEIGHT/2)-24, SCREEN_WIDTH-4, (SCREEN_HEIGHT/2)+24);

                  setFontAnchor( 0, 0 );

                  bnr_game_str_ex((u16*)str,m_banner[sidx]);
                  _drawFontStringW( (SCREEN_WIDTH/2) + 12 ,(SCREEN_HEIGHT/2)-8, str );
                  bnr_maker_str_ex((u16*)str,m_banner[sidx]);
                  _drawFontStringW( (SCREEN_WIDTH/2) + 12 ,(SCREEN_HEIGHT/2)+8, str );


                  pcent = 0;

                  clearFrameBufEx( FRAME_BUF_WIDTH/2-52, FRAME_BUF_HEIGHT/4-12-16+32+16, FRAME_BUF_WIDTH/2+52, FRAME_BUF_HEIGHT/4+12+32+16);
                  setFrameBufCol( 2 );
                  drawRectFill( FRAME_BUF_WIDTH/2-50, FRAME_BUF_HEIGHT/4-12+32+16, FRAME_BUF_WIDTH/2-50+pcent, FRAME_BUF_HEIGHT/4+12+32+16);
                  setFrameBufCol( 1 );
                  drawRect( FRAME_BUF_WIDTH/2-52, FRAME_BUF_HEIGHT/4-12+32+16, FRAME_BUF_WIDTH/2+52, FRAME_BUF_HEIGHT/4+12+32+16);
                  setFontAnchor(0,0);
                  drawFontString( FRAME_BUF_WIDTH/2, FRAME_BUF_HEIGHT/4+32+16, "%u%%", pcent );
                  resetFontAnchor();

                  setFrameBufCol( 1 );
                  setLineThickness( 2 );
                  drawRect( 4, (SCREEN_HEIGHT/2)-24, SCREEN_WIDTH-4, (SCREEN_HEIGHT/2)+24);

                  flushFrameBuf();

                  f = FAT_fopen( filename, "r" );
                  if( f == 0 )
                  {
                        drawFwFatal( "File Error RO!\n" );
                        // code never reaches here
                        while(1);
                  }

                  for(i=0;i<256*1024;i+=SAVE_BUF_SIZE)   {

                     FAT_fread( save_buffer, SAVE_BUF_SIZE, 1, f );
                     mk6_disable();
                     flash_wr_2m( i, save_buffer, SAVE_BUF_SIZE );
                     mk6_enable();
                     pcent = i/(256*1024/100);

                     clearFrameBufEx( FRAME_BUF_WIDTH/2-52, FRAME_BUF_HEIGHT/4-12-16+32+16, FRAME_BUF_WIDTH/2+52, FRAME_BUF_HEIGHT/4+12+32+16);
                     setFrameBufCol( 2 );
                     drawRectFill( FRAME_BUF_WIDTH/2-50, FRAME_BUF_HEIGHT/4-12+32+16, FRAME_BUF_WIDTH/2-50+pcent, FRAME_BUF_HEIGHT/4+12+32+16);
                     setFrameBufCol( 1 );
                     drawRect( FRAME_BUF_WIDTH/2-52, FRAME_BUF_HEIGHT/4-12+32+16, FRAME_BUF_WIDTH/2+52, FRAME_BUF_HEIGHT/4+12+32+16);
                     setFontAnchor(0,0);
                     drawFontString( FRAME_BUF_WIDTH/2, FRAME_BUF_HEIGHT/4+32+16, "%u%%", pcent );
                     resetFontAnchor();

                     setFrameBufCol( 1 );
                     setLineThickness( 2 );
                     drawRect( 4, (SCREEN_HEIGHT/2)-24, SCREEN_WIDTH-4, (SCREEN_HEIGHT/2)+24);

                     flushFrameBuf();

                  }

                  FAT_fclose(f);




            }
start_game_only:
               //////////////////////////////////////////////////////////////////////

               clearFrameBuf();
               drawFwTitle();
               drawFwToolbar();

               drawMessage("Start Game");
               drawButtonA("A: OK");
               drawButtonB("B: Cancel");


               setFrameBufCol( 1 );
               setLineThickness( 2 );
               drawRect( 4, (SCREEN_HEIGHT/2)-24, SCREEN_WIDTH-4, (SCREEN_HEIGHT/2)+24);

               setFontAnchor( 0, 0 );

               bnr_game_str_ex((u16*)str,m_banner[sidx]);
               _drawFontStringW( (SCREEN_WIDTH/2) + 12 ,(SCREEN_HEIGHT/2)-8, str );
               bnr_maker_str_ex((u16*)str,m_banner[sidx]);
               _drawFontStringW( (SCREEN_WIDTH/2) + 12 ,(SCREEN_HEIGHT/2)+8, str );

               flushFrameBuf();

               i=0;
               mem_cpy16( &SPRITE_PALETTE[16*i], (u16*)&m_banner[sidx][544], 32);
               mem_cpy16( &SPRITE_GFX[16*16*i], (u16*)&m_banner[sidx][32], 512 );
               OBJClear( &OAM[ i*4 ] );
               OBJSize( &OAM[ i*4 ], OBJ_SIZE_32_32 );
               OBJPos( &OAM[ i*4 ], 8, (SCREEN_HEIGHT/2)-16 );
               OBJPal( &OAM[ i*4 ], i );
               OBJTile( &OAM[ i*4 ], i*16 );


               while(1)   {
                  scanKeys();
                  if( keysDown() & KEY_A )         break;

                  if( keysDown() & KEY_B )   {

                     while(keysDown() & KEY_B)
                        scanKeys();

                     browserProcess( 0x1000, 0 );

                     return 1;

                  }
                  swiWaitForVBlank();

               }

            } else {
               return 1;
            }

            clearFrameBuf();
               drawFwTitle();
               drawFwToolbar();
               u16 str[512];
               drawMessage("Loading %s...",m_fileName[ sidx ]);

               setFrameBufCol( 1 );
               setLineThickness( 2 );
               drawRect( 4, (SCREEN_HEIGHT/2)-24, SCREEN_WIDTH-4, (SCREEN_HEIGHT/2)+24);

               setFontAnchor( 0, 0 );

               bnr_game_str_ex((u16*)str,m_banner[sidx]);
               _drawFontStringW( (SCREEN_WIDTH/2) + 12 ,(SCREEN_HEIGHT/2)-8, str );
               bnr_maker_str_ex((u16*)str,m_banner[sidx]);
               _drawFontStringW( (SCREEN_WIDTH/2) + 12 ,(SCREEN_HEIGHT/2)+8, str );

               flushFrameBuf();
               _flushFrameBuf();

               // turn these off now to increase stability and loading speed

               REG_IME=0;



            f=FAT_fopen( m_fileName[ sidx ], "r" );
            if( f != (FAT_FILE*)-1 )   {

               // Cheap hack ... :(
               if (FileFragmented()) {
                  clearFrameBuf();
                  drawFwTitle();
                  drawFwToolbar();
                  drawMessage("File fragmentation detected");
                  flushFrameBuf();
                  _flushFrameBuf();
                  while(1);
               }


               ndsImageOffset = (((( FAT_GetFileCluster() -2) * filesysSecPerClus ) + filesysData) * 512 );

               // Clear memory
               memset((void*)0x02000000, 0, 0x00280000);
               memset((void*)0x02380000, 0, 0x0007F000);

               DC_FlushAll();
               IC_InvalidateAll();


               nds_header=(NDS_HEADER*)(0x02400000-0x200);


               FAT_fseek( f, 0, SEEK_SET );   // to support seek error
               if( FAT_fread( nds_header, 0x160, 1, f ) != 0x160 )   {
                  clearFrameBuf();
                  drawFwTitle();
                  drawFwToolbar();
                  drawMessage("Header Read Error 1!");
                  flushFrameBuf();
                  _flushFrameBuf();
                  while(1);
               }

               // do a cheap verify here. so we see error messages instead of black/white screens
               // can be removed when it is stable!

               FAT_fseek( f, 0, SEEK_SET );   // to support seek error
               if( FAT_fread( &nds_vheader, 0x160, 1, f ) != 0x160 )   {
                  clearFrameBuf();
                  drawFwTitle();
                  drawFwToolbar();
                  drawMessage("Header Read Error 2!");
                  flushFrameBuf();
                  _flushFrameBuf();
                  while(1);
               }

               if( memcmp( (void*)nds_header, (void*)&nds_vheader, 0x160 ) )   {
                  clearFrameBuf();
                  drawFwTitle();
                  drawFwToolbar();
                  drawMessage("Header Verify Error!");
                  flushFrameBuf();
                  _flushFrameBuf();
                  while(1);
               }

               //if ((nds_header->arm9_dst >= 0x02000000) && (nds_header->arm9_dst < 0x02800000) && (nds_header->arm7_dst >= 0x02000000) && (nds_header->arm7_dst < 0x02800000)) {

                  u8 *p;
                  u32 dst = nds_header->arm7_dst;

                  // If ARM7 is to be copied into ARM7 EWRAM, let the loader do it. (Homebrew only!)
                  if ((dst >= 0x02000000) && (dst < 0x02800000)) {

                     p = (u8*)dst;

                     size = ((nds_header->arm7_size + 0x1FF) & ~0x1FF);
                     FAT_fseek( f, 0, SEEK_END );
                     if ((nds_header->arm7_src + size) > FAT_ftell(f)) size = nds_header->arm7_size;

                     FAT_fseek( f, nds_header->arm7_src, SEEK_SET );
                     if( FAT_fread( p, size, 1, f ) != size )   {
                        clearFrameBuf();
                        drawFwTitle();
                        drawFwToolbar();
                        drawMessage("ARM7 Read Error 1!");
                        flushFrameBuf();
                        _flushFrameBuf();
                        while(1);
                     }
                  }

#ifdef NEO_LOAD_VERIFY

                  FAT_fseek( f, nds_header->arm7_src, SEEK_SET );
                  for(i=0;i<size;i+=512)   {

                     if( FAT_fread( buf, 512, 1, f ) != 512 )   {
                        clearFrameBuf();
                        drawFwTitle();
                        drawFwToolbar();
                        drawMessage("ARM7 Read Error 2!");
                        flushFrameBuf();
                        _flushFrameBuf();
                        while(1);
                     }

                     if( memcmp( (void*)buf, (void*)&p, 512 ) )   {
                        clearFrameBuf();
                        drawFwTitle();
                        drawFwToolbar();
                        drawMessage("ARM7 Verify Error!");
                        flushFrameBuf();
                        _flushFrameBuf();
                        while(1);
                     }

                  }

#endif

                  // FIXME: The loaders cannot handle ARM9 destinations above 0x02280000
                  if (nds_header->arm9_dst >= 0x02280000) {
                     clearFrameBuf();
                     drawFwTitle();
                     drawFwToolbar();
                     drawMessage("ARM9 Read Error 1b!");
                     flushFrameBuf();
                     _flushFrameBuf();
                     while(1);
                  }

                  p=(u8*)nds_header->arm9_dst;

                  size = ((nds_header->arm9_size + 0x1FF) & ~0x1FF);
                  FAT_fseek( f, 0, SEEK_END );
                  if ((nds_header->arm9_src + size) > FAT_ftell(f)) size = nds_header->arm9_size;

                  // FIXME: Need proper handling of large executables
                  // This will only load data up to address 0x02280000 (Homebrew only!)
                  if ((nds_header->arm9_dst + size) > 0x02280000) size = (0x02280000 - nds_header->arm9_dst);

                  FAT_fseek( f, nds_header->arm9_src, SEEK_SET );
                  if( FAT_fread( p, size, 1, f ) != size )   {
                     clearFrameBuf();
                     drawFwTitle();
                     drawFwToolbar();
                     drawMessage("ARM9 Read Error 1!");
                     flushFrameBuf();
                     _flushFrameBuf();
                     while(1);
                  }

#ifdef NEO_LOAD_VERIFY

                  FAT_fseek( f, nds_header->arm9_src, SEEK_SET );
                  for(i=0;i<size;i+=512)   {

                     if( FAT_fread( buf, 512, 1, f ) != 512 )   {
                        clearFrameBuf();
                        drawFwTitle();
                        drawFwToolbar();
                        drawMessage("ARM9 Read Error 2!");
                        flushFrameBuf();
                        _flushFrameBuf();
                        while(1);
                     }

                     if( memcmp( (void*)buf, (void*)&p, 512 ) )   {
                        clearFrameBuf();
                        drawFwTitle();
                        drawFwToolbar();
                        drawMessage("ARM9 Verify Error!");
                        flushFrameBuf();
                        _flushFrameBuf();
                        while(1);
                     }

                  }

#endif

//               }

               FAT_fclose( f );

               /*scanKeys();
               if( (keysDown() & KEY_B) == 0 )
                  scanKeys();*/
   //            if ((nds_header->arm9_dst >= 0x02000000) && (nds_header->arm9_dst < 0x02800000) && (nds_header->arm7_dst >= 0x02000000) && (nds_header->arm7_dst < 0x02800000)) {


                     if (CheckForSDK()) {
                        if ((nds_header->arm9_dst + nds_header->arm9_size) >= 0x02280000) {
                           clearFrameBuf();
                           drawFwTitle();
                           drawFwToolbar();
                           drawMessage("ARM9 Read Error 1b!");
                           flushFrameBuf();
                           _flushFrameBuf();
                           while(1);
                        }

                        neoExit();

                        NDS_CONFIG *config;
                        if ((config = getLoaderConfig())) {
                           if (config->force_save != SAVE_PATCH_NONE) {
                              // This would be better done with a 'simulation' patch
                              // ... or even better, finding out why the default patching does not work!
                              /*
                                 clearFrameBuf();
                                 drawFwTitle();
                                 drawFwToolbar();
                                 drawMessage("This game requires a different save type");
                                 flushFrameBuf();
                                 _flushFrameBuf();
                                 while(1);
                              */
                           }

                           if (config->mem_alloc == MEM_ALLOC_SECURE) {
                              startLoaderDriverSecure( 0, ndsImageOffset, 0, 0, 0, mk6_tf_drv_bin, mk6_tf_drv_bin_size );
                           }
                           else {
                              startLoaderDriver( 0, ndsImageOffset, 0, 0, 0, mk6_tf_drv_bin, mk6_tf_drv_bin_size );
                           }

                           setLoaderConfig( config->mem_alloc, config->patch_save, config->force_save );
                        }
                        else {
                           startLoaderDriverSecure( 0, ndsImageOffset, 0, 0, 0, mk6_tf_drv_bin, mk6_tf_drv_bin_size );
                        }

                        startLoader();
                     }
                     else {
                        neoExit();
                        startLoaderDriverHomebrew( 0, ndsImageOffset, 0, 0, 0, mk6_tf_drv_bin, mk6_tf_drv_bin_size );
                        startLoaderHomebrew();
                     }

                     // code should never reaches here
   //            } else {
   //               exitProcess();
   //            }

            }
            return 1;
         }
      }
      return 1;
   }

   if( msg == 2 && arg == KEY_B )   {
         if( changeBrowserDir( ".." ) == true )   {
            sidx=0;
            soff=0;
            for(i=0;i<128;i++)   {
               OAM[i*4]      = ATTR0_DISABLED;
               OAM_SUB[i*4]      = ATTR0_DISABLED;
            }
            browserProcess( 0x1000, 0 );
         } else {
            for(i=0;i<128;i++)   {
               OAM[i*4]      = ATTR0_DISABLED;
               OAM_SUB[i*4]      = ATTR0_DISABLED;
            }
            exitProcess();

         }
         return 1;
   }

   if( msg == 2 && arg == KEY_X )   {
      startProcess( dumperProcess01, 0 );
      return 1;
   }
/*
   if( msg == 2 && arg == KEY_Y )   {
      strcpy( m_menuFlashFileName, m_fileName[ sidx ] );
      startProcess( menuFlashProcess01, 0 );
      return 1;
   }
*/
   if( msg == 2 && arg == KEY_UP )   {
      if( sidx < m_numFiles-1 )   {
         soff-=32+8;
         sidx++;
         browserProcess( 0x1000, 0 );
      }
      return 1;
   }

   if( msg == 2 && arg == KEY_DOWN )   {
      if( sidx > 0 )   {
         soff+=32+8;
         sidx--;
         browserProcess( 0x1000, 0 );
      }
      return 1;
   }

      if( msg == 2 && arg == KEY_R )   {
      if( sidx < m_numFiles-4 )   {
         soff-=(32+8)*4;
         sidx+=4;
         browserProcess( 0x1000, 0 );
      }
      return 1;
   }

   if( msg == 2 && arg == KEY_L )   {
      if( sidx > 3 )   {
         soff+=(32+8)*4;
         sidx-=4;
         browserProcess( 0x1000, 0 );
      }
      return 1;
   }

   if( msg == 0x1000 )   {

      for(i=0;i<128;i++)   {
         OAM[i*4]      = ATTR0_DISABLED;
         OAM_SUB[i*4]      = ATTR0_DISABLED;
      }

      clearFrameBuf();
      drawFwTitle();
      drawFwToolbarBrowser();
      y=getFontHeight()*2;

      setFrameBufClip( 0, getFontHeight()*2, FRAME_BUF_WIDTH, FRAME_BUF_HEIGHT-(getFontHeight()*2) );


      for(i=0,x=0,y=(FRAME_BUF_HEIGHT/2)+soff+4;x<m_numFiles;x++,y+=32+8)   {

         if( x >= sidx - 4 && x <= sidx + 3 )   {
            if( sidx == x )   setFrameBufCol( 1 );
            else                  setFrameBufCol( 2 );
            drawFontString(32+8,y,m_fileName
);

            if( m_fileType
== FT_DIR )                  drawFontString(32+8,y+getFontHeight(), "Directory" );

            else if( m_fileType
== FT_FILE )   {


               if( m_banner
)   {

                  drawFontString(32+8,y+getFontHeight(), "NDS Game File" );
               } else {
                  drawFontString(32+8,y+getFontHeight(), "Unknown File Type" );
               }
            }

            if( x >= sidx && m_banner
)   {

                  mem_cpy16( &SPRITE_PALETTE_SUB[16*i], (u16*)&m_banner
[544], 32);

                  mem_cpy16( &SPRITE_GFX_SUB[16*16*i], (u16*)&m_banner
[32], 512 );

                  OBJClear( &OAM_SUB[ i*4 ] );
                  OBJSize( &OAM_SUB[ i*4 ], OBJ_SIZE_32_32 );
                  OBJPos( &OAM_SUB[ i*4 ], 2, y-SCREEN_HEIGHT );
                  OBJPal( &OAM_SUB[ i*4 ], i );
                  OBJTile( &OAM_SUB[ i*4 ], i*16 );
                  i++;
            } else if( m_banner
) {

                  mem_cpy16( &SPRITE_PALETTE[16*i], (u16*)&m_banner
[544], 32);

                  mem_cpy16( &SPRITE_GFX[16*16*i], (u16*)&m_banner
[32], 512 );

                  OBJClear( &OAM[ i*4 ] );
                  OBJSize( &OAM[ i*4 ], OBJ_SIZE_32_32 );
                  OBJPos( &OAM[ i*4 ], 2, y );
                  OBJPal( &OAM[ i*4 ], i );
                  OBJTile( &OAM[ i*4 ], i*16 );
                  i++;
            }

         }

      }

      resetFrameBufClip();

      return 1;
   }

   return 0;
}

int firmwareProcess( unsigned int msg, unsigned int arg )   {

   static int i = 0;
   
   u8 buf[512];
   u8 newbuf[512];

for(i=0;i<512;i++)
{
   buf=i;
}

   if( msg == PROCESS_MSG_START )   {

         clearFrameBuf();
         drawFwTitle();
         drawFwToolbar();
         drawMessage("Insert an SD card into NEO2");
         drawButtonA("A: OK");

         return 1;

   }

   if( ( msg == PROCESS_MSG_KEY ) && ( arg == KEY_A ) )   {

   //*(vu8*)(0x0A000000) = i++;

         clearFrameBuf();
         drawFwTitle();
         drawFwToolbar();
         drawMessage("Please wait...");
         flushFrameBuf();
         _flushFrameBuf();
//         REG_IME=0;

         if( FAT_InitFiles() == true )
         {
//            REG_IME=1;
            //startProcess( browserProcess, 0 );
            
               clearFrameBuf();
         drawFwTitle();
         drawFwToolbar();
         drawMessage("press a to write...");
         flushFrameBuf();
         _flushFrameBuf();
            
               while(1)   {
                  scanKeys();
                  if( keysDown() & KEY_A )         break;
                  swiWaitForVBlank();

               }
            
            clearFrameBuf();
         drawFwTitle();
         drawFwToolbar();
         drawMessage("writing...");
         flushFrameBuf();
         _flushFrameBuf();
         
         
         mk6_tf_write_sector( buf, 1024*1024*200 );
         
            clearFrameBuf();
         drawFwTitle();
         drawFwToolbar();
         drawMessage("press a to read...");
         flushFrameBuf();
         _flushFrameBuf();
            
               while(1)   {
                  scanKeys();
                  if( keysDown() & KEY_A )         break;
                  swiWaitForVBlank();

               }
         
               clearFrameBuf();
         drawFwTitle();
         drawFwToolbar();
         drawMessage("reading...");
         flushFrameBuf();
         _flushFrameBuf();
         
         
         mk6_tf_read_sector_b( newbuf, 1024*1024*200 );
            
            
         if( memcmp( buf, newbuf, 512 ) == 0 )
         {   
            clearFrameBuf();
            drawFwTitle();
            drawFwToolbar();
            drawMessage("write ok");
            flushFrameBuf();
            _flushFrameBuf();
         }
         else
         {
            clearFrameBuf();
            drawFwTitle();
            drawFwToolbar();
            drawMessage("write error");
            flushFrameBuf();
            _flushFrameBuf();
         }
         
         while(1);
            
         }
         else
         {
//            REG_IME=1;
            firmwareProcess( 1, 0 );
         }



         return 1;

   }

   return 0;
}



//------------------------------------------------------------------------------
int main() {

   u32 i;
   char buf[64];
   vu8 *p=(vu8*)(0x08000000);


   char neomagic[4];
   char filename[256];
   FAT_FILE *f;
   u32 size;
   NDS_HEADER *nds_header;
   NDS_CONFIG *config;
   u16 wait_cr;

   ndsImageOffset = *(u32*)(0x023FF9DC);
   if (ndsImageOffset) {

      wait_cr = WAIT_CR;
      WAIT_CR &= ~(0x8080);

      set_neo2_asic_cr( 3 );
      read_neo2_cache( neomagic, 0, 4 );

      if( strcmp( neomagic, "NEO" ) == 0 )
      {
         // we have an old game save
         // so read it in
         read_neo2_cache( filename, 4, 256 );

         // back to sd mode
         set_neo2_asic_cr( 1<<7 | 1<<10 );
      }
      else
      {
         // no old game save
         while(1);
      }

      i = strlen( filename );
      while ( filename[--i] != '.');
      strcpy( &filename, ".NDS" );


      FAT_InitFiles();
      f = FAT_fopen( filename, "r" );


      // Clear memory
      memset((void*)0x02000000, 0, 0x00280000);
      memset((void*)0x02380000, 0, 0x0007F000);

      DC_FlushAll();
      IC_InvalidateAll();


      nds_header=(NDS_HEADER*)(0x02400000-0x200);


      FAT_fseek( f, 0, SEEK_SET );
      FAT_fread( nds_header, 0x160, 1, f );


      u8 *p;
      p = (u8*)nds_header->arm7_dst;

      size = ((nds_header->arm7_size + 0x1FF) & ~0x1FF);
      FAT_fseek( f, 0, SEEK_END );
      if ((nds_header->arm7_src + size) > FAT_ftell(f)) size = nds_header->arm7_size;

      FAT_fseek( f, nds_header->arm7_src, SEEK_SET );
      FAT_fread( p, size, 1, f );


      p=(u8*)nds_header->arm9_dst;

      size = ((nds_header->arm9_size + 0x1FF) & ~0x1FF);
      FAT_fseek( f, 0, SEEK_END );
      if ((nds_header->arm9_src + size) > FAT_ftell(f)) size = nds_header->arm9_size;

      FAT_fseek( f, nds_header->arm9_src, SEEK_SET );
      FAT_fread( p, size, 1, f );


      FAT_fclose( f );


      WAIT_CR = wait_cr;


      if ((config = getLoaderConfig())) {
         if (config->mem_alloc == MEM_ALLOC_SECURE) {
            startLoaderDriverSecure( 0, ndsImageOffset, 0, 0, 0, mk6_tf_drv_bin, mk6_tf_drv_bin_size );
         }
         else {
            startLoaderDriver( 0, ndsImageOffset, 0, 0, 0, mk6_tf_drv_bin, mk6_tf_drv_bin_size );
         }

         setLoaderConfig( config->mem_alloc, config->patch_save, config->force_save );
      }
      else {
         startLoaderDriverSecure( 0, ndsImageOffset, 0, 0, 0, mk6_tf_drv_bin, mk6_tf_drv_bin_size );
      }

      startLoader();

      // Should never reach here
   }


   neoStart();
   processInit();

/*   set_neo2_asic_cr( 1<<13 );

   neoFlashHandle_t handle;

   if( neoFlashOpen( &handle ) )   {
      clearFrameBuf();
      drawFwTitle();
      drawFwToolbar();
      drawMessage( "%s %s %u %u", handle.makerString, handle.devString, handle.blockSize, handle
