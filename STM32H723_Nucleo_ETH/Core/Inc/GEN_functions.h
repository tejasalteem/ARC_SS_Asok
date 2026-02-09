extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart3;

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == UART4)
	{
		DMA_Tx_Stage = 2;
	}
}

void DMA_Transmission_LCD(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 107;

	if(DMA_Tx_Stage == 0)
	{
		if(Flag_High_Priority_LCD_XY_Mode != 0)
		{
			if(Flag_LCD_Init_Error == 0)
			{
				HAL_UART_Transmit_IT(&huart4, Buffer_High_Priority_LCD_XY_Mode, Length_High_Priority_LCD_XY_Mode);
				DMA_Tx_Stage = 1;
			}
			else
			{
				DMA_Tx_Stage = 2;
			}
			Flag_High_Priority_LCD_XY_Mode = 0;
		}
		else if(DMA_Tx_Wr_Count != DMA_Tx_Rd_Count)
		{
			if(Flag_LCD_Init_Error == 0)
			{
				HAL_UART_Transmit_IT(&huart4, DMA_Tx_Buffer[DMA_Tx_Rd_Count], DMA_Tx_Length[DMA_Tx_Rd_Count]);
				DMA_Tx_Stage = 1;
			}
			else
			{
				DMA_Tx_Stage = 2;
			}
			DMA_Tx_Length[DMA_Tx_Rd_Count] = 0;		//After Reading make that length para zero

			DMA_Tx_Rd_Count++;
			if(DMA_Tx_Rd_Count == DMA_Tx_Buffer_length)
				DMA_Tx_Rd_Count = 0;
			if(DMA_Tx_Wr_Count == DMA_Tx_Rd_Count)
			{
				DMA_Tx_Wr_Count = 0;
				DMA_Tx_Rd_Count = 0;
				Flag_DMA_Data_Type = DMA_Data_Graph;
				DMA_Process_Status = DMA_Process_Idle;
			}
		}
		else if(DMA_Tx_Wr_Count == DMA_Tx_Rd_Count)			//Added 09.08.2021
		{
			DMA_Tx_Wr_Count = 0;
			DMA_Tx_Rd_Count = 0;
			Flag_DMA_Data_Type = DMA_Data_Graph;
			DMA_Process_Status = DMA_Process_Idle;
		}
	}
}

void Set_Alarm_Status(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 108;

	if((Flag_Over_Error != 0) || (Flag_Mark_Missing != 0) || (Flag_BaseLine_Error != 0))
	{
		Flag_Alarm = 1;
		if(Flag_TCP_Connected == 0)
		{
			HAL_GPIO_WritePin(DO_5_GPIO_Port, DO_5_Pin,Pulse_Level_Active);
			HAL_GPIO_WritePin(DO_6_GPIO_Port, DO_6_Pin,Pulse_Level_Active);
		}
	}
	else
	{
		Flag_Alarm = 0;
		if(Flag_TCP_Connected == 0)
		{
			HAL_GPIO_WritePin(DO_5_GPIO_Port, DO_5_Pin,Pulse_Level_Idle);
			HAL_GPIO_WritePin(DO_6_GPIO_Port, DO_6_Pin,Pulse_Level_Idle);
		}
	}
}

void Manage_Error_String(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 109;

	if((Prev_Error_Status_Code != Cur_Error_Status_Code) || (Flag_Home != 0))
	{
		if(Flag_Alarm != 0)
		{
			Error_Make_and_Send_String();
		}
		Set_Colour_Alarm(Flag_Alarm);

		if(Flag_Home != 0)
			Flag_Home = 0;
		Prev_Error_Status_Code = Cur_Error_Status_Code;
	}
}

void Error_Make_and_Send_String(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 110;

	if(Flag_Multiple_Errors != 0)
		strcpy((char *)Error_Status_String,"MULTIPLE ERRORS.. OPEN ERROR SCREEN TO IDENTIFY");
	else
	{
		if((Error_Status_Code & 0x0001) != 0)
		{
			strcpy((char *)Error_Status_String,"ERROR OUT OF RANGE");
		}

		if((Error_Status_Code & 0x0002) != 0)
		{
			strcpy((char *)Error_Status_String,"ERROR : MARK MISSING");
		}

		if((Error_Status_Code & 0x0004) != 0)
		{
			strcpy((char *)Error_Status_String,"ERROR : DISPLAY ERROR");
		}

		if((Error_Status_Code & 0x0008) != 0)
		{
			strcpy((char *)Error_Status_String,"WARNING : BASELINE ERROR.. PLS CHECK SENSOR..");
		}
	}
	Error_Send_String();
}

//===================================

void Set_Application_Mode(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 111;

	uint8_t temp_page_offset = (Config_Parameters[Alt_Correction_Direction_Horz] == Config_Parameters[Alt_Machine_Direction] ? 1 : 0);
	Scr_Home = Page_Home + temp_page_offset;
	Scr_Good_Copy = Page_Good_Copy + temp_page_offset;
}



void Set_Colour_Alarm(_Bool Flag_AlarmStatus)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 112;

    uint8_t temp_data[11],temp_length = 2;

    temp_data[0] = 0xAA;
    temp_data[1] = 0x82;

    temp_data[2] = 0x00;
    temp_data[3] = 0x08;
    temp_data[4] = 0x00;
    temp_data[5] = 0xD6;

    temp_data[6] = temp_length;

    if(Flag_AlarmStatus == 0)	//Alarm OFF
    {
    	Seperate_Bytes(0X0000,&temp_data[7],&temp_data[8]);				//Black Font
		Seperate_Bytes(0X0000,&temp_data[9],&temp_data[10]);			//Black BG
    }
    else						//Alarm ON
    {
		Seperate_Bytes(0XF800,&temp_data[7],&temp_data[8]);				//Red Font
		Seperate_Bytes(0X39E7,&temp_data[9],&temp_data[10]);			//GREY BG
    }
	temp_length = temp_length * 2 + 7;
	Push_Data_DMA_Tx_Buffer(temp_data,temp_length,1);
}


void Error_Send_String(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 113;

	uint8_t temp_len;
	uint8_t wr_str_start[]={0xAA,0x42,0x00,0x00,0x01,0x00};

	temp_len = strlen((char *)Error_Status_String) + 1;

	Push_Data_DMA_Tx_Buffer(wr_str_start,6,0);
	Push_Data_DMA_Tx_Buffer(Error_Status_String,temp_len,1);
}

void Job_Name_Send_String(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 114;

	uint8_t temp_len;
	uint8_t wr_str_start[]={0xAA,0x42,0x00,0x00,0x00,0x80};

	temp_len = strlen((char *)Job_Name) + 1;

	Push_Data_DMA_Tx_Buffer(wr_str_start,6,0);
	Push_Data_DMA_Tx_Buffer((uint8_t *)Job_Name,temp_len,1);
}

void File_Name_Send_String(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 115;

	uint8_t temp_len;
	uint8_t wr_str_start[]={0xAA,0x42,0x00,0x00,0x01,0x80};

	temp_len = strlen((char *)OTA_File_Name) + 1;

	HAL_UART_Transmit(&huart4,wr_str_start,6,100);
	Push_Data_DMA_Tx_Buffer((uint8_t *)OTA_File_Name,temp_len,1);
}

void Transmit_Buzzer_ON(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 117;

	uint8_t br_array[]={0XAA,0X7A,0XFF,0X02,0X02,0X05,0X00};	//brightness min

	Push_Data_DMA_Tx_Buffer(br_array,7,1);
}

void Transmit_Buzzer_OFF(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 118;

	uint8_t br_array[]={0XAA,0X7A,0X01,0X00,0X00,0X00,0X00};	//brightness min

	Push_Data_DMA_Tx_Buffer(br_array,7,1);
}
