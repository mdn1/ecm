#include <unistd.h>        //Needed for I2C port
#include <fcntl.h>         //Needed for I2C port
#include <sys/ioctl.h>     //Needed for I2C port
#include <linux/i2c-dev.h> //Needed for I2C port
#include <stdio.h>         //Needed for printf
#include <signal.h>        //Needed for keyboard signal capturing.
#include <unistd.h>        //Needed for sleep
#include <iostream>        //Needed for cout
#include <bitset>

#include "conf.hpp"
#include "gpio_board.hpp"
#include "tcp.hpp"
#include "tcp_message.hpp"

gpio_boards::gpio_board ledPanel1(i2cBusPort, gpio_boards::gpio_boards_enum::GPIO_BOARD_1);
gpio_boards::gpio_board ledPanel2(i2cBusPort, gpio_boards::gpio_boards_enum::GPIO_BOARD_2);
gpio_boards::gpio_board ledPanel3(i2cBusPort, gpio_boards::gpio_boards_enum::GPIO_BOARD_3);
gpio_boards::gpio_board ledPanel4(i2cBusPort, gpio_boards::gpio_boards_enum::GPIO_BOARD_4);
gpio_boards::gpio_board ledPanel5(i2cBusPort, gpio_boards::gpio_boards_enum::GPIO_BOARD_5);
gpio_boards::gpio_board ledPanel6(i2cBusPort, gpio_boards::gpio_boards_enum::GPIO_BOARD_6);
gpio_boards::gpio_board ledPanel7(i2cBusPort, gpio_boards::gpio_boards_enum::GPIO_BOARD_7);
gpio_boards::gpio_board ledPanel8(i2cBusPort, gpio_boards::gpio_boards_enum::GPIO_BOARD_8);

pthread_t tcpThread;

// Forward declarations.
bool executeCommand(parsedMsgType msg);
bool updateOutputs(std::map<parameterType, std::bitset<8>> outputStates);
void runOutputTest();


void init()
{
        runOutputTest();
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
                                parsedMsgType newState = TcpMessage::parseMsg(splittedMsg);

                                // Update the outputs.
                                executeCommand(newState);
                        }
                        else
                        {
                                pthread_mutex_unlock(&mutex);
                        }
                }
        }

        pthread_join(tcpThread, NULL);
        printf("Application terminated.\n");
}

bool executeCommand(parsedMsgType msg)
{
        if (msg.parsingError != 0)
        {
                return false;
        }

        switch (msg.command)
        {
        case commandType::commandTest:
                runOutputTest();
                break;
        case commandType::commandControl:
                if (msg.argument == argumentType::argumentLed)
                {
                        if (msg.parameters.size() > 0)
                        {
                                updateOutputs(msg.parameters);
                        }
                }
                else
                {
                        return false;
                }
                break;
        }

        // Ok.
        return true;
}

bool updateOutputs(std::map<parameterType, std::bitset<8>> outputStates)
{
        // Search if any command to ALL the panels have been given.
        auto search = outputStates.find(parameterType::parameterPanelAll);
        if (search != outputStates.end())
        {
                std::cout <<"smth asked" <<std::endl;
                if(search->second == gpio_boards::allOn)
                {
                        // All On
                        for (gpio_boards::gpio_board *ledPanelX : gpio_boards::allLedPanels)
                        {
                                ledPanelX->write_all(gpio_boards::allOn);
                                return true;
                        }
                }
                else if (search->second == gpio_boards::allOff)
                {
                        // All Off
                        for (gpio_boards::gpio_board *ledPanelX : gpio_boards::allLedPanels)
                        {
                                ledPanelX->write_all(gpio_boards::allOff);
                                return true;
                        }
                }
        }

        for (auto itr = outputStates.begin(); itr != outputStates.end(); ++itr)
        {
                switch (itr->first)
                {
                case parameterType::parameterPanel1:
                        ledPanel1.write_all(itr->second);
                        break;
                case parameterType::parameterPanel2:
                        ledPanel2.write_all(itr->second);
                        break;
                case parameterType::parameterPanel3:
                        ledPanel3.write_all(itr->second);
                        break;
                case parameterType::parameterPanel4:
                        ledPanel4.write_all(itr->second);
                        break;
                case parameterType::parameterPanel5:
                        ledPanel5.write_all(itr->second);
                        break;
                case parameterType::parameterPanel6:
                        ledPanel6.write_all(itr->second);
                        break;
                case parameterType::parameterPanel7:
                        ledPanel7.write_all(itr->second);
                        break;
                case parameterType::parameterPanel8:
                        ledPanel8.write_all(itr->second);
                        break;
                }
        }

        // Ok.
        return true;
}


void runOutputTest()
{
        std::bitset<8> startValue = 0b00000001;

        // On-Off cycles.
        for (int repetition = 0; repetition < repetitionsForOutputTest; repetition++)
        {
                // All On
                for (gpio_boards::gpio_board *ledPanelX : gpio_boards::allLedPanels)
                {
                        ledPanelX->write_all(gpio_boards::allOn);
                }
                usleep(500000);

                // All Off
                for (gpio_boards::gpio_board *ledPanelX : gpio_boards::allLedPanels)
                {
                        ledPanelX->write_all(gpio_boards::allOff);
                }
                usleep(100000);
        }

        // Iterate every LED panel.
        for (gpio_boards::gpio_board *ledPanelX : gpio_boards::allLedPanels)
        {
                // Iterate every LED of the panel.
                for (int i = 0; i <= 6; i++) // = 6 so it turns off.
                {
                        ledPanelX->write_all(startValue << i);
                        usleep(delayLedJumpForOutputTestMs *1000); //ms -> µs
                }
        }
}