
#include "DS18B20.h"


static void ds1302_init(void)
{
	//io input
	DQ = 1;
}

static void ds1302_reset(void)
{
	bool flag = 1;

	while (flag)
	{
		while (flag)
	 	{
 			DQ = 1;
			udelay(10);
 			DQ = 0;
 			udelay(550); // 550us
 			DQ = 1; 
 			udelay(66);  // 66us
 			flag = DQ;
   		}
		udelay(500);    // 500us
		flag = ~DQ;
	}
	DQ=1;
}

static void ds1302_write_byte(uint8 val)
{
	uint8 i;

	for (i=0; i<8; i++)
	{
		DQ = 1;
		udelay(1);
		DQ = 0;
		udelay(4); //4us
		DQ = val & 0x01; 
		udelay(66); //66us
		val >>= 1;
	}
	DQ = 1;
	udelay(10);  
}

static uint8 ds1302_read_byte(void)
{
	uint8 i, value=0;

	for (i=0; i<8; i++)
	{
		DQ=1;
		udelay(1);
		value >>= 1;
		DQ = 0;
		udelay(4);   //4us
		DQ = 1;
		udelay(4);   //4us 
		if (DQ)
			value|=0x80;
		udelay(66);  //66us
	}
	DQ=1;

	return value;
}

static void ds1302_start(void)
{
	ds1302_reset();
	ds1302_write_byte(0xCC); // Skip ROM cmd
	ds1302_write_byte(0x44); // convert cmd
}

static int16 ds1302_read_temp(void)
{
	uint8 temp_data[2]; // ¶Á³öÎÂ¶ÈÔÝ·Å
	int16 temp;

	ds1302_reset();
	ds1302_write_byte(0xCC); // Skip ROM cmd
	ds1302_write_byte(0xBE); // read
	temp_data[0] = ds1302_read_byte();  // low 8 bits
	temp_data[1] = ds1302_read_byte();  // high 8 bits

	temp = temp_data[1];
	temp <<= 8;
	temp |= temp_data[0];
	temp >>= 4;

	return temp;
}


static int8 ds18b20_open(uint8 id)
{
	UNUSED(id);
	ds1302_init();
	
	return 0;
}

static void ds18b20_close(uint8 id)
{
	UNUSED(id);
}

static int8 ds18b20_ioctl(uint8 id, uint8 cmd, uint16 args)
{
	int16 *p_temp = (int16 *)args;

	if (GET_CMD_DIR(cmd) == CMD_GET)
	{
		switch (cmd)
		{
			case DS18B20_GET:
				ds1302_start();
				OSWait(K_TMO, OS_TICKS_PER_SEC); // delay 1S
				*p_temp = ds1302_read_temp();
				break;
			default: 
				break;
		}
	}
	
	UNUSED(id);
	
	return 0;
}


const struct file_operations ds18b20_fops = {
	ds18b20_open,
	ds18b20_close,
	ds18b20_ioctl
};

