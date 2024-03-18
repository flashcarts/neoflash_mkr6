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

#include "process.h"

#define PROCESS_MAX   256   // unreasonably high maximum number of processes, total

static unsigned int m_processLevel;
static process_t m_process[ PROCESS_MAX ];
static unsigned int m_arg[ PROCESS_MAX ];


void processInit()   {
   unsigned int i;
   m_processLevel=0;
   for(i=0;i<PROCESS_MAX;i++)
      m_process=0;
}

process_t getCurrentProcess()   {
   if(m_processLevel == 0 )
      return 0;
   return m_process[ m_processLevel ];
}

int startProcess( process_t process, unsigned int arg )   {
   if( m_processLevel == PROCESS_MAX - 1 )   return 0;
   m_processLevel++;
   m_process[ m_processLevel ] = process;
   m_arg[ m_processLevel ] = arg;
   return process( PROCESS_MSG_START, arg );
}

int exitProcess()   {
   process_t process;
   unsigned int arg;
   if( m_processLevel == 0 )
      return 0;
   m_processLevel--;
   process=getCurrentProcess();
   if( process )   {
      arg = m_arg[ m_processLevel ];
      m_processLevel--;
      return startProcess( process, arg );
   }
   return 0;
}

void exitAllProcesses()   {
   m_processLevel = 0;
}

int switchProcess( process_t process, unsigned int arg )   {
   m_processLevel--;
   return startProcess( process, arg );
}

int pumpProcess()   {
   process_t process=getCurrentProcess();
   if( process == 0 )   return 0;
   scanKeys();
   if( keysDown() & KEY_A )         process( PROCESS_MSG_KEY, KEY_A );
   if( keysDown() & KEY_B )         process( PROCESS_MSG_KEY, KEY_B );
   if( keysDown() & KEY_L )         process( PROCESS_MSG_KEY, KEY_L );
   if( keysDown() & KEY_R )         process( PROCESS_MSG_KEY, KEY_R );
   if( keysDown() & KEY_START )   process( PROCESS_MSG_KEY, KEY_START );
   if( keysDown() & KEY_SELECT )   process( PROCESS_MSG_KEY, KEY_SELECT );
   if( keysDown() & KEY_LEFT )      process( PROCESS_MSG_KEY, KEY_LEFT );
   if( keysDown() & KEY_RIGHT )   process( PROCESS_MSG_KEY, KEY_RIGHT );
   if( keysDown() & KEY_UP )         process( PROCESS_MSG_KEY, KEY_UP );
   if( keysDown() & KEY_DOWN )      process( PROCESS_MSG_KEY, KEY_DOWN );
   if( keysDown() & KEY_X )         process( PROCESS_MSG_KEY, KEY_X );
   if( keysDown() & KEY_Y )         process( PROCESS_MSG_KEY, KEY_Y );
   process( PROCESS_MSG_MAIN, 0 );
   return 1;
}
