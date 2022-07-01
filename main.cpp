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



gpio_boards::gpio_board *ledPanel1 = new gpio_boards::gpio_board(
    i2cBusPort, gpio_boards::gpio_boards_enum::GPIO_BOARD_1);
gpio_boards::gpio_board *ledPanel2 = new gpio_boards::gpio_board(
    i2cBusPort, gpio_boards::gpio_boards_enum::GPIO_BOARD_2);
gpio_boards::gpio_board *ledPanel3 = new gpio_boards::gpio_board(
    i2cBusPort, gpio_boards::gpio_boards_enum::GPIO_BOARD_3);
gpio_boards::gpio_board *ledPanel4 = new gpio_boards::gpio_board(
    i2cBusPort, gpio_boards::gpio_boards_enum::GPIO_BOARD_4);
gpio_boards::gpio_board *ledPanel5 = new gpio_boards::gpio_board(
    i2cBusPort, gpio_boards::gpio_boards_enum::GPIO_BOARD_5);
gpio_boards::gpio_board *ledPanel6 = new gpio_boards::gpio_board(
    i2cBusPort, gpio_boards::gpio_boards_enum::GPIO_BOARD_6);
gpio_boards::gpio_board *ledPanel7 = new gpio_boards::gpio_board(
    i2cBusPort, gpio_boards::gpio_boards_enum::GPIO_BOARD_7);
gpio_boards::gpio_board *ledPanel8 = new gpio_boards::gpio_board(
    i2cBusPort, gpio_boards::gpio_boards_enum::GPIO_BOARD_8);

pthread_t tcpThread;

int updateOutputs(std::vector<std::string> newState);

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

                                // Parse the strting content.
                                int ret = updateOutputs(splittedMsg);
                        }
                        else
                        {
                                pthread_mutex_unlock(&mutex);
                        }
                }
        }

        delete ledPanel1;
        delete ledPanel2;
        delete ledPanel3;
        delete ledPanel4;
        delete ledPanel5;
        delete ledPanel6;
        delete ledPanel7;
        delete ledPanel8;

        pthread_join(tcpThread, NULL);
        printf("Application terminated.\n");
}

int updateOutputs(std::vector<std::string> newState)
{
        commandType command;
        argumentType argument;

        //******************************************
        //              Parse command
        //******************************************
        // At least 1 word for the command.
        if (newState.size() < 1)
        {
                // No message sent.
                return 1;
        }

        // Parse first word.
        if (newState[0] == commandControl)
        {
                command = commandType::commandControl;
        }
        else if (0)
        {
                // currently not in use
                // command = commandType::XYZ;
        }
        else
        {
                // No recognized command found.
                return 2;
        }

        //******************************************
        //              Parse argument
        //******************************************
        // At least 1 word for the argument.
        if (newState.size() < 2)
        {
                // No argument found.
                return 3;
        }

        // Parse the argument
        if (newState[1] == argumentLed)
        {
                argument = argumentType::argumentLed;
        }
        else if (0)
        {
                // currently not in use
                // argument = argumentType::XYZ;
        }
        else
        {
                // No recognized argument found.
                return 4;
        }

        //******************************************
        //              Parse parameters
        //******************************************
        // At least 1 word for the parameters.
        if (newState.size() < 3)
        {
                // No parameter sent.
                return 5;
        }

        // Control Led selected.
        if (command == commandType::commandControl && argument == argumentType::argumentLed)
        {
                int positionInMsg = 2; // Posistion 0 is the command. Position 1 is the argument.
                size_t pos = 0;
                

                while (positionInMsg <= newState.size() - 1 )
                {
                        if ((pos = newState[positionInMsg].find(tcpMsgParameterDelimiter)) == std::string::npos)
                        {
                                // Error with parameter format.
                                return 6;
                        }

                        // Parameter parameterPanel= "--panel" found.
                        if(newState[positionInMsg].find(parameterPanel) != std::string::npos)
                        {
                                char newPanelValue = 0b00000000;
                                std::string binString = "";

                                // Extract binary value from string.
                                binString = newState[positionInMsg].substr(pos+ tcpMsgParameterDelimiter.size());

                                // Convert string to binary number (char).
                                newPanelValue = std::stoi(binString, nullptr, 2);

                                // Delete binary value "=XXXXXXX".
                                newState[positionInMsg].erase(pos);
                                
                                // Update the correct led panel.
                                if (newState[positionInMsg] == parameterPanel1)
                                {
                                        std::cout << "Panel 1 activated." << std::endl;
                                        ledPanel1->write_all(~newPanelValue);
                                }
                                else if (newState[positionInMsg] == parameterPanel2)
                                {
                                        std::cout << "Panel 2 activated." << std::endl;
                                        ledPanel2->write_all(~newPanelValue);
                                }
                                else if (newState[positionInMsg] == parameterPanel3)
                                {
                                        std::cout << "Panel 3 activated." << std::endl;
                                        ledPanel3->write_all(~newPanelValue);
                                }
                                else if (newState[positionInMsg] == parameterPanel4)
                                {
                                        std::cout << "Panel 4 activated." << std::endl;
                                        ledPanel4->write_all(~newPanelValue);
                                }
                                else if (newState[positionInMsg] == parameterPanel5)
                                {
                                        std::cout << "Panel 5 activated." << std::endl;
                                        ledPanel5->write_all(~newPanelValue);
                                }
                                else if (newState[positionInMsg] == parameterPanel6)
                                {
                                        std::cout << "Panel 6 activated." << std::endl;
                                        ledPanel6->write_all(~newPanelValue);
                                }
                                else if (newState[positionInMsg] == parameterPanel7)
                                {
                                        std::cout << "Panel 7 activated." << std::endl;
                                        ledPanel7->write_all(~newPanelValue);
                                }
                                else if (newState[positionInMsg] == parameterPanel8)
                                {
                                        std::cout << "Panel 8 activated." << std::endl;
                                        ledPanel8->write_all(~newPanelValue);
                                }
                        }
                        positionInMsg ++;
                }
        }

        // Ok.
        return 0;
}