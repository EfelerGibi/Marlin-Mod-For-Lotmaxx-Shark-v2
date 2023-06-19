
#pragma once

//  logic_position = native_position + offset
//  so more bigger the offset is, the more lower position the native_position is.
constexpr float MAX_ZOFFSET = 3.0;
constexpr float MIN_ZOFFSET = -5.0;

constexpr uint16_t AXISMOVESPEED = 600;

enum : char{
  X_AXIS_CHAR = 'X',
  Y_AXIS_CHAR = 'Y',
  Z_AXIS_CHAR = 'Z',
  E_AXIS_CHAR = 'E',
};

enum : char{
  SUB_CHAR = '-',
  ADD_CHAR = '+',
};



void Move(char axisCode, float moveValue,char sign, uint16_t speed);
