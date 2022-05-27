//======================================================================================================================
// C O P Y R I G H T
//======================================================================================================================
/// \file       pcf8574_io.hpp
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
// Header Protection
//======================================================================================================================
#ifndef PCF8574_HPP
#define PCF8574_HPP 1


//======================================================================================================================
// Inclusions
//======================================================================================================================
#include <cstdint>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "conf.hpp"


//======================================================================================================================
// Type Definitions / Enums / Defines / Macros / Consts
//======================================================================================================================
enum class GpioType : uint8_t
{
    Input = 0x00,
    Output = 0x01,
};
typedef GpioType gpioType_t;

//======================================================================================================================
// Extern Variables
//======================================================================================================================


//======================================================================================================================
// External Constants
//======================================================================================================================


//======================================================================================================================
// Prototypes
//======================================================================================================================
class pcf8574
{
private:
    uint16_t address;
    unsigned char state;
    std::string i2cBusPort;
    gpioType_t gpioType;

public:
    /// ----------------------------------------------------------------------------------------------------------------
    /// <summary>
    /// Class constructor for the pcf8574 class.
    /// <para><b>Description</b></para>
    ///     </summary>
    /// <param name='i2cBusPort'>
    ///     The I2C port referred to the Raspi pinout (e.g. "/dev/i2c-1").
    ///     </param>
    /// <param name='address'>
    ///     I2C address of the pcf8574.
    ///     </param>
    /// ----------------------------------------------------------------------------------------------------------------
    pcf8574(const std::string& i2cBusPort, uint16_t address, gpioType_t gpioType)    
    {
        this->i2cBusPort = i2cBusPort;
        this->address = address;
        this->state = 0xff;
        this->gpioType = gpioType;
        setByte(state);
    }

    /// ----------------------------------------------------------------------------------------------------------------
    /// <summary>
    /// Class destructor. It is called automatically.
    /// <para><b>Description</b></para>
    ///     This method should not be called by the user.
    ///     </summary>
    /// ----------------------------------------------------------------------------------------------------------------
    ~pcf8574()
    {
        //  setByte(0xff);//return chip to default state TODO: test if it helps.
    }


    /// ----------------------------------------------------------------------------------------------------------------
    /// <summary>
    /// Set all outputs at once by sending a state-byte.
    /// <para><b>Description</b></para>
    ///     </summary>
    /// <param name='value'>
    ///     Byte containing all output states (e.g. 0x01011100).
    ///     </param>
    /// ----------------------------------------------------------------------------------------------------------------
    void setByte(unsigned char value);


    /// ----------------------------------------------------------------------------------------------------------------
    /// <summary>
    /// Set the output-state of only one output.
    /// <para><b>Description</b></para>
    ///     </summary>
    /// <param name='outputPin'>
    ///     The pin number of the output to set (e.g. [0-8]).
    ///     </param>
    /// <param name='newState'>
    ///     The desired new state of the output (0 = Off, 1 = On).
    ///     </param>
    /// ----------------------------------------------------------------------------------------------------------------
    void setBit(int outputPin, int newState);


    /// ----------------------------------------------------------------------------------------------------------------
    /// <summary>
    /// Read all the output states at once.
    /// <para><b>Description</b></para>
    ///     Return: Byte containing all the output states.
    ///     </summary>
    /// ----------------------------------------------------------------------------------------------------------------
    unsigned char getByte();


    /// ----------------------------------------------------------------------------------------------------------------
    /// <summary>
    /// Read the output-state of only one output.
    /// <para><b>Description</b></para>
    ///     </summary>
    /// <param name='pin'>
    ///     The pin number of the output to read (e.g. [0-8]).
    ///     </param>
    /// ----------------------------------------------------------------------------------------------------------------
    uint8_t getBit(uint8_t pin);
    

    // void setAddress(uint16_t address) { this->address = address; }
    // void seti2cBusPort(const std::string& i2cBusPort) { this->i2cBusPort = i2cBusPort; }
    // uint16_t getAddress() { return address; }
    // const std::string& geti2cBusPort() { return i2cBusPort; }
};

#endif // PCF8574_HPP