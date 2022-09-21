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

#include "../../../../../inc/MarlinConfigPre.h"

#if ENABLED(DGUS_LCD_UI_LOTMAXX)

#include "../DGUSScreenHandler.h"

#include "../../../../../MarlinCore.h"
#include "../../../../../gcode/queue.h"
#include "../../../../../libs/duration_t.h"
#include "../../../../../module/settings.h"
#include "../../../../../module/temperature.h"
#include "../../../../../module/motion.h"
#include "../../../../../module/planner.h"
#include "../../../../../module/printcounter.h"
#include "../../../../../sd/cardreader.h"
#include "../../../../../gcode/gcode.h"
#include "screen_common.h"

constexpr float DEGREE01  = 0.1;
constexpr float DEGREE005 = 0.05;

static float OffsetStep = DEGREE01;

void DGUSScreenHandler::LEVELING_ReturnButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  UpdateNewScreen(LOTMAXXLCD_SCREEN_CONTROL, false);
}

void DGUSScreenHandler::LEVELING_AutoLevelingButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  celsius_t hotendtargetold = thermalManager.degTargetHotend(HID_E0);
  celsius_t hotbedtargetold = thermalManager.degTargetBed();
  
  //  before leveling preheat the hotend
  #if ENABLED(PREHEAT_BEFORE_LEVELING)
    DGUSLCD_TextDisplay(VP_LEVELING_REMAIN_TEXT, text_display.L_3dPrintHotEndStat1, strlen(text_display.L_3dPrintHotEndStat1));
    probe.preheat_for_probing(LEVELING_NOZZLE_TEMP, LEVELING_BED_TEMP);
    DGUSLCD_TextDisplay(VP_LEVELING_REMAIN_TEXT, "", 0);
  #endif
  DGUSLCD_TextDisplay(VP_LEVELING_REMAIN_TEXT, "", 0);
  gcode.process_subcommands_now("G29 P1 S0 N");
  gcode.process_subcommands_now("G29 P1 C");
  queue.enqueue_one("M500"); 
  thermalManager.setTargetHotend(hotendtargetold, HID_E0);
  thermalManager.setTargetBed(hotbedtargetold);
}

void DGUSScreenHandler::LEVELING_ManualButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{

}

void DGUSScreenHandler::LEVELING_Step01ButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  OffsetStep = DEGREE01;
	DGUSLCD_IconDisplay(ICON_45BMP_A, 1);
  DGUSLCD_IconDisplay(ICON_45BMP_B, 0);
}

void DGUSScreenHandler::LEVELING_Step005ButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  OffsetStep = DEGREE005;
  DGUSLCD_IconDisplay(ICON_45BMP_A, 0);
	DGUSLCD_IconDisplay(ICON_45BMP_B, 1);
}

void DGUSScreenHandler::LEVELING_AddButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  char temp[20];
  float Zoffset;
  float ZLogicalPosition = LOGICAL_Z_POSITION(current_position[Z_AXIS]);

  if(*(float *)var.memadr >= MAX_ZOFFSET){
    return;
  }
  Zoffset = *(float *)var.memadr + OffsetStep;
  NOMORE(Zoffset, MAX_ZOFFSET);

  set_home_offset(Z_AXIS, Zoffset);
  sprintf(temp, "G0 Z%s", String(ZLogicalPosition).c_str());
  queue.inject((const char * const)temp);
  queue.enqueue_one("M500");
}

void DGUSScreenHandler::LEVELING_SubButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  char temp[20];
  float Zoffset;
  float ZLogicalPosition = LOGICAL_Z_POSITION(current_position[Z_AXIS]);

  if(*(float *)var.memadr <= MIN_ZOFFSET){
    return;
  }
  Zoffset = *(float *)var.memadr - OffsetStep;
  NOLESS(Zoffset, MIN_ZOFFSET);

  set_home_offset(Z_AXIS, Zoffset);
  sprintf(temp, "G0 Z%s", String(ZLogicalPosition).c_str());
  queue.inject((const char * const)temp);
  queue.enqueue_one("M500");
}

void DGUSScreenHandler::DGUSLCD_SendLevelingOffsetToDisplay(DGUS_VP_Variable &var)
{
  char temp[20];
  String ZoffsetString(*(float *)var.memadr, 1);
  sprintf(temp, "Z offset %smm", ZoffsetString.c_str());
  DGUSLCD_TextDisplay(VP_LEVELING_OFFSET_TEXT, temp, strlen(temp));
}

#endif
