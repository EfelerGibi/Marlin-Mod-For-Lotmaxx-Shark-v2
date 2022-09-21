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
#include "../../../../../module/endstops.h"
#include "../../../../../module/stepper.h"
#include "../../../../../feature/lotmaxx/printmode.h"
#include "../../../../../sd/cardreader.h"
#include "screen_common.h"

typedef enum : uint8_t{
  DEGREE1 = 1,
  DEGREE5 = 5,
  DEGREE10 = 10,
}Step;

static Step AxisStep = DEGREE1;

float focusLocation;

void DGUSScreenHandler::MOVE_LASER_ReturnButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  UpdateNewScreen(LOTMAXXLCD_SCREEN_MAIN);
}

void DGUSScreenHandler::MOVE_LASER_Z_AddButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  Move(Z_AXIS_CHAR, (float)AxisStep, ADD_CHAR, AXISMOVESPEED);
}

void DGUSScreenHandler::MOVE_LASER_Y_SubButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  Move(Y_AXIS_CHAR, (float)AxisStep, SUB_CHAR, AXISMOVESPEED);
}

void DGUSScreenHandler::MOVE_LASER_X_SubButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  Move(X_AXIS_CHAR, (float)AxisStep, SUB_CHAR, AXISMOVESPEED);
}

void DGUSScreenHandler::MOVE_LASER_HomeButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  queue.enqueue_one("G28");
}

void DGUSScreenHandler::MOVE_LASER_X_AddButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  Move(X_AXIS_CHAR, (float)AxisStep, ADD_CHAR, AXISMOVESPEED);
}

void DGUSScreenHandler::MOVE_LASER_Z_SubButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  Move(Z_AXIS_CHAR, (float)AxisStep, SUB_CHAR, AXISMOVESPEED);
}

void DGUSScreenHandler::MOVE_LASER_Y_AddButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  Move(Y_AXIS_CHAR, (float)AxisStep, ADD_CHAR, AXISMOVESPEED);
}

void DGUSScreenHandler::MOVE_LASER_SwitchButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  bool laserswitch = *(uint8_t*)val_ptr;
  if(laserswitch) {
    queue.enqueue_one("M3 I1 S30");
  } else {
    queue.enqueue_one("M3 I1 S0");
  }
}

void DGUSScreenHandler::MOVE_LASER_StepButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  uint8_t ButtonVal = *(uint8_t*)val_ptr;
  if(ButtonVal == 0) {
    AxisStep = DEGREE1;
  } else if (ButtonVal == 1) {
    AxisStep = DEGREE5;
  } else if (ButtonVal == 2) {
    AxisStep = DEGREE10;
  }
}

void DGUSScreenHandler::MOVE_LASER_Save_DistenceButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  static bool no_reentry = false;  // Prevent reentry
  if (no_reentry) return;
  REMEMBER(mh, no_reentry, true);

  char strtemp[20];
  char cmd[MAX_CMD_SIZE];

  endstops.enable(true);                                  // Enable endstops for next homing move
  abce_pos_t target = planner.get_axis_positions_mm();    // Get current position
  target[Z_AXIS] = 0;
  planner.set_machine_position_mm(target);
  target[Z_AXIS] = -Z_MAX_POS * 1.5;                      // The move will be towards the endstop
  planner.buffer_segment(target, homing_feedrate(Z_AXIS));// homing z axis
  planner.synchronize();

  // calculate focusLocation
  focusLocation = abs((stepper.triggered_position(Z_AXIS) / planner.settings.axis_steps_per_mm[Z_AXIS]));
  SERIAL_ECHOLNPGM("triggered_position ", focusLocation);
  
  set_axis_is_at_home(Z_AXIS);
  sync_plan_position();
  destination[Z_AXIS] = current_position[Z_AXIS];

  endstops.validate_homing_move();
  endstops.not_homing();
  
  sprintf_P(cmd, "G0 Z%s", dtostrf(focusLocation, 1, 3, strtemp));
  queue.enqueue_one(cmd);
  queue.enqueue_one("M500");
}

#endif
