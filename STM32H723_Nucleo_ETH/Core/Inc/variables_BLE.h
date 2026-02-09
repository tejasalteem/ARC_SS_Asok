#define Max_Count_Slave_DMA_Buffer_Write_Read	80

uint8_t Flag_Slave_DMA_Buffer_Write = 0,Flag_Slave_DMA_Buffer_Read = 0;
uint8_t Slave_receive_buff[Max_Count_Slave_DMA_Buffer_Write_Read][10];

uint8_t BLE_Received_Data_Length[Max_Count_Slave_DMA_Buffer_Write_Read];
	
//===============================================================
	
uint8_t BLE_received_cmd;

#define Type_16_Bit	0
#define Type_32_Bit	1

#define Max_KeyID_Address 31
uint8_t BLE_KeyID_Replace[]   = {0,5,6,7,8,	9,10,11,15,19,	26,27,28,29,30,	31,32,33,34,20,	21,22,23,35,36,	37,38,39,40,17,	18,12};
uint8_t BLE_KeyID_to_RegAdd[] = {0,0,0,0,0,	0,0,0,0,0,	30,30,31,31,30,	30,31,31,0,15,	16,24,25,23,13,	14,17,0,0,27,	19,29};

#define Max_DispReg_Address 28 //	Total 27 : 21 int, 6 float
uint8_t BLE_DispReg_Type[] 			= 	{0,0,1,1,0,		0,0,0,1,1,		1,1,0,0,0,		0,0,0,0,0,		0,0,0,0,0,		0,1,1,0};
//0 : 16 bit, 1 : 32 bit
uint8_t BLE_Display_Para_Index[] 	= 	{0,1,0,1,2,		3,4,5,2,3,		4,5,6,7,8,		9,10,11,12,13,	14,15,16,17,18,	19,6,7,20};
//For Int, 	it is index of BLE_Display_Registers
//For Float, it is index of BLE_Display_Float
	
int16_t BLE_Display_Registers[21];
float BLE_Display_Float[8];

#define BLE_DISP_AVG_HORZ			0
#define BLE_DISP_AVG_VERT			1

#define BLE_CORR_MS_HORZ			2
#define BLE_CORR_MS_VERT			3

#define BLE_DISP_MARK_HGT_mm		4
#define BLE_DISP_SPEED				5

#define BLE_DISP_KST				6
#define BLE_DISP_ERC				7

#define BLE_DISP_GATE_MH_START		8
#define BLE_DISP_GATE_MH_END		9
#define BLE_DISP_GATE_MV_START		10
#define BLE_DISP_GATE_MV_END		11
#define BLE_DISP_GATE_SLV_START		12
#define BLE_DISP_GATE_SLV_END		13

#define BLE_DISP_VALID_MARKS		14
#define BLE_DISP_BASELINE			15

#define BLE_DISP_ENCODER_COUNTS		16

#define BLE_DISP_GATE_MH_START_PPR	17
#define BLE_DISP_GATE_MV_START_PPR	18
#define BLE_DISP_GATE_SLV_START_PPR	19
#define BLE_DISP_SER_DEBUG_EN		20

//===================================================

#define BLE_DispF_ErrorRaw_Horz		0
#define BLE_DispF_ErrorRaw_Vert		1
#define BLE_DispF_MarkHeight_Horz	2
#define BLE_DispF_ErrorAvg_Horz		3
#define BLE_DispF_ErrorAvg_Vert		4
#define BLE_DispF_Production		5
#define BLE_DispF_SetValue_Horz		6
#define BLE_DispF_SetValue_Vert		7

_Bool Flag_BLE_Read_ON 	 = 0;
_Bool Flag_Last_Status_Read_ON = 0;
uint16_t BLE_Disp_Start_Address,BLE_Disp_Data_length;

//===================================================

#define Max_HoldReg_Address 48		//Total 48 : 30 - 16 bit, 18 - 32 bit

uint8_t BLE_HoldReg_Type[] 		= {	0,0,0,0,0,	0,0,0,0,0,
									0,0,0,0,0,	0,0,0,0,0,
									0,0,0,0,0,	0,0,0,0,0,
									1,1,1,1,1,	1,1,1,1,1,
									1,1,1,1,1,	1,1,1,1};		//0 : 16 bit, 1 : 32 bit

uint8_t BLE_Config_Para_Index[] = {	0,	1,	2,	3,	4,	5,	6,	7,	8,	9,
									10,	11,	12,	13,	14,	15,	16,	17,	18,	19,
									20,	21,	22,	23,	24,	25,	40, 42, 41, 43,
									0,	1,	2,	3,	4,	5,	6,	7,	8,	9,
									10,	11,	12,	13,	14, 15,	16, 17, 18};

#define BLE_Not_Accessible	100
//32
uint8_t BLE_HoldReg_Index[No_Of_Config_Parameters] = 	{	0,	1,	2,	3,	4,	5,	6,	7,	8,	9,
															10,	11,	12,	13,	14,	15,	16,	17,	18,	19,
															20,	21,	22,	23,	24,	25,	100,100,100,100,
															100,100,100,100,100,100,100,100,100,100,
															26,	28,	27,	29,	100
														};
//4																												
uint8_t BLE_HoldReg_Float_Index[No_Of_Config_Float] = 	{30,31,32,33,34,35,	36,37,38,39,40,	41,42,43,44,45,	46,47,48};

_Bool BLE_Reg_Data_Type;

uint8_t BLE_Index = 0;
uint16_t BLE_Reply_Index = 0;
uint16_t BLE_Request_Received = 0;

uint8_t BLE_rec_buff[10];

//ERROR CODES
#define BLE_Error_Code_No_Error					0
#define BLE_Error_Code_CRC_Not_matched			1
#define BLE_Error_Code_Same_Data_No_Action		2
#define BLE_Error_Code_Address_Out_Of_Range		3
#define BLE_Error_Code_Data_Not_Valid			4
#define BLE_Error_Code_Data_Type_Mismatched		5
#define BLE_Error_Code_Not_Processed			6

#define BLE_Command_Request_Station_ID			0X10		//Response 0x30, Error Response 0x90
#define BLE_Command_Buzzer_Control				0x11		//Response 0x31, Error Response 0x91
#define BLE_Command_Read_Config_Reg				0X12		//Response 0x32, Error Response 0x92
#define BLE_Command_Write_Single_Key			0X13		//Response 0x33, Error Response 0x93
#define BLE_Command_Write_Single_Config16_Reg	0X14		//Response 0x34, Error Response 0x94
#define BLE_Command_Write_Single_Config32_Reg	0X15		//Response 0x35, Error Response 0x95

#define BLE_Command_Write_Job_Name				0X16		//Response 0x36, Error Response 0x96
#define BLE_Command_Request_OTA					0X17		//Response 0x37, Error Response 0x97

#define BLE_Command_Enable_Disable				0X19		//Response 0x39, Error Response 0x90
#define BLE_Command_Read_Input_Reg_Group		0x1A		//Response 0x3A, Error Response 0x9A

//INFORMING APPLICATION ABOUT CHANGES AT DEVICE END
#define BLE_Command_Input_Reg_Home				0X1F
#define BLE_Command_Input_Reg_Home_RawErr		0X2F
#define BLE_Command_Input_Reg_Diagnosis			0X3F
#define BLE_Command_Graph_Data_Array			0X4F

#define BLE_Command_Communication_Check_Frame	0x8F
#define BLE_Command_Info_Write_From_Device		0X9F
#define BLE_Command_Info_Device_Reset_or_TCP_Recreate		0xAF

uint16_t CRC_Disp;

_Bool Flag_BLE_Value_Received = 0;
uint16_t Temp_BLE_Reg_Address,Temp_BLE_Reg_Data;
float Temp_BLE_Reg_Data_Float;

//============================================
_Bool Flag_BLE_KeyPress_Event = 0;
uint16_t Time_BLE_RepeatKeyPress_ms = 0;
//=============================================

#define Select_Node_Master		0
#define Select_Node_RS485		1
#define Select_Node_Slave		2
#define Select_Node_None		3

uint8_t Select_Node = Select_Node_None;						//0 : Main, 1 : Slave, 2 : Both
uint8_t Prev_Select_Node;

_Bool Flag_Slave_ReceivedValue = 0;

//============================================

const uint16_t wCRCTable[] = {
  0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
  0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
  0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
  0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
  0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
  0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
  0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
  0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
  0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
  0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
  0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
  0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
  0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
  0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
  0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
  0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
  0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
  0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
  0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
  0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
  0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
  0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
  0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
  0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
  0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
  0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
  0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
  0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
  0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
  0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
  0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
  0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040
};

//=====================================================

uint16_t BLE_Temp_Timer_Disp = 0;
_Bool Flag_BLE_DMA_Data_Type = DMA_Data_Graph;

uint8_t BLE_DMA_Process_Status = DMA_Process_Idle;

_Bool Flag_BLE_Graph_ON = 0;

uint16_t Actual_Count_ble;
int16_t Count_ble = 0;

_Bool Flag_BLE_Enable = 0;

_Bool BLE_Coil_Write_Success = 0;

int16_t  Correction_ms_Horz = 0;
int16_t  Correction_ms_Vert = 0;

uint32_t BLE_time_taken_for_graph = 0;
uint16_t Real_Baglength_Arrive_Count = 0, Executed_Baglength_Arrive_Count = 0,Prev_Baglength_Arrive_Count = 0;

volatile _Bool Flag_TCP_Connected = 0;
volatile _Bool Flag_TCP_Retry = 0;

uint32_t BLE_Display_Time_Execution = 0;

//============================================

#define Read_Input_Group_Mode_OFF			0
#define Read_Input_Group_Mode_Home			1
#define Read_Input_Group_Mode_Home_RawErr	2
#define Read_Input_Group_Mode_Diagnosis		3
#define Read_Input_Group_Mode_Graph			4

#define Read_Input_Group_Mode_Home_Start_Address		4		//2
#define Read_Input_Group_Mode_Home_Data_Length			10		//10

#define Read_Input_Group_Mode_Graph_Start_Address		4
#define Read_Input_Group_Mode_Graph_Data_Length			18

#define Read_Input_Group_Mode_Diagnosis_Start_Address	0
#define Read_Input_Group_Mode_Diagnosis_Data_Length		29

#define Read_Input_Group_Mode_Home_RawErr_Start_Address	0
#define Read_Input_Group_Mode_Home_RawErr_Data_Length	14

uint8_t Read_Input_Group_Mode = 0;	//0 : All OFF, 1 : Home, 2 : Graph, 3 : Diagnosis

_Bool Flag_Memory_Read_Done = 0;
uint8_t Temp_Mac_Address;

float BLE_Total_Bytes_Sent = 0;
uint16_t BLE_Total_Group_Sent = 0;

#define TCP_Try_For_Reconnect_Time_ms	30000

//Local Permission to change when TCP is connected
//0 : Not Permitted, 1 : Permitted
uint8_t Config16_Local_Permission[No_Of_Config_Parameters] = 	{	0,	0,	0,	0,	1,		0,	0,	0,	0,	0,
																	0,	0,	0,	1,	1,		0,	0,	0,	0,	1,
																	0,	0,	0,	0,	0,		0,	1,	0,	1,	1,
																	1,	1,	1,	1,	1,		1,	1,	1,	1,	1,
																	0,	0,	0,	0,	1
																};


uint8_t Config32_Local_Permission[No_Of_Config_Float] = 		{0,0,0,0,0,	0,0,0,0,0,	0,0,0,0,0,	0,0,0};

volatile _Bool Flag_ETH_Link_Status = 0;

uint16_t Poll_Count = 0;
uint8_t Poll_Error = 0, Poll_State = 0;
uint8_t Close_Location = 0;

char Job_Name[22] = "DEFAULT";
char Temp_Job_Name[25];

char OTA_File_Name[31];
char Temp_File_Name[40];

_Bool Flag_Encoder_Direction_Reverse = 0;
_Bool Flag_OTA_Request_Received = 0;

#define add_type_config	0
#define add_type_ble	1

_Bool Flag_coil_reply_data_type = 0;
uint16_t coil_reply_address;

_Bool Flag_Job_Name_Received = 0;
_Bool Flag_First_Time_TCP_Request_Done = 0;

uint16_t TCP_Dis_Total_Count = 0;
uint16_t TCP_Dis_Link_Count = 0;
uint16_t TCP_Dis_Ack_Missing_Count = 0;
