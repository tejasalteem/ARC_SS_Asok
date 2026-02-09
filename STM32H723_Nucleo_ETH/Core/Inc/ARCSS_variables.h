																	//Min			Max			Default
#include <stdio.h>

#define Alt_Station_ID			0
#define Alt_Station_Colour		1
#define Alt_BagLength 			2
#define Alt_GateWidth			3
#define Alt_Index_mm			4

#define Alt_Master_ID_Horz		5
#define Alt_Master_ID_Vert		6

//Recipe related int16 data
#define Alt_Corr_Repeat_Horz	7
#define Alt_Corr_Repeat_Vert	8

#define Alt_Corr_Max_Horz       9
#define Alt_Corr_Max_Vert     	10

#define Alt_Corr_Min_Horz		11
#define Alt_Corr_Min_Vert		12

#define Alt_Auto_Averaging_Horz	13
#define Alt_Auto_Averaging_Vert	14

#define Alt_Correction_Direction_Horz	15
#define Alt_Correction_Direction_Vert	16

#define Alt_Home_Mode			17
#define Alt_Time_Average		18
#define Alt_Use_With_PC			19

//=================================

#define Alt_MinMarkHeight		20
#define Alt_MaxMarkHeight		21
#define Alt_ValidBaseLine		22

#define Alt_Encoder_Time_Mode	23

#define Alt_MasterMarkPosition	24
#define Alt_MarkType			25

#define Alt_Temp_Password		26
#define Alt_Position_Gate_Slave_GCount	27

#define Alt_IP_Address_Byte3	28
#define Alt_IP_Address_Byte2	29
#define Alt_IP_Address_Byte1	30
#define Alt_IP_Address_Byte0	31

#define Alt_Server_Address_Byte3	32
#define Alt_Server_Address_Byte2	33
#define Alt_Server_Address_Byte1	34
#define Alt_Server_Address_Byte0	35

#define Alt_Gateway_Address_Byte3	36
#define Alt_Gateway_Address_Byte2	37
#define Alt_Gateway_Address_Byte1	38
#define Alt_Gateway_Address_Byte0	39

#define Alt_Sensor_Gain				40
#define Alt_Valid_BL_Window_Lower	41

#define Alt_Machine_Direction		42
#define Alt_Print_Mode				43
#define Alt_Mem_Read_OK				44

//==========================================//

#define AltF_Corr_Offset_Horz		0
#define AltF_Corr_Offset_Vert		1

#define AltF_Corr_PGain_Horz		2
#define AltF_Corr_PGain_Vert		3

#define AltF_Corr_DGain_Horz		4
#define AltF_Corr_DGain_Vert		5

#define AltF_Corr_Min_Error_Horz	6
#define AltF_Corr_Min_Error_Vert	7

#define AltF_Corr_Max_Error_Horz	8
#define AltF_Corr_Max_Error_Vert	9

#define AltF_Error_DGain_Horz		10
#define AltF_Error_DGain_Vert		11

#define AltF_Error_Alarm_Horz		12
#define AltF_Error_Alarm_Vert		13

#define AltF_SetValue				14

#define AltF_Error_Filter_Horz		15
#define AltF_Error_Filter_Vert		16

#define AltF_BaseLine_Error_Alarm_Limit 17
#define AltF_Min_Control_Speed		18

//18 reserved for config parameters

#define AltF_Disp_MarkHeight_Horz	19

#define AltF_Disp_Error_Horz		20
#define AltF_Disp_Error_Vert		21

#define AltF_Disp_Production		22

#define AltF_Disp_SetValue_Horz		23
#define AltF_Disp_SetValue_Vert		24

#define AltF_Disp_SetValue_Plus_OffSetV	25

//====================================

#define No_Of_Config_Parameters	45
#define	No_Of_Config_Float		19

int16_t Config_Parameters[No_Of_Config_Parameters];
float Config_Float[26];

float Config_Float_Limits[No_Of_Config_Float][3] =
{
	{-8.0,	8.0,	0.0},		//AltF_Corr_Offset_Horz
	{-10.0,	10.0,	0.0},		//AltF_Corr_Offset_Vert

	{0.0,	3000.0,	500.0},		//AltF_PGain_Horz
	{0.0,	3000.0,	500.0},		//AltF_PGain_Vert

	{0.0,	3000.0,	350.0},		//AltF_DGain_Horz
	{0.0,	3000.0,	350.0},		//AltF_DGain_Vert

	{0.01,	2.0,	0.02},		//AltF_Corr_Min_Error_Horz 		//Must be < AltF_Corr_Max_Error
	{0.01,	2.0,	0.02},		//AltF_Corr_Min_Error_Vert		//Must be < AltF_Corr_Max_Error_Vert

	{0.01,	10.0,	5.0},		//AltF_Corr_Max_Error_Horz		//Must be > AltF_Corr_Min_Error
	{0.01,	10.0,	5.0},		//AltF_Corr_Max_Error_Vert		//Must be > AltF_Corr_Min_Error_Vert

	{0.01,	10.0,	0.2},		//AltF_Error_DGain_Horz
	{0.01,	10.0,	0.2},		//AltF_Error_DGain_Vert

	{0.01,	3.0,	1.0},		//AltF_Error_Alarm_Horz
	{0.01,	3.0,	1.0},		//AltF_Error_Alarm_Vert

	{0.0,	2000.0,	20.0},		//AltF_SetValue, Should be Higher than Gatewidth

	{0.01,	10.0,	0.2},		//AltF_Error_Filter_Horz
	{0.01,	10.0,	0.2},		//AltF_Error_Filter_Vert

	{0.0,	4095.0,	2048.0},	//AltF_BaseLine_Error_Alarm_Limit
	{0.0,	100.0,	10.0}		//AltF_Min_Control_Speed
};

int16_t Config_Para_Limits[45][3] = 	//uint16 to int16
{
	//min, max, default
	{2,		11, 	2},		//0.Alt_Station_ID

	{-32768, 32767, 0},		//1.Alt_Station_Colour
	{300,	2000, 	500},	//2.Alt_BagLength		//
	{10,	40, 	20},	//3.Alt_GateWidth		//Should be Less than SetValue
							//set value for ABR ( For ARC it is changed in main during initialization )
	{0,		2000, 	 0},	//4.Alt_Index_mm
	{1,		10, 	 1},	//5.Alt_Master_ID_Horz
	{1,		10, 	 1},	//6.Alt_Master_ID_Vert

	{1,		50, 	4},		//7.Alt_Corr_Repeat_Horz
	{1,		50, 	3},		//8.Alt_Corr_Repeat_Vert

	{1,		6000, 	1000},	//9.Alt_Corr_Max_Horz		//Must be > Alt_Corr_Min
	{1,		6000, 	1000},	//10.Alt_Corr_Max_Vert		//Must be > Alt_Corr_Min_Neg

	{1,		6000, 	30},	//11.Alt_Corr_Min_Horz		//Must be < Alt_Corr_Max
	{1,		6000, 	30},	//12.Alt_Corr_Min_Vert		//Must be < Alt_Corr_Max_Neg

//=================================================================

	{0,		1, 		0},		//13.Alt_Auto_Averaging_Horz
	{0,		1, 		0},		//14.Alt_Auto_Averaging_Vert

	{0,		1, 		0},		//15.Alt_Correction_Direction_Horz
	{0,		1, 		0},		//16.Alt_Correction_Direction_Vert

	{0,		1, 		0},		//17.Alt_Home
	{0,		10, 	3},		//18.Alt_Time_Average
	{0,		1,  	1},		//19.Alt_Use_With_PC

//================================================================

	{1,		8, 		2},		//20.Alt_MinMarkHeight
	{2,		30, 	8},		//21.Alt_MaxMarkHeight
	{1,		4095, 	250},	//22.Alt_ValidBaseLine		//2048

	{0,		1,    	0},		//23.Alt_Encoder_Time_Mode
	{0,		1, 		0},		//24.Alt_MasterMarkPosition
	{0,		1, 		0},		//25.Alt_MarkType

	{0,		32767,  32767},	//26.Alt_Temp_Password
	{0,		799,	400},	//27.Alt_Position_Gate_Slave_GCount

	{0,		255,	192},	//28.Alt_IP_Address_Byte3
	{0,		255,	168},	//29.Alt_IP_Address_Byte2
	{0,		255,	0},		//30.Alt_IP_Address_Byte1
	{0,		255,	95},	//31.Alt_IP_Address_Byte0

	{0,		255,	192},	//32.Alt_Server_Address_Byte3
	{0,		255,	168},	//33.Alt_Server_Address_Byte2
	{0,		255,	0},		//34.Alt_Server_Address_Byte1
	{0,		255,	16},	//35.Alt_Server_Address_Byte0

	{0,		255,	192},	//36.Alt_Gateway_Address_Byte3
	{0,		255,	168},	//37.Alt_Gateway_Address_Byte2
	{0,		255,	0},		//38.Alt_Gateway_Address_Byte1
	{0,		255,	1},		//39.Alt_Gateway_Address_Byte0

	{0,		4095,	4000},	//40.Alt_Sensor_Gain
	{0,		4095,	50},	//41.Alt_Valid_BL_Window_Lower

	{0,		1, 		0},		//42.Alt_Machine_Direction
	{0,		1, 		1},		//43.Alt_Print_Mode
	{1946,	1948,	1947}	//44.Alt_Mem_Read_OK
	};

#define Alt_Encoder_Count 	20000		//Max Encoder Count Value

#define Falling_Edge 1
#define Rising_Edge  0

/****** LCD Defaults*******/
#define LCD_XY_Key_Released		0x72
#define LCD_Value_Received    	0x77
#define LCD_Key_Up            	0x78
#define LCD_Key_Pressed       	0x79

#define LCD_Data_Length_Event 		9
#define LCD_Data_Length_XY_Release 	10
#define LCD_Data_Length_Value 		12
#define LCD_Data_Length_Float 		14
#define LCD_Data_Length_LCD_Init 	23

//buttons touch id
#define LCD_Btn_Auto_Horz			0
#define LCD_Btn_Home 				1
#define LCD_Btn_Diagnose 			2
#define LCD_Btn_Graph 				3
#define LCD_Btn_Settings 			4

#define LCD_BtnG_LeftFine 			5
#define LCD_BtnG_RightFine			6
#define LCD_BtnG_Left 				7
#define LCD_BtnG_Right 				8
#define LCD_BtnG_GateSet			9

#define LCD_Btn_AutoSearch			10

#define LCD_Btn_Alarm				11

#define LCD_Btn_Print_Mode			12

#define LCD_Btn_Confirm_Yes			13
#define LCD_Btn_Confirm_No			14

#define LCD_Btn_Auto_Vert			15

//#define LCD_Btn_Alarm_Clear		16

#define LCD_Btn_Machine_Direction	17
#define LCD_Btn_Use_With_PC			18

#define LCD_Btn_Clear_Production	19

#define LCD_Btn_Correction_Rev_Horz		20
#define LCD_Btn_Correction_Rev_Vert		21

#define LCD_Btn_MasterMarkPosition		22
#define LCD_Btn_MarkType				23

#define LCD_Btn_Recipe					24
#define LCD_Btn_Recipe_Edit				25

#define LCD_Btn_HorzF_Plus_Right		26
#define LCD_Btn_HorzF_Minus_Left		27

#define LCD_Btn_VertF_Plus_Up			28
#define LCD_Btn_VertF_Minus_Dn			29

#define LCD_Btn_HorzC_Plus_Right		30
#define LCD_Btn_HorzC_Minus_Left		31

#define LCD_Btn_VertC_Plus_Up			32
#define LCD_Btn_VertC_Minus_Dn			33

#define LCD_Btn_Serial_Debug_Enable		34
#define LCD_Btn_Encoder_Time_Mode		35

#define LCD_Btn_Auto_Averaging_Horz		36
#define LCD_Btn_Auto_Averaging_Vert		37

#define LCD_Btn_Home_Mode				38

#define LCD_Btn_Good_Copy_Horz_TCP		39
#define LCD_Btn_Good_Copy_Vert_TCP		40

#define LCD_Btn_Good_Copy_Horz			139
#define LCD_Btn_Good_Copy_Vert			140

#define LCD_Btn_Confirm_Good_Copy		101

_Bool Flag_Perform_Good_Copy = 0;

uint8_t Scr_Home,Scr_Good_Copy;

//Default Page Values of ALTIMATCH
uint16_t Page_Home 			= 1;
uint16_t Page_Graph 		= 3;
uint16_t Page_Password 		= 4;
uint16_t Page_Setting 		= 5;
uint16_t Page_Recipe_Edit 	= 6;
uint16_t Page_Recipe_View 	= 7;
uint16_t Page_IP_Settings 	= 8;
uint16_t Page_Diagnose 		= 9;
uint16_t Page_Good_Copy 	= 10;
uint16_t Page_Error 		= 12;

//LCD PAGE-ID
#define Page_Home_PP				18
#define Page_Graph_PP				20
#define Page_Password_PP			21
#define Page_Setting_PP				22
#define Page_Recipe_Edit_PP			23
#define Page_Recipe_View_PP			24
#define Page_IP_Settings_PP			25
#define Page_Diagnose_PP			26
#define Page_Good_Copy_PP			27
#define	Page_Error_PP				29

#define Page_Parameter_Default_Confirm	13
#define Page_Parameter_Default			14
#define Page_OTA						15

uint8_t Screen_ID;

uint16_t Disp_Encoder_Count 	= 0;
uint16_t Disp_Base_Line 		= 0;
uint16_t Disp_Valid_Mark 		= 0;

_Bool Flag_GateSet 				= 0;
_Bool Flag_BagLength 			= 0;
float Disp_Actual_Error[2] 	= {0,0};

uint16_t mpm_int;

#define size_buffer_lcd		806		//4 start + 800 data + 2 end
#define pixels_graph_lcd	800		//4 start + 480 data + 2 end	06.01.2025

#define size_buffer_ble	827			//3 start + 800 data + 1 end = 804
									//Additional 23 int16 data - 1 starting address, 22 display data	//804 + 23 = 827

#define buffer_ble_Graph_Disp_Data_Start_Index	803

uint16_t Total_Baglength_Count = 0;
uint32_t Actual_Meter_Count = 0;

int16_t BagLength_Count = 0;
int Diff_BagLength = 0;
int Prev_Master_Count = 0;
uint16_t GateWidth_Graph_Count = 0;
uint16_t GateWidth_PPR = 0;
int16_t Distance_From_Slave_PPR[2] = {0,0};
int16_t SetValue_Graph_Count[2] = {0,0};

int16_t My_Error[2] = {0,0};
int32_t My_Error_uS[2] = {0,0};
uint16_t Valid_Mark_Count;

_Bool Mark_Found = 0;

_Bool Flag_BagLength_Arrived = 0;
_Bool Flag_Z_Received = 0;

int16_t Count_lcd = 0;
uint16_t Buffer_Start_Position = 0;
uint8_t Skip_Limit ;
_Bool Flag_Buffer_RD = 0,Flag_Buffer_WR = 0;
_Bool Flag_Write_LCD_Buffer = 0;

/* Gate and Error calculations*/

uint16_t Position_Gate[3];

uint16_t Gate_Start[3];
uint16_t Gate_End[3];
uint16_t Gate_Start_ppr[3];
uint16_t Gate_End_ppr[3];
char Flag_Gate_Start = 0;

#define Master_Horz		0
#define Master_Vert		1
#define Slave			2

uint16_t Gate_Graph_Min_Valid_Value_Slave_Count,Gate_Graph_Max_Valid_Value_Slave_Count;	//,Graph_Set_Value;
uint16_t Gate_Graph_Min_Valid_Value_Slave_PPR,Gate_Graph_Max_Valid_Value_Slave_PPR;

#define Gate_Graph_Default_Value	400

uint16_t Mark_Position[2];
uint16_t Prev_Mark_Position[2];

_Bool Flag_Debug = 0;
_Bool Flag_System_Start = 0;
_Bool Flag_Init_Calc_Done = 0;

_Bool Flag_New_Error_Arrived_Vertical = 0;
_Bool Flag_New_Error_Arrived_Horizontal = 0;

float CurError[2] = {0,0};
float PrevError[2] =  {0,0};

float CurError_Avg[2] = {0,0};
float PrevError_Avg[2] = {0,0};

float CurErrorPositive_Avg[2] = {0,0};

short int Repeat_Length[2] = {0,0};
uint16_t Min_Error_PPR[2] = {0,0};
uint16_t Max_Error_PPR[2] = {0,0};

float Convert_mm_To_PPR = 0;
float Convert_PPR_To_mm = 0;

uint16_t Temp_Timer_Disp = 0;

_Bool Flag_Auto_Horizontal = 0;
_Bool Flag_Auto_Vertical = 0;
_Bool Flag_Auto_Any = 0;

_Bool Flag_Fine_Left_Key = 0;
_Bool Flag_Fine_Right_Key = 0;

uint16_t retry_count = 0;

float Error_Green_Upper_Limit = 1.0;
float Error_Orange_Upper_Limit = 5.0;
_Bool Flag_Clear = 0;

_Bool Flag_M0_Missing = 0;
_Bool Flag_M1_Missing = 0;
_Bool Flag_M2_Missing = 0;

//FOR FLOAT

union
{
	float df;
	uint32_t di;
	char dc[4];
}Float_Disp;

union
{
	uint32_t di;
	uint8_t dc[4];
}Int_Disp;

//===============================

#define Master_Password		23375
_Bool Flag_Master_Login = 0;

#define Er_Password			21380
#define Admin_Password 		28143
#define Operator_Password 	1995
uint8_t Page_to_Proceed;

uint16_t Spl_Task_Password = 12321;

_Bool Flag_Password_OK = 0;			//Password entered ok.. used to switch between screens having password rights

_Bool Flag_Over_Error = 0;
_Bool Flag_Over_Error_Horz 	= 0;
_Bool Flag_Over_Error_Vert 	= 0;
_Bool Flag_Mark_Missing = 0;

uint16_t Zero_Spd_Time = 0;
#define Zero_Spd_Time_Limit	10000		//10 Seconds

uint16_t Enc_Count_at_Z = 0;
uint16_t Prev_Enc_Count_at_Z = 0;

#define Length_High_Priority_LCD_XY_Mode	13
uint8_t Buffer_High_Priority_LCD_XY_Mode[13] = {0xAA,0xE0,0x55,0xAA,0x5A,0xA5,0x07,0x82,0x00,0xCC,0x33,0xC3,0x3C};
_Bool Flag_High_Priority_LCD_XY_Mode = 0;

#define Display_Rate_System_Mode	200
#define Display_Rate_BLE_Mode		1000

#define LED_Status_Code_Location				55		//0x6E(110) / 2

#define ARC_Gate_Slave_Min_Valid_Value_Location	104		//0XD0(208) / 2
#define ARC_Gate_Slave_Max_Valid_Value_Location	105		//0XD2(210) / 2

#define Error_Status_Code_Location				106		//0xD4(212) / 2

//If want to show baseling in Graph, Enable this
#define ARC_Graph_Valid_BaseLine_Location		113		//0XE2(226) / 2

#define ARC_MasterFirst	0
#define ARC_SlaveFirst	1

float Distance_From_Slave_mm[2];
#define Horizontal	0
#define Vertical	1

#define Step_SizeF_offset	0.05
#define Step_SizeC_offset	0.10

_Bool Flag_Index = 0;
_Bool Flag_Index1 = 0;

#define Max_Value_Timer_Idle	60000

//========FROM GENERAL VARIABLES================
#define BUFFER_SIZE_DMA_UART4	255
uint8_t receive_buff[2][255];

_Bool Flag_DMA_Buffer_Write = 0,Flag_DMA_Buffer_Read = 0;
_Bool Flag_DMA_ON = 0;

uint16_t DMA_Tx_ms = 0;
uint8_t DMA_Tx_Wr_Count = 0,DMA_Tx_Rd_Count = 0;
uint8_t DMA_Tx_Stage = 0;

#define DMA_Tx_Buffer_length	80
uint8_t DMA_Tx_Buffer[DMA_Tx_Buffer_length][100],DMA_Tx_Length[DMA_Tx_Buffer_length] = {0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0};

//=========LCD RELATED DEFINITIONS=============
uint8_t lcd_received_cmd,lcd_received_bytes[10];
uint8_t EndBytes[4] = {0xCC,0x33,0xC3,0x3C};

unsigned char rec_function_byte=0xFF;

_Bool Flag_TouchKeyPressed 	= 0;
_Bool Flag_GenKeyPressed 	= 0;
_Bool Flag_ReceivedValue 	= 0;

//TYPE OF RECEIVED VALUE
uint8_t Flag_Value_Type	= 0;
#define Value_Type_Int		0
#define Value_Type_Float	1

//FORMAT LCD
uint8_t Format_LCD_Check = 0;
uint16_t Format_LCD_Password[] = {15847,32767,13143};

uint8_t Parameter_Default_Check = 0;
uint16_t Parameter_Default_Password[] = {26150,21069};

_Bool Flag_LCD_Init = 0;
//===============================================

_Bool Flag_Alarm 	= 0;
_Bool Flag_Previous_Alarm = 0;
_Bool Flag_Setting 	= 0;
_Bool Flag_Home 	= 0;

//ADC3 for BaseLine Calculations
uint32_t bl_sum = 0, bl_count = 0, bl_average = 0;
uint32_t Disp_bl_count = 0;

_Bool Flag_bl_buffer_index = 0;

#define bl_buffer_size_half	500
#define bl_buffer_size		1000
volatile uint16_t bl_buffer[bl_buffer_size] __attribute__((section(".dma_buffer"), aligned(32)));

uint8_t Timer_OverFlow_ms = 5;
uint16_t Buttons_Status_LEDs = 0;
uint16_t BLE_Buttons_Status_LEDs = 0;
#define LCD_Brightness 0x3F

//EEPROM RELATED DEFINITIONS
#define 	EEPROM_ADDRESS 		0xA0
#define 	_EEPROM_PSIZE     	32

//========================
_Bool Flag_Spl_Rights = 0;

_Bool Flag_Multiple_Errors 	= 0;
_Bool Flag_Error_Screen 	= 0;

short int Max_Mark_Height_Count = 0;
short int Min_Mark_Height_Count = 0;

uint8_t start_header[8]		=	{0xAA,0x4D,0x00,0x06,0x00,0x00,0x00,0x00};
uint8_t end_header[4]		=	{0xCC,0x33,0xC3,0x3C};

short int buffer_lcd[2][size_buffer_lcd];

_Bool Flag_Mark_Start = 0;

#define DMA_Data_Graph	0
#define DMA_Data_Disp	1

_Bool Flag_DMA_Data_Type = DMA_Data_Graph;

#define DMA_Process_Idle		0
#define DMA_Graph_Continue		1
#define DMA_Graph_Completed		2

uint8_t DMA_Process_Status = DMA_Process_Idle;

uint32_t ADC_Value = 0;
uint16_t ADC_ReadCount = 0;

uint8_t MarkCount = 0;
uint16_t Mark_Encoder_Value[3][2];
uint32_t Mark_TimeStamp_uS[3][2];

#define Master_Mark_First	0
#define Slave_Mark_First	1

uint16_t max_base_line = 0;

#define Max_Count_Base_Line	4095		//4050

_Bool Flag_My_Error_Calculated = 0;
_Bool Flag_My_ErrorH_Calculated = 0;
_Bool Flag_My_ErrorV_Calculated = 0;

//========================================

#define Pulse_Upper 0
#define Pulse_Lower 1
_Bool Flag_Pulse_Type = 0;

#define Pulse_Level_Active	1
#define Pulse_Level_Idle	0

_Bool Flag_Pulse_Continue 	= 0;

_Bool Flag_M0_Detected = 0;
_Bool Flag_M1_Detected = 0;
_Bool Flag_M2_Detected = 0;

uint16_t Mark_M0_Height_Diff_Count = 0;
uint16_t Mark_M0_Height_Disp_Count = 0;

uint16_t Mark_M1_Height_Diff_Count = 0;
uint16_t Mark_M1_Height_Disp_Count = 0;

uint16_t Mark_M2_Height_Diff_Count = 0;
uint16_t Mark_M2_Height_Disp_Count = 0;

uint16_t Actual_Count_lcd;

//========FROM TC VARIABLES===================

#define Touch_CAL_Confirm			124		//YES in Cal Confirm
#define Touch_Back_To_CalMain		128		//NO in Cal Confirm
uint16_t Error_Status_Code = 0;
uint16_t Cur_Error_Status_Code = 0, Prev_Error_Status_Code = 0;
uint8_t Error_Status_String[50];

#define TRUE	1
#define FALSE	0

_Bool Flag_Timer_5ms = 0;

uint8_t Flag_Alarm_Blink = 0;

_Bool Debug_Flag_Gain_Used = 0;
#define PGAIN	0
#define DGAIN	1
char Debug_Gain_Used[2] = {'P','D'};

_Bool Flag_Serial_Debug_Enable = 0;
#define Serial_Debug_Enable_Location	103	//0XCE(206) / 2

uint32_t value_microsecZ = 0,value_microsecZ_avg = 0;
#define Encoder_Mode	0
#define Time_Mode		1

_Bool Flag_Z_Missing = 1;
_Bool Flag_Disp_Z_Missing = 1;

uint8_t Stage_AutoSearch = 0;
uint16_t AutoSearch_Setmm = 170;
uint16_t AutoSearch_mm,AutoSearch_PPR;

_Bool Flag_AutoSearch_Update = 0,Flag_AutoSearch_Error = 0;
uint16_t AutoSearch_Rising_PPR,AutoSearch_Falling_PPR;

//Auto Averaging
#define Auto_Averaging_Speed_Tolerance_Horz		5
#define Auto_Averaging_Speed_Tolerance_Vert		5

uint16_t Auto_Averaging_Speed_Ref_Horz[7] = {40,80,120,160,200,240,300};
uint16_t Auto_Averaging_Average_Count_Horz[7] = {2,3,4,5,6,7,8};
#define Auto_Averaging_Average_Count_High_Speed_Horz	9	//If no condition match above

uint16_t Auto_Averaging_Speed_Ref_Vert[7] = {40,80,120,160,200,240,300};
uint16_t Auto_Averaging_Average_Count_Vert[7] = {2,3,4,5,6,7,8};
#define Auto_Averaging_Average_Count_High_Speed_Vert	9	//If no condition match above

uint16_t Disp_auto_average[2] = {0,0};

_Bool Flag_Print_Mode = 0;

_Bool Flag_Home_Mode = 0;
_Bool Flag_Event_Gate_Set = 0;
float SetValue_Home_Mode;

_Bool Flag_Update_SetValue = 0;

uint32_t Speed_Values[20];
_Bool Flag_Time_Average = 0;
uint8_t Speed_index = 0;
_Bool Flag_Speed_Avg_Reset = 1;

//=================================================

#define LCD_DISP_MOVE_ICON_CMD_01	0
#define LCD_DISP_MOVE_ICON_CMD_02	1
#define LCD_DISP_MOVE_ICON_CMD_03	2
#define LCD_DISP_MOVE_ICON_CMD_04	3
#define LCD_DISP_MOVE_ICON_CMD_05	4

#define LCD_DISP_AVG_HORZ			5		//0 EARLIER
#define LCD_DISP_AVG_VERT			6
#define LCD_DISP_MARK_HGT_mm		7
#define LCD_DISP_MARK_HGT_PROG		8
#define LCD_DISP_SPEED				9

#define LCD_DISP_KST				10

#define LCD_DISP_VALID_MARKS		11
#define LCD_DISP_BASELINE			12

#define LCD_DISP_TEMP_DIFF			13

#define LCD_DISP_GATE_DRAW_CMD_01		14
#define LCD_DISP_GATE_DRAW_CMD_02_LEN	15

#define LCD_DISP_GATE_DRAW_CMD_03_HSX	16
#define LCD_DISP_GATE_DRAW_CMD_04_HSY	17
#define LCD_DISP_GATE_DRAW_CMD_05_HEX	18
#define LCD_DISP_GATE_DRAW_CMD_06_HEY	19
#define LCD_DISP_GATE_DRAW_CMD_07_HCL	20

#define LCD_DISP_GATE_DRAW_CMD_08_VSX	21
#define LCD_DISP_GATE_DRAW_CMD_09_VSY	22
#define LCD_DISP_GATE_DRAW_CMD_10_VEX	23
#define LCD_DISP_GATE_DRAW_CMD_11_VEY	24
#define LCD_DISP_GATE_DRAW_CMD_12_VCL	25

#define LCD_DISP_GATE_DRAW_CMD_13_SSX	26
#define LCD_DISP_GATE_DRAW_CMD_14_SSY	27
#define LCD_DISP_GATE_DRAW_CMD_15_SEX	28
#define LCD_DISP_GATE_DRAW_CMD_16_SEY	29
#define LCD_DISP_GATE_DRAW_CMD_17_SCL	30

#define LCD_DISP_GATE_MH_START		31
#define LCD_DISP_GATE_MH_END		32
#define LCD_DISP_GATE_MV_START		33
#define LCD_DISP_GATE_MV_END		34
#define LCD_DISP_GATE_SLV_START		35
#define LCD_DISP_GATE_SLV_END		36

#define LCD_DISP_GATE_MH_START_PPR	37
#define LCD_DISP_GATE_MV_START_PPR	38
#define LCD_DISP_GATE_SLV_START_PPR	39

#define LCD_DISP_ENCODER_COUNTS		40
#define LCD_DISP_ERC				41

int16_t LCD_Display_Registers[42];

//==================================================

_Bool Flag_BLE_Buffer_RD = 0,Flag_BLE_Buffer_WR = 0;
_Bool Flag_BLE_BagLength_Arrived = 0;
short int buffer_ble[2][size_buffer_ble];

//==================================================

_Bool Flag_Correction_Horz_Upper = 0;
_Bool Flag_Correction_Horz_Lower = 0;
_Bool Flag_Correction_Vert_Upper = 0;
_Bool Flag_Correction_Vert_Lower = 0;

uint8_t Indexing_stage_at_System_Start = 1;
	//1 : yet to search marks inside gates
	//2 : start searching marks inside gates
	//0 : initial indexing in completed

_Bool Flag_Indexing_at_System_Start = 0;
_Bool Flag_All_Marks_OK = 0;

//=========Home Screen Move Icon==============
#define maximum_mm_move_icon		2.5		//2.5

//ALTIMATCH									//PRINT PILOT
uint16_t icon_id_move_icon = 64;			//123
uint16_t maximum_pixel_move_icon = 75;		//38
uint16_t icon_half_width_pixel = 5;			//15

float multiplier_mm_to_pixel_move_icon;

//(68,172) starting point of circle, 56,56 center of circle from starting point
//#define position_move_icon_center_x		127		//124		//068 + 56
//#define position_move_icon_center_y		212		//228		//172 + 56

uint16_t position_move_icon_center_x = 200;		//127
uint16_t position_move_icon_center_y = 265;		//212

int16_t position_move_icon_horz_x = 0, position_move_icon_vert_y = 0;

//================HOME COLOUR BOX==============

#define COLOUR_BOX_LCD_ADDRESS	0X00C0

//FOR ALTIMATCH						//PRINT PILOT
uint16_t COLOUR_BOX_START_X = 1;	//16
uint16_t COLOUR_BOX_START_Y = 1;	//33
uint16_t COLOUR_BOX_END_X = 47;		//237
uint16_t COLOUR_BOX_END_Y = 47;		//67

uint8_t Perform_Good_Copy_Horz_Stage = 0;
uint8_t Perform_Good_Copy_Vert_Stage = 0;
//0 : Idle
//1 : Activated
//2 : Avg Value based transfer in offset done.
//3 : update offset value, store and display it

//================GRAPH DRAW PULSES============
//FOR ALTIMATCH								//PRINT PILOT
uint16_t LCD_Graph_Gate_Y_Start = 75;		//85
uint16_t LCD_Graph_Gate_Y_End 	= 324;		//297

#define LCD_Graph_Colour_Master_Horz	0X328A		//R = 47, 	G = 79, 	B = 79, 	RGB = 0x328A
#define LCD_Graph_Colour_Master_Vert	0x7412		//R = 112, 	G = 128, 	B = 144, 	RGB = 0x7412
#define LCD_Graph_Colour_Slave			0x6B4D		//R = 105, 	G = 105, 	B = 105, 	RGB = 0x6B4D

//=============================================

uint16_t alt_second_ms = 0;
_Bool Flag_Time_Second = 0;

//=============================================

_Bool Flag_Start_Timer_Vertical 	= 0;
_Bool Flag_Start_Timer_Horizontal 	= 0;

_Bool Flag_LCD_Init_Error = 0;
_Bool Flag_LCD_Init_Alarm = 0;

#define Display_Error_Location				109		//0xDA / 2 = 218 / 2 = 109
#define LCD_Flag_Permit_Action_Location		112		//0xE0 / 2 = 224 / 2 = 112

#define Version_Higher_Byte	2
#define Version_Lower_Byte	6

#define Version_Higher_Location				110
#define Version_Lower_Location				111

#define MODEL_PRINTPILOT	0
#define MODEL_ALTIMATCH		1

_Bool Flag_ARCSS_Model = MODEL_ALTIMATCH;

uint16_t Float_Length_Home = 3;
uint16_t Float_Length_Diagnosis = 4;

uint8_t Resp_Bytes[50];
uint8_t Resp_Length;

uint32_t temp_speed_multiplier;

_Bool Flag_Use_With_PC = 0;
_Bool Flag_Permit_Actions = 0;
_Bool Flag_Update_Permit_Actions = 0;

uint8_t Auto_HV_Status = 0;
#define Flash_Address_PC	0x0300		//Also defined in stm32h7xx_it.c. any change here should be reflected there also.
#define Flash_Address_Auto	0x0310

uint8_t Reset_Cause;
_Bool Flag_Reset_Info_Sent = 0;
_Bool Flag_TCP_Recreate = 0;
_Bool Flag_TCP_Reconnect = 0;

uint8_t Seconds_From_TCP_Connect = 0;
uint8_t Seconds_From_TCP_Recreate = 0;
uint8_t Seconds_From_TCP_Reconnect = 0;

uint8_t TCP_Reconnect_Cause = 0;

#define TCP_Reconnect_Cause_Unknown 			0
#define TCP_Reconnect_Cause_Link_Disconnect		1
#define TCP_Reconnect_Cause_Ack_Missing			2

#define Info_Reset 			0
#define Info_TCP_Recreate 	1
#define Info_TCP_Reconnect 	2
#define Info_Encoder_Error	3

_Bool Flag_First_Threshold = 1;
uint16_t Temp_Diff_BL_VBL = 0, Prev_Diff_BL_VBL = 0;
uint16_t Temp_Diff_BL_SG = 0;

_Bool Z_to_Z_Averaging_Done = 0;
_Bool Flag_Z2Z_Completed = 0;

_Bool Flag_BaseLine_Error = 0, Flag_Prev_BaseLine_Error = 0;

uint8_t BaseLine_Valid_Count = 0;
uint8_t arc_execution_main_stage = 0;

uint8_t arc_execution_func_prev_id_02 = 0;
uint8_t arc_execution_func_prev_id_01 = 0;
uint8_t arc_execution_func_prev_id = 0;
uint8_t arc_execution_func_cur_id = 0;

uint8_t arc_execution_int_prev_id = 0;
uint8_t arc_execution_int_id = 0;

uint8_t max_queue_count = 0;

_Bool Flag_Update_Graph_Gates = 1;

uint16_t ADC_H_Count = 0, Disp_H_Count = 0;

_Bool Flag_Set_Threshold = 1;

#define Permit_Set_Threshold_Limit_Perc 25
uint16_t Permit_Set_Threshold_Limit_Count;
uint16_t Set_Threshold_Count = 0;

_Bool Flag_TIM3_Baglength_Set_Threshold = 0;

_Bool Flag_EncError = 0;
uint16_t EncError_DispCount = 0;
uint32_t EncError_TotalCount = 0;
_Bool Flag_EncError_SkipInc = 0;

uint16_t Temp_Disp_Enc_Count_at_Z = 0;
uint16_t Temp_Disp_Prev_Enc_Count_at_Z = 0;
float Prev_Corrected_Error = 5000.0;
