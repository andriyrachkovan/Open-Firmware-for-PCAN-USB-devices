		
		_delay_ms(10);
		
		Endpoint_ConfigureEndpoint_(0x81, 2, 16, 1);			
		
		if()LEDS_ON;


			
			
			
			
			
						if (Endpoint_IsINReady_() && Endpoint_IsReadWriteAllowed_()) 
			{
				uint8_t i;
				for (i = 0; i < 16; ++i) 
				{
					UEDATX = 0x45;//USBtoUSART_Buffer_Data_[i];
				}
            while (!(UEINTX & (1 << TXINI)));
            Endpoint_ClearIN_();
            LEDS_ON;
			}



set adres    0   5
get status   80  0
get config   80   6    2



			//Endpoint_ConfigureEndpoint_(0x81, 2, 16, 1);
			Endpoint_SelectEndpoint_(0x81);
			Endpoint_DisableEndpoint_();
			UECFG1X &= ~(1 << ALLOC);
			Endpoint_EnableEndpoint_();
			UECFG0X = 0b01000001;//UECFG0XTemp;
			UECFG1X = 0b00010010;//UECFG1XTemp;
			UEIENX  = 0b00000000;//UEIENXTemp;
			if (Endpoint_IsConfigured_())LEDS_ON;




//			_delay_us(1);
