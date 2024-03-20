const PROGMEM char my_STRING_Vendor[]={0,0,2,8,1,9,0x23,0x25,6,5,0xD4,7,0,0,0,0};
const char frst_sms_[]=    {0x02,0x01,0x42,0x04,0x01};
const PROGMEM char sms_1[]={0x02,0x02,0x42,0x04,0x01};
const PROGMEM char answer_standard_head[]={0x02,0x01};
char DLC=0;
uint8_t i_ring_hex=0;
uint16_t ring_hex[]=    {
	0xC9ED,
	0xC94D,
	0xC9AD,
	0xC90D,
	0xC96D,
	0xC9CD,
	0xC92D,
	0xC98D,
	};
char ID_standard[]={0x00,0x00};
uint16_t COBID=0;

char answer_timer[]={0,0};
const PROGMEM char answer_data[]=
{
	0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88
};


const PROGMEM char my_ManufacturerString[] = {12,3,'e',0,'l',0,'e',0,'g',0,'y',0};
const PROGMEM char my_EmptyString[] ={12,3,'e',0,'m',0,'p',0,'t',0,'y',0};
const PROGMEM char my_ProductString[] = {22,3,'P',0,'E',0,'A',0,'K',0,'-',0,'C',0,'L',0,'O',0,'N',0,'E',0};

enum USB_Control_Request_t_
	{
	REQ_GetStatus_          = 0, 
	REQ_ClearFeature_       = 1, 
	REQ_SetFeature_         = 3, 
	REQ_SetAddress_         = 5, 
	REQ_GetDescriptor_      = 6, 
	REQ_SetConfiguration_   = 9, 
	my_REQ_VendorRequest	= 0xC3,	
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

enum my_USB_Device_States_t
	{
	DEVICE_STATE_Unattached_                   = 0, 
	DEVICE_STATE_Powered_                      = 1, 
	DEVICE_STATE_Default_                      = 2, 
	DEVICE_STATE_Addressed_                    = 3, 
	DEVICE_STATE_Configured_                   = 4, 
	DEVICE_STATE_Suspended_                    = 5, 
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
	};

typedef struct
	{
	uint8_t  my_bmRequestType; 
	uint8_t  my_bRequest; 
	uint16_t my_wValue; 
	uint16_t my_wIndex; 
	uint16_t my_wLength; 
	}my_USB_Request_Header_t ;

my_USB_Request_Header_t my_USB_ControlRequest;

const PROGMEM char descriptor_PEAK[]=
	{	
	18,			//Size
	1,			//Type
	0x00,0x01,	//USBSpecification
	0x00,		//Class
	0x00,		//SubClass
	0x00,		//Protocol
	0x10,		//Endpoint0Size
	0x72,0x0C,	//VendorID
	0x0C,0x00,	//ProductID
	0x00,0x1C,	//ReleaseNumber
	0x00,		//ManufacturerStrIndex
	0x03,		//ProductStrIndex
	0x00,		//SerialNumStrIndex
	0x03		//NumberOfConfigurations
	};      
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
const PROGMEM char config_PEAK[]=	
	{
	9,		//Size
	2,		//Type
	0x2e,0,	//TotalConfigurationSize
	1,		//TotalInterfaces
	1,		//ConfigurationNumber
	0,		//ConfigurationStrIndex
	0x80,	//ConfigAttributes
	0x64,	//MaxPowerConsumption
					
	9,		//Size
	4,		//Type
	0,		//InterfaceNumber
	0,		//AlternateSetting
	4,		//TotalEndpoints
	0,		//Class
	0,		//SubClass
	0,		//Protocol
	0,		//InterfaceStrIndex
									
	7,		//Size
	5,		//Type
	0x81,	//EndpointAddress
	2,		//Attributes
	16,0,	//EndpointSize
	20,		//PollingIntervalMS
					
	7,		//Size
	5,		//Type
	0x1,	//EndpointAddress
	2,		//Attributes
	16,0,	//EndpointSize
	20,		//PollingIntervalMS
										
	7,		//Size
	5,		//Type
	0x82,	//EndpointAddress
	2,		//Attributes
	64,0,	//EndpointSize
	0x1,	//PollingIntervalMS
					
	7,		//Size
	5,		//Type
	0x2,	//EndpointAddress
	2,		//Attributes
	64,0,	//EndpointSize
	0x1,	//PollingIntervalMS
	};











