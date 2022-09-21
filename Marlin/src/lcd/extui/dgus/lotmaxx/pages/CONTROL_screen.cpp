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
#include "../../../../../module/stepper.h"
#include "../../../../../sd/cardreader.h"

void DGUSScreenHandler::CONTROL_ReturnButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  UpdateNewScreen(LOTMAXXLCD_SCREEN_MAIN, false);
}

void DGUSScreenHandler::CONTROL_TempratureButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  GotoScreen(LOTMAXXLCD_SCREEN_TEMPRATURE);
}

void DGUSScreenHandler::CONTROL_MoveButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  UpdateNewScreen(LOTMAXXLCD_SCREEN_MOVE, false);
}

void DGUSScreenHandler::CONTROL_ZeroButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  UpdateNewScreen(LOTMAXXLCD_SCREEN_ZERO, false);
}

void DGUSScreenHandler::CONTROL_ExtrusionButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  GotoScreen(LOTMAXXLCD_SCREEN_EXTRUDER);
}

void DGUSScreenHandler::CONTROL_LevelingButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  GotoScreen(LOTMAXXLCD_SCREEN_AUTOLEVELING);
}

void DGUSScreenHandler::CONTROL_FanButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  GotoScreen(LOTMAXXLCD_SCREEN_FAN);
}

void DGUSScreenHandler::CONTROL_StopButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  stepper.disable_all_steppers();
}

void DGUSScreenHandler::CONTROL_BeepButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  uint8_t isBeep = *(uint8_t *)val_ptr;
  uint8_t Beep[4] = {0x5a, 0x00, 0x00, 0x30};
  
  if(isBeep == true){
    Beep[3] = 0x38;
  } else {
    Beep[3] = 0x30;
  }

  dgusdisplay.WriteVariable(0x0080, Beep, sizeof(Beep), false);
}

#endif
