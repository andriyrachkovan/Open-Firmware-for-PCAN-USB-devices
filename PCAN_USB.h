#include <avr/io.h>		
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include <string.h>
#include <avr/wdt.h>
#define F_CPU         16000000UL
#include <util/delay.h>

#define CLEAR_OUT 0b01111011
#define CLEAR_IN 0b01111110
#define OUT_MASK 0b00000100 
#define IN_MASK 0b00000001 
#define IDtoID(x,y) (((((y&0x0F)<<4)|(x>>4))<<8)|(((x&0x0F)<<4)|((y>>4)&0xF)))<<1	
#define SECuNDa 1000000
#define AVR__DDR_PORT DDRD
#define AVR_LINE_PORT PORTD
#define LEDS_ON AVR_LINE_PORT  &=0xCF
#define LEDS_OFF AVR_LINE_PORT |=0x30
#define LED2_ON AVR_LINE_PORT  &=0xEF
#define LED1_ON AVR_LINE_PORT  &=0xDF
#define LED2_OFF AVR_LINE_PORT |=0x10
#define LED1_OFF AVR_LINE_PORT |=0x20
#define LEDS_ALTERNATE PORTD ^= (1 << PD4) | (1 << PD5)
#define LED1_ALTERNATE PORTD ^= (1 << PD5)
#define LED2_ALTERNATE PORTD ^= (1 << PD4)
#define MIN_(x, y)               (((x) < (y)) ? (x) : (y))
#define USB_DeviceState_            CONCAT_EXPANDED_(GPIOR, 0)
#define CONCAT_(x, y)            x ## y
#define CONCAT_EXPANDED_(x, y)   CONCAT_(x, y)
#define ENDPOINT_TOTAL_ENDPOINTS_        5
#define USB_OPT_MANUAL_PLL_                 (1 << 2)
#define USB_Options_ (USB_DEVICE_OPT_FULLSPEED_|USB_OPT_REG_ENABLED_|USB_OPT_AUTO_PLL_)
#define USB_DEVICE_OPT_FULLSPEED_               (0 << 0)
#define USB_OPT_REG_ENABLED_                (0 << 1)
#define USB_OPT_AUTO_PLL_                   (0 << 2)
#define SERIAL_2X_UBBRVAL(Baud) ((((F_CPU / 8) + (Baud / 2)) / (Baud)) - 1)
#define SERIAL_UBBRVAL(Baud)    ((((F_CPU / 16) + (Baud / 2)) / (Baud)) - 1)
#define  TEMPLATE_BUFFER_OFFSET_(Length)            0
#define  TEMPLATE_BUFFER_MOVE_(BufferPtr, Amount)   BufferPtr += Amount
#define  TEMPLATE_TRANSFER_BYTE_(BufferPtr)         Endpoint_Write_8_(pgm_read_byte(BufferPtr))

#include "Descriptors.h"


void Endpoint_ConfigureEndpoint_(const uint8_t Address,const uint8_t Type,const uint16_t Size,const uint8_t Banks);

void PrintInt(uint16_t int_val);
void PrintIntDec(uint16_t int_val);
void Serial_SendByte_ring(char data);
void RING_TransmitString(const char* String_);
void USB_Device_SetDeviceAddress_(const uint8_t Address);
void USB_Device_EnableDeviceAddress_(const uint8_t Address);
void USB_Device_SetAddress_(void);
void USB_Device_GetDescriptor_(void);
void USB_ResetInterface_(void);



void Serial_Init(const uint32_t BaudRate,const bool DoubleSpeed);
void Serial_SendByte(const char DataByte);
bool Serial_IsSendReady(void);
void PrintBuffer(const uint8_t* buffer, size_t size);
void PrintInt(uint16_t int_val);
void PrintIntDec(uint16_t int_val);
void UART_TransmitString(const char* String);
void RING_TransmitString(const char* String_);





uint8_t Endpoint_Write_Control_PStream_LE_(const void* const Buffer,uint16_t Length);
uint8_t Endpoint_BytesToEPSizeMask_(const uint16_t Bytes);

uint16_t CALLBACK_USB_GetDescriptor_(const uint16_t my_wValue,const uint16_t my_wIndex,void** const  DescriptorAddress);
uint8_t I_ring=0;
uint8_t U_ring=0;
uint8_t      USBtoUSART_Buffer_Data_[]=
{
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0, 
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0, 
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
};
char ring_buffer[]=
{
	'1','n','n','n','n','n','n','n', 'n','n','n','n','n','n','n','n', 'n','n','n','n','n','n','n','n', 'n','n','n','n','n','n','1','\n',
	'2','n','n','n','n','n','n','n', 'n','n','n','n','n','n','n','n', 'n','n','n','n','n','n','n','n', 'n','n','n','n','n','n','2','\n',
	'3','n','n','n','n','n','n','n', 'n','n','n','n','n','n','n','n', 'n','n','n','n','n','n','n','n', 'n','n','n','n','n','n','3','\n',
	'4','n','n','n','n','n','n','n', 'n','n','n','n','n','n','n','n', 'n','n','n','n','n','n','n','n', 'n','n','n','n','n','n','4','\n',
	'5','n','n','n','n','n','n','n', 'n','n','n','n','n','n','n','n', 'n','n','n','n','n','n','n','n', 'n','n','n','n','n','n','5','\n',
	'6','n','n','n','n','n','n','n', 'n','n','n','n','n','n','n','n', 'n','n','n','n','n','n','n','n', 'a','b','c','d','e','f','6','\n',
	'7','n','n','n','n','n','n','n', 'n','n','n','n','n','n','n','n', 'n','n','n','n','n','n','n','n', 'n','n','n','n','n','n','7','\n',
	'8','n','n','n','n','n','n','n', 'n','n','n','n','n','n','n','n', 'n','n','n','n','n','n','n','n', 'n','n','n','n','n','n','8',0x0,
	};
uint8_t step=0;
volatile uint32_t total_micros=0;
volatile uint32_t total_micros_DIV10=0;
volatile uint16_t frames=0;
volatile uint16_t prox_frame_number=0;
volatile uint16_t frame_number=0;
volatile uint8_t microbloks=0;
uint16_t total_micros_DIV16;
bool time_token_loaded=false;
bool data_token_enabled=false;

bool Serial_IsSendReady_(void)
	{
	return ((UCSR1A & (1 << UDRE1)) ? true : false);
	}
bool Endpoint_IsINReady_(void)
	{
	return ((UEINTX & (1 << TXINI)) ? true : false);
	}
bool Endpoint_IsSETUPReceived_(void)
	{
	return ((UEINTX & (1 << RXSTPI)) ? true : false);
	}
bool Endpoint_IsOUTReceived_(void)
	{
	return ((UEINTX & (1 << RXOUTI)) ? true : false);
	}
bool Endpoint_IsReadWriteAllowed_(void)
	{
	return ((UEINTX & (1 << RWAL)) ? true : false);
	}
bool USB_PLL_IsReady_(void)
	{
	return ((PLLCSR & (1 << PLOCK)) ? true : false);
	}
	
void USB_Device_SetDeviceAddress_(const uint8_t Address)
	{
	UDADDR = (UDADDR & (1 << ADDEN)) | (Address & 0x7F);
	}
void USB_Device_EnableDeviceAddress_(const uint8_t Address)
	{
	(void)Address;
	UDADDR |= (1 << ADDEN);
	}
void Endpoint_Write_8_(const uint8_t Data)
	{
	UEDATX = Data;
	}
void Endpoint_ClearOUT_(void)
	{
	UEINTX &= ~((1 << RXOUTI) | (1 << FIFOCON));
	}
void Serial_SendByte_(const char DataByte)
	{
	while (!(Serial_IsSendReady_()));
	UDR1 = DataByte;
	}
void Endpoint_Write_16_LE_(const uint16_t Data)
	{
	UEDATX = (Data & 0xFF);
	UEDATX = (Data >> 8);
	}
void Endpoint_ClearStatusStage_(void)
{	if (my_USB_ControlRequest.my_bmRequestType & 0x80)
	{		while (!(Endpoint_IsOUTReceived_()))
		{
			if (USB_DeviceState_ == DEVICE_STATE_Unattached_)return;
		}
		UEINTX &= ~((1 << RXOUTI) | (1 << FIFOCON));
	Endpoint_ClearOUT_();}
	else	{while (!(Endpoint_IsINReady_()))
		{
			if (USB_DeviceState_ == DEVICE_STATE_Unattached_)return;
		}
		UEINTX &= ~((1 << TXINI) | (1 << FIFOCON));//Endpoint_ClearIN_();
		}
}
		



void USB_Device_ProcessControlRequest_(void)
	{
	uint8_t i;	
	uint8_t* RequestHeader = (uint8_t*)&my_USB_ControlRequest;
	uint8_t RequestHeaderByte;
	for (RequestHeaderByte = 0; RequestHeaderByte < sizeof(my_USB_ControlRequest); RequestHeaderByte++)
		*(RequestHeader++) = UEDATX;
	if (Endpoint_IsSETUPReceived_())
		{
		uint8_t my_bmRequestType = my_USB_ControlRequest.my_bmRequestType;
		if(my_bmRequestType==my_REQ_VendorRequest)my_USB_ControlRequest.my_bRequest=my_REQ_VendorRequest;
		
		
		switch (my_USB_ControlRequest.my_bRequest)
			{
			case REQ_GetStatus_:
											UEINTX &= ~(1 << RXSTPI);//Endpoint_ClearSETUP_();
											Endpoint_Write_16_LE_(1);
											UEINTX &= ~((1 << TXINI) | (1 << FIFOCON));//Endpoint_ClearIN_();
											Endpoint_ClearStatusStage_();
											break;
			case REQ_ClearFeature_:
			case REQ_SetFeature_:
											UENUM=2;
											UECONX |= (1 << STALLRQC);
											//Endpoint_ResetEndpoint_(2);
											UERST = (1 <<2);
											UERST = 0;

											UECONX |= (1 << RSTDT);//Endpoint_ResetDataToggle_();
											UENUM=0;
											UEINTX &= ~(1 << RXSTPI);
											Endpoint_ClearStatusStage_();
											break;
			case REQ_SetAddress_:
											USB_Device_SetAddress_();
											break;
			case REQ_GetDescriptor_:
											USB_Device_GetDescriptor_();
											break;
			case REQ_SetConfiguration_:
											UEINTX &= ~(1 << RXSTPI);//Endpoint_ClearSETUP_();
											Endpoint_ClearStatusStage_();
											USB_DeviceState_ = DEVICE_STATE_Configured_;
											Endpoint_ConfigureEndpoint_(0x82, 2,64, 1);
											UEIENX=OUT_MASK;//|(1<<NAKOUTE)|(1<<FLERRE)|(1<<STALLEDE);//|(1<<NAKINE)
											Endpoint_ConfigureEndpoint_(0x1, 2, 16, 1);
											UEIENX=OUT_MASK;
											break;
			case my_REQ_VendorRequest:
											UEINTX &= ~(1 << RXSTPI);
											for(i=0;i<16;i++)
												UEDATX=pgm_read_byte(&my_STRING_Vendor[i]);
											UEINTX &= ~((1 << TXINI) | (1 << FIFOCON));
											Endpoint_ClearStatusStage_();	
											break;
				
			default:
				break;
			}
		}
		
	if (Endpoint_IsSETUPReceived_())
		{
		UEINTX &= ~(1 << RXSTPI);//Endpoint_ClearSETUP_();
		UECONX |= (1 << STALLRQ);//Endpoint_StallTransaction_();
		}
	}
uint8_t Endpoint_Write_Control_PStream_LE_ (const void* const Buffer,uint16_t Length)
{

	
	uint8_t* DataStream     = ((uint8_t*)Buffer + TEMPLATE_BUFFER_OFFSET_(Length));
	bool     LastPacketFull = false;

	if (Length > my_USB_ControlRequest.my_wLength)
	  Length = my_USB_ControlRequest.my_wLength;
	else if (!(Length))
			UEINTX &= ~((1 << TXINI) | (1 << FIFOCON));//Endpoint_ClearIN_();

	while (Length || LastPacketFull)
	{
		uint8_t USB_DeviceState_LCL = USB_DeviceState_;

		if (USB_DeviceState_LCL == DEVICE_STATE_Unattached_)
		  return ENDPOINT_RWCSTREAM_DeviceDisconnected_;
		else if (USB_DeviceState_LCL == DEVICE_STATE_Suspended_)
		  return ENDPOINT_RWCSTREAM_BusSuspended_;
		else if (Endpoint_IsSETUPReceived_())
		  return ENDPOINT_RWCSTREAM_HostAborted_;
		else if (Endpoint_IsOUTReceived_())
		  break;

		if (Endpoint_IsINReady_())
		{
			uint16_t BytesInEndpoint = UEBCLX;

			while (Length && (BytesInEndpoint < 16))
			{
				TEMPLATE_TRANSFER_BYTE_(DataStream);
				TEMPLATE_BUFFER_MOVE_(DataStream, 1);
				Length--;
				BytesInEndpoint++;
			}

			LastPacketFull = (BytesInEndpoint == 16);
			UEINTX &= ~((1 << TXINI) | (1 << FIFOCON));//Endpoint_ClearIN_();
		}
	}

	while (!(Endpoint_IsOUTReceived_()))
	{
		uint8_t USB_DeviceState_LCL = USB_DeviceState_;

		if (USB_DeviceState_LCL == DEVICE_STATE_Unattached_)
		  return ENDPOINT_RWCSTREAM_DeviceDisconnected_;
		else if (USB_DeviceState_LCL == DEVICE_STATE_Suspended_)
		  return ENDPOINT_RWCSTREAM_BusSuspended_;
		else if (Endpoint_IsSETUPReceived_())
		  return ENDPOINT_RWCSTREAM_HostAborted_;
	}

	return ENDPOINT_RWCSTREAM_NoError_;
}


void USB_ResetInterface_(void)
{
}
			
	
uint8_t Endpoint_BytesToEPSizeMask_(const uint16_t Bytes)
	{
	uint8_t  MaskVal    = 0;
	uint16_t CheckBytes = 8;
	while (CheckBytes < Bytes)
		{
		MaskVal++;
		CheckBytes <<= 1;
		}
	return (MaskVal << EPSIZE0);
	}
	
void Endpoint_ConfigureEndpoint_(const uint8_t Address,
								const uint8_t Type,
								const uint16_t Size,
								const uint8_t Banks)
	{

		UENUM=Address&0xf;//выбор точки
		UECONX &= ~(1 << EPEN);//дизактивация точки
		UECFG1X = 0;  // Сброс ALLOC
		UECONX |= (1 << EPEN);//активация точки
		UECFG0X=(Type<<6)|(Address>>7);//   тип  |  направление
		//UECFG0X = (Type << 6) | ((Address & 0x80) ? (1 << 0) : 0);  // Тип и направление
		uint8_t _Size=0b110010;
		switch(Size)
		{
			case 16:_Size=0b10010;
			break;
			case 64:_Size=0b110010;
			break;
		}
		UECFG1X=0;
		UECFG1X=_Size;//размер    ,   количество банков равно 1,   разрешает ALLOC
		UEINTX=0;
		UEIENX=0;
		while(!(UESTA0X&(1<<CFGOK)));
	
	}
	
	
void USB_Device_SetAddress_(void)
{
	uint8_t DeviceAddress = (my_USB_ControlRequest.my_wValue & 0x7F);
	USB_Device_SetDeviceAddress_(DeviceAddress);
	UEINTX &= ~(1 << RXSTPI);//Endpoint_ClearSETUP_();
	Endpoint_ClearStatusStage_();
	while (!(Endpoint_IsINReady_()));
	UDADDR |= (1 << ADDEN);//USB_Device_EnableDeviceAddress_(DeviceAddress);
	USB_DeviceState_ = (DeviceAddress) ? DEVICE_STATE_Addressed_ : DEVICE_STATE_Default_;
}

uint16_t CALLBACK_USB_GetDescriptor_(const uint16_t my_wValue,
                                    const uint16_t my_wIndex,
                                    void** const  DescriptorAddress)
{


	const uint8_t  DescriptorType   = (my_wValue >> 8);
	const uint8_t  DescriptorNumber = (my_wValue & 0xFF);
	const void* Address = NULL;
	uint16_t    Size    = 0;
	switch (DescriptorType)
	{
		case my_DTYPE_Device:
			Address = &descriptor_PEAK;//&my_DeviceDescriptor;
			Size    = descriptor_PEAK[0];//18;
			break;
		case my_DTYPE_Configuration:
			Address = &config_PEAK;//&my_ConfigurationDescriptor;//
			Size    = config_PEAK[2];//0x2e;
			break;
		case my_DTYPE_String:
			switch (DescriptorNumber)
			{
				case my_STRING_ID_Manufacturer:
					Address = &my_ManufacturerString;
					Size    = my_ManufacturerString[0];
					break;
				case my_Empty:
					Address = &my_EmptyString;
					Size    = my_EmptyString[0];
					break;
				case my_STRING_ID_Product:
					Address = &my_ProductString;
					Size    = my_ProductString[0];
					break;	
				default:break;	
		}

			break;
	}
	*DescriptorAddress = (void*)Address;
	return Size;
}
void USB_Device_GetDescriptor_(void)
{
	void** const DescriptorPointer;
	uint16_t    DescriptorSize;
	
	if ((DescriptorSize = CALLBACK_USB_GetDescriptor_(my_USB_ControlRequest.my_wValue, my_USB_ControlRequest.my_wIndex,
	   (void**)&DescriptorPointer)) == 0)
	{
		return; 
	}
	UEINTX &= ~(1 << RXSTPI);//Endpoint_ClearSETUP_();
	Endpoint_Write_Control_PStream_LE_(DescriptorPointer, DescriptorSize);
	Endpoint_ClearOUT_();
}

void Serial_Init(const uint32_t BaudRate,const bool DoubleSpeed)
	{
		UBRR1  = (DoubleSpeed ? SERIAL_2X_UBBRVAL(BaudRate) : SERIAL_UBBRVAL(BaudRate));
		UCSR1C = ((1 << UCSZ11) | (1 << UCSZ10));
		UCSR1A = (DoubleSpeed ? (1 << U2X1) : 0);
		UCSR1B = (
				  (1 << TXEN1)  
				| (1 << RXEN1)
				| (1 << UDRIE1)
				//| (1 << TXCIE1)
				);
		DDRD  |= (1 << 3);
		PORTD |= (1 << 2);
		sei();
	}
void Serial_SendByte(const char DataByte)
	{
		while (!(Serial_IsSendReady()));
		UDR1 = DataByte;
	}
bool Serial_IsSendReady(void)
	{
		return ((UCSR1A & (1 << UDRE1)) ? true : false);
	}

void UART_TransmitString(const char* String) {
    while (*String) {
        while (!(UCSR1A & (1 << UDRE1)));
        UDR1 = *String++;
    }
		return;
}

void RING_TransmitString(const char *String_) {
     // uint8_t i=0;
	  while (*String_) {
      //ring_buffer[I_ring++] = String_[i++];
    }
	//ring_buffer[I_ring]=0;
    return;
}

void PrintBuffer(const uint8_t* buffer, size_t size) {
	char ABC[]={"0123456789ABCDEF"};
	size_t i;
        Serial_SendByte('{');
    for ( i= 0; i < size; ++i) {
        Serial_SendByte('0');
        Serial_SendByte('x');
        Serial_SendByte(ABC[buffer[i]>>4]);
        Serial_SendByte(ABC[buffer[i]&0xF]);
        if(i<size-1)Serial_SendByte(',');
    }
        Serial_SendByte('}');
        Serial_SendByte(';');
        Serial_SendByte('\r');
        Serial_SendByte('\n');
		return;
}


void PrintBuffer_ring(const uint8_t* buffer, size_t size) {
	char ABC[]={"0123456789ABCDEF"};
	size_t i;
        Serial_SendByte_ring('{');
    for ( i= 0; i < size; ++i) {
        Serial_SendByte_ring('0');
        Serial_SendByte_ring('x');
        Serial_SendByte_ring(ABC[buffer[i]>>4]);
        Serial_SendByte_ring(ABC[buffer[i]&0xF]);
        if(i<size-1)Serial_SendByte_ring(',');
    }
        Serial_SendByte_ring('}');
        Serial_SendByte_ring(';');
        Serial_SendByte_ring('\r');
        Serial_SendByte_ring('\n');
        Serial_SendByte_ring(0);
		return;
}



void Serial_SendByte_ring(char data)
{
	//ring_buffer[I_ring++]=data;
	return;
}




void PrintInt(uint16_t int_val) {
	char ABC[]={"0123456789ABCDEF"};
    Serial_SendByte(ABC[(int_val>>12)&0xF]);
    Serial_SendByte(ABC[(int_val>>8)&0xF]);
    Serial_SendByte(ABC[(int_val>>4)&0xF]);
    Serial_SendByte(ABC[(int_val>>0)&0xF]);
}


void PrintIntDec(uint16_t int_val) {
    char ABC[] = "0123456789";
    uint8_t i;
    for (i = 0; i < 5; ++i) {
        uint16_t divisor = 1;
		uint8_t j;
        for (j = 0; j < 4 - i; ++j) {
            divisor *= 10;
        }
        Serial_SendByte(ABC[(int_val / divisor) % 10]);
    }
}


void PrintIntDec_(uint16_t int_val) {
	char ABC[]={"0123456789"};
	uint32_t dec4=(int_val>>12)&0xF;
	uint32_t dec3=(int_val>>8)&0xF;
	uint32_t dec2=(int_val>>4)&0xF;
	uint32_t dec1=(int_val>>0)&0xF;
	uint32_t dec=(dec4<<12)+(dec3<<8)+(dec2<<4)+dec1;
    uint32_t dec50=dec/10000;
    uint32_t dec40=(dec/1000)-dec50*10;
    uint32_t dec30=(dec/100)-dec50*100-dec40*10;
    uint32_t dec20=(dec/10)-dec50*1000-dec40*100-dec30*10;
    uint32_t dec10=(dec/1)-dec50*10000-dec40*1000-dec30*100-dec20*10;
    Serial_SendByte(ABC[dec50]);
    Serial_SendByte(ABC[dec40]);
    Serial_SendByte(ABC[dec30]);
    Serial_SendByte(ABC[dec20]);
    Serial_SendByte(ABC[dec10]);
	return;
}



void timer0_init() {
    cli(); // Здесь CLI отключает глобальные прерывания, если это необходимо.
	GTCCR|=1<<TSM;
    TIMSK1 =0;// (1 << OCIE1A);
    TIMSK0 =0;// (1 << OCIE0A); // Разрешаем прерывание по совпадению с OCR0A
    TCCR0A = 0; // Очищаем регистр TCCR0A
    TCCR0B = 0; // Очищаем регистр TCCR0B
    TCNT0 = 0;  // Обнуляем счетчик
    TCCR0A |= (1 << WGM01); // 
    TCCR0B |= (1 << CS00); // Устанавливаем предделитель на 1 (без деления)
    TCCR1B |= (1 << WGM12);//НАСТРОЙКА НА 1 СЕКУНДУ
    TCCR1B |= (1 << CS12);
    TCCR1B |= (0 << CS11);
    TCCR1B |= (0 << CS10);
    OCR1A = 0;
    OCR0A = 0; // 
    OCR1A |= 62499;//
    OCR0A |= 255;// 
 //   OCR0A |= 159;// 

    TIMSK1 |= (1 << OCIE1A);
    TIMSK0 |= (1 << OCIE0A); // Разрешаем прерывание по совпадению с OCR0A
	GTCCR&=~(1<<TSM);
	while(GTCCR&(1<<PSRSYNC))asm volatile("nop");
    sei(); // Здесь SEI включает глобальные прерывания, если это необходимо.
}


