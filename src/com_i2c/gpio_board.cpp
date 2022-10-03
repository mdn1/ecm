//======================================================================================================================
// C O P Y R I G H T
//======================================================================================================================
/// \file       gpio_board.cpp
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
#include "gpio_board.hpp"


//======================================================================================================================
// Type Definitions / Enums / Defines / Macros / Consts
//======================================================================================================================


//======================================================================================================================
// Local Method Prototypes
//======================================================================================================================


//======================================================================================================================
// Variables, Objects
//======================================================================================================================
namespace gpio_boards
{
    std::vector<gpio_board *> allLedPanels;
}

//======================================================================================================================
// Local Implementation
//======================================================================================================================


//======================================================================================================================
// API Implementation
//======================================================================================================================
namespace gpio_boards
{
    /// ------------------------------------------------------------------------------------------------------------
    /// See description in Header file.
    /// ------------------------------------------------------------------------------------------------------------
    gpio_board::gpio_board(const std::string& i2cBusPort, gpio_boards_enum board_number)
    {
        this->board_number = board_number;

        switch (this->board_number)
        {
        case gpio_boards_enum::GPIO_BOARD_1:
        {
            gpio_board_1 board;
            write_handle = new pcf8574(i2cBusPort, board.write_adress, gpioType_t::Output);
            read_handle = new pcf8574(i2cBusPort, board.read_adress, gpioType_t::Input);
            break;
        }
        case gpio_boards_enum::GPIO_BOARD_2:
        {
            gpio_board_2 board;
            write_handle = new pcf8574(i2cBusPort, board.write_adress, gpioType_t::Output);
            read_handle = new pcf8574(i2cBusPort, board.read_adress, gpioType_t::Input);
            break;
        }
        case gpio_boards_enum::GPIO_BOARD_3:
        {
            gpio_board_3 board;
            write_handle = new pcf8574(i2cBusPort, board.write_adress, gpioType_t::Output);
            read_handle = new pcf8574(i2cBusPort, board.read_adress, gpioType_t::Input);
            break;
        }
        case gpio_boards_enum::GPIO_BOARD_4:
        {
            gpio_board_4 board;
            write_handle = new pcf8574(i2cBusPort, board.write_adress, gpioType_t::Output);
            read_handle = new pcf8574(i2cBusPort, board.read_adress, gpioType_t::Input);
            break;
        }
        case gpio_boards_enum::GPIO_BOARD_5:
        {
            gpio_board_5 board;
            write_handle = new pcf8574(i2cBusPort, board.write_adress, gpioType_t::Output);
            read_handle = new pcf8574(i2cBusPort, board.read_adress, gpioType_t::Input);
            break;
        }
        case gpio_boards_enum::GPIO_BOARD_6:
        {
            gpio_board_6 board;
            write_handle = new pcf8574(i2cBusPort, board.write_adress, gpioType_t::Output);
            read_handle = new pcf8574(i2cBusPort, board.read_adress, gpioType_t::Input);
            break;
        }
        case gpio_boards_enum::GPIO_BOARD_7:
        {
            gpio_board_7 board;
            write_handle = new pcf8574(i2cBusPort, board.write_adress, gpioType_t::Output);
            read_handle = new pcf8574(i2cBusPort, board.read_adress, gpioType_t::Input);
            break;
        }
        case gpio_boards_enum::GPIO_BOARD_8:
        {
            gpio_board_8 board;
            write_handle = new pcf8574(i2cBusPort, board.write_adress, gpioType_t::Output);
            read_handle = new pcf8574(i2cBusPort, board.read_adress, gpioType_t::Input);
            break;
        }
        }

        allLedPanels.push_back(this);
    }


    /// ------------------------------------------------------------------------------------------------------------
    /// See description in Header file.
    /// ------------------------------------------------------------------------------------------------------------
    gpio_board::~gpio_board()
    {
        delete write_handle;
        delete read_handle;

        // Remove the deleted panel from the list of all panels.
        allLedPanels.erase(std::remove(allLedPanels.begin(), allLedPanels.end(), this), allLedPanels.end());
    }

    void gpio_board::write(uint8_t output_pin, uint8_t value)
    {
        write_handle->setBit(output_pin, value);
    }

    void gpio_board::write_all(std::bitset<8> value)
    {
        // Flip bits because the the outputs need to be pulled down to be activated
        std::bitset<8> inversedValue = ~value;
        unsigned char valueAsChar = static_cast<unsigned char>(inversedValue.to_ulong());
        
        write_handle->setByte(valueAsChar);
    }

    uint8_t gpio_board::read(uint8_t input_pin)
    {
        return read_handle->getBit(input_pin);
    }

    unsigned char gpio_board::read_all()
    {
        return read_handle->getByte();
    }
}