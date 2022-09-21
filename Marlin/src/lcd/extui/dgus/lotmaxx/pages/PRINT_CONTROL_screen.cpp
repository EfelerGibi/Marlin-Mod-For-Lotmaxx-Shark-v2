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

void DGUSScreenHandler::PRINT_CONTROL_ReturnButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  GotoScreen(LOTMAXXLCD_SCREEN_PRINT);
}

void DGUSScreenHandler::PRINT_CONTROL_TemperatureButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  GotoScreen(LOTMAXXLCD_SCREEN_TEMPRATURE);
}

void DGUSScreenHandler::PRINT_CONTROL_FanButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  GotoScreen(LOTMAXXLCD_SCREEN_FAN);
}

void DGUSScreenHandler::PRINT_CONTROL_SpeedButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  GotoScreen(LOTMAXXLCD_SCREEN_PRINT_SPEED);
}

void DGUSScreenHandler::PRINT_CONTROL_ExtrusionButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  GotoScreen(LOTMAXXLCD_SCREEN_EXTRUSION);
}

void DGUSScreenHandler::PRINT_CONTROL_ZoffsetButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  GotoScreen(LOTMAXXLCD_SCREEN_ZOFFSET);
}

void DGUSScreenHandler::DGUSLCD_SendPrintCtrlSpeedToDisplay(DGUS_VP_Variable &var)
{
  char temp[20];
  sprintf(temp, "%03d%%", *(uint16_t *)var.memadr);
  DGUSLCD_TextDisplay(var.VP, temp, strlen(temp));
}

void DGUSScreenHandler::DGUSLCD_SendPrintCtrlZoffsetToDisplay(DGUS_VP_Variable &var)
{
  char temp[20];
  sprintf(temp, "%smm", String(*(float *)var.memadr).c_str());
  DGUSLCD_TextDisplay(var.VP, temp, strlen(temp));
}

void DGUSScreenHandler::DGUSLCD_SendPrintCtrlExtrusionToDisplay(DGUS_VP_Variable &var)
{
  char temp[20];
  sprintf(temp, "%03d%%", *(uint16_t *)var.memadr);
  DGUSLCD_TextDisplay(var.VP, temp, strlen(temp));
}

void DGUSScreenHandler::DGUSLCD_SendPrintCtrlExtruder1ToDisplay(DGUS_VP_Variable &var)
{
  char temp[20];
  sprintf(temp, "%03d℃", (int)thermalManager.degHotend(HID_E0));
  DGUSLCD_TextDisplay(var.VP, temp, strlen(temp));
}

void DGUSScreenHandler::DGUSLCD_SendPrintCtrlHotbedToDisplay(DGUS_VP_Variable &var)
{
  char temp[20];
  sprintf(temp, "%03d℃", (int)thermalManager.degBed());
  DGUSLCD_TextDisplay(var.VP, temp, strlen(temp));
}

void DGUSScreenHandler::DGUSLCD_SendPrintCtrlFan1ToDisplay(DGUS_VP_Variable &var)
{
  char temp[20];
  sprintf(temp, "%d", *(uint16_t *)var.memadr);
  DGUSLCD_TextDisplay(var.VP, temp, strlen(temp));
}

void DGUSScreenHandler::DGUSLCD_SendPrintCtrlFan2ToDisplay(DGUS_VP_Variable &var)
{
  char temp[20];
  sprintf(temp, "%d", 255);
  DGUSLCD_TextDisplay(var.VP, temp, strlen(temp));
}

#endif
