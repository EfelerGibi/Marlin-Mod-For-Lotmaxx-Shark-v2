/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "stdint.h"

typedef enum : uint8_t {
    PRINT3D,
    LASER,
}printmode_t;

class Printmode{
private:
    static printmode_t currentprintmode;
public:
    Printmode() { currentprintmode = PRINT3D; }
    static void set_printmode(const printmode_t _printmode) { currentprintmode = _printmode; }
    static printmode_t get_printmode()  { return currentprintmode; }
};

#define IS_LASER_PRINTMODE()    (printmode.get_printmode() == LASER)
#define IS_PRINT3D_PRINTMODE()  (printmode.get_printmode() == PRINT3D)

extern Printmode printmode;