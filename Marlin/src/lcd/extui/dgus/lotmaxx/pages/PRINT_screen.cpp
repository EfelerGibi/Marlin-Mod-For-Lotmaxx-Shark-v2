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
#include "../../../../../feature/powerloss.h"
#include "../../../ui_api.h"

void DGUSScreenHandler::PRINT_StopButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  if(!IS_SD_FILE_OPEN()){
    return ;
  }
  GotoScreen(LOTMAXXLCD_SCREEN_PRINT_STOP);
}

void DGUSScreenHandler::PRINT_ControlButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  GotoScreen(LOTMAXXLCD_SCREEN_PRINT_CTRL);
}

void DGUSScreenHandler::DGUSLCD_SendHotbedToDisplay(DGUS_VP_Variable &var)
{
  char temp[20];
  sprintf(temp, "%03d℃", (int)thermalManager.degBed());
  DGUSLCD_TextDisplay(VP_PRINT_HOTBED_TEXT, temp, strlen(temp));
}


void DGUSScreenHandler::DGUSLCD_SendHotend1ToDisplay(DGUS_VP_Variable &var)
{
  char temp[20];
  sprintf(temp, "%03d℃", (int)thermalManager.degHotend(HID_E0));
  DGUSLCD_TextDisplay(VP_PRINT_HOTEND1_TEXT, temp, strlen(temp));
}

void DGUSScreenHandler::DGUSLCD_SendSpeedToDisplay(DGUS_VP_Variable &var)
{
  char temp[20];
  sprintf(temp, "%03d%%", *(uint16_t *)var.memadr);
  DGUSLCD_TextDisplay(VP_PRINT_SPEED_TEXT, temp, strlen(temp));
}

void DGUSScreenHandler::DGUSLCD_SendPrintZoffsetToDisplay(DGUS_VP_Variable &var)
{
  char temp[20];
  sprintf(temp, "%smm", String(*(float *)var.memadr).c_str());
  DGUSLCD_TextDisplay(VP_PRINT_ZOFFSET_TEXT, temp, strlen(temp));
}

void DGUSScreenHandler::DGUSLCD_SendPrintExtrusionToDisplay(DGUS_VP_Variable &var)
{
  char temp[20];
  sprintf(temp, "%03d%%", *(uint16_t *)var.memadr);
  DGUSLCD_TextDisplay(VP_PRINT_EXTRUSION_TEXT, temp, strlen(temp));
}

void DGUSScreenHandler::DGUSLCD_SendPrintPercentToDisplay(DGUS_VP_Variable &var)
{
  uint16_t print_percent = ExtUI::getProgress_percent();
  dgusdisplay.WriteVariable(var.VP, print_percent);
}

void DGUSScreenHandler::DGUSLCD_SendPrintedTimeToDisplay(DGUS_VP_Variable &var) {
  duration_t elapsed = print_job_timer.duration();
  char buf[32];
  elapsed.toString(buf);
  DGUSLCD_TextDisplay(var.VP, buf, strlen(buf));
}

void DGUSScreenHandler::DGUSLCD_SendPrintedFilenameToDisplay(DGUS_VP_Variable &var) {
  const char * filename = filelist.filename();
  DGUSLCD_TextDisplay(var.VP, filename, strlen(filename));
}

#endif
