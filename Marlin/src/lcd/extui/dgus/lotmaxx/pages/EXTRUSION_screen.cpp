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

constexpr uint16_t MAX_EXTRUSION = 500;
constexpr uint16_t MIN_EXTRUSION = 10;

typedef enum : uint8_t{
  DEGREE5 = 5,
  DEGREE10 = 10,
  DEGREE20 = 20,
}Step_t;

Step_t Step = DEGREE5;

void DGUSScreenHandler::EXTRUSION_ReturnButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  GotoScreen(LOTMAXXLCD_SCREEN_PRINT_CTRL);
}

void DGUSScreenHandler::EXTRUSION_Step5ButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  Step = DEGREE5;
  DGUSLCD_IconDisplay(ICON_29BMP_A, 1);
  DGUSLCD_IconDisplay(ICON_29BMP_B, 0);
  DGUSLCD_IconDisplay(ICON_29BMP_C, 0);
}

void DGUSScreenHandler::EXTRUSION_Step10ButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  Step = DEGREE10;
  DGUSLCD_IconDisplay(ICON_29BMP_A, 0);
  DGUSLCD_IconDisplay(ICON_29BMP_B, 1);
  DGUSLCD_IconDisplay(ICON_29BMP_C, 0);
}

void DGUSScreenHandler::EXTRUSION_Step20ButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  Step = DEGREE20;
  DGUSLCD_IconDisplay(ICON_29BMP_A, 0);
  DGUSLCD_IconDisplay(ICON_29BMP_B, 0);
  DGUSLCD_IconDisplay(ICON_29BMP_C, 1);
}

void DGUSScreenHandler::EXTRUSION_AddButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  int16_t extrusion;
  extrusion = *(int16_t *)var.memadr + Step;
  NOMORE(extrusion, MAX_EXTRUSION);
  
  planner.set_flow(ExtUI::E0, extrusion);
  planner.set_flow(ExtUI::E1, extrusion);
}

void DGUSScreenHandler::EXTRUSION_SubButtonHandler(DGUS_VP_Variable &var, void *val_ptr)
{
  int16_t extrusion;
  extrusion = *(int16_t *)var.memadr - Step;
  NOLESS(extrusion, MIN_EXTRUSION);
  
  planner.set_flow(ExtUI::E0, extrusion);
  planner.set_flow(ExtUI::E1, extrusion);
}

void DGUSScreenHandler::DGUSLCD_SendExtrusionToDisplay(DGUS_VP_Variable &var)
{
  char temp[20];
  sprintf(temp,"%d%%", *(int16_t *)var.memadr);
  DGUSLCD_TextDisplay(VP_EXTRUSION_TEXT, temp, strlen(temp));
}

#endif
