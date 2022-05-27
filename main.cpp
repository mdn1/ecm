#include <unistd.h>        //Needed for I2C port
#include <fcntl.h>         //Needed for I2C port
#include <sys/ioctl.h>     //Needed for I2C port
#include <linux/i2c-dev.h> //Needed for I2C port
#include <stdio.h>         //Needed for printf
#include <unistd.h>        //Needed for sleep

#include "conf.hpp"
#include "gpio_board.hpp"
#include "pcf8574.hpp"

char i2c_bus_location[i2c_bus_adress_length];

void init()
{
    // set the location of the used i2c port of the Raspi
    snprintf(i2c_bus_location, 19, "/dev/i2c-%d", adapter_nr);
}

int main()
{
    init();
    gpio_boards::gpio_board *board0 = new gpio_boards::gpio_board(
        i2c_bus_location, gpio_boards::gpio_boards_enum::GPIO_BOARD_0);
    gpio_boards::gpio_board *board1 = new gpio_boards::gpio_board(
        i2c_bus_location, gpio_boards::gpio_boards_enum::GPIO_BOARD_1);
    gpio_boards::gpio_board *board2 = new gpio_boards::gpio_board(
        i2c_bus_location, gpio_boards::gpio_boards_enum::GPIO_BOARD_2);


    board0->write_all(~(0b00000001));
    board1->write_all(~(0b00000011));
    board2->write_all(~(0b00000111));
    sleep(2);
    board0->write_all(~(0b00000001));
    board1->write_all(~(0b00000001));
    board2->write_all(~(0b00000001));
    sleep(1);
    board0->write_all(~(0b00000010));
    board1->write_all(~(0b00000010));
    board2->write_all(~(0b00000010));
    sleep(1);
    board0->write_all(~(0b00000100));
    board1->write_all(~(0b00000100));
    board2->write_all(~(0b00000100));
    sleep(1);
    board0->write_all(~(0b00001000));
    board1->write_all(~(0b00001000));
    board2->write_all(~(0b00001000));
    sleep(1);
    board0->write_all(~(0b00010000));
    board1->write_all(~(0b00010000));
    board2->write_all(~(0b00010000));
    sleep(1);
    board0->write_all(~(0b00100000));
    board1->write_all(~(0b00100000));
    board2->write_all(~(0b00100000));
    sleep(1);
    board0->write_all(0xff);
    board1->write_all(0xff);
    board2->write_all(0xff);
    delete board0;
    delete board1;
    delete board2;
}