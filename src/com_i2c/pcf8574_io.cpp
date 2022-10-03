//======================================================================================================================
// C O P Y R I G H T
//======================================================================================================================
/// \file       pcf8574_io.cpp
/// \brief      TODO
/// \details    TODO
/// \author     maintained by: Mario D. Nevola
///
/// \copyright  Copyright (c) 2022 by Universität Stuttgart. All rights reserved. \n
///             The reproduction, distribution and utilization of this file as well as the communication of its \n
///             contents to others without express authorization is prohibited. Offenders will be held liable for \n
///             the payment of damages. \n
///             All rights reserved in the event of the grant of a patent, utility model or design. \n
///             \b Disclaimer: Any modification or usage outside of the intended purpose is not under authors liability. \n
///             \b Usage: Further use of source code files or code snippets is under full liability of the user.
//======================================================================================================================
 

//======================================================================================================================
// Inclusions
//====================================================================================================================== 
#include "pcf8574_io.hpp"


//======================================================================================================================
// Type Definitions / Enums / Defines / Macros / Consts
//======================================================================================================================


//======================================================================================================================
// Local Method Prototypes
//======================================================================================================================


//======================================================================================================================
// Variables, Objects
//======================================================================================================================


//======================================================================================================================
// Local Implementation
//======================================================================================================================


//======================================================================================================================
// API Implementation
//======================================================================================================================


/// ------------------------------------------------------------------------------------------------------------
/// See description in Header file.
/// ------------------------------------------------------------------------------------------------------------
void pcf8574::setByte(unsigned char value)
{
    struct i2c_smbus_ioctl_data data;
    data.read_write = I2C_SMBUS_WRITE;
    data.size = I2C_SMBUS_BYTE;
    data.data = NULL;
    data.command = value;

    int device;
    if ((device = open(i2cBusPort.c_str(), O_RDWR)) == -1)
    {
        fprintf(stderr,
                "ERROR: pcf8574::setByte failed to open i2c bus at %s\n", i2cBusPort);
        exit(EXIT_FAILURE);
    }

    ioctl(device, I2C_SLAVE, address);
    ioctl(device, I2C_SMBUS, &data);
    close(device);
    state = value;
}


/// ------------------------------------------------------------------------------------------------------------
/// See description in Header file.
/// ------------------------------------------------------------------------------------------------------------
void pcf8574::setBit(int outputPin, int newState)
{
    char mask = 0x01 << outputPin;
    setByte(newState ? (state | mask) : (state & (~mask)));
}


/// ------------------------------------------------------------------------------------------------------------
/// See description in Header file.
/// ------------------------------------------------------------------------------------------------------------
unsigned char pcf8574::getByte()
{
    union i2c_smbus_data data_union;
    struct i2c_smbus_ioctl_data data;
    data.read_write = I2C_SMBUS_READ;
    data.size = I2C_SMBUS_BYTE;
    data.data = &data_union;
    data.command = 0;

    int device;
    if ((device = open(i2cBusPort.c_str(), O_RDWR)) == -1)
    {
        fprintf(stderr,
                "ERROR: pcf8574::getByte failed to open i2c bus at %s\n", i2cBusPort);
        exit(EXIT_FAILURE);
    }

    ioctl(device, I2C_SLAVE, address);
    ioctl(device, I2C_SMBUS, &data);
    close(device);
    return data_union.byte;
}


/// ------------------------------------------------------------------------------------------------------------
/// See description in Header file.
/// ------------------------------------------------------------------------------------------------------------
uint8_t pcf8574::getBit(uint8_t pin)
{
    char mask = 0x01 << pin;
    return getByte() & mask ? 1 : 0;
}