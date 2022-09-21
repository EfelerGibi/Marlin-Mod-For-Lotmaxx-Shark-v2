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
#include "../../../../../module/stepper.h"
#include "screen_common.h"

typedef enum : uint8_t{
  E0, E1,
}EAXISType;

typedef enum : uint8_t{
  DEGREE01 = 1,
  DEGREE1 = 10,
  DEGREE10 = 100,
}Step;

static Step AxisStep = DEGREE01;
static EAXISType EAxisType = E0;

float GetAxisStep(void)
{
  return (float)AxisStep / 10.0;
}

void DGUSScreenHandler::MOVE_3D_ReturnButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  UpdateNewScreen(LOTMAXXLCD_SCREEN_CONTROL, false);
}

void DGUSScreenHandler::MOVE_3D_E_AXIS_SubButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  Move(E_AXIS_CHAR, GetAxisStep(), SUB_CHAR, AXISMOVESPEED);
}

void DGUSScreenHandler::MOVE_3D_Y_AXIS_SubButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  Move(Y_AXIS_CHAR, GetAxisStep(), SUB_CHAR, AXISMOVESPEED);
}

void DGUSScreenHandler::MOVE_3D_Z_AXIS_AddButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  Move(Z_AXIS_CHAR, GetAxisStep(), ADD_CHAR, AXISMOVESPEED);
}

void DGUSScreenHandler::MOVE_3D_X_AXIS_SubButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  Move(X_AXIS_CHAR, GetAxisStep(), SUB_CHAR, AXISMOVESPEED); 
}

void DGUSScreenHandler::MOVE_3D_ZeroButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
	queue.enqueue_one("G28");
}

void DGUSScreenHandler::MOVE_3D_X_AXIS_AddButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  Move(X_AXIS_CHAR, GetAxisStep(), ADD_CHAR, AXISMOVESPEED); 
}

void DGUSScreenHandler::MOVE_3D_E_AXIS_AddButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  Move(E_AXIS_CHAR, GetAxisStep(), ADD_CHAR, AXISMOVESPEED); 
}

void DGUSScreenHandler::MOVE_3D_Y_AXIS_AddButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  Move(Y_AXIS_CHAR, GetAxisStep(), ADD_CHAR, AXISMOVESPEED);
}

void DGUSScreenHandler::MOVE_3D_Z_AXIS_SubButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  Move(Z_AXIS_CHAR, GetAxisStep(), SUB_CHAR, AXISMOVESPEED); 
}

void DGUSScreenHandler::MOVE_3D_E0ButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  queue.enqueue_one("T0");
  DGUSLCD_IconDisplay(ICON_6BMP_A,1);
	DGUSLCD_IconDisplay(ICON_6BMP_B,0);
}

void DGUSScreenHandler::MOVE_3D_E1ButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  queue.enqueue_one("T1");
  DGUSLCD_IconDisplay(ICON_6BMP_A,0);
	DGUSLCD_IconDisplay(ICON_6BMP_B,1);
}

void DGUSScreenHandler::MOVE_3D_MOVE_Step01ButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  AxisStep = DEGREE01;
  DGUSLCD_IconDisplay(ICON_6BMP_C,1);
	DGUSLCD_IconDisplay(ICON_6BMP_D,0);
	DGUSLCD_IconDisplay(ICON_6BMP_E,0);
}

void DGUSScreenHandler::MOVE_3D_MOVE_Step1ButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  AxisStep = DEGREE1;
  DGUSLCD_IconDisplay(ICON_6BMP_C,0);
	DGUSLCD_IconDisplay(ICON_6BMP_D,1);
	DGUSLCD_IconDisplay(ICON_6BMP_E,0);
}

void DGUSScreenHandler::MOVE_3D_MOVE_Step10ButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  AxisStep = DEGREE10;
  DGUSLCD_IconDisplay(ICON_6BMP_C,0);
  DGUSLCD_IconDisplay(ICON_6BMP_D,0);
  DGUSLCD_IconDisplay(ICON_6BMP_E,1);
}

void DGUSScreenHandler::MOVE_3D_MOVE_StopButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  stepper.disable_all_steppers();
}

#endif
