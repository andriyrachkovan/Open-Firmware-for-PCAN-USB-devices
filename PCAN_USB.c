#include "PCAN_USB.h"
////////////////////////////////////////////////////	
////////////////////////////////////////////////////

///////////////////////////////////////////////////
////////////////////////////////////////////////////	
////////////////////////////////////////////////////	
void SetupHardware_(void)
	{
	MCUSR &= ~(1 << WDRF);
	wdt_disable();
	cli();
	 CLKPR = (1 << CLKPCE);
     CLKPR = 0;
	sei();
	USB_Init_();
	TCCR0B = (1 << CS02);
	AVR__DDR_PORT  |= 0x30;
	LEDS_OFF;
	}
////////////////////////////////////////////////////	
////////////////////////////////////////////////////	
////////////////////////////////////////////////////

int main(void)
{
	_delay_ms(10);
    SetupHardware_();
    RingBuffer_InitBuffer_(&USBtoUSART_Buffer_, USBtoUSART_Buffer_Data_, sizeof(USBtoUSART_Buffer_Data_));
    RingBuffer_InitBuffer_(&USARTtoUSB_Buffer_, USARTtoUSB_Buffer_Data_, sizeof(USARTtoUSB_Buffer_Data_));
    sei();
uint8_t i;
//wdt_enable(WDTO_500MS);
for (;;)
{
	wdt_reset();
	if(!step_Vendor)goto end_cycle;
	Endpoint_SelectEndpoint_(1);
	if(step_out1)goto end_out1;
	while(!Endpoint_IsOUTReceived_())Endpoint_SelectEndpoint_(1);//ждем OUT пакет
	while(!Endpoint_IsReadWriteAllowed_());//ждем готовность буфера
	i=0;
	while(Endpoint_IsReadWriteAllowed_())//читаем пока не заполним весь буфер
		{
			USBtoUSART_Buffer_Data_[i++]=Endpoint_Read_8_();
		}
	Endpoint_ClearOUT_();//подтверждаем его прием
	if((USBtoUSART_Buffer_Data_[0]==9)&&(USBtoUSART_Buffer_Data_[1]==1))//знак конца пакетов OUT и перемены направления
	{
		step_out1=true;
		UECFG0X|=1;//простое изменение направления с OUT на IN
		step_in1=false;
	}
	if((USBtoUSART_Buffer_Data_[0]==9)
		&&(USBtoUSART_Buffer_Data_[1]==1)	
		&&(USBtoUSART_Buffer_Data_[2]==0)
		&&(USBtoUSART_Buffer_Data_[3]==0))//знак конца пакетов OUT и ответа 9
	{
		USBtoUSART_Buffer_Data_[3]=9;
	}
	if((USBtoUSART_Buffer_Data_[0]==9)
		&&(USBtoUSART_Buffer_Data_[1]==2)
		&&(USBtoUSART_Buffer_Data_[2]==0)
		&&(USBtoUSART_Buffer_Data_[3]==1)
		) //знак конца общения с драйвером
	{
		//wdt_disable();
		goto end_cycle;
	}	
end_out1:	
	if(step_in1)goto end_cycle;
	while(!Endpoint_IsINReady_());//ждем пакета IN1
	while(!Endpoint_IsReadWriteAllowed_());//ждем готовность буфера
	i=0;
	while(Endpoint_IsReadWriteAllowed_())
	{
		Endpoint_Write_8_(USBtoUSART_Buffer_Data_[i++]);//заполняем буфер данными	
	}
	Endpoint_ClearIN_();//подтверждаем заполнение
	while (!(UEINTX &0b10000001));//убедимся что буфер сброшен
	step_in1=true;
	UECFG0X&=0b11111110;//простое изменение направления с IN на OUT
	step_out1=false;
end_cycle:	asm("nop");
}



}
/////////////////////////////////////////	
//UERST – USB Endpoint Reset Register
			//-  -    -   EPRST D4   EPRST D3    EPRST D2    EPRST D1     EPRST D0
//UECONX – USB Endpoint Control Register
			//-	 -	 STALLRQ	 STALLRQC	 RSTDT	 -	 -	 EPEN
			
//UECFG0X – USB Endpoint Configuration 0 Register
			//EPTYPE1:0	 -	 -	 -	 -	 -	 EPDIR
			
//UENUM – USB Endpoint Number Register
//UEBCLX – USB Endpoint Byte Count Register 
//UEDATX – USB Data Endpoint Register 
//UEIENX – USB Endpoint Interrupt Enable Register
			//FLERRE	NAKINE 		_ 		NAKOUTE 	RXSTPE	 	RXOUTE	 	STALLEDE	 TXINE

//UEINTX – USB Endpoint Interrupt Register
			//FIFOCON	 NAKINI	 RWAL	 NAKOUTI	 RXSTPI	 RXOUTI	 STALLEDI 	TXINI
			

////////////////////////////////////////////////////	
////////////////////////////////////////////////////	
////////////////////////////////////////////////////	


ISR(USB_COM_vect, ISR_BLOCK)
	{
	uint8_t PrevSelectedEndpoint = Endpoint_GetCurrentEndpoint_();
	Endpoint_SelectEndpoint_(ENDPOINT_CONTROLEP_);
	USB_INT_Disable_(USB_INT_RXSTPI_);
	GlobalInterruptEnable_();
	USB_Device_ProcessControlRequest_();
	Endpoint_SelectEndpoint_(ENDPOINT_CONTROLEP_);
	USB_INT_Enable_(USB_INT_RXSTPI_);
	Endpoint_SelectEndpoint_(PrevSelectedEndpoint);
	return;
	}

ISR(USART1_RX_vect, ISR_BLOCK)
	{
		return;
	uint8_t ReceivedByte = UDR1;
	if ((USB_DeviceState_ == DEVICE_STATE_Configured_) && !(RingBuffer_IsFull_(&USARTtoUSB_Buffer_)))
	  RingBuffer_Insert_(&USARTtoUSB_Buffer_, ReceivedByte);	
	}
	
ISR(USB_GEN_vect, ISR_BLOCK)
	{
		
		
		
	if (USB_INT_HasOccurred_(USB_INT_SOFI_) && USB_INT_IsEnabled_(USB_INT_SOFI_))
		{
		USB_INT_Clear_(USB_INT_SOFI_);
		
		}


	if (USB_INT_HasOccurred_(USB_INT_SUSPI_) && USB_INT_IsEnabled_(USB_INT_SUSPI_))
		{
		USB_INT_Disable_(USB_INT_SUSPI_);
		USB_INT_Enable_(USB_INT_WAKEUPI_);
		USB_CLK_Freeze_();
		if (!(USB_Options_ & USB_OPT_MANUAL_PLL_))USB_PLL_Off_();
		USB_DeviceState_ = DEVICE_STATE_Unattached_;
		}
	if (USB_INT_HasOccurred_(USB_INT_WAKEUPI_) && USB_INT_IsEnabled_(USB_INT_WAKEUPI_))
		{
		if (!(USB_Options_ & USB_OPT_MANUAL_PLL_))
			{
			USB_PLL_On_();
			while (!(USB_PLL_IsReady_()));
			}
		USB_CLK_Unfreeze_();
		USB_INT_Clear_(USB_INT_WAKEUPI_);
		USB_INT_Disable_(USB_INT_WAKEUPI_);
		USB_INT_Enable_(USB_INT_SUSPI_);
		}
	if (USB_INT_HasOccurred_(USB_INT_EORSTI_) && USB_INT_IsEnabled_(USB_INT_EORSTI_))
		{
//////////////		
		USB_INT_Clear_(USB_INT_EORSTI_);
		USB_DeviceState_                = DEVICE_STATE_Default_;
		USB_INT_Clear_(USB_INT_SUSPI_);
		USB_INT_Disable_(USB_INT_SUSPI_);
		USB_INT_Enable_(USB_INT_WAKEUPI_);
		Endpoint_ConfigureEndpoint_(ENDPOINT_CONTROLEP_, EP_TYPE_CONTROL_,
		                           USB_Device_ControlEndpointSize_, 1);
		USB_INT_Enable_(USB_INT_RXSTPI_);
		//USB_INT_Enable_(USB_INT_SOFI_);
		}

	}
