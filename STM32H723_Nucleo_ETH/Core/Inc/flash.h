//0X0000 : CONFIG PARAMETERS
//0X0100 : CONFIG FLOAT PARAMETERS
//0x0200 : JOB NAME
//0x0300 : RESET VALUES OF PROGRAM COUNTER AND HANDLER IDENTIFICATION
//0x0310 : AUTO STATUS

extern I2C_HandleTypeDef hi2c1;

void Flash_Read_Config_Parameters(void);
void Flash_Write_Config_Parameters(void);

int16_t Flash_Read_Config_Parameters_Single(uint16_t);
void Flash_Write_Config_Parameters_Single(uint16_t);
void Flash_Write_Config_Parameters_Multiple(uint16_t,uint8_t);

//Float
void Flash_Write_Config_Float(void);
void Flash_Read_Config_Float(void);

void Flash_Write_Config_Float_Single(uint16_t Config_Index);
float Flash_Read_Config_Float_Single(uint16_t Config_Index);

//WatchDog
void Flash_Write_Single_Byte(uint16_t Address, uint8_t value);
uint8_t Flash_Read_Single_Byte(uint16_t Address);
void Flash_Write_Buffer(uint16_t Address, uint8_t * Data, uint8_t len);
void Flash_Read_Buffer(uint16_t Address, uint8_t * Data, uint8_t len);

//=======================

_Bool ee24_isConnected(void);
_Bool ee24_write(uint16_t, uint8_t *, size_t);
_Bool ee24_read(uint16_t, uint8_t *, size_t);

void Flash_Write_Config_Parameters(void)
{
	ee24_write(0x0000, (uint8_t *)&Config_Parameters[0],  32);		//	0 - 15	16 => total 16
	osDelay(10);
	ee24_write(0x0020, (uint8_t *)&Config_Parameters[16], 32);		//	16 - 31 16 => total 32
	osDelay(10);
	ee24_write(0x0040, (uint8_t *)&Config_Parameters[32], 26);		//	32 - 44 13 => total 45
	osDelay(10);
}

void Flash_Read_Config_Parameters(void)
{
	ee24_read(0x0000, (uint8_t *)Config_Parameters, No_Of_Config_Parameters * 2);						//58 * 2 = 116 bytes
	osDelay(10);
}

void Flash_Write_Config_Parameters_Single(uint16_t Config_Index)
{
	ee24_write(Config_Index * 2, (uint8_t *)&Config_Parameters[Config_Index], 2);	//*2,2
}

void Flash_Write_Config_Parameters_Multiple(uint16_t Config_Index,uint8_t wr_length)			//Multiple can not cross 32 byte boundaries
{
	ee24_write(Config_Index * 2, (uint8_t *)&Config_Parameters[Config_Index], wr_length * 2);
}

int16_t Flash_Read_Config_Parameters_Single(uint16_t Config_Index)					//uint16_t
{
	int16_t temp_read;
	ee24_read(Config_Index * 2, (uint8_t *)&temp_read, 2);							//*2,2
	return(temp_read);
}

//=================FLOAT READ WRITE====================

void Flash_Write_Config_Float(void)
{
	ee24_write(0x0100, (uint8_t *)&Config_Float[0], 32);		//0 - 7		total 8
	osDelay(10);
	ee24_write(0x0120, (uint8_t *)&Config_Float[8], 32);		//8 - 15	total 16
	osDelay(10);
	ee24_write(0x0140, (uint8_t *)&Config_Float[16], 12);		//16 - 18	total 19
	osDelay(10);
}

void Flash_Read_Config_Float(void)
{
	ee24_read(0x0100, (uint8_t *)Config_Float, No_Of_Config_Float * 4);		//14 * 4 = 56 bytes
	osDelay(10);
}

void Flash_Write_Config_Float_Single(uint16_t Config_Index)
{
	ee24_write((Config_Index * 4) + 0x0100, (uint8_t *)&Config_Float[Config_Index], 4);
}

float Flash_Read_Config_Float_Single(uint16_t Config_Index)						//uint16_t
{
	uint32_t temp_read;
	ee24_read((Config_Index * 4) + 0x0100, (uint8_t *)&temp_read, 4);
	return(temp_read);
}

//==============JOB NAME===========================================

void Flash_Write_Job_Name(void)
{
	ee24_write(0x0200, (uint8_t *)Job_Name, 22);
	osDelay(10);
}

void Flash_Read_Job_Name(void)
{
	ee24_read(0x0200, (uint8_t *)Job_Name, 22);
}

//==============WATCHDOG===========================================

void Flash_Write_Single_Byte(uint16_t Address, uint8_t value)
{
	ee24_write(Address, (uint8_t *)&value, 1);	//*2,2
}

uint8_t Flash_Read_Single_Byte(uint16_t Address)
{
	int8_t temp_read;
	ee24_read(Address, (uint8_t *)&temp_read, 1);
	return(temp_read);
}

void Flash_Write_Buffer(uint16_t Address, uint8_t *Data, uint8_t len)
{
	ee24_write(Address, Data, len);	//*2,2
}

void Flash_Read_Buffer(uint16_t Address, uint8_t * Data, uint8_t len)
{
	ee24_read(Address, Data, len);	//*2,2
}


//============EEPROM BASIC FUNCTIONS===============================

_Bool ee24_isConnected(void)
{
  if (HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_ADDRESS, 2, 100)==HAL_OK)
    return(1);
  else
    return(0);
}

_Bool ee24_write(uint16_t address, uint8_t *data, size_t len)
{
    if (HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADDRESS, address, I2C_MEMADD_SIZE_16BIT, data, len, 1000) == HAL_OK)
    {
		return(1);
    }
	else
	{
		return(0);
	}
}

_Bool ee24_read(uint16_t address, uint8_t *data, size_t len)
{
  if (HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADDRESS, address, I2C_MEMADD_SIZE_16BIT, data, len,1000) == HAL_OK)
  {
    return(1);
  }
  else
  {
    return(0);
  }
}
