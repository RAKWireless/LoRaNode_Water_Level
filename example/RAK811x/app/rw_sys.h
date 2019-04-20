#ifndef  _RW_SYS_H_
#define  _RW_SYS_H_

/*!
 * User application data buffer size
 */
#if defined( USE_BAND_433 ) || defined( USE_BAND_780 ) || defined( REGION_EU868 ) || defined( REGION_AS923 )

#define MAX_LORA_SEND_SIZE                       16

#elif defined( USE_BAND_915 ) || defined( USE_BAND_915_HYBRID )

#define MAX_LORA_SEND_SIZE                       11

#endif

#define P2P_FREQ_MIN                  860000000 //470000000 //860000000 
#define P2P_FREQ_MAX                  1020000000 //1020000000

#define CHECK_UART_BAUD(baud) ((baud==9600) ||(baud==19200) ||(baud==38400) ||(baud==57600) ||(baud==115200)||(baud==230400) || (baud==460800) || (baud==921600))
#define CHECK_UART_DATABIT(databit) (databit==UART_8_BIT)
#define CHECK_UART_PARITY(parity) ((parity==NO_PARITY) || (parity==EVEN_PARITY) || (parity==ODD_PARITY))
#define CHECK_UART_STOPBIT(stop) ((stop==UART_1_STOP_BIT) || (stop==UART_2_STOP_BIT))
#define CHECK_UART_FLOW(flow) ((flow == 0) || (flow == 1))

/***check p2p parameters add by junhua****/
#define CHECK_P2P_FREQ(freq) ((freq >= P2P_FREQ_MIN) && (freq <= P2P_FREQ_MAX))
#define CHECK_P2P_SF(sf) ((sf >= 6) && (sf <= 12))
#define CHECK_P2P_BDW(bandw) ((bandw == 0) || (bandw == 1) || (bandw == 2))
#define CHECK_P2P_CR(cr) ((cr >= 1) && (cr <= 4))
#define CHECK_P2P_PREMLEN(prelen) ((prelen >= 5) && (prelen <= 65535))
#define CHECK_P2P_PWR(pwr) ((pwr >= 5) && (pwr <= 20))

#endif