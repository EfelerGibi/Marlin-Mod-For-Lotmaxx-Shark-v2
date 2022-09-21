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
#include "../../../../../lcd/extui/ui_api.h"

typedef enum : uint8_t{
  FAN0, FAN1
}FanType;

typedef enum : uint8_t{
  DEGREE5 = 5,
  DEGREE10 = 10,
  DEGREE20 = 20,
}FanSpeedStep;

static FanSpeedStep fanSpeedStep = DEGREE5;
static FanType fanType = FAN0;

void DGUSScreenHandler::FAN_ReturnButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  PopToOldScreen();
}

void DGUSScreenHandler::FAN_Fan1ButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  fanType = FAN0;
  DGUSLCD_IconDisplay(ICON_11BMP_A,0);
	DGUSLCD_IconDisplay(ICON_11BMP_B,1);
	DGUSLCD_IconDisplay(ICON_11BMP_C,0);
}

void DGUSScreenHandler::FAN_Fan2ButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  fanType = FAN1;
  DGUSLCD_IconDisplay(ICON_11BMP_A,1);
	DGUSLCD_IconDisplay(ICON_11BMP_B,0);
	DGUSLCD_IconDisplay(ICON_11BMP_C,1);
}

void DGUSScreenHandler::FAN_Step5ButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  fanSpeedStep = DEGREE5;
  DGUSLCD_IconDisplay(ICON_11BMP_D,1);
	DGUSLCD_IconDisplay(ICON_11BMP_E,0);
	DGUSLCD_IconDisplay(ICON_11BMP_F,0);
}

void DGUSScreenHandler::FAN_Step10ButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  fanSpeedStep = DEGREE10;
  DGUSLCD_IconDisplay(ICON_11BMP_D,0);
	DGUSLCD_IconDisplay(ICON_11BMP_E,1);
	DGUSLCD_IconDisplay(ICON_11BMP_F,0);
}

void DGUSScreenHandler::FAN_Step20ButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  fanSpeedStep = DEGREE20;
  DGUSLCD_IconDisplay(ICON_11BMP_D,0);
	DGUSLCD_IconDisplay(ICON_11BMP_E,0);
	DGUSLCD_IconDisplay(ICON_11BMP_F,1);
}

void DGUSScreenHandler::FAN_SubButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  int16_t fanSpeed_Target;
  if(fanType == FAN0){

    fanSpeed_Target = thermalManager.fan_speed[ExtUI::FAN0] - fanSpeedStep;
    NOLESS(fanSpeed_Target, FAN_MIN_PWM);
    thermalManager.set_fan_speed(ExtUI::FAN0, fanSpeed_Target);
  }
}

void DGUSScreenHandler::FAN_AddButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  int16_t fanSpeed_Target;
  if(fanType == FAN0){

    fanSpeed_Target = thermalManager.fan_speed[ExtUI::FAN0] + fanSpeedStep;
    NOMORE(fanSpeed_Target, FAN_MAX_PWM);
    thermalManager.set_fan_speed(ExtUI::FAN0, fanSpeed_Target);
  }
}

void DGUSScreenHandler::FAN_StopButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  thermalManager.zero_fan_speeds();
}

void DGUSScreenHandler::DGUSLCD_SendFanSpeedToDisplay(DGUS_VP_Variable &var)
{
  char temp[20];
  if(fanType == FAN0){
    sprintf(temp, "%d", thermalManager.fan_speed[ExtUI::FAN0]); 
	  DGUSLCD_TextDisplay(VP_FAN_VALUE_TEXT, temp, strlen(temp));
  } else if(fanType == FAN1){
    //  there is no interface to get the speed of the auto fans.
    //  The auto fan runs at a full duty cycle when the temperature of the hotend is greater than the threshold
    //  and shuts down when the temperature is lower than the threshold.
    if(thermalManager.degHotend(HID_E0) > 50){
      sprintf(temp, "%d", 255);
    } else {
      sprintf(temp, "%d", 0);
    }

    DGUSLCD_TextDisplay(VP_FAN_VALUE_TEXT, temp, strlen(temp));
  }
}

#endif
