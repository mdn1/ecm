#include <unistd.h>        //Needed for I2C port
#include <fcntl.h>         //Needed for I2C port
#include <sys/ioctl.h>     //Needed for I2C port
#include <linux/i2c-dev.h> //Needed for I2C port
#include <stdio.h>         //Needed for printf
#include <signal.h>        //Needed for keyboard signal capturing.
#include <unistd.h>        //Needed for sleep
#include <iostream>        //Needed for cout

#include "conf.hpp"
#include "gpio_board.hpp"
#include "tcp.hpp"
#include "tcp_message.hpp"

gpio_boards::gpio_board *board0 = new gpio_boards::gpio_board(
    i2cBusPort, gpio_boards::gpio_boards_enum::GPIO_BOARD_0);
gpio_boards::gpio_board *board1 = new gpio_boards::gpio_board(
    i2cBusPort, gpio_boards::gpio_boards_enum::GPIO_BOARD_1);
gpio_boards::gpio_board *board2 = new gpio_boards::gpio_board(
    i2cBusPort, gpio_boards::gpio_boards_enum::GPIO_BOARD_2);

pthread_t tcpThread;

void init()
{
        // set the location of the used i2c port of the Raspi
        // snprintf(i2cBusPort, 19, i2cPort, i2cAdapterNr); //   "/dev/i2c-%d"
}

// Define the function to be called when ctrl-c (SIGINT) is sent to process
void signal_callback_handler(sig_atomic_t signum)
{
        if (signum == 2)
        {
                printf("\n***Shutdown requested.***\n");
                shutdownOccured = true;
        }
}

int main(int argc, char **argv)
{
        int err = 0;

        // Register signal and signal handler
        signal(SIGINT, signal_callback_handler);

        init();

        // Start TCP Thread.
        err = pthread_create(&tcpThread, NULL, startTcpServer, NULL);
        if (err)
        {
                std::cout << "TCP thread creation failed : " << strerror(err);
        }
        else
        {
                std::cout << "TCP thread created with ID : " << tcpThread << std::endl;
        }

        while (shutdownOccured == false)
        {
                // Check if it is safe to use tcpTxMsg.
                if (pthread_mutex_trylock(&mutex) == 0)
                {
                        if (tcpRxMsgs.empty() == false)
                        {
                                std::string lastMsg = tcpRxMsgs.front().getPayload();

                                // Remove last message from queue.
                                tcpRxMsgs.pop();

                                // Release resources.
                                pthread_mutex_unlock(&mutex);

                                // Split message for parsing.
                                std::vector<std::string> splittedMsg = TcpMessage::splitMsg(lastMsg);
                        }
                        else
                        {
                                pthread_mutex_unlock(&mutex);
                        }
                }

                // board0->write_all(~(0b00000001));
                // // std::cout << board0->read_all() << '\n';
                // board1->write_all(~(0b00000011));
                // board2->write_all(~(0b00000111));
                // sleep(1);
                // board0->write_all(~(0b00000001));
                // // std::cout << board0->read_all();
                // board1->write_all(~(0b00000001));
                // board2->write_all(~(0b00000001));
                // sleep(1);
                // board0->write_all(~(0b00000010));
                // // std::cout << board0->read_all();
                // board1->write_all(~(0b00000010));
                // board2->write_all(~(0b00000010));
                // sleep(1);
                // board0->write_all(~(0b00000100));
                // // std::cout << board0->read_all();
                // board1->write_all(~(0b00000100));
                // board2->write_all(~(0b00000100));
                // sleep(1);
                // board0->write_all(~(0b00001000));
                // board1->write_all(~(0b00001000));
                // board2->write_all(~(0b00001000));
                // sleep(1);
                // board0->write_all(~(0b00010000));
                // board1->write_all(~(0b00010000));
                // board2->write_all(~(0b00010000));
                // sleep(1);
                // board0->write_all(~(0b00100000));
                // board1->write_all(~(0b00100000));
                // board2->write_all(~(0b00100000));
                // sleep(1);
                // board0->write_all(0xff);
                // board1->write_all(0xff);
                // board2->write_all(0xff);
        }

        delete board0;
        delete board1;
        delete board2;

        pthread_join(tcpThread, NULL);
        printf("Application terminated.\n");
}