#include "PCAN_USB.h"
static inline void Serial_Init(const uint32_t BaudRate,const bool DoubleSpeed);
static inline void Serial_SendByte(const char DataByte);
static inline bool Serial_IsSendReady(void);
static inline bool Serial_IsSendComplete(void);
static inline int16_t Serial_ReceiveByte(void);
void PrintBuffer(const uint8_t* buffer, size_t size);
static inline bool Serial_IsCharReceived(void);
void PrintInt(uint16_t int_val);
void PrintIntDec(uint16_t int_val);
void UART_TransmitString(const char* String);
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
//конфигурация UART
Serial_Init(500000,false);

UART_TransmitString("PCAN-USB(clone)\r\n");
UART_TransmitString("---------------\r\n");
PrintIntDec(0xfedc);

UART_TransmitString("\r\n");

	}
////////////////////////////////////////////////////
bool token_77=false;
int main(void)
{
	_delay_ms(10);
    SetupHardware_();
    RingBuffer_InitBuffer_(&USBtoUSART_Buffer_, USBtoUSART_Buffer_Data_, sizeof(USBtoUSART_Buffer_Data_));
    RingBuffer_InitBuffer_(&USARTtoUSB_Buffer_, USARTtoUSB_Buffer_Data_, sizeof(USARTtoUSB_Buffer_Data_));
    sei();


for (;;)
{	
c0_:

	Endpoint_SelectEndpoint_(1);
	if(Endpoint_IsOUTReceived_())
	{
		uint8_t i=0;
		while(Endpoint_IsReadWriteAllowed_())//читаем пока не выгрузим весь буфер
		{
			USBtoUSART_Buffer_Data_[i++]=Endpoint_Read_8_();
		}
		Endpoint_ClearOUT_();//подтверждаем его прием
		UART_TransmitString("CANView DRIVER ");
		PrintBuffer(USBtoUSART_Buffer_Data_,16);
		UART_TransmitString(" \r\n");
		LED1_ALTERNATE;
	}
	if(memcmp(next_is_bulk_2, USBtoUSART_Buffer_Data_, 4)==0)
	{
		timer_led1(5000);
		goto c_1;//знак настройки точки 2 на прием	
	}
}


c_1:	
	Endpoint_ConfigureEndpoint_(0x2, 2,64, 1);
	Endpoint_SelectEndpoint_(2);
	USB_INT_Enable_(USB_INT_RXOUTI_);//включить прерывание OUT
	USB_INT_Enable_(USB_INT_TXINI_);//включить прерывание IN
	goto c0_;

	

}
//////////////////////////////////////////////////////////////////////////
uint16_t p_cont=0;
ISR(USB_COM_vect, ISR_BLOCK)
	{
	uint8_t PrevSelectedEndpoint = Endpoint_GetCurrentEndpoint_();

    Endpoint_SelectEndpoint_(2); 
	if (USB_INT_HasOccurred_(USB_INT_RXOUTI_)&&USB_INT_IsEnabled_(USB_INT_RXOUTI_)) 
	{
		uint8_t i=0;
		while(Endpoint_IsReadWriteAllowed_())//читаем пока не выгрузим весь буфер
		{
			USBtoUSART_Buffer_Data_[i++]=Endpoint_Read_8_();
		}
		Endpoint_ClearOUT_();//подтверждаем его прием
		uint8_t size_packet=13;
		if(USBtoUSART_Buffer_Data_[2]==0x28)size_packet=15;
		UART_TransmitString("CANView ");
		PrintBuffer(USBtoUSART_Buffer_Data_,size_packet);
		UART_TransmitString("       ");
		PrintIntDec(++p_cont);
		UART_TransmitString(" \r\n");
		LED1_ALTERNATE;
	}
	
	
	if (USB_INT_HasOccurred_(USB_INT_TXINI_)&&USB_INT_IsEnabled_(USB_INT_TXINI_)) 
	{
		UEINTX &= ~(1 << TXINI);
	}


	Endpoint_SelectEndpoint_(ENDPOINT_CONTROLEP_);
	USB_INT_Disable_(USB_INT_RXSTPI_);
	GlobalInterruptEnable_();
	USB_Device_ProcessControlRequest_();
	Endpoint_SelectEndpoint_(ENDPOINT_CONTROLEP_);
	USB_INT_Enable_(USB_INT_RXSTPI_);
	Endpoint_SelectEndpoint_(PrevSelectedEndpoint);
	return;
	}

ISR(USB_GEN_vect, ISR_BLOCK)
	{
	
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

