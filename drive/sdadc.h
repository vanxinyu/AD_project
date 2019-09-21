#ifndef __SDADC_H
#define	__SDADC_H

#include "stm32f37x.h"
#include "systick.h"


#define POT_GPIO_PORT        GPIOB
#define POT_GPIO_PIN         GPIO_Pin_1
#define POT_GPIO_CLK         RCC_AHBPeriph_GPIOB
#define POT_SDADC            SDADC1
#define POT_SDADC_CLK        RCC_APB2Periph_SDADC1
#define POT_SDADC_PWR        PWR_SDADCAnalog_1
#define POT_SDADC_VREF       SDADC_VREF_Ext /* External reference is selected */
#define POT_SDADC_GAIN       SDADC_Gain_1   /* Internal gain 1 is seleted: 
                                               SDADC_GAIN must be updated according to                                              POT_SDADC_GAIN */
#define SDADC_GAIN           (uint32_t) 1  /* SDADC internal gain is set to 1: update this define
                                              according to POT_SDADC_GAIN */
#define POT_SDADC_CHANNEL    SDADC_Channel_5
#define SDADC_RESOL          (uint32_t) 65535 /* 2e16 - 1 */
#define SDADC_INIT_TIMEOUT   30 /* ~ about two SDADC clock cycles after INIT is set */
#define SDADC_CAL_TIMEOUT    4*30720 /*  ~5.12 ms at 6 MHz  in a single calibration sequence */
#define SDADC_VREF           (float) 3300  /* SDADC external reference is set to 3.3V */

uint32_t SDADC1_Config(void);

#endif /* __SDADC_H */
