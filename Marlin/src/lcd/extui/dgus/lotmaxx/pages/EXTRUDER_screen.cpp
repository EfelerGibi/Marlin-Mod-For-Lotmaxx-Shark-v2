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
#include "../../../../marlinui.h"
#include "../../../ui_api.h"
#include "screen_common.h"

typedef enum : uint8_t{
  DEGREE1 = 1,
  DEGREE5 = 5,
  DEGREE10 = 10,
}Step_t;

typedef enum : uint16_t{
  SPEED_LOW     = 200,
  SPEED_NORMAL  = 400,
  SPEED_FAST    = 800,
}Speed_t;

typedef struct {
  Speed_t     Speed;
  Step_t      Step;
  celsius_t   OldTemperatureTarget;
  float       OldEAxisPosition;
  bool        isTempTargetChanged;
  bool        isEAxisPositionChanged;
}ExtruderScreen_t;

ExtruderScreen_t ExtruderScreen = {
  .Speed                = SPEED_LOW,
  .Step                 = DEGREE10,
  .OldTemperatureTarget = 0,
  .OldEAxisPosition     = 0,
  .isTempTargetChanged    = 0,
  .isEAxisPositionChanged = 0,
};

static void _Clear(void)
{
  ExtruderScreen.OldEAxisPosition = 0;
  ExtruderScreen.OldTemperatureTarget = 0;
  ExtruderScreen.isEAxisPositionChanged = false;
  ExtruderScreen.isTempTargetChanged = false;
}

void DGUSScreenHandler::EXTRUDER_ReturnButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  if(ExtruderScreen.isTempTargetChanged == true){
    thermalManager.setTargetHotend(ExtruderScreen.OldTemperatureTarget, HID_E0);
  }
  if(ExtruderScreen.isEAxisPositionChanged == true){
    planner.set_e_position_mm(ExtruderScreen.OldEAxisPosition);
  }
  _Clear();
  if(printingIsPaused()){
    ExtUI::setFilamentRunoutState(false);
    ExtUI::resumePrint();
  }
  PopToOldScreen();
}

void DGUSScreenHandler::EXTRUDER_E0ButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  queue.enqueue_one("T0");
  DGUSLCD_IconDisplay(ICON_30BMP_A, 1);
	DGUSLCD_IconDisplay(ICON_30BMP_B, 0);
}

void DGUSScreenHandler::EXTRUDER_E1ButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  queue.enqueue_one("T1");
	DGUSLCD_IconDisplay(ICON_30BMP_A, 0);
	DGUSLCD_IconDisplay(ICON_30BMP_B, 1);
}

void DGUSScreenHandler::EXTRUDER_Step10ButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  ExtruderScreen.Step = DEGREE10;
  DGUSLCD_IconDisplay(ICON_30BMP_C, 1);
  DGUSLCD_IconDisplay(ICON_30BMP_D, 0);
  DGUSLCD_IconDisplay(ICON_30BMP_E, 0);
}

void DGUSScreenHandler::EXTRUDER_Step5ButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  ExtruderScreen.Step = DEGREE5;
	DGUSLCD_IconDisplay(ICON_30BMP_C, 0);
  DGUSLCD_IconDisplay(ICON_30BMP_D, 1);
  DGUSLCD_IconDisplay(ICON_30BMP_E, 0);
}

void DGUSScreenHandler::EXTRUDER_Step1ButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  ExtruderScreen.Step = DEGREE1;
	DGUSLCD_IconDisplay(ICON_30BMP_C, 0);
  DGUSLCD_IconDisplay(ICON_30BMP_D, 0);
  DGUSLCD_IconDisplay(ICON_30BMP_E, 1);
}

void DGUSScreenHandler::EXTRUDER_SpeedlowButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  ExtruderScreen.Speed = SPEED_LOW;
  DGUSLCD_IconDisplay(ICON_30BMP_F, 1);
	DGUSLCD_IconDisplay(ICON_30BMP_G, 0);
	DGUSLCD_IconDisplay(ICON_30BMP_H, 0);
}

void DGUSScreenHandler::EXTRUDER_SpeednormalButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  ExtruderScreen.Speed = SPEED_NORMAL;
  DGUSLCD_IconDisplay(ICON_30BMP_F, 0);
	DGUSLCD_IconDisplay(ICON_30BMP_G, 1);
	DGUSLCD_IconDisplay(ICON_30BMP_H, 0);
}

void DGUSScreenHandler::EXTRUDER_SpeedfastButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  ExtruderScreen.Speed = SPEED_FAST;
  DGUSLCD_IconDisplay(ICON_30BMP_F, 0);
	DGUSLCD_IconDisplay(ICON_30BMP_G, 0);
	DGUSLCD_IconDisplay(ICON_30BMP_H, 1);
}

static void _EnsureTemperature(void)
{
  if((thermalManager.degTargetHotend(HID_E0) < PREHEAT_1_TEMP_HOTEND) || \
     (thermalManager.degHotend(HID_E0) < (PREHEAT_1_TEMP_HOTEND - TEMP_WINDOW))){
    thermalManager.setTargetHotend(PREHEAT_1_TEMP_HOTEND, HID_E0);
  }
}

static void _StoreOldestInfo(void)
{
  if(ExtruderScreen.isEAxisPositionChanged == false){
    ExtruderScreen.OldEAxisPosition = planner.get_axis_position_mm(E0_AXIS);
    ExtruderScreen.isEAxisPositionChanged = true;
  }
  if(ExtruderScreen.isTempTargetChanged == false){
    ExtruderScreen.OldTemperatureTarget = thermalManager.degTargetHotend(HID_E0);
    ExtruderScreen.isTempTargetChanged = true;
  }
}

void DGUSScreenHandler::EXTRUDER_LoadButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  _StoreOldestInfo();
  _EnsureTemperature();
  unscaled_e_move(ExtruderScreen.Step, MMM_TO_MMS(ExtruderScreen.Speed));
}

void DGUSScreenHandler::EXTRUDER_UnloadButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  _StoreOldestInfo();
  _EnsureTemperature();
  unscaled_e_move(-ExtruderScreen.Step, MMM_TO_MMS(ExtruderScreen.Speed));
}

void DGUSScreenHandler::DGUSLCD_SendExtruder1temperatureSpeedToDisplay(DGUS_VP_Variable &var)
{
  char temp[20];
  sprintf(temp, "%03d℃/%03d℃", (int)thermalManager.degHotend(HID_E0), (int)thermalManager.degTargetHotend(HID_E0)); 
  DGUSLCD_TextDisplay(VP_EXTRUDER_TEMPERATURE1_TEXT, temp, strlen(temp));
}

#endif
