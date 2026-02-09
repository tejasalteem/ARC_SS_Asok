
extern TIM_HandleTypeDef htim2,htim3;

void ARC_LCD_Display(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 121;

	if(Flag_DMA_Data_Type == DMA_Data_Graph)						//Send Graph Data
	{
		if((Flag_BagLength_Arrived != 0) || ((mpm_int == 0) && (Temp_Timer_Disp >= Display_Rate_System_Mode)))
		{	//If speed becomes zero, once it will go into this routine and will change flag type
			if(DMA_Process_Status == DMA_Process_Idle)
			{
				if(Flag_BagLength_Arrived != 0)
				{
					if(Screen_ID == Page_Graph)
					{
						if(DMA_Tx_Stage == 0)
						{
							ARC_Graph_Fill_Dummy_Data_at_End();

							if((Flag_LCD_Init_Error == 0) && (Stage_AutoSearch == 0))
							{
								Send_lcd_buffer_DMA();
								DMA_Tx_Stage = 1;
							}
							else
							{
								DMA_Tx_Stage = 2;
							}
							DMA_Process_Status = DMA_Graph_Continue;
						}
					}
					else
					{
						DMA_Process_Status = DMA_Graph_Completed;
						Flag_BagLength_Arrived = 0;				//Now buffer can be switched
					}
				}
				else
				{
					DMA_Process_Status = DMA_Graph_Completed;
				}
				Temp_Timer_Disp = 0;
			}
			else if(DMA_Process_Status == DMA_Graph_Continue)	//Waiting for Graph Data portion sent
			{
				if(DMA_Tx_Stage == 0)
				{
					Transmit_Display_Parameters_Float();		//Float data required only on baglength
					DMA_Process_Status = DMA_Graph_Completed;
					Flag_BagLength_Arrived = 0;					//Now buffer can be switched
				}
				Temp_Timer_Disp = 0;
			}

			if(DMA_Process_Status == DMA_Graph_Completed)
			{
				Flag_DMA_Data_Type = DMA_Data_Disp;

				Arrange_Display_System_Mode_LCD_Data_Array();

				if(Screen_ID == Scr_Home)
				{
					Transmit_Display_Parameters_Float();
					Transmit_Display_Parameters_Home_ARCSS();
				}
				else if(Screen_ID == Page_Graph)
				{
					Transmit_Display_Parameters_Float();
					Transmit_Display_Parameters_Graph_ARCSS();
				}
				else if(Screen_ID == Page_Diagnose)
				{
					Transmit_Display_Parameters_Float();
					Transmit_Display_Parameters_Diagnosis_ARCSS();
				}
				else if(Screen_ID == Page_Setting)
				{
					Transmit_Config_Parameters_Single(LED_Status_Code_Location,Buttons_Status_LEDs);
				}
				else if(Screen_ID == Page_Error)
				{
					Transmit_Config_Parameters_Single(Error_Status_Code_Location,Cur_Error_Status_Code);
				}

				//==============================================
				if(Flag_Previous_Alarm != Flag_Alarm)
				{
					if(Flag_Alarm == 0)
					{
						Transmit_Buzzer_OFF();
					}
					else
					{
						Transmit_Buzzer_ON();
					}
				}
				Flag_Previous_Alarm = Flag_Alarm;

				//==============================================
				Manage_Error_String();
				//==============================================

				Temp_Timer_Disp = 0;
			}
		}
		else if(DMA_Process_Status == DMA_Process_Idle)	//All data is sent and inbetween other data to be sent like scr change etc
		{
			DMA_Transmission_LCD();
		}
	}
	else	//When Graph Data is sent, and remaining data is being sent
	{
		DMA_Transmission_LCD();
	}
}

void Clear_Buffer(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 122;

	int j,k,end_index;
	uint8_t i;

	end_index = size_buffer_lcd - 2;

	for(i = 0; i < 2; i++)
	{
		for(j = 4, k = 3; j < end_index; j++, k++)	//0 to 3 are reserved for starting header, last2 for end header
		{
			buffer_lcd[i][j] = 0;
			buffer_ble[i][k] = 0;
		}
	}
	Count_lcd = 4;									//0 to 3 are reserved for starting header
	Count_ble = 3;
}

void Fill_lcd_buffer_StartEndBytes(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 123;

	uint8_t i;

	for(i = 0; i < 2; i++)
	{
		buffer_lcd[i][0] = 0x4DAA;
		buffer_lcd[i][1] = 0x0600;
		buffer_lcd[i][2] = 0x0000;
		buffer_lcd[i][3] = 0x2003;

		buffer_lcd[i][size_buffer_lcd - 2] = 0x33CC;
		buffer_lcd[i][size_buffer_lcd - 1] = 0x3CC3;

		//TOTAL LENGTH = 1654
		//LENGTH VARIABLES = 1654 - 3 = 1651 = 0x0673
		buffer_ble[i][0] = 0x7300 | Config_Parameters[Alt_Station_ID];		//LENL 	- Station ID
		buffer_ble[i][1] = 0x0006;		//INDEX - LENL						//Index will be updated at every graph data sent
		buffer_ble[i][2] = 0x004F;		//0X02 	- CMD(0X4F)
		buffer_ble[i][size_buffer_ble - 1] = 0x5555;
	}
}

void Send_lcd_buffer_DMA(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 124;
	HAL_UART_Transmit_IT(&huart4, (uint8_t *)buffer_lcd[Flag_Buffer_RD], size_buffer_lcd*2);
}

void ARC_Graph_Fill_Dummy_Data_at_End(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 125;

	uint16_t temp_value,temp_value_h,temp_value_l,end_index;
	end_index = size_buffer_lcd - 2;

	temp_value = 4095 - bl_average;

	temp_value_h 	= temp_value / 0x100;
	temp_value_l 	= temp_value % 0x100;
	temp_value 		= temp_value_l * 0x100 + temp_value_h;

	for(uint16_t i = Actual_Count_lcd; i < end_index; i++)
	{
		buffer_lcd[Flag_Buffer_RD][i] 	= temp_value;
	}
	//AA-CMD-ADD1-ADD2-ADD3-ADD4-LENL-LENH
	buffer_lcd[Flag_Buffer_RD][0] = 0x4DAA;
	buffer_lcd[Flag_Buffer_RD][1] = 0x0600;
	buffer_lcd[Flag_Buffer_RD][2] = 0x0000;
	buffer_lcd[Flag_Buffer_RD][3] = 0x2003;

	//END BYTES
	buffer_lcd[Flag_Buffer_RD][size_buffer_lcd - 2] = 0x33CC;
	buffer_lcd[Flag_Buffer_RD][size_buffer_lcd - 1] = 0x3CC3;
}

/* Graph Fill Up Interrupt of Encoder after every Skip Count ****/
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	arc_execution_int_prev_id = arc_execution_func_cur_id;
	arc_execution_int_id = 11;

	if(Flag_BagLength == 0)
	{
		uint16_t arr = __HAL_TIM_GET_AUTORELOAD(&htim3);
		uint16_t next = __HAL_TIM_GET_COMPARE(&htim3,TIM_CHANNEL_3)+Skip_Limit;
		if(next < arr)
		{
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,next);

			uint16_t temp_ADC_Value = (uint16_t)(hadc1.Instance->DR & 0x0FFF);

			uint16_t temp_value,temp_value_h,temp_value_l;

			temp_value = 4095 - temp_ADC_Value;
			buffer_ble[Flag_BLE_Buffer_WR][Count_ble] = temp_value;

			temp_value_h 	= temp_value / 0x100;
			temp_value_l 	= temp_value % 0x100;
			temp_value 		= temp_value_l * 0x100 + temp_value_h;

			buffer_lcd[Flag_Buffer_WR][Count_lcd] = temp_value;

			//In Graph buffer, data starts from index 4 i.e. 4 to 803 = 800 data.. so should not increment when value is 803, else it will fill data on 804
			if(Count_lcd < (size_buffer_lcd - 3))	//806 - 3 = 803 is last index to be filled
			{
				Count_lcd++;
				Count_ble++;
			}
		}
	}
	arc_execution_int_id = 12;
}

/* ADC WatchDog Mode Outof Window Callback */
void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef *hadc)
{
	if(Flag_TIM3_Baglength_Set_Threshold == 0)
	{
	arc_execution_int_prev_id = arc_execution_func_cur_id;
	arc_execution_int_id = 13;

	static uint16_t MarkFndEncCnt = 0;
	static uint32_t MarkFndTimeuS = 0;
	uint16_t temp_diff;
	uint16_t temp_encoder_val = __HAL_TIM_GET_COUNTER(&htim3);		//PPR

	if(Flag_Mark_Start == 1)		// Comes here at Falling Edge of mark in Graph
	{
		ADC_H_Count++;
		Flag_Mark_Start = 0;

		if(Temp_Diff_BL_VBL != 0)
		{
			ADC_ChangeThreshold(Temp_Diff_BL_VBL,Max_Count_Base_Line);
		}

		temp_diff = temp_encoder_val - MarkFndEncCnt;

		if((temp_diff >= Min_Mark_Height_Count) && (temp_diff <= Max_Mark_Height_Count))
		{
			//AutoSearch
			if((Stage_AutoSearch == 2) && (Flag_GateSet == 0))
			{
				if(MarkCount == 0)
				{
						AutoSearch_PPR = temp_encoder_val;
						AutoSearch_Rising_PPR = MarkFndEncCnt;
						AutoSearch_Falling_PPR = temp_encoder_val;
				}
				else
				{
					if(MarkCount == 1)
					{
						uint16_t temp_distance = temp_encoder_val - AutoSearch_PPR;
						AutoSearch_PPR = temp_encoder_val;

						if((temp_distance >= (14 * Convert_mm_To_PPR)) && (temp_distance <= (24 * Convert_mm_To_PPR)))
						{
							AutoSearch_PPR = AutoSearch_PPR - GateWidth_PPR;
							AutoSearch_Rising_PPR = MarkFndEncCnt - GateWidth_PPR;
							AutoSearch_Falling_PPR = AutoSearch_PPR;
						}
						else
						{
							MarkCount --;
						}
					}
				}
			}

			MarkCount++;

//			if((Flag_GateSet != 0) || (Indexing_stage_at_System_Start == 2))
			{
				if((temp_encoder_val >= Gate_Start_ppr[Master_Horz]) && (temp_encoder_val < Gate_End_ppr[Master_Horz]))
				{
					Flag_M0_Detected = 1;
					Mark_M0_Height_Diff_Count = temp_diff;

					if(Config_Parameters[Alt_Encoder_Time_Mode] == Encoder_Mode)
					{
						Mark_Encoder_Value[Master_Horz][Rising_Edge] = MarkFndEncCnt;
						Mark_Encoder_Value[Master_Horz][Falling_Edge] = temp_encoder_val;
					}
					else
					{
						Mark_TimeStamp_uS[Master_Horz][Rising_Edge] = MarkFndTimeuS;
						Mark_TimeStamp_uS[Master_Horz][Falling_Edge] = __HAL_TIM_GET_COUNTER(&htim2);
					}
				}

				if((temp_encoder_val >= Gate_Start_ppr[Master_Vert]) && (temp_encoder_val < Gate_End_ppr[Master_Vert]))
				{
					Flag_M1_Detected = 1;
					Mark_M1_Height_Diff_Count = temp_diff;

					if(Config_Parameters[Alt_Encoder_Time_Mode] == Encoder_Mode)
					{
						Mark_Encoder_Value[Master_Vert][Rising_Edge] = MarkFndEncCnt;
						Mark_Encoder_Value[Master_Vert][Falling_Edge] = temp_encoder_val;
					}
					else
					{
						Mark_TimeStamp_uS[Master_Vert][Rising_Edge] = MarkFndTimeuS;
						Mark_TimeStamp_uS[Master_Vert][Falling_Edge] = __HAL_TIM_GET_COUNTER(&htim2);
					}
				}

				if ((temp_encoder_val >= Gate_Start_ppr[Slave]) && (temp_encoder_val < Gate_End_ppr[Slave]))
				{
					Flag_M2_Detected = 1;
					Mark_M2_Height_Diff_Count = temp_diff;

					if(Config_Parameters[Alt_Encoder_Time_Mode] == Encoder_Mode)
					{
						Mark_Encoder_Value[Slave][Rising_Edge] = MarkFndEncCnt;
						Mark_Encoder_Value[Slave][Falling_Edge] = temp_encoder_val;

					}
					else
					{
						Mark_TimeStamp_uS[Slave][Rising_Edge] = MarkFndTimeuS;
						Mark_TimeStamp_uS[Slave][Falling_Edge] = __HAL_TIM_GET_COUNTER(&htim2);
					}
				}
			}
		}
	}
	else		// Comes here at Rising Edge of mark in Graph
	{
		if(Temp_Diff_BL_VBL != 0)
		{
//			ADC_ChangeThreshold(0,Temp_Diff_BL_VBL);
			ADC_ChangeThreshold(0,Temp_Diff_BL_SG);
		}

		MarkFndEncCnt = temp_encoder_val;
		MarkFndTimeuS = __HAL_TIM_GET_COUNTER(&htim2);
		Flag_Mark_Start = 1;
	}
	arc_execution_int_id = 14;
	}
}

//==============================================================================

void Transmit_Display_Parameters_Float(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 128;

    uint8_t temp_length;

    if(Screen_ID == Scr_Home)
    {
    	temp_length = Float_Length_Home;
    	Transmit_Config_Float(AltF_Disp_MarkHeight_Horz,temp_length);
    }
    else if(Screen_ID == Page_Graph)
    {
    	temp_length = 2;	//Two Errors in Graph Screen
    	Transmit_Config_Float(AltF_Disp_Error_Horz,temp_length);
    }
    else
    {
    	temp_length = Float_Length_Diagnosis;
    	Transmit_Config_Float(AltF_Disp_MarkHeight_Horz,temp_length);
    }
}

void Transmit_Display_Parameters_Home_ARCSS(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 129;

    uint8_t temp_data[35],temp_length = 11,index,i;		//11 * 2 + 7 = 29

    temp_data[0] = 0xAA;
    temp_data[1] = 0x82;

    temp_data[2] = 0x00;
    temp_data[3] = 0x08;
    temp_data[4] = 0x00;
    temp_data[5] = 0x5A;		//0x60 earlier

    temp_data[6] = temp_length;

    index = 7;

    for(i = 0; i < temp_length; i++)
    {
    	Seperate_Bytes(LCD_Display_Registers[i],&temp_data[index],&temp_data[index + 1]);
    	index = index + 2;
    }
	temp_length = (temp_length * 2) + 7;

	Push_Data_DMA_Tx_Buffer(temp_data,temp_length,1);
}

void Transmit_Display_Parameters_Graph_ARCSS(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 130;

    uint8_t temp_data[60],temp_length = 5,index,i;		//(max 22 * 2 + 7 = 51)

    uint8_t i_end;

    if(Flag_Update_Graph_Gates != 0)
    	temp_length = 22;

    Flag_Update_Graph_Gates = 0;

    temp_data[0] = 0xAA;
    temp_data[1] = 0x82;

    temp_data[2] = 0x00;
    temp_data[3] = 0x08;
    temp_data[4] = 0x00;
    temp_data[5] = 0x6C;
    temp_data[6] = temp_length;

    index = 7;

    i_end = LCD_DISP_SPEED + temp_length;

    for(i = LCD_DISP_SPEED; i < i_end; i++)
    {
    	Seperate_Bytes(LCD_Display_Registers[i],&temp_data[index],&temp_data[index + 1]);
    	index = index + 2;
    }

	temp_length = (temp_length * 2) + 7;
	Push_Data_DMA_Tx_Buffer(temp_data,temp_length,1);
}

void Transmit_Display_Parameters_Diagnosis_ARCSS(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 131;

    uint8_t temp_data[90],temp_length = 36,index,i;				//36 * 2 + 7 = 79 without Error LEDs
    uint8_t i_end;

    temp_data[0] = 0xAA;
    temp_data[1] = 0x82;

    temp_data[2] = 0x00;
    temp_data[3] = 0x08;
    temp_data[4] = 0x00;
    temp_data[5] = 0x64;

    temp_data[6] = temp_length;

    index = 7;

    i_end = LCD_DISP_AVG_HORZ + temp_length;

    for(i = LCD_DISP_AVG_HORZ; i < i_end; i++)
    {
    	Seperate_Bytes(LCD_Display_Registers[i],&temp_data[index],&temp_data[index + 1]);
    	index = index + 2;
    }

	temp_length = (temp_length * 2) + 7;
	Push_Data_DMA_Tx_Buffer(temp_data,temp_length,1);
}
