//======================================================================================================================
// C O P Y R I G H T
//======================================================================================================================
/// \file       gpio_board.hpp
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
#ifndef GPIO_BOARDS_GPIO_BOARD_HPP
#define GPIO_BOARDS_GPIO_BOARD_HPP 1


//======================================================================================================================
// Inclusions
//======================================================================================================================
#include <cstdint>
#include <string.h>

#include "pcf8574_io.hpp"


//======================================================================================================================
// Type Definitions / Enums / Defines / Macros / Consts
//======================================================================================================================
namespace gpio_boards
{
    /*
     * A maximum of 8 pcf8574 are allowed on the same I2C Bus.
     * This is based on the fact that pcf8574 only have a maximum
     * of 8 custom adresses.
     */
    enum class gpio_boards_enum
    {
        GPIO_BOARD_0,
        GPIO_BOARD_1,
        GPIO_BOARD_2,
        GPIO_BOARD_3,
        GPIO_BOARD_4,
        GPIO_BOARD_5,
        GPIO_BOARD_6,
        GPIO_BOARD_7
    };

    /*
     * On the pcf8574 the adress is coded as follow:
     * A2:L, A1:L, A0:L
     */
    struct gpio_board_0
    {
        uint16_t write_adress = 0x40 >> 1; //20
        uint16_t read_adress = 0x41 >> 1;
    };

    /*
     * On the pcf8574 the adress is coded as follow:
     * A2:L, A1:L, A0:H
     */
    struct gpio_board_1
    {
        uint16_t write_adress = 0x42 >> 1; //21
        uint16_t read_adress = 0x43 >> 1;
    };

    /*
     * On the pcf8574 the adress is coded as follow:
     * A2:L, A1:H, A0:L
     */
    struct gpio_board_2
    {
        uint16_t write_adress = 0x44 >> 1;
        uint16_t read_adress = 0x45 >> 1;
    };

    /*
     * On the pcf8574 the adress is coded as follow:
     * A2:L, A1:H, A0:H
     */
    struct gpio_board_3
    {
        uint16_t write_adress = 0x46 >> 1;
        uint16_t read_adress = 0x47 >> 1;
    };

    /*
     * On the pcf8574 the adress is coded as follow:
     * A2:H, A1:L, A0:L
     */
    struct gpio_board_4
    {
        uint16_t write_adress = 0x48 >> 1;
        uint16_t read_adress = 0x49 >> 1;
    };

    /*
     * On the pcf8574 the adress is coded as follow:
     * A2:H, A1:L, A0:H
     */
    struct gpio_board_5
    {
        uint16_t write_adress = 0x4A >> 1;
        uint16_t read_adress = 0x4B >> 1;
    };

    /*
     * On the pcf8574 the adress is coded as follow:
     * A2:H, A1:H, A0:L
     */
    struct gpio_board_6
    {
        uint16_t write_adress = 0x4C >> 1;
        uint16_t read_adress = 0x4D >> 1;
    };

    /*
     * On the pcf8574 the adress is coded as follow:
     * A2:H, A1:H, A0:H
     */
    struct gpio_board_7
    {
        uint16_t write_adress = 0x4E >> 1;
        uint16_t read_adress = 0x4F >> 1;
    };
} // namespace gpio_boards

//======================================================================================================================
// Extern Variables
//======================================================================================================================


//======================================================================================================================
// External Constants
//======================================================================================================================


//======================================================================================================================
// Prototypes
//======================================================================================================================
/// ----------------------------------------------------------------------------------------------------------------
/// <summary>
/// One line description.
/// <para><b>Description</b></para>
///     Multiple line description.
///     Return: U8 status, 0=OK (no error)
///     </summary>
/// <param name='param1'>
///     Description of the first paramter.
///     </param>
/// <param name='param2'>
///     Description of the second paramter.
///     </param>
/// ----------------------------------------------------------------------------------------------------------------
//static uint8_t exampleMethod (uint32_t* param1, uint32_t param2);



namespace gpio_boards
{
    class gpio_board
    {

    private:
        pcf8574 *write_handle;
        pcf8574 *read_handle;
        gpio_boards_enum board_number;
    public:
        gpio_board(const std::string& i2cBusPort, gpio_boards_enum board_number);
        ~gpio_board();
        void write(uint8_t output_pin, uint8_t value);
        void write_all(unsigned char value);
        uint8_t read(uint8_t input_pin); // pin to read the status from
        unsigned char read_all();
        // int get_board_address() { return address; }
        // void set_board_address(int address) { this->address = address; }
        // char *geti2cBusPort() { return i2cBusPort; }
        // void seti2cBusPort(char *i2cBusPort) { strcpy(this->i2cBusPort, i2cBusPort); }
    };
} // namespace gpio_boards

#endif // GPIO_BOARDS_GPIO_BOARD_HPP