// Header protection
#ifndef GPIO_BOARDS_GPIO_BOARD_HPP
#define GPIO_BOARDS_GPIO_BOARD_HPP 1

#include <cstdint>
#include <string.h>

#include "pcf8574.hpp"

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

    class gpio_board
    {
    public:
        gpio_board(char *i2c_bus_location, gpio_boards_enum board_number);
        ~gpio_board();
        void write(uint8_t output_pin, uint8_t value);
        void write_all(unsigned char value);
        uint8_t read(uint8_t input_pin); // pin to read the status from
        unsigned char read_all(uint8_t input_pin);
        // int get_board_address() { return address; }
        // void set_board_address(int address) { this->address = address; }
        // char *geti2c_bus_location() { return i2c_bus_location; }
        // void seti2c_bus_location(char *i2c_bus_location) { strcpy(this->i2c_bus_location, i2c_bus_location); }

    private:
        pcf8574 *write_handle;
        pcf8574 *read_handle;
        gpio_boards_enum board_number;
        // char i2c_bus_location[256];
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
#endif // GPIO_BOARDS_GPIO_BOARD_HPP