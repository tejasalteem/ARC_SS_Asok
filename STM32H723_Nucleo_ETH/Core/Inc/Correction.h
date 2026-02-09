extern TIM_HandleTypeDef htim15;
extern char Debug_String[200];


void Common_Correction_Logic_Vertical(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 95;

//	if((Flag_GateSet == 0) || (Flag_Auto_Vertical == 0) || (Flag_New_Error_Arrived_Vertical == 0) || (Flag_BaseLine_Error != 0) || (Flag_Print_Mode == 0))
	if(
			(Flag_Auto_Vertical == 0)
			|| (Flag_New_Error_Arrived_Vertical == 0)
			|| (Stop_Controlling_if_Error_IsDecreasing(CurError_Avg[Vertical]) == 1)
	)
	{
		return;
	}

	Do_Correction_Vertical(CheckReverse_Correction_Vertical(CurError_Avg[Vertical],PrevError_Avg[Vertical]), CurError_Avg[Vertical], PrevError_Avg[Vertical]);
	PrevError_Avg[Vertical] = CurError_Avg[Vertical];
	Flag_New_Error_Arrived_Vertical = 0;

	if(CurErrorPositive_Avg[Vertical] > Config_Float[AltF_Error_Alarm_Vert])
	{
		Flag_Over_Error_Vert = 1;
	}
	else
	{
		Flag_Over_Error_Vert = 0;
	}
}

void Common_Correction_Logic_Horizontal(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 96;

//	if((Flag_GateSet == 0) || (Flag_Auto_Horizontal == 0) || (Flag_New_Error_Arrived_Horizontal == 0) || (Flag_BaseLine_Error != 0) || (Flag_Print_Mode == 0))
	if((Flag_Auto_Horizontal == 0) || (Flag_New_Error_Arrived_Horizontal == 0))
	{
		return;
	}
	Do_Correction_Horizontal(CheckReverse_Correction_Horizontal(CurError_Avg[Horizontal],PrevError_Avg[Horizontal]), CurError_Avg[Horizontal], PrevError_Avg[Horizontal]);
	PrevError_Avg[Horizontal] = CurError_Avg[Horizontal];
	Flag_New_Error_Arrived_Horizontal = 0;

	if(CurErrorPositive_Avg[Horizontal] > Config_Float[AltF_Error_Alarm_Horz])
	{
		Flag_Over_Error_Horz = 1;
	}
	else
	{
		Flag_Over_Error_Horz = 0;
	}
}

unsigned char CheckReverse_Correction_Vertical(float MyCurError , float MyPrevError)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 97;

	float Error_Correction =  (MyCurError < 0) ? MyCurError * -1 : MyCurError;
	float Prev_Error_Correction =  (MyPrevError < 0) ? MyPrevError * -1 : MyPrevError;
	static unsigned char revCount = 0;

	//when error switch from pos to neg or neg to pos
	if(((MyCurError<0) && (MyPrevError > 0)) || ((MyCurError>0) && (MyPrevError < 0)))
	{
			revCount = 0;
			return revCount;
	}

	if((Prev_Error_Correction - Error_Correction) >= Config_Float[AltF_Error_DGain_Vert])
	{
		revCount++;
	}
	else
	{
		revCount = 0;
	}
	return revCount;
}

unsigned char CheckReverse_Correction_Horizontal(float MyCurError , float MyPrevError)		//CHECKED OK 17.07.2021
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 98;

	float Error_Correction =  (MyCurError < 0) ? MyCurError * -1 : MyCurError;
	float Prev_Error_Correction =  (MyPrevError < 0) ? MyPrevError * -1 : MyPrevError;
	static unsigned char revCount = 0;

	//when error switch from pos to neg or neg to pos
	if(((MyCurError<0) && (MyPrevError > 0)) || ((MyCurError>0) && (MyPrevError < 0)))
	{
			revCount = 0;
			return revCount;
	}

	if((Prev_Error_Correction - Error_Correction) >= Config_Float[AltF_Error_DGain_Horz])
	{
		revCount++;
	}
	else
	{
		revCount = 0;
	}
	return revCount;
}

void Do_Correction_Vertical(unsigned char Reverse_Correction , float MyCurError , float MyPrevError)		//UPDATED 17.07.2021
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 99;

	_Bool Flag_UL_Corr = 0;
	uint16_t Current_Correction = 0;

	//Positive Value based on MuCurError
	float Error_Correction = (MyCurError < 0) ? MyCurError * -1 : MyCurError;

	if(Reverse_Correction == 0)
	{
		if((Error_Correction > Config_Float[AltF_Corr_Max_Error_Vert]) || (Error_Correction < Config_Float[AltF_Corr_Min_Error_Vert]))
		{
			Stop_Digital_Correction_Vertical();
			if(Flag_Serial_Debug_Enable != 0)
			{
				sprintf(Debug_String,"VERT: RET_ErRange, Emm: %.3f\n",Error_Correction);
				Flag_Debug = 1;
			}
			return;
		}
	}
	Current_Correction = Error_Correction * Config_Float[AltF_Corr_PGain_Vert];
	Debug_Flag_Gain_Used = PGAIN;

	if(Reverse_Correction > 0)
	{
		if(Error_Correction > 2)			//If error > 2, do not perform any correction
		{
			if(Flag_Serial_Debug_Enable != 0)
			{
				sprintf(Debug_String,"VERT: RET_R, Emm: %.3f\n",Error_Correction);
				Flag_Debug = 1;
			}
			return;
		}
		Error_Correction = (MyPrevError < 0 ) ? MyPrevError * -1 : MyPrevError;
		Current_Correction = Error_Correction * Config_Float[AltF_Corr_DGain_Vert];
		Debug_Flag_Gain_Used = DGAIN;
	}
	//Value in Current Correction either based on CurError or PrevError

	if(Current_Correction <= Config_Parameters[Alt_Corr_Min_Vert])
	{
		if(Reverse_Correction == 0)
		{
			Current_Correction = Config_Parameters[Alt_Corr_Min_Vert];
		}
		else
		{
			Current_Correction = Config_Parameters[Alt_Corr_Min_Vert] * 0.75;
		}
	}
	else if(Current_Correction >= Config_Parameters[Alt_Corr_Max_Vert])
	{
		Current_Correction = Config_Parameters[Alt_Corr_Max_Vert];
	}

//===================================================================

	if(Config_Parameters[Alt_Correction_Direction_Vert] != 0)
	{
		if(Reverse_Correction == 0)   //Reverse Correction if required
		{
			if(MyCurError > 0)
			{
				Flag_UL_Corr = Pulse_Upper;
			}
			else
			{
				Flag_UL_Corr = Pulse_Lower;
			}
		}
		else
		{
			if(MyCurError > 0)
			{
				Flag_UL_Corr = Pulse_Lower;
			}
			else
			{
				Flag_UL_Corr = Pulse_Upper;
			}
		}
	}
	else
	{
		if(Reverse_Correction == 0)
		{
			if(MyCurError > 0)
			{
				Flag_UL_Corr = Pulse_Lower;
			}
			else
			{
				Flag_UL_Corr = Pulse_Upper;
			}
		}
		else
		{
			if(MyCurError > 0)
			{
				Flag_UL_Corr = Pulse_Upper;
			}
			else
			{
				Flag_UL_Corr = Pulse_Lower;
			}
		}
	}
	Digital_Correction_Vertical(Flag_UL_Corr,Current_Correction);
}

void Do_Correction_Horizontal(unsigned char Reverse_Correction , float MyCurError , float MyPrevError)		//UPDATED 17.07.2021
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 100;

	_Bool Flag_UL_Corr = 0;

	uint16_t Current_Correction = 0;

	//Positive Value based on MuCurError
	float Error_Correction = (MyCurError < 0) ? MyCurError * -1 : MyCurError;

	if(Reverse_Correction == 0)
	{
		if((Error_Correction > Config_Float[AltF_Corr_Max_Error_Horz]) || (Error_Correction < Config_Float[AltF_Corr_Min_Error_Horz]))
		{
			Stop_Digital_Correction_Horizontal();
			if(Flag_Serial_Debug_Enable != 0)
			{
				sprintf(Debug_String,"HORZ: RET_ErRange, Emm: %.3f\n",Error_Correction);
				Flag_Debug = 1;
			}
			return;
		}
	}

	Current_Correction = Error_Correction * Config_Float[AltF_Corr_PGain_Horz];

	if(Reverse_Correction > 0)
	{
		if(Error_Correction > 2)			//If error > 15, do not perform any correction
		{
			if(Flag_Serial_Debug_Enable != 0)
			{
				sprintf(Debug_String,"HORZ: RET_R, Emm: %.3f\n",Error_Correction);
				Flag_Debug = 1;
			}
			return;
		}
		Error_Correction = (MyPrevError < 0 ) ? MyPrevError * -1 : MyPrevError;				//Positive value based on PrevError

		Current_Correction = Error_Correction * Config_Float[AltF_Corr_DGain_Horz];
	}
	//Value in Current Correction either based on CurError or PrevError
	if(Current_Correction <= Config_Parameters[Alt_Corr_Min_Horz])
	{
		if(Reverse_Correction == 0)
		{
			Current_Correction = Config_Parameters[Alt_Corr_Min_Horz];
		}
		else
		{
			Current_Correction = Config_Parameters[Alt_Corr_Min_Horz] * 0.75;
		}
	}
	else if(Current_Correction >= Config_Parameters[Alt_Corr_Max_Horz])
	{
		Current_Correction = Config_Parameters[Alt_Corr_Max_Horz];
	}

	//======================================================================

	if(Config_Parameters[Alt_Correction_Direction_Horz] != 0)
	{
		if(Reverse_Correction == 0)   //Reverse Correction if required
		{
			if(MyCurError > 0)
			{
				Flag_UL_Corr = Pulse_Upper;
			}
			else
			{
				Flag_UL_Corr = Pulse_Lower;
			}
		}
		else
		{
			if(MyCurError > 0)
			{
				Flag_UL_Corr = Pulse_Lower;
			}
			else
			{
				Flag_UL_Corr = Pulse_Upper;
			}
		}
	}
	else
	{
		if(Reverse_Correction == 0)
		{
			if(MyCurError > 0)
			{
				Flag_UL_Corr = Pulse_Lower;
			}
			else
			{
				Flag_UL_Corr = Pulse_Upper;
			}
		}
		else
		{
			if(MyCurError > 0)
			{
				Flag_UL_Corr = Pulse_Upper;
			}
			else
			{
				Flag_UL_Corr = Pulse_Lower;
			}
		}
	}
	Digital_Correction_Horizontal(Flag_UL_Corr,Current_Correction);
}

void Digital_Correction_Vertical(_Bool Flag_Temp_Pulse_Type, uint16_t MyCurError)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 101;

	Correction_ms_Vert = MyCurError;
	MyCurError = MyCurError * 10;	//1 timer tick is of 100 us, so to convert ms into timer tick, it requires to mul by 10

	if(MyCurError == 0)
	{
		if(Flag_Serial_Debug_Enable != 0)
		{
			sprintf(Debug_String,"VERT: RET_Zero, Cms: %d\n",MyCurError);
			Flag_Debug = 1;
		}
		return;
	}

	if(MyCurError > 60000)
	{
		MyCurError = 60000;
		Correction_ms_Vert = 6000;
	}
	__HAL_TIM_SET_AUTORELOAD(&htim15, MyCurError);
	__HAL_TIM_SET_COUNTER(&htim15,0);

	if(Flag_Start_Timer_Vertical != 0)
	{
		HAL_TIM_Base_Start_IT(&htim15);
	}

	if(Flag_Temp_Pulse_Type == Pulse_Upper)
	{
		HAL_GPIO_WritePin(DO_2_GPIO_Port, DO_2_Pin,Pulse_Level_Idle);
		HAL_GPIO_WritePin(DO_1_GPIO_Port, DO_1_Pin,Pulse_Level_Active);
		if(Flag_Serial_Debug_Enable != 0)
		{
			sprintf(Debug_String,"VERT: CE: %+.3f, PE: %+.3f, CR_U_%c, Cms: %d\n",CurError_Avg[Vertical],PrevError_Avg[Vertical],Debug_Gain_Used[Debug_Flag_Gain_Used],MyCurError);
			Flag_Debug = 1;
		}
		Flag_Correction_Vert_Upper = 1;
	}
	else
	{
		HAL_GPIO_WritePin(DO_1_GPIO_Port, DO_1_Pin,Pulse_Level_Idle);
		HAL_GPIO_WritePin(DO_2_GPIO_Port, DO_2_Pin,Pulse_Level_Active);
		if(Flag_Serial_Debug_Enable != 0)
		{
			sprintf(Debug_String,"VERT: CE: %+.3f, PE: %+.3f, CR_L_%c, Cms: %d\n",CurError_Avg[Vertical],PrevError_Avg[Vertical],Debug_Gain_Used[Debug_Flag_Gain_Used],MyCurError);
			Flag_Debug = 1;
		}
		Correction_ms_Vert = Correction_ms_Vert * (-1);
		Flag_Correction_Vert_Lower = 1;
	}
}

void Digital_Correction_Horizontal(_Bool Flag_Temp_Pulse_Type, uint16_t MyCurError)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 102;

	Correction_ms_Horz = MyCurError;

	MyCurError = MyCurError * 10;	//1 timer tick is of 100 us, so to convert ms into timer tick, it requires to mul by 10

	if(MyCurError == 0)
	{
		if(Flag_Serial_Debug_Enable != 0)
		{
			sprintf(Debug_String,"HORZ: RET_Zero, Cms: %d\n",MyCurError);
			Flag_Debug = 1;
		}
		return;
	}

	if(MyCurError > 60000)
	{
		MyCurError = 60000;
		Correction_ms_Horz = 6000;
	}
	__HAL_TIM_SET_AUTORELOAD(&htim16, MyCurError);
	__HAL_TIM_SET_COUNTER(&htim16,0);

	if(Flag_Start_Timer_Horizontal != 0)
	{
		HAL_TIM_Base_Start_IT(&htim16);
	}

	if(Flag_Temp_Pulse_Type == Pulse_Upper)
	{
		HAL_GPIO_WritePin(DO_4_GPIO_Port, DO_4_Pin,Pulse_Level_Idle);
		HAL_GPIO_WritePin(DO_3_GPIO_Port, DO_3_Pin,Pulse_Level_Active);

		if(Flag_Serial_Debug_Enable != 0)
		{
			sprintf(Debug_String,"HORZ: CE: %+.3f, PE: %+.3f, CR_U_%c, Cms: %d\n",CurError_Avg[Horizontal],PrevError_Avg[Horizontal],Debug_Gain_Used[Debug_Flag_Gain_Used],MyCurError);
			Flag_Debug = 1;
		}
		Flag_Correction_Horz_Upper = 1;
	}
	else
	{
		HAL_GPIO_WritePin(DO_3_GPIO_Port, DO_3_Pin,Pulse_Level_Idle);
		HAL_GPIO_WritePin(DO_4_GPIO_Port, DO_4_Pin,Pulse_Level_Active);
		if(Flag_Serial_Debug_Enable != 0)
		{
			sprintf(Debug_String,"HORZ: CE: %+.3f, PE: %+.3f, CR_L_%c, Cms: %d\n",CurError_Avg[Horizontal],PrevError_Avg[Horizontal],Debug_Gain_Used[Debug_Flag_Gain_Used],MyCurError);
			Flag_Debug = 1;
		}
		Correction_ms_Horz = Correction_ms_Horz * (-1);
		Flag_Correction_Horz_Lower = 1;
	}
}

void Stop_Digital_Correction_Vertical(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 103;

	HAL_GPIO_WritePin(DO_1_GPIO_Port, DO_1_Pin,Pulse_Level_Idle);
	HAL_GPIO_WritePin(DO_2_GPIO_Port, DO_2_Pin,Pulse_Level_Idle);
}

void Stop_Digital_Correction_Horizontal(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 104;

	HAL_GPIO_WritePin(DO_3_GPIO_Port, DO_3_Pin,Pulse_Level_Idle);
	HAL_GPIO_WritePin(DO_4_GPIO_Port, DO_4_Pin,Pulse_Level_Idle);
}

void Action_LCD_Commands(void)			//CHECKED OK 17.07.2021
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 105;

	if(Flag_GenKeyPressed != 0)
	{
		if(Flag_TouchKeyPressed == 0)
		{
			Flag_GenKeyPressed = 0;
			rec_function_byte = 0;
		}
	}

   if(rec_function_byte == 'L')  		// Reverse Gate Movement
   {
	   	if(Flag_GenKeyPressed == 0)
		{
			Flag_GenKeyPressed = 1;
			Flag_TouchKeyPressed = 1;
			Flag_GateSet = 0;
		}

		if(Flag_Timer_5ms != 0)
		{
			if(Position_Gate[Slave] > Gate_Graph_Min_Valid_Value_Slave_Count)
			{
				Position_Gate[Master_Horz] 	-= 1;
				Position_Gate[Master_Vert] 	-= 1;
				Position_Gate[Slave] 		-= 1;
				Gate_Calculations();
			}
			Flag_Timer_5ms = 0;
		}
	}
   else if(rec_function_byte == 'R')  //Right Gate Movement
   {
		if(Flag_GenKeyPressed == 0)
		{
			Flag_GenKeyPressed = 1;
			Flag_TouchKeyPressed = 1;

			Flag_GateSet = 0;
		}

		if(Flag_Timer_5ms != 0)
		{
			if(Position_Gate[Slave] < Gate_Graph_Max_Valid_Value_Slave_Count)
			{
				Position_Gate[Master_Horz] += 1;
				Position_Gate[Master_Vert] += 1;
				Position_Gate[Slave] += 1;
				Gate_Calculations();
			}
			Flag_Timer_5ms = 0;
		}
	}

   	else if(rec_function_byte == 'l')	//Fine tune to Reverse Gate movement
	{
		if(Flag_GenKeyPressed == 0)
		{
			Flag_GateSet = 0;

			if(Position_Gate[Slave] > Gate_Graph_Min_Valid_Value_Slave_Count)
			{
				Position_Gate[Master_Horz] -= 1;
				Position_Gate[Master_Vert] -= 1;
				Position_Gate[Slave] -= 1;
				Gate_Calculations();
			}
		}
		rec_function_byte = 0;
		Flag_Fine_Left_Key = 1;
	}

   	else if(rec_function_byte == 'r')
	{
		if(Flag_GenKeyPressed == 0)
		{
			Flag_GateSet = 0;

			if(Position_Gate[Slave] < Gate_Graph_Max_Valid_Value_Slave_Count)
			{
				Position_Gate[Master_Horz] += 1;
				Position_Gate[Master_Vert] += 1;
				Position_Gate[Slave] += 1;
				Gate_Calculations();
			}
		}
		rec_function_byte = 0;
		Flag_Fine_Right_Key = 1;
	}
}

void Action_LCD_Commands_Home(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 106;

	if(Flag_GenKeyPressed != 0)
	{
		if(Flag_TouchKeyPressed == 0)
		{
			Flag_GenKeyPressed = 0;
			rec_function_byte = 0;
		}
	}

   if(rec_function_byte == 'L')  		// Reverse Gate Movement
   {
	   	if(Flag_GenKeyPressed == 0)
		{
			Flag_GenKeyPressed = 1;
			Flag_TouchKeyPressed = 1;
			Flag_GateSet = 0;
		}

		if(Flag_Timer_5ms != 0)
		{
			if(Position_Gate[Slave] > Gate_Graph_Min_Valid_Value_Slave_Count)
			{
				Position_Gate[Slave] -= 1;
				Gate_Calculations_Slave();
			}
			Flag_Timer_5ms = 0;
		}
	}
   else if(rec_function_byte == 'R')  //Right Gate Movement
   {
		if(Flag_GenKeyPressed == 0)
		{
			Flag_GenKeyPressed = 1;
			Flag_TouchKeyPressed = 1;
			Flag_GateSet = 0;
		}

		if(Flag_Timer_5ms != 0)
		{
			if(Position_Gate[Slave] < Gate_Graph_Max_Valid_Value_Slave_Count)
			{
				Position_Gate[Slave] += 1;
				Gate_Calculations_Slave();
			}
			Flag_Timer_5ms = 0;
		}
	}

   	else if(rec_function_byte == 'l')	//Fine tune to Reverse Gate movement
	{
		if(Flag_GenKeyPressed == 0)
		{
			Flag_GateSet = 0;

			if(Position_Gate[Slave] > Gate_Graph_Min_Valid_Value_Slave_Count)
			{
				Position_Gate[Slave] -= 1;
				Gate_Calculations_Slave();
			}
		}
		rec_function_byte = 0;
		Flag_Fine_Left_Key = 1;
	}

   	else if(rec_function_byte == 'r')
	{
		if(Flag_GenKeyPressed == 0)
		{
			Flag_GateSet = 0;

			if(Position_Gate[Slave] < Gate_Graph_Max_Valid_Value_Slave_Count)
			{
				Position_Gate[Slave] += 1;
				Gate_Calculations_Slave();
			}
		}
		rec_function_byte = 0;
		Flag_Fine_Right_Key = 1;
	}
}

