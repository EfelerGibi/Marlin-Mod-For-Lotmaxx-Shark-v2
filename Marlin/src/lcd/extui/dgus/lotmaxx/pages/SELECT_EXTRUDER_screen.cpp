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

void DGUSScreenHandler::SELECT_EXTRUDER_1_ButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  char temp[40];
  queue.enqueue_one("T0");
  sprintf(temp,"%s?",text_display.L_3dPrintExtruder1);
  GotoScreen(LOTMAXXLCD_SCREEN_CONFIRM_EXTRUDER);
  DGUSLCD_TextDisplay(VP_EXTRUDER_CONFIRM_Title, temp, strlen(temp));
}

void DGUSScreenHandler::SELECT_EXTRUDER_2_ButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  char temp[40];
  queue.enqueue_one("T1");
  sprintf(temp,"%s?",text_display.L_3dPrintExtruder2);
  GotoScreen(LOTMAXXLCD_SCREEN_CONFIRM_EXTRUDER);
  DGUSLCD_TextDisplay(VP_EXTRUDER_CONFIRM_Title, temp, strlen(temp));
}

void DGUSScreenHandler::SELECT_EXTRUDER_DoubleButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  char temp[40];
  sprintf(temp,"%s?",text_display.L_3dPrintExtruder12);
  GotoScreen(LOTMAXXLCD_SCREEN_CONFIRM_EXTRUDER);
  DGUSLCD_TextDisplay(VP_EXTRUDER_CONFIRM_Title, temp, strlen(temp));
}

void DGUSScreenHandler::SELECT_EXTRUDER_CancelButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  GotoScreen(LOTMAXXLCD_SCREEN_FILELIST);
}

#endif
