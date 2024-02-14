		#include "Descriptors.h"
bool step_Vendor;
		
static my_RingBuffer_t USBtoUSART_Buffer_;
static uint8_t      USBtoUSART_Buffer_Data_[My_BULK_size];
static my_RingBuffer_t USARTtoUSB_Buffer_;
static uint8_t      USARTtoUSB_Buffer_Data_[My_BULK_size];
void GlobalInterruptDisable_(void);
uint8_t TEMPLATE_FUNC_NAME_ (const void* const Buffer,uint16_t Length);
void USB_Device_SetDeviceAddress_(const uint8_t Address);
uint32_t Endpoint_Read_32_LE_(void);
void USB_Device_GetInternalSerialDescriptor_(void);
void USB_Device_ClearSetFeature_(void);
void USB_INT_Disable_(const uint8_t Interrupt);
bool Endpoint_ConfigureEndpointTable_(const my_USB_Endpoint_Table_t* const Table,const uint8_t Entries);
bool Device_ConfigureEndpoints_(My_USB_Info_Device_t* const my_InterfaceInfo);
uint16_t USB_Device_GetFrameNumber_(void);
uint8_t Endpoint_WaitUntilReady_(void);
bool Endpoint_ConfigureEndpoint_(const uint8_t Address,const uint8_t Type,const uint16_t Size,const uint8_t Banks);
uint8_t my_Device_Flush_(My_USB_Info_Device_t* const my_InterfaceInfo);
void USB_Device_GetSerialString_(uint16_t* const UnicodeString);
void USB_Device_EnableDeviceAddress_(const uint8_t Address);
void USB_Device_SetAddress_(void);
uint16_t RingBuffer_GetCount_(my_RingBuffer_t* const Buffer);
bool Endpoint_ConfigureEndpointTable_(const my_USB_Endpoint_Table_t* const Table,
                                     const uint8_t Entries);
void USB_Device_GetDescriptor_(void);
void USB_Device_GetStatus_(void);
void USB_INT_Clear_(const uint8_t Interrupt);
void USB_INT_Enable_(const uint8_t Interrupt);
void USB_Init_Device_(void);	
void USB_Device_SetConfiguration_(void);	
void USB_Device_GetConfiguration_(void);	
void EVENT_USB_Device_ConfigurationChanged_(void);
void USB_ResetInterface_(void);
uint8_t Endpoint_GetEndpointDirection_(void);
uint16_t CALLBACK_USB_GetDescriptor_(const uint16_t my_wValue,const uint16_t my_wIndex,void** const  DescriptorAddress);
uint8_t Endpoint_Write_Control_Stream_LE_ (const void* const Buffer,uint16_t Length);
uint8_t Endpoint_Write_Control_PStream_LE_(const void* const Buffer,uint16_t Length);
uint8_t Endpoint_BytesToEPSizeMask_(const uint16_t Bytes);
bool Endpoint_ConfigureEndpoint_Prv_(const uint8_t Number,const uint8_t UECFG0XData,const uint8_t UECFG1XData);
void EVENT_USB_Device_ControlRequest_(void);
bool        USB_IsInitialized_;
bool    USB_Device_CurrentlySelfPowered_=true;
bool    USB_Device_RemoteWakeupEnabled_=false;
uint8_t USB_Device_ConfigurationNumber_=0;

// Функция для переключения направления конечной точки
void ToggleEndpointDirection_(void) {
    // Выключаем конечную точку
    UECONX &= ~(1 << EPEN);

    // Переключаем направление
    UECFG0X ^= (1 << EPDIR);

    // Включаем конечную точку обратно
    UECONX |= (1 << EPEN);
}

// Пример использования:
void USB_CLK_Freeze_(void)
	{
	USBCON |=  (1 << FRZCLK);
	}
void USB_PLL_Off_(void)
	{
	PLLCSR = 0;
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
uint8_t Endpoint_GetCurrentEndpoint_(void)
	{
	return ((UENUM & ENDPOINT_EPNUM_MASK_) | Endpoint_GetEndpointDirection_());
	}
void Endpoint_Write_8_(const uint8_t Data)
	{
	UEDATX = Data;
	}
uint8_t Endpoint_Read_8_(void)
	{
	return UEDATX;
	}
uint16_t Endpoint_BytesInEndpoint_(void)
	{
	return UEBCLX;
	}
void Endpoint_ClearIN_(void)
	{
	UEINTX &= ~((1 << TXINI) | (1 << FIFOCON));
	}
bool Endpoint_IsStalled_(void)
	{
	return ((UECONX & (1 << STALLRQ)) ? true : false);
	}
bool Endpoint_IsEnabled_(void)
	{
	return ((UECONX & (1 << EPEN)) ? true : false);
	}
bool Serial_IsSendReady_(void)
	{
	return ((UCSR1A & (1 << UDRE1)) ? true : false);
	}
bool RingBuffer_IsEmpty_(my_RingBuffer_t* const Buffer)
	{
	return (RingBuffer_GetCount_(Buffer) == 0);
	}
void Endpoint_ClearOUT_(void)
	{
	UEINTX &= ~((1 << RXOUTI) | (1 << FIFOCON));
	}
bool Endpoint_IsConfigured_(void)
	{
	return ((UESTA0X & (1 << CFGOK)) ? true : false);
	}
void Endpoint_DisableEndpoint_(void)
	{
	UECONX &= ~(1 << EPEN);
	}	
void USB_Controller_Reset_(void)
	{
	USBCON &= ~(1 << USBE);
	USBCON |=  (1 << USBE);
	}
void USB_PLL_On_(void)
	{
	PLLCSR = USB_PLL_PSC_;
	PLLCSR = (USB_PLL_PSC_ | (1 << PLLE));
	}
void USB_INT_DisableAllInterrupts_(void)
	{
	UDIEN   = 0;
	}
void USB_INT_ClearAllInterrupts_(void)
	{
	UDINT  = 0;
	}
void USB_CLK_Unfreeze_(void)
	{
	USBCON &= ~(1 << FRZCLK);
	}
void Serial_SendByte_(const char DataByte)
	{
	while (!(Serial_IsSendReady_()));
	UDR1 = DataByte;
	}
bool Endpoint_IsINReady_(void)
	{
	return ((UEINTX & (1 << TXINI)) ? true : false);
	}
bool Endpoint_IsINReceived_(void)
{
    return ((UEINTX & (1 << TXINI)) ? true : false);
}
bool USB_Device_IsAddressSet_(void)
	{
	return (UDADDR & (1 << ADDEN));
	}
uint8_t Endpoint_GetEndpointDirection_(void)
	{
	return (UECFG0X & (1 << EPDIR)) ? 0x80 : 0x00;
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
void Endpoint_ClearSETUP_(void)
	{
	UEINTX &= ~(1 << RXSTPI);
	}
bool USB_PLL_IsReady_(void)
	{
	return ((PLLCSR & (1 << PLOCK)) ? true : false);
	}
void Endpoint_ResetDataToggle_(void)
	{
	UECONX |= (1 << RSTDT);
	}
void USB_Attach_(void)
	{
	UDCON  &= ~(1 << DETACH);
	}
void Endpoint_StallTransaction_(void)
	{
	UECONX |= (1 << STALLRQ);
	}
void Endpoint_ClearStall_(void)
	{
	UECONX |= (1 << STALLRQC);
	}
void Endpoint_SelectEndpoint_(const uint8_t Address)
	{
	UENUM = (Address & ENDPOINT_EPNUM_MASK_);
	}	
void Endpoint_Write_16_LE_(const uint16_t Data)
	{
	UEDATX = (Data & 0xFF);
	UEDATX = (Data >> 8);
	}
void Endpoint_ResetEndpoint_(const uint8_t Address)
	{
	UERST = (1 << (Address & ENDPOINT_EPNUM_MASK_));
	UERST = 0;
	}
uint8_t GetGlobalInterruptMask_(void)
	{
	GCC_MEMORY_BARRIER_();
	return SREG;
	}
void GlobalInterruptEnable_(void)
	{
	GCC_MEMORY_BARRIER_();
	sei();
	GCC_MEMORY_BARRIER_();
	}
void GlobalInterruptDisable_(void)
	{
	GCC_MEMORY_BARRIER_();
	cli();
	GCC_MEMORY_BARRIER_();
	}
void SetGlobalInterruptMask_(const uint8_t GlobalIntState)
	{
	GCC_MEMORY_BARRIER_();
	SREG = GlobalIntState;
	GCC_MEMORY_BARRIER_();
	}
void Endpoint_EnableEndpoint_(void)
	{
	UECONX |= (1 << EPEN);
	}
void Endpoint_Write_32_LE_(const uint32_t Data)
	{
	UEDATX = (Data &  0xFF);
	UEDATX = (Data >> 8);
	UEDATX = (Data >> 16);
	UEDATX = (Data >> 24);
	}
	
bool USB_INT_HasOccurred_(const uint8_t Interrupt)
			{
				switch (Interrupt)
				{
					case USB_INT_WAKEUPI_:
						return (UDINT  & (1 << WAKEUPI));
					case USB_INT_SUSPI_:
						return (UDINT  & (1 << SUSPI));
					case USB_INT_EORSTI_:
						return (UDINT  & (1 << EORSTI));
					case USB_INT_SOFI_:
						return (UDINT  & (1 << SOFI));
					case USB_INT_RXSTPI_:
						return (UEINTX & (1 << RXSTPI));
					case USB_INT_TXINI_:
						return (UEINTX & (1 << TXINI));
					default:
						return false;
				}
			}

bool USB_INT_IsEnabled_(const uint8_t Interrupt)
			{
				switch (Interrupt)
				{
					case USB_INT_WAKEUPI_:
						return (UDIEN  & (1 << WAKEUPE));
					case USB_INT_SUSPI_:
						return (UDIEN  & (1 << SUSPE));
					case USB_INT_EORSTI_:
						return (UDIEN  & (1 << EORSTE));
					case USB_INT_SOFI_:
						return (UDIEN  & (1 << SOFE));
					case USB_INT_RXSTPI_:
						return (UEIENX & (1 << RXSTPE));
					case USB_INT_TXINI_:
						return (UEIENX & (1 << TXINE));
					default:
						return false;
				}
			}
	
	
	
	
void EVENT_CDC_Device_LineEncodingChanged_(My_USB_Info_Device_t* const my_InterfaceInfo)
	{
	uint8_t ConfigMask = 0;
	switch (my_InterfaceInfo->My_State.My_LineEncoding.my_ParityType)
		{
		case CDC_PARITY_Odd_:
			ConfigMask = ((1 << UPM11) | (1 << UPM10));
			break;
		case CDC_PARITY_Even_:
			ConfigMask = (1 << UPM11);
			break;
		}
	if (my_InterfaceInfo->My_State.My_LineEncoding.my_CharFormat == my_CDC_LINEENCODING_TwoStopBits)
	  ConfigMask |= (1 << USBS1);
	switch (my_InterfaceInfo->My_State.My_LineEncoding.my_DataBits)
	{
	case 6:
		ConfigMask |= (1 << UCSZ10);
		break;
	case 7:
		ConfigMask |= (1 << UCSZ11);
		break;
	case 8:
		ConfigMask |= ((1 << UCSZ11) | (1 << UCSZ10));
		break;
	}
	PORTD |= (1 << 3);
	UCSR1B = 0;
	UCSR1A = 0;
	UCSR1C = 0;
	UBRR1  = SERIAL_2X_UBBRVAL_(my_InterfaceInfo->My_State.My_LineEncoding.my_BaudRateBPS);
	UCSR1C = ConfigMask;
	UCSR1A = (1 << U2X1);
	UCSR1B = ((1 << RXCIE1) | (1 << TXEN1) | (1 << RXEN1));
	PORTD &= ~(1 << 3);
	}
			
void EVENT_Device_ControLineStateChanged_(My_USB_Info_Device_t* const my_InterfaceInfo)
{
	bool CurrentDTRState = (my_InterfaceInfo->My_State.My_ControlLineStates.my_HostToDevice & my_CDC_CONTROL_LINE_OUT_DTR);
	if (CurrentDTRState)
	  AVR_LINE_PORT &= 0x7F;
	else
	  AVR_LINE_PORT |= 0x80;
}
			
		///////////////////////////////////TOP  LINE//////////////////////////////	
void Endpoint_ClearStatusStage_(void)
{	if (my_USB_ControlRequest.my_bmRequestType & REQDIR_DEVICETOHOST_)
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
		Endpoint_ClearIN_();}
}
		
void Device_ProcessControlRequest_(My_USB_Info_Device_t* const my_InterfaceInfo)
{
	if (!(Endpoint_IsSETUPReceived_()))
	  return;

	if (my_USB_ControlRequest.my_wIndex != my_InterfaceInfo->My_Config.my_ControlInterfaceNumber)
	  return;

	switch (my_USB_ControlRequest.my_bRequest)
	{
		case CDC_REQ_GetLineEncoding_://0x21
			if (my_USB_ControlRequest.my_bmRequestType == (REQDIR_DEVICETOHOST_ | REQTYPE_CLASS_ | REQREC_INTERFACE_))
			{	Endpoint_ClearSETUP_();
				while (!(Endpoint_IsINReady_()));
				Endpoint_Write_32_LE_(my_InterfaceInfo->My_State.My_LineEncoding.my_BaudRateBPS);
				UEDATX = my_InterfaceInfo->My_State.My_LineEncoding.my_CharFormat;
				UEDATX = my_InterfaceInfo->My_State.My_LineEncoding.my_ParityType;
				UEDATX = my_InterfaceInfo->My_State.My_LineEncoding.my_DataBits;
				Endpoint_ClearIN_();
				Endpoint_ClearStatusStage_();
			}
			break;
		case CDC_REQ_SetLineEncoding_://0x20
		//LEDS_ON;
			if (my_USB_ControlRequest.my_bmRequestType == (REQDIR_HOSTTODEVICE_ | REQTYPE_CLASS_ | REQREC_INTERFACE_))
			{Endpoint_ClearSETUP_();
				while (!(Endpoint_IsOUTReceived_()))
					{if (USB_DeviceState_ == DEVICE_STATE_Unattached_)return;}
				my_InterfaceInfo->My_State.My_LineEncoding.my_BaudRateBPS = Endpoint_Read_32_LE_();
				my_InterfaceInfo->My_State.My_LineEncoding.my_CharFormat  = Endpoint_Read_8_();
				my_InterfaceInfo->My_State.My_LineEncoding.my_ParityType  = Endpoint_Read_8_();
				my_InterfaceInfo->My_State.My_LineEncoding.my_DataBits    = Endpoint_Read_8_();
				Endpoint_ClearOUT_();
				Endpoint_ClearStatusStage_();
				EVENT_CDC_Device_LineEncodingChanged_(my_InterfaceInfo);
			}

			break;
		case CDC_REQ_SetControlLineState_://0x22
			if (my_USB_ControlRequest.my_bmRequestType == (REQDIR_HOSTTODEVICE_ | REQTYPE_CLASS_ | REQREC_INTERFACE_))
			{
				Endpoint_ClearSETUP_();
				Endpoint_ClearStatusStage_();
				my_InterfaceInfo->My_State.My_ControlLineStates.my_HostToDevice = my_USB_ControlRequest.my_wValue;
				EVENT_Device_ControLineStateChanged_(my_InterfaceInfo);
			}
			break;
	}
}



void USB_Device_ProcessControlRequest_(void)
	{
	uint8_t* RequestHeader = (uint8_t*)&my_USB_ControlRequest;
	uint8_t RequestHeaderByte;
	for (RequestHeaderByte = 0; RequestHeaderByte < sizeof(my_USB_Request_Header_t); RequestHeaderByte++)
		*(RequestHeader++) = Endpoint_Read_8_();
	//EVENT_USB_Device_ControlRequest_();
	if (Endpoint_IsSETUPReceived_())
		{
		uint8_t my_bmRequestType = my_USB_ControlRequest.my_bmRequestType;
		if(my_bmRequestType==my_REQ_VendorRequest)my_USB_ControlRequest.my_bRequest=my_REQ_VendorRequest;
		
		//if(my_bmRequestType==0x0&&my_USB_ControlRequest.my_bRequest==9)LEDS_ON;
		
		switch (my_USB_ControlRequest.my_bRequest)
			{
			case REQ_GetStatus_:
				if ((my_bmRequestType == (REQDIR_DEVICETOHOST_ | REQTYPE_STANDARD_ | REQREC_DEVICE_)) ||
					(my_bmRequestType == (REQDIR_DEVICETOHOST_ | REQTYPE_STANDARD_ | REQREC_ENDPOINT_)))
					{
					USB_Device_GetStatus_();
					}
				break;
			case REQ_ClearFeature_:
			case REQ_SetFeature_:
				if ((my_bmRequestType == (REQDIR_HOSTTODEVICE_ | REQTYPE_STANDARD_ | REQREC_DEVICE_)) ||
					(my_bmRequestType == (REQDIR_HOSTTODEVICE_ | REQTYPE_STANDARD_ | REQREC_ENDPOINT_)))
					{
						USB_Device_ClearSetFeature_();
					}
				break;
			case REQ_SetAddress_:
				if (my_bmRequestType == (REQDIR_HOSTTODEVICE_ | REQTYPE_STANDARD_ | REQREC_DEVICE_))
					USB_Device_SetAddress_();
				break;
			case REQ_GetDescriptor_:
				if ((my_bmRequestType == (REQDIR_DEVICETOHOST_ | REQTYPE_STANDARD_ | REQREC_DEVICE_)) ||
					(my_bmRequestType == (REQDIR_DEVICETOHOST_ | REQTYPE_STANDARD_ | REQREC_INTERFACE_)))
					{
					USB_Device_GetDescriptor_();
					}
				break;
			//case REQ_GetConfiguration_:LEDS_ON;
			//	if (my_bmRequestType == (REQDIR_DEVICETOHOST_ | REQTYPE_STANDARD_ | REQREC_DEVICE_))
			//	  USB_Device_GetConfiguration_();
			// 	break;
			case REQ_SetConfiguration_:
				if (my_bmRequestType == (REQDIR_HOSTTODEVICE_ | REQTYPE_STANDARD_ | REQREC_DEVICE_))
				{
					USB_Device_SetConfiguration_();
				}
				break;
					
			case my_REQ_VendorRequest:
				Endpoint_ClearSETUP_();
				uint8_t i_Vendor_1;
				for(i_Vendor_1=0;i_Vendor_1<sizeof(my_STRING_Vendor);i_Vendor_1++)
					Endpoint_Write_8_(my_STRING_Vendor[i_Vendor_1]);
				Endpoint_ClearIN_();
				Endpoint_ClearStatusStage_();	
				step_Vendor=true;
				break;
				
			default:
				break;
			}
		}
		
	if (Endpoint_IsSETUPReceived_())
		{
		Endpoint_ClearSETUP_();
		Endpoint_StallTransaction_();
		}
	}
	
	
	
	

void  my_USB_Device_Process_BULK_OUT()
	{
		uint8_t i;
		for (i = 0; i < My_BULK_size_signific; i++) 
			my_data_bulk[i] = Endpoint_Read_8_();
		if(Endpoint_IsOUTReceived_())
			Endpoint_ClearOUT_();


		return;	
	}
void  my_USB_Device_Process_BULK_IN()
	{

		uint8_t i;
		for (i = 0; i < My_BULK_size_signific_for_answer; i++)
			Endpoint_Write_8_(my_data_bulk[i]);
		
		Endpoint_Write_8_(9);// для эксперимента пока просто число
			
		for (i = 0; i < My_BULK_size_common; i++)
			Endpoint_Write_8_(0);
		Endpoint_ClearIN_();
		return;	
	}
void USB_DeviceTask_(void)
{
return;
	uint8_t PrevSelectedEndpoint = Endpoint_GetCurrentEndpoint_();
			  	Endpoint_SelectEndpoint_(0x81);
				Endpoint_EnableEndpoint_();
			  	Endpoint_SelectEndpoint_(0x1);
				Endpoint_EnableEndpoint_();
	
	Endpoint_SelectEndpoint_(1);
	if (Endpoint_IsOUTReceived_())
	  my_USB_Device_Process_BULK_OUT();

	Endpoint_SelectEndpoint_(0x81);
	while(!Endpoint_IsINReady_())Endpoint_SelectEndpoint_(0x81);
	
		my_USB_Device_Process_BULK_IN();

	Endpoint_SelectEndpoint_(PrevSelectedEndpoint);
	return;
}
	



	
void CDC_Device_USBTask_(My_USB_Info_Device_t* const my_InterfaceInfo)
{

	if ((USB_DeviceState_ != DEVICE_STATE_Configured_))
	  return;
	Endpoint_SelectEndpoint_(my_InterfaceInfo->My_Config.my_DataINEndpoint.my_Address);

	if (Endpoint_IsINReady_())
		{
			//LEDS_ON;
		my_Device_Flush_(my_InterfaceInfo);
		}
}
uint8_t CDC_Device_SendByte_(My_USB_Info_Device_t* const my_InterfaceInfo,
                            const uint8_t Data)
{

//	if ((USB_DeviceState_ != DEVICE_STATE_Configured_) || !(my_InterfaceInfo->My_State.My_LineEncoding.my_BaudRateBPS))
	if ((USB_DeviceState_ != DEVICE_STATE_Configured_))
	  return ENDPOINT_RWSTREAM_DeviceDisconnected_;

	Endpoint_SelectEndpoint_(my_InterfaceInfo->My_Config.my_DataINEndpoint.my_Address);

	if (!(Endpoint_IsReadWriteAllowed_()))
	{
		Endpoint_ClearIN_();

		uint8_t ErrorCode;

		if ((ErrorCode = Endpoint_WaitUntilReady_()) != ENDPOINT_READYWAIT_NoError_)
		  return ErrorCode;
	}

	Endpoint_Write_8_(Data);
	return ENDPOINT_READYWAIT_NoError_;
}


uint8_t RingBuffer_Remove_(my_RingBuffer_t* Buffer)
		{
			GCC_FORCE_POINTER_ACCESS_(Buffer);

			uint8_t Data = *Buffer->Out;

			if (++Buffer->Out == Buffer->End)
			  Buffer->Out = Buffer->Start;

			uint8_t CurrentGlobalInt = GetGlobalInterruptMask_();
			cli();

			Buffer->Count--;

			SetGlobalInterruptMask_(CurrentGlobalInt);

			return Data;
		}
		static inline void RingBuffer_InitBuffer_(my_RingBuffer_t* Buffer,
		                                         uint8_t* const DataPtr,
		                                         const uint16_t Size)
		{
			GCC_FORCE_POINTER_ACCESS_(Buffer);
			uint8_t CurrentGlobalInt = GetGlobalInterruptMask_();
			GlobalInterruptDisable_();
			Buffer->In     = DataPtr;
			Buffer->Out    = DataPtr;
			Buffer->Start  = &DataPtr[0];
			Buffer->End    = &DataPtr[Size];
			Buffer->Size   = Size;
			Buffer->Count  = 0;
			SetGlobalInterruptMask_(CurrentGlobalInt);
		}
			
bool RingBuffer_IsFull_(my_RingBuffer_t* const Buffer)
		{
			return (RingBuffer_GetCount_(Buffer) == Buffer->Size);
		}
		
		
void my_Device_SendPacket(My_USB_Info_Device_t* const my_InterfaceInfo, uint8_t* buffer)
{
				Endpoint_SelectEndpoint_(0x81);
			if (Endpoint_IsINReady_() && Endpoint_IsReadWriteAllowed_())
			{	
				//LEDS_ON;
				uint8_t i;
				for (i = 0; i < 16; ++i)
				{
					UEDATX = buffer[i];
				}
				while (!(UEINTX & (1 << TXINI)));
				Endpoint_ClearIN_();			
			}
return;
}



void my_Device_ReceivePacket(My_USB_Info_Device_t* const my_InterfaceInfo, uint8_t* buffer)
{
    if ((USB_DeviceState_ != DEVICE_STATE_Configured_))
        return ;  
	Endpoint_SelectEndpoint_(1);
	while(!Endpoint_IsOUTReceived_());
    if (Endpoint_IsOUTReceived_())
		{
        if (Endpoint_BytesInEndpoint_() >1)
			{
			uint8_t i;
            for (i = 0; i < 16; i++)
				{
                buffer[i] = Endpoint_Read_8_();
				}
            Endpoint_ClearOUT_();
		}
		
	}
}
void RingBuffer_Insert_(my_RingBuffer_t* Buffer,
		                                     const uint8_t Data)
		{
			GCC_FORCE_POINTER_ACCESS_(Buffer);

			*Buffer->In = Data;

			if (++Buffer->In == Buffer->End)
			  Buffer->In = Buffer->Start;

			uint8_t CurrentGlobalInt = GetGlobalInterruptMask_();
			GlobalInterruptDisable_();

			Buffer->Count++;

			SetGlobalInterruptMask_(CurrentGlobalInt);
		}
uint16_t RingBuffer_GetCount_(my_RingBuffer_t* const Buffer)
		{
			uint16_t Count;

			uint8_t CurrentGlobalInt = GetGlobalInterruptMask_();
			GlobalInterruptDisable_();

			Count = Buffer->Count;

			SetGlobalInterruptMask_(CurrentGlobalInt);
			return Count;
		}
uint8_t RingBuffer_Peek_(my_RingBuffer_t* const Buffer)
		{
			return *Buffer->Out;
		}

void My_Device_ConfigureEndpoints_(const my_USB_Descriptor_Configuration_t* my_config_1)
	{
		Endpoint_ConfigureEndpoint_(my_config_1->my_DataINEndpoint.my_EndpointAddress,
			my_config_1->my_DataINEndpoint.my_Attributes,
			my_config_1->my_DataINEndpoint.my_EndpointSize,
			my_config_1->my_DataINEndpoint.my_PollingIntervalMS);

		Endpoint_ConfigureEndpoint_(my_config_1->my_DataOutEndpoint.my_EndpointAddress,
			my_config_1->my_DataOutEndpoint.my_Attributes,
			my_config_1->my_DataOutEndpoint.my_EndpointSize,
			my_config_1->my_DataOutEndpoint.my_PollingIntervalMS);
		return;
	}

void EVENT_USB_Device_ConfigurationChanged_(void)
{
	bool ConfigSuccess = true;
	ConfigSuccess &= Device_ConfigureEndpoints_(&my_Virtual_Interface);

//LEDS_ON;
}




void EVENT_USB_Device_ControlRequest_(void)
{
	Device_ProcessControlRequest_(&my_Virtual_Interface);
}


void USB_REG_Off_(void)
	{
	REGCR  |=  (1 << REGDIS);
	}
void USB_REG_On_(void)
	{
	REGCR  &= ~(1 << REGDIS);
	}
void USB_Init_(void)
{
	if (!(USB_Options_ & USB_OPT_REG_DISABLED_))
	  USB_REG_On_();
	else
	  USB_REG_Off_();

	if (!(USB_Options_ & USB_OPT_MANUAL_PLL_))
	{
	}
	USB_IsInitialized_ = true;
	USB_ResetInterface_();
}

void USB_Device_ClearSetFeature_(void)
{
	switch (my_USB_ControlRequest.my_bmRequestType & CONTROL_REQTYPE_RECIPIENT_)
	{
		case REQREC_DEVICE_:
		{
			if ((uint8_t)my_USB_ControlRequest.my_wValue == FEATURE_SEL_DeviceRemoteWakeup_)
			  USB_Device_RemoteWakeupEnabled_ = (my_USB_ControlRequest.my_bRequest == REQ_SetFeature_);
			else
			  return;
			break;
		}
		case REQREC_ENDPOINT_:
		{
			if ((uint8_t)my_USB_ControlRequest.my_wValue == FEATURE_SEL_EndpointHalt_)
			{
				uint8_t EndpointIndex = ((uint8_t)my_USB_ControlRequest.my_wIndex & ENDPOINT_EPNUM_MASK_);
				if (EndpointIndex == ENDPOINT_CONTROLEP_ || EndpointIndex >= ENDPOINT_TOTAL_ENDPOINTS_)
				  return;
				Endpoint_SelectEndpoint_(EndpointIndex);
				if (Endpoint_IsEnabled_())
				{
					if (my_USB_ControlRequest.my_bRequest == REQ_SetFeature_)
					{
						Endpoint_StallTransaction_();
					}
					else
					{
						Endpoint_ClearStall_();
						Endpoint_ResetEndpoint_(EndpointIndex);
						Endpoint_ResetDataToggle_();
					}
				}
			}

			break;
		}
		default:
			return;
	}
	Endpoint_SelectEndpoint_(ENDPOINT_CONTROLEP_);
	Endpoint_ClearSETUP_();
	Endpoint_ClearStatusStage_();
}



uint8_t Endpoint_Write_Control_PStream_LE_ (const void* const Buffer,uint16_t Length)
{
		#define  TEMPLATE_BUFFER_OFFSET_(Length)            0
		#define  TEMPLATE_BUFFER_MOVE_(BufferPtr, Amount)   BufferPtr += Amount
		#define  TEMPLATE_TRANSFER_BYTE_(BufferPtr)         Endpoint_Write_8_(pgm_read_byte(BufferPtr))

	
	uint8_t* DataStream     = ((uint8_t*)Buffer + TEMPLATE_BUFFER_OFFSET_(Length));
	bool     LastPacketFull = false;

	if (Length > my_USB_ControlRequest.my_wLength)
	  Length = my_USB_ControlRequest.my_wLength;
	else if (!(Length))
			Endpoint_ClearIN_();

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
			uint16_t BytesInEndpoint = Endpoint_BytesInEndpoint_();

			while (Length && (BytesInEndpoint < USB_Device_ControlEndpointSize_))
			{
				TEMPLATE_TRANSFER_BYTE_(DataStream);
				TEMPLATE_BUFFER_MOVE_(DataStream, 1);
				Length--;
				BytesInEndpoint++;
			}

			LastPacketFull = (BytesInEndpoint == USB_Device_ControlEndpointSize_);
			Endpoint_ClearIN_();
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

void USB_Device_GetConfiguration_(void)
	{
	Endpoint_ClearSETUP_();
	Endpoint_Write_8_(USB_Device_ConfigurationNumber_);
	Endpoint_ClearIN_();	
	Endpoint_ClearStatusStage_();
	}

void USB_Device_SetConfiguration_(void)
	{
	if ((uint8_t)my_USB_ControlRequest.my_wValue > 1)return;
	Endpoint_ClearSETUP_();
	USB_Device_ConfigurationNumber_ = (uint8_t)my_USB_ControlRequest.my_wValue;
	Endpoint_ClearStatusStage_();
	if (USB_Device_ConfigurationNumber_)USB_DeviceState_ = DEVICE_STATE_Configured_;
	else	USB_DeviceState_ = (USB_Device_IsAddressSet_()) ? DEVICE_STATE_Configured_ : DEVICE_STATE_Powered_;
	EVENT_USB_Device_ConfigurationChanged_();

	}
void USB_ResetInterface_(void)
{
	USB_INT_DisableAllInterrupts_();
	USB_INT_ClearAllInterrupts_();
	USB_Controller_Reset_();
	USB_CLK_Unfreeze_();

	if (USB_CurrentMode_ == USB_MODE_Device_)
	{
		if (!(USB_Options_ & USB_OPT_MANUAL_PLL_))
		{
			USB_PLL_On_();
			while (!(USB_PLL_IsReady_()));
		}

		USB_Init_Device_();
	}
	else if (USB_CurrentMode_ == USB_MODE_Host_)
	{
	}
}
bool first_IN=false;

void USB_Init_Device_(void)
{
	USB_DeviceState_                 = DEVICE_STATE_Unattached_;
	USB_Device_ConfigurationNumber_  = 0;
	USB_Device_RemoteWakeupEnabled_  = false;
	USB_Device_CurrentlySelfPowered_ = true;
	Endpoint_ConfigureEndpoint_(ENDPOINT_CONTROLEP_, EP_TYPE_CONTROL_, USB_Device_ControlEndpointSize_, 1);
	USB_INT_Clear_(USB_INT_SUSPI_);
	USB_INT_Enable_(USB_INT_SUSPI_);
	USB_INT_Enable_(USB_INT_EORSTI_);
	
	USB_Attach_();
	first_IN=false;
}
void USB_INT_Enable_(const uint8_t Interrupt)
	{
	switch (Interrupt)
		{
		case USB_INT_WAKEUPI_:	UDIEN  |= (1 << WAKEUPE);
								break;
		case USB_INT_SUSPI_:	UDIEN  |= (1 << SUSPE);
								break;
		case USB_INT_EORSTI_:	UDIEN  |= (1 << EORSTE);
								break;
		case USB_INT_SOFI_:		UDIEN  |= (1 << SOFE);
								break;
		case USB_INT_RXSTPI_:	UEIENX |= (1 << RXSTPE);
								break;
		case USB_INT_TXINI_:	UEIENX |= (1 << TXINE);
								break;
		
		default:	break;
		}
			}
void USB_INT_Clear_(const uint8_t Interrupt)
	{
	switch (Interrupt)
		{
		case USB_INT_WAKEUPI_:	UDINT  &= ~(1 << WAKEUPI);
								break;
		case USB_INT_SUSPI_:		UDINT  &= ~(1 << SUSPI);
								break;
		case USB_INT_EORSTI_:	UDINT  &= ~(1 << EORSTI);
								break;
		case USB_INT_SOFI_:		UDINT  &= ~(1 << SOFI);
								break;
		case USB_INT_RXSTPI_:	UEINTX &= ~(1 << RXSTPI);
								break;
								
		case USB_INT_TXINI_:	UEINTX &=~(1 << TXINI);
								break;
		default:	break;
		}
	}
bool Endpoint_ConfigureEndpoint_(const uint8_t Address,const uint8_t Type,const uint16_t Size,const uint8_t Banks)
	{

	uint8_t Number = (Address & ENDPOINT_EPNUM_MASK_);
	if (Number >= ENDPOINT_TOTAL_ENDPOINTS_)return false;
	
	return Endpoint_ConfigureEndpoint_Prv_(Number,
			((Type << EPTYPE0) | ((Address & my_ENDPOINT_DIR_IN) ? (1 << EPDIR) : 0)),
			((1 << ALLOC) | ((Banks > 1) ? (1 << EPBK0) : 0) | Endpoint_BytesToEPSizeMask_(Size)));
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
bool Endpoint_ConfigureEndpoint_Prv_(const uint8_t Number,
                                    const uint8_t UECFG0XData,
                                    const uint8_t UECFG1XData)
{
	uint8_t EPNum; 
	for (EPNum = Number; EPNum < ENDPOINT_TOTAL_ENDPOINTS_; EPNum++)
	{
		uint8_t UECFG0XTemp;
		uint8_t UECFG1XTemp;
		uint8_t UEIENXTemp;

		Endpoint_SelectEndpoint_(EPNum);

		if (EPNum == Number)
		{
			UECFG0XTemp = UECFG0XData;
			UECFG1XTemp = UECFG1XData;
			UEIENXTemp  = 0;
			
		}
		else
		{
			UECFG0XTemp = UECFG0X;
			UECFG1XTemp = UECFG1X;
			UEIENXTemp  = UEIENX;
		}

		if (!(UECFG1XTemp & (1 << ALLOC)))
		  continue;

		Endpoint_DisableEndpoint_();
		UECFG1X &= ~(1 << ALLOC);

		Endpoint_EnableEndpoint_();
		UECFG0X = UECFG0XTemp;
		UECFG1X = UECFG1XTemp;
		UEIENX  = UEIENXTemp;

		if(!(Endpoint_IsConfigured_()))return false;
	}

	Endpoint_SelectEndpoint_(Number);
	return true;
}

bool Device_ConfigureEndpoints_(My_USB_Info_Device_t* const my_InterfaceInfo)
{
	Endpoint_ConfigureEndpoint_(0x82, 2,64, 1);
	Endpoint_ConfigureEndpoint_(0x1, 2, 16, 1);

	return true;
}





uint16_t USB_Device_GetFrameNumber_(void)
			{
				return UDFNUM;
			}






uint8_t Endpoint_WaitUntilReady_(void)
{
	uint8_t  TimeoutMSRem = USB_STREAM_TIMEOUT_MS_;
	uint16_t PreviousFrameNumber = USB_Device_GetFrameNumber_();
	for (;;)
	{
		if (Endpoint_GetEndpointDirection_() == my_ENDPOINT_DIR_IN)
		{
			if (Endpoint_IsINReady_())
			  return ENDPOINT_READYWAIT_NoError_;
		}
		else
		{
			if (Endpoint_IsOUTReceived_())
			  return ENDPOINT_READYWAIT_NoError_;
		}

		uint8_t USB_DeviceState_LCL = USB_DeviceState_;

		if (USB_DeviceState_LCL == DEVICE_STATE_Unattached_)
		  return ENDPOINT_READYWAIT_DeviceDisconnected_;
		else if (USB_DeviceState_LCL == DEVICE_STATE_Suspended_)
		  return ENDPOINT_READYWAIT_BusSuspended_;
		else if (Endpoint_IsStalled_())
		  return ENDPOINT_READYWAIT_EndpointStalled_;

		uint16_t CurrentFrameNumber = USB_Device_GetFrameNumber_();

		if (CurrentFrameNumber != PreviousFrameNumber)
		{
			PreviousFrameNumber = CurrentFrameNumber;

			if (!(TimeoutMSRem--))
			  return ENDPOINT_READYWAIT_Timeout_;
		}
	}
}

uint8_t my_Device_Flush_(My_USB_Info_Device_t* const my_InterfaceInfo)
{
	if ((USB_DeviceState_ != DEVICE_STATE_Configured_) || !(my_InterfaceInfo->My_State.My_LineEncoding.my_BaudRateBPS))
	  return ENDPOINT_RWSTREAM_DeviceDisconnected_;

	uint8_t ErrorCode;

	Endpoint_SelectEndpoint_(my_InterfaceInfo->My_Config.my_DataINEndpoint.my_Address);

	if (!(Endpoint_BytesInEndpoint_()))
	  return ENDPOINT_READYWAIT_NoError_;

	bool BankFull = !(Endpoint_IsReadWriteAllowed_());

	Endpoint_ClearIN_();

	if (BankFull)
	{
		if ((ErrorCode = Endpoint_WaitUntilReady_()) != ENDPOINT_READYWAIT_NoError_)
		  return ErrorCode;

		Endpoint_ClearIN_();
	}

	return ENDPOINT_READYWAIT_NoError_;
}

void USB_Device_GetSerialString_(uint16_t* const UnicodeString)
			{
				uint8_t CurrentGlobalInt = GetGlobalInterruptMask_();
				GlobalInterruptDisable_();

				uint8_t SigReadAddress = INTERNAL_SERIAL_START_ADDRESS_;
				uint8_t SerialCharNum;
				for (SerialCharNum = 0; SerialCharNum < (INTERNAL_SERIAL_LENGTH_BITS_ / 4); SerialCharNum++)
				{
					uint8_t SerialByte = boot_signature_byte_get(SigReadAddress);

					if (SerialCharNum & 0x01)
					{
						SerialByte >>= 4;
						SigReadAddress++;
					}

					SerialByte &= 0x0F;

					UnicodeString[SerialCharNum] = my_cpu_to_le16((SerialByte >= 10) ?
															   (('A' - 10) + SerialByte) : ('0' + SerialByte));
				}

				SetGlobalInterruptMask_(CurrentGlobalInt);
			}
void USB_Device_SetAddress_(void)
{
	uint8_t DeviceAddress = (my_USB_ControlRequest.my_wValue & 0x7F);
	USB_Device_SetDeviceAddress_(DeviceAddress);
	Endpoint_ClearSETUP_();
	Endpoint_ClearStatusStage_();
	while (!(Endpoint_IsINReady_()));
	USB_Device_EnableDeviceAddress_(DeviceAddress);
	USB_DeviceState_ = (DeviceAddress) ? DEVICE_STATE_Addressed_ : DEVICE_STATE_Default_;
}

uint32_t Endpoint_Read_32_LE_(void)
			{
				union
				{
					uint32_t Value;
					uint8_t  Bytes[4];
				} Data;

				Data.Bytes[0] = UEDATX;
				Data.Bytes[1] = UEDATX;
				Data.Bytes[2] = UEDATX;
				Data.Bytes[3] = UEDATX;

				return Data.Value;
			}
bool Endpoint_ConfigureEndpointTable_(const my_USB_Endpoint_Table_t* const Table,
                                     const uint8_t Entries)
{

	uint8_t i;
	for (i = 0; i < Entries; i++)
	{
		if (!(Table[i].my_Address))
		  continue;
 		if (!(Endpoint_ConfigureEndpoint_(Table[i].my_Address, Table[i].my_Type, Table[i].my_Size, Table[i].my_Banks)))
		  return false;
	}
	return true;
}
uint16_t CALLBACK_USB_GetDescriptor_(const uint16_t my_wValue,
                                    const uint16_t my_wIndex,
                                    void** const  DescriptorAddress)
{
	const uint8_t  DescriptorType   = (my_wValue >> 8);
	const uint8_t  DescriptorNumber = (my_wValue & 0xFF);
	const void* Address = NULL;
	uint16_t    Size    = my_NO_DESCRIPTOR;
	switch (DescriptorType)
	{
		case my_DTYPE_Device:
			Address = &my_DeviceDescriptor;
			Size    = sizeof(my_USB_Descriptor_Device_t);
			break;
		case my_DTYPE_Configuration:
			Address = &my_ConfigurationDescriptor;
			Size    = sizeof(my_USB_Descriptor_Configuration_t);
			break;
		case my_DTYPE_String:
			switch (DescriptorNumber)
			{
				case my_STRING_ID_Language:
					Address = &my_LanguageString;
					Size    = pgm_read_byte(&my_LanguageString.my_Header.my_Size);
					break;
				case my_STRING_ID_Manufacturer:
					Address = &my_ManufacturerString;
					Size    = pgm_read_byte(&my_ManufacturerString.my_Header.my_Size);
					break;
				case my_Empty:
					Address = &my_EmptyString;
					Size    = pgm_read_byte(&my_EmptyString.my_Header.my_Size);
					break;
				case my_STRING_ID_Product:
					Address = &my_ProductString;
					Size    = pgm_read_byte(&my_ProductString.my_Header.my_Size);
					break;	
				case 8:break;
				case 9:break;
				case 10:break;
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
	if (my_USB_ControlRequest.my_wValue == ((my_DTYPE_String << 8) | my_USE_INTERNAL_SERIAL))
	{
		USB_Device_GetInternalSerialDescriptor_();
		return;
	}
	
	if ((DescriptorSize = CALLBACK_USB_GetDescriptor_(my_USB_ControlRequest.my_wValue, my_USB_ControlRequest.my_wIndex,
	                                                 (void**)&DescriptorPointer)) == my_NO_DESCRIPTOR)return;
	Endpoint_ClearSETUP_();
	Endpoint_Write_Control_PStream_LE_(DescriptorPointer, DescriptorSize);
	Endpoint_ClearOUT_();
}





void USB_Device_GetStatus_(void)
{
	//LEDS_ON;
	uint8_t CurrentStatus = 0;
	switch (my_USB_ControlRequest.my_bmRequestType)
	{
		case (REQDIR_DEVICETOHOST_ | REQTYPE_STANDARD_ | REQREC_DEVICE_):
		{
			if (USB_Device_CurrentlySelfPowered_)
			  CurrentStatus |= FEATURE_SELFPOWERED_ENABLED_;
			if (USB_Device_RemoteWakeupEnabled_)
			  CurrentStatus |= FEATURE_REMOTE_WAKEUP_ENABLED_;
			break;
		}
		case (REQDIR_DEVICETOHOST_ | REQTYPE_STANDARD_ | REQREC_ENDPOINT_):
		{
			uint8_t EndpointIndex = ((uint8_t)my_USB_ControlRequest.my_wIndex & ENDPOINT_EPNUM_MASK_);
			if (EndpointIndex >= ENDPOINT_TOTAL_ENDPOINTS_)
				return;
			Endpoint_SelectEndpoint_(EndpointIndex);
			CurrentStatus = Endpoint_IsStalled_();
			Endpoint_SelectEndpoint_(ENDPOINT_CONTROLEP_);
			break;
		}
		default:
			return;
	}

	Endpoint_ClearSETUP_();
	Endpoint_Write_16_LE_(CurrentStatus);
	Endpoint_ClearIN_();
	Endpoint_ClearStatusStage_();
}




uint16_t CALLBACK_USB_GetDescriptor(const uint16_t my_wValue,const uint16_t my_wIndex,void** const  DescriptorAddress){return 0;}//estern

void USB_Device_GetInternalSerialDescriptor_(void)
{
	struct
	{
		my_USB_Descriptor_Header_t my_Header;
		uint16_t                UnicodeString[INTERNAL_SERIAL_LENGTH_BITS_ / 4];
	} SignatureDescriptor;
	SignatureDescriptor.my_Header.my_Type = my_DTYPE_String;
	SignatureDescriptor.my_Header.my_Size = USB_STRING_LEN_(INTERNAL_SERIAL_LENGTH_BITS_ / 4);
	USB_Device_GetSerialString_(SignatureDescriptor.UnicodeString);
	Endpoint_ClearSETUP_();
	Endpoint_Write_Control_Stream_LE_(&SignatureDescriptor, sizeof(SignatureDescriptor));
	Endpoint_ClearOUT_();
}

void USB_INT_Disable_(const uint8_t Interrupt)
			{
				switch (Interrupt)
				{
					case USB_INT_WAKEUPI_:
						UDIEN  &= ~(1 << WAKEUPE);
						break;
					case USB_INT_SUSPI_:
						UDIEN  &= ~(1 << SUSPE);
						break;
					case USB_INT_EORSTI_:
						UDIEN  &= ~(1 << EORSTE);
						break;
					case USB_INT_SOFI_:
						UDIEN  &= ~(1 << SOFE);
						break;
					case USB_INT_RXSTPI_:
						UEIENX &= ~(1 << RXSTPE);
						break;
					case USB_INT_TXINI_:
						UEIENX &= ~(1 << TXINE);
						break;
					default:
						break;
				}
			}
			
			
			
			
			
uint8_t Endpoint_Write_Control_Stream_LE_ (const void* const Buffer,uint16_t Length)
{
#define  TEMPLATE_BUFFER_OFFSET__(Length)            0
#define  TEMPLATE_BUFFER_MOVE__(BufferPtr, Amount)   BufferPtr += Amount
#define  TEMPLATE_TRANSFER_BYTE__(BufferPtr)         Endpoint_Write_8_(*BufferPtr)
	uint8_t* DataStream     = ((uint8_t*)Buffer + TEMPLATE_BUFFER_OFFSET__(Length));
	bool     LastPacketFull = false;
	if (Length > my_USB_ControlRequest.my_wLength)
	  Length = my_USB_ControlRequest.my_wLength;
	else if (!(Length))
	  Endpoint_ClearIN_();

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
			uint16_t BytesInEndpoint = Endpoint_BytesInEndpoint_();

			while (Length && (BytesInEndpoint < USB_Device_ControlEndpointSize_))
			{
				TEMPLATE_TRANSFER_BYTE__(DataStream);
				TEMPLATE_BUFFER_MOVE__(DataStream, 1);
				Length--;
				BytesInEndpoint++;
			}

			LastPacketFull = (BytesInEndpoint == USB_Device_ControlEndpointSize_);
			Endpoint_ClearIN_();
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
			
			