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

void DGUSScreenHandler::ZERO_3D_ReturnButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  UpdateNewScreen(LOTMAXXLCD_SCREEN_CONTROL, false);
}

void DGUSScreenHandler::ZERO_3D_X_AxisButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  queue.enqueue_one("G28 X");
}

void DGUSScreenHandler::ZERO_3D_Y_AxisButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  queue.enqueue_one("G28 Y");
}

void DGUSScreenHandler::ZERO_3D_Z_AxisButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  queue.enqueue_one("G28 Z");
}

void DGUSScreenHandler::ZERO_3D_AllButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  queue.enqueue_one("G28");
}

void DGUSScreenHandler::ZERO_3D_StopButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  //  G28 process can't stop, if quickstop there will be kill 
}


#endif
