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

#define PROCESS_MSG_MAIN   0
#define PROCESS_MSG_START   1
#define PROCESS_MSG_KEY      2

typedef int (*process_t)(unsigned int, unsigned int);

void processInit();
process_t getCurrentProcess();
int startProcess( process_t process, unsigned int arg );
int exitProcess();
void exitAllProcesses();
int switchProcess( process_t process, unsigned int arg );
int pumpProcess();
