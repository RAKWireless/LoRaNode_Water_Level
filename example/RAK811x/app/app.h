#ifndef _APP_H_
#define _APP_H_
#include "board.h"
#include "LoRaMac.h"
#include "Region.h"
#include "lora_config.h"
#include "partition.h"
#include "log.h"
#include "cmd_error.h"
#include "rw_assert.h"
#include "pins_conf.h"

#define DEBUG_FW           1

#define MAJOR_VER          2
#define CUSTOM_VER         0
#define FUN_VER            2
#define BUG_VER            4
#define TEST_VER           5  //1: GPS U6 U6A 3£ºAU915 remove ACMA
                              //4: alan water demo

#if defined( REGION_EU868 )
#define LORAMAC_REGION_DEFAULT     LORAMAC_REGION_EU868
#elif defined( REGION_AS923 ) 
#define LORAMAC_REGION_DEFAULT     LORAMAC_REGION_AS923
#elif defined( REGION_US915 ) 
#define LORAMAC_REGION_DEFAULT     LORAMAC_REGION_US915
#elif defined( REGION_US915_HYBRID ) 
#define LORAMAC_REGION_DEFAULT     LORAMAC_REGION_US915_HYBRID
#elif defined( REGION_KR920 ) 
#define LORAMAC_REGION_DEFAULT     LORAMAC_REGION_KR920
#elif defined( REGION_IN865) 
#define LORAMAC_REGION_DEFAULT     LORAMAC_REGION_IN865
#elif defined( REGION_CN779) 
#define LORAMAC_REGION_DEFAULT     LORAMAC_REGION_CN779
#elif defined( REGION_CN470) 
#define LORAMAC_REGION_DEFAULT     LORAMAC_REGION_CN470
#elif defined( REGION_AU915) 
#define LORAMAC_REGION_DEFAULT     LORAMAC_REGION_AU915
#elif defined( REGION_EU433) 
#define LORAMAC_REGION_DEFAULT     LORAMAC_REGION_EU433
#endif

/*!
 * LoRaMac datarates definition
 */
#define SF_12                                       12  // SF12 - BW125
#define SF_11                                       11  // SF11 - BW125
#define SF_10                                       10  // SF10 - BW125
#define SF_9                                        9  // SF9  - BW125
#define SF_8                                        8  // SF8  - BW125
#define SF_7                                        7  // SF7  - BW125
#define SF_6                                        6  // SF7  - BW250

enum lora_event{
    LORA_EVENT_RECV_DATA = 0,
    LORA_EVENT_TX_COMFIRMED = 1,
    LORA_EVENT_TX_UNCOMFIRMED = 2,
    LORA_EVENT_JOINED_OTAA  = 3,
    LORA_EVENT_JOINED_FAILED = 4,
    LORA_EVENT_TX_TIMEOUT = 5,
    LORA_EVENT_RX2_TIMEOUT = 6,
    LORA_EVENT_DOWNLINK_REPEATED = 7,
    LORA_EVENT_WAKEUP = 8,
    /**Lora P2P define**/
    LORA_EVENT_P2PTX_COMPLETE = 9,
    LORA_EVENT_LINK_CHECK = 10,
    LORA_EVENT_UNKNOWN = 100,
};

typedef enum eJoinMode{
   ABP_MODE = 0,
   OTAA_MODE,
   MAX_MODE
}E_JOIN_MODE;

#define SET_UART_CONFIG_DEFAULT(a) {\
                                       uart_config_t temp = DEFAULT_VALUE;\
                                       memcpy(&a, &temp, sizeof(temp));\
                                       write_partition(PARTITION_1, (char *)&a, sizeof(a));\
                                    }\


extern lora_config_t     g_lora_config;
extern lora_system_t     g_lora_system;

extern TimerTime_t SendRadioP2PFrame( S_LORAP2P_PARAM LoraP2PParams, void *fBuffer, uint16_t fBufferSize);
extern RadioState_t RxRadioP2PFrame( S_LORAP2P_PARAM LoraP2PParams );

#endif