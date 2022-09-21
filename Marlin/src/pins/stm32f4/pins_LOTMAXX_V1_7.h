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

#define ALLOW_STM32DUINO
#include "env_validate.h"

#if HOTENDS > 2 || E_STEPPERS > 2
  #error "Lotmaxx V1.7 supports up to 1 hotends / E-steppers."
#endif

#define BOARD_INFO_NAME "Lotmaxx V1_7"

#if MOTHERBOARD == BOARD_LOTMAXX_V1_7
  #define LOTMAXX_MULTI_LANGUAGE
#endif

// Avoid conflict with TIMER_TONE
#define STEP_TIMER 10

//
// BLTOUCH
//
#define SERVO0_PIN                                PB1   // Enable BLTOUCH
#define Z_MIN_PROBE_PIN                           PB0

//
// EEPROM
//
#define I2C_EEPROM
#define SOFT_I2C_EEPROM
#define MARLIN_EEPROM_SIZE                        0x2000  // 8KB
#define I2C_SDA_PIN                               PB10
#define I2C_SCL_PIN                               PB11

//
// Limit Switches
//
#define X_STOP_PIN                          PD15
#define Y_STOP_PIN                          PD14
#define Z_MIN_PIN                           PD13

#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PD11  // MT_DET
  #define FIL_RUNOUT2_PIN                   PD10
#endif

//
// Steppers
//
#define X_ENABLE_PIN                        PA6
#define X_STEP_PIN                          PA5
#define X_DIR_PIN                           PA4

#define Y_ENABLE_PIN                        PA3
#define Y_STEP_PIN                          PE7
#define Y_DIR_PIN                           PA2

#define Z_ENABLE_PIN                        PC3
#define Z_STEP_PIN                          PC2
#define Z_DIR_PIN                           PC1

#define E0_ENABLE_PIN                       PC15
#define E0_STEP_PIN                         PC14
#define E0_DIR_PIN                          PC13

#define E1_ENABLE_PIN                       PE6
#define E1_STEP_PIN                         PE5
#define E1_DIR_PIN                          PE4

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PA7   // TH1
#define TEMP_1_PIN                          PC5   // TH2
#define TEMP_BED_PIN                        PC4   // TB1

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PE11   // HEATER1
#define HEATER_1_PIN                        PD14   // HEATER2
#define HEATER_BED_PIN                      PE10   // HOT BED

#define FAN_PIN                             PE8    // FAN

#define LED_PIN                             PB2

#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION ONBOARD
#endif

//#define USE_NEW_SPI_API 1

//
// Onboard SD card
// NOT compatible with LCD
//
// Detect pin doesn't work when ONBOARD and NO_SD_HOST_DRIVE disabled

#define ENABLE_SPI3
#define SD_SS_PIN                       -1
#define SDSS                            PD0
#define SD_SCK_PIN                      PC10
#define SD_MISO_PIN                     PC11
#define SD_MOSI_PIN                     PC12
#define SD_DETECT_PIN                   PA15

//
// SPI FLASH
//
#define SPI_FLASH
#define HAS_SPI_FLASH                          1
#define SPI_DEVICE                             2
#define SPI_FLASH_SIZE                 0x1000000
#if ENABLED(SPI_FLASH)
  #define SPI_FLASH_CS_PIN                  PB12
  #define SPI_FLASH_MOSI_PIN                PB15
  #define SPI_FLASH_MISO_PIN                PB14
  #define SPI_FLASH_SCK_PIN                 PB13
#endif

//
// DGUS
//
#ifdef DGUS_LCD_UI_LOTMAXX
  #define DGUS_POWER_PIN            PB5
#endif

//
// POWER LOSS
//
#define POWER_LOSS_PIN              PC9
#define POWER_LOSS_STATE            HIGH
#define POWER_LOSS_PULLUP
#define PS_ON_PIN                   PC8
//#define PSU_ACTIVE_STATE            HIGH

//
// LASER 
//
#define SPINDLE_LASER_ENA_PIN       PE3
#define SPINDLE_LASER_PWM_PIN       PE13
#define LASER_DET_PIN               PD8
#define LASER_ACTIVE_STATE          LOW
#define LASER_MIN_PIN               PD9
#define LASER_MIN_ENDSTOP_INVERTING true
