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

#ifndef __SPI_H
#define __SPI_H

#include <nds/jtypes.h>

//void inline spi_wait();
//void inline spi_open();
//u8 inline spi_exch(u8 data);
//void inline spi_send(u8 data);
//u8 inline spi_recv();
//void inline spi_close();

//extern "C"
//{

void spi_open( u16 freq );
void spi_close( void );
u8 spi ( u8 data );
void spi_wait( void );

//}
#define spi_exch(x) spi( x )
#define spi_send(x) spi( x )
#define spi_recv(x)   spi( 0xFF )

#endif
