
void BLE_Request_Station_ID(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 57;

	uint16_t BLE_CRC,Temp_CRC;

	BLE_CRC = BLE_Combine_Bytes(BLE_rec_buff[2],BLE_rec_buff[3]);

	Temp_CRC = mmodbus_crc16(BLE_rec_buff, 2);
	CRC_Disp = Temp_CRC;

//	HAL_UART_Transmit(&huart3, &CRC_Disp, 2, 100);

	if(Temp_CRC != BLE_CRC)
	{
		BLE_Send_Error_Response_General(BLE_Error_Code_CRC_Not_matched);
	}
	else
	{
		BLE_Response_Request_Station_ID();
	}
}

void BLE_Response_Request_Station_ID(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 58;

	uint8_t temp_ble_data[8],byte_index;

	temp_ble_data[0] = 0xAA;

	temp_ble_data[3] = BLE_Index;
	BLE_Index++;

	temp_ble_data[4] = BLE_Command_Request_Station_ID | 0X20;
	temp_ble_data[5] = Config_Parameters[Alt_Station_ID] & 0x00FF;

	byte_index = 6;

	BLE_Common_End_Function(temp_ble_data,byte_index);
}

void BLE_Send_Communication_Check_Frame(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 59;

	uint8_t temp_ble_data[10],byte_index;
	int8_t OTA_Permit_Byte;

	temp_ble_data[0] = 0xAA;

	temp_ble_data[3] = BLE_Index;
	BLE_Index++;

	temp_ble_data[4] = BLE_Command_Communication_Check_Frame;
	temp_ble_data[5] = Config_Parameters[Alt_Station_ID] & 0x00FF;

	if((Flag_Auto_Any != 0) || (mpm_int != 0))
	{
		OTA_Permit_Byte = Version_Higher_Byte;		//Positive Byte indicating OTA Not Permitted
	}
	else											//If Auto OFF & Speed is Zero, then only OTA will be Permitted indicating Negative Version
	{
		OTA_Permit_Byte = Version_Higher_Byte;
		OTA_Permit_Byte = OTA_Permit_Byte * (-1);
	}

	temp_ble_data[6] = OTA_Permit_Byte;
	temp_ble_data[7] = Version_Lower_Byte;

	byte_index = 8;

	BLE_Common_End_Function(temp_ble_data,byte_index);
}

void BLE_Send_Error_Response_General(uint8_t err_code)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 60;

	uint8_t temp_error_str[8],byte_index;

	temp_error_str[0] = 	BLE_rec_buff[0];

	temp_error_str[3] = 	BLE_Index;
	BLE_Index++;
	temp_error_str[4] = 	BLE_rec_buff[1] | 0x80;
	temp_error_str[5] = 	err_code;

	byte_index = 6;

	BLE_Common_End_Function(temp_error_str,byte_index);
}

//=====READ CONFIG REGISTERS=======

void BLE_Read_Config_Registers(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 61;

	uint16_t BLE_Start_Address,BLE_Data_length,BLE_CRC,Temp_CRC;

	BLE_Start_Address 	= BLE_Combine_Bytes(BLE_rec_buff[2],BLE_rec_buff[3]);
	BLE_Data_length 	= BLE_Combine_Bytes(BLE_rec_buff[4],BLE_rec_buff[5]);
	BLE_CRC 			= BLE_Combine_Bytes(BLE_rec_buff[6],BLE_rec_buff[7]);

	Temp_CRC = mmodbus_crc16(BLE_rec_buff, 6);
	CRC_Disp = Temp_CRC;

//	HAL_UART_Transmit(&huart3, &CRC_Disp, 2, 100);

	if(Temp_CRC != BLE_CRC)
	{
		BLE_Send_Error_Response_General(BLE_Error_Code_CRC_Not_matched);
	}
	else if((BLE_Start_Address + BLE_Data_length) > (Max_HoldReg_Address + 1))		//ex. for 11 data... max add : 10, total 11 data.. add 0 to 11.. poss read: 0-11,1-10,10-1
	{
		BLE_Send_Error_Response_General(BLE_Error_Code_Address_Out_Of_Range);
	}
	else
	{
		BLE_Response_Read_Config_Registers(BLE_Start_Address,BLE_Data_length);
	}
}

void BLE_Response_Read_Config_Registers(uint16_t BLE_Start_Address, uint16_t BLE_Data_length)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 62;

	uint8_t temp_ble_data[155];						//30 int + 19 float + 7 init + 2 CRC = 60+76+7+2 = 145
	uint8_t i = 0,byte_index = 7,Conf_index;

	temp_ble_data[0] = BLE_rec_buff[0];
	temp_ble_data[1] = 0x00;		//Length
	temp_ble_data[2] = 0x00;		//Length
	temp_ble_data[3] = BLE_Index;
	BLE_Index++;

	temp_ble_data[4] =  BLE_Command_Read_Config_Reg | 0X20;

	BLE_Seperate_Bytes(BLE_Start_Address,&temp_ble_data[5], &temp_ble_data[6]);	//Starting Address

	for(i = 0; i < BLE_Data_length; i++)
	{
			//Get Real Config Parameter Index based on BLE address
			Conf_index = BLE_Config_Para_Index[BLE_Start_Address + i];

			if(BLE_HoldReg_Type[BLE_Start_Address + i] == Type_16_Bit)
			{
				BLE_Seperate_Bytes(Config_Parameters[Conf_index],&temp_ble_data[byte_index], &temp_ble_data[byte_index + 1]);	//Data starts from 5th index	LSB FIRST
				byte_index = byte_index + 2;
			}
			else	//32 bit
			{
				BLE_Seperate_Bytes_Float(Config_Float[Conf_index],&temp_ble_data[byte_index]);											//Data starts from 5th index	LSB FIRST
				byte_index = byte_index + 4;
			}
	}
	BLE_Common_End_Function(temp_ble_data,byte_index);
}

//=================WRITE COIL=========================

void BLE_Send_Error_Response_Write_Coil(uint16_t Coil_Address, uint16_t Coil_Data, uint8_t err_code)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 63;

	uint8_t temp_error_str[12],byte_index;

	temp_error_str[0] = 	BLE_rec_buff[0];

	temp_error_str[3] = 	BLE_Index;
	BLE_Index++;
	temp_error_str[4] = 	BLE_rec_buff[1] | 0x80;
	temp_error_str[5] = 	err_code;

	BLE_Seperate_Bytes(Coil_Address,&temp_error_str[6],&temp_error_str[7]);
	BLE_Seperate_Bytes(Coil_Data,&temp_error_str[8],&temp_error_str[9]);

	byte_index = 10;
	BLE_Common_End_Function(temp_error_str,byte_index);
}

void BLE_Write_Coil_Config_Info_Response(_Bool reg_type, uint16_t reg_add)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 64;

	if(reg_type == Type_16_Bit)
	{
		BLE_Info_Config_Register_Changed(reg_add,Config_Parameters[reg_add]);
	}
	else
	{
		BLE_Info_Config_Register_Float_Changed(reg_add,Config_Float[reg_add]);
	}
}

void BLE_Write_Single_Coil(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 65;

	uint16_t BLE_Coil_Address,BLE_Coil_Data,BLE_CRC,Temp_CRC;

	BLE_Coil_Address 	= BLE_Combine_Bytes(BLE_rec_buff[2],BLE_rec_buff[3]);
	BLE_Coil_Data 		= BLE_Combine_Bytes(BLE_rec_buff[4],BLE_rec_buff[5]);
	BLE_CRC 			= BLE_Combine_Bytes(BLE_rec_buff[6],BLE_rec_buff[7]);

	Temp_CRC = mmodbus_crc16(BLE_rec_buff, 6);
	CRC_Disp = Temp_CRC;

	uint8_t temp_conf_reg_address;
	_Bool Flag_coil_changes_conf_value = 0;
	if(BLE_Coil_Address <= Max_KeyID_Address)
	{
		temp_conf_reg_address = BLE_KeyID_to_RegAdd[BLE_Coil_Address];
		if(temp_conf_reg_address != 0)
		{
			Flag_coil_changes_conf_value = 1;
			Flag_coil_reply_data_type = BLE_HoldReg_Type[temp_conf_reg_address];	//Get Type from BLE_HoldReg_Type
			coil_reply_address = BLE_Config_Para_Index[temp_conf_reg_address];		//Get Address of Int or Float from BLE_Config_Para_Index
		}
	}
//	HAL_UART_Transmit(&huart3, &CRC_Disp, 2, 100);

	if(Temp_CRC != BLE_CRC)
	{
		BLE_Send_Error_Response_Write_Coil(BLE_Coil_Address, BLE_Coil_Data, BLE_Error_Code_CRC_Not_matched);
	}
	else if(BLE_Coil_Address > Max_KeyID_Address)
	{
		BLE_Send_Error_Response_Write_Coil(BLE_Coil_Address, BLE_Coil_Data, BLE_Error_Code_Address_Out_Of_Range);
	}
	else if((BLE_Coil_Data != 0xFF00) && (BLE_Coil_Data != 0x0000))
	{
		BLE_Send_Error_Response_Write_Coil(BLE_Coil_Address, BLE_Coil_Data, BLE_Error_Code_Data_Not_Valid);
	}
	else
	{
		Temp_BLE_Reg_Address = BLE_KeyID_Replace[BLE_Coil_Address];

		if(BLE_Coil_Data == 0xFF00)				//Pressed or Click Event
		{
			Flag_BLE_Value_Received = 1;
			BLE_Coil_Write_Success = 0;

			if(Flag_GenKeyPressed == 0)
			{
				Check_For_LCD_Touch_Command_Touch_Buttons();
			}

			if(Flag_BLE_KeyPress_Event != 0)
			{
				Time_BLE_RepeatKeyPress_ms = 0;
			}

			Flag_BLE_Value_Received = 0;
		}
		else									//Released Event
		{
			rec_function_byte = 0;
			Flag_TouchKeyPressed = 0;
			Flag_BLE_KeyPress_Event = 0;
			Time_BLE_RepeatKeyPress_ms = 0;		//Added 29.11.2022

			BLE_Coil_Write_Success = 1;
		}

		if(BLE_Coil_Write_Success != 0)
		{
			BLE_Response_Write_Single_Coil(BLE_Coil_Data);
		}
		else
		{
			BLE_Send_Error_Response_Write_Coil(BLE_Coil_Address, BLE_Coil_Data, BLE_Error_Code_Not_Processed);
		}
	}

	if(Flag_coil_changes_conf_value != 0)
	{
		BLE_Write_Coil_Config_Info_Response(Flag_coil_reply_data_type,coil_reply_address);
	}
}

void BLE_Response_Write_Single_Coil(uint16_t BLE_Data)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 66;

	uint8_t temp_str[11],byte_index;

	temp_str[0] = 	BLE_rec_buff[0];

	temp_str[3] = 	BLE_Index;		//Index
	BLE_Index++;

	temp_str[4] = 	BLE_rec_buff[1] | 0x20;		//Command
	temp_str[5] = 	BLE_rec_buff[2];			//Address
	temp_str[6] = 	BLE_rec_buff[3];			//Address

	BLE_Seperate_Bytes(BLE_Data,&temp_str[7], &temp_str[8]);

	byte_index = 9;
	BLE_Common_End_Function(temp_str,byte_index);
}

//=================WRITE HOLDING REGISTERS=========================

void BLE_Send_Error_Response_Write_Int(_Bool add_type, uint16_t reg_add, uint8_t err_code)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 67;

	uint8_t temp_error_str[12],byte_index;

	temp_error_str[0] = 	BLE_rec_buff[0];

	temp_error_str[3] = 	BLE_Index;
	BLE_Index++;
	temp_error_str[4] = 	BLE_rec_buff[1] | 0x80;
	temp_error_str[5] = 	err_code;

	uint16_t Temp_Conf_Index;
	if(add_type == add_type_ble)
	{
		BLE_Seperate_Bytes(reg_add,&temp_error_str[6],&temp_error_str[7]);	//use direct ble address to send
		Temp_Conf_Index = BLE_Config_Para_Index[reg_add];					//Get Config_Parameter Index to get value
	}
	else	//add_type = add_type_config
	{
		BLE_Seperate_Bytes(BLE_HoldReg_Index[reg_add],&temp_error_str[6],&temp_error_str[7]);
																			//Get ble address from config address
		Temp_Conf_Index = reg_add;											//use Config_Parameter Index directly to get data
	}

	if((err_code == BLE_Error_Code_Address_Out_Of_Range) || (err_code == BLE_Error_Code_Data_Type_Mismatched))
	{
		BLE_Seperate_Bytes(0,&temp_error_str[8], &temp_error_str[9]);
	}
	else
	{
		BLE_Seperate_Bytes(Config_Parameters[Temp_Conf_Index],&temp_error_str[8], &temp_error_str[9]);
	}

	byte_index = 10;
	BLE_Common_End_Function(temp_error_str,byte_index);
}

void BLE_Write_Single_Config_Register(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 68;

	uint16_t BLE_Reg_Address,BLE_Reg_Data,BLE_CRC,Temp_CRC;

	BLE_Reg_Address = BLE_Combine_Bytes(BLE_rec_buff[2],BLE_rec_buff[3]);
	BLE_Reg_Data 	= BLE_Combine_Bytes(BLE_rec_buff[4],BLE_rec_buff[5]);
	BLE_CRC 		= BLE_Combine_Bytes(BLE_rec_buff[6],BLE_rec_buff[7]);

	Temp_CRC = mmodbus_crc16(BLE_rec_buff, 6);
	CRC_Disp = Temp_CRC;

//	HAL_UART_Transmit(&huart3, &CRC_Disp, 2, 100);

	if(Temp_CRC != BLE_CRC)
	{
		BLE_Send_Error_Response_Write_Int(add_type_ble, BLE_Reg_Address, BLE_Error_Code_CRC_Not_matched);
	}
	else if(BLE_Reg_Address > Max_HoldReg_Address)
	{
		BLE_Send_Error_Response_Write_Int(add_type_ble, BLE_Reg_Address, BLE_Error_Code_Address_Out_Of_Range);
	}
	else if(BLE_HoldReg_Type[BLE_Reg_Address] != Type_16_Bit)
	{
		BLE_Send_Error_Response_Write_Int(add_type_ble, BLE_Reg_Address, BLE_Error_Code_Data_Type_Mismatched);
	}
	else
	{
		Flag_BLE_Value_Received = 1;

		//Get Config Index(lcd component id) based on BLE Reg Address
		Temp_BLE_Reg_Address	= BLE_Config_Para_Index[BLE_Reg_Address];
		Temp_BLE_Reg_Data		= BLE_Reg_Data;

		Check_For_LCD_Touch_Command_Int();

		Flag_BLE_Value_Received = 0;
	}
}

void BLE_Response_Write_Single_Config_Reg(uint16_t BLE_Data)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 69;

	uint8_t temp_str[11],byte_index;

	temp_str[0] = 	BLE_rec_buff[0];

	temp_str[3] = 	BLE_Index;					//Index
	BLE_Index++;

	temp_str[4] = 	BLE_rec_buff[1] | 0x20;		//Command

	temp_str[5] = 	BLE_rec_buff[2];			//Address
	temp_str[6] = 	BLE_rec_buff[3];			//Address

	BLE_Seperate_Bytes(BLE_Data,&temp_str[7], &temp_str[8]);

	byte_index = 9;
	BLE_Common_End_Function(temp_str,byte_index);
}

void BLE_Info_Config_Register_Changed(uint16_t BLE_Reg_Address, int16_t BLE_Data)			//lcd_component_id
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 70;

	uint8_t temp_str[11],byte_index;

	temp_str[0] = 	Config_Parameters[Alt_Station_ID];

	temp_str[3] = 	BLE_Index;
	BLE_Index++;

	temp_str[4] = 	BLE_Command_Info_Write_From_Device;

	//Get HoldReg Index for BLE Application
	BLE_Seperate_Bytes(BLE_HoldReg_Index[BLE_Reg_Address],&temp_str[5],&temp_str[6]);
	BLE_Seperate_Bytes(BLE_Data,&temp_str[7], &temp_str[8]);

	byte_index = 9;
	BLE_Common_End_Function(temp_str,byte_index);
}

//====WRITE SINGLE FLOAT HOLDING/CONFIG REGISTERS=======================================

void BLE_Send_Error_Response_Write_Float(_Bool add_type, uint16_t reg_add, uint8_t err_code)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 71;

	uint8_t temp_error_str[14],byte_index;

	temp_error_str[0] = 	BLE_rec_buff[0];

	temp_error_str[3] = 	BLE_Index;
	BLE_Index++;
	temp_error_str[4] = 	BLE_rec_buff[1] | 0x80;
	temp_error_str[5] = 	err_code;

	uint16_t Temp_Conf_Index;
	if(add_type == add_type_ble)
	{
		BLE_Seperate_Bytes(reg_add,&temp_error_str[6],&temp_error_str[7]);	//use direct ble address to send
		Temp_Conf_Index = BLE_Config_Para_Index[reg_add];					//Get Config_Parameter Index to get value
	}
	else	//add_type = add_type_config
	{
		BLE_Seperate_Bytes(BLE_HoldReg_Float_Index[reg_add],&temp_error_str[6],&temp_error_str[7]);
																			//Get ble address from config address
		Temp_Conf_Index = reg_add;											//use Config_Parameter Index directly to get data
	}

	if((err_code == BLE_Error_Code_Address_Out_Of_Range) || (err_code == BLE_Error_Code_Data_Type_Mismatched))
	{
		BLE_Seperate_Bytes_Float(0,&temp_error_str[8]);
	}
	else
	{
		BLE_Seperate_Bytes_Float(Config_Float[Temp_Conf_Index],&temp_error_str[8]);
	}

	byte_index = 12;
	BLE_Common_End_Function(temp_error_str,byte_index);
}

void BLE_Write_Single_Config_Register_Float(void)		//0x07
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 72;

	uint16_t BLE_Reg_Address,BLE_CRC,Temp_CRC;
	float BLE_Reg_Data;

	BLE_Reg_Address = BLE_Combine_Bytes(BLE_rec_buff[2],BLE_rec_buff[3]);
	BLE_Reg_Data 	= BLE_Combine_Bytes_Float(&BLE_rec_buff[4]);					//4,5,6,7
	BLE_CRC 		= BLE_Combine_Bytes(BLE_rec_buff[8],BLE_rec_buff[9]);

	Temp_CRC = mmodbus_crc16(BLE_rec_buff, 8);
	CRC_Disp = Temp_CRC;

//	HAL_UART_Transmit(&huart3, &CRC_Disp, 2, 100);

	if(Temp_CRC != BLE_CRC)
	{
		BLE_Send_Error_Response_Write_Float(add_type_ble, BLE_Reg_Address, BLE_Error_Code_CRC_Not_matched);
	}
	else if(BLE_Reg_Address > Max_HoldReg_Address)
	{
		BLE_Send_Error_Response_Write_Float(add_type_ble, BLE_Reg_Address, BLE_Error_Code_Address_Out_Of_Range);
	}
	else if(BLE_HoldReg_Type[BLE_Reg_Address] != Type_32_Bit)
	{
		BLE_Send_Error_Response_Write_Float(add_type_ble, BLE_Reg_Address, BLE_Error_Code_Data_Type_Mismatched);
	}
	else
	{
		Flag_BLE_Value_Received = 1;

		Temp_BLE_Reg_Address	= BLE_Config_Para_Index[BLE_Reg_Address];
		Temp_BLE_Reg_Data_Float	= BLE_Reg_Data;

		Check_For_LCD_Touch_Command_Float();

		Flag_BLE_Value_Received = 0;
	}
}

void BLE_Response_Write_Single_Config_Register_Float(float BLE_Data)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 73;

	uint8_t temp_str[13],byte_index;

	temp_str[0] = 	BLE_rec_buff[0];

	temp_str[3] = 	BLE_Index;		//Index
	BLE_Index++;

	temp_str[4] = 	BLE_rec_buff[1] | 0x20;	//Command

	temp_str[5] = 	BLE_rec_buff[2];				//Address
	temp_str[6] = 	BLE_rec_buff[3];				//Address

	BLE_Seperate_Bytes_Float(BLE_Data,&temp_str[7]);

	byte_index = 11;
	BLE_Common_End_Function(temp_str,byte_index);
}

void BLE_Info_Config_Register_Float_Changed(uint16_t BLE_Reg_Address,float BLE_Data)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 74;

	uint8_t temp_str[13],byte_index;

	temp_str[0] = 	Config_Parameters[Alt_Station_ID];

	temp_str[3] = 	BLE_Index;
	BLE_Index++;

	temp_str[4] = 	BLE_Command_Info_Write_From_Device;

	BLE_Seperate_Bytes(BLE_HoldReg_Float_Index[BLE_Reg_Address],&temp_str[5],&temp_str[6]);
	BLE_Seperate_Bytes_Float(BLE_Data,&temp_str[7]);

	byte_index = 11;
	BLE_Common_End_Function(temp_str,byte_index);
}

//==================READ INPUT(DISPLAY) REGISTERS===================

void BLE_Send_Display_Data(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 75;

	uint8_t temp_ble_data[100];						//21 int, 8 float + 7 init + 2 crc = 42 + 32 + 7 + 2 = 83, Length can be reduced
	uint8_t i = 0,byte_index = 7,Conf_index;

	temp_ble_data[0] = Config_Parameters[Alt_Station_ID];
	temp_ble_data[1] = 0x00;						//Length
	temp_ble_data[2] = 0x00;						//Length

	temp_ble_data[3] = BLE_Index;
	BLE_Index++;

	temp_ble_data[4] = ( Read_Input_Group_Mode * 0x10 ) | 0x0F;

	BLE_Seperate_Bytes(BLE_Disp_Start_Address,&temp_ble_data[5], &temp_ble_data[6]);	//Starting Address

	for(i = 0; i < BLE_Disp_Data_length; i++)
	{
		Conf_index = BLE_Display_Para_Index[BLE_Disp_Start_Address + i];

		if(BLE_DispReg_Type[BLE_Disp_Start_Address + i] == Type_16_Bit)
		{
			BLE_Seperate_Bytes(BLE_Display_Registers[Conf_index],&temp_ble_data[byte_index], &temp_ble_data[byte_index + 1]);	//Data starts from 5th index	LSB FIRST
			byte_index = byte_index + 2;
		}
		else	//32 bit
		{
			BLE_Seperate_Bytes_Float(BLE_Display_Float[Conf_index],&temp_ble_data[byte_index]);											//Data starts from 5th index	LSB FIRST
			byte_index = byte_index + 4;
		}
	}

	BLE_Common_End_Function(temp_ble_data,byte_index);
}

void BLE_Write_Job_Name(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 76;

	uint8_t BLE_Data_length;
	uint16_t BLE_CRC,Temp_CRC;

	BLE_Data_length = BLE_rec_buff[2];
	BLE_CRC 		= BLE_Combine_Bytes(Temp_Job_Name[BLE_Data_length+1],Temp_Job_Name[BLE_Data_length+2]);	//Last 2 Bytes

	Temp_CRC = mmodbus_crc16((uint8_t *)Temp_Job_Name, BLE_Data_length+1);
	CRC_Disp = Temp_CRC;

	if(Temp_CRC != BLE_CRC)
	{
		BLE_Send_Error_Response_General(BLE_Error_Code_CRC_Not_matched);
	}
	else
	{
		BLE_Response_Write_Job_Name(BLE_Data_length-2);	//Job Name Length
	}
}

void BLE_Response_Write_Job_Name(uint8_t length_name)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 77;

	uint8_t temp_ble_data[30];		//20 job name + 5 init bytes + 2 CRC
	uint8_t i = 0,j,byte_index;

	temp_ble_data[0] = BLE_rec_buff[0];

	temp_ble_data[1] = 0x00;		//Length
	temp_ble_data[2] = 0x00;		//Length

	temp_ble_data[3] = BLE_Index;
	BLE_Index++;

	temp_ble_data[4] = BLE_Command_Write_Job_Name | 0X20;

	byte_index = 5;

	j = 3;	//index in Temp_Job_Name from where name starts

	for(i = 0; i < length_name; i++,j++)
	{
		temp_ble_data[byte_index + i] = Temp_Job_Name[j];
		Job_Name[i] = Temp_Job_Name[j];
	}
	Job_Name[i] = '\0';
	Job_Name[21] = length_name;	//Last byte shows length of string

	byte_index = byte_index + length_name;
	BLE_Common_End_Function(temp_ble_data,byte_index);

	Send_String_UART_232((uint8_t *)Job_Name);
	Flash_Write_Job_Name();
	Job_Name_Send_String();

	if(Flag_Permit_Actions == 0)
	{
		Flag_Permit_Actions = 1;
		Transmit_Config_Parameters_Single(LCD_Flag_Permit_Action_Location,Flag_Permit_Actions);
	}
	Flag_Job_Name_Received = 1;
}

//===================================================================================

_Bool BLE_Request_OTA(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 78;

	uint8_t BLE_Data_length;
	uint16_t BLE_CRC,Temp_CRC;

	BLE_Data_length = BLE_rec_buff[2];
	BLE_CRC 		= BLE_Combine_Bytes(Temp_File_Name[BLE_Data_length+1],Temp_File_Name[BLE_Data_length+2]);	//Last 2 Bytes

	Temp_CRC = mmodbus_crc16((uint8_t *)Temp_File_Name, BLE_Data_length+1);
	CRC_Disp = Temp_CRC;

	if(Temp_CRC != BLE_CRC)
	{
		BLE_Send_Error_Response_General(BLE_Error_Code_CRC_Not_matched);
		return(0);
	}
	else
	{
		BLE_Response_Request_OTA(BLE_Data_length-2);	//Job Name Length
		return(1);
	}
}

void BLE_Response_Request_OTA(uint8_t length_name)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 79;

	uint8_t i = 0,j,byte_index;
	uint16_t crc_calc;

	Resp_Bytes[0] = BLE_rec_buff[0];

	Resp_Bytes[1] = 0x00;		//Length
	Resp_Bytes[2] = 0x00;		//Length

	Resp_Bytes[3] = BLE_Index;
	BLE_Index++;

	Resp_Bytes[4] = BLE_Command_Request_OTA | 0X20;

	byte_index = 5;

	j = 3;	//index in Temp_File_Name from where name starts

	for(i = 0; i < length_name; i++,j++)
	{
		Resp_Bytes[byte_index + i] = Temp_File_Name[j];
		OTA_File_Name[i] = Temp_File_Name[j];
	}
	OTA_File_Name[i] = '\0';

	byte_index = byte_index + length_name;

	Resp_Length = byte_index + 2;
	BLE_Seperate_Bytes(Resp_Length - 3,&Resp_Bytes[1],&Resp_Bytes[2]);

	crc_calc = mmodbus_crc16(Resp_Bytes,byte_index);
	BLE_Seperate_Bytes(crc_calc,&Resp_Bytes[byte_index], &Resp_Bytes[byte_index + 1]);
}

//======FUNCTIONS FOR 16 BIT REGISTERS=============================================

void BLE_Seperate_Bytes(uint16_t Temp_Value,uint8_t *Byte_Index_Low, uint8_t *Byte_Index_High)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 80;

	*Byte_Index_Low = 	(uint8_t)(Temp_Value & 0x00FF);				//Lower index has LSB
	*Byte_Index_High = 	(uint8_t)((Temp_Value & 0xFF00) >> 8);		//Higher index has MSB
}

uint16_t BLE_Combine_Bytes(uint8_t Data_LSB, uint8_t Data_MSB)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 81;

	uint16_t Temp_Low,Temp_Value;
	//Lower Index has LSB and Higher Index has MSB

	Temp_Low 	= Data_LSB;
	Temp_Value 	= Data_MSB;
	Temp_Value 	= Temp_Value << 8;				//Higher Byte at MSB
	Temp_Value 	= Temp_Value | Temp_Low;			//Lower Byte at LSB
	return(Temp_Value);
}

//======FUNCTIONS FOR 32 BIT FLOAT REGISTERS=======================

void BLE_Seperate_Bytes_Float(float Temp_Value,uint8_t *Float_Bytes)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 82;

	Float_Disp.df = Temp_Value;
	Float_Bytes[0] = Float_Disp.dc[0];
	Float_Bytes[1] = Float_Disp.dc[1];
	Float_Bytes[2] = Float_Disp.dc[2];
	Float_Bytes[3] = Float_Disp.dc[3];
}

float BLE_Combine_Bytes_Float(uint8_t *Float_Bytes)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 83;

	Float_Disp.dc[0] = Float_Bytes[0];
	Float_Disp.dc[1] = Float_Bytes[1];
	Float_Disp.dc[2] = Float_Bytes[2];
	Float_Disp.dc[3] = Float_Bytes[3];
	return(Float_Disp.df);
}

void BLE_Seperate_Bytes_Int32(uint32_t Temp_Value,uint8_t *Int_Bytes)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 84;

	Float_Disp.di = Temp_Value;
	Int_Bytes[0] = Float_Disp.dc[0];
	Int_Bytes[1] = Float_Disp.dc[1];
	Int_Bytes[2] = Float_Disp.dc[2];
	Int_Bytes[3] = Float_Disp.dc[3];
}

uint32_t BLE_Combine_Bytes_Int32(uint8_t *Int_Bytes)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 85;

	Float_Disp.dc[0] = Int_Bytes[0];
	Float_Disp.dc[1] = Int_Bytes[1];
	Float_Disp.dc[2] = Int_Bytes[2];
	Float_Disp.dc[3] = Int_Bytes[3];
	return(Float_Disp.di);
}


void Arrange_Display_System_Mode_LCD_Data_Array(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 86;

	//16 BIT VARIABLES

    Calculate_Position_Move_Icon_As_Per_Error(Disp_Actual_Error[Horizontal],Disp_Actual_Error[Vertical]);

    LCD_Display_Registers[LCD_DISP_MOVE_ICON_CMD_01] = 0X0007;
    LCD_Display_Registers[LCD_DISP_MOVE_ICON_CMD_02] = 0X0001;
    LCD_Display_Registers[LCD_DISP_MOVE_ICON_CMD_03] = position_move_icon_horz_x;
    LCD_Display_Registers[LCD_DISP_MOVE_ICON_CMD_04] = position_move_icon_vert_y;
    LCD_Display_Registers[LCD_DISP_MOVE_ICON_CMD_05] = icon_id_move_icon;

//	LCD_Display_Registers[LCD_DISP_AVG_HORZ] = Disp_auto_average[Horizontal];
//	LCD_Display_Registers[LCD_DISP_AVG_VERT] = Disp_auto_average[Vertical];

	if(Screen_ID == Page_Diagnose)
	{
		LCD_Display_Registers[LCD_DISP_AVG_HORZ] = Set_Threshold_Count;
		LCD_Display_Registers[LCD_DISP_AVG_VERT] = Disp_H_Count;
	}
	else
	{
		LCD_Display_Registers[LCD_DISP_AVG_HORZ] = Disp_auto_average[Horizontal];
		LCD_Display_Registers[LCD_DISP_AVG_VERT] = Disp_auto_average[Vertical];
	}

	LCD_Display_Registers[LCD_DISP_MARK_HGT_mm] 	= Mark_M2_Height_Disp_Count * Convert_PPR_To_mm;
	LCD_Display_Registers[LCD_DISP_MARK_HGT_PROG] 	= Mark_M2_Height_Disp_Count * 100 / Max_Mark_Height_Count;

	LCD_Display_Registers[LCD_DISP_SPEED] = mpm_int;

	update_lcd_buttons_status();
	LCD_Display_Registers[LCD_DISP_KST] = Buttons_Status_LEDs;

	//GRAPH SCREEN=======================================================================================

	LCD_Display_Registers[LCD_DISP_VALID_MARKS] 	= Disp_Valid_Mark;
	LCD_Display_Registers[LCD_DISP_BASELINE] 		= Disp_Base_Line;

	LCD_Display_Registers[LCD_DISP_GATE_DRAW_CMD_01] 		= 0X0005;
	LCD_Display_Registers[LCD_DISP_GATE_DRAW_CMD_02_LEN] 	= 0X0003;

	LCD_Display_Registers[LCD_DISP_GATE_DRAW_CMD_03_HSX] 	= Gate_Start[Master_Horz];
	LCD_Display_Registers[LCD_DISP_GATE_DRAW_CMD_04_HSY] 	= LCD_Graph_Gate_Y_Start;
	LCD_Display_Registers[LCD_DISP_GATE_DRAW_CMD_05_HEX] 	= Gate_End[Master_Horz];
	LCD_Display_Registers[LCD_DISP_GATE_DRAW_CMD_06_HEY] 	= LCD_Graph_Gate_Y_End;
	LCD_Display_Registers[LCD_DISP_GATE_DRAW_CMD_07_HCL] 	= LCD_Graph_Colour_Master_Horz;

	LCD_Display_Registers[LCD_DISP_GATE_DRAW_CMD_08_VSX] 	= Gate_Start[Master_Vert];;
	LCD_Display_Registers[LCD_DISP_GATE_DRAW_CMD_09_VSY] 	= LCD_Graph_Gate_Y_Start;
	LCD_Display_Registers[LCD_DISP_GATE_DRAW_CMD_10_VEX] 	= Gate_End[Master_Vert];
	LCD_Display_Registers[LCD_DISP_GATE_DRAW_CMD_11_VEY] 	= LCD_Graph_Gate_Y_End;
	LCD_Display_Registers[LCD_DISP_GATE_DRAW_CMD_12_VCL] 	= LCD_Graph_Colour_Master_Vert;

	LCD_Display_Registers[LCD_DISP_GATE_DRAW_CMD_13_SSX] 	= Gate_Start[Slave];
	LCD_Display_Registers[LCD_DISP_GATE_DRAW_CMD_14_SSY] 	= LCD_Graph_Gate_Y_Start;
	LCD_Display_Registers[LCD_DISP_GATE_DRAW_CMD_15_SEX] 	= Gate_End[Slave];
	LCD_Display_Registers[LCD_DISP_GATE_DRAW_CMD_16_SEY] 	= LCD_Graph_Gate_Y_End;
	LCD_Display_Registers[LCD_DISP_GATE_DRAW_CMD_17_SCL] 	= LCD_Graph_Colour_Slave;

	LCD_Display_Registers[LCD_DISP_GATE_MH_START] 	= Gate_Start[Master_Horz];
	LCD_Display_Registers[LCD_DISP_GATE_MH_END] 	= Gate_End[Master_Horz];
	LCD_Display_Registers[LCD_DISP_GATE_MV_START] 	= Gate_Start[Master_Vert];
	LCD_Display_Registers[LCD_DISP_GATE_MV_END] 	= Gate_End[Master_Vert];
	LCD_Display_Registers[LCD_DISP_GATE_SLV_START] 	= Gate_Start[Slave];
	LCD_Display_Registers[LCD_DISP_GATE_SLV_END] 	= Gate_End[Slave];

	LCD_Display_Registers[LCD_DISP_GATE_MH_START_PPR] 	= Gate_Start_ppr[Master_Horz];
	LCD_Display_Registers[LCD_DISP_GATE_MV_START_PPR] 	= Gate_Start_ppr[Master_Vert];
	LCD_Display_Registers[LCD_DISP_GATE_SLV_START_PPR] 	= Gate_Start_ppr[Slave];
	LCD_Display_Registers[LCD_DISP_ENCODER_COUNTS] 		= Disp_Encoder_Count;

	LCD_Display_Registers[LCD_DISP_ERC] = Cur_Error_Status_Code;

	LCD_Display_Registers[LCD_DISP_TEMP_DIFF] = Temp_Diff_BL_VBL;

	//32 BIT VARIABLES

	Config_Float[AltF_Disp_MarkHeight_Horz] = Mark_M0_Height_Disp_Count * Convert_PPR_To_mm;

	Config_Float[AltF_Disp_Error_Horz] 	= Disp_Actual_Error[Horizontal];
	Config_Float[AltF_Disp_Error_Vert] 	= Disp_Actual_Error[Vertical];

//	Config_Float[AltF_Disp_Production] 	= Actual_Meter_Count * Config_Parameters[Alt_BagLength]/1000.0;
	if(Screen_ID == Page_Diagnose)
	{
		Config_Float[AltF_Disp_Production] 	= EncError_TotalCount;
	}
	else
	{
		Config_Float[AltF_Disp_Production] 	= Actual_Meter_Count * Config_Parameters[Alt_BagLength]/1000.0;
	}
}

void Arrange_Display_System_Mode_BLE_Data_Array(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 87;
	//16 BIT VARIABLES

	//A value which is combination of Skip Flag, RevCorrCount & Average Count
	uint16_t temp_var = RevCorrCount[Horizontal];
	RevCorrCount[Horizontal] = 0;
	temp_var = temp_var << 8;
	if(Flag_SkipErr[Horizontal] != 0)
	{
		temp_var = temp_var | 0x8000;
	}
	Flag_SkipErr[Horizontal] = 0;
	BLE_Display_Registers[BLE_DISP_AVG_HORZ] 		= temp_var | LCD_Display_Registers[LCD_DISP_AVG_HORZ];

	temp_var = RevCorrCount[Vertical];
	RevCorrCount[Vertical] = 0;
	temp_var = temp_var << 8;
	if(Flag_SkipErr[Vertical] != 0)
	{
		temp_var = temp_var | 0x8000;
	}
	Flag_SkipErr[Vertical] = 0;
	BLE_Display_Registers[BLE_DISP_AVG_VERT]		= temp_var | LCD_Display_Registers[LCD_DISP_AVG_VERT];

//	BLE_Display_Registers[BLE_DISP_AVG_HORZ] 		= LCD_Display_Registers[LCD_DISP_AVG_HORZ];
//	BLE_Display_Registers[BLE_DISP_AVG_VERT] 		= LCD_Display_Registers[LCD_DISP_AVG_VERT];

	BLE_Display_Registers[BLE_CORR_MS_HORZ] 		= Correction_ms_Horz;
	BLE_Display_Registers[BLE_CORR_MS_VERT] 		= Correction_ms_Vert;
	Correction_ms_Horz = 0;
	Correction_ms_Vert = 0;

	BLE_Display_Registers[BLE_DISP_MARK_HGT_mm]		= LCD_Display_Registers[LCD_DISP_MARK_HGT_mm];

	BLE_Display_Registers[BLE_DISP_SPEED] 			= LCD_Display_Registers[LCD_DISP_SPEED];
	BLE_Display_Registers[BLE_DISP_KST] 			= BLE_Buttons_Status_LEDs;

	BLE_Display_Registers[BLE_DISP_ERC]				= LCD_Display_Registers[LCD_DISP_ERC];

	BLE_Display_Registers[BLE_DISP_GATE_MH_START] 	= LCD_Display_Registers[LCD_DISP_GATE_MH_START];
	BLE_Display_Registers[BLE_DISP_GATE_MH_END] 	= LCD_Display_Registers[LCD_DISP_GATE_MH_END];

	BLE_Display_Registers[BLE_DISP_GATE_MV_START] 	= LCD_Display_Registers[LCD_DISP_GATE_MV_START];
	BLE_Display_Registers[BLE_DISP_GATE_MV_END] 	= LCD_Display_Registers[LCD_DISP_GATE_MV_END];
	BLE_Display_Registers[BLE_DISP_GATE_SLV_START] 	= LCD_Display_Registers[LCD_DISP_GATE_SLV_START];
	BLE_Display_Registers[BLE_DISP_GATE_SLV_END] 	= LCD_Display_Registers[LCD_DISP_GATE_SLV_END];

	BLE_Display_Registers[BLE_DISP_VALID_MARKS] 	= LCD_Display_Registers[LCD_DISP_VALID_MARKS];
	BLE_Display_Registers[BLE_DISP_BASELINE] 		= LCD_Display_Registers[LCD_DISP_BASELINE];

	BLE_Display_Registers[BLE_DISP_GATE_MH_START_PPR] = LCD_Display_Registers[LCD_DISP_GATE_MH_START_PPR];	//OR M1C
	BLE_Display_Registers[BLE_DISP_GATE_MV_START_PPR] = LCD_Display_Registers[LCD_DISP_GATE_MV_START_PPR];	//OR M2C

	BLE_Display_Registers[BLE_DISP_GATE_SLV_START_PPR] 	= LCD_Display_Registers[LCD_DISP_GATE_SLV_START_PPR];
	BLE_Display_Registers[BLE_DISP_ENCODER_COUNTS] 		= LCD_Display_Registers[LCD_DISP_ENCODER_COUNTS];

	BLE_Display_Registers[BLE_DISP_SER_DEBUG_EN] 	= Flag_Serial_Debug_Enable;

	BLE_Display_Float[BLE_DispF_ErrorRaw_Horz]	= CurError[Horizontal];
	BLE_Display_Float[BLE_DispF_ErrorRaw_Vert]	= CurError[Vertical];
	BLE_Display_Float[BLE_DispF_MarkHeight_Horz]= Config_Float[AltF_Disp_MarkHeight_Horz];
	BLE_Display_Float[BLE_DispF_ErrorAvg_Horz]	= CurError_Avg[Horizontal];
	BLE_Display_Float[BLE_DispF_ErrorAvg_Vert]	= CurError_Avg[Vertical];
	BLE_Display_Float[BLE_DispF_Production]		= Config_Float[AltF_Disp_Production];
}

void BLE_ARC_Send_Graph_Buffer_DMA(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 88;

	uint16_t temp_index;

	BLE_ARC_Graph_Fill_Dummy_Data_at_End();
	BLE_ARC_Graph_Fill_Display_Data();

	buffer_ble[Flag_BLE_Buffer_RD][1] = 0x0006;					//LENH - INDEX		//Index will be updated at every graph data sent

	temp_index = BLE_Index;
	temp_index = temp_index << 8;
	temp_index = temp_index & 0xFF00;

	buffer_ble[Flag_BLE_Buffer_RD][1] = buffer_ble[Flag_BLE_Buffer_RD][1] | temp_index;
	BLE_Index++;

	//----------------------------------------------------------
	//TOTAL LENGTH = 1654, LENGTH VARIABLES = 1654 - 3 = 1651 = 0x0673
	buffer_ble[Flag_BLE_Buffer_RD][0] = 0x7300 | Config_Parameters[Alt_Station_ID];		//LENL 	- Station ID
//	buffer_ble[Flag_BLE_Buffer_RD][1] = 0x0006;											//INDEX - LENL		//Index will be updated at every graph data sent
	buffer_ble[Flag_BLE_Buffer_RD][2] = 0x004F;											//0X00 	- CMD(0X4F)
	buffer_ble[Flag_BLE_Buffer_RD][size_buffer_ble - 1] = 0x5555;

	BLE_Push_Data_DMA_Tx_Buffer((uint8_t *)buffer_ble[Flag_BLE_Buffer_RD], size_buffer_ble*2);

	BLE_Total_Bytes_Sent = BLE_Total_Bytes_Sent + (size_buffer_ble*2);
	BLE_Total_Group_Sent++;
}

void BLE_ARC_Graph_Fill_Dummy_Data_at_End(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 89;

	uint16_t temp_value,end_index;

	end_index = buffer_ble_Graph_Disp_Data_Start_Index;	//data should be filled till 802 index

	temp_value = 4095 - bl_average;

	for(uint16_t i = Actual_Count_ble; i < end_index; i++)
	{
		buffer_ble[Flag_BLE_Buffer_RD][i] = temp_value;
	}
}

void BLE_ARC_Graph_Fill_Display_Data(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 90;

	Arrange_Display_System_Mode_BLE_Data_Array();

	uint8_t temp_ble_data[4];
	uint8_t i = 0,Conf_index;
	uint16_t temp_val16;
	uint16_t byte_index;

	byte_index = buffer_ble_Graph_Disp_Data_Start_Index;

	buffer_ble[Flag_BLE_Buffer_RD][byte_index] 	= BLE_Disp_Start_Address;
	byte_index++;

	for(i = 0; i < BLE_Disp_Data_length; i++)
	{
		Conf_index = BLE_Display_Para_Index[BLE_Disp_Start_Address + i];

		if(BLE_DispReg_Type[BLE_Disp_Start_Address + i] == Type_16_Bit)
		{
			buffer_ble[Flag_BLE_Buffer_RD][byte_index] = BLE_Display_Registers[Conf_index];
			byte_index++;
		}
		else	//32 bit
		{
			BLE_Seperate_Bytes_Float(BLE_Display_Float[Conf_index],temp_ble_data);

			temp_val16 = temp_ble_data[1];
			temp_val16 = (temp_val16 << 8) + temp_ble_data[0];
			buffer_ble[Flag_BLE_Buffer_RD][byte_index] = temp_val16;
			byte_index++;

			temp_val16 = temp_ble_data[3];
			temp_val16 = (temp_val16 << 8) + temp_ble_data[2];
			buffer_ble[Flag_BLE_Buffer_RD][byte_index] = temp_val16;
			byte_index++;
		}
	}
}

void BLE_ARC_LCD_Display(void)							//SIMPLIFY THIS FUNCTION
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 91;

	if((Flag_BLE_BagLength_Arrived != 0) || ((mpm_int == 0) && (BLE_Temp_Timer_Disp >= Display_Rate_BLE_Mode))) 		//Added 09.08.2021..
	{//If speed becomes zero, once it will go into this routine and will change flag type

		if(Flag_BLE_BagLength_Arrived != 0)
		{
			if(Flag_BLE_Graph_ON != 0)
			{
				BLE_ARC_Send_Graph_Buffer_DMA();
			}
			else if(Flag_BLE_Read_ON != 0)
			{
				Arrange_Display_System_Mode_BLE_Data_Array();
				BLE_Send_Display_Data();
			}
			Flag_BLE_BagLength_Arrived = 0;
		}
		else if(Flag_BLE_Read_ON != 0)
		{
			Arrange_Display_System_Mode_BLE_Data_Array();
			BLE_Send_Display_Data();
		}
		BLE_Temp_Timer_Disp = 0;
	}
}

void BLE_Common_End_Function(uint8_t *temp_data,uint16_t byte_index)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 92;

	uint16_t total_length;
	uint16_t crc_calc;

	total_length = byte_index + 2;
	BLE_Seperate_Bytes(total_length - 3,&temp_data[1],&temp_data[2]);

	crc_calc = mmodbus_crc16(temp_data,byte_index);
	BLE_Seperate_Bytes(crc_calc,&temp_data[byte_index], &temp_data[byte_index + 1]);

	BLE_Push_Data_DMA_Tx_Buffer(temp_data,total_length);
//	HAL_UART_Transmit(&huart3, temp_data, total_length, 100);

	BLE_Total_Bytes_Sent = BLE_Total_Bytes_Sent + total_length;
	BLE_Total_Group_Sent++;
}


_Bool BLE_Connect_Response(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 93;

	uint16_t crc_received,crc_calc;
	uint8_t temp_str[8],byte_index;


	crc_calc = mmodbus_crc16(BLE_rec_buff,3);
	CRC_Disp = crc_calc;

//	HAL_UART_Transmit(&huart3, &CRC_Disp, 2, 100);

	crc_received = BLE_Combine_Bytes(BLE_rec_buff[3],BLE_rec_buff[4]);

	if(crc_calc == crc_received)
	{
		temp_str[0] = 	BLE_rec_buff[0];		//0xAA
		//length in temp_str[1] & [2]

		temp_str[3] = 	BLE_Index;
		BLE_Index++;
		temp_str[4] = 	BLE_rec_buff[1] | 0X20;
		temp_str[5] = 	BLE_rec_buff[2];

		byte_index = 6;

		BLE_Common_End_Function(temp_str,byte_index);
		return(1);	//OK
	}
	else
	{
		BLE_Send_Error_Response_General(BLE_Error_Code_CRC_Not_matched);
		return(0);	//Error
	}
}

void BLE_Info_Reset_Recreate_Reconnect_EncErr(uint8_t Reset_Recreate_Reconnect_EncErr)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 94;

	//RESET 		: 00 - RESET CAUSE 			- FAULT - PC(4) - 00
	//TCPRECREATE   : 01 - 00 					- DTC - DTC - DAM - DAM - DLC - DLC
	//RECON 		: 02 - RECONNECT CAUSE 		- DTC - DTC - DAM - DAM - DLC - DLC
	//ENCODER ERR	: 03 - 00					- ENC - ENC - TEC - TEC - TEC - TEC - CEC - CEC - PEC - PEC
	uint8_t temp_str[25],byte_index;

	temp_str[0] = Config_Parameters[Alt_Station_ID];

	temp_str[3] = BLE_Index;
	BLE_Index++;

	temp_str[4] = BLE_Command_Info_Device_Reset_or_TCP_Recreate;

	temp_str[5] = Reset_Recreate_Reconnect_EncErr;

	if(Reset_Recreate_Reconnect_EncErr == Info_Reset)
	{
		temp_str[6] = Reset_Cause;
		temp_str[7] = err_pc.FAULT;
		BLE_Seperate_Bytes_Int32(err_pc.PC,&temp_str[8]);
		temp_str[12] = 0x00;
		byte_index = 13;
	}
	else if(Reset_Recreate_Reconnect_EncErr == Info_TCP_Recreate)
	{
		temp_str[6] = 0x00;

		BLE_Seperate_Bytes(TCP_Dis_Total_Count, &temp_str[7], &temp_str[8]);
		BLE_Seperate_Bytes(TCP_Dis_Ack_Missing_Count, &temp_str[9], &temp_str[10]);
		BLE_Seperate_Bytes(TCP_Dis_Link_Count, &temp_str[11], &temp_str[12]);
		byte_index = 13;
	}
	else if(Reset_Recreate_Reconnect_EncErr == Info_TCP_Reconnect)
	{
		temp_str[6] = TCP_Reconnect_Cause;

		BLE_Seperate_Bytes(TCP_Dis_Total_Count, &temp_str[7], &temp_str[8]);
		BLE_Seperate_Bytes(TCP_Dis_Ack_Missing_Count, &temp_str[9], &temp_str[10]);
		BLE_Seperate_Bytes(TCP_Dis_Link_Count, &temp_str[11], &temp_str[12]);
		byte_index = 13;
	}
	else	//Encoder Error
	{
		temp_str[6] = 0x00;
		BLE_Seperate_Bytes(EncError_DispCount,&temp_str[7], &temp_str[8]);
		BLE_Seperate_Bytes_Int32(EncError_TotalCount,&temp_str[9]);
		BLE_Seperate_Bytes(Temp_Disp_Enc_Count_at_Z,&temp_str[13], &temp_str[14]);
		BLE_Seperate_Bytes(Temp_Disp_Prev_Enc_Count_at_Z,&temp_str[15], &temp_str[16]);
		byte_index = 17;
	}
	BLE_Common_End_Function(temp_str,byte_index);
}

