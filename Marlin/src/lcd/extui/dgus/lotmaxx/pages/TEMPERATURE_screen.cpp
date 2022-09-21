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

typedef enum : uint8_t{
  HOTEND, HOTBED
}HotType;
typedef enum : uint8_t{
  DEGREE1 = 1,
  DEGREE5 = 5,
  DEGREE10 = 10,
}HotStep;

static HotStep temperature_interval = DEGREE1;
static HotType hot_type = HOTEND;

void DGUSScreenHandler::TEMPERATURE_ReturnButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  PopToOldScreen();
}

void DGUSScreenHandler::TEMPERATURE_PLAButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  thermalManager.setTargetHotend(PREHEAT_1_TEMP_HOTEND, HID_E0);
}

void DGUSScreenHandler::TEMPERATURE_ABSButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  thermalManager.setTargetHotend(PREHEAT_2_TEMP_HOTEND, HID_E0);
}

void DGUSScreenHandler::TEMPERATURE_HotbedButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  char temp[20];
  hot_type = HOTBED;
  DGUSLCD_IconDisplay(ICON_5BMP_A, 2);
  DGUSLCD_IconDisplay(ICON_5BMP_B, 1);
  DGUSLCD_IconDisplay(ICON_5BMP_C, 0);
  sprintf(temp,"%3d℃/%3d℃", (int)thermalManager.degBed(), (int)thermalManager.degTargetBed());
	DGUSLCD_TextDisplay(VP_PREHEAT_TEMPERATURE_TEXT, temp, strlen(temp));
}

void DGUSScreenHandler::TEMPERATURE_Extruder1ButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  char temp[20];
  hot_type = HOTEND;
  DGUSLCD_IconDisplay(ICON_5BMP_A,0);
  DGUSLCD_IconDisplay(ICON_5BMP_B,0);
  DGUSLCD_IconDisplay(ICON_5BMP_C,1);
  sprintf(temp, "%3d℃/%3d℃",(int)thermalManager.degHotend((uint8_t)HID_E0), \
          (int)thermalManager.degTargetHotend((uint8_t)HID_E0));
  DGUSLCD_TextDisplay(VP_PREHEAT_TEMPERATURE_TEXT, temp, strlen(temp));
}

void DGUSScreenHandler::TEMPERATURE_Step1ButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  temperature_interval = DEGREE1;
  DGUSLCD_IconDisplay(ICON_5BMP_E,1);
  DGUSLCD_IconDisplay(ICON_5BMP_F,0);
  DGUSLCD_IconDisplay(ICON_5BMP_G,0);
}

void DGUSScreenHandler::TEMPERATURE_Step5ButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  temperature_interval = DEGREE5;
  DGUSLCD_IconDisplay(ICON_5BMP_E,0);
  DGUSLCD_IconDisplay(ICON_5BMP_F,1);
  DGUSLCD_IconDisplay(ICON_5BMP_G,0);
}

void DGUSScreenHandler::TEMPERATURE_Step10ButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  temperature_interval = DEGREE10;
  DGUSLCD_IconDisplay(ICON_5BMP_E,0);
  DGUSLCD_IconDisplay(ICON_5BMP_F,0);
  DGUSLCD_IconDisplay(ICON_5BMP_G,1);
}

void DGUSScreenHandler::TEMPERATURE_SubButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  uint16_t target_temperature;
  if(hot_type == HOTBED){

    target_temperature = thermalManager.degTargetBed() - temperature_interval;
    NOLESS(target_temperature, 0);
    thermalManager.setTargetBed(target_temperature);
  } else if(hot_type == HOTEND){

    target_temperature = thermalManager.degTargetHotend((uint8_t)HID_E0) - temperature_interval;
    NOLESS(target_temperature, 0);
    thermalManager.setTargetHotend(target_temperature, (uint8_t)HID_E0);
  }
}

void DGUSScreenHandler::TEMPERATURE_AddButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  uint16_t target_temperature;
  if(hot_type == HOTBED){

    target_temperature = thermalManager.degTargetBed() + temperature_interval;
    NOMORE(target_temperature, BED_MAXTEMP);
    thermalManager.setTargetBed(target_temperature);
  } else if(hot_type == HOTEND){

    target_temperature = thermalManager.degTargetHotend((uint8_t)HID_E0) + temperature_interval;
    NOMORE(target_temperature, HEATER_0_MAXTEMP);
    thermalManager.setTargetHotend(target_temperature, (uint8_t)HID_E0);
  }
}

void DGUSScreenHandler::TEMPERATURE_StopButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  thermalManager.setTargetBed(0);
  thermalManager.setTargetHotend(0, HID_E0);
}

void DGUSScreenHandler::DGUSLCD_SendtemperatureToDisplay(DGUS_VP_Variable &var)
{
  char temp[20];
  if(hot_type == HOTBED){
    sprintf(temp,"%3d℃/%3d℃", (int)thermalManager.degBed(), (int)thermalManager.degTargetBed());
	  DGUSLCD_TextDisplay(VP_PREHEAT_TEMPERATURE_TEXT, temp, strlen(temp));
  } else if(hot_type == HOTEND){
    sprintf(temp, "%3d℃/%3d℃",(int)thermalManager.degHotend((uint8_t)HID_E0), \
          (int)thermalManager.degTargetHotend((uint8_t)HID_E0));
    DGUSLCD_TextDisplay(VP_PREHEAT_TEMPERATURE_TEXT, temp, strlen(temp));
  }
}

#endif
