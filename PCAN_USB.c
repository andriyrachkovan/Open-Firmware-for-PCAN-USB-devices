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

	timer0_init();
	AVR__DDR_PORT  |= 0x30;
	LEDS_OFF;
	}
////////////////////////////////////////////////////
int i;
uint16_t num_packet=0;

int main(void)
{
    SetupHardware_();
    sei();



for (;;)
{
		UENUM=2;
			

		if(
		data_token_enabled&&
		1
			)
			{
				data_token_enabled=false;
				LED1_ALTERNATE;

				while(!Endpoint_IsReadWriteAllowed_());

				for(i=0;i<sizeof(answer_standard_head);i++)
					UEDATX=pgm_read_byte(&answer_standard_head[i]);
				DLC=0x8;
				ID_standard[0]=0;
				ID_standard[1]=0;
				
				

				

				uint8_t d1=1;
				uint8_t d2=2;
				uint8_t d3=3;
				uint8_t d4=4;
				uint8_t d5=5;
				uint8_t d6=6;
				uint8_t d7=7;
				uint8_t d8=8;
				
				UEDATX=DLC;
				UEDATX=0x60;//ID_standard[0];
				UEDATX=0x24;//ID_standard[1];id=0x123

cli();
			UEDATX=time_stamp>>8;
			UEDATX=time_stamp;
sei();

				++num_packet;
				UEDATX=num_packet>>8;//d1;
				UEDATX=num_packet;//d2;
				UEDATX=d3;
				UEDATX=d4;
				UEDATX=d5;
				UEDATX=d6;
				UEDATX=d7;
				UEDATX=d8;
				
				

				for(i=0;i<49;i++)
					UEDATX=0;//дополняем до 64
				UEINTX&=CLEAR_IN;//подтверждаем
				while(UEINTX&0b10000001);//ждем опустошения
				UEIENX|=1;//активируем прерывание
				
			}

	
}
}
//////////////////////////////////////////////////////////////////////////
ISR(USB_COM_vect, ISR_BLOCK)
{

	uint8_t PrevSelectedEndpoint = UENUM;
	uint8_t i=0;
	UENUM=2;
	if(UEINTX&UEIENX&IN_MASK)
	{
		UEINTX&=CLEAR_IN;//подтверждаем
		if(step==1)step=2;
		while(UEINTX&0b10000001);//ждем опустошения
		UEIENX&=0b11111110;//выключаем прерывание
		if(time_token_loaded)
		{
			time_token_loaded=false;
			data_token_enabled=true;
		}
	}
	
	if(UEINTX&UEIENX&OUT_MASK)
	{
		i=0;
		while((UEINTX & (1 << RWAL)))//читаем пока не выгрузим весь буфер
		{
			USBtoUSART_Buffer_Data_[i++]=UEDATX;
		}
		UEINTX=CLEAR_OUT;
		goto end_isr;
	}
	UENUM=1;
	if(UEINTX&UEIENX&OUT_MASK)
	{
		i=0;
		while((UEINTX & (1 << RWAL)))//читаем пока не выгрузим весь буфер
		{
			USBtoUSART_Buffer_Data_[i++]=UEDATX;
		}
		UEINTX =CLEAR_OUT;
		char ciao_[]={0x3,0x2,0x1,0x0};//начало программы
		char arrivederci_[]={0x9,0x2,0x0,0x1};//завершение программы
		
		
		if(memcmp(ciao_, USBtoUSART_Buffer_Data_, 4)==0)
		{
			if(step==0)//приглашение программы
			{
				UEINTX =CLEAR_OUT;
				
				step=1;//переходим к 0 метке
				//ЖДЕМ 30 МС
				uint32_t wait_30;
				for(wait_30=0;wait_30<68485;wait_30++)asm("nop");
				//ПЕРЕДАЕМ ПЕРВУЮ МЕТКУ
				uint8_t tmpUENUM=UENUM;
				UENUM=2;
				for(i=0;i<sizeof(frst_sms_);i++)
					UEDATX=frst_sms_[i];
				UEDATX=time_stamp>>8;
				UEDATX=time_stamp;
				for(i=0;i<(64-sizeof(frst_sms_)-2);i++)
					UEDATX=0;
				UEINTX&=CLEAR_IN;
				UEIENX|=1;
				UENUM=tmpUENUM;
				//ПОДГОТАВЛИВАЕМ ПОСЛЕДУЮЩИЕ МЕТКИ
				for(i=0;i<5;i++)//первый елемент счетчика
					USBtoUSART_Buffer_Data_[i]=pgm_read_byte(&sms_1[i]);

			
				//АКТИВИРУЕМ КАДР
				UDIEN  |= (1 << SOFE);//активируем кадр
				goto end_isr;
			}
			if(step==3)
			{
				step=0;	
				UEINTX =CLEAR_OUT;
				goto end_isr;
			}
		}
		if(memcmp(arrivederci_, USBtoUSART_Buffer_Data_, 4)==0)
		{
			step=0;	
			frames=0;
			LED2_OFF;
			UDIEN  &= ~(1 << SOFE);//дизактивируем кадр

			UEINTX =CLEAR_OUT;
			goto end_isr;
		}
	}



	UENUM=0;
	UEIENX &= ~(1 << RXSTPE);
	SREG|=0x80;
	USB_Device_ProcessControlRequest_();
	UENUM=0;
	UEIENX |= (1 << RXSTPE);
end_isr:
	UENUM=PrevSelectedEndpoint;
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//проверено
ISR(TIMER0_COMPA_vect,ISR_BLOCK) 
{
}

ISR(TIMER1_COMPA_vect,ISR_BLOCK) 
{
add_tik = (add_tik == 19) ? 13 : 19;
time_stamp-=add_tik;
}





ISR(USB_GEN_vect, ISR_BLOCK)
{
	if ((UDINT  & (1 << EORSMI)) && (UDIEN  & (1 << EORSME)))
	{
		UDINT  &= ~(1 << EORSMI);

	}
	if ((UDINT  & (1 << SOFI)) && (UDIEN  & (1 << SOFE)))// начало кадра
	{
		UDINT  &= ~(1 << SOFI);//подтверждаем кадр
		{

//ПЕРЕДАЕМ ПЕРВЫЙ РАЗ ПРАВИЛЬНУЮ МЕТКУ
			if(UENUM==2&&step==2&&(++frames==1000))
			{
			step=3;		
			
			frames=0;
			time_stamp=0xC8ED;//ЗАДАЕМ НАЧАЛЬНУЮ ПРАВИЛЬНУЮ МЕТКУ
			uint16_t tst=time_stamp; 
			if(!(tst%2))tst+=13;
			
			for(i=0;i<5;i++)
					UEDATX=USBtoUSART_Buffer_Data_[i];//шапка пакета метки
			UEDATX=tst>>8;
			UEDATX=tst;
			UEDATX=0x40;//размер пакета времени
			UEDATX=0x01;//ошибки
			for(i=0;i<55;i++)
				UEDATX=0;
			UEINTX&=CLEAR_IN;
			UEIENX|=1;
			}
			 
			if(UENUM==2&&step==3&&(++frames==1000))
			{
				frames=0;
				process_frames();
				time_token_loaded=true;
			}
		}
		

	}

	
	if ((UDINT  & (1 << SUSPI)) && (UDIEN  & (1 << EORSME)))
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
		UDIEN  |= (1 << EORSME);
		Endpoint_ConfigureEndpoint_(0, 0,16, 1);
		UEIENX |= (1 << RXSTPE);
	}
}
//////////////////////////////////////////////////////////////////



process_frames()
{
			for(i=0;i<5;i++)
				UEDATX=USBtoUSART_Buffer_Data_[i];//шапка пакета метки
			uint16_t tst=time_stamp; 
			if(!(tst%2))tst+=13;

			UEDATX=tst>>8;
			UEDATX=tst;


			UEDATX=0x40;//размер пакета времени
			UEDATX=0x01;//ошибки
			for(i=0;i<55;i++)
				UEDATX=0;

			
			UEINTX&=CLEAR_IN;
			UEIENX|=1;
			step=3;
		
}

