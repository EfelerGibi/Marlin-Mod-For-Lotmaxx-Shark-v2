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
#include "../../../../../feature/lotmaxx/printmode.h"
#include "../../../../../sd/cardreader.h"

extern float focusLocation;

void DGUSScreenHandler::SELECT_CancelButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  UpdateNewScreen(LOTMAXXLCD_SCREEN_FILELIST);
}

void DGUSScreenHandler::SELECT_ConfirmButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  if(IS_LASER_PRINTMODE()){
    char temp[20];
	char strtemp[10];
	queue.enqueue_one("G28 Z");
	dtostrf(focusLocation, 1, 3, strtemp);
	sprintf(temp, "G1 Z%s", strtemp);
	queue.enqueue_one(temp);
	DGUSLCD_SD_StartPrint();
    GotoScreen(LOTMAXXLCD_SCREEN_PRINT_LASER);
  }
}

#endif
