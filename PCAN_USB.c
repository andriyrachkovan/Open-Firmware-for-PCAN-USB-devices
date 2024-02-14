#include "PCAN_USB.h"
////////////////////////////////////////////////////	
////////////////////////////////////////////////////
void Endpoint_Write_8_(const uint8_t);
uint8_t Endpoint_Read_8_(void);
uint16_t Endpoint_BytesInEndpoint_(void);
void Endpoint_ClearIN_(void);
bool Endpoint_IsStalled_(void);
bool Endpoint_IsEnabled_(void);
bool Serial_IsSendReady_(void);
void Endpoint_ClearOUT_(void);
bool Endpoint_IsConfigured_(void);
void Endpoint_DisableEndpoint_(void);
void USB_Controller_Reset_(void);
bool Endpoint_IsINReady_(void);
bool Endpoint_IsSETUPReceived_(void);
bool Endpoint_IsOUTReceived_(void);
bool Endpoint_IsINReceived_(void);
bool Endpoint_IsReadWriteAllowed_(void);
void Endpoint_ResetDataToggle_(void);
void Endpoint_StallTransaction_(void);
void Endpoint_ClearStall_(void);
void Endpoint_SelectEndpoint_(const uint8_t);
void Endpoint_ResetEndpoint_(const uint8_t);
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
bool conf_OUT=false;
bool conf_IN=false;
bool conf_START=false;
bool step_config_bulk_82=false;
bool step_config_bulk_1=false;
uint8_t current_Endpoint=0xFF;
int main(void)
{
    SetupHardware_();
    RingBuffer_InitBuffer_(&USBtoUSART_Buffer_, USBtoUSART_Buffer_Data_, sizeof(USBtoUSART_Buffer_Data_));
    RingBuffer_InitBuffer_(&USARTtoUSB_Buffer_, USARTtoUSB_Buffer_Data_, sizeof(USARTtoUSB_Buffer_Data_));
    sei();
uint8_t i;
for(i=0;i<16;i++)USBtoUSART_Buffer_Data_[i]=0xFF;//зачищаем наш приемник



bool step_out1=false;
bool step_out2=false;
bool step_in1=false;
bool step_out3=false;
bool step_in2=false;
bool step_out4=false;

for (;;)
{
	if(!step_Vendor)goto end_cycle;
	Endpoint_SelectEndpoint_(1);
	if(step_out1)goto end_out1;
	while(!Endpoint_IsOUTReceived_());//ждем OUT пакет
	while(!Endpoint_IsReadWriteAllowed_());//ждем готовность буфера
	i=0;
	while(Endpoint_IsReadWriteAllowed_())//читаем пока не заполним весь буфер
		{
			USBtoUSART_Buffer_Data_[i++]=Endpoint_Read_8_();
		}
	Endpoint_ClearOUT_();//подтверждаем его прием
	step_out1=true;
	for(i=0;i<16;i++)USBtoUSART_Buffer_Data_[i]=0xFF;//зачищаем наш приемник
end_out1:	
	if(step_out2)goto end_out2;
	while(!Endpoint_IsOUTReceived_());//ждем OUT пакет
	while(!Endpoint_IsReadWriteAllowed_());//ждем готовность буфера
	i=0;
	while(Endpoint_IsReadWriteAllowed_())//читаем пока не заполним весь буфер
		{
			USBtoUSART_Buffer_Data_[i++]=Endpoint_Read_8_();
		}
	Endpoint_ClearOUT_();//подтверждаем его прием
	step_out2=true;
	USBtoUSART_Buffer_Data_[3]=9;//теперь можно подготовить первый ответ
	UECFG0X|=1;//простое изменение направления с OUT на IN
end_out2:	
	if(step_in1)goto end_in1;
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

end_in1:	
	if(step_out3)goto end_out3;
	while(!Endpoint_IsOUTReceived_());//снова ждем OUT пакет
	while(!Endpoint_IsReadWriteAllowed_());//ждем готовность буфера
	for(i=0;i<16;i++)USBtoUSART_Buffer_Data_[i]=0xFF;//зачищаем наш приемник
	i=0;
	while(Endpoint_IsReadWriteAllowed_())//читаем пока не заполним весь буфер
		{
			USBtoUSART_Buffer_Data_[i++]=Endpoint_Read_8_();
		}
	Endpoint_ClearOUT_();//подтверждаем его прием
	if(USBtoUSART_Buffer_Data_[1]==1)//знак конца пакетов OUT и перемены направления
	{
		step_out3=true;	
		UECFG0X|=1;//простое изменение направления с OUT на IN
	}
end_out3:
	if(step_in2)goto end_in2;
	while(!Endpoint_IsINReady_());//ждем пакета IN2
	while(!Endpoint_IsReadWriteAllowed_());//ждем готовность буфера
	i=0;
	while(Endpoint_IsReadWriteAllowed_())
	{
		Endpoint_Write_8_(USBtoUSART_Buffer_Data_[i++]);//заполняем буфер данными	
	}
	Endpoint_ClearIN_();//подтверждаем заполнение
	while (!(UEINTX &0b10000001));//убедимся что буфер сброшен
	step_in2=true;
	UECFG0X&=0b11111110;//простое изменение направления с IN на OUT
end_in2:
	if(step_out4)goto end_out4;
	while(!Endpoint_IsOUTReceived_());//снова ждем OUT пакет
	while(!Endpoint_IsReadWriteAllowed_());//ждем готовность буфера
	for(i=0;i<16;i++)USBtoUSART_Buffer_Data_[i]=0xFF;//зачищаем наш приемник
	i=0;
	while(Endpoint_IsReadWriteAllowed_())//читаем пока не заполним весь буфер
		{
			USBtoUSART_Buffer_Data_[i++]=Endpoint_Read_8_();
		}
	Endpoint_ClearOUT_();//подтверждаем его прием
	if(USBtoUSART_Buffer_Data_[1]==1)//знак конца пакетов OUT и перемены направления
	{
		step_in2=false;
//		step_out4=true;	
		UECFG0X|=1;//простое изменение направления с OUT на IN
	}
    LEDS_ON;

end_out4:	
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
		USB_INT_Clear_(USB_INT_EORSTI_);
		USB_DeviceState_                = DEVICE_STATE_Default_;
		USB_INT_Clear_(USB_INT_SUSPI_);
		USB_INT_Disable_(USB_INT_SUSPI_);
		USB_INT_Enable_(USB_INT_WAKEUPI_);
		Endpoint_ConfigureEndpoint_(ENDPOINT_CONTROLEP_, EP_TYPE_CONTROL_,
		                           USB_Device_ControlEndpointSize_, 1);
		USB_INT_Enable_(USB_INT_RXSTPI_);
		}

	}
