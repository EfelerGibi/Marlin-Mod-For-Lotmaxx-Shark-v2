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

void DGUSScreenHandler::STATUS_ReturnButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  UpdateNewScreen(LOTMAXXLCD_SCREEN_SETTING, false);
}

void DGUSScreenHandler::DGUSLCD_SendStatusPositionToDisplay(DGUS_VP_Variable &var)
{
  char temp[50];
  abce_pos_t pos = planner.get_axis_positions_mm();
  String positionX(pos.a);
  String positionY(pos.b);
  String positionZ(pos.c);
  sprintf(temp, "X:%s Y:%s Z:%s", positionX.c_str(), positionY.c_str(), positionZ.c_str());
  DGUSLCD_TextDisplay(var.VP, temp, strlen(temp));
}

void DGUSScreenHandler::DGUSLCD_SendStatusHotend1ToDisplay(DGUS_VP_Variable &var)
{
  char temp[50];
  sprintf(temp, "%03d℃/%03d℃", (int)thermalManager.degHotend(HID_E0), (int)thermalManager.degTargetHotend(HID_E0));
  DGUSLCD_TextDisplay(var.VP, temp, strlen(temp));
}

void DGUSScreenHandler::DGUSLCD_SendStatusHotbedToDisplay(DGUS_VP_Variable &var)
{
  char temp[50];
  sprintf(temp, "%03d℃/%03d℃", (int)thermalManager.degBed(), (int)thermalManager.degTargetBed());
  DGUSLCD_TextDisplay(var.VP, temp, strlen(temp));
}

#endif
