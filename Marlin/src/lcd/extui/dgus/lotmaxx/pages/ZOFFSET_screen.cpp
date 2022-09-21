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
#include "screen_common.h"

constexpr float DEGREE01 = 0.1;
constexpr float DEGREE1 = 1.0;

static float step = DEGREE01;

void DGUSScreenHandler::ZOFFSET_ReturnButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  GotoScreen(LOTMAXXLCD_SCREEN_PRINT_CTRL);
}

void DGUSScreenHandler::ZOFFSET_Step01ButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  step = DEGREE01;
  DGUSLCD_IconDisplay(ICON_28BMP_A, 1);
  DGUSLCD_IconDisplay(ICON_28BMP_B, 0);
}

void DGUSScreenHandler::ZOFFSET_Step1ButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  step = DEGREE1;
  DGUSLCD_IconDisplay(ICON_28BMP_A, 0);
  DGUSLCD_IconDisplay(ICON_28BMP_B, 1);
}

void DGUSScreenHandler::ZOFFSET_AddButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  char temp[20];
  float Zoffset;
  float ZLogicalPosition = LOGICAL_Z_POSITION(current_position[Z_AXIS]);

  if(*(float *)var.memadr >= MAX_ZOFFSET){
    return;
  }
  Zoffset = *(float *)var.memadr + step;
  NOMORE(Zoffset, MAX_ZOFFSET);

  set_home_offset(Z_AXIS, Zoffset);
  sprintf(temp, "G0 Z%s", String(ZLogicalPosition).c_str());
  queue.inject((const char * const)temp);
  queue.enqueue_one("M500");
}

void DGUSScreenHandler::ZOFFSET_SubButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  char temp[20];
  float Zoffset;
  float ZLogicalPosition = LOGICAL_Z_POSITION(current_position[Z_AXIS]);

  if(*(float *)var.memadr <= MIN_ZOFFSET){
    return;
  }
  Zoffset = *(float *)var.memadr - step;
  NOLESS(Zoffset, MIN_ZOFFSET);

  set_home_offset(Z_AXIS, Zoffset);
  sprintf(temp, "G0 Z%s", String(ZLogicalPosition).c_str());
  queue.inject((const char * const)temp);
  queue.enqueue_one("M500");
}

void DGUSScreenHandler::DGUSLCD_SendZoffsetToDisplay(DGUS_VP_Variable &var)
{
  char temp[20];
  String ZoffsetString(*(float *)var.memadr, 1);
  sprintf(temp, "Z offset %smm", ZoffsetString.c_str());
  DGUSLCD_TextDisplay(VP_ZOFFSET_TEXT, temp, strlen(temp));
}

#endif
