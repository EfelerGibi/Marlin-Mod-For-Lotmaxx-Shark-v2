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
#include "pgmspace.h"

// dual Hotend are not support, so Skip prepare process
prepare_state_t DGUSScreenHandler::prepare_state = PREPARE_Finish;

void DGUSScreenHandler::PREPARE_StopButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  thermalManager.setTargetHotend(0, HID_E0);
  GotoScreen(LOTMAXXLCD_SCREEN_MAIN);
}

void DGUSScreenHandler::PREPARE_RefreshTemperature(DGUS_VP_Variable &var)
{
  char temp[100];
  static uint64_t start_time = 0;

  //  enqueue_one_now() will waiting command enqueue success, during this time this function will reentry.
  static bool no_reentry = false;  // Prevent reentry
  if (no_reentry) return;
  REMEMBER(mh, no_reentry, true);

  //  prepare state machine 
  switch(prepare_state){

    case PREPARE_WAIT_HEATING:
      if(thermalManager.degHotend(HID_E0) > PREHEAT_1_TEMP_HOTEND){
        queue.enqueue_one("G91");
        queue.enqueue_one("G1 E-100 F600");
        prepare_state = PREPARE_WAIT_RETURNFIALMENT;
        start_time = millis();
        DGUSLCD_TextDisplay(VP_EXTRUDER_STAT_TEXT, text_display.L_3dPrintHotEndStat2, strlen(text_display.L_3dPrintHotEndStat2));
      } else {
        DGUSLCD_TextDisplay(VP_EXTRUDER_STAT_TEXT, text_display.L_3dPrintHotEndStat1, strlen(text_display.L_3dPrintHotEndStat1));
      }
      break;

    case PREPARE_WAIT_RETURNFIALMENT:
      if(ELAPSED(millis(), start_time + FILAMENT_WAITTIME)){
        prepare_state = PREPARE_WAIT_INSTALLFIALMENT;
        queue.enqueue_one("G1 E100 F600");
        start_time = millis();
      }
      break;

    case PREPARE_WAIT_INSTALLFIALMENT:
      if(ELAPSED(millis(), start_time + FILAMENT_WAITTIME)){
        queue.enqueue_one("G90");
        prepare_state = PREPARE_Finish;
      }
      break;

    case PREPARE_Finish:
      // dual Hotend are not support, so Skip prepare process
      //prepare_state = PREPARE_WAIT_RETURNFIALMENT;
      DGUSLCD_SD_StartPrint();
      GotoScreen(LOTMAXXLCD_SCREEN_PRINT);
      break;

    default:
      break;  
  } 
  //  refresh temprature
  sprintf(temp, "%s %d℃/%d", text_display.L_3dPrintHotEndTemperature, \
          (int)thermalManager.degHotend(HID_E0), thermalManager.degTargetHotend(HID_E0));
  DGUSLCD_TextDisplay(var.VP, temp, strlen(temp));
}

#endif
