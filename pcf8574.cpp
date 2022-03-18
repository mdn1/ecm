#include "pcf8574.hpp"

// Example:  "/dev/i2c-1", 0x01
pcf8574::pcf8574(char *i2c_bus_location, uint16_t address)
{
    strcpy(this->i2c_bus_location, i2c_bus_location);
    this->address = address;
    this->state = 0xff;
    setByte(state);
}

void pcf8574::setByte(unsigned char value)
{
    struct i2c_smbus_ioctl_data data;
    data.read_write = I2C_SMBUS_WRITE;
    data.size = I2C_SMBUS_BYTE;
    data.data = NULL;
    data.command = value;

    int device;
    if ((device = open(i2c_bus_location, O_RDWR)) == -1)
    {
        fprintf(stderr,
                "ERROR: pcf8574::setByte failed to open i2c bus at %s\n", i2c_bus_location);
        exit(EXIT_FAILURE);
    }

    ioctl(device, I2C_SLAVE, address);
    ioctl(device, I2C_SMBUS, &data);
    close(device);
    state = value;
}

void pcf8574::setBit(int pin, int value)
{
    char mask = 0x01 << pin;
    setByte(value ? (state | mask) : (state & (~mask)));
}

unsigned char pcf8574::getByte()
{
    union i2c_smbus_data data_union;
    struct i2c_smbus_ioctl_data data;
    data.read_write = I2C_SMBUS_READ;
    data.size = I2C_SMBUS_BYTE;
    data.data = &data_union;
    data.command = 0;

    int device;
    if ((device = open(i2c_bus_location, O_RDWR)) == -1)
    {
        fprintf(stderr,
                "ERROR: pcf8574::getByte failed to open i2c bus at %s\n", i2c_bus_location);
        exit(EXIT_FAILURE);
    }

    ioctl(device, I2C_SLAVE, address);
    ioctl(device, I2C_SMBUS, &data);
    close(device);
    return data_union.byte;
}

uint8_t pcf8574::getBit(uint8_t pin)
{
    char mask = 0x01 << pin;
    return getByte() & mask ? 1 : 0;
}

pcf8574::~pcf8574()
{
    //  setByte(0xff);//return chip to default state TODO: test if it helps.
}