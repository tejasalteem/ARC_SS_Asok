extern osMessageQueueId_t TCP_WriteHandle;


void BLE_Push_Data_DMA_Tx_Buffer(uint8_t *str_ptr, uint16_t str_len)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 119;

//	  arc_tx *msg = malloc(sizeof(arc_tx));

	  if(osMessageQueueGetSpace(TCP_WriteHandle) == 0)
	  {
		  Send_String_UART_232((uint8_t *)"TCP_WriteQueue : No Space Available\n");
		  return;
	  }

	  tcp_queue *msgw = pvPortMalloc(sizeof(tcp_queue));

	  if(msgw == NULL)
	  {
		  Send_String_UART_232((uint8_t *)"TCP_WriteQueue : msg Malloc Error\n");
		  return;
	  }

	  msgw->length = str_len;
//	  msg->data = (uint8_t *)malloc(str_len);
	  msgw->data = (uint8_t *)pvPortMalloc(str_len);

	  if(msgw->data != NULL)
	  {
		  memcpy(msgw->data, str_ptr, str_len);

		  if(osMessageQueuePut(TCP_WriteHandle, &msgw, 0, 0) != osOK)
		  {
			  Send_String_UART_232((uint8_t *)"TCP_WriteQueue : Put Error - Queue Full\n");
			  vPortFree(msgw->data);
			  vPortFree(msgw);
		  }
	  }
	  else
	  {
		  vPortFree(msgw);
		  Send_String_UART_232((uint8_t *)"TCP_WriteQueue : Data Malloc Error\n");
	  }
}

void Slave_Handle_Line_Idle_Event(void)
{
	arc_execution_func_prev_id_02 = arc_execution_func_prev_id_01;
	arc_execution_func_prev_id_01 = arc_execution_func_prev_id;
	arc_execution_func_prev_id = arc_execution_func_cur_id;
	arc_execution_func_cur_id = 120;

	_Bool Flag_Write_Command_Received = 0;
	uint8_t i;
	
	uint8_t data_length = BLE_Received_Data_Length[Flag_Slave_DMA_Buffer_Read];

	if((Slave_receive_buff[Flag_Slave_DMA_Buffer_Read][0] == Config_Parameters[Alt_Station_ID]) && ((Slave_receive_buff[Flag_Slave_DMA_Buffer_Read][1] == BLE_Command_Write_Single_Key) || (Slave_receive_buff[Flag_Slave_DMA_Buffer_Read][1] == BLE_Command_Write_Single_Config16_Reg) || (Slave_receive_buff[Flag_Slave_DMA_Buffer_Read][1] == BLE_Command_Write_Single_Config32_Reg)))
	{
		Flag_Write_Command_Received = 1;
	}

	if((Select_Node == Select_Node_None) && (Flag_Write_Command_Received != 0))
	{
		Select_Node = Select_Node_Slave;
	}

	if((Select_Node == Select_Node_Slave) || (Flag_Write_Command_Received == 0))			//Slave
	{
		if(Slave_receive_buff[Flag_Slave_DMA_Buffer_Read][0] == Config_Parameters[Alt_Station_ID])
		{
			if((Flag_BLE_Enable != 0) && (data_length == 8))
			{
				if((Slave_receive_buff[Flag_Slave_DMA_Buffer_Read][1] >= 0X12) && (Slave_receive_buff[Flag_Slave_DMA_Buffer_Read][1] <= 0x14))
				{
					BLE_received_cmd = Slave_receive_buff[Flag_Slave_DMA_Buffer_Read][1];
					for(i = 0; i < data_length; i++)
					{
						BLE_rec_buff[i] = Slave_receive_buff[Flag_Slave_DMA_Buffer_Read][i];
					}
					BLE_Reg_Data_Type = Type_16_Bit;		//used only for 16 bit holding reg write
					Flag_Slave_ReceivedValue = 1;
				}
			}
			else if(data_length == 5)						//Read_Input_Reg_Group or Command_Enable_Disable
			{
				//BLE CONNECTED-DISCONNECTED, GRAPH ON-OFF
				if(((Flag_BLE_Enable != 0) && ((Slave_receive_buff[Flag_Slave_DMA_Buffer_Read][1] == BLE_Command_Read_Input_Reg_Group) || (Slave_receive_buff[Flag_Slave_DMA_Buffer_Read][1] == BLE_Command_Buzzer_Control))) || (Slave_receive_buff[Flag_Slave_DMA_Buffer_Read][1] == BLE_Command_Enable_Disable))
				{
					BLE_received_cmd = Slave_receive_buff[Flag_Slave_DMA_Buffer_Read][1];
					for(i = 0; i < data_length; i++)
					{
						BLE_rec_buff[i] = Slave_receive_buff[Flag_Slave_DMA_Buffer_Read][i];
					}
					Flag_Slave_ReceivedValue = 1;
				}
			}
			else if((Flag_BLE_Enable != 0) && (data_length == 10))
			{
				if(Slave_receive_buff[Flag_Slave_DMA_Buffer_Read][1] == BLE_Command_Write_Single_Config32_Reg)
				{
					BLE_received_cmd = Slave_receive_buff[Flag_Slave_DMA_Buffer_Read][1];
					for(i = 0; i < data_length; i++)
					{
						BLE_rec_buff[i] = Slave_receive_buff[Flag_Slave_DMA_Buffer_Read][i];
					}
					Flag_Slave_ReceivedValue = 1;
					BLE_Reg_Data_Type = Type_32_Bit;
				}
			}
			else if((Flag_BLE_Enable != 0) && (data_length == 3))	//Job_Name
			{
				if(Slave_receive_buff[Flag_Slave_DMA_Buffer_Read][1] == BLE_Command_Write_Job_Name)
				{
					BLE_received_cmd = Slave_receive_buff[Flag_Slave_DMA_Buffer_Read][1];
					for(i = 0; i < data_length; i++)
					{
						BLE_rec_buff[i] = Slave_receive_buff[Flag_Slave_DMA_Buffer_Read][i];
					}
					Flag_Slave_ReceivedValue = 1;
				}
			}
		}
		else if(Slave_receive_buff[Flag_Slave_DMA_Buffer_Read][0] == 0xAA)
		{
			if(data_length == 4)
			{
				if(Slave_receive_buff[Flag_Slave_DMA_Buffer_Read][1] == BLE_Command_Request_Station_ID)
				{
					//REQUEST STATION ID
					BLE_received_cmd = Slave_receive_buff[Flag_Slave_DMA_Buffer_Read][1];

					for(i = 0; i < data_length; i++)
					{
						BLE_rec_buff[i] = Slave_receive_buff[Flag_Slave_DMA_Buffer_Read][i];
					}
					Flag_Slave_ReceivedValue = 1;
				}
			}
			else if(data_length == 3)
			{
				if(Slave_receive_buff[Flag_Slave_DMA_Buffer_Read][1] == BLE_Command_Request_OTA)
				{
					BLE_received_cmd = Slave_receive_buff[Flag_Slave_DMA_Buffer_Read][1];
					for(i = 0; i < data_length; i++)
					{
						BLE_rec_buff[i] = Slave_receive_buff[Flag_Slave_DMA_Buffer_Read][i];
					}
					Flag_Slave_ReceivedValue = 1;
				}
			}
		}
	}
}

uint16_t mmodbus_crc16(uint8_t *nData, uint16_t wLength)		//MODIFIED
{
  uint8_t i;
  uint16_t wCRCWord = 0xFFFF,nTemp;
  for(i = 0; i < wLength; i++)
  {
		nTemp = (uint16_t)nData[i];
    nTemp = nTemp ^ wCRCWord;
		nTemp = nTemp & 0x00FF;
    wCRCWord >>= 8;
    wCRCWord  ^= wCRCTable[nTemp];
  }
  return wCRCWord;
}
