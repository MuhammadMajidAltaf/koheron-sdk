/// (c) Koheron

#include "gpio.hpp"

Gpio::Gpio(Klib::DevMem& dev_mem_)
: dev_mem(dev_mem_)
{
    status = CLOSED;
}

int Gpio::Open()
{
    if (status == CLOSED) {
        auto ids = dev_mem.RequestMemoryMaps<1>({{
            { GPIO_ADDR, GPIO_RANGE }
        }});

        if (dev_mem.CheckMapIDs(ids) < 0) {
            status = FAILED;
            return -1;
        }

        dev_num = ids[0];
        status = OPENED;
    }

    return 0;
}

int get_value_offset(uint32_t channel)
{
    if (channel == 0 || channel > 2)
        return -1;

    return (channel == 1 ? CHAN1_VALUE_OFF : CHAN2_VALUE_OFF);
}

int get_dir_offset(uint32_t channel)
{
    if (channel == 0 || channel > 2)
        return -1;

    return (channel == 1 ? CHAN1_DIR_OFF : CHAN2_DIR_OFF);
}


int Gpio::set_bit(uint32_t index, uint32_t channel)
{
    int offset = get_value_offset(channel);

    if (offset < 0 || index > MAX_BIT_IDX)
        return -1;

    Klib::SetBit(dev_mem.GetBaseAddr(dev_num) + offset, index);
    return 0;
}

int Gpio::clear_bit(uint32_t index, uint32_t channel)
{
    int offset = get_value_offset(channel);

    if (offset < 0 || index > MAX_BIT_IDX)
        return -1;

    Klib::ClearBit(dev_mem.GetBaseAddr(dev_num) + offset, index);
    return 0;
}

int Gpio::toggle_bit(uint32_t index, uint32_t channel)
{
    int offset = get_value_offset(channel);

    if(offset < 0 || index > MAX_BIT_IDX)
        return -1;

    Klib::ToggleBit(dev_mem.GetBaseAddr(dev_num) + offset, index);
    return 0;
}

int Gpio::set_as_input(uint32_t index, uint32_t channel)
{
    int offset = get_dir_offset(channel);

    if(offset < 0 || index > MAX_BIT_IDX)
        return -1;

    Klib::SetBit(dev_mem.GetBaseAddr(dev_num) + offset, index);
    return 0;
}

int Gpio::set_as_output(uint32_t index, uint32_t channel)
{
    int offset = get_dir_offset(channel);

    if(offset < 0 || index > MAX_BIT_IDX)
        return -1;

    Klib::ClearBit(dev_mem.GetBaseAddr(dev_num) + offset, index);
    return 0;
}