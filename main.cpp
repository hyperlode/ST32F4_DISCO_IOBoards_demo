

//notes: to convert to cpp:
//-change name
//-configuration -> compile "use CPP"
//-configurations-> link ->"don't use the standard system startup files"


#define HSE_VALUE ((uint32_t)8000000) /* STM32 discovery uses a 8Mhz external crystal */



#include "main.h"

char lodeStrTest []={'a','\0'};


/*
 * The USB data must be 4 byte aligned if DMA is enabled. This macro handles
 * the alignment, if necessary (it's actually magic, but don't tell anyone).
 */
__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;




/*
 * Define prototypes for interrupt handlers here. The conditional "extern"
 * ensures the weak declarations from startup_stm32f4xx.c are overridden.
 */
#ifdef __cplusplus
 extern "C" {
#endif
 void init();
 void ColorfulRingOfDeath(void);


void SysTick_Handler(void);
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void OTG_FS_IRQHandler(void);
void OTG_FS_WKUP_IRQHandler(void);

#ifdef __cplusplus
}
#endif


 void Delay(__IO uint32_t nCount);

 void Delay(__IO uint32_t nCount)
 {
   while(nCount--)
   {
   }
 }


int main(void)
{
	/* Set up the system clocks */
	SystemInit();

	/* Initialize USB, IO, SysTick, and all those other things you do in the morning */
	init();

	STM_EVAL_LEDInit(LED5);
	STM_EVAL_LEDOn(LED5);
	STM_EVAL_LEDInit(LED3);
	//set up adc
	//adc_configure();//Start configuration
	adc_multiChannelConfigure();
	while (ticker < 500)
	{


	}
	IOBoard panel1;

	printf("Userinterface: \r\n");
	printf("send 'v' for adc values \r\n");
	while (1)
	{
		if (ticker >= 500 && conversionEdgeMemory ==0){
			STM_EVAL_LEDToggle(LED3) ;
			ADC_SoftwareStartConv(ADC1);
		}
		conversionEdgeMemory = ticker >= 500 ;
		if (ticker>1000){
			ticker =0;
		}
/*
		// Blink the orange LED at 1Hz
		if (500 == ticker)
		{
			GPIOD->BSRRH = GPIO_Pin_13;

		}
		else if (1000 == ticker)
		{
			ticker = 0;
			GPIOD->BSRRL = GPIO_Pin_13;
		}
/**/

		// If there's data on the virtual serial port:
		 //  - Echo it back
		 //  - Turn the green LED on for 10ms
		 //
		uint8_t theByte;
		if (VCP_get_char(&theByte))
		{


			if ( theByte != '\r' &&  theByte != '\n'){
				printf("Char Sent: %c  \r\n", theByte); //VCP_put_char(theByte);

				if ( theByte == 'v'){
					//printf ("value %d /r/n", ConvertedValue);
					printf ("value TEMPERATURE %d \r\n", temp);
					printf ("value VREF %d \r\n", vref);

					for (uint8_t i=0; i<4;i++){
						printf ("value slider: %d = %d \r\n", i, adcValues[i]);
					}

				}else if (theByte == 's'){
					panel1.stats(lodeStrTest);
					printf ("lets do this: %s ", lodeStrTest);

				}else if (theByte == 'a') {
					printf("Doing some action here. \r\n");
				}else{
					IOBoard testje;

					printf("No valid command detected: \r\n");

				}
			}

			GPIOD->BSRRL = GPIO_Pin_12;
			downTicker = 10;





			//printf ("ticker %d /r/n", ticker);



		}

		if (0 == downTicker)
		{
			GPIOD->BSRRH = GPIO_Pin_12;
		}

		blinkTheLED();

		//red ADC value
		//ConvertedValue = adc_convert();//Read the ADC converted value






	}

	return 0;
}
#ifdef __cplusplus
 extern "C" {
#endif
void init()
{
	/* STM32F4 discovery LEDs */
	//GPIO_InitTypeDef LED_Config;

	/* Always remember to turn on the peripheral clock...  If not, you may be up till 3am debugging... */
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	//LED_Config.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	//LED_Config.GPIO_Mode = GPIO_Mode_OUT;
	//LED_Config.GPIO_OType = GPIO_OType_PP;
	//LED_Config.GPIO_Speed = GPIO_Speed_25MHz;
	//LED_Config.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIO_Init(GPIOD, &LED_Config);




	/* GPIOD Periph clock enable */
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	  GPIO_InitTypeDef GPIO_InitStructure;
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	  //GPIO_Init(GPIOD, &GPIO_InitStructure);
	  GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Setup SysTick or CROD! */
	if (SysTick_Config(SystemCoreClock / 1000))
	{
		ColorfulRingOfDeath();
	}


	/* Setup USB */
	USBD_Init(&USB_OTG_dev,
	            USB_OTG_FS_CORE_ID,
	            &USR_desc,
	            &USBD_CDC_cb,
	            &USR_cb);

	return;
}

/*
 * Call this to indicate a failure.  Blinks the STM32F4 discovery LEDs
 * in sequence.  At 168Mhz, the blinking will be very fast - about 5 Hz.
 * Keep that in mind when debugging, knowing the clock speed might help
 * with debugging.
 */

void ColorfulRingOfDeath(void){
	while (1)
	{
	 GPIO_SetBits(GPIOA, GPIO_Pin_3);
	}
}
void blinkTheLED(void)
{
	uint16_t ring = 1;
	//while (1)
	//{
		//uint32_t count = 0;
		//while (count++ < 500000);

		//GPIOD->BSRRH = (ring << 12);
		//ring = ring << 1;
		//if (ring >= 1<<4)
		//{
	//		ring = 1;
	//	}
	//	GPIOD->BSRRL = (ring << 12);






	   GPIO_SetBits(GPIOA, GPIO_Pin_3);

	   /* Insert delay */
	   Delay(0x1FFFFF);

	   GPIO_ResetBits(GPIOA, GPIO_Pin_3);
	   Delay(0x1FFFFF);
	//}



}

/*
 * Interrupt Handlers
 */


void SysTick_Handler(void)
{
	ticker++;
	if (downTicker > 0)
	{
		downTicker--;
	}
}

void NMI_Handler(void)       {}
void HardFault_Handler(void) { ColorfulRingOfDeath(); }
void MemManage_Handler(void) { ColorfulRingOfDeath(); }
void BusFault_Handler(void)  { ColorfulRingOfDeath(); }
void UsageFault_Handler(void){ ColorfulRingOfDeath(); }
void SVC_Handler(void)       {}
void DebugMon_Handler(void)  {}
void PendSV_Handler(void)    {}

void OTG_FS_IRQHandler(void)
{
  USBD_OTG_ISR_Handler (&USB_OTG_dev);
}

void OTG_FS_WKUP_IRQHandler(void)
{
  if(USB_OTG_dev.cfg.low_power)
  {
    *(uint32_t *)(0xE000ED10) &= 0xFFFFFFF9 ;
    SystemInit();
    USB_OTG_UngateClock(&USB_OTG_dev);
  }
  EXTI_ClearITPendingBit(EXTI_Line18);
}



void adc_multiChannelConfigure(){

		//set pin PC0 as analog in
		RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOCEN,ENABLE);//Clock for the ADC port!! Do not forget about this one ;)
		GPIO_InitTypeDef GPIO_initStructre; //Structure for analog input pin
		//Analog pin configuration
		GPIO_initStructre.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3  ;//The channel 10 is connected to PC0
		GPIO_initStructre.GPIO_Mode = GPIO_Mode_AN; //The PC0 pin is configured in analog mode
		GPIO_initStructre.GPIO_PuPd = GPIO_PuPd_NOPULL; //We don't need any pull up or pull down
		GPIO_Init(GPIOC,&GPIO_initStructre);//Affecting the port with the initialization structure configuration

	/* Unchanged: Define ADC init structures */
	        ADC_InitTypeDef       ADC_InitStructure;
	        ADC_CommonInitTypeDef ADC_CommonInitStructure;
	        NVIC_InitTypeDef NVIC_InitStructure;

	        /* Unchanged: populate default values before use */
	        ADC_StructInit(&ADC_InitStructure);
	        ADC_CommonStructInit(&ADC_CommonInitStructure);

	        /* Unchanged: enable ADC peripheral */
	        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	        /* Unchanged: init ADC */
	        ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	        ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	        ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	        ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	        ADC_CommonInit(&ADC_CommonInitStructure);

	        /* Changed: Enabled scan mode conversion*/
	        ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	        ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	        ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	        ADC_InitStructure.ADC_DataAlign= ADC_DataAlign_Right;
	        ADC_InitStructure.ADC_ExternalTrigConv= 0;
	        ADC_InitStructure.ADC_ExternalTrigConvEdge= 0;
	        ADC_InitStructure.ADC_NbrOfConversion= 6;

	        ADC_Init(ADC1, &ADC_InitStructure);

	        /* Enable Vref & Temperature channel */
	        ADC_TempSensorVrefintCmd(ENABLE);

	        /* Configure channels */
	        /* Temp sensor */
	        ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_480Cycles);
	        /* VREF_int (2nd) */
	        ADC_RegularChannelConfig(ADC1, ADC_Channel_17, 2, ADC_SampleTime_480Cycles);

	        ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 3, ADC_SampleTime_480Cycles); ///PC0  //channel10
	        ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 4, ADC_SampleTime_480Cycles); ///PC0  //channel10
	        ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 5, ADC_SampleTime_480Cycles); ///PC0  //channel10
	        ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 6, ADC_SampleTime_480Cycles); ///PC0  //channel10

	        ADC_EOCOnEachRegularChannelCmd(ADC1, ENABLE);

	        /* Enable ADC interrupts */
	        ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

	        /* Configure NVIC */
	        NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
	        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	        NVIC_Init(&NVIC_InitStructure);

	        /* Enable ADC1 **************************************************************/
	        ADC_Cmd(ADC1, ENABLE);


}
void ADC_IRQHandler() {
        /* acknowledge interrupt */
        uint16_t value;
        ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);

        value = ADC_GetConversionValue(ADC1);
		switch (counter){
		   case 0:
				temp = value;
				counter++;
				break;
		   case 1:
				vref = value;
				counter++;
				break;

			//all the other channels.
		   default:
			   adcValues[counter - 2] = value;
				counter++;
				if (counter ==6){
					counter =0;
				}
			   break;
		}



}
#ifdef __cplusplus
 }
#endif

/*
void adc_configure(){
	ADC_InitTypeDef ADC_init_structure; //Structure for adc confguration
	GPIO_InitTypeDef GPIO_initStructre; //Structure for analog input pin

	//Clock configuration
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//The ADC1 is connected the APB2 peripheral bus thus we will use its clock source
	RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOCEN,ENABLE);//Clock for the ADC port!! Do not forget about this one ;)

	//Analog pin configuration
	GPIO_initStructre.GPIO_Pin = GPIO_Pin_0;//The channel 10 is connected to PC0
	GPIO_initStructre.GPIO_Mode = GPIO_Mode_AN; //The PC0 pin is configured in analog mode
	GPIO_initStructre.GPIO_PuPd = GPIO_PuPd_NOPULL; //We don't need any pull up or pull down
	GPIO_Init(GPIOC,&GPIO_initStructre);//Affecting the port with the initialization structure configuration

	//ADC structure configuration
	ADC_DeInit();
	ADC_init_structure.ADC_DataAlign = ADC_DataAlign_Right;//data converted will be shifted to right
	ADC_init_structure.ADC_Resolution = ADC_Resolution_12b;//Input voltage is converted into a 12bit number giving a maximum value of 4096
	ADC_init_structure.ADC_ContinuousConvMode = ENABLE; //the conversion is continuous, the input data is converted more than once
	ADC_init_structure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;// conversion is synchronous with TIM1 and CC1 (actually I'm not sure about this one :/)
	ADC_init_structure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//no trigger for conversion
	ADC_init_structure.ADC_NbrOfConversion = 1;//I think this one is clear :p
	ADC_init_structure.ADC_ScanConvMode = DISABLE;//The scan is configured in one channel
	ADC_Init(ADC1,&ADC_init_structure);//Initialize ADC with the previous configuration

	//Enable ADC conversion
	ADC_Cmd(ADC1,ENABLE);

	//Select the channel to be read from
	ADC_RegularChannelConfig(ADC1,ADC_Channel_10,1,ADC_SampleTime_144Cycles);
}







int adc_convert(){
	ADC_SoftwareStartConv(ADC1);//Start the conversion
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));//Processing the conversion
	return ADC_GetConversionValue(ADC1); //Return the converted data
}
*/