		#include <avr/io.h>		
		#include <avr/pgmspace.h>
					#define SERIAL_2X_UBBRVAL(Baud) ((((F_CPU / 8) + (Baud / 2)) / (Baud)) - 1)

			#define SERIAL_UBBRVAL(Baud)    ((((F_CPU / 16) + (Baud / 2)) / (Baud)) - 1)
		#include <stdbool.h>
		#include <string.h>
		#include <avr/interrupt.h>
		#include <avr/boot.h>
		#include <avr/wdt.h>
        #define F_CPU         16000000UL
		#include <util/delay.h>
///////////////////////////////////////////////////////////
const uint8_t next_is_IN_1[]={0x9,0x1};
const uint8_t start_IN_1[]={0x9,0x1,0x0,0x0};
const uint8_t end_bulk_1[]={0x9,0x2,0x0,0x1};
const uint8_t next_is_bulk_2[]={0x3,0x2,0x1,0x0};
uint32_t bulks=0;
uint32_t timer_INs=0;
uint32_t timer_GENERAL=0;
uint32_t timer_LEDS=0;
uint32_t timer_LED1=0;
uint32_t timer_LED2=0;

        #define me_F_CPU         16000000UL
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
		#define USB_STREAM_TIMEOUT_MS_       100
		#define AVR_RESET_LINE_MASK_ (1 << 7)
		#define MIN_(x, y)               (((x) < (y)) ? (x) : (y))
		#define SERIAL_2X_UBBRVAL_(Baud) ((((me_F_CPU / 8) + (Baud / 2)) / (Baud)) - 1)
		#define USB_DeviceState_            CONCAT_EXPANDED_(GPIOR, 0)
		#define CONCAT_(x, y)            x ## y
		#define CONCAT_EXPANDED_(x, y)   CONCAT_(x, y)
		#define ENDPOINT_EPNUM_MASK_                     0x0F
		#define ENDPOINT_CONTROLEP_                      0
		#define REQDIR_DEVICETOHOST_        (1 << 7)
		#define REQDIR_HOSTTODEVICE_        (0 << 7)
		#define REQTYPE_STANDARD_           (0 << 5)
		#define REQTYPE_CLASS_              (1 << 5)
		#define REQTYPE_VENDOR_             (2 << 5)
		#define REQREC_DEVICE_              (0 << 0)
		#define REQREC_INTERFACE_           (1 << 0)
		#define REQREC_ENDPOINT_            (2 << 0)
		#define REQREC_OTHER_               (3 << 0)
		#define CONTROL_REQTYPE_RECIPIENT_  0x1F
		#define ENDPOINT_TOTAL_ENDPOINTS_        5
		#define FEATURE_SELFPOWERED_ENABLED_     (1 << 0)
		#define FEATURE_REMOTE_WAKEUP_ENABLED_   (1 << 1)
		#define INTERNAL_SERIAL_LENGTH_BITS_     80
		#define GCC_MEMORY_BARRIER_()                  __asm__ __volatile__("" ::: "memory");
		#define GCC_FORCE_POINTER_ACCESS_(StructPtr)   __asm__ __volatile__("" : "=b" (StructPtr) : "0" (StructPtr))
		#define USB_STRING_LEN_(UnicodeChars)      (sizeof(my_USB_Descriptor_Header_t) + ((UnicodeChars) << 1))
		#define INTERNAL_SERIAL_START_ADDRESS_   0x0E
		#define USB_Device_ControlEndpointSize_ 16//per peak usb//8
		#define EP_TYPE_CONTROL_  		0x00
		#define USB_CurrentMode_ 		USB_MODE_Device_
		#define USB_OPT_MANUAL_PLL_                 (1 << 2)
		#define USB_Options_ (USB_DEVICE_OPT_FULLSPEED_|USB_OPT_REG_ENABLED_|USB_OPT_AUTO_PLL_)
		#define USB_DEVICE_OPT_FULLSPEED_               (0 << 0)
		#define USB_OPT_REG_ENABLED_                (0 << 1)
		#define USB_OPT_AUTO_PLL_                   (0 << 2)
		#define USB_PLL_PSC_                (1 << PLLP0)
		#define USB_OPT_REG_DISABLED_               (1 << 1)
		#define my_cpu_to_le16(x)           (x)
		#define my_CDC_CONTROL_LINE_OUT_DTR         (1 << 0)
		#define my_ENDPOINT_DIR_OUT                   0x00
		#define my_ENDPOINT_DIR_IN                    0x80
		#define my_RX_EPADDR	(my_ENDPOINT_DIR_OUT | 1)
		#define my_TX_EPADDR	(my_ENDPOINT_DIR_IN  | 1)
		#define my_RX_EPADDR2	(my_ENDPOINT_DIR_OUT | 2)
		#define my_TX_EPADDR2	(my_ENDPOINT_DIR_IN  | 2)
		#define my_POLLING_INTERVAL_MS 20
		#define my_TXRX_EPSIZE                16	
		#define my_NOTIFICATION_EPADDR        (my_ENDPOINT_DIR_IN  | 2)
		#define my_NOTIFICATION_EPSIZE        8
		#define my_NO_DESCRIPTOR                     0
		#define my_USB_CONFIG_ATTR_RESERVED          0x80
		#define my_USB_CONFIG_ATTR_SELFPOWERED       0x40
		#define my_USB_CONFIG_POWER_MA(mA)           ((mA) >> 1)
		#define my_EP_TYPE_INTERRUPT                  0x03
		#define my_ENDPOINT_ATTR_NO_SYNC             (0 << 2)
		#define my_ENDPOINT_USAGE_DATA               (0 << 4)
		#define my_EP_TYPE_BULK                       0x02
		#define my_CPU_TO_LE16(x)           (x)
		#define my_VERSION_BCD(Major,Minor,Revision)my_CPU_TO_LE16(((Major&0xFF)<<8)|((Minor&0x0F)<<4)|(Revision&0x0F))
		#define my_USE_INTERNAL_SERIAL         0x0//0xDC
		#define my_USB_STRING_DESCRIPTOR_ARRAY(...)  { .my_Header = {.my_Size = sizeof(my_USB_Descriptor_Header_t) + sizeof((uint16_t[]){__VA_ARGS__}), .my_Type = my_DTYPE_String}, .my_UnicodeString = {__VA_ARGS__} }
		#define my_LANGUAGE_ID_ENG                   0x0409
		#define my_USB_STRING_DESCRIPTOR(my_String)     { .my_Header = {.my_Size = sizeof(my_USB_Descriptor_Header_t) + (sizeof(my_String) - 2), .my_Type = my_DTYPE_String}, .my_UnicodeString = my_String }
///////////////////////////////////////////////////////////////////////


enum CDC_ClassRequests_t_
	{
	CDC_REQ_SendEncapsulatedCommand_ = 0x00, 
	CDC_REQ_GetEncapsulatedResponse_ = 0x01, 
	CDC_REQ_SetLineEncoding_         = 0x20, 
	CDC_REQ_GetLineEncoding_         = 0x21, 
	CDC_REQ_SetControlLineState_     = 0x22, 
	};
enum CDC_LineEncodingParity_t_
	{
	CDC_PARITY_None_  = 0, 
	CDC_PARITY_Odd_   = 1, 
	CDC_PARITY_Even_  = 2, 
	};
enum Endpoint_WaitUntilReady_ErrorCodes_t_
	{
	ENDPOINT_READYWAIT_NoError_                 = 0, 
	ENDPOINT_READYWAIT_EndpointStalled_         = 1,
	ENDPOINT_READYWAIT_DeviceDisconnected_      = 2,	
	ENDPOINT_READYWAIT_BusSuspended_            = 3, 
	ENDPOINT_READYWAIT_Timeout_                 = 4, 
	};
enum Endpoint_Stream_RW_ErrorCodes_t_
	{
	ENDPOINT_RWSTREAM_NoError_            = 0, 
	ENDPOINT_RWSTREAM_EndpointStalled_    = 1, 
	ENDPOINT_RWSTREAM_DeviceDisconnected_ = 2, 
	};
enum USB_Control_Request_t_
	{
	REQ_GetStatus_           = 0, 
	REQ_ClearFeature_        = 1, 
	REQ_SetFeature_          = 3, 
	REQ_SetAddress_          = 5, 
	REQ_GetDescriptor_       = 6, 
	REQ_SetDescriptor_       = 7, 
	REQ_GetConfiguration_    = 8, 
	REQ_SetConfiguration_    = 9, 
	my_REQ_VendorRequest		= 0xC3,	
	};
enum USB_Feature_Selectors_t_
	{
	FEATURE_SEL_EndpointHalt_       = 0x00, 
	FEATURE_SEL_DeviceRemoteWakeup_ = 0x01, 
	};
enum Endpoint_ControlStream_RW_ErrorCodes_t_
	{
	ENDPOINT_RWCSTREAM_NoError_            = 0, 
	ENDPOINT_RWCSTREAM_HostAborted_        = 1, 
	ENDPOINT_RWCSTREAM_DeviceDisconnected_ = 2, 
	ENDPOINT_RWCSTREAM_BusSuspended_       = 3, 
	};
enum USB_Interrupts_t_
	{
	USB_INT_WAKEUPI_ = 2,
	USB_INT_SUSPI_   = 3,
	USB_INT_EORSTI_  = 4,
	USB_INT_SOFI_    = 5,
	USB_INT_RXSTPI_  = 6,
	USB_INT_TXINI_  = 7,
	USB_INT_RXOUTI_  = 8,
	};
enum USB_Modes_t_
	{
	USB_MODE_None_   = 0,
	USB_MODE_Device_ = 1,
	USB_MODE_Host_   = 2,
	};
enum my_USB_Device_States_t
	{
	DEVICE_STATE_Unattached_                   = 0, 
	DEVICE_STATE_Powered_                      = 1, 
	DEVICE_STATE_Default_                      = 2, 
	DEVICE_STATE_Addressed_                    = 3, 
	DEVICE_STATE_Configured_                   = 4, 
	DEVICE_STATE_Suspended_                    = 5, 
	};
enum my_CDC_LineEncodingFormats_t
	{
	my_CDC_LINEENCODING_OneStopBit          = 0, 
	my_CDC_LINEENCODING_OneAndAHalfStopBits = 1,
	my_CDC_LINEENCODING_TwoStopBits         = 2, 
	};
enum my_InterfaceDescriptors_t
	{
	my_INTERFACE_ID_CCI = 0, 
	my_INTERFACE_ID_CDC_DCI = 1
	};
enum my_StringDescriptors_t
	{
	my_STRING_ID_Language     = 0, 
	my_STRING_ID_Manufacturer = 1, 
	my_Empty = 2, 
	my_STRING_ID_Product      = 3, 
	};
enum USB_DescriptorTypes_t_
	{
	my_DTYPE_Device                    = 0x01, 
	my_DTYPE_Configuration             = 0x02, 
	my_DTYPE_String                    = 0x03, 
	my_DTYPE_Interface                 = 0x04,
	my_DTYPE_Endpoint                  = 0x05,
	};
enum my_CDC_DescriptorTypes_t
	{
	my_CDC_DTYPE_CSInterface                     = 0x24,
	};
enum my_CDC_DescriptorSubtypes_t
	{
	my_CDC_DSUBTYPE_CSInterface_Header           = 0x00, 
	my_CDC_DSUBTYPE_CSInterface_CallManagement   = 0x01, 
	my_CDC_DSUBTYPE_CSInterface_ACM              = 0x02, 
	my_CDC_DSUBTYPE_CSInterface_DirectLine       = 0x03, 
	my_CDC_DSUBTYPE_CSInterface_TelephoneRinger  = 0x04, 
	my_CDC_DSUBTYPE_CSInterface_TelephoneCall    = 0x05, 
	my_CDC_DSUBTYPE_CSInterface_Union            = 0x06, 
	};
enum my_CDC_Descriptor_ClassSubclassProtocol_t
	{
	my_CDC_CSCP_CDCClass               = 0x02, 
	my_CDC_CSCP_NoSpecificSubclass     = 0x00, 
	my_CDC_CSCP_ACMSubclass            = 0x02, 
	my_CDC_CSCP_ATCommandProtocol      = 0x01, 
	my_CDC_CSCP_NoSpecificProtocol     = 0x00, 
	my_CDC_CSCP_VendorSpecificProtocol = 0xFF, 
	my_CDC_CSCP_CDCDataClass           = 0x0A, 
	my_CDC_CSCP_NoDataSubclass         = 0x00, 
	my_CDC_CSCP_NoDataProtocol         = 0x00, 
	};
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
typedef struct
	{
	uint8_t  my_bmRequestType; 
	uint8_t  my_bRequest; 
	uint16_t my_wValue; 
	uint16_t my_wIndex; 
	uint16_t my_wLength; 
	}my_USB_Request_Header_t;
typedef struct
	{
	uint8_t* In; 
	uint8_t* Out; 
	uint8_t* Start;
	uint8_t* End; 
	uint16_t Size; 
	uint16_t Count; 
	} my_RingBuffer_t;
typedef struct 
	{
	uint32_t my_BaudRateBPS;
	uint8_t  my_CharFormat; 
	uint8_t  my_ParityType; 
	uint8_t  my_DataBits; 
	}my_LineEncoding_t;
typedef struct
	{
	uint8_t  my_Address;
	uint16_t my_Size;
	uint8_t  my_Type;
	uint8_t  my_Banks;
	} my_USB_Endpoint_Table_t;
typedef struct
	{
	uint8_t my_Size; 
	uint8_t my_Type; 
	}my_USB_Descriptor_Header_t;
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
typedef struct
	{
	my_USB_Descriptor_Header_t my_Header; 
	uint16_t my_TotalConfigurationSize; 
	uint8_t  my_TotalInterfaces; 
    uint8_t  my_ConfigurationNumber; 
	uint8_t  my_ConfigurationStrIndex; 
	uint8_t  my_ConfigAttributes; 
	uint8_t  my_MaxPowerConsumption; 
	}my_USB_Descriptor_Configuration_Header_t;
typedef struct
	{
	my_USB_Descriptor_Header_t my_Header;
	uint8_t                 my_Subtype; 
	uint16_t                my_CDCSpecification; 
	}my_USB_CDC_Descriptor_FunctionalHeader_t;
typedef struct
	{
	my_USB_Descriptor_Header_t my_Header; 
	uint8_t                 my_Subtype; 
	uint8_t                 my_Capabilities; 
	}my_USB_CDC_Descriptor_FunctionalACM_t;
typedef struct
	{
	my_USB_Descriptor_Header_t my_Header; 
	uint8_t                 my_Subtype; 
	uint8_t                 my_MasterInterfaceNumber; 
	uint8_t                 my_SlaveInterfaceNumber; 
	}my_USB_CDC_Descriptor_FunctionalUnion_t;
typedef struct
	{
	my_USB_Descriptor_Header_t my_Header; 
	uint8_t  my_EndpointAddress; 
	uint8_t  my_Attributes; 
	uint16_t my_EndpointSize; 
	uint8_t  my_PollingIntervalMS;
	}my_USB_Descriptor_Endpoint_t;
typedef struct
	{
	my_USB_Descriptor_Header_t my_Header; 
	uint8_t my_InterfaceNumber; 
	uint8_t my_AlternateSetting; 
	uint8_t my_TotalEndpoints; 
	uint8_t my_Class; 
	uint8_t my_SubClass; 
	uint8_t my_Protocol; 
	uint8_t my_InterfaceStrIndex; 
	}my_USB_Descriptor_Interface_t;
typedef struct
	{
	my_USB_Descriptor_Header_t my_Header;
	uint16_t my_USBSpecification; 
	uint8_t  my_Class; 
	uint8_t  my_SubClass; 
	uint8_t  my_Protocol; 
	uint8_t  my_Endpoint0Size; 
	uint16_t my_VendorID; 
	uint16_t my_ProductID;
	uint16_t my_ReleaseNumber; 
	uint8_t  my_ManufacturerStrIndex; 
	uint8_t  my_ProductStrIndex; 
	uint8_t  my_SerialNumStrIndex; 
	uint8_t  my_NumberOfConfigurations; 
	} my_USB_Descriptor_Device_t;
typedef struct
	{
	my_USB_Descriptor_Header_t my_Header;
	wchar_t  my_UnicodeString[256];
	}my_USB_Descriptor_String_t;
typedef struct
	{
	struct
		{
		uint8_t my_ControlInterfaceNumber; 
		my_USB_Endpoint_Table_t my_DataINEndpoint; 
		my_USB_Endpoint_Table_t my_DataOUTEndpoint; 
		//my_USB_Endpoint_Table_t my_DataINEndpoint2; 
		//my_USB_Endpoint_Table_t my_DataOUTEndpoint2; 
		my_USB_Endpoint_Table_t my_NotificationEndpoint; 
		} My_Config; 
	struct
		{
		struct
			{
			uint16_t my_HostToDevice; 
			uint16_t my_DeviceToHost; 
			} My_ControlLineStates; 
		my_LineEncoding_t My_LineEncoding;
		} My_State; 
	} My_USB_Info_Device_t;
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
typedef struct//config 1
{
    my_USB_Descriptor_Configuration_Header_t my_Config;
    my_USB_Descriptor_Interface_t my_Interface;

	// 81: Вход, Bulk, 16 байт
    my_USB_Descriptor_Endpoint_t my_DataINEndpoint;   
	// 01: Выход, Bulk, 16 байт
    my_USB_Descriptor_Endpoint_t my_DataOutEndpoint;  
	// 82: Вход, Bulk, 64 байта
    my_USB_Descriptor_Endpoint_t my_DataINEndpoint2;  
	// 02: Выход, Bulk, 64 байта
    my_USB_Descriptor_Endpoint_t my_DataOutEndpoint2; 
} my_USB_Descriptor_Configuration_t;
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
my_USB_Request_Header_t my_USB_ControlRequest;
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

My_USB_Info_Device_t my_Virtual_Interface =
	{
	.My_Config =
		{
		.my_ControlInterfaceNumber         = my_INTERFACE_ID_CCI,
		.my_DataINEndpoint                 =
			{
			.my_Address                = my_TX_EPADDR,
			.my_Size                   = my_TXRX_EPSIZE,
			.my_Banks                  = 1,
			},
		.my_DataOUTEndpoint                =
			{
			.my_Address                = my_RX_EPADDR,
			.my_Size                   = my_TXRX_EPSIZE,
			.my_Banks                  = 1,
			},
		/*.my_DataINEndpoint2                 =
			{
			.my_Address                = my_TX_EPADDR2,
			.my_Size                   = my_TXRX_EPSIZE,
			.my_Banks                  = 1,
			},
		.my_DataOUTEndpoint2                =
			{
			.my_Address                = my_RX_EPADDR2,
			.my_Size                   = my_TXRX_EPSIZE,
			.my_Banks                  = 1,
			},*/
		.my_NotificationEndpoint           =
			{
			.my_Address                = my_NOTIFICATION_EPADDR,
			.my_Size                   = my_NOTIFICATION_EPSIZE,
			.my_Banks                  = 1,
			},
		},
	};
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

const my_USB_Descriptor_Device_t PROGMEM my_DeviceDescriptor =
	{
	.my_Header	= 
		{
		.my_Size = sizeof(my_USB_Descriptor_Device_t), .my_Type = my_DTYPE_Device
		},
	.my_USBSpecification       = my_VERSION_BCD(1,0,0),
	.my_Class                  = 0x00,//my_CDC_CSCP_CDCClass,
	.my_SubClass               = 0x00,//my_CDC_CSCP_NoSpecificSubclass,
	.my_Protocol               = 0x00,//my_CDC_CSCP_NoSpecificProtocol,
	.my_Endpoint0Size          = 16,//8
	.my_VendorID               =  0x0C72,//0x2341,
	.my_ProductID          	  = 0x000C,//0x0001, 
	.my_ReleaseNumber          = my_VERSION_BCD(0x1C, 0, 0),
	.my_ManufacturerStrIndex   = 0x0,//my_STRING_ID_Manufacturer,
	.my_ProductStrIndex        = 0x3,//my_STRING_ID_Product,
	.my_SerialNumStrIndex      = my_USE_INTERNAL_SERIAL,
	.my_NumberOfConfigurations = 3//1
	};
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
const my_USB_Descriptor_Configuration_t PROGMEM my_ConfigurationDescriptor =
	{
	.my_Config = 
		{
		.my_Header                 = 
			{
			.my_Size = sizeof(my_USB_Descriptor_Configuration_Header_t),
			.my_Type = my_DTYPE_Configuration
			},
		.my_TotalConfigurationSize = sizeof(my_USB_Descriptor_Configuration_t),
		.my_TotalInterfaces        = 1,
		.my_ConfigurationNumber    = 1,
		.my_ConfigurationStrIndex  = my_NO_DESCRIPTOR,
		.my_ConfigAttributes       = (my_USB_CONFIG_ATTR_RESERVED),
		.my_MaxPowerConsumption    = my_USB_CONFIG_POWER_MA(200)
		},
	.my_Interface = 
		{
		.my_Header                 = 
			{
			.my_Size = sizeof(my_USB_Descriptor_Interface_t), 
			.my_Type = my_DTYPE_Interface
			},
		.my_InterfaceNumber        = 0,
		.my_AlternateSetting       = 0,
		.my_TotalEndpoints         = 4,
		.my_Class                  = 0x0,
		.my_SubClass               = 0x0,
		.my_Protocol               = 0x0,
		.my_InterfaceStrIndex      = my_NO_DESCRIPTOR
		},
	.my_DataINEndpoint =
		{
		.my_Header = 
			{
			.my_Size = sizeof(my_USB_Descriptor_Endpoint_t), 
			.my_Type = my_DTYPE_Endpoint
			},
		.my_EndpointAddress        = my_TX_EPADDR,//IN
		.my_Attributes             = (my_EP_TYPE_BULK ),
		.my_EndpointSize           = my_TXRX_EPSIZE,
		.my_PollingIntervalMS      = my_POLLING_INTERVAL_MS
		},
	.my_DataOutEndpoint =
		{
		.my_Header = 
			{
			.my_Size = sizeof(my_USB_Descriptor_Endpoint_t), 
			.my_Type = my_DTYPE_Endpoint
			},
		.my_EndpointAddress        = my_RX_EPADDR,//out
		.my_Attributes             = (my_EP_TYPE_BULK),
		.my_EndpointSize           = my_TXRX_EPSIZE,
		.my_PollingIntervalMS      = my_POLLING_INTERVAL_MS
		},
	.my_DataINEndpoint2 =
		{
		.my_Header = 
			{
			.my_Size = sizeof(my_USB_Descriptor_Endpoint_t), 
			.my_Type = my_DTYPE_Endpoint
			},
		.my_EndpointAddress = my_TX_EPADDR2,  // IN2
		.my_Attributes = (my_EP_TYPE_BULK),
		.my_EndpointSize = 64,//
		.my_PollingIntervalMS = 1,//my_POLLING_INTERVAL_MS
		},	
	.my_DataOutEndpoint2 =
		{
		.my_Header = 
			{
			.my_Size = sizeof(my_USB_Descriptor_Endpoint_t), 
			.my_Type = my_DTYPE_Endpoint
			},
		.my_EndpointAddress = my_RX_EPADDR2,  // OUT2
		.my_Attributes = (my_EP_TYPE_BULK),
		.my_EndpointSize = 64,//my_TXRX_EPSIZE, 
		.my_PollingIntervalMS = 1,//my_POLLING_INTERVAL_MS
		},
};



const my_USB_Descriptor_String_t PROGMEM my_LanguageString = my_USB_STRING_DESCRIPTOR_ARRAY(my_LANGUAGE_ID_ENG);
const my_USB_Descriptor_String_t PROGMEM my_ManufacturerString = my_USB_STRING_DESCRIPTOR(L\
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\
\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\
\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\
\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\
\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\
\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\
\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\
\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
);
const my_USB_Descriptor_String_t PROGMEM my_EmptyString = my_USB_STRING_DESCRIPTOR(L\
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\
\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\
\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\
\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\
\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\
\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\
\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\
\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
);


const my_USB_Descriptor_String_t PROGMEM my_ProductString = my_USB_STRING_DESCRIPTOR(L\
"\x56\x45\x52\x31\x3A\x50\x45\x41\x4B\x20\x20\x20\x20\x0A\x56\x45\
\x52\x32\x3A\x30\x32\x2E\x38\x2E\x30\x31\x20\x20\x20\x20\x20\x20\
\x20\x20\x0A\x44\x41\x54\x20\x3A\x30\x36\x2E\x30\x35\x2E\x32\x30\
\x30\x34\x0A\x54\x49\x4D\x45\x3A\x30\x39\x3A\x33\x35\x3A\x33\x37\
\x0A\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\
\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x2E\x2E\
\x2E\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\
\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
);

#define My_BULK_size 16
#define My_BULK_size_common 12
#define My_BULK_size_signific 4
#define My_BULK_size_signific_for_answer 3

uint8_t my_STRING_Vendor[]={0,0,2,8,1,9,0x23,0x25,6,5,0xD4,7,0,0,0,0};
uint8_t my_data_bulk[My_BULK_size];



//				_delay_ms(10);
//			Endpoint_ConfigureEndpoint_(0x81, 2, 16, 1);			
