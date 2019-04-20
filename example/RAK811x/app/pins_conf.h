/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: contains hardaware configuration Macros and Constants

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RAK811_PINS_H__
#define __RAK811_PINS_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define RAK811_PINS_MASK                            (0x076EE19E) //  byte0: 10011110
                                                                 //  byte1: 11100001
                                                                 //  byte2: 01101110
                                                                 //  byte3: 00000111 

#define RAK811_ADC_PINS_MASK                        (0x0068000E) //  byte0: 00001110
                                                                 //  byte1: 00000000
                                                                 //  byte2: 01101000
                                                                 //  byte3: 00000000

#define RAK811_PIN2                                 PB_12       //ADC_IN18  

#define RAK811_PIN3                                 PB_14      //ADC_IN20
   
#define RAK811_PIN4                                 PB_15       //ADC_IN21

#define RAK811_PIN5                                 PA_8
   
#define RAK811_PIN8                                 PA_12
   
#define RAK811_PIN9                                 PB_4
   
#define RAK811_PIN14                                 PA_15

#define RAK811_PIN15                                 PB_3

#define RAK811_PIN16                                 PB_5

#define RAK811_PIN18                                 PB_8  //I2C1_SCL

#define RAK811_PIN19                                 PB_9  //I2C1_SDA

#define RAK811_PIN20                                 PA_2  //ADC_IN2

#define RAK811_PIN22                                 PA_1  //ADC_IN1

#define RAK811_PIN23                                 PA_0  //ADC_IN0

#define RAK811_PIN25                                 PB_10 
   
#define RAK811_PIN26                                 PB_11 

#define RAK811_PIN27                                 PB_2   
   
   
const static PinNames RAK811_pin_array[32] ={ NC, RAK811_PIN2, RAK811_PIN3, RAK811_PIN4, RAK811_PIN5, NC, NC, RAK811_PIN8, 
                                              RAK811_PIN9, NC, NC, NC, NC, RAK811_PIN14, RAK811_PIN15, RAK811_PIN16,
                                              NC, RAK811_PIN18, RAK811_PIN19, RAK811_PIN20, NC, RAK811_PIN22, RAK811_PIN23, NC,
                                              RAK811_PIN25, RAK811_PIN26, RAK811_PIN27, NC, NC, NC, NC, NC};

//const static ADC_PinMap RAK811_adc_pin_map[32][2] ={{NC,0}, {RAK811_PIN2, ADC_CHANNEL_18}, {RAK811_PIN3, ADC_CHANNEL_20}, {RAK811_PIN4, ADC_CHANNEL_21},{NC,0},{NC,0},{NC,0},{NC,0},
//                                                    {NC,0}, {NC,0}, {NC,0}, {NC,0}, {NC,0}, {NC,0}, {NC,0}, {NC,0},
//                                                    {NC,0}, {NC,0}, {NC,0}, {RAK811_PIN20, ADC_CHANNEL_2}, {NC,0}, {RAK811_PIN22, ADC_CHANNEL_1}, {RAK811_PIN23, ADC_CHANNEL_0}, {NC,0},
//                                                    {NC,0}, {NC,0}, {NC,0}, {NC,0}, {NC,0}, {NC,0}, {NC,0}, {NC,0}};

const static uint32_t RAK811_adc_pin_map[32] ={0, ADC_CHANNEL_18, ADC_CHANNEL_20, ADC_CHANNEL_21, 0, 0, 0, 0,  //1-8
                                               0, 0, 0, 0, 0, 0, 0, 0,                                         //9-16
                                               0, 0, 0, ADC_CHANNEL_2, 0, ADC_CHANNEL_1, ADC_CHANNEL_0, 0,     //7-24
                                               0, 0, 0, 0, 0, 0, 0, 0};                                        //25-32

#ifdef __cplusplus
}
#endif

#endif /* __RAK811_PINS_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
