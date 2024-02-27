#include "PCAN_USB.h"
void SetupHardware_(void)
	{
		cli();
		wdt_disable();
		CLKPR = (1 << CLKPCE);
		CLKPR = 0;
		sei();
		

	REGCR  &= ~(1 << REGDIS);
	UDIEN   = 0;
	UDINT  = 0;
	USBCON &= ~(1 << USBE);
	USBCON |=  (1 << USBE);
	USBCON &= ~(1 << FRZCLK);

	PLLCSR = ((1 << PLLP0) | (1 << PLLE));
	while (!(USB_PLL_IsReady_()));
	
	
	Endpoint_ConfigureEndpoint_(0, 0, 16, 1);
	
	UDINT  &= ~(1 << SUSPI);
	UDIEN  |= (1 << SUSPE);
	UDIEN  |= (1 << EORSTE);
	UDCON  &= ~(1 << DETACH);

	TCCR0B=(1 << CS02);

//	Serial_Init(1000000,false);
	AVR__DDR_PORT  |= 0x30;
	LEDS_OFF;
	}
////////////////////////////////////////////////////
int main(void)
{
    SetupHardware_();
    sei();

bool timer_is_acive=false;
uint32_t timer_EP2=0;	
for (;;)
{
	if(timer_is_acive)
		if(timer_EP2++>500000)
		{
			timer_EP2=0;
			UENUM=2;
			UEIENX =1;//заново активируем прерывание IN
			UECFG0X|=1;//направление IN
		}
	UENUM=1;
	if(Endpoint_IsOUTReceived_())
	{
		uint8_t i=0;
		while(Endpoint_IsReadWriteAllowed_())//читаем пока не выгрузим весь буфер
		{
			USBtoUSART_Buffer_Data_[i++]=UEDATX;
		}
		UEINTX &= ~((1 << RXOUTI) | (1 << FIFOCON));
	}
	char t_[]={0x3,0x2,0x1,0x0};//начало балк 2
	if(memcmp(t_, USBtoUSART_Buffer_Data_, 4)==0)
	{
		UENUM=2;
		if(!timer_is_acive)
		{
			UEIENX =1;	//активация прерывания IN
			UECFG0X|=1;//направление IN
		}
		timer_is_acive=true;
	}

}
}
//////////////////////////////////////////////////////////////////////////
ISR(USB_COM_vect, ISR_BLOCK)
	{
	uint8_t PrevSelectedEndpoint = UENUM;
    UENUM=2; 

	if ((UEINTX & (1 << TXINI))&&(UEIENX & (1 << TXINE))) 
	{
		LED2_ALTERNATE;
		UEDATX=8;
		UEINTX &= ~((1 << TXINI) | (1 << FIFOCON));
		while (!(UEINTX &0b10000001));//убедимся что буфер сброшен
			UEIENX =0b100;	
			UECFG0X&=0b11111110;//направление OUT
	}

	if ((UEINTX & (1 << RXOUTI))&&(UEIENX & (1 << RXOUTE))) 
	{
		LED1_ALTERNATE;
		uint8_t i=0;
		while((UEINTX & (1 << RWAL)))//читаем пока не выгрузим весь буфер
		{
			USBtoUSART_Buffer_Data_[i++]=UEDATX;
		}
		UEINTX &= ~((1 << RXOUTI) | (1 << FIFOCON));
		UEINTX &= ~(1 << RXOUTI);
	}
	


	UENUM=0;
	UEIENX &= ~(1 << RXSTPE);
	SREG|=0x80;
	USB_Device_ProcessControlRequest_();
	UENUM=0;
	UEIENX |= (1 << RXSTPE);
	UENUM=PrevSelectedEndpoint;
	}

ISR(USB_GEN_vect, ISR_BLOCK)
	{
	
	if ((UDINT  & (1 << SUSPI)) && (UDIEN  & (1 << SUSPE)))
		{
		UDIEN  &= ~(1 << SUSPE);
		UDIEN  |= (1 << WAKEUPE);
		USBCON |=  (1 << FRZCLK);
		
		PLLCSR = 0;
		USB_DeviceState_ = DEVICE_STATE_Unattached_;
		}
	if ((UDINT  & (1 << WAKEUPI)) && (UDIEN  & (1 << WAKEUPE)))
		{
		if (!(USB_Options_ & USB_OPT_MANUAL_PLL_))
			{
				PLLCSR = (1 << PLLP0);
				PLLCSR = ((1 << PLLP0) | (1 << PLLE));
				while (!(PLLCSR & (1 << PLOCK)));
			}
		USBCON &= ~(1 << FRZCLK);
		UDINT  &= ~(1 << WAKEUPI);
		UDIEN  &= ~(1 << WAKEUPE);
		UDIEN  |= (1 << SUSPE);
		}

	if ((UDINT  & (1 << EORSTI)) && (UDIEN  & (1 << EORSTE)))
		{
		UDINT  &= ~(1 << EORSTI);
		USB_DeviceState_ = 2;
		UDINT  &= ~(1 << SUSPI);
		UDIEN  &= ~(1 << SUSPE);
		UDIEN  |= (1 << WAKEUPE);
		Endpoint_ConfigureEndpoint_(0, 0,16, 1);
		UEIENX |= (1 << RXSTPE);
		}
	}

ISR(USART1_TX_vect, ISR_BLOCK) 
{
		if(ring_buffer[U_ring]!=0x0)
		{
			UDR1=ring_buffer[U_ring];
			U_ring++;
		}
}

ISR(USART1_UDRE_vect ISR_BLOCK)
	{
		if(timer_LED1++==10000)
		{
			timer_LED1=0;		
			//LED1_ALTERNATE;
			if(ring_buffer[U_ring]!=0x0)
			{
				UDR1=ring_buffer[U_ring];
				U_ring++;
			}
		}
	}

