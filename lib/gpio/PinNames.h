#pragma once
#include <driver/gpio.h>

enum PinName{
    P0,
    P1,
    P2,
    P3,
    P4,
    P5,
    P6,
    P7,
    P8,
    P9,
    P10,
    P11,
    P12,
    P13,
    P14,
    P15,
    P16,
    P17,
    P18,
    P19,
    P20,
    P21,
    P22,
    P23,
    P24,
    P25,
    P26,
    P27,
    P28,
    P29,
    P30,
    P31,
    P32,
    P33,
    P34,
    P35,
    P36,
    P37,
    P38,
    P39,
    E00,
    E01,
    E02,
    E03,
    E04,
    E05,
    E06,
    E07,
    E10,
    E11,
    E12,
    E13,
    E14,
    E15,
    E16,
    E17
};

const PinName Pe0A=P4;
const PinName Pe0B=P5;
const PinName Pe0C=P12;
const PinName Pe0D=P13;
const PinName Pe1A=P14;
const PinName Pe1B=P15;
const PinName Pe1C=P16;
const PinName Pe1D=P17;
const PinName Pe2A=P18;
const PinName Pe2B=P19;
const PinName Pe2C=P26;
const PinName Pe2D=P27;
const PinName USER=P2;

enum PinMode{
    OUTPUT,
    INPUT_PU,
    INPUT_PD,
    INPUT
};

enum Device{
    INTERNAL,
    EXPANDER
};
