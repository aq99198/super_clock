#ifndef _DS18B20_H_
#define _DS18B20_H_

#include "drivers.h"


sbit DQ  = P3^2;

extern const struct file_operations ds18b20_fops;

#endif //_DS18B20_H_

