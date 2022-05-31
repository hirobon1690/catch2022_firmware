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

enum PinMode{
    OUTPUT,
    INPUT_PU,
    INPUT_PD
};

enum Device{
    INTERNAL,
    EXPANDER
};
