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

void DGUSScreenHandler::FILELIST_ReturnButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  GotoScreen(LOTMAXXLCD_SCREEN_MAIN);
}

void DGUSScreenHandler::FILELIST_PageupButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  DGUSLCD_SD_ScrollFilelist(var, (void *)&PageupButtonVal);
}

void DGUSScreenHandler::FILELIST_PagedownButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  DGUSLCD_SD_ScrollFilelist(var, (void *)&PagedownButtonVal);
}

void DGUSScreenHandler::FILELIST_UpButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  DGUSLCD_SD_ScrollFilelist(var, (void *)&UpButtonVal);
}

void DGUSScreenHandler::DGUSLCD_SendPathToDisplay(DGUS_VP_Variable &var)
{
  DGUSLCD_TextDisplay(VP_FILEPATH_TEXT, "", strlen(""));
}

#endif
