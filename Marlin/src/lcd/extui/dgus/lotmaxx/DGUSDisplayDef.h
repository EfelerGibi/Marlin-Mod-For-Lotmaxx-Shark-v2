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

#include "../DGUSDisplayDef.h"

enum DGUSLCD_Screens : uint8_t {
  DGUSLCD_SCREEN_BOOT                 = 120,
  DGUSLCD_SCREEN_MAIN                 =   1,

  DGUSLCD_SCREEN_STATUS               =   1,
  DGUSLCD_SCREEN_STATUS2              =   1,
  DGUSLCD_SCREEN_PREHEAT              =  18,
  DGUSLCD_SCREEN_POWER_LOSS           =  38,
  DGUSLCD_SCREEN_MANUALMOVE           = 192,
  DGUSLCD_SCREEN_UTILITY              = 120,
  DGUSLCD_SCREEN_FILAMENT_UNLOADING   = 158,
  DGUSLCD_SCREEN_SDFILELIST           =  15,
  DGUSLCD_SCREEN_SDPRINTMANIPULATION  =  15,
  DGUSLCD_SCREEN_SDPRINTTUNE          =  17,

  LOTMAXXLCD_SCREEN_BOOT             = 0,
  LOTMAXXLCD_SCREEN_MAIN             = 1,
  LOTMAXXLCD_SCREEN_FILELIST         = 2,
  LOTMAXXLCD_SCREEN_CONTROL          = 3,
  LOTMAXXLCD_SCREEN_SETTING          = 4,
  LOTMAXXLCD_SCREEN_MOVE             = 6,
  LOTMAXXLCD_SCREEN_ZERO             = 7,
  LOTMAXXLCD_SCREEN_LEVELING         = 9,
  LOTMAXXLCD_SCREEN_AUTOLEVELING     = 45,
  LOTMAXXLCD_SCREEN_ABOUT            = 12,
  LOTMAXXLCD_SCREEN_LANGUAGE         = 13,
  LOTMAXXLCD_SCREEN_PRINT_CTRL       = 39,
  LOTMAXXLCD_SCREEN_PRINT_LASER      = 17,
  LOTMAXXLCD_SCREEN_MOVE_LASER       = 18,
  LOTMAXXLCD_SCREEN_SELECT           = 20,
  LOTMAXXLCD_SCREEN_PRINT_STOP       = 21,
  LOTMAXXLCD_SCREEN_NONEFILAMENT     = 22,
  LOTMAXXLCD_SCREEN_PRINT_COMPLTED   = 24,
  LOTMAXXLCD_SCREEN_CONFIRM_EXTRUDER = 26,
  LOTMAXXLCD_SCREEN_PREPARE          = 27,
  LOTMAXXLCD_SCREEN_EXTRUDER         = 41,
  LOTMAXXLCD_SCREEN_EXTRUSION        = 40,
  LOTMAXXLCD_SCREEN_FILAMENT_MANAGE  = 30,
  LOTMAXXLCD_SCREEN_PRINT_SPEED      = 31,
  LOTMAXXLCD_SCREEN_SELECT_EXTRUDER  = 32,
  LOTMAXXLCD_SCREEN_PRINT            = 38,
  LOTMAXXLCD_SCREEN_STATUS           = 43,
  LOTMAXXLCD_SCREEN_TEMPRATURE       = 44,
  LOTMAXXLCD_SCREEN_ZOFFSET          = 36,
  LOTMAXXLCD_SCREEN_FAN              = 37,
  LOTMAXXLCD_SCREEN_HOMING           = 46,

  DGUSLCD_SCREEN_CONFIRM                = 240,
  DGUSLCD_SCREEN_KILL                   = 250, ///< Kill Screen. Must always be 250 (to be able to display "Error wrong LCD Version")
  DGUSLCD_SCREEN_WAITING                = 251,
  DGUSLCD_SCREEN_POPUP                  = 252, ///< special target, popup screen will also return this code to say "return to previous screen"
  DGUSLCD_SCREEN_UNUSED                 = 255,
};

// Storage space for the Killscreen messages. 0x1100 - 0x1200 . Reused for the popup.
constexpr uint16_t VP_MSGSTR1 = 0x1110;
constexpr uint8_t VP_MSGSTR1_LEN = 0x20;  // might be more place for it...
constexpr uint16_t VP_MSGSTR2 = 0x1150;
constexpr uint8_t VP_MSGSTR2_LEN = 0x20;
constexpr uint16_t VP_MSGSTR3 = 0x1190;
constexpr uint8_t VP_MSGSTR3_LEN = 0x20;
constexpr uint16_t VP_MSGSTR4 = 0x11D0;
constexpr uint8_t VP_MSGSTR4_LEN = 0x20;

// Screenchange request for screens that only make sense when printer is idle.
// e.g movement is only allowed if printer is not printing.
// Marlin must confirm by setting the screen manually.
constexpr uint16_t VP_SCREENCHANGE_ASK = 0x2000;
constexpr uint16_t VP_SCREENCHANGE = 0x2001;   // Key-Return button to new menu pressed. Data contains target screen in low byte and info in high byte.
constexpr uint16_t VP_TEMP_ALL_OFF = 0x2002;   // Turn all heaters off. Value arbitrary ;)=
constexpr uint16_t VP_SCREENCHANGE_WHENSD = 0x2003; // "Print" Button touched -- go only there if there is an SD Card.

constexpr uint16_t VP_CONFIRMED = 0x2010; // OK on confirm screen.

constexpr uint16_t VP_SD_ResumePauseAbort = 0x2026; // Resume(Data=0), Pause(Data=1), Abort(Data=2) SD Card prints
constexpr uint16_t VP_SD_AbortPrintConfirmed = 0x2028; // Abort print confirmation (virtual, will be injected by the confirm dialog)
constexpr uint16_t VP_SD_Print_Setting = 0x2040;
constexpr uint16_t VP_SD_Print_LiveAdjustZ = 0x2050; // Data: 0 down, 1 up

// Controls for movement (we can't use the incremental / decremental feature of the display at this feature works only with 16 bit values
// (which would limit us to 655.35mm, which is likely not a problem for common setups, but i don't want to rule out hangprinters support)
// A word about the coding: The VP will be per axis and the return code will be an signed 16 bit value in 0.01 mm resolution, telling us
// the relative travel amount t he user wants to do. So eg. if the display sends us VP=2100 with value 100, the user wants us to move X by +1 mm.
constexpr uint16_t VP_MOVE_X = 0x2100;
constexpr uint16_t VP_MOVE_Y = 0x2102;
constexpr uint16_t VP_MOVE_Z = 0x2104;
constexpr uint16_t VP_MOVE_E0 = 0x2110;
constexpr uint16_t VP_MOVE_E1 = 0x2112;
//constexpr uint16_t VP_MOVE_E2 = 0x2114;
//constexpr uint16_t VP_MOVE_E3 = 0x2116;
//constexpr uint16_t VP_MOVE_E4 = 0x2118;
//constexpr uint16_t VP_MOVE_E5 = 0x211A;
constexpr uint16_t VP_HOME_ALL = 0x2120;
constexpr uint16_t VP_MOTOR_LOCK_UNLOK = 0x2130;

// Power loss recovery
constexpr uint16_t VP_POWER_LOSS_RECOVERY = 0x2180;

// Fan Control Buttons , switch between "off" and "on"
constexpr uint16_t VP_FAN0_CONTROL = 0x2200;
constexpr uint16_t VP_FAN1_CONTROL = 0x2202;
//constexpr uint16_t VP_FAN2_CONTROL = 0x2204;
//constexpr uint16_t VP_FAN3_CONTROL = 0x2206;

// Heater Control Buttons , triged between "cool down" and "heat PLA" state
constexpr uint16_t VP_E0_CONTROL = 0x2210;
constexpr uint16_t VP_E1_CONTROL = 0x2212;
//constexpr uint16_t VP_E2_CONTROL = 0x2214;
//constexpr uint16_t VP_E3_CONTROL = 0x2216;
//constexpr uint16_t VP_E4_CONTROL = 0x2218;
//constexpr uint16_t VP_E5_CONTROL = 0x221A;
constexpr uint16_t VP_BED_CONTROL = 0x221C;

// Preheat
constexpr uint16_t VP_E0_BED_PREHEAT = 0x2220;
constexpr uint16_t VP_E1_BED_CONTROL = 0x2222;
//constexpr uint16_t VP_E2_BED_CONTROL = 0x2224;
//constexpr uint16_t VP_E3_BED_CONTROL = 0x2226;
//constexpr uint16_t VP_E4_BED_CONTROL = 0x2228;
//constexpr uint16_t VP_E5_BED_CONTROL = 0x222A;

// Filament load and unload
constexpr uint16_t VP_E0_FILAMENT_LOAD_UNLOAD = 0x2300;
constexpr uint16_t VP_E1_FILAMENT_LOAD_UNLOAD = 0x2302;

// Settings store , reset
constexpr uint16_t VP_SETTINGS = 0x2400;

// PID autotune
constexpr uint16_t VP_PID_AUTOTUNE_E0 = 0x2410;
//constexpr uint16_t VP_PID_AUTOTUNE_E1 = 0x2412;
//constexpr uint16_t VP_PID_AUTOTUNE_E2 = 0x2414;
//constexpr uint16_t VP_PID_AUTOTUNE_E3 = 0x2416;
//constexpr uint16_t VP_PID_AUTOTUNE_E4 = 0x2418;
//constexpr uint16_t VP_PID_AUTOTUNE_E5 = 0x241A;
constexpr uint16_t VP_PID_AUTOTUNE_BED = 0x2420;

// Firmware version on the boot screen.
constexpr uint16_t VP_MARLIN_VERSION = 0x3000;
constexpr uint8_t VP_MARLIN_VERSION_LEN = 16;   // there is more space on the display, if needed.

// Place for status messages.
constexpr uint16_t VP_M117 = 0x3020;
constexpr uint8_t VP_M117_LEN = 0x20;

// Temperatures.
constexpr uint16_t VP_T_E0_Is = 0x3060;  // 4 Byte Integer
constexpr uint16_t VP_T_E0_Set = 0x3062; // 2 Byte Integer
constexpr uint16_t VP_T_E1_Is = 0x3064;  // 4 Byte Integer

// reserved to support up to 6 Extruders:
//constexpr uint16_t VP_T_E1_Set = 0x3066; // 2 Byte Integer
//constexpr uint16_t VP_T_E2_Is = 0x3068;  // 4 Byte Integer
//constexpr uint16_t VP_T_E2_Set = 0x306A; // 2 Byte Integer
//constexpr uint16_t VP_T_E3_Is = 0x306C;  // 4 Byte Integer
//constexpr uint16_t VP_T_E3_Set = 0x306E; // 2 Byte Integer
//constexpr uint16_t VP_T_E4_Is = 0x3070;  // 4 Byte Integer
//constexpr uint16_t VP_T_E4_Set = 0x3072; // 2 Byte Integer
//constexpr uint16_t VP_T_E4_Is = 0x3074;  // 4 Byte Integer
//constexpr uint16_t VP_T_E4_Set = 0x3076; // 2 Byte Integer
//constexpr uint16_t VP_T_E5_Is = 0x3078;  // 4 Byte Integer
//constexpr uint16_t VP_T_E5_Set = 0x307A; // 2 Byte Integer

constexpr uint16_t VP_T_Bed_Is = 0x3080;  // 4 Byte Integer
constexpr uint16_t VP_T_Bed_Set = 0x3082; // 2 Byte Integer

constexpr uint16_t VP_Flowrate_E0 = 0x3090; // 2 Byte Integer
constexpr uint16_t VP_Flowrate_E1 = 0x3092; // 2 Byte Integer

// reserved for up to 6 Extruders:
//constexpr uint16_t VP_Flowrate_E2 = 0x3094;
//constexpr uint16_t VP_Flowrate_E3 = 0x3096;
//constexpr uint16_t VP_Flowrate_E4 = 0x3098;
//constexpr uint16_t VP_Flowrate_E5 = 0x309A;

constexpr uint16_t VP_Fan0_Percentage = 0x3100;  // 2 Byte Integer (0..100)
constexpr uint16_t VP_Fan1_Percentage = 0x33A2;  // 2 Byte Integer (0..100)
//constexpr uint16_t VP_Fan2_Percentage = 0x33A4;  // 2 Byte Integer (0..100)
//constexpr uint16_t VP_Fan3_Percentage = 0x33A6;  // 2 Byte Integer (0..100)

constexpr uint16_t VP_Feedrate_Percentage = 0x3102; // 2 Byte Integer (0..100)
constexpr uint16_t VP_PrintProgress_Percentage = 0x3104; // 2 Byte Integer (0..100)

constexpr uint16_t VP_PrintTime = 0x3106;
constexpr uint16_t VP_PrintTime_LEN = 10;

constexpr uint16_t VP_PrintAccTime = 0x3160;
constexpr uint16_t VP_PrintAccTime_LEN = 32;

constexpr uint16_t VP_PrintsTotal = 0x3180;
constexpr uint16_t VP_PrintsTotal_LEN = 16;

// Actual Position
constexpr uint16_t VP_XPos = 0x3110;  // 4 Byte Fixed point number; format xxx.yy
constexpr uint16_t VP_YPos = 0x3112;  // 4 Byte Fixed point number; format xxx.yy
constexpr uint16_t VP_ZPos = 0x3114;  // 4 Byte Fixed point number; format xxx.yy

constexpr uint16_t VP_EPos = 0x3120;  // 4 Byte Fixed point number; format xxx.yy

constexpr uint16_t VP_SD_Print_ProbeOffsetZ = 0x32A0; //
constexpr uint16_t VP_SD_Print_Filename = 0x32C0; //

// Fan status
constexpr uint16_t VP_FAN0_STATUS = 0x3300;
constexpr uint16_t VP_FAN1_STATUS = 0x3302;
//constexpr uint16_t VP_FAN2_STATUS = 0x3304;
//constexpr uint16_t VP_FAN3_STATUS = 0x3306;

// Heater status
constexpr uint16_t VP_E0_STATUS = 0x3310;
//constexpr uint16_t VP_E1_STATUS = 0x3312;
//constexpr uint16_t VP_E2_STATUS = 0x3314;
//constexpr uint16_t VP_E3_STATUS = 0x3316;
//constexpr uint16_t VP_E4_STATUS = 0x3318;
//constexpr uint16_t VP_E5_STATUS = 0x331A;
constexpr uint16_t VP_BED_STATUS = 0x331C;

constexpr uint16_t VP_MOVE_OPTION = 0x3400;

// Step per mm
constexpr uint16_t VP_X_STEP_PER_MM = 0x3600; // at the moment , 2 byte unsigned int , 0~1638.4
//constexpr uint16_t VP_X2_STEP_PER_MM = 0x3602;
constexpr uint16_t VP_Y_STEP_PER_MM = 0x3604;
//constexpr uint16_t VP_Y2_STEP_PER_MM = 0x3606;
constexpr uint16_t VP_Z_STEP_PER_MM = 0x3608;
//constexpr uint16_t VP_Z2_STEP_PER_MM = 0x360A;
constexpr uint16_t VP_E0_STEP_PER_MM = 0x3610;
constexpr uint16_t VP_E1_STEP_PER_MM = 0x3612;
//constexpr uint16_t VP_E2_STEP_PER_MM = 0x3614;
//constexpr uint16_t VP_E3_STEP_PER_MM = 0x3616;
//constexpr uint16_t VP_E4_STEP_PER_MM = 0x3618;
//constexpr uint16_t VP_E5_STEP_PER_MM = 0x361A;

// PIDs
constexpr uint16_t VP_E0_PID_P = 0x3700; // at the moment , 2 byte unsigned int , 0~1638.4
constexpr uint16_t VP_E0_PID_I = 0x3702;
constexpr uint16_t VP_E0_PID_D = 0x3704;
constexpr uint16_t VP_BED_PID_P = 0x3710;
constexpr uint16_t VP_BED_PID_I = 0x3712;
constexpr uint16_t VP_BED_PID_D = 0x3714;

// Waiting screen status
constexpr uint16_t VP_WAITING_STATUS = 0x3800;

// SPs for certain variables...
// located at 0x5000 and up
// Not used yet!
// This can be used e.g to make controls / data display invisible
constexpr uint16_t SP_T_E0_Is = 0x5000;
constexpr uint16_t SP_T_E0_Set = 0x5010;
constexpr uint16_t SP_T_E1_Is = 0x5020;
constexpr uint16_t SP_T_Bed_Is = 0x5030;
constexpr uint16_t SP_T_Bed_Set = 0x5040;

/*************************************************************************************************************************
 *************************************************************************************************************************
 *                                                 DGUS for LOTMAXX Mem layout
 ************************************************************************************************************************
 ************************************************************************************************************************/

// MAIN page
constexpr uint16_t VP_MAIN_Print    = 0x1000;
constexpr uint16_t VP_MAIN_Control  = 0x1001;
constexpr uint16_t VP_MAIN_Setting  = 0x1002;

// SDCard File Listing
constexpr uint16_t VP_FILELIST_Return       = 0x1100;
constexpr uint16_t VP_FILELIST_Pageup       = 0x1101;
constexpr uint16_t VP_FILELIST_Pagedown     = 0x1102;
constexpr uint16_t VP_FILELIST_FileSelected = 0x1103;
constexpr uint16_t VP_FILELIST_Up           = 0x1104;

constexpr uint16_t VP_SD_FileName_LEN = 80; // LEN is shared for all entries.
constexpr uint16_t DGUS_SD_FILESPERSCREEN = 6; //  move that info to the display and read it from there.
constexpr uint16_t PagedownButtonVal = 0x0001;
constexpr uint16_t PageupButtonVal   = 0xFFFF;
constexpr uint16_t UpButtonVal       = 0x0000;

constexpr uint16_t VP_SD_FileName0 = 0x2C00;
constexpr uint16_t VP_SD_FileName1 = 0x2C50;
constexpr uint16_t VP_SD_FileName2 = 0x2CA0;
constexpr uint16_t VP_SD_FileName3 = 0x2CF0;
constexpr uint16_t VP_SD_FileName4 = 0x2D40;
constexpr uint16_t VP_SD_FileName5 = 0x2D90;

constexpr uint16_t VP_SD_FileSelectConfirm = 0x2024;
constexpr uint16_t VP_FILEPATH_TEXT        = 0x1090;

constexpr uint16_t VP_Reset = 0x0004;
constexpr uint16_t VP_PowerOnResponse = 0x7000;

//  CONTROL page
constexpr uint16_t VP_CONTROL_Return      = 0x1200;
constexpr uint16_t VP_CONTROL_Temprature  = 0x1201;
constexpr uint16_t VP_CONTROL_Move        = 0x1202;
constexpr uint16_t VP_CONTROL_Zero        = 0x1203;
constexpr uint16_t VP_CONTROL_Extrusion   = 0x1204;
constexpr uint16_t VP_CONTROL_Leveling    = 0x1205;
constexpr uint16_t VP_CONTROL_Fan         = 0x1206;
constexpr uint16_t VP_CONTROL_Stop        = 0x1207;
constexpr uint16_t VP_CONTROL_Beep        = 0x1208;

// SETTING page
constexpr uint16_t VP_SETTING_Return      = 0x1300;
constexpr uint16_t VP_SETTING_About       = 0x1301;
constexpr uint16_t VP_SETTING_Language    = 0x1302;
constexpr uint16_t VP_SETTING_State       = 0x1303;
constexpr uint16_t VP_SETTING_Break       = 0x1304;

// SELECT EXTRUDER page
constexpr uint16_t VP_SELECT_EXTRUDER_1       = 0x2800;
constexpr uint16_t VP_SELECT_EXTRUDER_2       = 0x2801;
constexpr uint16_t VP_SELECT_EXTRUDER_Double  = 0x2802;
constexpr uint16_t VP_SELECT_EXTRUDER_Cancel  = 0x2803;

// EXTRUDER CONFIRM page
constexpr uint16_t VP_EXTRUDERCONFIRM_CONFIRM = 0x2900;
constexpr uint16_t VP_EXTRUDERCONFIRM_CANCEL  = 0x2901;
constexpr uint16_t VP_EXTRUDER_CONFIRM_Title  = 0x2910;

// PREPARE page
constexpr uint16_t VP_PREPARE_STOP            = 0x2A00;

constexpr uint16_t VP_EXTRUDER_TEMPERATURE	  = 0x2A10;
constexpr uint16_t VP_EXTRUDER_STAT			      = 0x2A30;

constexpr uint16_t FILAMENT_WAITTIME          = 10000;

// PRINT page
constexpr uint16_t VP_PRINT_CTRL              = 0x1e00;
constexpr uint16_t VP_PRINT_PAUSE             = 0x1e01;
constexpr uint16_t VP_PRINT_STOP              = 0x1e02;

constexpr uint16_t VP_PRINT_PERCENT_PROGRESSBAR	= 0x1e03;
constexpr uint16_t VP_PRINT_PAUSE_TEXT		      = 0x1eb0;
constexpr uint16_t VP_PRINT_STOP_TEXT			      = 0x2430;
constexpr uint16_t VP_PRINT_TIME_TEXT			      = 0x1e40;
constexpr uint16_t VP_PRINT_HOTBED_TEXT		      = 0x1e50;
constexpr uint16_t VP_PRINT_HOTEND1_TEXT	      = 0x1e60;
constexpr uint16_t VP_PRINT_EXTRUSION_TEXT		  = 0x1e70;
constexpr uint16_t VP_PRINT_SPEED_TEXT	        = 0x1e80;
constexpr uint16_t VP_PRINT_ZOFFSET_TEXT		    = 0x1e90;
constexpr uint16_t VP_PRINT_FILENAME_TEXT		    = 0x1e13;
constexpr uint16_t VP_PRINT_COMPLETE_TEXT		    = 0x1ed0;

// PRINT CONTROL page
constexpr uint16_t	VP_PRINT_CTRL_RETURN        = 0x1f00;
constexpr uint16_t	VP_PRINT_CTRL_TEMPERATURE   = 0x1f01;
constexpr uint16_t	VP_PRINT_CTRL_FAN           = 0x1f02;
constexpr uint16_t	VP_PRINT_CTRL_SPEED         = 0x1f03;
constexpr uint16_t	VP_PRINT_CTRL_EXTRUSION     = 0x1f04;
constexpr uint16_t	VP_PRINT_CTRL_ZOFFSET       = 0x1f05;

constexpr uint16_t  VP_PRINT_CTRL_SPEED_TEXT	  = 0x1f30;
constexpr uint16_t  VP_PRINT_CTRL_ZOFFSET_TEXT  = 0x1f40;
constexpr uint16_t  VP_PRINT_CTRL_EXTRUSION_TEXT= 0x1f50;
constexpr uint16_t  VP_PRINT_CTRL_EXTRUDER1_TEXT= 0x1f60;
constexpr uint16_t  VP_PRINT_CTRL_HOTBED_TEXT		= 0x1f80;
constexpr uint16_t  VP_PRINT_CTRL_FAN1_TEXT			= 0x1f90;
constexpr uint16_t  VP_PRINT_CTRL_FAN2_TEXT		  = 0x1fa0;

// TEMPERATURE page
constexpr uint16_t VP_TEMPERATURE_RETURN        = 0x1400;
constexpr uint16_t VP_TEMPERATURE_PLA           = 0x1401;
constexpr uint16_t VP_TEMPERATURE_ABS           = 0x1402;
constexpr uint16_t VP_TEMPERATURE_HOTBED        = 0x1403;
constexpr uint16_t VP_TEMPERATURE_EXTRUDER1     = 0x1404;
constexpr uint16_t VP_TEMPERATURE_EXTRUDER2     = 0x1405;
constexpr uint16_t VP_TEMPERATURE_STEP1         = 0x1406;
constexpr uint16_t VP_TEMPERATURE_STEP5         = 0x1407;
constexpr uint16_t VP_TEMPERATURE_STEP10        = 0x1408;
constexpr uint16_t VP_TEMPERATURE_SUB           = 0x1409;
constexpr uint16_t VP_TEMPERATURE_ADD           = 0x140A;
constexpr uint16_t VP_TEMPERATURE_STOP          = 0x140B;

constexpr uint16_t VP_PREHEAT_TEMPERATURE_TEXT  = 0x1430;

// FAN page
constexpr uint16_t VP_FAN_RETURN                = 0x1a00;
constexpr uint16_t VP_FAN_FAN1                  = 0x1a01;
constexpr uint16_t VP_FAN_FAN2                  = 0x1a02;
constexpr uint16_t VP_FAN_STEP5                 = 0x1a03;
constexpr uint16_t VP_FAN_STEP10                = 0x1a04;
constexpr uint16_t VP_FAN_STEP20                = 0x1a05;
constexpr uint16_t VP_FAN_SUB                   = 0x1a06;
constexpr uint16_t VP_FAN_ADD                   = 0x1a07;
constexpr uint16_t VP_FAN_STOP                  = 0x1a08;

constexpr uint16_t VP_FAN_VALUE_TEXT	          = 0x1a30;

// MOVE
constexpr uint16_t VP_MOVE_3D_RETURN            = 0x1500;
constexpr uint16_t VP_MOVE_3D_E_AXIS_SUB        = 0x1501;
constexpr uint16_t VP_MOVE_3D_Y_AXIS_SUB        = 0x1502;
constexpr uint16_t VP_MOVE_3D_Z_AXIS_ADD        = 0x1503;
constexpr uint16_t VP_MOVE_3D_X_AXIS_SUB        = 0x1504;
constexpr uint16_t VP_MOVE_3D_ZERO              = 0x1505;
constexpr uint16_t VP_MOVE_3D_X_AXIS_ADD        = 0x1506;
constexpr uint16_t VP_MOVE_3D_E_AXIS_ADD        = 0x1507;
constexpr uint16_t VP_MOVE_3D_Y_AXIS_ADD        = 0x1508;
constexpr uint16_t VP_MOVE_3D_Z_AXIS_SUB        = 0x1509;
constexpr uint16_t VP_MOVE_3D_E0                = 0x150A;
constexpr uint16_t VP_MOVE_3D_E1                = 0x150B;
constexpr uint16_t VP_MOVE_3D_MOVE_STEP01       = 0x150C;
constexpr uint16_t VP_MOVE_3D_MOVE_STEP1        = 0x150D;
constexpr uint16_t VP_MOVE_3D_MOVE_STEP10       = 0x150E;
constexpr uint16_t VP_MOVE_3D_MOVE_STOP         = 0x150F;

// ZERO page
constexpr uint16_t VP_ZERO_3D_RETURN            = 0x1600;
constexpr uint16_t VP_ZERO_3D_X_AXIS            = 0x1601;
constexpr uint16_t VP_ZERO_3D_Y_AXIS            = 0x1602;
constexpr uint16_t VP_ZERO_3D_Z_AXIS            = 0x1603;
constexpr uint16_t VP_ZERO_3D_ALL               = 0x1604;
constexpr uint16_t VP_ZERO_3D_STOP              = 0x1605;

// EXTRUDER page
constexpr uint16_t VP_EXTRUDER_RETURN           = 0x1700;
constexpr uint16_t VP_EXTRUDER_E0               = 0x1701;
constexpr uint16_t VP_EXTRUDER_E1               = 0x1702;
constexpr uint16_t VP_EXTRUDER_STEP10           = 0x1703;
constexpr uint16_t VP_EXTRUDER_STEP5            = 0x1704;
constexpr uint16_t VP_EXTRUDER_STEP1            = 0x1705;
constexpr uint16_t VP_EXTRUDER_SPEEDLOW         = 0x1706;
constexpr uint16_t VP_EXTRUDER_SPEEDNORMAL      = 0x1707;
constexpr uint16_t VP_EXTRUDER_SPEEDFAST        = 0x1708;
constexpr uint16_t VP_EXTRUDER_LOAD             = 0x1709;
constexpr uint16_t VP_EXTRUDER_UNLOAD           = 0x170A;

constexpr uint16_t VP_EXTRUDER_TEMPERATURE1_TEXT		= 0x1730;
constexpr uint16_t VP_EXTRUDER_TEMPERATURE2_TEXT		= 0x1740;
constexpr uint16_t VP_EXTRUDER_STAT_TEXT        		=	0x2A30;

// LANGUAGE page
constexpr uint16_t VP_LANGUAGE_RETURN           = 0x1c00;
constexpr uint16_t VP_LANGUAGE_SELECT           = 0x1c01;

// STATUS page
constexpr uint16_t VP_STATUS_RETURN		          = 0x1d00;
constexpr uint16_t VP_STATUS_POSITION_TEXT		  = 0x1d50;
constexpr uint16_t VP_STATUS_HOTEND1_TEXT			  = 0x1d70;
constexpr uint16_t VP_STATUS_HOTEND2_TEXT			  = 0x1d80;
constexpr uint16_t VP_STATUS_HOTBED_TEXT		    = 0x1d90;

// PRINT STOP page
constexpr uint16_t VP_STOP_CANCEL		            = 0x2400;
constexpr uint16_t VP_STOP_CONFIRM			        = 0x2401;

// SPEED page
constexpr uint16_t VP_SPEED_RETURN              = 0x2200;
constexpr uint16_t VP_SPEED_STEP5               = 0x2201;	
constexpr uint16_t VP_SPEED_STEP10              = 0x2202;	
constexpr uint16_t VP_SPEED_STEP20              = 0x2203;
constexpr uint16_t VP_SPEED_SUB                 = 0x2204;
constexpr uint16_t VP_SPEED_ADD                 = 0x2205;

constexpr uint16_t VP_SPEED_TEXT		            = 0x1e80;

// EXTRUSION page
constexpr uint16_t VP_EXTRUSION_RETURN          = 0x2600;
constexpr uint16_t VP_EXTRUSION_STEP5           = 0x2601;
constexpr uint16_t VP_EXTRUSION_STEP10          = 0x2602;
constexpr uint16_t VP_EXTRUSION_STEP20          = 0x2603;
constexpr uint16_t VP_EXTRUSION_SUB             = 0x2604;
constexpr uint16_t VP_EXTRUSION_ADD             = 0x2605;

constexpr uint16_t VP_EXTRUSION_TEXT            =	0x2630;

// ZOFFSET page
constexpr uint16_t VP_ZOFFSET_RETURN            = 0x2b00;
constexpr uint16_t VP_ZOFFSET_STEP01            = 0x2b01;
constexpr uint16_t VP_ZOFFSET_STEP1             = 0x2b02;
constexpr uint16_t VP_ZOFFSET_ADD               = 0x2b03;
constexpr uint16_t VP_ZOFFSET_SUB               = 0x2b04;

constexpr uint16_t VP_ZOFFSET_TEXT			        = 0x2B30;

// ABOUT page
constexpr uint16_t VP_ABOUT_RETURN              = 0x1B00;

// LEVELING page
constexpr uint16_t VP_LEVELING_RETURN           = 0x1900;
constexpr uint16_t VP_LEVELING_START            = 0x1901;
constexpr uint16_t VP_LEVELING_MANUAL           = 0x1902;
constexpr uint16_t VP_LEVELING_STEP01           = 0x1903;
constexpr uint16_t VP_LEVELING_STEP005          = 0x1904;
constexpr uint16_t VP_LEVELING_ADD              = 0x1905;
constexpr uint16_t VP_LEVELING_SUB              = 0x1906;

constexpr uint16_t VP_LEVELING_OFFSET_TEXT		  = 0x1930;
constexpr uint16_t VP_LEVELING_REMAIN_TEXT		  = 0x19A0;

// MANUAL LEVELING page
constexpr uint16_t VP_MANUAL_LEVELING_RETURN    = 0x1800;
constexpr uint16_t VP_MANUAL_LEVELING_POINT1    = 0x1801;
constexpr uint16_t VP_MANUAL_LEVELING_POINT2    = 0x1802;
constexpr uint16_t VP_MANUAL_LEVELING_POINT3    = 0x1803;
constexpr uint16_t VP_MANUAL_LEVELING_POINT4    = 0x1804;
constexpr uint16_t VP_MANUAL_LEVELING_POINT5    = 0x1805;

// PRINT COMPLETED page
constexpr uint16_t VP_PRINT_COMPLETED_CONFIRM   = 0x2700;

// LASER MOVE page
constexpr uint16_t VP_MOVE_LASER_RETURN         = 0x2100;
constexpr uint16_t VP_MOVE_LASER_Z_ADD          = 0x2101;
constexpr uint16_t VP_MOVE_LASER_Y_SUB          = 0x2102;
constexpr uint16_t VP_MOVE_LASER_X_SUB          = 0x2103;
constexpr uint16_t VP_MOVE_LASER_HOME           = 0x2104;
constexpr uint16_t VP_MOVE_LASER_X_ADD          = 0x2105;
constexpr uint16_t VP_MOVE_LASER_Z_SUB          = 0x2106;
constexpr uint16_t VP_MOVE_LASER_Y_ADD          = 0x2107;
constexpr uint16_t VP_MOVE_LASER_SWITCH         = 0x2108;
constexpr uint16_t VP_MOVE_LASER_STEP           = 0x2109;
constexpr uint16_t VP_MOVE_LASER_SAVE_DISTENCE  = 0x210A;

// LASER PRINT page
constexpr uint16_t VP_PRINT_LASER_STOP           = 0x2001;
constexpr uint16_t VP_PRINT_LASER_PAUSE          = 0x2002;
constexpr uint16_t VP_PRINT_LASER_Z_ADD          = 0x2003;
constexpr uint16_t VP_PRINT_LASER_Z_SUB          = 0x2004;

constexpr uint16_t VP_PRINT_LASER_PAUSE_TEXT		      = 0x2070;	
constexpr uint16_t VP_PRINT_LASER_PERCENT_PROGRESSBAR = 0x2010;

// SELECT page
constexpr uint16_t VP_SELECT_CANCEL              = 0x2300;
constexpr uint16_t VP_SELECT_CONFIRM             = 0x2301;

constexpr uint16_t VP_SELECT_FILENAME_TEXT       = 0x2333;

// HOMING page
constexpr uint16_t VP_HOMING_TEXT                = 0x2730;

constexpr uint16_t DP_START_TEXT	               = 0x8000;
constexpr uint16_t DP_LENTH_TEXT	               = 0x10;

// ICON
constexpr uint16_t GETGCODE_ICON1	        = 0x1105;
constexpr uint16_t GETGCODE_ICON2	        = 0x11f1;		
constexpr uint16_t GETGCODE_ICON3	        = 0x11f2;
constexpr uint16_t GETGCODE_ICON4	        = 0x11f3;
constexpr uint16_t GETGCODE_ICON5	        = 0x11f4;
constexpr uint16_t GETGCODE_ICON6	        = 0x11f5;

constexpr uint16_t ICON_5BMP_A		        = 0X1440;
constexpr uint16_t ICON_5BMP_B		        = 0X1441;
constexpr uint16_t ICON_5BMP_C		        = 0X1442;
constexpr uint16_t ICON_5BMP_D		        = 0X1443;
constexpr uint16_t ICON_5BMP_E		        = 0X1444;
constexpr uint16_t ICON_5BMP_F		        = 0X1445;
constexpr uint16_t ICON_5BMP_G		        = 0X1446;

constexpr uint16_t ICON_6BMP_A		        = 0X1530;
constexpr uint16_t ICON_6BMP_B		        = 0X1531;
constexpr uint16_t ICON_6BMP_C		        = 0X1534;
constexpr uint16_t ICON_6BMP_D		        = 0X1535;
constexpr uint16_t ICON_6BMP_E		        = 0X1536;

constexpr uint16_t ICON_30BMP_A	          = 0X17A0;
constexpr uint16_t ICON_30BMP_B	          = 0X17A1;
constexpr uint16_t ICON_30BMP_C	          = 0X17A2;
constexpr uint16_t ICON_30BMP_D	          = 0X17A3;
constexpr uint16_t ICON_30BMP_E	          = 0X17A4;
constexpr uint16_t ICON_30BMP_F	          = 0X17A5;
constexpr uint16_t ICON_30BMP_G	          = 0X17A6;
constexpr uint16_t ICON_30BMP_H	          = 0X17A7;

constexpr uint16_t ICON_11BMP_A	          = 0X1A70;
constexpr uint16_t ICON_11BMP_B	          = 0X1A71;
constexpr uint16_t ICON_11BMP_C	          = 0X1A72;
constexpr uint16_t ICON_11BMP_D	          = 0X1A73;
constexpr uint16_t ICON_11BMP_E	          = 0X1A74;
constexpr uint16_t ICON_11BMP_F	          = 0X1A75;

constexpr uint16_t ICON_19BMP_A 	        = 0X2240;
constexpr uint16_t ICON_19BMP_B	          = 0X2241;
constexpr uint16_t ICON_19BMP_C	          = 0X2242;

constexpr uint16_t ICON_28BMP_A	          = 0X2b50;
constexpr uint16_t ICON_28BMP_B	          = 0X2b51;

constexpr uint16_t ICON_29BMP_A	          = 0X2606;
constexpr uint16_t ICON_29BMP_B	          = 0X2607;
constexpr uint16_t ICON_29BMP_C	          = 0X2608;

constexpr uint16_t ICON_45BMP_A	          = 0X1940;
constexpr uint16_t ICON_45BMP_B	          = 0X1941;
