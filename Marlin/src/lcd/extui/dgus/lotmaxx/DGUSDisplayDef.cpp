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

/**
 * lcd/extui/dgus/origin/DGUSDisplayDef.cpp
 */

#include "../../../../inc/MarlinConfigPre.h"

#if ENABLED(DGUS_LCD_UI_LOTMAXX)

#include "DGUSDisplayDef.h"
#include "../DGUSDisplay.h"
#include "../DGUSScreenHandler.h"

#include "../../../../module/temperature.h"
#include "../../../../module/motion.h"
#include "../../../../module/planner.h"

#include "../../../marlinui.h"
#include "../../ui_api.h"

#if ENABLED(DGUS_UI_MOVE_DIS_OPTION)
  uint16_t distanceToMove = 10;
#endif
using namespace ExtUI;

const uint16_t VPList_Filelist[] PROGMEM = { VP_SD_FileName0, VP_SD_FileName1, VP_SD_FileName2, \
                                             VP_SD_FileName3, VP_SD_FileName4, VP_SD_FileName5, \
                                             VP_FILEPATH_TEXT, 0x0000};

const uint16_t VPList_Prepare[] PROGMEM = { VP_EXTRUDER_TEMPERATURE, 0x0000};

const uint16_t VPList_Print[]   PROGMEM = { VP_PRINT_TIME_TEXT, VP_PRINT_HOTBED_TEXT, VP_PRINT_HOTEND1_TEXT, \
                                            VP_PRINT_SPEED_TEXT, VP_PRINT_ZOFFSET_TEXT, VP_PRINT_EXTRUSION_TEXT,\
                                            VP_PRINT_PERCENT_PROGRESSBAR, VP_PRINT_FILENAME_TEXT, 0x0000};

const uint16_t VPList_PrintCtrl[] PROGMEM = { VP_PRINT_CTRL_SPEED_TEXT, VP_PRINT_CTRL_ZOFFSET_TEXT, \
                                              VP_PRINT_CTRL_EXTRUSION_TEXT, VP_PRINT_CTRL_EXTRUDER1_TEXT, \
                                              VP_PRINT_CTRL_HOTBED_TEXT, VP_PRINT_CTRL_FAN1_TEXT, \
                                              VP_PRINT_CTRL_FAN2_TEXT, 0x0000};

const uint16_t VPList_Temperature[] PROGMEM = { VP_PREHEAT_TEMPERATURE_TEXT, 0x0000};

const uint16_t VPList_Extruder[] PROGMEM = {VP_EXTRUDER_TEMPERATURE1_TEXT, 0x0000};

const uint16_t VPList_Fan[] PROGMEM = { VP_FAN_VALUE_TEXT, 0x0000};

const uint16_t VPList_Status[] PROGMEM = { VP_STATUS_POSITION_TEXT, VP_STATUS_HOTEND1_TEXT, \
                                           VP_STATUS_HOTBED_TEXT, 0x0000};

const uint16_t VPList_Extrusion[] PROGMEM = {VP_EXTRUSION_TEXT, 0x0000};

const uint16_t VPList_Zoffset[] PROGMEM = {VP_ZOFFSET_TEXT, 0x0000};

const uint16_t VPList_Leveling[] PROGMEM = {VP_LEVELING_OFFSET_TEXT, 0x0000};

const uint16_t VPList_PrintLaser[] PROGMEM = {VP_PRINT_TIME_TEXT, VP_PRINT_LASER_PERCENT_PROGRESSBAR, 0x0000};

const uint16_t VPList_Select[] PROGMEM = {VP_SELECT_FILENAME_TEXT, 0x0000};

const struct VPMapping VPMap[] PROGMEM = {
  {LOTMAXXLCD_SCREEN_FILELIST,      VPList_Filelist     },
  {LOTMAXXLCD_SCREEN_PREPARE,       VPList_Prepare      },
  {LOTMAXXLCD_SCREEN_PRINT,         VPList_Print        },
  {LOTMAXXLCD_SCREEN_PRINT_CTRL,    VPList_PrintCtrl    },
  {LOTMAXXLCD_SCREEN_TEMPRATURE,    VPList_Temperature  },
  {LOTMAXXLCD_SCREEN_FAN,           VPList_Fan          },
  {LOTMAXXLCD_SCREEN_EXTRUDER,      VPList_Extruder     },
  {LOTMAXXLCD_SCREEN_STATUS,        VPList_Status       },
  {LOTMAXXLCD_SCREEN_EXTRUSION,     VPList_Extrusion    },
  {LOTMAXXLCD_SCREEN_ZOFFSET,       VPList_Zoffset      },
  {LOTMAXXLCD_SCREEN_AUTOLEVELING,  VPList_Leveling     },
  {LOTMAXXLCD_SCREEN_PRINT_LASER,   VPList_PrintLaser   },
  {LOTMAXXLCD_SCREEN_SELECT,        VPList_Select       },
  { 0 , nullptr } // List is terminated with an nullptr as table entry.
};

const char MarlinVersion[] PROGMEM = SHORT_BUILD_VERSION;

const struct DGUS_VP_Variable ListOfVP[] PROGMEM = {

  //  MAIN page 
  VPHELPER(VP_MAIN_Print, nullptr, ScreenHandler.GotoFLELIST, nullptr),
  VPHELPER(VP_MAIN_Control, nullptr, ScreenHandler.GotoCONTROL, nullptr),
  VPHELPER(VP_MAIN_Setting, nullptr, ScreenHandler.GotoSETTING, nullptr),

  //  FILELIST page
  VPHELPER(VP_FILELIST_Return, nullptr, ScreenHandler.FILELIST_ReturnButtonHandler, nullptr),
  VPHELPER(VP_FILELIST_Pageup, nullptr, ScreenHandler.FILELIST_PageupButtonHandler, nullptr),
  VPHELPER(VP_FILELIST_Pagedown, nullptr, ScreenHandler.FILELIST_PagedownButtonHandler, nullptr),
  VPHELPER(VP_FILELIST_FileSelected, nullptr, ScreenHandler.DGUSLCD_SD_FileSelected, nullptr),
  VPHELPER(VP_FILELIST_Up, nullptr, ScreenHandler.FILELIST_UpButtonHandler, nullptr),
  VPHELPER(VP_FILEPATH_TEXT, nullptr, nullptr, ScreenHandler.DGUSLCD_SendPathToDisplay),

  VPHELPER_STR(VP_SD_FileName0, nullptr, VP_SD_FileName_LEN, nullptr, ScreenHandler.DGUSLCD_SD_SendFilename),
  VPHELPER_STR(VP_SD_FileName1, nullptr, VP_SD_FileName_LEN, nullptr, ScreenHandler.DGUSLCD_SD_SendFilename),
  VPHELPER_STR(VP_SD_FileName2, nullptr, VP_SD_FileName_LEN, nullptr, ScreenHandler.DGUSLCD_SD_SendFilename),
  VPHELPER_STR(VP_SD_FileName3, nullptr, VP_SD_FileName_LEN, nullptr, ScreenHandler.DGUSLCD_SD_SendFilename),
  VPHELPER_STR(VP_SD_FileName4, nullptr, VP_SD_FileName_LEN, nullptr, ScreenHandler.DGUSLCD_SD_SendFilename),
  VPHELPER_STR(VP_SD_FileName5, nullptr, VP_SD_FileName_LEN, nullptr, ScreenHandler.DGUSLCD_SD_SendFilename),

  //  CONTROL page
  VPHELPER(VP_CONTROL_Return, nullptr, ScreenHandler.CONTROL_ReturnButtonHandler, nullptr),
  VPHELPER(VP_CONTROL_Temprature, nullptr, ScreenHandler.CONTROL_TempratureButtonHandler, nullptr),
  VPHELPER(VP_CONTROL_Move, nullptr, ScreenHandler.CONTROL_MoveButtonHandler, nullptr),
  VPHELPER(VP_CONTROL_Zero, nullptr, ScreenHandler.CONTROL_ZeroButtonHandler, nullptr),
  VPHELPER(VP_CONTROL_Extrusion, nullptr, ScreenHandler.CONTROL_ExtrusionButtonHandler, nullptr),
  VPHELPER(VP_CONTROL_Fan, nullptr, ScreenHandler.CONTROL_FanButtonHandler, nullptr),
  VPHELPER(VP_CONTROL_Stop, nullptr, ScreenHandler.CONTROL_StopButtonHandler, nullptr),
  VPHELPER(VP_CONTROL_Beep, nullptr, ScreenHandler.CONTROL_BeepButtonHandler, nullptr),
  VPHELPER(VP_CONTROL_Leveling, nullptr, ScreenHandler.CONTROL_LevelingButtonHandler, nullptr),

  //  SETTING page
  VPHELPER(VP_SETTING_Return, nullptr, ScreenHandler.SETTING_ReturnButtonHandler, nullptr),
  VPHELPER(VP_SETTING_About, nullptr, ScreenHandler.SETTING_AboutButtonHandler, nullptr),
  VPHELPER(VP_SETTING_Language, nullptr, ScreenHandler.SETTING_LanguageButtonHandler, nullptr),
  VPHELPER(VP_SETTING_State, nullptr, ScreenHandler.SETTING_StateButtonHandler, nullptr),

  //  SELECT EXTRUDER page
  VPHELPER(VP_SELECT_EXTRUDER_1, nullptr, ScreenHandler.SELECT_EXTRUDER_1_ButtonHandler, nullptr),
  VPHELPER(VP_SELECT_EXTRUDER_2, nullptr, ScreenHandler.SELECT_EXTRUDER_2_ButtonHandler, nullptr),
  VPHELPER(VP_SELECT_EXTRUDER_Double, nullptr, ScreenHandler.SELECT_EXTRUDER_DoubleButtonHandler, nullptr),
  VPHELPER(VP_SELECT_EXTRUDER_Cancel, nullptr, ScreenHandler.SELECT_EXTRUDER_CancelButtonHandler, nullptr),

  // EXTRUDER CONFIRM page
  VPHELPER(VP_EXTRUDERCONFIRM_CONFIRM, nullptr, ScreenHandler.EXTRUDERCONFIRM_CONFIRMButtonHandler, nullptr),
  VPHELPER(VP_EXTRUDERCONFIRM_CANCEL, nullptr, ScreenHandler.EXTRUDERCONFIRM_CANCELButtonHandler, nullptr),

  // PREPARE page
  VPHELPER(VP_PREPARE_STOP, nullptr, ScreenHandler.PREPARE_StopButtonHandler, nullptr),
  VPHELPER(VP_EXTRUDER_TEMPERATURE, &thermalManager.temp_hotend[0].celsius, nullptr, ScreenHandler.PREPARE_RefreshTemperature),

  // PRINT page
  VPHELPER(VP_PRINT_CTRL, nullptr, ScreenHandler.PRINT_ControlButtonHandler, nullptr),
  VPHELPER(VP_PRINT_PAUSE, nullptr, ScreenHandler.DGUSLCD_SD_ResumePauseAbort, nullptr),
  VPHELPER(VP_PRINT_STOP, nullptr, ScreenHandler.PRINT_StopButtonHandler, nullptr),

  VPHELPER(VP_PRINT_TIME_TEXT, nullptr, nullptr, ScreenHandler.DGUSLCD_SendPrintedTimeToDisplay),
  VPHELPER(VP_PRINT_HOTBED_TEXT, nullptr, nullptr, ScreenHandler.DGUSLCD_SendHotbedToDisplay),
  VPHELPER(VP_PRINT_HOTEND1_TEXT, nullptr, nullptr, ScreenHandler.DGUSLCD_SendHotend1ToDisplay),
  VPHELPER(VP_PRINT_SPEED_TEXT, &feedrate_percentage, nullptr, ScreenHandler.DGUSLCD_SendSpeedToDisplay),
  VPHELPER(VP_PRINT_ZOFFSET_TEXT, &workspace_offset.z, nullptr, ScreenHandler.DGUSLCD_SendPrintZoffsetToDisplay),
  VPHELPER(VP_PRINT_EXTRUSION_TEXT, &planner.flow_percentage[ExtUI::extruder_t::E0], nullptr, ScreenHandler.DGUSLCD_SendPrintExtrusionToDisplay),
  VPHELPER(VP_PRINT_PERCENT_PROGRESSBAR, nullptr, nullptr, ScreenHandler.DGUSLCD_SendPrintPercentToDisplay),
  VPHELPER(VP_PRINT_FILENAME_TEXT, nullptr, nullptr, ScreenHandler.DGUSLCD_SendPrintedFilenameToDisplay),

  // PRINT CONTROL page
  VPHELPER(VP_PRINT_CTRL_RETURN, nullptr, ScreenHandler.PRINT_CONTROL_ReturnButtonHandler, nullptr),
  VPHELPER(VP_PRINT_CTRL_TEMPERATURE, nullptr, ScreenHandler.PRINT_CONTROL_TemperatureButtonHandler, nullptr),
  VPHELPER(VP_PRINT_CTRL_FAN, nullptr, ScreenHandler.PRINT_CONTROL_FanButtonHandler, nullptr),
  VPHELPER(VP_PRINT_CTRL_SPEED, nullptr, ScreenHandler.PRINT_CONTROL_SpeedButtonHandler, nullptr),
  VPHELPER(VP_PRINT_CTRL_EXTRUSION, nullptr, ScreenHandler.PRINT_CONTROL_ExtrusionButtonHandler, nullptr),
  VPHELPER(VP_PRINT_CTRL_ZOFFSET, nullptr, ScreenHandler.PRINT_CONTROL_ZoffsetButtonHandler, nullptr),
  
  VPHELPER(VP_PRINT_CTRL_SPEED_TEXT, &feedrate_percentage, nullptr, ScreenHandler.DGUSLCD_SendPrintCtrlSpeedToDisplay),
  VPHELPER(VP_PRINT_CTRL_ZOFFSET_TEXT, &workspace_offset.z, nullptr, ScreenHandler.DGUSLCD_SendPrintCtrlZoffsetToDisplay),
  VPHELPER(VP_PRINT_CTRL_EXTRUSION_TEXT, &planner.flow_percentage[ExtUI::extruder_t::E0], nullptr, ScreenHandler.DGUSLCD_SendPrintCtrlExtrusionToDisplay),
  VPHELPER(VP_PRINT_CTRL_EXTRUDER1_TEXT, nullptr, nullptr, ScreenHandler.DGUSLCD_SendPrintCtrlExtruder1ToDisplay),
  VPHELPER(VP_PRINT_CTRL_HOTBED_TEXT, nullptr, nullptr, ScreenHandler.DGUSLCD_SendPrintCtrlHotbedToDisplay),
  VPHELPER(VP_PRINT_CTRL_FAN1_TEXT, &thermalManager.fan_speed[FAN0], nullptr, ScreenHandler.DGUSLCD_SendPrintCtrlFan1ToDisplay),
  VPHELPER(VP_PRINT_CTRL_FAN2_TEXT, nullptr, nullptr, ScreenHandler.DGUSLCD_SendPrintCtrlFan2ToDisplay),
  
  // TEMPERATURE page
  VPHELPER(VP_TEMPERATURE_RETURN, nullptr, ScreenHandler.TEMPERATURE_ReturnButtonHandler, nullptr),
  VPHELPER(VP_TEMPERATURE_PLA, nullptr, ScreenHandler.TEMPERATURE_PLAButtonHandler, nullptr),
  VPHELPER(VP_TEMPERATURE_ABS, nullptr, ScreenHandler.TEMPERATURE_ABSButtonHandler, nullptr),
  VPHELPER(VP_TEMPERATURE_HOTBED, nullptr, ScreenHandler.TEMPERATURE_HotbedButtonHandler, nullptr),
  VPHELPER(VP_TEMPERATURE_EXTRUDER1, nullptr, ScreenHandler.TEMPERATURE_Extruder1ButtonHandler, nullptr),
  VPHELPER(VP_TEMPERATURE_STEP1, nullptr, ScreenHandler.TEMPERATURE_Step1ButtonHandler, nullptr),
  VPHELPER(VP_TEMPERATURE_STEP5, nullptr, ScreenHandler.TEMPERATURE_Step5ButtonHandler, nullptr),
  VPHELPER(VP_TEMPERATURE_STEP10, nullptr, ScreenHandler.TEMPERATURE_Step10ButtonHandler, nullptr),
  VPHELPER(VP_TEMPERATURE_SUB, nullptr, ScreenHandler.TEMPERATURE_SubButtonHandler, nullptr),
  VPHELPER(VP_TEMPERATURE_ADD, nullptr, ScreenHandler.TEMPERATURE_AddButtonHandler, nullptr),
  VPHELPER(VP_TEMPERATURE_STOP, nullptr, ScreenHandler.TEMPERATURE_StopButtonHandler, nullptr),

  VPHELPER(VP_PREHEAT_TEMPERATURE_TEXT, nullptr, nullptr, ScreenHandler.DGUSLCD_SendtemperatureToDisplay),

  // FAN page
  VPHELPER(VP_FAN_RETURN, nullptr, ScreenHandler.FAN_ReturnButtonHandler, nullptr),
  VPHELPER(VP_FAN_FAN1, nullptr, ScreenHandler.FAN_Fan1ButtonHandler, nullptr),
  VPHELPER(VP_FAN_FAN2, nullptr, ScreenHandler.FAN_Fan2ButtonHandler, nullptr),
  VPHELPER(VP_FAN_STEP5, nullptr, ScreenHandler.FAN_Step5ButtonHandler, nullptr),
  VPHELPER(VP_FAN_STEP10, nullptr, ScreenHandler.FAN_Step10ButtonHandler, nullptr),
  VPHELPER(VP_FAN_STEP20, nullptr, ScreenHandler.FAN_Step20ButtonHandler, nullptr),
  VPHELPER(VP_FAN_SUB, nullptr, ScreenHandler.FAN_SubButtonHandler, nullptr),
  VPHELPER(VP_FAN_ADD, nullptr, ScreenHandler.FAN_AddButtonHandler, nullptr),
  VPHELPER(VP_FAN_STOP, nullptr, ScreenHandler.FAN_StopButtonHandler, nullptr),

  VPHELPER(VP_FAN_VALUE_TEXT, nullptr, nullptr, ScreenHandler.DGUSLCD_SendFanSpeedToDisplay),

  // MOVE page
  VPHELPER(VP_MOVE_3D_RETURN, nullptr, ScreenHandler.MOVE_3D_ReturnButtonHandler, nullptr),
  VPHELPER(VP_MOVE_3D_E_AXIS_SUB, nullptr, ScreenHandler.MOVE_3D_E_AXIS_SubButtonHandler, nullptr),
  VPHELPER(VP_MOVE_3D_Y_AXIS_SUB, nullptr, ScreenHandler.MOVE_3D_Y_AXIS_SubButtonHandler, nullptr),
  VPHELPER(VP_MOVE_3D_Z_AXIS_ADD, nullptr, ScreenHandler.MOVE_3D_Z_AXIS_AddButtonHandler, nullptr),
  VPHELPER(VP_MOVE_3D_X_AXIS_SUB, nullptr, ScreenHandler.MOVE_3D_X_AXIS_SubButtonHandler, nullptr),
  VPHELPER(VP_MOVE_3D_ZERO, nullptr, ScreenHandler.MOVE_3D_ZeroButtonHandler, nullptr),
  VPHELPER(VP_MOVE_3D_X_AXIS_ADD, nullptr, ScreenHandler.MOVE_3D_X_AXIS_AddButtonHandler, nullptr),
  VPHELPER(VP_MOVE_3D_E_AXIS_ADD, nullptr, ScreenHandler.MOVE_3D_E_AXIS_AddButtonHandler, nullptr),
  VPHELPER(VP_MOVE_3D_Y_AXIS_ADD, nullptr, ScreenHandler.MOVE_3D_Y_AXIS_AddButtonHandler, nullptr),
  VPHELPER(VP_MOVE_3D_Z_AXIS_SUB, nullptr, ScreenHandler.MOVE_3D_Z_AXIS_SubButtonHandler, nullptr),
  VPHELPER(VP_MOVE_3D_E0, nullptr, ScreenHandler.MOVE_3D_E0ButtonHandler, nullptr),
  VPHELPER(VP_MOVE_3D_E1, nullptr, ScreenHandler.MOVE_3D_E1ButtonHandler, nullptr),
  VPHELPER(VP_MOVE_3D_MOVE_STEP01, nullptr, ScreenHandler.MOVE_3D_MOVE_Step01ButtonHandler, nullptr),
  VPHELPER(VP_MOVE_3D_MOVE_STEP1, nullptr, ScreenHandler.MOVE_3D_MOVE_Step1ButtonHandler, nullptr),
  VPHELPER(VP_MOVE_3D_MOVE_STEP10, nullptr, ScreenHandler.MOVE_3D_MOVE_Step10ButtonHandler, nullptr),
  VPHELPER(VP_MOVE_3D_MOVE_STOP, nullptr, ScreenHandler.MOVE_3D_MOVE_StopButtonHandler, nullptr),

  // ZERO page
  VPHELPER(VP_ZERO_3D_RETURN, nullptr, ScreenHandler.ZERO_3D_ReturnButtonHandler, nullptr),
  VPHELPER(VP_ZERO_3D_X_AXIS, nullptr, ScreenHandler.ZERO_3D_X_AxisButtonHandler, nullptr),
  VPHELPER(VP_ZERO_3D_Y_AXIS, nullptr, ScreenHandler.ZERO_3D_Y_AxisButtonHandler, nullptr),
  VPHELPER(VP_ZERO_3D_Z_AXIS, nullptr, ScreenHandler.ZERO_3D_Z_AxisButtonHandler, nullptr),
  VPHELPER(VP_ZERO_3D_ALL, nullptr, ScreenHandler.ZERO_3D_AllButtonHandler, nullptr),
  VPHELPER(VP_ZERO_3D_STOP, nullptr, ScreenHandler.ZERO_3D_StopButtonHandler, nullptr),

  // EXTRUDER page
  VPHELPER(VP_EXTRUDER_RETURN, nullptr, ScreenHandler.EXTRUDER_ReturnButtonHandler, nullptr),
  VPHELPER(VP_EXTRUDER_E0, nullptr, ScreenHandler.EXTRUDER_E0ButtonHandler, nullptr),
  VPHELPER(VP_EXTRUDER_E1, nullptr, ScreenHandler.EXTRUDER_E1ButtonHandler, nullptr),
  VPHELPER(VP_EXTRUDER_STEP10, nullptr, ScreenHandler.EXTRUDER_Step10ButtonHandler, nullptr),
  VPHELPER(VP_EXTRUDER_STEP5, nullptr, ScreenHandler.EXTRUDER_Step5ButtonHandler, nullptr),
  VPHELPER(VP_EXTRUDER_STEP1, nullptr, ScreenHandler.EXTRUDER_Step1ButtonHandler, nullptr),
  VPHELPER(VP_EXTRUDER_SPEEDLOW, nullptr, ScreenHandler.EXTRUDER_SpeedlowButtonHandler, nullptr),
  VPHELPER(VP_EXTRUDER_SPEEDNORMAL, nullptr, ScreenHandler.EXTRUDER_SpeednormalButtonHandler, nullptr),
  VPHELPER(VP_EXTRUDER_SPEEDFAST, nullptr, ScreenHandler.EXTRUDER_SpeedfastButtonHandler, nullptr),
  VPHELPER(VP_EXTRUDER_LOAD, nullptr, ScreenHandler.EXTRUDER_LoadButtonHandler, nullptr),
  VPHELPER(VP_EXTRUDER_UNLOAD, nullptr, ScreenHandler.EXTRUDER_UnloadButtonHandler, nullptr),

  VPHELPER(VP_EXTRUDER_TEMPERATURE1_TEXT, nullptr, nullptr, ScreenHandler.DGUSLCD_SendExtruder1temperatureSpeedToDisplay),
  
  // LANGUAGE page
  VPHELPER(VP_LANGUAGE_RETURN, nullptr, ScreenHandler.LANGUAGE_ReturnButtunHandler, nullptr),
  VPHELPER(VP_LANGUAGE_SELECT, nullptr, ScreenHandler.LANGUAGE_SelectButtunHandler, nullptr),

  // STATUS page
  VPHELPER(VP_STATUS_RETURN, nullptr, ScreenHandler.STATUS_ReturnButtonHandler, nullptr),
  VPHELPER(VP_STATUS_POSITION_TEXT, nullptr, nullptr, ScreenHandler.DGUSLCD_SendStatusPositionToDisplay),
  VPHELPER(VP_STATUS_HOTEND1_TEXT, nullptr, nullptr, ScreenHandler.DGUSLCD_SendStatusHotend1ToDisplay),
  VPHELPER(VP_STATUS_HOTBED_TEXT, nullptr, nullptr, ScreenHandler.DGUSLCD_SendStatusHotbedToDisplay),

  // PRINT STOP page
  VPHELPER(VP_STOP_CANCEL, nullptr, ScreenHandler.STOP_CancelButtonHandler, nullptr),
  VPHELPER(VP_STOP_CONFIRM, nullptr, ScreenHandler.STOP_ConfirmButtonHandler, nullptr),

  // PRINT SPEED page
  VPHELPER(VP_SPEED_RETURN, nullptr, ScreenHandler.SPEED_ReturnButtonHandler, nullptr),
  VPHELPER(VP_SPEED_STEP5, nullptr, ScreenHandler.SPEED_Step5ButtonHandler, nullptr),
  VPHELPER(VP_SPEED_STEP10, nullptr, ScreenHandler.SPEED_Step10ButtonHandler, nullptr),
  VPHELPER(VP_SPEED_STEP20, nullptr, ScreenHandler.SPEED_Step20ButtonHandler, nullptr),
  VPHELPER(VP_SPEED_SUB, nullptr, ScreenHandler.SPEED_SubButtonHandler, nullptr),
  VPHELPER(VP_SPEED_ADD, nullptr, ScreenHandler.SPEED_AddButtonHandler, nullptr),

  // EXTRUSION page
  VPHELPER(VP_EXTRUSION_RETURN, nullptr, ScreenHandler.EXTRUSION_ReturnButtonHandler, nullptr),
  VPHELPER(VP_EXTRUSION_STEP5, nullptr, ScreenHandler.EXTRUSION_Step5ButtonHandler, nullptr),
  VPHELPER(VP_EXTRUSION_STEP10, nullptr, ScreenHandler.EXTRUSION_Step10ButtonHandler, nullptr),
  VPHELPER(VP_EXTRUSION_STEP20, nullptr, ScreenHandler.EXTRUSION_Step20ButtonHandler, nullptr),
  VPHELPER(VP_EXTRUSION_SUB, &planner.flow_percentage[ExtUI::E0], ScreenHandler.EXTRUSION_SubButtonHandler, nullptr),
  VPHELPER(VP_EXTRUSION_ADD, &planner.flow_percentage[ExtUI::E0], ScreenHandler.EXTRUSION_AddButtonHandler, nullptr),
  VPHELPER(VP_EXTRUSION_TEXT, &planner.flow_percentage[ExtUI::E0], nullptr, ScreenHandler.DGUSLCD_SendExtrusionToDisplay),
  
  // ZOFFSET page
  VPHELPER(VP_ZOFFSET_RETURN, nullptr, ScreenHandler.ZOFFSET_ReturnButtonHandler, nullptr),
  VPHELPER(VP_ZOFFSET_STEP01, nullptr, ScreenHandler.ZOFFSET_Step01ButtonHandler, nullptr),
  VPHELPER(VP_ZOFFSET_STEP1, nullptr, ScreenHandler.ZOFFSET_Step1ButtonHandler, nullptr),
  VPHELPER(VP_ZOFFSET_ADD, &home_offset[Z_AXIS], ScreenHandler.ZOFFSET_AddButtonHandler, nullptr),
  VPHELPER(VP_ZOFFSET_SUB, &home_offset[Z_AXIS], ScreenHandler.ZOFFSET_SubButtonHandler, nullptr),
  VPHELPER(VP_ZOFFSET_TEXT, &home_offset[Z_AXIS], nullptr, ScreenHandler.DGUSLCD_SendZoffsetToDisplay),

  // LEVELING page
  VPHELPER(VP_LEVELING_RETURN, nullptr, ScreenHandler.LEVELING_ReturnButtonHandler, nullptr),
  VPHELPER(VP_LEVELING_START, nullptr, ScreenHandler.LEVELING_AutoLevelingButtonHandler, nullptr),
  VPHELPER(VP_LEVELING_MANUAL, nullptr, ScreenHandler.LEVELING_ManualButtonHandler, nullptr),
  VPHELPER(VP_LEVELING_STEP01, nullptr, ScreenHandler.LEVELING_Step01ButtonHandler, nullptr),
  VPHELPER(VP_LEVELING_STEP005, nullptr, ScreenHandler.LEVELING_Step005ButtonHandler, nullptr),
  VPHELPER(VP_LEVELING_ADD, &home_offset[Z_AXIS], ScreenHandler.LEVELING_AddButtonHandler, nullptr),
  VPHELPER(VP_LEVELING_SUB, &home_offset[Z_AXIS], ScreenHandler.LEVELING_SubButtonHandler, nullptr),

  VPHELPER(VP_LEVELING_OFFSET_TEXT, &home_offset[Z_AXIS], nullptr, ScreenHandler.DGUSLCD_SendLevelingOffsetToDisplay),
  
  // MANAUL LEVELING page
  VPHELPER(VP_MANUAL_LEVELING_RETURN, nullptr, ScreenHandler.MANUAL_LEVELING_ReturnButtonHandler, nullptr),
  VPHELPER(VP_MANUAL_LEVELING_POINT1, nullptr, ScreenHandler.MANUAL_LEVELING_Point1ButtonHandler, nullptr),
  VPHELPER(VP_MANUAL_LEVELING_POINT2, nullptr, ScreenHandler.MANUAL_LEVELING_Point2ButtonHandler, nullptr),
  VPHELPER(VP_MANUAL_LEVELING_POINT3, nullptr, ScreenHandler.MANUAL_LEVELING_Point3ButtonHandler, nullptr),
  VPHELPER(VP_MANUAL_LEVELING_POINT4, nullptr, ScreenHandler.MANUAL_LEVELING_Point4ButtonHandler, nullptr),
  VPHELPER(VP_MANUAL_LEVELING_POINT5, nullptr, ScreenHandler.MANUAL_LEVELING_Point5ButtonHandler, nullptr),

  // PRINT COMPLETE page
  VPHELPER(VP_PRINT_COMPLETED_CONFIRM, nullptr, ScreenHandler.PRINT_COMPLETED_ConfirmButtonHandler, nullptr),

  //  ABOUT page
  VPHELPER(VP_ABOUT_RETURN, nullptr, ScreenHandler.ABOUT_ReturnButtonHandler, nullptr),

  //  MOVE LASER page
  VPHELPER(VP_MOVE_LASER_RETURN, nullptr, ScreenHandler.MOVE_LASER_ReturnButtonHandler, nullptr),
  VPHELPER(VP_MOVE_LASER_Z_ADD, nullptr, ScreenHandler.MOVE_LASER_Z_AddButtonHandler, nullptr),
  VPHELPER(VP_MOVE_LASER_Y_SUB, nullptr, ScreenHandler.MOVE_LASER_Y_SubButtonHandler, nullptr),
  VPHELPER(VP_MOVE_LASER_X_SUB, nullptr, ScreenHandler.MOVE_LASER_X_SubButtonHandler, nullptr),
  VPHELPER(VP_MOVE_LASER_HOME, nullptr, ScreenHandler.MOVE_LASER_HomeButtonHandler, nullptr),
  VPHELPER(VP_MOVE_LASER_X_ADD, nullptr, ScreenHandler.MOVE_LASER_X_AddButtonHandler, nullptr),
  VPHELPER(VP_MOVE_LASER_Z_SUB, nullptr, ScreenHandler.MOVE_LASER_Z_SubButtonHandler, nullptr),
  VPHELPER(VP_MOVE_LASER_Y_ADD, nullptr, ScreenHandler.MOVE_LASER_Y_AddButtonHandler, nullptr),
  VPHELPER(VP_MOVE_LASER_SWITCH, nullptr, ScreenHandler.MOVE_LASER_SwitchButtonHandler, nullptr),
  VPHELPER(VP_MOVE_LASER_STEP, nullptr, ScreenHandler.MOVE_LASER_StepButtonHandler, nullptr),
  VPHELPER(VP_MOVE_LASER_SAVE_DISTENCE, nullptr, ScreenHandler.MOVE_LASER_Save_DistenceButtonHandler, nullptr),

  // LASER PRINT page
  VPHELPER(VP_PRINT_LASER_STOP, nullptr, ScreenHandler.PRINT_LASER_StopButtonHandler, nullptr),
  VPHELPER(VP_PRINT_LASER_PAUSE, nullptr, ScreenHandler.DGUSLCD_SD_ResumePauseAbort, nullptr),
  VPHELPER(VP_PRINT_LASER_Z_ADD, nullptr, ScreenHandler.PRINT_LASER_Z_AddButtonHandler, nullptr),
  VPHELPER(VP_PRINT_LASER_Z_SUB, nullptr, ScreenHandler.PRINT_LASER_Z_SubButtonHandler, nullptr),
  VPHELPER(VP_PRINT_LASER_PERCENT_PROGRESSBAR, nullptr, nullptr, ScreenHandler.DGUSLCD_SendPrintPercentToDisplay),

  // SELECT page
  VPHELPER(VP_SELECT_CANCEL, nullptr, ScreenHandler.SELECT_CancelButtonHandler, nullptr),
  VPHELPER(VP_SELECT_CONFIRM, nullptr, ScreenHandler.SELECT_ConfirmButtonHandler, nullptr),
  VPHELPER(VP_SELECT_FILENAME_TEXT, nullptr, nullptr, ScreenHandler.DGUSLCD_SendPrintedFilenameToDisplay),

  //  kill page
  // Messages for the User, shared by the popup and the kill screen. They can't be autouploaded as we do not buffer content.
  VPHELPER_STR(VP_MSGSTR1, nullptr, VP_MSGSTR1_LEN, nullptr, ScreenHandler.DGUSLCD_SendStringToDisplayPGM),
  VPHELPER_STR(VP_MSGSTR2, nullptr, VP_MSGSTR2_LEN, nullptr, ScreenHandler.DGUSLCD_SendStringToDisplayPGM),
  VPHELPER_STR(VP_MSGSTR3, nullptr, VP_MSGSTR3_LEN, nullptr, ScreenHandler.DGUSLCD_SendStringToDisplayPGM),
  VPHELPER_STR(VP_MSGSTR4, nullptr, VP_MSGSTR4_LEN, nullptr, ScreenHandler.DGUSLCD_SendStringToDisplayPGM),

  // System VP
  VPHELPER(VP_PowerOnResponse, nullptr, ScreenHandler.DGUSLCD_PowerOnResponse, nullptr),
  VPHELPER(0, 0, 0, 0)  // must be last entry.
};

#endif // DGUS_LCD_UI_LOTMAXX
