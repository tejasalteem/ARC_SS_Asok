/* Himanshu */
/* @brief  Change Repetition Counter Register */
#define __HAL_TIM_SET_REPETITION_COUNTER(__HANDLE__, __COUNTER__) \
do { \
  ((__HANDLE__)->Instance->RCR = (__COUNTER__));				\
  ((__HANDLE__)->Instance->EGR = 1);							\
}while(0)

extern UART_HandleTypeDef huart4,huart3;
extern TIM_HandleTypeDef htim16,htim15,htim3,htim2,htim1,htim6;
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc3;
extern DMA_HandleTypeDef hdma_adc3;
extern DAC_HandleTypeDef hdac1;
extern char Debug_String[200];

extern uint16_t RxXferSize;               /*!< UART Rx Transfer size              */
extern uint16_t RxXferCount;              /*!< UART Rx Transfer Counter           */

extern uint16_t filled_count;

//==================================================================================

uint16_t Get_Received_Data_Length(UART_HandleTypeDef *huart);

void Check_Config_Float_Limits(void);
_Bool Check_Config_Float_Limits_Single(uint8_t,float);	//Return 1 if within range, else 0
void Check_Config_Para_Limits(void);
_Bool Check_Config_Para_Limits_Single(uint8_t,int16_t);

void Check_For_LCD_Touch_Command(void);
void Check_For_LCD_Touch_Command_Touch_Buttons(void);
void Check_For_LCD_Touch_Command_Float(void);
void Check_For_LCD_Touch_Command_Int(void);

void Config_LCD_XY_Coordinates(_Bool);

void Gate_Calculations(void);
void Gate_Calculations_Master_Horizontal(void);
void Gate_Calculations_Master_Vertical(void);
void Gate_Calculations_Slave(void);
void Gate_Limit_Calculations(void);
void Gate_Limit_Calculations_Home(void);

void Handle_Line_Idle_Event(void);
void Init_Calc_ARCSS(void);
void Initialize_Config_Parameters(void);
void Initialize_Config_Float(void);

void Push_Data_DMA_Tx_Buffer(uint8_t *, uint8_t, _Bool);

void Send_String_UART_232(uint8_t *);
void Seperate_Bytes(int16_t, uint8_t *, uint8_t *);
void Seperate_Bytes_Float(float,uint8_t *);

void Transmit_Brightness_Value_00_3F(uint8_t);

void Transmit_Config_Parameters_Single(uint8_t,int16_t);
void Transmit_Config_Float(uint8_t start_para_index, uint8_t temp_length);
void Transmit_Config_Parameters(uint8_t start_para_index, uint8_t temp_length);

void Transmit_Display_Parameters_Home_ARCSS(void);
void Transmit_Display_Parameters_Graph_ARCSS(void);
void Transmit_Display_Parameters_Diagnosis_ARCSS(void);
void Transmit_Display_Parameters_Float(void);

void Transmit_EndBytes(void);
void Transmit_Init_Check(void);
void Transmit_Page_ID(unsigned char);

void update_lcd_buttons_status(void);
void Wait_for_LCD_Init(void);
void Format_LCD(void);

void Seperate_Bytes_Int32(uint32_t,uint8_t *);

//=======================
void check_password(uint16_t);

void Error_Send_String(void);
void Error_Make_and_Send_String(void);
void Set_Colour_Alarm(_Bool);

void Transmit_Buzzer_ON(void);
void Transmit_Buzzer_OFF(void);

void Set_Alarm_Status(void);
void Manage_Error_String(void);

//=================NEW FUNCTIONS ARC+TC=================

void ADC_ChangeThreshold(uint32_t LowThreshold,uint32_t HighThreshold);
void Clear_Buffer(void);
void DMA_Transmission_LCD(void);
void Action_LCD_Commands(void);
void Action_LCD_Commands_Home(void);

void Send_lcd_buffer_StartBytes(void);
void Send_lcd_buffer_DMA(void);
void Send_lcd_buffer_EndBytes(void);

void Set_Application_Mode(void);
void Common_Correction_Logic_Vertical(void);
void Common_Correction_Logic_Horizontal(void);
void Common_Flow_Logic(void);

void ARC_LCD_Display(void);
void ARC_Graph_Fill_Dummy_Data_at_End(void);

void Avg_Errors_Vertical(float,float);
void Avg_Errors_Horizontal(float,float);

//====================================================

void Digital_Correction_Vertical(_Bool,uint16_t);
void Digital_Correction_Horizontal(_Bool,uint16_t);
void Stop_Digital_Correction_Vertical(void);
void Stop_Digital_Correction_Horizontal(void);

void Do_Correction_Vertical(unsigned char,float,float);
void Do_Correction_Horizontal(unsigned char,float,float);
unsigned char CheckReverse_Correction_Vertical(float,float);
unsigned char CheckReverse_Correction_Horizontal(float,float);

uint16_t Get_Auto_Averaging_Count_Based_On_Speed_Horz(void);
uint16_t Get_Auto_Averaging_Count_Based_On_Speed_Vert(void);

void Calculate_Error_Home(void);
void Update_Diagnosis_Screen_Values(void);

void Clear_Timer_Interrupt(TIM_HandleTypeDef *htim);

//====================================================

void Arrange_Display_System_Mode_LCD_Data_Array(void);

void Slave_Handle_Line_Idle_Event(void);
uint16_t mmodbus_crc16(uint8_t *nData, uint16_t wLength);

void BLE_Send_Error_Response_General(uint8_t err_code);
void BLE_Send_Display_Data(void);	//05.02.2025

void BLE_Seperate_Bytes(uint16_t Temp_Value,uint8_t *Byte_Index_Low, uint8_t *Byte_Index_High);
uint16_t BLE_Combine_Bytes(uint8_t Data_LSB, uint8_t Data_MSB);
void BLE_Seperate_Bytes_Float(float Temp_Value,uint8_t *Float_Bytes);
float BLE_Combine_Bytes_Float(uint8_t *Float_Bytes);
void BLE_Seperate_Bytes_Int32(uint32_t Temp_Value,uint8_t *Int_Bytes);
uint32_t BLE_Combine_Bytes_Int32(uint8_t *Int_Bytes);
void BLE_ARC_Graph_Fill_Dummy_Data_at_End(void);
void BLE_ARC_Graph_Fill_Display_Data(void);
void BLE_ARC_LCD_Display(void);
void BLE_ARC_Send_Graph_Buffer_DMA(void);

void Arrange_Display_System_Mode_BLE_Data_Array(void);

void BLE_Common_End_Function(uint8_t *temp_data,uint16_t byte_index);
void BLE_Read_Config_Registers(void);
void BLE_Response_Read_Config_Registers(uint16_t BLE_Start_Address, uint16_t BLE_Data_length);

void BLE_Write_Single_Config_Register(void);
void BLE_Response_Write_Single_Config_Reg(uint16_t BLE_Data);
void BLE_Send_Error_Response_Write_Int(_Bool add_type, uint16_t reg_add, uint8_t err_code);
void BLE_Info_Config_Register_Changed(uint16_t BLE_Reg_Address, int16_t BLE_Data);

void BLE_Write_Single_Config_Register_Float(void);
void BLE_Response_Write_Single_Config_Register_Float(float BLE_Data);
void BLE_Send_Error_Response_Write_Float(_Bool add_type, uint16_t reg_add, uint8_t err_code);
void BLE_Info_Config_Register_Float_Changed(uint16_t BLE_Reg_Address,float BLE_Data);

void BLE_Write_Single_Coil(void);
void BLE_Response_Write_Single_Coil(uint16_t BLE_Data);
void BLE_Send_Error_Response_Write_Coil(uint16_t BLE_Coil_Address, uint16_t BLE_Coil_Data, uint8_t err_code);
void BLE_Write_Coil_Config_Info_Response(_Bool reg_type, uint16_t reg_add);

void BLE_Write_Job_Name(void);
void BLE_Response_Write_Job_Name(uint8_t);
void Job_Name_Send_String(void);
void Check_Job_Name(void);
void Initialize_Job_Name(void);

_Bool BLE_Connect_Response(void);

void BLE_Request_Station_ID(void);
void BLE_Response_Request_Station_ID(void);

_Bool BLE_Request_OTA(void);
void BLE_Response_Request_OTA(uint8_t);
void File_Name_Send_String(void);

void BLE_Send_Communication_Check_Frame(void);
void BLE_Info_Reset_Recreate_Reconnect_EncErr(uint8_t Flag_Reset_or_TCP);
void BLE_Push_Data_DMA_Tx_Buffer(uint8_t *str_ptr, uint16_t str_len);

//=======================================================

void Calculate_Multiplier_mm_to_pixel(void);
void Calculate_Position_Move_Icon_As_Per_Error(float error_horz, float error_vert);
void LCD_Draw_Colour_Box(void);

void Encoder_Init(void);

void Calculate_Baseline_ADC3_DMA(void);

//=======================================================

void Transmit_EndBytes(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 1;

	HAL_UART_Transmit(&huart4,EndBytes,4,10);
}

void Initialize_Config_Parameters(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 2;

	for( uint8_t i = 0; i < No_Of_Config_Parameters; i++)
	{
		Config_Parameters[i] = Config_Para_Limits[i][2];
	}
}

void Initialize_Config_Float(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 3;

	for( uint8_t i = 0; i < No_Of_Config_Float; i++)
	{
		Config_Float[i] = Config_Float_Limits[i][2];
	}
}

void Initialize_Job_Name(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 4;

	uint8_t length;
	strcpy(Job_Name,"DEFAULT");
	length = strlen(Job_Name);
	Job_Name[21] = length;
}

void Check_Job_Name(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 5;

	uint8_t length;
	length = strlen(Job_Name);
	if(length != Job_Name[21])
	{
		Initialize_Job_Name();
	}
}


_Bool Check_Config_Para_Limits_Single(uint8_t rec_index,int16_t rec_value)	//Return 1 if within range, else 0
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
		arc_execution_func_prev_id = arc_execution_func_cur_id;
		arc_execution_func_cur_id = 6;

		if((rec_value >= Config_Para_Limits[rec_index][0]) && (rec_value <= Config_Para_Limits[rec_index][1]))
		{
			return(1);
		}	
		else
			return(0);
}


_Bool Check_Config_Float_Limits_Single(uint8_t rec_index,float rec_value)	//Return 1 if within range, else 0
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
		arc_execution_func_prev_id = arc_execution_func_cur_id;
		arc_execution_func_cur_id = 7;

		if((rec_value >= Config_Float_Limits[rec_index][0]) && (rec_value <= Config_Float_Limits[rec_index][1]))
		{
			return(1);
		}
		else
			return(0);
}

void Check_Config_Para_Limits(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 8;

	for(uint8_t i = 0; i < (No_Of_Config_Parameters - 1); i++)
	{
		if((Config_Parameters[i] < Config_Para_Limits[i][0]) || (Config_Parameters[i] > Config_Para_Limits[i][1]))
		{
			Config_Parameters[i] = Config_Para_Limits[i][2];
		}
	}	
}

void Check_Config_Float_Limits(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 9;

	for(uint8_t i = 0; i < No_Of_Config_Float; i++)
	{
		if((Config_Float[i] < Config_Float_Limits[i][0]) || (Config_Float[i] > Config_Float_Limits[i][1]))
		{
			Config_Float[i] = Config_Float_Limits[i][2];
		}
	}
}

void Transmit_Config_Parameters(uint8_t start_para_index, uint8_t temp_length)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 10;

    unsigned char i,index;
    unsigned char temp_data[110];				//max 45 * 2 + 7 = 97

    temp_data[0] = 0xAA;
    temp_data[1] = 0x82;

    temp_data[2] = 0x00;
    temp_data[3] = 0x08;

    temp_data[4] = 0x00;
    temp_data[5] = start_para_index * 2;

	temp_data[6] = temp_length;

	index = 7;

	for(i = 0; i < temp_length; i++)
	{
		Seperate_Bytes(Config_Parameters[start_para_index + i],&temp_data[index], &temp_data[index + 1]);
		index = index + 2;
	}

	temp_length = index;

	Push_Data_DMA_Tx_Buffer(temp_data,temp_length,1);
}

void Transmit_Config_Parameters_Single(uint8_t para_index, int16_t para_value)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 11;

    unsigned char temp_data[9];				//max 64 * 2 + 7

    temp_data[0] = 0xAA;
    temp_data[1] = 0x82;

    temp_data[2] = 0x00;
    temp_data[3] = 0x08;

    temp_data[4] = 0x00;
    temp_data[5] = para_index * 2;

	temp_data[6] = 0x01;

	Seperate_Bytes(para_value,&temp_data[7], &temp_data[8]);

	Push_Data_DMA_Tx_Buffer(temp_data,9,1);
}

void Transmit_Config_Float(uint8_t start_para_index,uint8_t temp_length)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 12;

    uint8_t temp_data[90],i,index;			//Max Length 19 => 76 + 7 = 83

    temp_data[0] = 0xAA;
    temp_data[1] = 0x82;

    temp_data[2] = 0x00;
    temp_data[3] = 0x02;

    temp_data[4] = 0x00;
    temp_data[5] = start_para_index * 4;

    temp_data[6] = temp_length;			//No of Floats to send

	index = 7;

	for(i = 0; i < temp_length; i++)
	{
		Seperate_Bytes_Float(Config_Float[start_para_index + i], &temp_data[index]);
		index = index + 4;
	}

	temp_length = index;

	Push_Data_DMA_Tx_Buffer(temp_data,temp_length,1);
}

void Transmit_Config_Float_Single(uint8_t para_index,float para_value)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 13;

    uint8_t temp_data[11];			//Max Length 16 => 64 + 7 = 71

    temp_data[0] = 0xAA;
    temp_data[1] = 0x82;

    temp_data[2] = 0x00;
    temp_data[3] = 0x02;

    temp_data[4] = 0x00;
    temp_data[5] = para_index * 4;

    temp_data[6] = 0x01;			//No of Floats to send

	Seperate_Bytes_Float(para_value, &temp_data[7]);

	Push_Data_DMA_Tx_Buffer(temp_data,11,1);
}


void Send_String_UART_232(uint8_t *str_u3)
{
	uint16_t temp_len = strlen((char *)str_u3);
    HAL_UART_Transmit(&huart3, (uint8_t *)str_u3, temp_len, 100);
}

void Check_For_LCD_Touch_Command_Touch_Buttons(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 15;

	uint8_t lcd_Component_id;

	if(Flag_BLE_Value_Received != 0)
	{
		lcd_Component_id = Temp_BLE_Reg_Address;
	}
	else
	{
		lcd_Component_id = lcd_received_bytes[2];
	}

    if(Flag_GenKeyPressed == 0)
    {
    	switch(lcd_Component_id)
    	{
			case LCD_Btn_Auto_Horz	:
								if(Flag_Auto_Horizontal == 0)
								{
									if(Flag_GateSet != 0)
									{
										if(Flag_Permit_Actions != 0)
										{
											Repeat_Length[Horizontal] = 0;
											__HAL_TIM_SET_COUNTER(&htim16,0);
											__HAL_TIM_SET_AUTORELOAD(&htim16,Max_Value_Timer_Idle);		//Max_Value_Timer_Idle
											HAL_TIM_Base_Start_IT(&htim16);
											Flag_Auto_Horizontal = 1;
											Flag_Auto_Any = 1;

											BLE_Coil_Write_Success = 1;

											Auto_HV_Status = Flag_Auto_Horizontal * 2 + Flag_Auto_Vertical;
											Flash_Write_Single_Byte(Flash_Address_Auto,Auto_HV_Status);
										}
									}
								}
								else
								{
									Flag_Auto_Horizontal = 0;
									Repeat_Length[Horizontal] = 0;
									Stop_Digital_Correction_Horizontal();
									HAL_TIM_Base_Stop(&htim16);
									if(Flag_Auto_Vertical == 0)
									{
										Flag_Auto_Any = 0;
									}
									Flag_Over_Error_Horz = 0;

									BLE_Coil_Write_Success = 1;

									Auto_HV_Status = Flag_Auto_Horizontal * 2 + Flag_Auto_Vertical;
									Flash_Write_Single_Byte(Flash_Address_Auto,Auto_HV_Status);
								}
								break;

			case LCD_Btn_Auto_Vert	:
								if(Flag_Auto_Vertical == 0)
								{
									if(Flag_GateSet != 0)
									{
										if(Flag_Permit_Actions != 0)
										{
											Repeat_Length[Vertical] = 0;
											__HAL_TIM_SET_COUNTER(&htim15,0);
											__HAL_TIM_SET_AUTORELOAD(&htim15,Max_Value_Timer_Idle);
											HAL_TIM_Base_Start_IT(&htim15);
											Flag_Auto_Vertical = 1;
											Flag_Auto_Any = 1;

											BLE_Coil_Write_Success = 1;

											Auto_HV_Status = Flag_Auto_Horizontal * 2 + Flag_Auto_Vertical;
											Flash_Write_Single_Byte(Flash_Address_Auto,Auto_HV_Status);
										}
									}
								}
								else
								{
									Flag_Auto_Vertical = 0;
									Repeat_Length[Vertical] = 0;
									Stop_Digital_Correction_Vertical();
									HAL_TIM_Base_Stop(&htim15);
									if(Flag_Auto_Horizontal == 0)
									{
										Flag_Auto_Any = 0;
									}
									Flag_Over_Error_Vert = 0;

									BLE_Coil_Write_Success = 1;

									Auto_HV_Status = Flag_Auto_Horizontal * 2 + Flag_Auto_Vertical;
									Flash_Write_Single_Byte(Flash_Address_Auto,Auto_HV_Status);
								}
								break;

			case LCD_Btn_Home	:
								Screen_ID = Scr_Home;
                				Transmit_Page_ID(Screen_ID);
                				Flag_Password_OK = 0;
    							Flag_Spl_Rights = 0;
    							Flag_Home = 1;
								break;

    		case LCD_Btn_Graph	:
								Screen_ID = Page_Graph;
                				Transmit_Page_ID(Screen_ID);
                				Flag_Password_OK = 0;
    							Flag_Spl_Rights = 0;

    							Flag_Update_Graph_Gates = 1;

    							if((Flag_Home_Mode != 0) || (Flag_GateSet != 0) || (Flag_All_Marks_OK != 0) ||
    									(Flag_BaseLine_Error != 0) || (Indexing_stage_at_System_Start != 0) || (Stage_AutoSearch != 0))
    							{
    								break;
    							}

								Stage_AutoSearch = 1;
								Config_Parameters[Alt_Index_mm] = 0;
								Flag_Index = 1;
								Flag_Index1 = 0;
                				break;

			case LCD_Btn_Diagnose	:
								if(Flag_Master_Login != 0)
								{
									Flag_Password_OK = 1;
									Flag_Spl_Rights = 1;
								}

								if(Flag_Password_OK == 0)		//From any screen, other than recipe_edit
								{
									Screen_ID = Page_Password;
									Page_to_Proceed = Page_Diagnose;
									Flag_Spl_Rights = 0;
								}
								else
								{
									Screen_ID = Page_Diagnose;
								}
								Format_LCD_Check = 0;
								Parameter_Default_Check = 0;
								Transmit_Page_ID(Screen_ID);
								break;

			case LCD_Btn_Settings:
								if(Flag_Master_Login != 0)
								{
									Flag_Password_OK = 1;
									Flag_Spl_Rights = 1;
								}

								if(Flag_Password_OK == 0)		//From any screen, other than recipe_edit
								{
									Screen_ID = Page_Password;
									Page_to_Proceed = Page_Setting;
	    							Flag_Spl_Rights = 0;
								}
								else
								{
									Screen_ID = Page_Setting;
								}
								Format_LCD_Check = 0;
								Parameter_Default_Check = 0;
								Transmit_Page_ID(Screen_ID);
								break;

    		case LCD_BtnG_Left	:
								if((Flag_GenKeyPressed == 0) && (Flag_Auto_Any == 0))
    							{
    								rec_function_byte = 'L';

    								if(Flag_BLE_Value_Received == 0)
    								{
    									Config_LCD_XY_Coordinates(0);		//XY
    								}
    								else
    								{
    									Flag_BLE_KeyPress_Event = 1;
    								}
									BLE_Coil_Write_Success = 1;
    							}
    							break;

    		case LCD_BtnG_Right	:
								if((Flag_GenKeyPressed == 0) && (Flag_Auto_Any == 0))
								{
									rec_function_byte = 'R';

    								if(Flag_BLE_Value_Received == 0)
    								{
    									Config_LCD_XY_Coordinates(0);		//XY
    								}
    								else
    								{
    									Flag_BLE_KeyPress_Event = 1;
    								}
									BLE_Coil_Write_Success = 1;
								}
    							break;

    		case LCD_BtnG_LeftFine	:
								if((Flag_GenKeyPressed == 0) && (Flag_Auto_Any == 0))
								{
									rec_function_byte = 'l';

									BLE_Coil_Write_Success = 1;
								}
    							break;

    		case LCD_BtnG_RightFine :
								if((Flag_GenKeyPressed == 0) && (Flag_Auto_Any == 0))
								{
									rec_function_byte = 'r';

									BLE_Coil_Write_Success = 1;
								}
    							break;

    		case LCD_Btn_Alarm :
    							if(Flag_Alarm != 0)
    							{
									Screen_ID = Page_Error;
									Transmit_Page_ID(Screen_ID);
    							}
    							break;

    		case LCD_BtnG_GateSet :
								if((Flag_GateSet != 0) || (Flag_Auto_Any != 0) || (Flag_Index != 0) || (Flag_Index1 != 0) ||
										(Stage_AutoSearch != 0) || (Flag_BaseLine_Error != 0) || (Flag_Permit_Actions == 0))
									break;

								Flag_GateSet = 1;
								Repeat_Length[Horizontal] = 0;
								Repeat_Length[Vertical] = 0;
								Flash_Write_Config_Parameters_Single(Alt_Position_Gate_Slave_GCount);

								Flag_Event_Gate_Set = Flag_Home_Mode;
								BLE_Coil_Write_Success = 1;
								break;

			case LCD_Btn_Clear_Production :
								if((Flag_TCP_Connected != 0) && (Flag_Use_With_PC != 0) && (Flag_BLE_Value_Received == 0))
									break;

								Actual_Meter_Count  = 0;
								Transmit_Config_Float_Single(AltF_Disp_Production,0);	//Production Meter = 0
								BLE_Coil_Write_Success = 1;

//								Flag_Slave_DMA_Buffer_Write = 0;	//Why??	Disabled 02.08.2025
//								Flag_Slave_DMA_Buffer_Read  = 0;	//Why?? Disabled 02.08.2025
								break;

			case LCD_Btn_Recipe :
								if(Flag_Master_Login != 0)
								{
									Flag_Password_OK = 1;
									Flag_Spl_Rights = 1;
								}

								if(Flag_Password_OK == 0)			//From Home Screen
								{
									Screen_ID = Page_Recipe_View;
								}
								else								//From Setting Screen
								{
									Screen_ID = Page_Recipe_Edit;
								}
								Transmit_Page_ID(Screen_ID);
								break;

			case LCD_Btn_Recipe_Edit:
								Screen_ID = Page_Password;
								Page_to_Proceed = Page_Recipe_Edit;
								Transmit_Page_ID(Screen_ID);
								break;

			//Confirming Parameter Default
			case LCD_Btn_Confirm_Yes 	:
								Config_Parameters[Alt_Mem_Read_OK] = 0;
								Flash_Write_Config_Parameters_Single(Alt_Mem_Read_OK);
								Transmit_Page_ID(Page_Parameter_Default);
								break;

			case LCD_Btn_Confirm_No		:
								Flag_Password_OK = 0;
								Screen_ID = Scr_Home;
								Transmit_Page_ID(Screen_ID);
								Flag_Spl_Rights = 0;
								Flag_Home = 1;
								Format_LCD_Check = 0;
								Parameter_Default_Check = 0;
								break;

//===================================================================================

    		case LCD_Btn_Correction_Rev_Horz :
    							if((Flag_TCP_Connected != 0) && (Flag_Use_With_PC != 0) && (Config16_Local_Permission[Alt_Correction_Direction_Horz] == 0))
    								break;

    							if(Flag_Auto_Horizontal == 0)
    							{
    								Config_Parameters[Alt_Correction_Direction_Horz] = 1 - Config_Parameters[Alt_Correction_Direction_Horz];
									Transmit_Config_Parameters_Single(Alt_Correction_Direction_Horz,Config_Parameters[Alt_Correction_Direction_Horz]);
									Flash_Write_Config_Parameters_Single(Alt_Correction_Direction_Horz);

									if(Flag_BLE_Enable != 0)
									{
										if(Flag_BLE_Value_Received == 0)
										{
											BLE_Info_Config_Register_Changed(Alt_Correction_Direction_Horz,Config_Parameters[Alt_Correction_Direction_Horz]);
										}
									}
									BLE_Coil_Write_Success = 1;

									uint8_t temp_page_offset = (Config_Parameters[Alt_Correction_Direction_Horz] == Config_Parameters[Alt_Machine_Direction] ? 1 : 0);
									if(Screen_ID == Scr_Home)
									{
										Scr_Home = Page_Home + temp_page_offset;
										Screen_ID = Scr_Home;
										Transmit_Page_ID(Screen_ID);
									}
									else
									{
										Scr_Home = Page_Home + temp_page_offset;
									}
									Scr_Good_Copy = Page_Good_Copy + temp_page_offset;
    							}
								break;

    		case LCD_Btn_Correction_Rev_Vert :
								if((Flag_TCP_Connected != 0) && (Flag_Use_With_PC != 0) && (Config16_Local_Permission[Alt_Correction_Direction_Vert] == 0))
									break;

    							if(Flag_Auto_Vertical == 0)
    							{
    								Config_Parameters[Alt_Correction_Direction_Vert] = 1 - Config_Parameters[Alt_Correction_Direction_Vert];
									Transmit_Config_Parameters_Single(Alt_Correction_Direction_Vert,Config_Parameters[Alt_Correction_Direction_Vert]);
									Flash_Write_Config_Parameters_Single(Alt_Correction_Direction_Vert);

									if(Flag_BLE_Enable != 0)
									{
										if(Flag_BLE_Value_Received == 0)
										{
											BLE_Info_Config_Register_Changed(Alt_Correction_Direction_Vert,Config_Parameters[Alt_Correction_Direction_Vert]);
										}
									}
									BLE_Coil_Write_Success = 1;
    							}
								break;

    		case LCD_Btn_Auto_Averaging_Horz :
								if((Flag_TCP_Connected != 0) && (Flag_Use_With_PC != 0) && (Config16_Local_Permission[Alt_Auto_Averaging_Horz] == 0))
									break;

								Config_Parameters[Alt_Auto_Averaging_Horz] = 1 - Config_Parameters[Alt_Auto_Averaging_Horz];
								Transmit_Config_Parameters_Single(Alt_Auto_Averaging_Horz,Config_Parameters[Alt_Auto_Averaging_Horz]);
								Flash_Write_Config_Parameters_Single(Alt_Auto_Averaging_Horz);

								if(Flag_BLE_Enable != 0)
								{
									if(Flag_BLE_Value_Received == 0)
									{
										BLE_Info_Config_Register_Changed(Alt_Auto_Averaging_Horz,Config_Parameters[Alt_Auto_Averaging_Horz]);
									}
								}
								BLE_Coil_Write_Success = 1;
								break;

    		case LCD_Btn_Auto_Averaging_Vert :
								if((Flag_TCP_Connected != 0) && (Flag_Use_With_PC != 0) && (Config16_Local_Permission[Alt_Auto_Averaging_Vert] == 0))
									break;

								Config_Parameters[Alt_Auto_Averaging_Vert] = 1 - Config_Parameters[Alt_Auto_Averaging_Vert];
								Transmit_Config_Parameters_Single(Alt_Auto_Averaging_Vert,Config_Parameters[Alt_Auto_Averaging_Vert]);
								Flash_Write_Config_Parameters_Single(Alt_Auto_Averaging_Vert);

								if(Flag_BLE_Enable != 0)
								{
									if(Flag_BLE_Value_Received == 0)
									{
										BLE_Info_Config_Register_Changed(Alt_Auto_Averaging_Vert,Config_Parameters[Alt_Auto_Averaging_Vert]);
									}
								}
								BLE_Coil_Write_Success = 1;
								break;

    		case LCD_Btn_MasterMarkPosition :
								if((Flag_TCP_Connected != 0) && (Flag_Use_With_PC != 0) && (Config16_Local_Permission[Alt_MasterMarkPosition] == 0))
									break;

								if((Flag_Home_Mode == 0) && (Flag_Auto_Any == 0))
								{
									Config_Parameters[Alt_MasterMarkPosition] = 1 - Config_Parameters[Alt_MasterMarkPosition];
									Transmit_Config_Parameters_Single(Alt_MasterMarkPosition,Config_Parameters[Alt_MasterMarkPosition]);
									Flash_Write_Config_Parameters_Single(Alt_MasterMarkPosition);

									Flag_GateSet = 0;

									Position_Gate[Slave] = Gate_Graph_Default_Value;							//Gate_Graph_Default_Value;
									Init_Calc_ARCSS();
									Update_Diagnosis_Screen_Values();

									if(Flag_BLE_Enable != 0)
									{
										if(Flag_BLE_Value_Received == 0)
										{
											BLE_Info_Config_Register_Changed(Alt_MasterMarkPosition,Config_Parameters[Alt_MasterMarkPosition]);
										}
									}

									Stage_AutoSearch = 1;
									Config_Parameters[Alt_Index_mm] = 0;
									Flag_Index = 1;
									Flag_Index1 = 0;

									BLE_Coil_Write_Success = 1;
								}
								break;

    		case LCD_Btn_MarkType :
								if((Flag_TCP_Connected != 0) && (Flag_Use_With_PC != 0) && (Config16_Local_Permission[Alt_MarkType] == 0))
									break;

    							if(Flag_Auto_Any == 0)
								{
									Config_Parameters[Alt_MarkType] = 1 - Config_Parameters[Alt_MarkType];
									Transmit_Config_Parameters_Single(Alt_MarkType,Config_Parameters[Alt_MarkType]);
									Flash_Write_Config_Parameters_Single(Alt_MarkType);

									if(Flag_BLE_Enable != 0)
									{
										if(Flag_BLE_Value_Received == 0)
										{
											BLE_Info_Config_Register_Changed(Alt_MarkType,Config_Parameters[Alt_MarkType]);
										}
									}
									BLE_Coil_Write_Success = 1;
								}
								break;

    		case LCD_Btn_Encoder_Time_Mode :
								if((Flag_TCP_Connected != 0) && (Flag_Use_With_PC != 0) && (Config16_Local_Permission[Alt_Encoder_Time_Mode] == 0))
									break;

    							if((Flag_Auto_Any == 0) && (Flag_GateSet == 0))
								{
									Config_Parameters[Alt_Encoder_Time_Mode] = 1 - Config_Parameters[Alt_Encoder_Time_Mode];
									Transmit_Config_Parameters_Single(Alt_Encoder_Time_Mode,Config_Parameters[Alt_Encoder_Time_Mode]);
									Flash_Write_Config_Parameters_Single(Alt_Encoder_Time_Mode);

									if(Config_Parameters[Alt_Encoder_Time_Mode] == Time_Mode)
									{
										Speed_index = 0;
										Flag_Speed_Avg_Reset = 1;
									}

									if(Flag_BLE_Enable != 0)
									{
										if(Flag_BLE_Value_Received == 0)
										{
											BLE_Info_Config_Register_Changed(Alt_Encoder_Time_Mode,Config_Parameters[Alt_Encoder_Time_Mode]);
										}
									}
									BLE_Coil_Write_Success = 1;
								}
								break;

    		case LCD_Btn_Home_Mode :
								if((Flag_TCP_Connected != 0) && (Flag_Use_With_PC != 0) && (Config16_Local_Permission[Alt_Home_Mode] == 0))
									break;

    							if(Flag_Auto_Any == 0)
    							{
									Config_Parameters[Alt_Home_Mode] = 1 - Config_Parameters[Alt_Home_Mode];
									Transmit_Config_Parameters_Single(Alt_Home_Mode,Config_Parameters[Alt_Home_Mode]);
									Flash_Write_Config_Parameters_Single(Alt_Home_Mode);
									Flag_Home_Mode = Config_Parameters[Alt_Home_Mode];

									Flag_GateSet = 0;
									Position_Gate[Slave] = Gate_Graph_Default_Value;
									Init_Calc_ARCSS();

									Update_Diagnosis_Screen_Values();

									if(Flag_BLE_Enable != 0)
									{
										if(Flag_BLE_Value_Received == 0)
										{
											BLE_Info_Config_Register_Changed(Alt_Home_Mode,Config_Parameters[Alt_Home_Mode]);
										}
									}
									BLE_Coil_Write_Success = 1;
    							}
    							break;

			case LCD_Btn_Print_Mode :
								if((Flag_TCP_Connected != 0) && (Flag_Use_With_PC != 0) && (Config16_Local_Permission[Alt_Print_Mode] == 0))
									break;

								Config_Parameters[Alt_Print_Mode] = 1 - Config_Parameters[Alt_Print_Mode];
								Transmit_Config_Parameters_Single(Alt_Print_Mode,Config_Parameters[Alt_Print_Mode]);
								Flash_Write_Config_Parameters_Single(Alt_Print_Mode);
								Flag_Print_Mode = Config_Parameters[Alt_Print_Mode];

								if(Flag_Print_Mode == 0)
								{
									HAL_TIM_Base_Stop(&htim6);		//Stop ADC1 & ADC3 DMA for baseline
									__HAL_ADC_DISABLE_IT(&hadc1, ADC_IT_AWD1);
									ADC_H_Count = 0;
									Prev_Diff_BL_VBL = 0;
								}
								else
								{
									HAL_TIM_Base_Start(&htim6);		//Start ADC1 & ADC3 DMA for baseline
								}

								if(Flag_BLE_Enable != 0)
								{
									if(Flag_BLE_Value_Received == 0)
									{
										BLE_Info_Config_Register_Changed(Alt_Print_Mode,Config_Parameters[Alt_Print_Mode]);
									}
								}
								BLE_Coil_Write_Success = 1;
								break;

    		case LCD_Btn_Machine_Direction :
								if((Flag_TCP_Connected != 0) && (Flag_Use_With_PC != 0) && (Config16_Local_Permission[Alt_Machine_Direction] == 0))
									break;

    							if(Flag_Auto_Any == 0)
    							{
									Config_Parameters[Alt_Machine_Direction] = 1 - Config_Parameters[Alt_Machine_Direction];
									Transmit_Config_Parameters_Single(Alt_Machine_Direction,Config_Parameters[Alt_Machine_Direction]);
									Flash_Write_Config_Parameters_Single(Alt_Machine_Direction);

									if(Flag_BLE_Enable != 0)
									{
										if(Flag_BLE_Value_Received == 0)
										{
											BLE_Info_Config_Register_Changed(Alt_Machine_Direction,Config_Parameters[Alt_Machine_Direction]);
										}
									}
									BLE_Coil_Write_Success = 1;

									uint8_t temp_page_offset = (Config_Parameters[Alt_Correction_Direction_Horz] == Config_Parameters[Alt_Machine_Direction] ? 1 : 0);
									if(Screen_ID == Scr_Home)
									{
										Scr_Home = Page_Home + temp_page_offset;
										Screen_ID = Scr_Home;
										Transmit_Page_ID(Screen_ID);
									}
									else
									{
										Scr_Home = Page_Home + temp_page_offset;
									}
									Scr_Good_Copy = Page_Good_Copy + temp_page_offset;
    							}
								break;

//==========================================================================================================================

    		case LCD_Btn_VertF_Plus_Up :
    							if((Flag_Auto_Vertical != 0) && ((Config_Float[AltF_Corr_Offset_Vert] + Step_SizeF_offset) < Config_Float_Limits[AltF_Corr_Offset_Vert][1]))
    							{
									Config_Float[AltF_Corr_Offset_Vert] = Config_Float[AltF_Corr_Offset_Vert] + Step_SizeF_offset;
									Transmit_Config_Float(AltF_Corr_Offset_Vert,1);
									Flash_Write_Config_Float_Single(AltF_Corr_Offset_Vert);

									Config_Float[AltF_Disp_SetValue_Plus_OffSetV] = Config_Float[AltF_SetValue] + Config_Float[AltF_Corr_Offset_Vert];
									Transmit_Config_Float_Single(AltF_Disp_SetValue_Plus_OffSetV,Config_Float[AltF_Disp_SetValue_Plus_OffSetV]);

									if(Flag_BLE_Enable != 0)
									{
										if(Flag_BLE_Value_Received == 0)
										{
											BLE_Info_Config_Register_Float_Changed(AltF_Corr_Offset_Vert,Config_Float[AltF_Corr_Offset_Vert]);
										}
									}
									BLE_Coil_Write_Success = 1;
    							}
       							else if(Flag_Auto_Vertical == 0)
       							{
       								Flag_Start_Timer_Vertical = 1;
       								Digital_Correction_Vertical(Pulse_Upper,Config_Parameters[Alt_Corr_Min_Vert]);
       								Flag_Start_Timer_Vertical = 0;
       								BLE_Coil_Write_Success = 1;
       							}
								break;

    		case LCD_Btn_VertF_Minus_Dn :
       							if((Flag_Auto_Vertical != 0) && ((Config_Float[AltF_Corr_Offset_Vert] - Step_SizeF_offset) > Config_Float_Limits[AltF_Corr_Offset_Vert][0]))
    							{
    								Config_Float[AltF_Corr_Offset_Vert] = Config_Float[AltF_Corr_Offset_Vert] - Step_SizeF_offset;
        							Transmit_Config_Float(AltF_Corr_Offset_Vert,1);
        							Flash_Write_Config_Float_Single(AltF_Corr_Offset_Vert);

									Config_Float[AltF_Disp_SetValue_Plus_OffSetV] = Config_Float[AltF_SetValue] + Config_Float[AltF_Corr_Offset_Vert];
									Transmit_Config_Float_Single(AltF_Disp_SetValue_Plus_OffSetV,Config_Float[AltF_Disp_SetValue_Plus_OffSetV]);

									if(Flag_BLE_Enable != 0)
									{
										if(Flag_BLE_Value_Received == 0)
										{
											BLE_Info_Config_Register_Float_Changed(AltF_Corr_Offset_Vert,Config_Float[AltF_Corr_Offset_Vert]);
										}
									}
									BLE_Coil_Write_Success = 1;
    							}
       							else if(Flag_Auto_Vertical == 0)
       							{
       								Flag_Start_Timer_Vertical = 1;
     								Digital_Correction_Vertical(Pulse_Lower,Config_Parameters[Alt_Corr_Min_Vert]);
       								Flag_Start_Timer_Vertical = 0;
       								BLE_Coil_Write_Success = 1;
       							}
								break;

    		case LCD_Btn_HorzF_Plus_Right :
   								if((Flag_Auto_Horizontal != 0) && ((Config_Float[AltF_Corr_Offset_Horz] + Step_SizeF_offset) < Config_Float_Limits[AltF_Corr_Offset_Horz][1]))
								{
									Config_Float[AltF_Corr_Offset_Horz] = Config_Float[AltF_Corr_Offset_Horz] + Step_SizeF_offset;
									Transmit_Config_Float(AltF_Corr_Offset_Horz,1);
									Flash_Write_Config_Float_Single(AltF_Corr_Offset_Horz);

									if(Flag_BLE_Enable != 0)
									{
										if(Flag_BLE_Value_Received == 0)
										{
											BLE_Info_Config_Register_Float_Changed(AltF_Corr_Offset_Horz,Config_Float[AltF_Corr_Offset_Horz]);
										}
									}
									BLE_Coil_Write_Success = 1;
								}
								else if(Flag_Auto_Horizontal == 0)
								{
									Flag_Start_Timer_Horizontal = 1;
									Digital_Correction_Horizontal(Pulse_Upper,Config_Parameters[Alt_Corr_Min_Horz]);
									Flag_Start_Timer_Horizontal = 0;
									BLE_Coil_Write_Success = 1;
								}
								break;

    		case LCD_Btn_HorzF_Minus_Left :
								if((Flag_Auto_Horizontal != 0) && ((Config_Float[AltF_Corr_Offset_Horz] - Step_SizeF_offset) > Config_Float_Limits[AltF_Corr_Offset_Horz][0]))
    							{
									Config_Float[AltF_Corr_Offset_Horz] = Config_Float[AltF_Corr_Offset_Horz] - Step_SizeF_offset;
									Transmit_Config_Float(AltF_Corr_Offset_Horz,1);
									Flash_Write_Config_Float_Single(AltF_Corr_Offset_Horz);

									if(Flag_BLE_Enable != 0)
									{
										if(Flag_BLE_Value_Received == 0)
										{
											BLE_Info_Config_Register_Float_Changed(AltF_Corr_Offset_Horz,Config_Float[AltF_Corr_Offset_Horz]);
										}
									}
									BLE_Coil_Write_Success = 1;
    							}
								else if(Flag_Auto_Horizontal == 0)
								{
									Flag_Start_Timer_Horizontal = 1;
									Digital_Correction_Horizontal(Pulse_Lower,Config_Parameters[Alt_Corr_Min_Horz]);
									Flag_Start_Timer_Horizontal = 0;
									BLE_Coil_Write_Success = 1;
								}
								break;

//==========================================================================================================================

    		case LCD_Btn_VertC_Plus_Up :
       							if((Flag_Auto_Vertical != 0) && ((Config_Float[AltF_Corr_Offset_Vert] + Step_SizeC_offset) < Config_Float_Limits[AltF_Corr_Offset_Vert][1]))
    							{
									Config_Float[AltF_Corr_Offset_Vert] = Config_Float[AltF_Corr_Offset_Vert] + Step_SizeC_offset;
									Transmit_Config_Float(AltF_Corr_Offset_Vert,1);
									Flash_Write_Config_Float_Single(AltF_Corr_Offset_Vert);

									Config_Float[AltF_Disp_SetValue_Plus_OffSetV] = Config_Float[AltF_SetValue] + Config_Float[AltF_Corr_Offset_Vert];
									Transmit_Config_Float_Single(AltF_Disp_SetValue_Plus_OffSetV,Config_Float[AltF_Disp_SetValue_Plus_OffSetV]);

									if(Flag_BLE_Enable != 0)
									{
										if(Flag_BLE_Value_Received == 0)
										{
											BLE_Info_Config_Register_Float_Changed(AltF_Corr_Offset_Vert,Config_Float[AltF_Corr_Offset_Vert]);
										}
									}
									BLE_Coil_Write_Success = 1;
    							}
       							else if(Flag_Auto_Vertical == 0)
       							{
       								Flag_Start_Timer_Vertical = 1;
       								Digital_Correction_Vertical(Pulse_Upper,Config_Parameters[Alt_Corr_Min_Vert]);
       								Flag_Start_Timer_Vertical = 0;
       								BLE_Coil_Write_Success = 1;
       							}
								break;

    		case LCD_Btn_VertC_Minus_Dn :
    							if((Flag_Auto_Vertical != 0) && ((Config_Float[AltF_Corr_Offset_Vert] - Step_SizeC_offset) > Config_Float_Limits[AltF_Corr_Offset_Vert][0]))
    							{
									Config_Float[AltF_Corr_Offset_Vert] = Config_Float[AltF_Corr_Offset_Vert] - Step_SizeC_offset;
									Transmit_Config_Float(AltF_Corr_Offset_Vert,1);
									Flash_Write_Config_Float_Single(AltF_Corr_Offset_Vert);

									Config_Float[AltF_Disp_SetValue_Plus_OffSetV] = Config_Float[AltF_SetValue] + Config_Float[AltF_Corr_Offset_Vert];
									Transmit_Config_Float_Single(AltF_Disp_SetValue_Plus_OffSetV,Config_Float[AltF_Disp_SetValue_Plus_OffSetV]);

									if(Flag_BLE_Enable != 0)
									{
										if(Flag_BLE_Value_Received == 0)
										{
											BLE_Info_Config_Register_Float_Changed(AltF_Corr_Offset_Vert,Config_Float[AltF_Corr_Offset_Vert]);
										}
									}
									BLE_Coil_Write_Success = 1;
    							}
       							else if(Flag_Auto_Vertical == 0)
       							{
       								Flag_Start_Timer_Vertical = 1;
     								Digital_Correction_Vertical(Pulse_Lower,Config_Parameters[Alt_Corr_Min_Vert]);
       								Flag_Start_Timer_Vertical = 0;
       								BLE_Coil_Write_Success = 1;
       							}
								break;

    		case LCD_Btn_HorzC_Plus_Right :
								if((Flag_Auto_Horizontal != 0) && ((Config_Float[AltF_Corr_Offset_Horz] + Step_SizeC_offset) < Config_Float_Limits[AltF_Corr_Offset_Horz][1]))
    							{
									Config_Float[AltF_Corr_Offset_Horz] = Config_Float[AltF_Corr_Offset_Horz] + Step_SizeC_offset;
									Transmit_Config_Float(AltF_Corr_Offset_Horz,1);
									Flash_Write_Config_Float_Single(AltF_Corr_Offset_Horz);

									if(Flag_BLE_Enable != 0)
									{
										if(Flag_BLE_Value_Received == 0)
										{
											BLE_Info_Config_Register_Float_Changed(AltF_Corr_Offset_Horz,Config_Float[AltF_Corr_Offset_Horz]);
										}
									}
									BLE_Coil_Write_Success = 1;
    							}
								else if(Flag_Auto_Horizontal == 0)
								{
									Flag_Start_Timer_Horizontal = 1;
									Digital_Correction_Horizontal(Pulse_Upper,Config_Parameters[Alt_Corr_Min_Horz]);
									Flag_Start_Timer_Horizontal = 0;
									BLE_Coil_Write_Success = 1;
								}
								break;

    		case LCD_Btn_HorzC_Minus_Left :
   								if((Flag_Auto_Horizontal != 0) && ((Config_Float[AltF_Corr_Offset_Horz] - Step_SizeC_offset) > Config_Float_Limits[AltF_Corr_Offset_Horz][0]))
    							{
									Config_Float[AltF_Corr_Offset_Horz] = Config_Float[AltF_Corr_Offset_Horz] - Step_SizeC_offset;
									Transmit_Config_Float(AltF_Corr_Offset_Horz,1);
									Flash_Write_Config_Float_Single(AltF_Corr_Offset_Horz);

									if(Flag_BLE_Enable != 0)
									{
										if(Flag_BLE_Value_Received == 0)
										{
											BLE_Info_Config_Register_Float_Changed(AltF_Corr_Offset_Horz,Config_Float[AltF_Corr_Offset_Horz]);
										}
									}
									BLE_Coil_Write_Success = 1;
    							}
								else if(Flag_Auto_Horizontal == 0)
								{
									Flag_Start_Timer_Horizontal = 1;
									Digital_Correction_Horizontal(Pulse_Lower,Config_Parameters[Alt_Corr_Min_Horz]);
									Flag_Start_Timer_Horizontal = 0;
									BLE_Coil_Write_Success = 1;
								}
								break;

//===============================================================================================

    		case LCD_Btn_AutoSearch:
								if((Flag_Home_Mode != 0) || (Flag_GateSet != 0) || (Flag_BaseLine_Error != 0) || (Stage_AutoSearch != 0))
									break;

								Stage_AutoSearch = 1;
//    								--------------------------------------------------
//    								AUTO SEARCH PROCESS
//    								-	Auto Search Click - Stage = 1
//    								-	First Z - Start a Z to Z Cycle
//    								-	BL Int - Start to find the first edge and AutoScan_PPR - stage = 2
//    								-	BL Int - Modify Index, Start a cycle for Circum + Index - stage = 3
//    								-	BL Int - Start a cycle for Circum - So this cycle will prepare graph to show - stage = 4
//    								-	BL Int - Now graph can be shown - stage = 0
//    								--------------------------------------------------

								Config_Parameters[Alt_Index_mm] = 0;
								Flag_Index = 1;
								Flag_Index1 = 0;
								BLE_Coil_Write_Success = 1;
								break;

    		case LCD_Btn_Serial_Debug_Enable:
    							if((Flag_TCP_Connected != 0) && (Flag_Use_With_PC != 0))
    							{
    								break;
    							}

								Flag_Serial_Debug_Enable = 1 - Flag_Serial_Debug_Enable;
								Transmit_Config_Parameters_Single(Serial_Debug_Enable_Location,Flag_Serial_Debug_Enable);
								break;

    		case LCD_Btn_Good_Copy_Horz_TCP :
    							if((Flag_GateSet != 0) && (Flag_Auto_Horizontal == 0) && (Perform_Good_Copy_Horz_Stage == 0))
    							{
    								Perform_Good_Copy_Horz_Stage = 1;
    								BLE_Coil_Write_Success = 1;
    							}
    							break;

    		case LCD_Btn_Good_Copy_Vert_TCP :
    							if((Flag_GateSet != 0) && (Flag_Auto_Vertical == 0) && (Perform_Good_Copy_Vert_Stage == 0))
    							{
    								Perform_Good_Copy_Vert_Stage = 1;
    								BLE_Coil_Write_Success = 1;
    							}
    							break;

    		case LCD_Btn_Good_Copy_Horz :
    							if((Flag_GateSet != 0) && (Flag_Auto_Horizontal == 0) && (Perform_Good_Copy_Horz_Stage == 0))
    							{
    								Flag_Perform_Good_Copy = Horizontal;
    								Transmit_Page_ID(Scr_Good_Copy);
    							}
    							break;

    		case LCD_Btn_Good_Copy_Vert:
								if((Flag_GateSet != 0) && (Flag_Auto_Vertical == 0) && (Perform_Good_Copy_Vert_Stage == 0))
								{
    								Flag_Perform_Good_Copy = Vertical;
    								Transmit_Page_ID(Scr_Good_Copy);
								}
								break;

    		case LCD_Btn_Confirm_Good_Copy :
    							if(Flag_Perform_Good_Copy == Horizontal)
    							{
    								if((Flag_GateSet != 0) && (Flag_Auto_Horizontal == 0) && (Perform_Good_Copy_Horz_Stage == 0))
									{
    									Perform_Good_Copy_Horz_Stage = 1;
									}
    							}
    							else
    							{
    								if((Flag_GateSet != 0) && (Flag_Auto_Vertical == 0) && (Perform_Good_Copy_Vert_Stage == 0))
    								{
    									Perform_Good_Copy_Vert_Stage = 1;
    								}
    							}
    							break;

    		case LCD_Btn_Use_With_PC :
								Config_Parameters[Alt_Use_With_PC] = 1 - Config_Parameters[Alt_Use_With_PC];
								Transmit_Config_Parameters_Single(Alt_Use_With_PC,Config_Parameters[Alt_Use_With_PC]);
								Flash_Write_Config_Parameters_Single(Alt_Use_With_PC);
								Flag_Use_With_PC = Config_Parameters[Alt_Use_With_PC];

								if(Flag_BLE_Enable != 0)
								{
									if(Flag_BLE_Value_Received == 0)
									{
										BLE_Info_Config_Register_Changed(Alt_Use_With_PC,Config_Parameters[Alt_Use_With_PC]);
									}
								}
								BLE_Coil_Write_Success = 1;

								if(Flag_Use_With_PC == 0)
								{
									Flag_Permit_Actions = 1;
									Flag_First_Time_TCP_Request_Done = 0;
								}
								else if((Flag_TCP_Connected == 0) || (Flag_Job_Name_Received == 0))
								{
									Flag_Permit_Actions = 0;		//Will wait for Job Name
								}
								Transmit_Config_Parameters_Single(LCD_Flag_Permit_Action_Location,Flag_Permit_Actions);
    							break;

    		default				:
								break;
    	}
    }
}

void Check_For_LCD_Touch_Command_Float(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 16;

	unsigned char lcd_Component_id;
	_Bool Flag_temp_limit = 0;
	_Bool Flag_Do_Not_Save = 0;
	float temp_rec_float;

	uint8_t temp_error_code = 0;

	if(Flag_BLE_Value_Received == 0)
	{
		lcd_Component_id = lcd_received_bytes[3]/4;		//Config_Parameters index number		//buff[5]
		Float_Disp.dc[3] = lcd_received_bytes[4];												//buff[6]
		Float_Disp.dc[2] = lcd_received_bytes[5];
		Float_Disp.dc[1] = lcd_received_bytes[6];
		Float_Disp.dc[0] = lcd_received_bytes[7];
		temp_rec_float = Float_Disp.df;
	}
	else
	{
		lcd_Component_id 	= Temp_BLE_Reg_Address;
		temp_rec_float 		= Temp_BLE_Reg_Data_Float;
	}

	_Bool Local_Value_Change_Permission = 1;
	if((Flag_TCP_Connected != 0) && (Flag_Use_With_PC != 0) && (Flag_BLE_Value_Received == 0))	//TCP connected, value change from lcd
	{
		if(Config32_Local_Permission[lcd_Component_id] == 0)		//Not Permitted Locally
		{
			Local_Value_Change_Permission = 0;
		}
	}

	if(Local_Value_Change_Permission != 0)
	{
		Flag_temp_limit = Check_Config_Float_Limits_Single(lcd_Component_id,temp_rec_float);
		if(Flag_temp_limit == 0)
		{
			temp_error_code = BLE_Error_Code_Data_Not_Valid;
		}
		else if(temp_rec_float == Config_Float[lcd_Component_id])
		{
			Flag_temp_limit = 0;
			temp_error_code = BLE_Error_Code_Same_Data_No_Action;
		}
	}

	if((lcd_Component_id < No_Of_Config_Float) && (Flag_temp_limit != 0))
	{
		switch(lcd_Component_id)
		{
			case AltF_Corr_Min_Error_Horz :
									if(temp_rec_float < Config_Float[AltF_Corr_Max_Error_Horz])
									{
										Config_Float[lcd_Component_id] = temp_rec_float;
										Min_Error_PPR[Horizontal] = Convert_mm_To_PPR * temp_rec_float;
									}
									else
									{
										Flag_temp_limit = 0;
										temp_error_code = BLE_Error_Code_Not_Processed;
									}
									break;

			case AltF_Corr_Min_Error_Vert :
									if(temp_rec_float < Config_Float[AltF_Corr_Max_Error_Vert])
									{
										Config_Float[lcd_Component_id] = temp_rec_float;
										Min_Error_PPR[Vertical] = Convert_mm_To_PPR * temp_rec_float;
									}
									else
									{
										Flag_temp_limit = 0;
										temp_error_code = BLE_Error_Code_Not_Processed;
									}
									break;

			case AltF_Corr_Max_Error_Horz :
									if(temp_rec_float > Config_Float[AltF_Corr_Min_Error_Horz])
									{
										Config_Float[lcd_Component_id] = temp_rec_float;
										Max_Error_PPR[Horizontal] = Convert_mm_To_PPR * temp_rec_float;
									}
									else
									{
										Flag_temp_limit = 0;
										temp_error_code = BLE_Error_Code_Not_Processed;
									}
									break;

			case AltF_Corr_Max_Error_Vert :
									if(temp_rec_float > Config_Float[AltF_Corr_Min_Error_Vert])
									{
										Config_Float[lcd_Component_id] = temp_rec_float;
										Max_Error_PPR[Vertical] = Convert_mm_To_PPR * temp_rec_float;
									}
									else
									{
										Flag_temp_limit = 0;
										temp_error_code = BLE_Error_Code_Not_Processed;
									}
									break;

			case AltF_SetValue :
									if((Flag_Auto_Any == 0) && (temp_rec_float >= Config_Parameters[Alt_GateWidth]) && (temp_rec_float <= Config_Parameters[Alt_BagLength]))
									{
										Config_Float[lcd_Component_id] = temp_rec_float;
										Flag_GateSet = 0;

										Init_Calc_ARCSS();
										Update_Diagnosis_Screen_Values();

										AutoSearch_Setmm = (Config_Parameters[Alt_BagLength] - (8 * Config_Float[AltF_SetValue])) / 2;
									}
									else
									{
										Flag_temp_limit = 0;
										temp_error_code = BLE_Error_Code_Not_Processed;
									}
									break;
//RECIPE PARAMETERS START
			case AltF_Corr_Offset_Vert	:
									Config_Float[lcd_Component_id] = temp_rec_float;
									Config_Float[AltF_Disp_SetValue_Plus_OffSetV] = Config_Float[AltF_SetValue] + Config_Float[AltF_Corr_Offset_Vert];
									Transmit_Config_Float_Single(AltF_Disp_SetValue_Plus_OffSetV,Config_Float[AltF_Disp_SetValue_Plus_OffSetV]);
									break;
			case AltF_Corr_Offset_Horz	:

			case AltF_Error_Alarm_Horz	:
			case AltF_Error_Alarm_Vert	:

			case AltF_Corr_PGain_Horz	:
			case AltF_Corr_PGain_Vert	:

			case AltF_Corr_DGain_Horz	:
			case AltF_Corr_DGain_Vert	:

			case AltF_Error_Filter_Horz	:
			case AltF_Error_Filter_Vert	:

			case AltF_Error_DGain_Horz :
			case AltF_Error_DGain_Vert :

			case AltF_BaseLine_Error_Alarm_Limit :
			case AltF_Min_Control_Speed :
										Config_Float[lcd_Component_id] = temp_rec_float;
										break;
//RECIPE PARAMETERS END
			default						:
										break;
		}
	}

	if(Flag_temp_limit == 0)				//Value not OK
	{
		if(Flag_BLE_Value_Received == 0)	//Value received from LCD
		{
			//Value out of range, send current value
			Transmit_Config_Float_Single(lcd_Component_id,Config_Float[lcd_Component_id]);
		}
		else	//Value is from BLE and not OK, then overwrite that value on BLE screen with current value
		{
			BLE_Send_Error_Response_Write_Float(add_type_config, lcd_Component_id, temp_error_code);
			//if value is from BLE and not valid value, do not require to send value to LCD
		}

	}
	else	//Value OK
	{
		if(Flag_Do_Not_Save == 0)	//Parameter other then recipe
		{
			Flash_Write_Config_Float_Single(lcd_Component_id);
		}

		if(Flag_BLE_Value_Received != 0)	//Value is from BLE, send data to LCD also
		{
			//if value is from BLE and a valid value, send value to LCD
			Transmit_Config_Float_Single(lcd_Component_id,Config_Float[lcd_Component_id]);

			//Send Success Response to BLE
			BLE_Response_Write_Single_Config_Register_Float(Config_Float[lcd_Component_id]);

			osDelay(2);
		}
		else if(Flag_BLE_Enable != 0)	//Value is from LCD, and BLE is Enabled
		{
			//Inform about modified value to BLE also,if it is connected and that parameter is available at BLE application
			if(BLE_HoldReg_Float_Index[lcd_Component_id] != BLE_Not_Accessible)
			{
				BLE_Info_Config_Register_Float_Changed(lcd_Component_id,temp_rec_float);
			}
		}
	}
}

void Check_For_LCD_Touch_Command_Int(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 17;

	unsigned char lcd_temp_h,lcd_temp_l,lcd_Component_id;
	_Bool Flag_temp_limit = 0;

	int16_t temp_rec_value;		//uint16 to int16

	_Bool Flag_Do_Not_Save = 0;

	uint8_t temp_error_code = 0;

	if(Flag_BLE_Value_Received == 0)
	{
		lcd_Component_id = lcd_received_bytes[3]/2;		//Config_Parameters index number		//buff[5]
		lcd_temp_h = lcd_received_bytes[4];														//buff[6]
		lcd_temp_l = lcd_received_bytes[5];														//buff[7]

		temp_rec_value = lcd_temp_h;
		temp_rec_value = (temp_rec_value << 8) + lcd_temp_l;
	}
	else
	{
		lcd_Component_id 	= Temp_BLE_Reg_Address;
		temp_rec_value 		= Temp_BLE_Reg_Data;
	}

	_Bool Local_Value_Change_Permission = 1;
	if((Flag_TCP_Connected != 0) && (Flag_Use_With_PC != 0) && (Flag_BLE_Value_Received == 0))	//TCP connected, value change from lcd
	{
		if(Config16_Local_Permission[lcd_Component_id] == 0)	//Not Permitted Locally
		{
			Local_Value_Change_Permission = 0;
		}
	}

	if(Local_Value_Change_Permission != 0)		//From TCP Or if from LCD, permission is available
	{
		Flag_temp_limit = Check_Config_Para_Limits_Single(lcd_Component_id,temp_rec_value);
		if(Flag_temp_limit == 0)
		{
			temp_error_code = BLE_Error_Code_Data_Not_Valid;
		}
		else if(temp_rec_value == Config_Parameters[lcd_Component_id])
		{
			Flag_temp_limit = 0;
			temp_error_code = BLE_Error_Code_Same_Data_No_Action;
		}
	}

	if((lcd_Component_id < No_Of_Config_Parameters) && (Flag_temp_limit != 0))
	{
		switch(lcd_Component_id)
		{
			case Alt_Temp_Password:
								if(Flag_Spl_Rights == 0)
								{
									check_password(temp_rec_value);
								}
								Flag_Do_Not_Save = 1;
								Config_Parameters[Alt_Temp_Password] = 32723;
								break;

			case Alt_BagLength	:
								if(Flag_Auto_Any == 0)
								{
									Position_Gate[Slave] = Gate_Graph_Default_Value;

									Config_Parameters[lcd_Component_id] = temp_rec_value;
									Flag_GateSet = 0;

									Init_Calc_ARCSS();

									Min_Mark_Height_Count = Config_Parameters[Alt_MinMarkHeight] * Convert_mm_To_PPR;		//Only for ARC
									Max_Mark_Height_Count = Config_Parameters[Alt_MaxMarkHeight] * Convert_mm_To_PPR;		//Only for ARC

									Update_Diagnosis_Screen_Values();

									Config_Parameters[Alt_Index_mm] = 0;
									Transmit_Config_Parameters_Single(Alt_Index_mm,Config_Parameters[Alt_Index_mm]);
									Flash_Write_Config_Parameters_Single(Alt_Index_mm);

									if(Flag_BLE_Enable != 0)
									{
										BLE_Info_Config_Register_Changed(Alt_Index_mm,Config_Parameters[Alt_Index_mm]);
									}

									osDelay(10);

									//Being Used for Speed Calculation at Baglength Interrupt
									temp_speed_multiplier = 60000;
									temp_speed_multiplier = temp_speed_multiplier * Config_Parameters[Alt_BagLength];

									AutoSearch_Setmm = ( temp_rec_value - (8 * Config_Float[AltF_SetValue]) ) / 2;
									Clear_Buffer();
									Flag_BagLength = 1;
								}
								else
								{
									Flag_temp_limit = 0;
									temp_error_code = BLE_Error_Code_Not_Processed;
								}
								break;

			case Alt_GateWidth	:
								if((Flag_Auto_Any == 0) && (temp_rec_value <= Config_Float[AltF_SetValue]))
								{
									Config_Parameters[lcd_Component_id] = temp_rec_value;
									Flag_GateSet = 0;

									Init_Calc_ARCSS();
									Update_Diagnosis_Screen_Values();
								}
								else
								{
									Flag_temp_limit = 0;
									temp_error_code = BLE_Error_Code_Not_Processed;
								}
								break;

			case Alt_Index_mm	:
								if((Flag_Auto_Any == 0) && (temp_rec_value < Config_Parameters[Alt_BagLength]))
								{
									Config_Parameters[lcd_Component_id] = temp_rec_value;
									Flag_GateSet = 0;

									Stage_AutoSearch = 0;
									Flag_Index = 1;
									Flag_Index1 = 0;
								}
								else
								{
									Flag_temp_limit = 0;
									temp_error_code = BLE_Error_Code_Not_Processed;
								}
								break;

			case Alt_Station_ID	:
								if((Flag_Auto_Any == 0) && (temp_rec_value > Config_Parameters[Alt_Master_ID_Horz]) && (temp_rec_value > Config_Parameters[Alt_Master_ID_Vert]))
								{
									Position_Gate[Slave] = Gate_Graph_Default_Value;

									Config_Parameters[lcd_Component_id] = temp_rec_value;
									Flag_GateSet = 0;
									Init_Calc_ARCSS();
									Update_Diagnosis_Screen_Values();
								}
								else
								{
									Flag_temp_limit = 0;
									temp_error_code = BLE_Error_Code_Not_Processed;
								}
								break;

			case Alt_Station_Colour	:
								Config_Parameters[lcd_Component_id] = temp_rec_value;
								LCD_Draw_Colour_Box();
								break;

			case Alt_Time_Average :
								Config_Parameters[lcd_Component_id] = temp_rec_value;
								Speed_index = 0;
								Flag_Speed_Avg_Reset = 1;
								break;

			case Alt_Master_ID_Horz	:
			case Alt_Master_ID_Vert	:
								if((Flag_Auto_Any == 0) && (temp_rec_value < Config_Parameters[Alt_Station_ID]))
								{
									Config_Parameters[lcd_Component_id] = temp_rec_value;
									Flag_GateSet = 0;

									Init_Calc_ARCSS();
									Update_Diagnosis_Screen_Values();
								}
								else
								{
									Flag_temp_limit = 0;
									temp_error_code = BLE_Error_Code_Not_Processed;
								}
								break;

//RECIPE PARAMETERS START
			case Alt_Corr_Repeat_Horz	:
								Config_Parameters[lcd_Component_id] = temp_rec_value;
								break;

			case Alt_Corr_Repeat_Vert	:
								Config_Parameters[lcd_Component_id] = temp_rec_value;
								break;

			case Alt_Corr_Max_Horz 		:
								if(temp_rec_value > Config_Parameters[Alt_Corr_Min_Horz])
								{
									Config_Parameters[lcd_Component_id] = temp_rec_value;
								}
								else
								{
									Flag_temp_limit = 0;
									temp_error_code = BLE_Error_Code_Not_Processed;
								}
								break;

			case Alt_Corr_Max_Vert 		:
								if(temp_rec_value > Config_Parameters[Alt_Corr_Min_Vert])
								{
									Config_Parameters[lcd_Component_id] = temp_rec_value;
								}
								else
								{
									Flag_temp_limit = 0;
									temp_error_code = BLE_Error_Code_Not_Processed;
								}
								break;

			case Alt_Corr_Min_Horz 		:
								if(temp_rec_value < Config_Parameters[Alt_Corr_Max_Horz])
								{
									Config_Parameters[lcd_Component_id] = temp_rec_value;
								}
								else
								{
									Flag_temp_limit = 0;
									temp_error_code = BLE_Error_Code_Not_Processed;
								}
								break;

			case Alt_Corr_Min_Vert 		:
								if(temp_rec_value < Config_Parameters[Alt_Corr_Max_Vert])
								{
									Config_Parameters[lcd_Component_id] = temp_rec_value;
								}
								else
								{
									Flag_temp_limit = 0;
									temp_error_code = BLE_Error_Code_Not_Processed;
								}
								break;

//=====================================================================================
			case Alt_MinMarkHeight :
								if(temp_rec_value < Config_Parameters[Alt_MaxMarkHeight])
								{
									Config_Parameters[lcd_Component_id] = temp_rec_value;
									Min_Mark_Height_Count = Config_Parameters[Alt_MinMarkHeight] * Convert_mm_To_PPR;
									Flag_GateSet = 0;
								}
								else
								{
									Flag_temp_limit = 0;
									temp_error_code = BLE_Error_Code_Not_Processed;
								}
								break;

			case Alt_MaxMarkHeight 		:
								if(temp_rec_value > Config_Parameters[Alt_MinMarkHeight])
								{
									Config_Parameters[lcd_Component_id] = temp_rec_value;
									Max_Mark_Height_Count = Config_Parameters[Alt_MaxMarkHeight] * Convert_mm_To_PPR;		//Only for ARC
									Flag_GateSet = 0;
								}
								else
								{
									Flag_temp_limit = 0;
									temp_error_code = BLE_Error_Code_Not_Processed;
								}
								break;

			case Alt_ValidBaseLine :
								if(Flag_Auto_Any == 0)
								{
									if(temp_rec_value > Config_Parameters[Alt_Valid_BL_Window_Lower])
									{
										Config_Parameters[lcd_Component_id] = temp_rec_value;
										Flag_GateSet = 0;

										Permit_Set_Threshold_Limit_Count = Permit_Set_Threshold_Limit_Perc;
										Permit_Set_Threshold_Limit_Count = Permit_Set_Threshold_Limit_Count * Config_Parameters[Alt_ValidBaseLine] / 100;
									}
									else
									{
										Flag_temp_limit = 0;
										temp_error_code = BLE_Error_Code_Not_Processed;
									}
								}
								else
								{
									Flag_temp_limit = 0;
									temp_error_code = BLE_Error_Code_Not_Processed;
								}

								break;

			case Alt_Sensor_Gain :
								Config_Parameters[lcd_Component_id] = temp_rec_value;
								HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, temp_rec_value);
								break;

			case Alt_Valid_BL_Window_Lower :
								if(Flag_Auto_Any == 0)
								{
									if(temp_rec_value < Config_Parameters[Alt_ValidBaseLine])
									{
										Config_Parameters[lcd_Component_id] = temp_rec_value;
										Flag_GateSet = 0;
									}
									else
									{
										Flag_temp_limit = 0;
										temp_error_code = BLE_Error_Code_Not_Processed;
									}
								}
								else
								{
									Flag_temp_limit = 0;
									temp_error_code = BLE_Error_Code_Not_Processed;
								}
								break;

//BOOLEAN PARAMETER VALUES, WILL BE RECEIVED FROM BLE ONLY
			case Alt_Auto_Averaging_Horz :
								Config_Parameters[lcd_Component_id] = temp_rec_value;
								break;

			case Alt_Auto_Averaging_Vert :
								Config_Parameters[lcd_Component_id] = temp_rec_value;
								break;

			case Alt_Correction_Direction_Horz :
								if(Flag_Auto_Horizontal == 0)
								{
									Config_Parameters[lcd_Component_id] = temp_rec_value;

									uint8_t temp_page_offset = (Config_Parameters[Alt_Correction_Direction_Horz] == Config_Parameters[Alt_Machine_Direction] ? 1 : 0);
									if(Screen_ID == Scr_Home)
									{
										Scr_Home = Page_Home + temp_page_offset;
										Screen_ID = Scr_Home;
										Transmit_Page_ID(Screen_ID);
									}
									else
									{
										Scr_Home = Page_Home + temp_page_offset;
									}
									Scr_Good_Copy = Page_Good_Copy + temp_page_offset;
								}
								else
								{
									Flag_temp_limit = 0;
									temp_error_code = BLE_Error_Code_Not_Processed;
								}
								break;

			case Alt_Correction_Direction_Vert :
								if(Flag_Auto_Vertical == 0)
								{
									Config_Parameters[lcd_Component_id] = temp_rec_value;
								}
								else
								{
									Flag_temp_limit = 0;
									temp_error_code = BLE_Error_Code_Not_Processed;
								}
								break;

			case Alt_Home_Mode :
								if(Flag_Auto_Any == 0)
								{
									Position_Gate[Slave] = Gate_Graph_Default_Value;

									Config_Parameters[lcd_Component_id] = temp_rec_value;
									Flag_Home_Mode = temp_rec_value;

									Flag_GateSet = 0;

									Init_Calc_ARCSS();
									Update_Diagnosis_Screen_Values();
								}
								else
								{
									Flag_temp_limit = 0;
									temp_error_code = BLE_Error_Code_Not_Processed;
								}
								break;

			case Alt_Print_Mode :
								Config_Parameters[lcd_Component_id] = temp_rec_value;
								Flag_Print_Mode = temp_rec_value;

								if(Flag_Print_Mode == 0)
								{
									HAL_TIM_Base_Stop(&htim6);		//Stop ADC1 & ADC3 DMA for baseline
									__HAL_ADC_DISABLE_IT(&hadc1, ADC_IT_AWD1);
									ADC_H_Count = 0;
									Prev_Diff_BL_VBL = 0;
								}
								else
								{
									HAL_TIM_Base_Start(&htim6);		//Start ADC1 & ADC3 DMA for baseline
								}
								break;

			case Alt_Encoder_Time_Mode :
								if((Flag_Auto_Any == 0) && (Flag_GateSet == 0))
								{
									Config_Parameters[lcd_Component_id] = temp_rec_value;

									if(temp_rec_value == Time_Mode)
									{
										Speed_index = 0;
										Flag_Speed_Avg_Reset = 1;
									}
								}
								else
								{
									Flag_temp_limit = 0;
									temp_error_code = BLE_Error_Code_Not_Processed;
								}
								break;

			case Alt_MasterMarkPosition :
								if((Flag_Home_Mode == 0) && (Flag_Auto_Any == 0))
								{
									Position_Gate[Slave] = Gate_Graph_Default_Value;		//Gate_Graph_Default_Value;

									Config_Parameters[lcd_Component_id] = temp_rec_value;
									Flag_GateSet = 0;

									Init_Calc_ARCSS();
									Update_Diagnosis_Screen_Values();

									Stage_AutoSearch = 1;
									Config_Parameters[Alt_Index_mm] = 0;
									Flag_Index = 1;
									Flag_Index1 = 0;
								}
								else
								{
									Flag_temp_limit = 0;
									temp_error_code = BLE_Error_Code_Not_Processed;
								}
								break;

			case Alt_MarkType :
								if(Flag_Auto_Any == 0)
								{
									Config_Parameters[lcd_Component_id] = temp_rec_value;
								}
								else
								{
									Flag_temp_limit = 0;
									temp_error_code = BLE_Error_Code_Not_Processed;
								}
								break;


			case Alt_Machine_Direction :
								if(Flag_Auto_Any == 0)
								{
									Config_Parameters[lcd_Component_id] = temp_rec_value;

									uint8_t temp_page_offset = (Config_Parameters[Alt_Correction_Direction_Horz] == Config_Parameters[Alt_Machine_Direction] ? 1 : 0);
									if(Screen_ID == Scr_Home)
									{
										Scr_Home = Page_Home + temp_page_offset;
										Screen_ID = Scr_Home;
										Transmit_Page_ID(Screen_ID);
									}
									else
									{
										Scr_Home = Page_Home + temp_page_offset;
									}
									Scr_Good_Copy = Page_Good_Copy + temp_page_offset;
								}
								else
								{
									Flag_temp_limit = 0;
									temp_error_code = BLE_Error_Code_Not_Processed;
								}
								break;

    		case Alt_Use_With_PC :
								Config_Parameters[Alt_Use_With_PC] = temp_rec_value;
								Flag_Use_With_PC = temp_rec_value;

								if(Flag_Use_With_PC == 0)
								{
									Flag_Permit_Actions = 1;
									Flag_First_Time_TCP_Request_Done = 0;
								}
								else if((Flag_TCP_Connected == 0) || (Flag_Job_Name_Received == 0))
								{
									Flag_Permit_Actions = 0;		//Will wait for Job Name
								}
    							break;

//=====================================================================================
			default				:
								Config_Parameters[lcd_Component_id] = temp_rec_value;
								break;
		}
	}

	if(Flag_temp_limit == 0)				//Value not OK
	{
		if(Flag_BLE_Value_Received == 0)	//Value received from LCD
		{
			//Value out of range, send current value
			Transmit_Config_Parameters_Single(lcd_Component_id,Config_Parameters[lcd_Component_id]);
		}
		else	//Value is from BLE and not OK, then overwrite that value on BLE screen with current value
		{
			BLE_Send_Error_Response_Write_Int(add_type_config, lcd_Component_id, temp_error_code);
			//if value is from BLE and not valid value, do not require to send value to LCD
		}
	}
	else	//Value OK
	{
		if(Flag_Do_Not_Save == 0)	//Parameter other then recipe
		{
			Flash_Write_Config_Parameters_Single(lcd_Component_id);
		}

		if(Flag_BLE_Value_Received != 0)	//Value is from BLE, send data to LCD also
		{
			//if value is from BLE and a valid value, send value to LCD
			Transmit_Config_Parameters_Single(lcd_Component_id,Config_Parameters[lcd_Component_id]);

			//Send Success Response to BLE
			BLE_Response_Write_Single_Config_Reg(Config_Parameters[lcd_Component_id]);

			osDelay(2);
		}
		else if(Flag_BLE_Enable != 0)	//Value is from LCD, and BLE is Enabled
		{
			//Inform about modified value to BLE also,if it is connected and that parameter is available at BLE application
			if(BLE_HoldReg_Index[lcd_Component_id] != BLE_Not_Accessible)
			{
				BLE_Info_Config_Register_Changed(lcd_Component_id,Config_Parameters[lcd_Component_id]);
			}
		}
	}
}

void Update_Diagnosis_Screen_Values(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 18;

	Config_Float[AltF_Disp_SetValue_Horz] = Distance_From_Slave_mm[Horizontal];
	Config_Float[AltF_Disp_SetValue_Vert] = Distance_From_Slave_mm[Vertical];
	Config_Float[AltF_Disp_SetValue_Plus_OffSetV] = Config_Float[AltF_SetValue] + Config_Float[AltF_Corr_Offset_Vert];

	BLE_Display_Float[BLE_DispF_SetValue_Horz] = Distance_From_Slave_mm[Horizontal];
	BLE_Display_Float[BLE_DispF_SetValue_Vert] = Distance_From_Slave_mm[Vertical];

	Transmit_Config_Float(AltF_Disp_SetValue_Horz,3);
	Transmit_Config_Parameters_Single(ARC_Gate_Slave_Min_Valid_Value_Location,Gate_Graph_Min_Valid_Value_Slave_Count);
	Transmit_Config_Parameters_Single(ARC_Gate_Slave_Max_Valid_Value_Location,Gate_Graph_Max_Valid_Value_Slave_Count);
}

void Check_For_LCD_Touch_Command(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
		arc_execution_func_prev_id = arc_execution_func_cur_id;
		arc_execution_func_cur_id = 19;
        if(Flag_ReceivedValue != 0)
        {
            if(lcd_received_cmd == LCD_Key_Pressed)
            {
           		Check_For_LCD_Touch_Command_Touch_Buttons();
            }
            else if(lcd_received_cmd == LCD_Key_Up)				//Key Release in case of Fwd and Rev
            {
                Flag_TouchKeyPressed = 0;
                rec_function_byte = 0;
            }
            else if(lcd_received_cmd == LCD_XY_Key_Released)	//0x72
            {
                Flag_TouchKeyPressed = 0;
                rec_function_byte = 0;
				Config_LCD_XY_Coordinates(1);					//Regular
            }
            else if((lcd_received_cmd == LCD_Value_Received) && (Flag_Value_Type == Value_Type_Float))
            {
           		Check_For_LCD_Touch_Command_Float();
            }
            else if((lcd_received_cmd == LCD_Value_Received) && (Flag_Value_Type == Value_Type_Int))
            {
           		Check_For_LCD_Touch_Command_Int();			//Changes directly in Config Parameters
			}
			Flag_ReceivedValue = 0;
        }
		else if(Flag_Slave_ReceivedValue != 0)
		{
			switch(BLE_received_cmd)
			{

				case BLE_Command_Request_Station_ID :
													BLE_Request_Station_ID();
													break;

				case BLE_Command_Buzzer_Control 	:
													if(BLE_Connect_Response() != 0)
													{
														_Bool Flag_Temp_Buzzer_Status = BLE_rec_buff[2];
														HAL_GPIO_WritePin(DO_5_GPIO_Port, DO_5_Pin,Flag_Temp_Buzzer_Status);
														HAL_GPIO_WritePin(DO_6_GPIO_Port, DO_6_Pin,Flag_Temp_Buzzer_Status);
													}
													break;

				case BLE_Command_Read_Config_Reg 	:
													BLE_Read_Config_Registers();
													break;

				case BLE_Command_Write_Single_Key 	:
													BLE_Write_Single_Coil();
													break;

				case BLE_Command_Write_Single_Config16_Reg :
													BLE_Write_Single_Config_Register();
													break;

				case BLE_Command_Write_Single_Config32_Reg :
													BLE_Write_Single_Config_Register_Float();
													break;

				case BLE_Command_Write_Job_Name 	:
													BLE_Write_Job_Name();
													break;

				case BLE_Command_Request_OTA 		:
													if(BLE_Request_OTA() != 0)
													{
														HAL_TIM_Base_Stop(&htim3);
														HAL_TIM_Base_Stop(&htim15);
														HAL_TIM_Base_Stop(&htim16);
														HAL_TIM_Base_Stop(&htim1);

														HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);   	//Z
														HAL_NVIC_DisableIRQ(ADC_IRQn);			//ADC
														HAL_NVIC_DisableIRQ(TIM3_IRQn);			//ENCODER
														HAL_NVIC_DisableIRQ(TIM1_UP_IRQn);		//5 mS Timer

														Flag_BLE_Enable 		= 0;
														Flag_BLE_Read_ON 		= 0;
														Read_Input_Group_Mode 	= 0;
														Flag_BLE_Graph_ON		= 0;
														Flag_Job_Name_Received 	= 0;

														uint16_t temp_loop = 0,ref_count = 0;

														//Time around 2 sec so that any ongoing process gets completed
														for(ref_count = 0; ref_count < 2000; ref_count++)
														{
															osDelay(1);
														}

														//Making All Digital Outputs Idle
														HAL_GPIO_WritePin(DO_1_GPIO_Port, DO_1_Pin,Pulse_Level_Idle);
														HAL_GPIO_WritePin(DO_2_GPIO_Port, DO_2_Pin,Pulse_Level_Idle);
														HAL_GPIO_WritePin(DO_3_GPIO_Port, DO_3_Pin,Pulse_Level_Idle);
														HAL_GPIO_WritePin(DO_4_GPIO_Port, DO_4_Pin,Pulse_Level_Idle);
														HAL_GPIO_WritePin(DO_5_GPIO_Port, DO_5_Pin,Pulse_Level_Idle);
														HAL_GPIO_WritePin(DO_6_GPIO_Port, DO_6_Pin,Pulse_Level_Idle);
														HAL_GPIO_WritePin(DO_7_GPIO_Port, DO_7_Pin,Pulse_Level_Idle);
//														HAL_GPIO_WritePin(DO_8_GPIO_Port, DO_8_Pin,Pulse_Level_Idle);

														Flag_DMA_ON = 0;
														File_Name_Send_String();
														BLE_Push_Data_DMA_Tx_Buffer(Resp_Bytes,Resp_Length);

														for(temp_loop = 0; temp_loop < 100; temp_loop++)
														{
															osDelay(1);
														}
														Transmit_Page_ID(Page_OTA);

														Flag_OTA_Request_Received = 1;

														temp_loop = 0;
														while(1)
														{
															  temp_loop++;
															  if(temp_loop >= 250)
															  {
																  temp_loop = 0;
																  HAL_GPIO_TogglePin(LED_RED_GPIO_Port,LED_RED_Pin);
																  Send_String_UART_232((uint8_t *)"OTA..\n");
															  }
															  osDelay(1);
														}
													}
													break;

				case BLE_Command_Enable_Disable	:
													if(BLE_Connect_Response() != 0)
													{
														Flag_BLE_Enable = BLE_rec_buff[2];
														if(Flag_BLE_Enable == 0)
														{
															Flag_BLE_Read_ON 		= 0;
															Read_Input_Group_Mode 	= 0x00;
															Flag_BLE_Graph_ON		= 0;
														}
													}
													break;

				case BLE_Command_Read_Input_Reg_Group :
													if(BLE_Connect_Response() != 0)
													{
														Read_Input_Group_Mode = BLE_rec_buff[2];

														if(Read_Input_Group_Mode == Read_Input_Group_Mode_Home)
														{
															BLE_Disp_Start_Address 	= Read_Input_Group_Mode_Home_Start_Address;
															BLE_Disp_Data_length 	= Read_Input_Group_Mode_Home_Data_Length;
															Flag_BLE_Graph_ON 		= 0;
															Flag_BLE_Read_ON 		= 1;
														}
														else if(Read_Input_Group_Mode == Read_Input_Group_Mode_Home_RawErr)
														{
															BLE_Disp_Start_Address 	= Read_Input_Group_Mode_Home_RawErr_Start_Address;
															BLE_Disp_Data_length 	= Read_Input_Group_Mode_Home_RawErr_Data_Length;
															Flag_BLE_Graph_ON 		= 0;
															Flag_BLE_Read_ON 		= 1;
														}
														else if(Read_Input_Group_Mode == Read_Input_Group_Mode_Diagnosis)
														{
															BLE_Disp_Start_Address 	= Read_Input_Group_Mode_Diagnosis_Start_Address;
															BLE_Disp_Data_length 	= Read_Input_Group_Mode_Diagnosis_Data_Length;
															Flag_BLE_Graph_ON 		= 0;
															Flag_BLE_Read_ON 		= 1;
														}
														else if(Read_Input_Group_Mode == Read_Input_Group_Mode_Graph)
														{
															BLE_Disp_Start_Address 	= Read_Input_Group_Mode_Graph_Start_Address;
															BLE_Disp_Data_length 	= Read_Input_Group_Mode_Graph_Data_Length;
															Flag_BLE_Graph_ON 		= 1;
															Flag_BLE_Read_ON 		= 0;
														}
														else
														{
															Read_Input_Group_Mode 	= 0x00;
															Flag_BLE_Read_ON 		= 0;
															Flag_BLE_Graph_ON 		= 0;
														}
													}
													break;

				default							:	break;
			}
			BLE_received_cmd = 0;
			Flag_Slave_ReceivedValue = 0;
		}
}

void Transmit_Brightness_Value_00_3F(uint8_t br_val)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 20;

	if(Flag_LCD_Init_Error != 0)
		return;

	uint8_t br_array[]={0xAA,0x5F,0x00};	//brightness min
	br_array[2] = br_val;

	HAL_UART_Transmit(&huart4,br_array,3,50);
	Transmit_EndBytes();
}

void Transmit_Init_Check(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 21;

	uint8_t temp_init[]={0xAA,0x30};

	HAL_UART_Transmit(&huart4,temp_init,2,50);							//Includes start bytes and length
	Transmit_EndBytes();
}

void Wait_for_LCD_Init(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 22;

  	uint8_t temp_count= 0;
  	char temp_str[40];
  	while(1)
  	{
  		Transmit_Init_Check();
  		retry_count++;

  		temp_count = 0;
  		while(1)
  		{
//  		HAL_IWDG_Refresh(&hiwdg1);
//  		HAL_WWDG_Refresh(&hwwdg1);
  			if((UART4->ISR & 0X00000010) != 0)
  			{
  				Handle_Line_Idle_Event();
  	  			if(Flag_LCD_Init != 0)
  	  				break;
  			}
  			osDelay(100);
  			temp_count++;
  			if(temp_count == 10)
  				break;
  		}
		sprintf(temp_str,"LCD Init Retry Count : %d\n",retry_count);
		Send_String_UART_232((uint8_t *)temp_str);
		if(Flag_LCD_Init != 0)
			break;
		if(retry_count >= 15)
		{
			sprintf(temp_str,"LCD COMMUNICATION ERROR\n");
			Send_String_UART_232((uint8_t *)temp_str);
			Flag_LCD_Init_Error = 1;
			break;
		}
  	}
}

void Handle_Line_Idle_Event(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 23;

	uint8_t Temp_received_cmd,Temp_Add_Type;
	__HAL_UART_CLEAR_FLAG(&huart4,UART_CLEAR_IDLEF);

	uint16_t data_length = Get_Received_Data_Length(&huart4);

	Flag_DMA_Buffer_Read  = Flag_DMA_Buffer_Write;
	Flag_DMA_Buffer_Write = 1 - Flag_DMA_Buffer_Write;

	HAL_UART_AbortReceive(&huart4);
	HAL_UART_Receive_IT(&huart4, (uint8_t*)receive_buff[Flag_DMA_Buffer_Write], BUFFER_SIZE_DMA_UART4);

	Temp_received_cmd = receive_buff[Flag_DMA_Buffer_Read][1];

	if(receive_buff[Flag_DMA_Buffer_Read][0] == 0xAA)
	{
		switch(Temp_received_cmd)
		{
			case LCD_Key_Up			:
									if(data_length == LCD_Data_Length_Event)
									{
										lcd_received_cmd = LCD_Key_Up;
										Flag_ReceivedValue = 1;
									}
									break;
			case LCD_Key_Pressed	:
									if(data_length == LCD_Data_Length_Event)
									{
										lcd_received_cmd = LCD_Key_Pressed;

										lcd_received_bytes[0] = receive_buff[Flag_DMA_Buffer_Read][2];		//buff[2]
										lcd_received_bytes[1] = receive_buff[Flag_DMA_Buffer_Read][3];		//buff[3]
										lcd_received_bytes[2] = receive_buff[Flag_DMA_Buffer_Read][4];		//buff[4]
										Flag_ReceivedValue = 1;
									}
									break;
			case LCD_XY_Key_Released:
									if(data_length == LCD_Data_Length_XY_Release)
									{
										lcd_received_cmd = LCD_XY_Key_Released;
										Flag_ReceivedValue = 1;
									}
									break;
			case LCD_Value_Received	:
									Temp_Add_Type = receive_buff[Flag_DMA_Buffer_Read][3];
									if(Temp_Add_Type == 0x08)		//Address N16
									{
										if(data_length == LCD_Data_Length_Value)
										{
											lcd_received_cmd = LCD_Value_Received;
											lcd_received_bytes[0] = receive_buff[Flag_DMA_Buffer_Read][2];		//buff[2]	//address always 0x00
											lcd_received_bytes[1] = receive_buff[Flag_DMA_Buffer_Read][3];		//buff[3]	//address
											lcd_received_bytes[2] = receive_buff[Flag_DMA_Buffer_Read][4];		//buff[4]	//address
											lcd_received_bytes[3] = receive_buff[Flag_DMA_Buffer_Read][5];		//buff[5]	//address

											lcd_received_bytes[4] = receive_buff[Flag_DMA_Buffer_Read][6];		//buff[6]	//Data
											lcd_received_bytes[5] = receive_buff[Flag_DMA_Buffer_Read][7];		//buff[7]	//Data

											Flag_Value_Type = Value_Type_Int;
											Flag_ReceivedValue = 1;
										}
									}
									else if(Temp_Add_Type == 0x02)		//Address N32
									{
										if(data_length == LCD_Data_Length_Float)
										{
											lcd_received_cmd = LCD_Value_Received;
											lcd_received_bytes[0] = receive_buff[Flag_DMA_Buffer_Read][2];		//buff[2]	//address
											lcd_received_bytes[1] = receive_buff[Flag_DMA_Buffer_Read][3];		//buff[3]	//address
											lcd_received_bytes[2] = receive_buff[Flag_DMA_Buffer_Read][4];		//buff[4]	//address
											lcd_received_bytes[3] = receive_buff[Flag_DMA_Buffer_Read][5];		//buff[5]	//address

											lcd_received_bytes[4] = receive_buff[Flag_DMA_Buffer_Read][6];		//buff[6]	//Data
											lcd_received_bytes[5] = receive_buff[Flag_DMA_Buffer_Read][7];		//buff[7]	//Data
											lcd_received_bytes[6] = receive_buff[Flag_DMA_Buffer_Read][8];		//buff[6]	//Data
											lcd_received_bytes[7] = receive_buff[Flag_DMA_Buffer_Read][9];		//buff[7]	//Data
											Flag_Value_Type = Value_Type_Float;
											Flag_ReceivedValue = 1;
										}
									}
									break;

			case 0x30				:
									if((Flag_LCD_Init == 0) && (data_length == LCD_Data_Length_LCD_Init))
									{
										Flag_LCD_Init = 1;
									}
									break;
			default					:
									break;
		}
	}
}

void update_lcd_buttons_status(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 24;

		Buttons_Status_LEDs = 0;

		if(Flag_Auto_Horizontal != 0)
				Buttons_Status_LEDs |= 0x0001;		//POS 0

		if(Flag_Auto_Vertical != 0)
				Buttons_Status_LEDs |= 0x0002;		//POS 1

//==============================================

		if(Flag_Correction_Horz_Upper != 0)
		{
				Buttons_Status_LEDs |= 0x0004;		//POS 2 Horizontal Upper Correction ( Right )
				Flag_Correction_Horz_Upper = 0;
		}

		if(Flag_Correction_Horz_Lower != 0)
		{
				Buttons_Status_LEDs |= 0x0008;		//POS 3 Horizontal Lower Correction ( Left )
				Flag_Correction_Horz_Lower = 0;
		}

		if(Flag_Correction_Vert_Upper != 0)
		{
				Buttons_Status_LEDs |= 0x0010;		//POS 4 Vertical Upper Correction ( Up )
				Flag_Correction_Vert_Upper = 0;
		}

		if(Flag_Correction_Vert_Lower != 0)
		{
				Buttons_Status_LEDs |= 0x0020;		//POS 5 Vertical Lower Correction ( Down )
				Flag_Correction_Vert_Lower = 0;
		}

//===================================================================

		if(Flag_GateSet != 0)
				Buttons_Status_LEDs |= 0x0080;		//POS 7 MAIN GATE SET LED

		if(Flag_Disp_Z_Missing != 0)
			Buttons_Status_LEDs |= 0x0200;			//POS 9	Z missing

		if(rec_function_byte == 'L')
				Buttons_Status_LEDs |= 0x0800;		//POS 11

		if(Flag_Fine_Left_Key != 0)
		{
			Buttons_Status_LEDs |= 0x1000;			//POS 12
			Flag_Fine_Left_Key = 0;
		}

		if(rec_function_byte == 'R')
				Buttons_Status_LEDs |= 0x2000;		//POS 13

		if(Flag_Fine_Right_Key != 0)
		{
			Buttons_Status_LEDs |= 0x4000;			//POS 14
			Flag_Fine_Right_Key = 0;
		}

//===================================================================

		BLE_Buttons_Status_LEDs = Buttons_Status_LEDs;

		//For LCD Only - TCP and Link bits are not required at Server
		if(Flag_TCP_Connected != 0)
				Buttons_Status_LEDs |= 0x0040;		//POS 6 TCP Connection

		if(Flag_ETH_Link_Status != 0)
				Buttons_Status_LEDs |= 0x0100;		//POS 8 ETHERNET LINK STATUS

		//For BLE Only
		if(Flag_Permit_Actions != 0)
				BLE_Buttons_Status_LEDs |= 0x0100;	//POS 8 For BLE Only

		if((TIM3->CR1 & 0x0010) != 0)
		{
			Flag_Encoder_Direction_Reverse = 1;
			BLE_Buttons_Status_LEDs |= 0x0400;		//POS 10 For BLE Only
		}
		else
		{
			Flag_Encoder_Direction_Reverse = 0;
		}

		if(Screen_ID == Page_Diagnose)
		{
			if(Flag_Encoder_Direction_Reverse != 0)
				Buttons_Status_LEDs |= 0x0400;		//POS 10
		}
		else
		{
			if(Flag_TCP_Retry != 0)
			{
				Buttons_Status_LEDs |= 0x0400;		//POS 10
			}
		}

//==============================================================

		Error_Status_Code = 0;
		if(Flag_Alarm != 0)
		{
			BLE_Buttons_Status_LEDs |= 0x8000;

			uint8_t No_Of_Errors = 0;

			if(Flag_Alarm_Blink < 2)
				Buttons_Status_LEDs |= 0x8000;			//POS 15
			Flag_Alarm_Blink++;
			if(Flag_Alarm_Blink > 3)
				Flag_Alarm_Blink = 0;

			if(Flag_Over_Error != 0)
			{
				Error_Status_Code |= 0x0001;			//Error Code POS 0 Combined OverError
				No_Of_Errors++;

				if(Flag_Over_Error_Horz != 0)
					Error_Status_Code |= 0x0100;		//Error Code POS 8 OverError Horz

				if(Flag_Over_Error_Vert != 0)
					Error_Status_Code |= 0x0200;		//Error Code POS 9 OverError Vert

			}

			if(Flag_Mark_Missing != 0)
			{
				Error_Status_Code |= 0x0002;			//Error Code POS 1 Combined Mark Missing
				No_Of_Errors++;

				if(Flag_M0_Missing != 0)
				{
					Error_Status_Code |= 0x0400;		//Error Code POS 10 Mark Missing M0
				}

				if(Flag_M1_Missing != 0)
				{
					Error_Status_Code |= 0x0800;		//Error Code POS 11 Mark Missing M1
				}

				if(Flag_M2_Missing != 0)
				{
					Error_Status_Code |= 0x1000;		//Error Code POS 12 Mark Missing M2
				}
			}

			if(Flag_BaseLine_Error != 0)
			{
				Error_Status_Code |= 0x0008;			//Error Code POS 4 BaseLine Error
				No_Of_Errors++;
			}

			if(No_Of_Errors > 1)
				Flag_Multiple_Errors = 1;
			else
				Flag_Multiple_Errors = 0;
		}
		else
			Flag_Multiple_Errors = 0;

		Cur_Error_Status_Code = Error_Status_Code;
//==============
}

void Transmit_Page_ID(unsigned char pg_id)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 25;

	uint8_t pg_array[]={0xAA,0x70,0x00,0x00};	
	
	pg_array[3] = pg_id;
	Push_Data_DMA_Tx_Buffer(pg_array,4,1);
}

void Seperate_Bytes(int16_t Temp_Value,uint8_t *Byte_Higher, uint8_t *Byte_Lower)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 26;

		*Byte_Higher = 	(Temp_Value & 0xFF00) >> 8;
		*Byte_Lower = 	Temp_Value & 0x00FF;
}

void Seperate_Bytes_Float(float Temp_Value,uint8_t *Float_Bytes)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 27;

	Float_Disp.df = Temp_Value;
	Float_Bytes[0] = Float_Disp.dc[3];
	Float_Bytes[1] = Float_Disp.dc[2];
	Float_Bytes[2] = Float_Disp.dc[1];
	Float_Bytes[3] = Float_Disp.dc[0];
}

void Init_Calc_ARCSS(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 28;

	Convert_PPR_To_mm = (float)Config_Parameters[Alt_BagLength] / Alt_Encoder_Count;	//not required every time, only when circum is changed
	Convert_mm_To_PPR =  Alt_Encoder_Count / (float)Config_Parameters[Alt_BagLength];	//not required every time, only when circum is changed

	Total_Baglength_Count = Alt_Encoder_Count;
	
	if((Total_Baglength_Count % pixels_graph_lcd) == 0)									//circum or baglength change
	{
		Skip_Limit = Total_Baglength_Count / pixels_graph_lcd;
	}
	else
	{
		Skip_Limit = (Total_Baglength_Count / pixels_graph_lcd) + 1;
	}

	GateWidth_PPR = Convert_mm_To_PPR * Config_Parameters[Alt_GateWidth] ; 	//ppr		//circum or gatewidth change
	GateWidth_Graph_Count = GateWidth_PPR / Skip_Limit;									//graph value 0 to 800 ( circum or gatewidth change )

	Min_Error_PPR[Horizontal] 	= Convert_mm_To_PPR * Config_Float[AltF_Corr_Min_Error_Horz];
	Max_Error_PPR[Horizontal] 	= Convert_mm_To_PPR * Config_Float[AltF_Corr_Max_Error_Horz];

	Min_Error_PPR[Vertical] 	= Convert_mm_To_PPR * Config_Float[AltF_Corr_Min_Error_Vert];
	Max_Error_PPR[Vertical] 	= Convert_mm_To_PPR * Config_Float[AltF_Corr_Max_Error_Vert];

	if(Flag_Home_Mode == 0)
	{
		Distance_From_Slave_mm[Horizontal] = (Config_Parameters[Alt_Station_ID] - Config_Parameters[Alt_Master_ID_Horz])*Config_Float[AltF_SetValue];
		Distance_From_Slave_PPR[Horizontal] = Convert_mm_To_PPR * Distance_From_Slave_mm[Horizontal];
		SetValue_Graph_Count[Horizontal] = Distance_From_Slave_PPR[Horizontal] / Skip_Limit;

		Distance_From_Slave_mm[Vertical] = (Config_Parameters[Alt_Station_ID] - Config_Parameters[Alt_Master_ID_Vert])*Config_Float[AltF_SetValue];
		Distance_From_Slave_PPR[Vertical] = Convert_mm_To_PPR * Distance_From_Slave_mm[Vertical];
		SetValue_Graph_Count[Vertical] = Distance_From_Slave_PPR[Vertical] / Skip_Limit;

		if(Config_Parameters[Alt_MasterMarkPosition] == Master_Mark_First)	//M1=>M/S	Slave will be at right side
		{
			Position_Gate[Master_Horz] 	= Position_Gate[Slave] - SetValue_Graph_Count[Horizontal];
			Position_Gate[Master_Vert] 	= Position_Gate[Slave] - SetValue_Graph_Count[Vertical];
		}
		else																//M/S=>M1	Slave will be at left side
		{
			Position_Gate[Master_Horz] 	= Position_Gate[Slave] + SetValue_Graph_Count[Horizontal];
			Position_Gate[Master_Vert] 	= Position_Gate[Slave] + SetValue_Graph_Count[Vertical];
		}
	}
	Gate_Limit_Calculations();
	Gate_Calculations();
	Flag_Init_Calc_Done = 1;
}

void Gate_Limit_Calculations(void)	//05.04.2023
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 29;

	_Bool Flag_Temp_HV;

	if(Flag_Home_Mode != 0)
	{
		Gate_Limit_Calculations_Home();

		Gate_Start[Master_Horz] 		= 0;
		Gate_Start_ppr[Master_Horz] 	= 0;
		Gate_End_ppr[Master_Horz]   	= 0;
		Gate_End[Master_Horz] 			= 0;

		Gate_Start[Master_Vert] 		= 0;
		Gate_Start_ppr[Master_Vert] 	= 0;
		Gate_End_ppr[Master_Vert]   	= 0;
		Gate_End[Master_Vert] 			= 0;

		return;
	}

	if(Distance_From_Slave_mm[Horizontal] >= Distance_From_Slave_mm[Vertical])
		Flag_Temp_HV = Horizontal;		//Flag will indicate gate with larger distance from slave
	else
		Flag_Temp_HV = Vertical;

	if(Config_Parameters[Alt_MasterMarkPosition] == Master_Mark_First)		//M1=>M/S Slave will be at right
	{
		Gate_Graph_Max_Valid_Value_Slave_Count = (Total_Baglength_Count - GateWidth_PPR) / Skip_Limit;
		Gate_Graph_Min_Valid_Value_Slave_Count = SetValue_Graph_Count[Flag_Temp_HV];

		Gate_Graph_Max_Valid_Value_Slave_PPR = Total_Baglength_Count - GateWidth_PPR;
		Gate_Graph_Min_Valid_Value_Slave_PPR = Distance_From_Slave_PPR[Flag_Temp_HV];
	}
	else															//M/S=>M1 Slave will be at left
	{
		Gate_Graph_Min_Valid_Value_Slave_Count = 0;
		Gate_Graph_Max_Valid_Value_Slave_Count = (Total_Baglength_Count - GateWidth_PPR) / Skip_Limit - SetValue_Graph_Count[Flag_Temp_HV];

		Gate_Graph_Min_Valid_Value_Slave_PPR = 0;
		Gate_Graph_Max_Valid_Value_Slave_PPR = Total_Baglength_Count - GateWidth_PPR - Distance_From_Slave_PPR[Flag_Temp_HV];
	}
}

void Gate_Limit_Calculations_Home(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 30;

	Gate_Graph_Max_Valid_Value_Slave_Count = (Total_Baglength_Count - GateWidth_PPR) / Skip_Limit;
	Gate_Graph_Min_Valid_Value_Slave_Count = 0;

	Gate_Graph_Max_Valid_Value_Slave_PPR = Total_Baglength_Count - GateWidth_PPR;
	Gate_Graph_Min_Valid_Value_Slave_PPR = 0;
}


void Gate_Calculations(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 31;

	if(Flag_Home_Mode == 0)
	{
		Gate_Calculations_Master_Horizontal();
		Gate_Calculations_Master_Vertical();
	}
	Gate_Calculations_Slave();
}

void Gate_Calculations_Master_Horizontal(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 32;

	Gate_Start[Master_Horz] 		= Position_Gate[Master_Horz];
	Gate_Start_ppr[Master_Horz] 	= Gate_Start[Master_Horz] * Skip_Limit;

	Gate_End_ppr[Master_Horz]   	= Gate_Start_ppr[Master_Horz] + GateWidth_PPR;
	Gate_End[Master_Horz] 			= Gate_End_ppr[Master_Horz] / Skip_Limit;
}

void Gate_Calculations_Master_Vertical(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 33;

	Gate_Start[Master_Vert] 		= Position_Gate[Master_Vert];
	Gate_Start_ppr[Master_Vert] 	= (Gate_Start[Master_Vert] * Skip_Limit);
	Gate_End_ppr[Master_Vert]   	= Gate_Start_ppr[Master_Vert] + GateWidth_PPR;
	Gate_End[Master_Vert] 			= Gate_End_ppr[Master_Vert] / Skip_Limit;
}

void Gate_Calculations_Slave(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 34;

	Gate_Start[Slave] 		= Position_Gate[Slave];
	Gate_Start_ppr[Slave] 	= (Gate_Start[Slave] * Skip_Limit);
	Gate_End_ppr[Slave]   	= Gate_Start_ppr[Slave] + GateWidth_PPR;
	Gate_End[Slave] 		= Gate_End_ppr[Slave] / Skip_Limit;

	Config_Parameters[Alt_Position_Gate_Slave_GCount] = Position_Gate[Slave];

	Flag_Update_Graph_Gates = 1;
}

_Bool Stop_Controlling_if_Error_IsDecreasing(float cur_Error)
{
	float temp_diff = (cur_Error < Prev_Corrected_Error)
			? Prev_Corrected_Error - cur_Error : cur_Error-Prev_Corrected_Error;

	char Cur_Error_State = (cur_Error < 0)? 0:1;
	char Prev_Error_State = (Prev_Corrected_Error < 0)? 0:1;

	float Pos_Cur_Error = (cur_Error < 0) ? cur_Error * -1 : cur_Error;
	float Pos_Prev_Error = (Prev_Corrected_Error < 0) ? Prev_Corrected_Error * -1 : Prev_Corrected_Error;

	if(Prev_Corrected_Error == 5000.0)
	{
		Prev_Corrected_Error = cur_Error;
		return(0);
	}
	if(Prev_Error_State != Cur_Error_State)
	{
		Prev_Corrected_Error = cur_Error;
		return(0);
	}
	else
	{
		if((Pos_Cur_Error < Pos_Prev_Error) && (temp_diff > 0.05))
		{
			return(1);
		}
		else
		{
			Prev_Corrected_Error = cur_Error;
			return(0);
		}
	}
	return(0);
}

//Check at the time of GateSet, avg and Avg_Value need to be reset
void Avg_Errors_Vertical(float prev , float cur)
{
	static uint8_t skip_corr_count = 0;
	static float Avg_value = 0;
	float diff = 0;
	uint16_t temp_repeat_length_limit;

	//Auto Averaging
	if(Config_Parameters[Alt_Auto_Averaging_Vert] != 0)
	{
		temp_repeat_length_limit = Get_Auto_Averaging_Count_Based_On_Speed_Vert();
	}
	else
	{
		temp_repeat_length_limit = Config_Parameters[Alt_Corr_Repeat_Vert];
	}
	Disp_auto_average[Vertical] = temp_repeat_length_limit;	//Remove
	//============================================================

	if(Repeat_Length[Vertical] == 0)
	{
		Avg_value = 0;
	}

	if(prev < cur)
	{
		diff = cur-prev;
	}
	else
	{
		diff = prev-cur;
	}

	if(diff > Config_Float[AltF_Error_Filter_Vert])
	{
		skip_corr_count++;
		if(skip_corr_count <= 2)
		{
			return;
		}
	}
	skip_corr_count = 0;

	Repeat_Length[Vertical]++;
	Avg_value= Avg_value + cur;

	if(Repeat_Length[Vertical] >= temp_repeat_length_limit)
	{
		CurError_Avg[Vertical] = Avg_value/Repeat_Length[Vertical];
		Repeat_Length[Vertical] = 0;
		Avg_value = 0;
		Flag_New_Error_Arrived_Vertical = 1;

		if(Perform_Good_Copy_Vert_Stage == 1)
		{
			Config_Float[AltF_Corr_Offset_Vert] = Config_Float[AltF_Corr_Offset_Vert] - CurError_Avg[Vertical];
			Perform_Good_Copy_Vert_Stage = 2;
		}
	}
}

void Avg_Errors_Horizontal(float prev , float cur)
{
	static uint8_t skip_corr_count = 0;
	static float Avg_value = 0;
	float diff = 0;
	uint16_t temp_repeat_length_limit;

	//Auto Averaging
	if(Config_Parameters[Alt_Auto_Averaging_Horz] != 0)
	{
		temp_repeat_length_limit = Get_Auto_Averaging_Count_Based_On_Speed_Horz();
	}
	else
	{
		temp_repeat_length_limit = Config_Parameters[Alt_Corr_Repeat_Horz];
	}
	Disp_auto_average[Horizontal] = temp_repeat_length_limit;		//Remove
	//============================================================

	if(Repeat_Length[Horizontal] == 0)
	{
		Avg_value = 0;
	}

	if(prev < cur)
	{
		diff = cur-prev;
	}
	else
	{
		diff = prev-cur;
	}

	if(diff > Config_Float[AltF_Error_Filter_Horz])
	{
		skip_corr_count++;
		if(skip_corr_count <= 2)
		{
			return;
		}
	}
	skip_corr_count = 0;

	Repeat_Length[Horizontal]++;
	Avg_value = Avg_value + cur;

	if(Repeat_Length[Horizontal] >= temp_repeat_length_limit)
	{
		CurError_Avg[Horizontal] = Avg_value/Repeat_Length[Horizontal];
		Repeat_Length[Horizontal] = 0;
		Avg_value = 0;
		Flag_New_Error_Arrived_Horizontal = 1;

		if(Perform_Good_Copy_Horz_Stage == 1)
		{
			Config_Float[AltF_Corr_Offset_Horz] = Config_Float[AltF_Corr_Offset_Horz] - CurError_Avg[Horizontal];
			Perform_Good_Copy_Horz_Stage = 2;
		}
	}
}

uint16_t Get_Auto_Averaging_Count_Based_On_Speed_Horz(void)
{
	for(uint8_t i = 0; i < 7; i++)
	{
		if(mpm_int <= (Auto_Averaging_Speed_Ref_Horz[i] + Auto_Averaging_Speed_Tolerance_Horz))
		{
			return(Auto_Averaging_Average_Count_Horz[i]);
		}
	}
	return(Auto_Averaging_Average_Count_High_Speed_Horz);
}

uint16_t Get_Auto_Averaging_Count_Based_On_Speed_Vert(void)
{
	for(uint8_t i = 0; i < 7; i++)
	{
		if(mpm_int <= (Auto_Averaging_Speed_Ref_Vert[i] + Auto_Averaging_Speed_Tolerance_Vert))
		{
			return(Auto_Averaging_Average_Count_Vert[i]);
		}
	}
	return(Auto_Averaging_Average_Count_High_Speed_Vert);
}

void Calculate_Error_Home(void)
{
	if(Flag_Event_Gate_Set != 0)
	{
		if(Config_Parameters[Alt_Encoder_Time_Mode] == Encoder_Mode)
		{
			SetValue_Home_Mode = Mark_Encoder_Value[Slave][Config_Parameters[Alt_MarkType]] * Convert_PPR_To_mm;
		}
		else
		{
			SetValue_Home_Mode = Mark_TimeStamp_uS[Slave][Config_Parameters[Alt_MarkType]] * Config_Parameters[Alt_BagLength];
			SetValue_Home_Mode = SetValue_Home_Mode / value_microsecZ_avg;
		}
		Flag_Event_Gate_Set = 0;
		Flag_Update_SetValue = 1;
	}
	else
	{
		PrevError[Vertical] = CurError[Vertical];
		if(Config_Parameters[Alt_Encoder_Time_Mode] == Encoder_Mode)
		{
			CurError[Vertical] = Mark_Encoder_Value[Slave][Config_Parameters[Alt_MarkType]] * Convert_PPR_To_mm;
		}
		else
		{
			CurError[Vertical] = Mark_TimeStamp_uS[Slave][Config_Parameters[Alt_MarkType]] * Config_Parameters[Alt_BagLength];
			CurError[Vertical] = CurError[Vertical] / value_microsecZ_avg;
		}
		CurError[Vertical] = SetValue_Home_Mode - CurError[Vertical] + Config_Float[AltF_Corr_Offset_Vert];
		Avg_Errors_Vertical(PrevError[Vertical],CurError[Vertical]);
		CurErrorPositive_Avg[Vertical] 	= CurError_Avg[Vertical] < 0 ? CurError_Avg[Vertical] * (-1) : CurError_Avg[Vertical];
	}

	if(Screen_ID == Page_Diagnose)
	{
		Disp_Actual_Error[Vertical]   = CurError[Vertical];
	}
	else
	{
		Disp_Actual_Error[Vertical]   = CurError_Avg[Vertical];
	}

	if((Flag_Serial_Debug_Enable != 0) && (Flag_Auto_Vertical == 0))
	{
		if(Config_Parameters[Alt_Encoder_Time_Mode] == Encoder_Mode)
		{
			sprintf(Debug_String,"SL: %u %u\n",
				Mark_Encoder_Value[Slave][Rising_Edge],
				Mark_Encoder_Value[Slave][Falling_Edge]);
		}
		else
		{
			sprintf(Debug_String,"SL: %lu %lu, SP: %lu, SPA:%lu\n",
					Mark_TimeStamp_uS[Slave][Rising_Edge],
					Mark_TimeStamp_uS[Slave][Falling_Edge],
					value_microsecZ,
					value_microsecZ_avg);
		}

		if(Flag_Update_SetValue != 0)
		{
			strcat(Debug_String,"Set^\n");
		}
		Flag_Debug = 1;
	}

	Mark_Encoder_Value[Slave][Rising_Edge] = 0;
	Mark_Encoder_Value[Slave][Falling_Edge] = 0;

	Mark_TimeStamp_uS[Slave][Rising_Edge] = 0;
	Mark_TimeStamp_uS[Slave][Falling_Edge] = 0;

	Flag_M2_Detected = 0;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)				//EXTI
{
	arc_execution_int_prev_id = arc_execution_func_cur_id;
	arc_execution_int_id = 1;

	if(GPIO_Pin == GPIO_PIN_8)	//Z'
	{
		Flag_Z_Missing = 0;

		Enc_Count_at_Z = __HAL_TIM_GET_COUNTER(&htim3); 			//Count_A

		Disp_Encoder_Count 	= Enc_Count_at_Z + (__HAL_TIM_GET_AUTORELOAD(&htim3) + 1) - Prev_Enc_Count_at_Z;

		Temp_Disp_Enc_Count_at_Z = Enc_Count_at_Z;
		Temp_Disp_Prev_Enc_Count_at_Z = Prev_Enc_Count_at_Z;

		Prev_Enc_Count_at_Z = Enc_Count_at_Z;

		//================
		Actual_Meter_Count++;

		HAL_GPIO_TogglePin(DETECT_Z_OP_GPIO_Port, DETECT_Z_OP_Pin);

		if(Z_to_Z_Averaging_Done == 0)	//First Z
		{
			bl_sum = 0;
			bl_count = 0;

			if(Flag_Print_Mode != 0)
			{
				HAL_TIM_Base_Start(&htim6);		//Start ADC1 & ADC3 DMA for baseline
			}
		}
		else
		{
			if(Disp_Encoder_Count != 20000)
			{
				if((Indexing_stage_at_System_Start == 0) && (Stage_AutoSearch == 0) && (Flag_Index == 0) && (Flag_Index1 == 0) && (Flag_EncError_SkipInc == 0))
				{
					Flag_EncError = 1;

					if(Disp_Encoder_Count >= 39000)
					{
						Disp_Encoder_Count = Disp_Encoder_Count - 20000;
					}
					else if(Disp_Encoder_Count <= 1000)
					{
						Disp_Encoder_Count = Disp_Encoder_Count + 20000;
					}
					EncError_DispCount = Disp_Encoder_Count;
					EncError_TotalCount++;
				}
				Flag_EncError_SkipInc = 0;
			}

			if(Flag_Z2Z_Completed == 0)
			{
				Calculate_Baseline_ADC3_DMA();
				Flag_Z2Z_Completed = 1;
			}
		}
		Z_to_Z_Averaging_Done = 1;

		/* Timer3 Encoder Counting Start in interrupt mode*/
		if(((Flag_System_Start == 0) && (Flag_Init_Calc_Done == 1) && (Flag_Z2Z_Completed == 1)) || (Flag_BagLength == 1))
		{
			Flag_Z_Received = 1;	//Added 10.11.2025

			TIM3->SR = 0;	//clears all pending flags

			__HAL_TIM_SET_AUTORELOAD(&htim3,Total_Baglength_Count  -1 );		//Total_Baglength_Count - 1
			__HAL_TIM_SET_COUNTER(&htim3,0);

			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,Skip_Limit);
			HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_3);

			//================
			HAL_TIM_Base_Start_IT(&htim3);
			//=================

			Flag_System_Start = 1;
			Flag_BagLength = 0;

			Flag_Index = 1;		//Added to use index at power reset
		}

		if(Flag_Index != 0)
		{
			uint16_t temp_index_ppr = Config_Parameters[Alt_Index_mm];
			temp_index_ppr = temp_index_ppr * Convert_mm_To_PPR;

			TIM3->SR = 0;	//clears all pending flags
			__HAL_TIM_SET_COUNTER(&htim3,0);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,Skip_Limit);
			__HAL_TIM_SET_AUTORELOAD(&htim3,Total_Baglength_Count + temp_index_ppr - 1);
			Flag_Index = 0;
			Flag_Index1 = 1;
		}
	}

	arc_execution_int_id = 2;
}

/* Encoder value OverFlow Interrupt*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	arc_execution_int_prev_id = arc_execution_func_cur_id;

	if(htim->Instance == TIM3)  								//TIM3_ Reload Interrupt
	{
		arc_execution_int_id = 3;

		if(Flag_Z_Received == 1)
		{
				Flag_Disp_Z_Missing = Flag_Z_Missing;
				Flag_Z_Missing = 1;

				HAL_GPIO_TogglePin(DETECT_TIM3_OP_GPIO_Port, DETECT_TIM3_OP_Pin);

				if(Flag_BagLength_Arrived 	==  0)		//CHANGE DONE ASHOK.. 27.11.2020..
				{
					Flag_Buffer_RD 			= Flag_Buffer_WR;
					Flag_Buffer_WR 			= 1 - Flag_Buffer_WR;
					Flag_BagLength_Arrived 	= 1;
					Actual_Count_lcd 		= Count_lcd;
				}
				Count_lcd = 4;

				//=====================================
				Real_Baglength_Arrive_Count++;

				if(Flag_BLE_BagLength_Arrived 	==  0)
				{
					Flag_BLE_Buffer_RD 			= Flag_BLE_Buffer_WR;
					Flag_BLE_Buffer_WR 			= 1 - Flag_BLE_Buffer_WR;
					Flag_BLE_BagLength_Arrived 	= 1;
					Actual_Count_ble 			= Count_ble;

					Executed_Baglength_Arrive_Count++;
				}
				Count_ble = 3;
				//=====================================

				Disp_Valid_Mark = MarkCount;
				MarkCount = 0;

				//======Auto Search and Indexing=====
				if(Stage_AutoSearch == 2)
				{
					 if(AutoSearch_PPR != 0)
					 {
						AutoSearch_mm = AutoSearch_PPR * Convert_PPR_To_mm;
						int16_t temp_diff = AutoSearch_mm - AutoSearch_Setmm;		//100 mm

						Config_Parameters[Alt_Index_mm] = Config_Parameters[Alt_Index_mm] + temp_diff;

						if(Config_Parameters[Alt_Index_mm] < 0)
						{
							Config_Parameters[Alt_Index_mm] = Config_Parameters[Alt_BagLength] + Config_Parameters[Alt_Index_mm];
						}
						Stage_AutoSearch = 3;

						uint16_t temp_index_ppr = Config_Parameters[Alt_Index_mm];
						temp_index_ppr = temp_index_ppr * Convert_mm_To_PPR;

						__HAL_TIM_SET_COUNTER(&htim3,0);
						__HAL_TIM_SET_AUTORELOAD(&htim3,Total_Baglength_Count + temp_index_ppr - 1);
					 }
					 else
					 {
						AutoSearch_mm = 0;
						Stage_AutoSearch = 0;
						Flag_AutoSearch_Error = 1;
						Flag_AutoSearch_Update = 1;
					 }
				}
				else if(Stage_AutoSearch == 3)
				{
					Stage_AutoSearch = 4;
					Flag_AutoSearch_Error 	= 0;
					Flag_AutoSearch_Update 	= 1;
					Flag_Index1 = 1;
				}
				else if(Stage_AutoSearch == 4)
				{
					Stage_AutoSearch = 0;
				}

				__HAL_TIM_CLEAR_IT(htim, TIM_IT_CC3);
				__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,Skip_Limit);

				if(Disp_Valid_Mark >= Config_Parameters[Alt_Station_ID])
				{
					if((Flag_M2_Detected != 0) && (Flag_M1_Detected != 0) && (Flag_M0_Detected != 0))
					{
						Flag_All_Marks_OK = 1;
					}
					else
					{
						Flag_All_Marks_OK = 0;
					}
				}
				else
				{
					Flag_All_Marks_OK = 0;
				}

				if(Flag_Z2Z_Completed)
				{
					Calculate_Baseline_ADC3_DMA();

					if(Flag_Print_Mode != 0)
					{
						if(bl_average < Config_Float[AltF_BaseLine_Error_Alarm_Limit])
						{
							Flag_BaseLine_Error = 1;
							BaseLine_Valid_Count = 0;
						}
						else if(bl_average > (Config_Float[AltF_BaseLine_Error_Alarm_Limit] + Config_Parameters[Alt_ValidBaseLine]))
						{
							Flag_BaseLine_Error = 0;
						}

						if(bl_average <= 4095)
						{
							if(bl_average > Config_Parameters[Alt_ValidBaseLine])
							{
								//Limit to detect Rising Edge
								Temp_Diff_BL_VBL = bl_average - Config_Parameters[Alt_ValidBaseLine];

								//Limit to detect Falling Edge
								Temp_Diff_BL_SG = Temp_Diff_BL_VBL + Config_Parameters[Alt_Valid_BL_Window_Lower];

								if(Flag_Set_Threshold != 0)
								{
									ADC_ChangeThreshold(Temp_Diff_BL_VBL, Max_Count_Base_Line);
									Set_Threshold_Count++;
									Prev_Diff_BL_VBL = Temp_Diff_BL_VBL;
									Flag_Mark_Start = 0;
									Flag_Set_Threshold = 0;
								}
								else
								{
									Flag_TIM3_Baglength_Set_Threshold = 1;
								}
								Disp_H_Count = ADC_H_Count;
								ADC_H_Count = 0;
							}
							else
							{
								Temp_Diff_BL_VBL = 0;
								}
						}
						else
						{
							Temp_Diff_BL_VBL = 0;
						}
					}
					else
					{
						Flag_BaseLine_Error = 0;
						Temp_Diff_BL_VBL = 0;
					}
				}

				if(Flag_Index1 != 0)
				{
					__HAL_TIM_SET_AUTORELOAD(&htim3,Total_Baglength_Count -1);
					Flag_Index1 = 0;
					Flag_EncError_SkipInc = 1;

					if(Stage_AutoSearch == 1)	//will complete a z to z cycle with index 0
					{
						Stage_AutoSearch = 2;	//will now try to find first edge after z
						AutoSearch_PPR = 0;
					}

					if(Indexing_stage_at_System_Start == 1)
					{
						Indexing_stage_at_System_Start = 2;
					}
				}
				else if(Indexing_stage_at_System_Start == 2)
				{
					if(Flag_All_Marks_OK != 0)
					{
						if((Flag_GateSet == 0) && (Flag_BaseLine_Error == 0))
						{
							Flag_GateSet = 1;

							if((Auto_HV_Status != 0) && ((Reset_Cause == RESET_WINDOW_WATCHDOG) || (Reset_Cause == RESET_INDEPENDENT_WATCHDOG)))
							{
								if(Auto_HV_Status == 1)			//1 = 01 = HOFF-VON
								{
									Flag_Auto_Vertical = 1;
									Repeat_Length[Vertical] = 0;
									__HAL_TIM_SET_COUNTER(&htim15,0);
									__HAL_TIM_SET_AUTORELOAD(&htim15,Max_Value_Timer_Idle);
									HAL_TIM_Base_Start_IT(&htim15);

									Flag_Auto_Any = 1;
								}
								else if(Auto_HV_Status == 2)	//2 = 10 = HON-VOFF
								{
									Flag_Auto_Horizontal = 1;
									Repeat_Length[Horizontal] = 0;
									__HAL_TIM_SET_COUNTER(&htim16,0);
									__HAL_TIM_SET_AUTORELOAD(&htim16,Max_Value_Timer_Idle);		//Max_Value_Timer_Idle
									HAL_TIM_Base_Start_IT(&htim16);

									Flag_Auto_Any = 1;
								}
								else if(Auto_HV_Status == 3)	//3 = 11 = HON-VON
								{
									Flag_Auto_Horizontal = 1;
									Repeat_Length[Horizontal] = 0;
									__HAL_TIM_SET_COUNTER(&htim16,0);
									__HAL_TIM_SET_AUTORELOAD(&htim16,Max_Value_Timer_Idle);		//Max_Value_Timer_Idle
									HAL_TIM_Base_Start_IT(&htim16);

									Flag_Auto_Vertical = 1;
									Repeat_Length[Vertical] = 0;
									__HAL_TIM_SET_COUNTER(&htim15,0);
									__HAL_TIM_SET_AUTORELOAD(&htim15,Max_Value_Timer_Idle);
									HAL_TIM_Base_Start_IT(&htim15);

									Flag_Auto_Any = 1;
								}
							}
						}
					}
					Indexing_stage_at_System_Start = 0;
				}

				//SPEED CALCULATION
				Zero_Spd_Time = 0;
				value_microsecZ = __HAL_TIM_GET_COUNTER(&htim2);
				__HAL_TIM_SET_COUNTER(&htim2,0);

				if((Config_Parameters[Alt_Encoder_Time_Mode] == Time_Mode) && (Config_Parameters[Alt_Time_Average] != 0))
				{
					uint32_t temp_sum = 0;

					Speed_Values[Speed_index] = value_microsecZ;

					if(Flag_Speed_Avg_Reset != 0)
					{
						for(uint8_t i = 0; i < (Speed_index + 1); i++)
						{
							temp_sum = temp_sum + Speed_Values[i];
						}
						value_microsecZ_avg = temp_sum / (Speed_index + 1);
					}
					else
					{
						for(uint8_t i = 0; i < Config_Parameters[Alt_Time_Average]; i++)
						{
							temp_sum = temp_sum + Speed_Values[i];
						}
						value_microsecZ_avg = temp_sum / Config_Parameters[Alt_Time_Average];
					}

					Speed_index++;
					if(Speed_index >= Config_Parameters[Alt_Time_Average])
					{
						Speed_index = 0;
						Flag_Speed_Avg_Reset = 0;
					}
				}
				else
				{
					value_microsecZ_avg = value_microsecZ;
				}

				//====================================================================

				if(value_microsecZ != 0)
				{
					mpm_int = temp_speed_multiplier / value_microsecZ;
				}
				else
				{
					mpm_int = 0;
				}

				//=====================================================================

				if(Flag_GateSet == 0)
				{
					Flag_New_Error_Arrived_Vertical 	= 0;
					Flag_New_Error_Arrived_Horizontal 	= 0;

					PrevError[Vertical] 	= 0;
					PrevError[Horizontal] 	= 0;

					PrevError_Avg[Vertical] = 0;
					PrevError_Avg[Horizontal] = 0;

					CurError[Vertical] = 0;
					CurError[Horizontal] = 0;

					CurError_Avg[Vertical] = 0;
					CurError_Avg[Horizontal] = 0;

					CurErrorPositive_Avg[Vertical] = 0;
					CurErrorPositive_Avg[Horizontal] = 0;

					Prev_Master_Count = 0;

					Flag_Mark_Missing = 0;
					Flag_M0_Missing = 0;
					Flag_M1_Missing = 0;
					Flag_M2_Missing = 0;

					Mark_M0_Height_Disp_Count = 0;
					Mark_M1_Height_Disp_Count = 0;
					Mark_M2_Height_Disp_Count = 0;

					Disp_auto_average[Horizontal] = 0;
					Disp_auto_average[Vertical] = 0;
				}
				else	//GateSet = 1;
				{
					if(Flag_M2_Detected != 0)		//Slave detected then only error calculation is possible
					{
						Flag_M2_Missing = 0;
						Mark_M2_Height_Disp_Count = Mark_M2_Height_Diff_Count;

						if(Flag_Home_Mode != 0)
						{
							Calculate_Error_Home();
							arc_execution_int_id++;
							return;
						}

						if(Flag_M0_Detected != 0)
						{
							Flag_M0_Missing = 0;

							if(Config_Parameters[Alt_Encoder_Time_Mode] == Encoder_Mode)
							{
								My_Error[Horizontal] = (Mark_Encoder_Value[Master_Horz][Rising_Edge] - Mark_Encoder_Value[Master_Horz][Falling_Edge])
										- (Mark_Encoder_Value[Slave][Rising_Edge] - Mark_Encoder_Value[Slave][Falling_Edge]);
								PrevError[Horizontal] = CurError[Horizontal];
								CurError[Horizontal] = My_Error[Horizontal] * Convert_PPR_To_mm;
							}
							else
							{
								My_Error_uS[Horizontal] = (Mark_TimeStamp_uS[Master_Horz][Rising_Edge] - Mark_TimeStamp_uS[Master_Horz][Falling_Edge])
										- (Mark_TimeStamp_uS[Slave][Rising_Edge] - Mark_TimeStamp_uS[Slave][Falling_Edge]);
								PrevError[Horizontal] 	= CurError[Horizontal];

								CurError[Horizontal] 	= My_Error_uS[Horizontal] * Config_Parameters[Alt_BagLength];
								CurError[Horizontal] 	= CurError[Horizontal] / value_microsecZ_avg;
							}
							CurError[Horizontal] = CurError[Horizontal] + Config_Float[AltF_Corr_Offset_Horz];
							Avg_Errors_Horizontal(PrevError[Horizontal],CurError[Horizontal]);
							CurErrorPositive_Avg[Horizontal] 	= CurError_Avg[Horizontal] < 0 ? CurError_Avg[Horizontal] * (-1) : CurError_Avg[Horizontal];

							Mark_M0_Height_Disp_Count = Mark_M0_Height_Diff_Count;
						}
						else
						{
							Flag_M0_Missing = 1;
							Mark_M0_Height_Disp_Count = 0;
							CurError[Horizontal] = 0;
							PrevError[Horizontal] = 0;
							CurError_Avg[Horizontal] = 0;
							CurErrorPositive_Avg[Horizontal] = 0;
							PrevError_Avg[Horizontal] = 0;
							Flag_New_Error_Arrived_Horizontal = 0;
							Disp_auto_average[Horizontal] = 0;
							Perform_Good_Copy_Horz_Stage = 0;
						}

						if(Flag_M1_Detected != 0)
						{
							Flag_M1_Missing = 0;

							if(Config_Parameters[Alt_Encoder_Time_Mode] == Encoder_Mode)
							{
								My_Error[Vertical] = Mark_Encoder_Value[Master_Vert][Config_Parameters[Alt_MarkType]]
																					 - Mark_Encoder_Value[Slave][Config_Parameters[Alt_MarkType]];
								if(My_Error[Vertical] < 0)
								{
									My_Error[Vertical] = My_Error[Vertical] * -1;
								}
								PrevError[Vertical] = CurError[Vertical];

								CurError[Vertical] 	= Distance_From_Slave_mm[Vertical] - (My_Error[Vertical] * Convert_PPR_To_mm);
							}
							else
							{
								My_Error_uS[Vertical] = Mark_TimeStamp_uS[Master_Vert][Config_Parameters[Alt_MarkType]] - Mark_TimeStamp_uS[Slave][Config_Parameters[Alt_MarkType]];
								if(My_Error_uS[Vertical] < 0)
								{
									My_Error_uS[Vertical] = My_Error_uS[Vertical] * -1;
								}
								PrevError[Vertical] = CurError[Vertical];

								CurError[Vertical] = My_Error_uS[Vertical] * Config_Parameters[Alt_BagLength];
								CurError[Vertical] = CurError[Vertical] / value_microsecZ_avg;
								CurError[Vertical] = Distance_From_Slave_mm[Vertical] - CurError[Vertical];
							}

							CurError[Vertical] = CurError[Vertical] + Config_Float[AltF_Corr_Offset_Vert];
							Avg_Errors_Vertical(PrevError[Vertical],CurError[Vertical]);
							CurErrorPositive_Avg[Vertical] 	= CurError_Avg[Vertical] < 0 ? CurError_Avg[Vertical] * (-1) : CurError_Avg[Vertical];

							Mark_M1_Height_Disp_Count = Mark_M1_Height_Diff_Count;
						}
						else
						{
							Flag_M1_Missing = 1;
							Mark_M1_Height_Disp_Count = 0;
							CurError[Vertical] = 0;
							PrevError[Vertical] = 0;
							CurError_Avg[Vertical] = 0;
							PrevError_Avg[Vertical] = 0;
							CurErrorPositive_Avg[Vertical] = 0;
							Flag_New_Error_Arrived_Vertical = 0;
							Disp_auto_average[Vertical] = 0;
							Perform_Good_Copy_Vert_Stage = 0;
						}
					}
					else
					{
						Flag_M2_Missing = 1;
						Mark_M2_Height_Disp_Count = 0;
						if(Flag_M0_Detected == 0)
						{
							Flag_M0_Missing = 1;
							Mark_M0_Height_Disp_Count = 0;
						}
						else
						{
							Flag_M0_Missing = 0;
							Mark_M0_Height_Disp_Count = Mark_M0_Height_Diff_Count;
						}

						if(Flag_M1_Detected == 0)
						{
							Flag_M1_Missing = 1;
							Mark_M1_Height_Disp_Count = 0;
						}
						else
						{
							Flag_M1_Missing = 0;
							Mark_M1_Height_Disp_Count = Mark_M1_Height_Diff_Count;
						}
						//Since M2 is missing, Error Calculations not done
						CurError[Vertical] = 0;
						CurErrorPositive_Avg[Vertical] = 0;
						PrevError[Vertical] = 0;
						CurError_Avg[Vertical] = 0;
						PrevError_Avg[Vertical] = 0;
						Flag_New_Error_Arrived_Vertical = 0;

						CurError[Horizontal] = 0;
						CurErrorPositive_Avg[Horizontal] = 0;
						PrevError[Horizontal] = 0;
						CurError_Avg[Horizontal] = 0;
						PrevError_Avg[Horizontal] = 0;
						Flag_New_Error_Arrived_Horizontal = 0;

						Disp_auto_average[Horizontal] = 0;
						Disp_auto_average[Vertical] = 0;

						Perform_Good_Copy_Horz_Stage = 0;
						Perform_Good_Copy_Vert_Stage = 0;
					}
				}

				//For Display Purpose since display interval and baglength interval can be different
				if(Screen_ID == Page_Diagnose)
				{
					Disp_Actual_Error[Vertical]   = CurError[Vertical];
					Disp_Actual_Error[Horizontal] = CurError[Horizontal];
				}
				else
				{
					Disp_Actual_Error[Vertical]   = CurError_Avg[Vertical];
					Disp_Actual_Error[Horizontal] = CurError_Avg[Horizontal];
				}
				//============================

				if((Flag_Serial_Debug_Enable != 0) && (Flag_Auto_Any == 0))
				{
					if(Config_Parameters[Alt_Encoder_Time_Mode] == Encoder_Mode)
					{
						sprintf(Debug_String,"MH:%u %u,MV: %u %u,SL: %u %u, MEH:%d, CEH:%f, CEA:%f, MEV:%d, CEV:%f, CEV:%f\n",
							Mark_Encoder_Value[Master_Horz][Rising_Edge],
							Mark_Encoder_Value[Master_Horz][Falling_Edge],
							Mark_Encoder_Value[Master_Vert][Rising_Edge],
							Mark_Encoder_Value[Master_Vert][Falling_Edge],
							Mark_Encoder_Value[Slave][Rising_Edge],
							Mark_Encoder_Value[Slave][Falling_Edge],
							My_Error[Horizontal],CurError[Horizontal],CurError_Avg[Horizontal],
							My_Error[Vertical],CurError[Vertical],CurError_Avg[Vertical]);
					}
					else
					{
						sprintf(Debug_String,"MH:%lu %lu,MV: %lu %lu,SL: %lu %lu,SP: %lu,SPA: %lu\n",
								Mark_TimeStamp_uS[Master_Horz][Rising_Edge],
								Mark_TimeStamp_uS[Master_Horz][Falling_Edge],
								Mark_TimeStamp_uS[Master_Vert][Rising_Edge],
								Mark_TimeStamp_uS[Master_Vert][Falling_Edge],
								Mark_TimeStamp_uS[Slave][Rising_Edge],
								Mark_TimeStamp_uS[Slave][Falling_Edge],
								value_microsecZ,value_microsecZ_avg);
					}
					Flag_Debug = 1;
				}

				Mark_Encoder_Value[Master_Horz][Rising_Edge] = 0;
				Mark_Encoder_Value[Master_Horz][Falling_Edge] = 0;
				Mark_Encoder_Value[Master_Vert][Rising_Edge] = 0;
				Mark_Encoder_Value[Master_Vert][Falling_Edge] = 0;
				Mark_Encoder_Value[Slave][Rising_Edge] = 0;
				Mark_Encoder_Value[Slave][Falling_Edge] = 0;

				Mark_TimeStamp_uS[Master_Horz][Rising_Edge] = 0;
				Mark_TimeStamp_uS[Master_Horz][Falling_Edge] = 0;
				Mark_TimeStamp_uS[Master_Vert][Rising_Edge] = 0;
				Mark_TimeStamp_uS[Master_Vert][Falling_Edge] = 0;
				Mark_TimeStamp_uS[Slave][Rising_Edge] = 0;
				Mark_TimeStamp_uS[Slave][Falling_Edge] = 0;

				My_Error[Vertical] = 0;
				My_Error[Horizontal] = 0;

				My_Error_uS[Vertical] = 0;
				My_Error_uS[Horizontal] = 0;

				Flag_M0_Detected = 0;
				Flag_M1_Detected = 0;
				Flag_M2_Detected = 0;
		}
	}

	if(htim->Instance == TIM15)
	{
		arc_execution_int_id = 5;

		HAL_GPIO_WritePin(DO_1_GPIO_Port, DO_1_Pin,Pulse_Level_Idle);		//Stop Upper Correction
		HAL_GPIO_WritePin(DO_2_GPIO_Port, DO_2_Pin,Pulse_Level_Idle);		//Stop Lower Correction

		if(Flag_Auto_Vertical == 0)
		{
			HAL_TIM_Base_Stop(&htim15);	//why not _IT
		}
	}

	if(htim->Instance == TIM16)
	{
		arc_execution_int_id = 7;

		HAL_GPIO_WritePin(DO_3_GPIO_Port, DO_3_Pin,Pulse_Level_Idle);		//Stop Upper Correction
		HAL_GPIO_WritePin(DO_4_GPIO_Port, DO_4_Pin,Pulse_Level_Idle);		//Stop Lower Correction

		if(Flag_Auto_Horizontal == 0)
		{
			HAL_TIM_Base_Stop(&htim16);	//why not _IT
		}
	}

	if(htim->Instance == TIM1)			//TIM1_
	{
		arc_execution_int_id = 9;

		Temp_Timer_Disp += Timer_OverFlow_ms;
		BLE_Temp_Timer_Disp += Timer_OverFlow_ms;
		Zero_Spd_Time 	+= Timer_OverFlow_ms;

		if(Flag_BLE_KeyPress_Event != 0)		//Key Press Received, But yet Key Release not received
		{
			Time_BLE_RepeatKeyPress_ms += Timer_OverFlow_ms;
		}

		Flag_Timer_5ms = 1;

		if(DMA_Tx_Stage == 2)
		{
			DMA_Tx_ms += Timer_OverFlow_ms;
			if(DMA_Tx_ms >=10)									//Delay between 2 send command to lcd
			{
				DMA_Tx_ms 		= 0;
				DMA_Tx_Stage 	= 0;
			}
		}
		alt_second_ms += Timer_OverFlow_ms;
		if(alt_second_ms >= 1000)
		{
			alt_second_ms = 0;
			Flag_Time_Second = 1;
		}
	}

	if (htim->Instance == TIM7)
	{
		HAL_IncTick();
	}

	arc_execution_int_id++;
}
//DMA
void Push_Data_DMA_Tx_Buffer(uint8_t *str_ptr, uint8_t str_len, _Bool Flag_str_end)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 45;

	uint8_t i,str_index;

	if(Flag_DMA_ON == 0)
	{
		if(Flag_LCD_Init_Error == 0)
		{
			HAL_UART_Transmit(&huart4,str_ptr,str_len,1000);
			Transmit_EndBytes();
		}
	}
	else
	{
		str_index = DMA_Tx_Length[DMA_Tx_Wr_Count];
		for(i = 0; i < str_len; i++,str_index++)
		{
			DMA_Tx_Buffer[DMA_Tx_Wr_Count][str_index] = str_ptr[i];
		}

		DMA_Tx_Length[DMA_Tx_Wr_Count] = str_index;

		if(Flag_str_end != 0)
		{
			for(i = 0; i < 4; i++,str_index++)
			{
				DMA_Tx_Buffer[DMA_Tx_Wr_Count][str_index] = EndBytes[i];
			}
			DMA_Tx_Length[DMA_Tx_Wr_Count] = str_index;

			DMA_Tx_Wr_Count++;
			if(DMA_Tx_Wr_Count == DMA_Tx_Buffer_length)
				DMA_Tx_Wr_Count = 0;
		}
	}
}

void Config_LCD_XY_Coordinates(_Bool Temp_Config)		//0 for xy(82), 1 for regular(83)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 46;

	if(Temp_Config == 0)
		Buffer_High_Priority_LCD_XY_Mode[7] = 0x82;
	else
		Buffer_High_Priority_LCD_XY_Mode[7] = 0x83;

	Flag_High_Priority_LCD_XY_Mode = 1;
}

void Seperate_Bytes_Int32(uint32_t Temp_Value,uint8_t *Int_Bytes)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 47;

	Int_Disp.di = Temp_Value;
	Int_Bytes[0] = Int_Disp.dc[3];
	Int_Bytes[1] = Int_Disp.dc[2];
	Int_Bytes[2] = Int_Disp.dc[1];
	Int_Bytes[3] = Int_Disp.dc[0];
}

void check_password(uint16_t temp_pw)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 48;

	if((temp_pw == Er_Password) || (temp_pw == Master_Password))
	{
		Screen_ID = Page_to_Proceed;
		Transmit_Page_ID(Screen_ID);
		Flag_Password_OK = 1;
		Flag_Spl_Rights = 1;

		if(temp_pw == Master_Password)
		{
			Flag_Master_Login = 1;
		}
		Format_LCD_Check = 0;
		Parameter_Default_Check = 0;
	}
	else if(temp_pw == Operator_Password)					//Operator Password
	{
		if(Page_to_Proceed == Page_Diagnose)
		{
			Screen_ID = Page_Diagnose;
			Transmit_Page_ID(Screen_ID);
		}
		Format_LCD_Check = 0;
		Parameter_Default_Check = 0;
	}
	else if(temp_pw == Format_LCD_Password[Format_LCD_Check])
	{
		Format_LCD_Check++;
		if(Format_LCD_Check >= 3)
		{
			Format_LCD();
			Format_LCD_Check = 0;
		}
		Parameter_Default_Check = 0;
	}
	else if(temp_pw == Parameter_Default_Password[Parameter_Default_Check])
	{
		Parameter_Default_Check++;
		if(Parameter_Default_Check >= 2)
		{
			Parameter_Default_Check = 0;
			if(Flag_Auto_Any == 0)
			{
				Transmit_Page_ID(Page_Parameter_Default_Confirm);
			}
		}
		Format_LCD_Check = 0;
	}
	else if(temp_pw == Spl_Task_Password)
	{
		  Program_Counter temp_pc;
		  temp_pc.FAULT = 0;
		  temp_pc.PC = 0;
		  Flash_Write_Buffer(Flash_Address_PC, (uint8_t*)&temp_pc, sizeof(temp_pc));
		  osDelay(10);
		  Flash_Read_Buffer(Flash_Address_PC, (uint8_t*)&err_pc, sizeof(err_pc));
		  sprintf(Debug_String, "EEPROM --> PC: 0x%08X \t ERROR HANDLER: %d\n", err_pc.PC, err_pc.FAULT);
		  Send_String_UART_232((uint8_t *)Debug_String);
	}
	else	//Wrong Password
	{
		Flag_Password_OK = 0;
		Screen_ID = Scr_Home;
		Transmit_Page_ID(Screen_ID);
		Flag_Spl_Rights = 0;
		Flag_Home = 1;
		Format_LCD_Check = 0;
		Parameter_Default_Check = 0;
	}
}

//======================================

void Format_LCD(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 49;

	uint8_t fg_array[]={0xAA,0xE2,0x55,0xAA,0x5A,0xA5};
	Push_Data_DMA_Tx_Buffer(fg_array,6,1);
}

uint16_t Get_Received_Data_Length(UART_HandleTypeDef *huart)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 50;

	uint16_t nb_rx_data = huart->RxXferSize - huart->RxXferCount;
	return(nb_rx_data);
}

void ADC_ChangeThreshold(uint32_t LowThreshold,uint32_t HighThreshold)
{
	HAL_ADC_Stop(&hadc1);
	__HAL_ADC_CLEAR_FLAG(&hadc1, ADC_FLAG_AWD1);
	ADC_AnalogWDGConfTypeDef AnalogWDGConfig = {0};
//	ADC_ChannelConfTypeDef sConfig = {0};
	/** Configure Analog WatchDog 1
	*/
	  AnalogWDGConfig.WatchdogNumber = ADC_ANALOGWATCHDOG_1;
	  AnalogWDGConfig.WatchdogMode = ADC_ANALOGWATCHDOG_SINGLE_REG;
	  AnalogWDGConfig.Channel = ADC_CHANNEL_2;
	  AnalogWDGConfig.ITMode = ENABLE;
	  AnalogWDGConfig.HighThreshold = HighThreshold;
	  AnalogWDGConfig.LowThreshold = LowThreshold;
	  if (HAL_ADC_AnalogWDGConfig(&hadc1, &AnalogWDGConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  HAL_ADC_Start(&hadc1);
}

void Clear_Timer_Interrupt(TIM_HandleTypeDef *htim)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 52;

	if(__HAL_TIM_GET_FLAG(htim, TIM_FLAG_UPDATE) != RESET)
	{
	  if (__HAL_TIM_GET_IT_SOURCE(htim, TIM_IT_UPDATE) != RESET)
	  {
		__HAL_TIM_CLEAR_IT(htim, TIM_IT_UPDATE);
	  }
	}
}

void Calculate_Multiplier_mm_to_pixel(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 53;

	multiplier_mm_to_pixel_move_icon = maximum_pixel_move_icon;
	multiplier_mm_to_pixel_move_icon = multiplier_mm_to_pixel_move_icon / maximum_mm_move_icon;
}

void Calculate_Position_Move_Icon_As_Per_Error(float error_horz, float error_vert)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 54;

	int16_t temp_position_x, temp_position_y;

	if(error_horz >= 0)
	{
		if(error_horz > 2.5)
			error_horz = 2.5;
	}
	else	//Negative
	{
		if(error_horz < -2.5)
			error_horz = -2.5;
	}

	if(error_vert >= 0)
	{
		if(error_vert > 2.5)
			error_vert = 2.5;
	}
	else	//Negative
	{
		if(error_vert < -2.5)
			error_vert = -2.5;
	}

	error_vert = error_vert * (-1);

	temp_position_x = error_horz * multiplier_mm_to_pixel_move_icon;
	temp_position_y = error_vert * multiplier_mm_to_pixel_move_icon;

	position_move_icon_horz_x = temp_position_x + position_move_icon_center_x - icon_half_width_pixel;
	position_move_icon_vert_y = temp_position_y + position_move_icon_center_y - icon_half_width_pixel;
}

//==================================================================

void LCD_Draw_Colour_Box(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 55;

	uint8_t clr_array[]={0xAA,0x82,0x00,0x08,0x00,0x00,0x07,0x00,0x05,0x00,0x01,0x55,0x55,0x55,0x55,0x33,0X33,0x33,0x33,0xCC,0xCC};

	clr_array[4] =	COLOUR_BOX_LCD_ADDRESS / 0x100;
	clr_array[5] =	COLOUR_BOX_LCD_ADDRESS % 0x100;

	clr_array[11] = COLOUR_BOX_START_X / 0x100;
	clr_array[12] = COLOUR_BOX_START_X % 0x100;
	clr_array[13] = COLOUR_BOX_START_Y / 0x100;
	clr_array[14] = COLOUR_BOX_START_Y % 0x100;

	clr_array[15] = COLOUR_BOX_END_X / 0x100;
	clr_array[16] = COLOUR_BOX_END_X % 0x100;
	clr_array[17] = COLOUR_BOX_END_Y / 0x100;
	clr_array[18] = COLOUR_BOX_END_Y % 0x100;

	Seperate_Bytes(Config_Parameters[Alt_Station_Colour],&(clr_array[19]),&(clr_array[20]));

	Push_Data_DMA_Tx_Buffer(clr_array,21,1);
}

void Calculate_Baseline_ADC3_DMA(void)
{
	if(bl_count != 0)
	{
		bl_average = bl_sum / bl_count;
	}

	if(Flag_Print_Mode == 0)
		bl_average = 0;

	Disp_bl_count = bl_count;
	Disp_Base_Line = bl_average;

	bl_sum = 0;
	bl_count = 0;
}
