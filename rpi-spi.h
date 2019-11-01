#ifndef _RPI_SPI_
#define _RPI_SPI_

#ifdef __cplusplus								// If we are including to a C++
extern "C" {									// Put extern C directive wrapper around
#endif

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++}
{																			}
{       Filename: rpi-spi.h													}
{       Copyright(c): Leon de Boer(LdB) 2019								}
{       Version: 1.00														}
{																			}
{***************[ THIS CODE IS FREEWARE UNDER CC Attribution]***************}
{																            }
{      The SOURCE CODE is distributed "AS IS" WITHOUT WARRANTIES AS TO      }
{   PERFORMANCE OF MERCHANTABILITY WHETHER EXPRESSED OR IMPLIED.            }
{   Redistributions of source code must retain the copyright notices to     }
{   maintain the author credit (attribution) .								}
{																			}
{***************************************************************************}
{                                                                           }
{   This provides all the SPI functionality and setup for each PI model		}
{																            }
{++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

void spi_init(void);



#ifdef __cplusplus								// If we are including to a C++ file
}												// Close the extern C directive wrapper
#endif

#endif