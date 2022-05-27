#include "gpio_board.hpp"

namespace gpio_boards
{

    gpio_board::gpio_board(const std::string& i2cBusPort, gpio_boards_enum board_number)
    {
        this->board_number = board_number;

        switch (this->board_number)
        {
        case gpio_boards_enum::GPIO_BOARD_0:
        {
            gpio_board_0 board;
            write_handle = new pcf8574(i2cBusPort, board.write_adress);
            read_handle = new pcf8574(i2cBusPort, board.read_adress);
            break;
        }
        case gpio_boards_enum::GPIO_BOARD_1:
        {
            gpio_board_1 board;
            write_handle = new pcf8574(i2cBusPort, board.write_adress);
            read_handle = new pcf8574(i2cBusPort, board.read_adress);
            break;
        }
        case gpio_boards_enum::GPIO_BOARD_2:
        {
            gpio_board_2 board;
            write_handle = new pcf8574(i2cBusPort, board.write_adress);
            read_handle = new pcf8574(i2cBusPort, board.read_adress);
            break;
        }
        case gpio_boards_enum::GPIO_BOARD_3:
        {
            gpio_board_3 board;
            write_handle = new pcf8574(i2cBusPort, board.write_adress);
            read_handle = new pcf8574(i2cBusPort, board.read_adress);
            break;
        }
        case gpio_boards_enum::GPIO_BOARD_4:
        {
            gpio_board_4 board;
            write_handle = new pcf8574(i2cBusPort, board.write_adress);
            read_handle = new pcf8574(i2cBusPort, board.read_adress);
            break;
        }
        case gpio_boards_enum::GPIO_BOARD_5:
        {
            gpio_board_5 board;
            write_handle = new pcf8574(i2cBusPort, board.write_adress);
            read_handle = new pcf8574(i2cBusPort, board.read_adress);
            break;
        }
        case gpio_boards_enum::GPIO_BOARD_6:
        {
            gpio_board_6 board;
            write_handle = new pcf8574(i2cBusPort, board.write_adress);
            read_handle = new pcf8574(i2cBusPort, board.read_adress);
            break;
        }
        case gpio_boards_enum::GPIO_BOARD_7:
        {
            gpio_board_7 board;
            write_handle = new pcf8574(i2cBusPort, board.write_adress);
            read_handle = new pcf8574(i2cBusPort, board.read_adress);
            break;
        }
        }
    }

    gpio_board::~gpio_board()
    {
        delete write_handle;
        delete read_handle;
    }

    void gpio_board::write(uint8_t output_pin, uint8_t value)
    {
        write_handle->setBit(output_pin, value);
    }

    void gpio_board::write_all(unsigned char value)
    {
        write_handle->setByte(value);
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