/*
██████╗  █████╗ ████████╗████████╗███████╗██████╗ ██╗   ██╗███████╗ █████╗ ██╗   ██╗███████╗██████╗ 
██╔══██╗██╔══██╗╚══██╔══╝╚══██╔══╝██╔════╝██╔══██╗╚██╗ ██╔╝██╔════╝██╔══██╗██║   ██║██╔════╝██╔══██╗
██████╔╝███████║   ██║      ██║   █████╗  ██████╔╝ ╚████╔╝ ███████╗███████║██║   ██║█████╗  ██████╔╝
██╔══██╗██╔══██║   ██║      ██║   ██╔══╝  ██╔══██╗  ╚██╔╝  ╚════██║██╔══██║╚██╗ ██╔╝██╔══╝  ██╔══██╗
██████╔╝██║  ██║   ██║      ██║   ███████╗██║  ██║   ██║   ███████║██║  ██║ ╚████╔╝ ███████╗██║  ██║
╚═════╝ ╚═╝  ╚═╝   ╚═╝      ╚═╝   ╚══════╝╚═╝  ╚═╝   ╚═╝   ╚══════╝╚═╝  ╚═╝  ╚═══╝  ╚══════╝╚═╝  ╚═╝
-Version 1.1
*/

//Globals
#define 	BATTERY_SAVER
volatile	uint16_t	ChargeSetModeCounter 	= 0;
volatile	uint8_t 	timeToSetCapacityDisplay = 0;
volatile	uint8_t		SetCapacityDisplay 		= 0;
#define 	FADE_OUT_CAP_AFTER_SETTING_CHARGEMAX 50

//locals for 0x54B
	uint8_t FanSpeed = 0;
	uint8_t VentModeStatus = 0;

	case 0x54B: 
	#ifdef BATTERY_SAVER
	VentModeStatus = (frame.data[3]); 
	FanSpeed = ((frame.data[4] & 0xF8) >> 3); //Fan speed is 0-7

	if (FanSpeed == 7 && VentModeStatus == 0x09) //Only enter chargemode setting when recirc is on and fan speed is max (7)
	{
		ChargeSetModeCounter++;	//increment for each 100ms can message
		
		if (ChargeSetModeCounter > 140) // 14s held
		{
			SOC_stopcharging = 100; 
			SetCapacityDisplay = 15; //15 = 12 capacity bars = 100% charge limiter
			timeToSetCapacityDisplay = FADE_OUT_CAP_AFTER_SETTING_CHARGEMAX; 
		}
		else if (ChargeSetModeCounter > 120) // 12s held
		{
			SOC_stopcharging = 93; 
			SetCapacityDisplay = 13; //13 = 11 capacity bars = 92% charge limiter
			timeToSetCapacityDisplay = FADE_OUT_CAP_AFTER_SETTING_CHARGEMAX; 
		}
		else if (ChargeSetModeCounter > 100) // 10s held
		{
			SOC_stopcharging = 84; 
			SetCapacityDisplay = 12; //12 = 10 capacity bars = 83% charge limiter
			timeToSetCapacityDisplay = FADE_OUT_CAP_AFTER_SETTING_CHARGEMAX; 
		}
		else if (ChargeSetModeCounter > 80) // 8s held
		{
			SOC_stopcharging = 76; 
			SetCapacityDisplay = 11; //11 = 9 capacity bars = 75% charge limiter
			timeToSetCapacityDisplay = FADE_OUT_CAP_AFTER_SETTING_CHARGEMAX; 
		}
		else if (ChargeSetModeCounter > 60) // 6s held
		{
			SOC_stopcharging = 67;
			SetCapacityDisplay = 9; //9 = 8 capacity bars = 66% charge limiter
			timeToSetCapacityDisplay = FADE_OUT_CAP_AFTER_SETTING_CHARGEMAX;
		}
		else if (ChargeSetModeCounter > 40) // 4s held
		{
			SOC_stopcharging = 59;
			SetCapacityDisplay = 8; //8 = 7 capacity bars = 58% charge limiter
			timeToSetCapacityDisplay = FADE_OUT_CAP_AFTER_SETTING_CHARGEMAX;
		}
		else if (ChargeSetModeCounter > 20) // 2s held
		{
			SOC_stopcharging = 51;
			SetCapacityDisplay = 7; //7 = 6 capacity bars = 50% charge limiter
			timeToSetCapacityDisplay = FADE_OUT_CAP_AFTER_SETTING_CHARGEMAX; 
		}
		else
		{
			//Condition has been held for less than 2s, do nothing
		}
	}
	else //Conditions no longer fulfilled
	{
		ChargeSetModeCounter = 0; //Reset the counter as soon as conditions no longer are valid
		if (timeToSetCapacityDisplay > 0) //Slowly decrease the capacity readout
		{
			timeToSetCapacityDisplay--;
		}
	}
	#endif
	break;

//locals for 0x5BC
	uint8_t mux_5bc_CapacityCharge = 0;

	case 0x5BC: 
	#ifdef BATTERY_SAVER
	//Section for visualizing the max charge with the help of capacity bars on the dash
	mux_5bc_CapacityCharge = (frame.data[4] & 0x0F); //Save the mux containing info if we have capacity or chargebars in the message [8 / 9] [40kWh 14/15]
	if (timeToSetCapacityDisplay > 0)
	{
		if (mux_5bc_CapacityCharge == 15)	//Only when the muxed field is equal to 15, ONLY VALID FOR 40kWh packs!!!, 9 for 2013-2016 24kWh BMS 
		{
			frame.data[2] = (uint8_t) ((frame.data[2] & 0x0F) | SetCapacityDisplay << 4);
		}
	}
	#endif
	break;

	case 0x1DB:
	#ifdef BATTERY_SAVER
	if (timeToSetCapacityDisplay > 0) //Visualize the max-charge also with SOC display whilst the user is setting it
	{
		frame.data[4] = (SOC_stopcharging-1);
	}
	if(end_of_charge) //Actual function for stopping the charge
	{
		frame.data[1] = (frame.data[1] & 0xE0) | 2; //request charging stop
	}
	#endif

	break;