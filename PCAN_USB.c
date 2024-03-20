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
				
				

//cli();
//				answer_timer[0]=total_micros_DIV16>>8;
//				answer_timer[1]=total_micros_DIV16>>0;//состояние пакета времени
//sei();
				
			i_ring_hex&=0b111;
			answer_timer[0]=ring_hex[i_ring_hex-1]>>8;//старшее виртуальное время
			answer_timer[1]=ring_hex[i_ring_hex-1];//младшее  виртуальное время из ринг массива

				uint8_t d1=0;
				uint8_t d2=0;
				uint8_t d3=0;
				uint8_t d4=0;
				uint8_t d5=0;
				uint8_t d6=0;
				uint8_t d7=0;
				uint8_t d8=0;
				
				UEDATX=DLC;
				UEDATX=ID_standard[0];
				UEDATX=ID_standard[1];

				UEDATX=answer_timer[0];
				UEDATX=answer_timer[1];

				++num_packet;
				UEDATX=0;
				UEDATX=0;
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
				UDIEN  |= (1 << SOFE);//активируем кадр
				goto end_isr;
			}
			if(step==3)
			{
				step=0;	
				i_ring_hex=0;
				UEINTX =CLEAR_OUT;
				goto end_isr;
			}
		}
		if(memcmp(arrivederci_, USBtoUSART_Buffer_Data_, 4)==0)
		{
			step=0;	
			i_ring_hex=0;
			LED2_OFF;
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
//	total_micros=total_micros+16;
	++total_micros_DIV16;
	asm("nop");
	if(++microbloks==100)
	{
		microbloks=0;
	}
	asm("nop");
}

ISR(TIMER1_COMPA_vect,ISR_BLOCK) 
{
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
			frame_number=(UDFNUMH<<8)|(UDFNUML);
			first_frame();//нулевая метка
			
				if(++frames==1000)
				{
				frames=0;
				LED2_ALTERNATE;
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
	uint8_t tmpUENUM=UENUM;
	UENUM=2;
	if(step==3)
		{
			for(i=0;i<5;i++)
				UEDATX=USBtoUSART_Buffer_Data_[i];//шапка пакета метки


			i_ring_hex&=0b111;
			UEDATX=ring_hex[i_ring_hex]>>8;//старшее виртуальное время
			UEDATX=ring_hex[i_ring_hex];//младшее  виртуальное время из ринг массива
			i_ring_hex++;

			UEDATX=0x40;//состояние пакета времени
			UEDATX=0x01;//состояние пакета времени
			for(i=0;i<55;i++)
				UEDATX=0;

			
			UEINTX&=CLEAR_IN;
			UEIENX|=1;
		}
	UENUM=tmpUENUM;
}

void first_frame()
{
	uint8_t tmpUENUM=UENUM;
	UENUM=2;
	if(step==1)//нулевая метка
		{
			prox_frame_number=(frame_number+1000)&0x7FF;
			step=2;
			for(i=0;i<sizeof(frst_sms_);i++)
				UEDATX=frst_sms_[i];
			UEDATX=(total_micros_DIV16>>8)&0xFF;
			UEDATX=total_micros_DIV16&0xFF;
			for(i=0;i<(64-sizeof(frst_sms_)-2);i++)
				UEDATX=0;
			UEINTX&=CLEAR_IN;
			UEIENX|=1;
		}
	if(step==2)
		{
			step=3;
			for(i=0;i<5;i++)//первый елемент счетчика
				USBtoUSART_Buffer_Data_[i]=pgm_read_byte(&sms_1[i]);
		}
	UENUM=tmpUENUM;
}