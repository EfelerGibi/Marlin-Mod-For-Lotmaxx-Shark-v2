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
#pragma once

#include "../DGUSDisplay.h"
#include "../DGUSVPVariable.h"
#include "../DGUSDisplayDef.h"
#include "Language.h"
#include "../../../../inc/MarlinConfig.h"
#include "../../ui_api.h"

enum DGUSLCD_Screens : uint8_t;

typedef enum : uint8_t {
  PREPARE_WAIT_HEATING,           //< waiting for hotend arrive target temperature.
  PREPARE_WAIT_RETURNFIALMENT,    //< waiting for return filament.
  PREPARE_WAIT_INSTALLFIALMENT,   //< waiting for install filament.
  PREPARE_Finish,                 //< prepare process finished, start print.
} prepare_state_t;

class DGUSScreenHandler {
public:
  DGUSScreenHandler() = default;

  static bool loop();
  static void init();
  // Send all 4 strings that are displayed on the infoscreen, confirmation screen and kill screen
  // The bools specifying whether the strings are in RAM or FLASH.
  static void sendinfoscreen(const char *line1, const char *line2, const char *line3, const char *line4, bool l1inflash, bool l2inflash, bool l3inflash, bool liinflash);
  static void sendinfoscreen(FSTR_P const line1, FSTR_P const line2, const char *line3, const char *line4, bool l1inflash, bool l2inflash, bool l3inflash, bool liinflash) {
    sendinfoscreen(FTOP(line1), FTOP(line2), line3, line4, l1inflash, l2inflash, l3inflash, liinflash);
  }
  static void sendinfoscreen(FSTR_P const line1, FSTR_P const line2, FSTR_P const line3, FSTR_P const line4, bool l1inflash, bool l2inflash, bool l3inflash, bool liinflash) {
    sendinfoscreen(FTOP(line1), FTOP(line2), FTOP(line3), FTOP(line4), l1inflash, l2inflash, l3inflash, liinflash);
  }

  static void HandleUserConfirmationPopUp(uint16_t ConfirmVP, const char *line1, const char *line2, const char *line3, const char *line4, bool l1inflash, bool l2inflash, bool l3inflash, bool liinflash);

  // "M117" Message -- msg is a RAM ptr.
  static void setstatusmessage(const char *msg);
  // The same for messages from Flash
  static void setstatusmessagePGM(PGM_P const msg);
  // Callback for VP "Display wants to change screen on idle printer"
  static void ScreenChangeHookIfIdle(DGUS_VP_Variable &var, void *val_ptr);
  // Callback for VP "Screen has been changed"
  static void ScreenChangeHook(DGUS_VP_Variable &var, void *val_ptr);

  // Callback for VP "All Heaters Off"
  static void HandleAllHeatersOff(DGUS_VP_Variable &var, void *val_ptr);
  // Hook for "Change this temperature"
  static void HandleTemperatureChanged(DGUS_VP_Variable &var, void *val_ptr);
  // Hook for "Change Flowrate"
  static void HandleFlowRateChanged(DGUS_VP_Variable &var, void *val_ptr);
  #if ENABLED(DGUS_UI_MOVE_DIS_OPTION)
    // Hook for manual move option
    static void HandleManualMoveOption(DGUS_VP_Variable &var, void *val_ptr);
  #endif

  // Hook for manual move.
  static void HandleManualMove(DGUS_VP_Variable &var, void *val_ptr);
  // Hook for manual extrude.
  static void HandleManualExtrude(DGUS_VP_Variable &var, void *val_ptr);
  // Hook for motor lock and unlook
  static void HandleMotorLockUnlock(DGUS_VP_Variable &var, void *val_ptr);
  #if ENABLED(POWER_LOSS_RECOVERY)
    // Hook for power loss recovery.
    static void HandlePowerLossRecovery(DGUS_VP_Variable &var, void *val_ptr);
  #endif
  // Hook for settings
  static void HandleSettings(DGUS_VP_Variable &var, void *val_ptr);
  static void HandleStepPerMMChanged(DGUS_VP_Variable &var, void *val_ptr);
  static void HandleStepPerMMExtruderChanged(DGUS_VP_Variable &var, void *val_ptr);

  #if HAS_PID_HEATING
    // Hook for "Change this temperature PID para"
    static void HandleTemperaturePIDChanged(DGUS_VP_Variable &var, void *val_ptr);
    // Hook for PID autotune
    static void HandlePIDAutotune(DGUS_VP_Variable &var, void *val_ptr);
  #endif
  #if HAS_BED_PROBE
    // Hook for "Change probe offset z"
    static void HandleProbeOffsetZChanged(DGUS_VP_Variable &var, void *val_ptr);
  #endif
  #if ENABLED(BABYSTEPPING)
    // Hook for live z adjust action
    static void HandleLiveAdjustZ(DGUS_VP_Variable &var, void *val_ptr);
  #endif
  #if HAS_FAN
    // Hook for fan control
    static void HandleFanControl(DGUS_VP_Variable &var, void *val_ptr);
  #endif
  // Hook for heater control
  static void HandleHeaterControl(DGUS_VP_Variable &var, void *val_ptr);
  #if ENABLED(DGUS_PREHEAT_UI)
    // Hook for preheat
    static void HandlePreheat(DGUS_VP_Variable &var, void *val_ptr);
  #endif
  #if ENABLED(DGUS_FILAMENT_LOADUNLOAD)
    // Hook for filament load and unload filament option
    static void HandleFilamentOption(DGUS_VP_Variable &var, void *val_ptr);
    // Hook for filament load and unload
    static void HandleFilamentLoadUnload(DGUS_VP_Variable &var);
  #endif

  #if ENABLED(SDSUPPORT)
    // Callback for VP "Display wants to change screen when there is a SD card"
    static void ScreenChangeHookIfSD(DGUS_VP_Variable &var, void *val_ptr);
    // Scroll buttons on the file listing screen.
    static void DGUSLCD_SD_ScrollFilelist(DGUS_VP_Variable &var, void *val_ptr);
    // File touched.
    static void DGUSLCD_SD_FileSelected(DGUS_VP_Variable &var, void *val_ptr);
    // start print after confirmation received.
    static void DGUSLCD_SD_StartPrint();
    // User hit the pause, resume or abort button.
    static void DGUSLCD_SD_ResumePauseAbort(DGUS_VP_Variable &var, void *val_ptr);
    // User confirmed the abort action
    static void DGUSLCD_SD_ReallyAbort(DGUS_VP_Variable &var, void *val_ptr);
    // User hit the tune button
    static void DGUSLCD_SD_PrintTune(DGUS_VP_Variable &var, void *val_ptr);
    // Send a single filename to the display.
    static void DGUSLCD_SD_SendFilename(DGUS_VP_Variable &var);
    // Marlin informed us that a new SD has been inserted.
    static void SDCardInserted();
    // Marlin informed us that the SD Card has been removed().
    static void SDCardRemoved();
    // Marlin informed us about a bad SD Card.
    static void SDCardError();
  #endif

  // OK Button the Confirm screen.
  static void ScreenConfirmedOK(DGUS_VP_Variable &var, void *val_ptr);

  // Update data after went to new screen (by display or by GotoScreen)
  // remember: store the last-displayed screen, so it can get returned to.
  // (e.g for pop up messages)
  static void UpdateNewScreen(DGUSLCD_Screens newscreen, bool popup=false);

  // Recall the remembered screen.
  static void PopToOldScreen();

  // Make the display show the screen and update all VPs in it.
  static void GotoScreen(DGUSLCD_Screens screen, bool ispopup = false);

  static void UpdateScreenVPData();

  // Helpers to convert and transfer data to the display.
  static void DGUSLCD_SendWordValueToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendStringToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendStringToDisplayPGM(DGUS_VP_Variable &var);
  static void DGUSLCD_SendTemperaturePID(DGUS_VP_Variable &var);
  static void DGUSLCD_SendPercentageToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendPrintProgressToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendPrintTimeToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendPrintedFilenameToDisplay(DGUS_VP_Variable &var);
  
  #if ENABLED(PRINTCOUNTER)
    static void DGUSLCD_SendPrintAccTimeToDisplay(DGUS_VP_Variable &var);
    static void DGUSLCD_SendPrintsTotalToDisplay(DGUS_VP_Variable &var);
  #endif
  #if HAS_FAN
    static void DGUSLCD_SendFanStatusToDisplay(DGUS_VP_Variable &var);
  #endif
  static void DGUSLCD_SendHeaterStatusToDisplay(DGUS_VP_Variable &var);
  #if ENABLED(DGUS_UI_WAITING)
    static void DGUSLCD_SendWaitingStatusToDisplay(DGUS_VP_Variable &var);
  #endif

  // Send a value from 0..100 to a variable with a range from 0..255
  static void DGUSLCD_PercentageToUint8(DGUS_VP_Variable &var, void *val_ptr);

  template<typename T>
  static void DGUSLCD_SetValueDirectly(DGUS_VP_Variable &var, void *val_ptr) {
    if (!var.memadr) return;
    union { unsigned char tmp[sizeof(T)]; T t; } x;
    unsigned char *ptr = (unsigned char*)val_ptr;
    LOOP_L_N(i, sizeof(T)) x.tmp[i] = ptr[sizeof(T) - i - 1];
    *(T*)var.memadr = x.t;
  }

  // Send a float value to the display.
  // Display will get a 4-byte integer scaled to the number of digits:
  // Tell the display the number of digits and it cheats by displaying a dot between...
  template<unsigned int decimals>
  static void DGUSLCD_SendFloatAsLongValueToDisplay(DGUS_VP_Variable &var) {
    if (var.memadr) {
      float f = *(float *)var.memadr;
      f *= cpow(10, decimals);
      dgusdisplay.WriteVariable(var.VP, (long)f);
    }
  }

  // Send a float value to the display.
  // Display will get a 2-byte integer scaled to the number of digits:
  // Tell the display the number of digits and it cheats by displaying a dot between...
  template<unsigned int decimals>
  static void DGUSLCD_SendFloatAsIntValueToDisplay(DGUS_VP_Variable &var) {
    if (var.memadr) {
      float f = *(float *)var.memadr;
      DEBUG_ECHOLNPGM(" >> ", f, 6);
      f *= cpow(10, decimals);
      dgusdisplay.WriteVariable(var.VP, (int16_t)f);
    }
  }

  // Force an update of all VP on the current screen.
  static void ForceCompleteUpdate() { update_ptr = 0; ScreenComplete = false; }
  // Has all VPs sent to the screen
  static bool IsScreenComplete() { return ScreenComplete; }

  static DGUSLCD_Screens getCurrentScreen() { return current_screen; }

  static void SetupConfirmAction( void (*f)()) { confirm_action_cb = f; }

  //  lotmaxx
  static void filamentRunout();
  static char get_language_choice(void);
  static char *get_language_choice_p(void);
  static void set_display_language(void);
  static void DIWEN_SetText(void);
  static void DGUSLCD_TextDisplay(uint16_t adr, const void *values, uint8_t valueslen);
  static void DGUSLCD_TextDisplayPGM(uint16_t adr, const void *values, uint8_t valueslen);
  static void DGUSLCD_IconDisplay(uint16_t adr, uint16_t value);
  static void DGUSLCD_SetIcon(void);

  // system
  static void DGUSLCD_PowerOnResponse(DGUS_VP_Variable &var, void *val_ptr);
  
  //  MAIN page
  static void GotoFLELIST(DGUS_VP_Variable &var, void *val_ptr);
  static void GotoCONTROL(DGUS_VP_Variable &var, void *val_ptr);
  static void GotoSETTING(DGUS_VP_Variable &var, void *val_ptr);
  
  //  FILELIST page
  static void FILELIST_ReturnButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void FILELIST_PageupButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void FILELIST_PagedownButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void FILELIST_UpButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void DGUSLCD_SendPathToDisplay(DGUS_VP_Variable &var);
  
  //  CONTROL page
  static void CONTROL_ReturnButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void CONTROL_TempratureButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void CONTROL_MoveButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void CONTROL_ZeroButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void CONTROL_ExtrusionButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void CONTROL_LevelingButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void CONTROL_FanButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void CONTROL_StopButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void CONTROL_BeepButtonHandler(DGUS_VP_Variable &var, void *val_ptr);

  //  SETTING page
  static void SETTING_ReturnButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void SETTING_AboutButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void SETTING_LanguageButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void SETTING_BreakButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void SETTING_StateButtonHandler(DGUS_VP_Variable &var, void *val_ptr);

  // SELECT EXTRUDER page
  static void SELECT_EXTRUDER_1_ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void SELECT_EXTRUDER_2_ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void SELECT_EXTRUDER_DoubleButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void SELECT_EXTRUDER_CancelButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  
  // EXTRUDER CONFIRM page
  static void EXTRUDERCONFIRM_CONFIRMButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void EXTRUDERCONFIRM_CANCELButtonHandler(DGUS_VP_Variable &var, void *val_ptr);

  // PREPARE page
  static void PREPARE_StopButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void PREPARE_RefreshTemperature(DGUS_VP_Variable &var);

  // PRINT page
  static void PRINT_StopButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void PRINT_ControlButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void PRINT_RefreshPrintTime(DGUS_VP_Variable &var);
  static void DGUSLCD_SendHotbedToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendHotend1ToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendSpeedToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendPrintZoffsetToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendPrintExtrusionToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendPrintPercentToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendPrintedTimeToDisplay(DGUS_VP_Variable &var);

  // PRINT CONTROL page
  static void PRINT_CONTROL_ReturnButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void PRINT_CONTROL_TemperatureButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void PRINT_CONTROL_FanButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void PRINT_CONTROL_SpeedButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void PRINT_CONTROL_ExtrusionButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void PRINT_CONTROL_ZoffsetButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void DGUSLCD_SendPrintCtrlSpeedToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendPrintCtrlZoffsetToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendPrintCtrlExtrusionToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendPrintCtrlExtruder1ToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendPrintCtrlHotbedToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendPrintCtrlFan1ToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendPrintCtrlFan2ToDisplay(DGUS_VP_Variable &var);

  // TEMPERATURE page
  static void TEMPERATURE_ReturnButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void TEMPERATURE_PLAButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void TEMPERATURE_ABSButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void TEMPERATURE_HotbedButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void TEMPERATURE_Extruder1ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void TEMPERATURE_Step1ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void TEMPERATURE_Step5ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void TEMPERATURE_Step10ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void TEMPERATURE_SubButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void TEMPERATURE_AddButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void TEMPERATURE_StopButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void DGUSLCD_SendtemperatureToDisplay(DGUS_VP_Variable &var);

  // FAN page
  static void FAN_ReturnButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void FAN_Fan1ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void FAN_Fan2ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void FAN_Step5ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void FAN_Step10ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void FAN_Step20ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void FAN_SubButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void FAN_AddButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void FAN_StopButtonHandler(DGUS_VP_Variable &var, void *val_ptr);

  static void DGUSLCD_SendFanSpeedToDisplay(DGUS_VP_Variable &var);

  // MOVE page
  static void MOVE_3D_ReturnButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void MOVE_3D_E_AXIS_SubButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void MOVE_3D_Y_AXIS_SubButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void MOVE_3D_Z_AXIS_AddButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void MOVE_3D_X_AXIS_SubButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void MOVE_3D_ZeroButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void MOVE_3D_X_AXIS_AddButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void MOVE_3D_E_AXIS_AddButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void MOVE_3D_Y_AXIS_AddButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void MOVE_3D_Z_AXIS_SubButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void MOVE_3D_E0ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void MOVE_3D_E1ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void MOVE_3D_MOVE_Step01ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void MOVE_3D_MOVE_Step1ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void MOVE_3D_MOVE_Step10ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void MOVE_3D_MOVE_StopButtonHandler(DGUS_VP_Variable &var, void *val_ptr);

  // EXTRUDER page
  static void EXTRUDER_ReturnButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void EXTRUDER_E0ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void EXTRUDER_E1ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void EXTRUDER_Step10ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void EXTRUDER_Step5ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void EXTRUDER_Step1ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void EXTRUDER_SpeedlowButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void EXTRUDER_SpeednormalButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void EXTRUDER_SpeedfastButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void EXTRUDER_LoadButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void EXTRUDER_UnloadButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void DGUSLCD_SendExtruder1temperatureSpeedToDisplay(DGUS_VP_Variable &var);

  // ZERO page
  static void ZERO_3D_ReturnButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void ZERO_3D_X_AxisButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void ZERO_3D_Y_AxisButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void ZERO_3D_Z_AxisButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void ZERO_3D_AllButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void ZERO_3D_StopButtonHandler(DGUS_VP_Variable &var, void *val_ptr);

  // LNAGUAGE page
  static void LANGUAGE_ReturnButtunHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void LANGUAGE_SelectButtunHandler(DGUS_VP_Variable &var, void *val_ptr);

  // STATUS page
  static void STATUS_ReturnButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void DGUSLCD_SendStatusPositionToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendStatusHotend1ToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendStatusHotbedToDisplay(DGUS_VP_Variable &var);

  // PRINT STOP page
  static void STOP_CancelButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void STOP_ConfirmButtonHandler(DGUS_VP_Variable &var, void *val_ptr);

  // SPEED page
  static void SPEED_ReturnButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void SPEED_Step5ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void SPEED_Step10ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void SPEED_Step20ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void SPEED_AddButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void SPEED_SubButtonHandler(DGUS_VP_Variable &var, void *val_ptr);

  // EXTRUSION page
  static void EXTRUSION_ReturnButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void EXTRUSION_Step5ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void EXTRUSION_Step10ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void EXTRUSION_Step20ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void EXTRUSION_AddButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void EXTRUSION_SubButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void DGUSLCD_SendExtrusionToDisplay(DGUS_VP_Variable &var);

  // ZOFFSET page
  static void ZOFFSET_ReturnButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void ZOFFSET_Step01ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void ZOFFSET_Step1ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void ZOFFSET_AddButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void ZOFFSET_SubButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void DGUSLCD_SendZoffsetToDisplay(DGUS_VP_Variable &var);

  // LEVELING page
  static void LEVELING_ReturnButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void LEVELING_AutoLevelingButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void LEVELING_ManualButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void LEVELING_Step01ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void LEVELING_Step005ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void LEVELING_AddButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void LEVELING_SubButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void DGUSLCD_SendLevelingOffsetToDisplay(DGUS_VP_Variable &var);

  // MANUAL LEVELING page
  static void MANUAL_LEVELING_ReturnButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void MANUAL_LEVELING_Point1ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void MANUAL_LEVELING_Point2ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void MANUAL_LEVELING_Point3ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void MANUAL_LEVELING_Point4ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void MANUAL_LEVELING_Point5ButtonHandler(DGUS_VP_Variable &var, void *val_ptr);

  // PRINT COMPLETED page
  static void PRINT_COMPLETED_ConfirmButtonHandler(DGUS_VP_Variable &var, void *val_ptr);

  // ABOUT page
  static void ABOUT_ReturnButtonHandler(DGUS_VP_Variable &var, void *val_ptr);

  // MOVE LASER page
  static void MOVE_LASER_ReturnButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void MOVE_LASER_Z_AddButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void MOVE_LASER_Y_SubButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void MOVE_LASER_X_SubButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void MOVE_LASER_HomeButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void MOVE_LASER_X_AddButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void MOVE_LASER_Z_SubButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void MOVE_LASER_Y_AddButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void MOVE_LASER_SwitchButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void MOVE_LASER_StepButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void MOVE_LASER_Save_DistenceButtonHandler(DGUS_VP_Variable &var, void *val_ptr);

  // LASER PRINT page
  static void PRINT_LASER_StopButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void PRINT_LASER_PauseButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void PRINT_LASER_Z_AddButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void PRINT_LASER_Z_SubButtonHandler(DGUS_VP_Variable &var, void *val_ptr);

  // SELECT page
  static void SELECT_CancelButtonHandler(DGUS_VP_Variable &var, void *val_ptr);
  static void SELECT_ConfirmButtonHandler(DGUS_VP_Variable &var, void *val_ptr);

  static void SDCardPrintFinished();

private:
  static DGUSLCD_Screens current_screen;  //< currently on screen
  static constexpr uint8_t NUM_PAST_SCREENS = 4;
  static DGUSLCD_Screens past_screens[NUM_PAST_SCREENS]; //< LIFO with past screens for the "back" button.

  static uint8_t update_ptr;      //< Last sent entry in the VPList for the actual screen.
  static uint16_t skipVP;         //< When updating the screen data, skip this one, because the user is interacting with it.
  static bool ScreenComplete;     //< All VPs sent to screen?

  static uint16_t ConfirmVP;      //< context for confirm screen (VP that will be emulated-sent on "OK").

  #if ENABLED(SDSUPPORT)
    static int16_t top_file;      //< file on top of file chooser
    static int16_t file_to_print; //< touched file to be confirmed
    static ExtUI::FileList filelist;//< current dir file list
  #endif
  static TextDisplay text_display;

  static void (*confirm_action_cb)();

  static prepare_state_t prepare_state;
};

#if ENABLED(POWER_LOSS_RECOVERY)
  #define PLR_SCREEN_RECOVER DGUSLCD_SCREEN_SDPRINTMANIPULATION
  #define PLR_SCREEN_CANCEL DGUSLCD_SCREEN_STATUS
#endif
