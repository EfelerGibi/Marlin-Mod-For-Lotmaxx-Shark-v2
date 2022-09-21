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

#include "../../../../inc/MarlinConfigPre.h"

#if ENABLED(DGUS_LCD_UI_LOTMAXX)

#include "../DGUSScreenHandler.h"

#include "../../../../MarlinCore.h"
#include "../../../../gcode/queue.h"
#include "../../../../libs/duration_t.h"
#include "../../../../module/settings.h"
#include "../../../../module/temperature.h"
#include "../../../../module/motion.h"
#include "../../../../module/planner.h"
#include "../../../../module/printcounter.h"
#include "../../../../sd/cardreader.h"
#include "../../../../feature/lotmaxx/printmode.h"
#include "Language.h"
#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../../feature/powerloss.h"
#endif

extern float focusLocation;
TextDisplay DGUSScreenHandler::text_display;

#if ENABLED(SDSUPPORT)

  void DGUSScreenHandler::DGUSLCD_SD_FileSelected(DGUS_VP_Variable &var, void *val_ptr) {
    //  DWIN Project filelist button begin with 1(usually arranged as follows: 1, 2, 3, 4, 5, 6)
    uint16_t touched_nr = (int16_t)*(uint8_t*)val_ptr + top_file - 1;     
    if (touched_nr > filelist.count()) return;
    if (!filelist.seek(touched_nr)) return;

    if (filelist.isDir()) {
      filelist.changeDir(filelist.filename());
      top_file = 0;
      ForceCompleteUpdate();
      return;
    }

    #if ENABLED(DGUS_PRINT_FILENAME)
      // Send print filename
      DGUSLCD_TextDisplay(VP_SD_Print_Filename, filelist.filename(), VP_SD_FileName_LEN);
    #endif

    file_to_print = touched_nr;

	if(IS_PRINT3D_PRINTMODE()) {
		GotoScreen(LOTMAXXLCD_SCREEN_SELECT_EXTRUDER);
	} else if(IS_LASER_PRINTMODE()) {
		GotoScreen(LOTMAXXLCD_SCREEN_SELECT);
	}
  }

  void DGUSScreenHandler::DGUSLCD_SD_StartPrint() {
    if (!filelist.seek(file_to_print)) return;
    ExtUI::printFile(filelist.shortFilename());
  }

  void DGUSScreenHandler::DGUSLCD_SD_ResumePauseAbort(DGUS_VP_Variable &var, void *val_ptr) {
	
	// avoid race condition when user stays in this menu and printer finishes.
    if (!ExtUI::isPrintingFromMedia()) return; 
    uint16_t ButtonVal = (*(uint16_t*)val_ptr) & 0x00FF;
	switch (ButtonVal) {
      case 0:  // Resume
        if (ExtUI::isPrintingFromMediaPaused()) {
          ExtUI::resumePrint();
		  if(IS_PRINT3D_PRINTMODE()){
			DGUSLCD_TextDisplay(VP_PRINT_PAUSE_TEXT, text_display.L_Pause, strlen(text_display.L_Pause));
		  } else if(IS_LASER_PRINTMODE()){
			queue.enqueue_one_now("M3");
			DGUSLCD_TextDisplay(VP_PRINT_LASER_PAUSE_TEXT, text_display.L_Pause, strlen(text_display.L_Pause));
		  }
		  
        }
      	break;

      case 1: // Pause
        if (!ExtUI::isPrintingFromMediaPaused()) {
          ExtUI::pausePrint();
		  if(IS_PRINT3D_PRINTMODE()){
			DGUSLCD_TextDisplay(VP_PRINT_PAUSE_TEXT, text_display.L_Pursue, strlen(text_display.L_Pursue));
		  } else if(IS_LASER_PRINTMODE()){
			queue.inject("M5");
			DGUSLCD_TextDisplay(VP_PRINT_LASER_PAUSE_TEXT, text_display.L_Pursue, strlen(text_display.L_Pursue));
		  }
          
        }
        break;
	  default:
	  	break;
    }
  }

  void DGUSScreenHandler::DGUSLCD_SD_SendFilename(DGUS_VP_Variable& var) {
	constexpr uint16_t GcodeIcon = 0x0000;
	constexpr uint16_t DirIcon = 0x0001;
	char tmpfilename[VP_SD_FileName_LEN + 1] = "";
    uint16_t target_line = (var.VP - VP_SD_FileName0) / VP_SD_FileName_LEN;

    if (target_line > DGUS_SD_FILESPERSCREEN) return;
	//  avoid to sdcard is not exist when read filelist
	if (!IS_SD_INSERTED()){
		GotoScreen(LOTMAXXLCD_SCREEN_MAIN);
		return ;
	}

    var.memadr = (void*)tmpfilename;

    if (filelist.seek(top_file + target_line)) {
      snprintf_P(tmpfilename, VP_SD_FileName_LEN, PSTR("%s%c"), filelist.filename(), filelist.isDir() ? '/' : 0); // snprintf_P(tmpfilename, VP_SD_FileName_LEN, PSTR("%s"), filelist.filename());
	}
    DGUSLCD_TextDisplay(var.VP, var.memadr, var.size);

	if(filelist.isDir()){
		dgusdisplay.WriteVariable(GETGCODE_ICON1 + target_line, DirIcon);
	} else {
		dgusdisplay.WriteVariable(GETGCODE_ICON1 + target_line, GcodeIcon);
	}
	
  }

  void DGUSScreenHandler::SDCardInserted() {
    top_file = 0;
    filelist.refresh();
  }

  void DGUSScreenHandler::SDCardRemoved() {
  }

#endif // SDSUPPORT

  void DGUSScreenHandler::filamentRunout() {
    ScreenHandler.GotoScreen(LOTMAXXLCD_SCREEN_EXTRUDER);
  }

void DGUSScreenHandler::ScreenChangeHook(DGUS_VP_Variable &var, void *val_ptr) {
  uint8_t *tmp = (uint8_t*)val_ptr;

  // The keycode in target is coded as <from-frame><to-frame>, so 0x0100A means
  // from screen 1 (main) to 10 (temperature). DGUSLCD_SCREEN_POPUP is special,
  // meaning "return to previous screen"
  DGUSLCD_Screens target = (DGUSLCD_Screens)tmp[1];

  DEBUG_ECHOLNPGM("\n DEBUG target", target);

  if (target == DGUSLCD_SCREEN_POPUP) {
    // Special handling for popup is to return to previous menu
    if (current_screen == DGUSLCD_SCREEN_POPUP && confirm_action_cb) confirm_action_cb();
    PopToOldScreen();
    return;
  }

  UpdateNewScreen(target);

  #ifdef DEBUG_DGUSLCD
    if (!DGUSLCD_FindScreenVPMapList(target)) DEBUG_ECHOLNPGM("WARNING: No screen Mapping found for ", target);
  #endif
}

void DGUSScreenHandler::HandleManualMove(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleManualMove");

  int16_t movevalue = swap16(*(uint16_t*)val_ptr);
  #if ENABLED(DGUS_UI_MOVE_DIS_OPTION)
    if (movevalue) {
      const uint16_t choice = *(uint16_t*)var.memadr;
      movevalue = movevalue < 0 ? -choice : choice;
    }
  #endif
  char axiscode;
  unsigned int speed = 1500; // FIXME: get default feedrate for manual moves, don't hardcode.

  switch (var.VP) {
    default: return;

    case VP_MOVE_X:
      axiscode = 'X';
      if (!ExtUI::canMove(ExtUI::axis_t::X)) goto cannotmove;
      break;

    case VP_MOVE_Y:
      axiscode = 'Y';
      if (!ExtUI::canMove(ExtUI::axis_t::Y)) goto cannotmove;
      break;

    case VP_MOVE_Z:
      axiscode = 'Z';
      speed = 300; // default to 5mm/s
      if (!ExtUI::canMove(ExtUI::axis_t::Z)) goto cannotmove;
      break;

    case VP_HOME_ALL: // only used for homing
      axiscode  = '\0';
      movevalue = 0; // ignore value sent from display, this VP is _ONLY_ for homing.
      break;
  }

  if (!movevalue) {
    // homing
    DEBUG_ECHOPGM(" homing ", AS_CHAR(axiscode));
    char buf[6] = "G28 X";
    buf[4] = axiscode;
    //DEBUG_ECHOPGM(" ", buf);
    queue.enqueue_one_now(buf);
    //DEBUG_ECHOLNPGM(" ✓");
    ForceCompleteUpdate();
    return;
  }
  else {
    // movement
    DEBUG_ECHOPGM(" move ", AS_CHAR(axiscode));
    bool old_relative_mode = relative_mode;
    if (!relative_mode) {
      //DEBUG_ECHOPGM(" G91");
      queue.enqueue_now(F("G91"));
      //DEBUG_ECHOPGM(" ✓ ");
    }
    char buf[32]; // G1 X9999.99 F12345
    unsigned int backup_speed = MMS_TO_MMM(feedrate_mm_s);
    char sign[] = "\0";
    int16_t value = movevalue / 100;
    if (movevalue < 0) { value = -value; sign[0] = '-'; }
    int16_t fraction = ABS(movevalue) % 100;
    snprintf_P(buf, 32, PSTR("G0 %c%s%d.%02d F%d"), axiscode, sign, value, fraction, speed);
    //DEBUG_ECHOPGM(" ", buf);
    queue.enqueue_one_now(buf);
    //DEBUG_ECHOLNPGM(" ✓ ");
    if (backup_speed != speed) {
      snprintf_P(buf, 32, PSTR("G0 F%d"), backup_speed);
      queue.enqueue_one_now(buf);
      //DEBUG_ECHOPGM(" ", buf);
    }
    // while (!enqueue_and_echo_command(buf)) idle();
    //DEBUG_ECHOLNPGM(" ✓ ");
    if (!old_relative_mode) {
      //DEBUG_ECHOPGM("G90");
      queue.enqueue_now(F("G90"));
      //DEBUG_ECHOPGM(" ✓ ");
    }
  }

  ForceCompleteUpdate();
  DEBUG_ECHOLNPGM("manmv done.");
  return;

  cannotmove:
    DEBUG_ECHOLNPGM(" cannot move ", AS_CHAR(axiscode));
    return;
}

#if HAS_PID_HEATING
  void DGUSScreenHandler::HandleTemperaturePIDChanged(DGUS_VP_Variable &var, void *val_ptr) {
    uint16_t rawvalue = swap16(*(uint16_t*)val_ptr);
    DEBUG_ECHOLNPGM("V1:", rawvalue);
    float value = (float)rawvalue / 10;
    DEBUG_ECHOLNPGM("V2:", value);
    float newvalue = 0;

    switch (var.VP) {
      default: return;
        #if HAS_HOTEND
          case VP_E0_PID_P: newvalue = value; break;
          case VP_E0_PID_I: newvalue = scalePID_i(value); break;
          case VP_E0_PID_D: newvalue = scalePID_d(value); break;
        #endif
        #if HAS_MULTI_HOTEND
          case VP_E1_PID_P: newvalue = value; break;
          case VP_E1_PID_I: newvalue = scalePID_i(value); break;
          case VP_E1_PID_D: newvalue = scalePID_d(value); break;
        #endif
        #if HAS_HEATED_BED
          case VP_BED_PID_P: newvalue = value; break;
          case VP_BED_PID_I: newvalue = scalePID_i(value); break;
          case VP_BED_PID_D: newvalue = scalePID_d(value); break;
        #endif
    }

    DEBUG_ECHOLNPGM("V3:", newvalue);
    *(float *)var.memadr = newvalue;

    skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
  }
#endif // HAS_PID_HEATING

#if ENABLED(BABYSTEPPING)
  void DGUSScreenHandler::HandleLiveAdjustZ(DGUS_VP_Variable &var, void *val_ptr) {
    DEBUG_ECHOLNPGM("HandleLiveAdjustZ");
    int16_t flag  = swap16(*(uint16_t*)val_ptr),
            steps = flag ? -20 : 20;
    ExtUI::smartAdjustAxis_steps(steps, ExtUI::axis_t::Z, true);
    ForceCompleteUpdate();
  }
#endif

#if ENABLED(DGUS_FILAMENT_LOADUNLOAD)

  void DGUSScreenHandler::HandleFilamentOption(DGUS_VP_Variable &var, void *val_ptr) {
    DEBUG_ECHOLNPGM("HandleFilamentOption");

    uint8_t e_temp = 0;
    filament_data.heated = false;
    uint16_t preheat_option = swap16(*(uint16_t*)val_ptr);
    if (preheat_option <= 8) {      // Load filament type
      filament_data.action = 1;
    }
    else if (preheat_option >= 10) { // Unload filament type
      preheat_option -= 10;
      filament_data.action = 2;
      filament_data.purge_length = DGUS_FILAMENT_PURGE_LENGTH;
    }
    else {                          // Cancel filament operation
      filament_data.action = 0;
    }

    switch (preheat_option) {
      case 0: // Load PLA
        #ifdef PREHEAT_1_TEMP_HOTEND
          e_temp = PREHEAT_1_TEMP_HOTEND;
        #endif
        break;
      case 1: // Load ABS
        TERN_(PREHEAT_2_TEMP_HOTEND, e_temp = PREHEAT_2_TEMP_HOTEND);
        break;
      case 2: // Load PET
        #ifdef PREHEAT_3_TEMP_HOTEND
          e_temp = PREHEAT_3_TEMP_HOTEND;
        #endif
        break;
      case 3: // Load FLEX
        #ifdef PREHEAT_4_TEMP_HOTEND
          e_temp = PREHEAT_4_TEMP_HOTEND;
        #endif
        break;
      case 9: // Cool down
      default:
        e_temp = 0;
        break;
    }

    if (filament_data.action == 0) { // Go back to utility screen
      #if HAS_HOTEND
        thermalManager.setTargetHotend(e_temp, ExtUI::extruder_t::E0);
        #if HAS_MULTI_HOTEND
          thermalManager.setTargetHotend(e_temp, ExtUI::extruder_t::E1);
        #endif
      #endif
      GotoScreen(DGUSLCD_SCREEN_UTILITY);
    }
    else { // Go to the preheat screen to show the heating progress
      switch (var.VP) {
        default: return;
          #if HAS_HOTEND
            case VP_E0_FILAMENT_LOAD_UNLOAD:
              filament_data.extruder = ExtUI::extruder_t::E0;
              thermalManager.setTargetHotend(e_temp, filament_data.extruder);
              break;
          #endif
          #if HAS_MULTI_HOTEND
            case VP_E1_FILAMENT_LOAD_UNLOAD:
              filament_data.extruder = ExtUI::extruder_t::E1;
              thermalManager.setTargetHotend(e_temp, filament_data.extruder);
              break;
          #endif
      }
      GotoScreen(DGUSLCD_SCREEN_FILAMENT_HEATING);
    }
  }

  void DGUSScreenHandler::HandleFilamentLoadUnload(DGUS_VP_Variable &var) {
    DEBUG_ECHOLNPGM("HandleFilamentLoadUnload");
    if (filament_data.action <= 0) return;

    // If we close to the target temperature, we can start load or unload the filament
    if (thermalManager.hotEnoughToExtrude(filament_data.extruder) && \
        thermalManager.targetHotEnoughToExtrude(filament_data.extruder)) {
      float movevalue = DGUS_FILAMENT_LOAD_LENGTH_PER_TIME;

      if (filament_data.action == 1) { // load filament
        if (!filament_data.heated) {
          //GotoScreen(DGUSLCD_SCREEN_FILAMENT_LOADING);
          filament_data.heated = true;
        }
        movevalue = ExtUI::getAxisPosition_mm(filament_data.extruder) + movevalue;
      }
      else { // unload filament
        if (!filament_data.heated) {
          GotoScreen(DGUSLCD_SCREEN_FILAMENT_UNLOADING);
          filament_data.heated = true;
        }
        // Before unloading extrude to prevent jamming
        if (filament_data.purge_length >= 0) {
          movevalue = ExtUI::getAxisPosition_mm(filament_data.extruder) + movevalue;
          filament_data.purge_length -= movevalue;
        }
        else {
          movevalue = ExtUI::getAxisPosition_mm(filament_data.extruder) - movevalue;
        }
      }
      ExtUI::setAxisPosition_mm(movevalue, filament_data.extruder);
    }
  }
#endif // DGUS_FILAMENT_LOADUNLOAD

bool DGUSScreenHandler::loop() {
  dgusdisplay.loop();

  const millis_t ms = millis();
  static millis_t next_event_ms = 0;

  if (!IsScreenComplete() || ELAPSED(ms, next_event_ms)) {
    next_event_ms = ms + DGUS_UPDATE_INTERVAL_MS;
    UpdateScreenVPData();
  }

  #if ENABLED(SHOW_BOOTSCREEN)
    static bool booted = false;

    if (!booted && TERN0(POWER_LOSS_RECOVERY, recovery.valid()))
      booted = true;

    if (!booted && ELAPSED(ms, BOOTSCREEN_TIMEOUT)) {
      booted = true;
      //GotoScreen(TERN0(POWER_LOSS_RECOVERY, recovery.valid()) ? DGUSLCD_SCREEN_POWER_LOSS : DGUSLCD_SCREEN_MAIN);
    }
  #endif

  return IsScreenComplete();
}

void DGUSScreenHandler::DGUSLCD_PowerOnResponse(DGUS_VP_Variable &var, void *val_ptr){
  uint8_t dat[2] = {0x00 ,0x01};
  dgusdisplay.WriteVariable(var.VP, dat, 2, false);
  DIWEN_SetText();
  DGUSLCD_SetIcon();
  delay(2000);
  dgusdisplay.RequestScreen(getCurrentScreen());
}

char DGUSScreenHandler::get_language_choice(void)
{
	return text_display.language_choice;
}

char *DGUSScreenHandler::get_language_choice_p(void)
{
	return &text_display.language_choice;
}

void DGUSScreenHandler::set_display_language(void)
{
	switch(text_display.language_choice)
	{
		case Chinese:
			text_display.L_Print = (char *)C_Print;
			text_display.L_Ctol = (char *)C_Ctol;
			text_display.L_Set = (char *)C_Set;
			text_display.L_Preheat = (char *)C_Preheat;
			text_display.L_Move = (char *)C_Move;
			text_display.L_Extrusion = (char *)C_Extrusion;
			text_display.L_Fan = (char *)C_Fan;
			text_display.L_About = (char *)C_About;
			text_display.L_Language = (char *)C_Language;
			text_display.L_Status = (char *)C_Status;
			text_display.L_PrintName = (char *)C_PrintName;
			text_display.L_Pause = (char *)C_Pause;
			text_display.L_Pursue = (char *)C_Pursue;
			text_display.L_Stop = (char *)C_Stop;
			text_display.L_Tempertuare = (char *)C_Tempertuare;
			text_display.L_Speed = (char *)C_Speed;
			text_display.L_PrintCtrl = (char *)C_PrintCtrl;
			text_display.L_PrintSpeed = (char *)C_PrintSpeed;
			text_display.L_SDPrint = (char *)C_SDPrint;
			text_display.L_Back = (char *)C_Back;
			text_display.L_PrintFinish = (char *)C_PrintFinish;
			text_display.L_Confirm = (char *)C_Confirm;
			text_display.L_LaserCtol = (char *)C_LaserCtol;
			text_display.L_LaserMove = (char *)C_LaserMove;
			text_display.L_Zero = (char *)C_Zero;
			text_display.L_Ctrl = (char *)C_Ctol;
			text_display.L_Cancel = (char *)C_Cancel;
			text_display.L_filament = (char *)C_Filament;
			text_display.L_StopPrint = (char *)C_StopPrint;
			text_display.L_Leveling = (char *)C_Leveling;
			text_display.L_Adjust = (char *)C_Adjust;
			text_display.L_Load = (char *)C_Load;
			text_display.L_Unload = (char *)C_Unload;
			text_display.L_Fast = (char *)C_Fast;
			text_display.L_Normal = (char *)C_Normal;
			text_display.L_Slow = (char *)C_Slow;
			text_display.L_autoleveling = (char *)C_autoleveling;
			text_display.L_offset = (char *)C_offset;
			text_display.L_Continue = (char *)C_Continue;
			text_display.L_3DPrintModeSelect = (char *)C_3DPrintModeSelect;
			text_display.L_3dPrintExtruder1 = (char *)C_3dPrintExtruder1;
			text_display.L_3dPrintExtruder2 = (char *)C_3dPrintExtruder2;
			text_display.L_3dPrintExtruder12 = (char *)C_3dPrintExtruder12;
			text_display.L_3dPrintHotEndReady = (char *)C_3dPrintHotEndReady;
			text_display.L_3dPrintHotEndTemperature = (char *)C_3dPrintHotEndTemperature; 
			text_display.L_3dPrintHotEndStat1 = (char *)C_3dPrintHotEndStat1; 
			text_display.L_3dPrintHotEndStat2 = (char *)C_3dPrintHotEndStat2;
			text_display.L_ZOffset = (char *)C_ZOffset;
			text_display.L_Homing = (char *)C_Homing;
			break;
		case English:
			text_display.L_Print = (char *)E_Print;
			text_display.L_Ctol = (char *)E_Ctol;
			text_display.L_Set = (char *)E_Set;
			text_display.L_Preheat = (char *)E_Preheat;
			text_display.L_Move = (char *)E_Move;
			text_display.L_Extrusion = (char *)E_Extrusion;
			text_display.L_Fan = (char *)E_Fan;
			text_display.L_About = (char *)E_About;
			text_display.L_Language = (char *)E_Language;
			text_display.L_Status = (char *)E_Status;
			text_display.L_PrintName = (char *)E_PrintName;
			text_display.L_Pause = (char *)E_Pause;
			text_display.L_Pursue = (char *)E_Pursue;
			text_display.L_Stop = (char *)E_Stop;
			text_display.L_Tempertuare = (char *)E_Tempertuare;
			text_display.L_Speed = (char *)E_Speed;
			text_display.L_PrintCtrl = (char *)E_PrintCtrl;
			text_display.L_PrintSpeed = (char *)E_PrintSpeed;
			text_display.L_SDPrint = (char *)E_SDPrint;
			text_display.L_Back = (char *)E_Back;
			text_display.L_PrintFinish = (char *)E_PrintFinish;
			text_display.L_Confirm = (char *)E_Confirm;
			text_display.L_LaserCtol = (char *)E_LaserCtol;
			text_display.L_LaserMove = (char *)E_LaserMove;
			text_display.L_Zero = (char *)E_Zero;
			text_display.L_Ctrl = (char *)E_Ctol;
			text_display.L_Cancel = (char *)E_Cancel;
			text_display.L_filament = (char *)E_Filament;
			text_display.L_StopPrint = (char *)E_StopPrint;
			text_display.L_Leveling = (char *)E_Leveling;
			text_display.L_Adjust = (char *)E_Adjust;
			text_display.L_Load = (char *)E_Load;
			text_display.L_Unload = (char *)E_Unload;
			text_display.L_Fast = (char *)E_Fast;
			text_display.L_Normal = (char *)E_Normal;
			text_display.L_Slow = (char *)E_Slow;
			text_display.L_autoleveling = (char *)E_autoleveling;
			text_display.L_offset = (char *)E_offset;
			text_display.L_Continue = (char *)E_Continue;
			text_display.L_3DPrintModeSelect = (char *)E_3DPrintModeSelect;
			text_display.L_3dPrintExtruder1 = (char *)E_3dPrintExtruder1;
			text_display.L_3dPrintExtruder2 = (char *)E_3dPrintExtruder2;
			text_display.L_3dPrintExtruder12 = (char *)E_3dPrintExtruder12;
			text_display.L_3dPrintHotEndReady = (char *)E_3dPrintHotEndReady;
			text_display.L_3dPrintHotEndTemperature = (char *)E_3dPrintHotEndTemperature;
			text_display.L_3dPrintHotEndStat1 = (char *)E_3dPrintHotEndStat1; 
			text_display.L_3dPrintHotEndStat2 = (char *)E_3dPrintHotEndStat2;
			text_display.L_ZOffset = (char *)E_ZOffset;
			text_display.L_Homing = (char *)E_Homing;
			break;
		case German:
			text_display.L_Print = (char *)G_Print;
			text_display.L_Ctol = (char *)G_Ctol;
			text_display.L_Set = (char *)G_Set;
			text_display.L_Preheat = (char *)G_Preheat;
			text_display.L_Move = (char *)G_Move;
			text_display.L_Extrusion = (char *)G_Extrusion;
			text_display.L_Fan = (char *)G_Fan;
			text_display.L_About = (char *)G_About;
			text_display.L_Language = (char *)G_Language;
			text_display.L_Status = (char *)G_Status;
			text_display.L_PrintName = (char *)G_PrintName;
			text_display.L_Pause = (char *)G_Pause;
			text_display.L_Pursue = (char *)G_Pursue;
			text_display.L_Stop = (char *)G_Stop;
			text_display.L_Tempertuare = (char *)G_Tempertuare;
			text_display.L_Speed = (char *)G_Speed;
			text_display.L_PrintCtrl = (char *)G_PrintCtrl;
			text_display.L_PrintSpeed = (char *)G_PrintSpeed;
			text_display.L_SDPrint = (char *)G_SDPrint;
			text_display.L_Back = (char *)G_Back;
			text_display.L_PrintFinish = (char *)G_PrintFinish;
			text_display.L_Confirm = (char *)E_Confirm;
			text_display.L_LaserCtol = (char *)G_LaserCtol;
			text_display.L_LaserMove = (char *)G_LaserMove;
			text_display.L_Zero = (char *)G_Zero;
			text_display.L_Ctrl = (char *)G_Ctol;
			text_display.L_Cancel = (char *)E_Cancel;
			text_display.L_filament = (char *)G_Filament;
			text_display.L_StopPrint = (char *)G_StopPrint;
			text_display.L_Leveling = (char *)G_Leveling;
			text_display.L_Adjust = (char *)G_Adjust;
			text_display.L_Load = (char *)G_Load;
			text_display.L_Unload = (char *)G_Unload;
			text_display.L_Fast = (char *)G_Fast;
			text_display.L_Normal = (char *)G_Normal;
			text_display.L_Slow = (char *)G_Slow;
			text_display.L_autoleveling = (char *)G_autoleveling;
			text_display.L_offset = (char *)G_offset;
			text_display.L_Continue = (char *)G_Continue;
			text_display.L_3DPrintModeSelect = (char *)G_3DPrintModeSelect;
			text_display.L_3dPrintExtruder1 = (char *)G_3dPrintExtruder1;
			text_display.L_3dPrintExtruder2 = (char *)G_3dPrintExtruder2;
			text_display.L_3dPrintExtruder12 = (char *)G_3dPrintExtruder12;
			text_display.L_3dPrintHotEndReady = (char *)E_3dPrintHotEndReady;
			text_display.L_3dPrintHotEndTemperature = (char *)E_3dPrintHotEndTemperature;
			text_display.L_3dPrintHotEndStat1 = (char *)E_3dPrintHotEndStat1; 
			text_display.L_3dPrintHotEndStat2 = (char *)E_3dPrintHotEndStat2;
			text_display.L_ZOffset = (char *)E_ZOffset;
			text_display.L_Homing = (char *)E_Homing;
			break;
		case French:
			text_display.L_Print = (char *)F_Print;
			text_display.L_Ctol = (char *)F_Ctol;
			text_display.L_Set = (char *)F_Set;
			text_display.L_Preheat = (char *)F_Preheat;
			text_display.L_Move = (char *)F_Move;
			text_display.L_Extrusion = (char *)F_Extrusion;
			text_display.L_Fan = (char *)F_Fan;
			text_display.L_About = (char *)F_About;
			text_display.L_Language = (char *)F_Language;
			text_display.L_Status = (char *)F_Status;
			text_display.L_PrintName = (char *)F_PrintName;
			text_display.L_Pause = (char *)F_Pause;
			text_display.L_Pursue = (char *)F_Pursue;
			text_display.L_Stop = (char *)F_Stop;
			text_display.L_Tempertuare = (char *)F_Tempertuare;
			text_display.L_Speed = (char *)F_Speed;
			text_display.L_PrintCtrl = (char *)F_PrintCtrl;
			text_display.L_PrintSpeed = (char *)F_PrintSpeed;
			text_display.L_SDPrint = (char *)F_SDPrint;
			text_display.L_Back = (char *)F_Back;
			text_display.L_PrintFinish = (char *)F_PrintFinish;
			text_display.L_Confirm = (char *)E_Confirm;
			text_display.L_LaserCtol = (char *)F_LaserCtol;
			text_display.L_LaserMove = (char *)F_LaserMove;
			text_display.L_Zero = (char *)F_Zero; 
			text_display.L_Ctrl = (char *)F_Ctol;
			text_display.L_Cancel = (char *)E_Cancel;
			text_display.L_filament = (char *)F_Filament;
			text_display.L_StopPrint = (char *)F_StopPrint;
			text_display.L_Leveling = (char *)F_Leveling;
			text_display.L_Adjust = (char *)F_Adjust;
			text_display.L_Load = (char *)F_Load;
			text_display.L_Unload = (char *)F_Unload;
			text_display.L_Fast = (char *)F_Fast;
			text_display.L_Normal = (char *)F_Normal;
			text_display.L_Slow = (char *)F_Slow;
			text_display.L_autoleveling = (char *)F_autoleveling;
			text_display.L_offset = (char *)F_offset;
			text_display.L_Continue = (char *)F_Continue;
			text_display.L_3DPrintModeSelect = (char *)F_3DPrintModeSelect;
			text_display.L_3dPrintExtruder1 = (char *)F_3dPrintExtruder1;
			text_display.L_3dPrintExtruder2 = (char *)F_3dPrintExtruder2;
			text_display.L_3dPrintExtruder12 = (char *)F_3dPrintExtruder12;
			text_display.L_3dPrintHotEndReady = (char *)E_3dPrintHotEndReady;
			text_display.L_3dPrintHotEndTemperature = (char *)E_3dPrintHotEndTemperature;
			text_display.L_3dPrintHotEndStat1 = (char *)E_3dPrintHotEndStat1; 
			text_display.L_3dPrintHotEndStat2 = (char *)E_3dPrintHotEndStat2;
			text_display.L_ZOffset = (char *)E_ZOffset;
			text_display.L_Homing = (char *)E_Homing;
			break;
		case Spanish:
			text_display.L_Print = (char *)S_Print;
			text_display.L_Ctol = (char *)S_Ctol;
			text_display.L_Set = (char *)S_Set;
			text_display.L_Preheat = (char *)S_Preheat;
			text_display.L_Move = (char *)S_Move;
			text_display.L_Extrusion = (char *)S_Extrusion;
			text_display.L_Fan = (char *)S_Fan;
			text_display.L_About = (char *)S_About;
			text_display.L_Language = (char *)S_Language;
			text_display.L_Status = (char *)S_Status;
			text_display.L_PrintName = (char *)S_PrintName;
			text_display.L_Pause = (char *)S_Pause;
			text_display.L_Pursue = (char *)S_Pursue;
			text_display.L_Stop = (char *)S_Stop;
			text_display.L_Tempertuare = (char *)S_Tempertuare;
			text_display.L_Speed = (char *)S_Speed;
			text_display.L_PrintCtrl = (char *)S_PrintCtrl;
			text_display.L_PrintSpeed = (char *)S_PrintSpeed;
			text_display.L_SDPrint = (char *)S_SDPrint;
			text_display.L_Back = (char *)S_Back;
			text_display.L_PrintFinish = (char *)S_PrintFinish;
			text_display.L_Confirm = (char *)E_Confirm;
			text_display.L_LaserCtol = (char *)S_LaserCtol;
			text_display.L_LaserMove = (char *)S_LaserMove;
			text_display.L_Zero = (char *)S_Zero;
			text_display.L_Ctrl = (char *)S_Ctol;
			text_display.L_Cancel = (char *)E_Cancel;
			text_display.L_filament = (char *)S_Filament;
			text_display.L_StopPrint = (char *)S_StopPrint;
			text_display.L_Leveling = (char *)S_Leveling;
			text_display.L_Adjust = (char *)S_Adjust;
			text_display.L_Load = (char *)S_Load;
			text_display.L_Unload = (char *)S_Unload;
			text_display.L_Fast = (char *)S_Fast;
			text_display.L_Normal = (char *)S_Normal;
			text_display.L_Slow = (char *)S_Slow;
			text_display.L_autoleveling = (char *)S_autoleveling;
			text_display.L_offset = (char *)S_offset;
			text_display.L_Continue = (char *)S_Continue;
			text_display.L_3DPrintModeSelect = (char *)S_3DPrintModeSelect;
			text_display.L_3dPrintExtruder1 = (char *)S_3dPrintExtruder1;
			text_display.L_3dPrintExtruder2 = (char *)S_3dPrintExtruder2;
			text_display.L_3dPrintExtruder12 = (char *)S_3dPrintExtruder12;
			text_display.L_3dPrintHotEndReady = (char *)E_3dPrintHotEndReady;
			text_display.L_3dPrintHotEndTemperature = (char *)E_3dPrintHotEndTemperature;
			text_display.L_3dPrintHotEndStat1 = (char *)E_3dPrintHotEndStat1; 
			text_display.L_3dPrintHotEndStat2 = (char *)E_3dPrintHotEndStat2;
			text_display.L_ZOffset = (char *)E_ZOffset;
			text_display.L_Homing = (char *)E_Homing;	
			break;
		case Portuguese:
			text_display.L_Print = (char *)P_Print;
			text_display.L_Ctol = (char *)P_Ctol;
			text_display.L_Set = (char *)P_Set;
			text_display.L_Preheat = (char *)P_Preheat;
			text_display.L_Move = (char *)P_Move;
			text_display.L_Extrusion = (char *)P_Extrusion;
			text_display.L_Fan = (char *)P_Fan;
			text_display.L_About = (char *)P_About;
			text_display.L_Language = (char *)P_Language;
			text_display.L_Status = (char *)P_Status;
			text_display.L_PrintName = (char *)P_PrintName;
			text_display.L_Pause = (char *)P_Pause;
			text_display.L_Pursue = (char *)P_Pursue;
			text_display.L_Stop = (char *)P_Stop;
			text_display.L_Tempertuare = (char *)P_Tempertuare;
			text_display.L_Speed = (char *)P_Speed;
			text_display.L_PrintCtrl = (char *)P_PrintCtrl;
			text_display.L_PrintSpeed = (char *)P_PrintSpeed;
			text_display.L_SDPrint = (char *)P_SDPrint;
			text_display.L_Back = (char *)P_Back;
			text_display.L_PrintFinish = (char *)P_PrintFinish;
			text_display.L_Confirm = (char *)E_Confirm;
			text_display.L_LaserCtol = (char *)P_LaserCtol;
			text_display.L_LaserMove = (char *)P_LaserMove;
			text_display.L_Zero = (char *)P_Zero;
			text_display.L_Ctrl = (char *)P_Ctol;
			text_display.L_Cancel = (char *)E_Cancel;
			text_display.L_filament = (char *)P_Filament;
			text_display.L_StopPrint = (char *)P_StopPrint;
			text_display.L_Leveling = (char *)P_Leveling;
			text_display.L_Adjust = (char *)P_Adjust;
			text_display.L_Load = (char *)P_Load;
			text_display.L_Unload = (char *)P_Unload;
			text_display.L_Fast = (char *)P_Fast;
			text_display.L_Normal = (char *)P_Normal;
			text_display.L_Slow = (char *)P_Slow;
			text_display.L_autoleveling = (char *)P_autoleveling;
			text_display.L_offset = (char *)P_offset;
			text_display.L_Continue = (char *)P_Continue;
			text_display.L_3DPrintModeSelect = (char *)P_3DPrintModeSelect;
			text_display.L_3dPrintExtruder1 = (char *)P_3dPrintExtruder1;
			text_display.L_3dPrintExtruder2 = (char *)P_3dPrintExtruder2;
			text_display.L_3dPrintExtruder12 = (char *)P_3dPrintExtruder12;
			text_display.L_3dPrintHotEndReady = (char *)E_3dPrintHotEndReady;
			text_display.L_3dPrintHotEndTemperature = (char *)E_3dPrintHotEndTemperature;
			text_display.L_3dPrintHotEndStat1 = (char *)E_3dPrintHotEndStat1; 
			text_display.L_3dPrintHotEndStat2 = (char *)E_3dPrintHotEndStat2;
			text_display.L_ZOffset = (char *)E_ZOffset;
			text_display.L_Homing = (char *)E_Homing;
			break;
		case Japanese:
			text_display.L_Print = (char *)J_Print;
			text_display.L_Ctol = (char *)J_Ctol;
			text_display.L_Set = (char *)J_Set;
			text_display.L_Preheat = (char *)J_Preheat;
			text_display.L_Move = (char *)J_Move;
			text_display.L_Extrusion = (char *)J_Extrusion;
			text_display.L_Fan = (char *)J_Fan;
			text_display.L_About = (char *)J_About;
			text_display.L_Language = (char *)J_Language;
			text_display.L_Status = (char *)J_Status;
			text_display.L_PrintName = (char *)J_PrintName;
			text_display.L_Pause = (char *)J_Pause;
			text_display.L_Pursue = (char *)J_Pursue;
			text_display.L_Stop = (char *)J_Stop;
			text_display.L_Tempertuare = (char *)J_Tempertuare;
			text_display.L_Speed = (char *)J_Speed;
			text_display.L_PrintCtrl = (char *)J_PrintCtrl;
			text_display.L_PrintSpeed = (char *)J_PrintSpeed;
			text_display.L_SDPrint = (char *)J_SDPrint;
			text_display.L_Back = (char *)J_Back;
			text_display.L_PrintFinish = (char *)J_PrintFinish;
			text_display.L_Confirm = (char *)E_Confirm;
			text_display.L_LaserCtol = (char *)J_LaserCtol;
			text_display.L_LaserMove = (char *)J_LaserMove;
			text_display.L_Zero = (char *)J_Zero;
			text_display.L_Ctrl = (char *)J_Ctol;
			text_display.L_Cancel = (char *)E_Cancel;
			text_display.L_filament = (char *)J_Filament;
			text_display.L_StopPrint = (char *)J_StopPrint;
			text_display.L_Leveling = (char *)J_Leveling;
			text_display.L_Adjust = (char *)J_Adjust;
			text_display.L_Load = (char *)J_Load;
			text_display.L_Unload = (char *)J_Unload;
			text_display.L_Fast = (char *)J_Fast;
			text_display.L_Normal = (char *)J_Normal;
			text_display.L_Slow = (char *)J_Slow;
			text_display.L_autoleveling = (char *)J_autoleveling;
			text_display.L_offset = (char *)J_offset;
			text_display.L_Continue = (char *)J_Continue;
			text_display.L_3DPrintModeSelect = (char *)J_3DPrintModeSelect;
			text_display.L_3dPrintExtruder1 = (char *)J_3dPrintExtruder1;
			text_display.L_3dPrintExtruder2 = (char *)J_3dPrintExtruder2;
			text_display.L_3dPrintExtruder12 = (char *)J_3dPrintExtruder12;
			text_display.L_3dPrintHotEndReady = (char *)E_3dPrintHotEndReady;
			text_display.L_3dPrintHotEndTemperature = (char *)E_3dPrintHotEndTemperature;
			text_display.L_3dPrintHotEndStat1 = (char *)E_3dPrintHotEndStat1; 
			text_display.L_3dPrintHotEndStat2 = (char *)E_3dPrintHotEndStat2;
			text_display.L_ZOffset = (char *)E_ZOffset;
			text_display.L_Homing = (char *)E_Homing;
			break;
		case Russian:
			text_display.L_Print = (char *)R_Print;
			text_display.L_Ctol = (char *)R_Ctol;
			text_display.L_Set = (char *)R_Set;
			text_display.L_Preheat = (char *)R_Preheat;
			text_display.L_Move = (char *)R_Move;
			text_display.L_Extrusion = (char *)R_Extrusion;
			text_display.L_Fan = (char *)R_Fan;
			text_display.L_About = (char *)R_About;
			text_display.L_Language = (char *)R_Language;
			text_display.L_Status = (char *)R_Status;
			text_display.L_PrintName = (char *)R_PrintName;
			text_display.L_Pause = (char *)R_Pause;
			text_display.L_Pursue = (char *)R_Pursue;
			text_display.L_Stop = (char *)R_Stop;
			text_display.L_Tempertuare = (char *)R_Tempertuare;
			text_display.L_Speed = (char *)R_Speed;
			text_display.L_PrintCtrl = (char *)R_PrintCtrl;
			text_display.L_PrintSpeed = (char *)R_PrintSpeed;
			text_display.L_SDPrint = (char *)R_SDPrint;
			text_display.L_Back = (char *)R_Back;
			text_display.L_PrintFinish = (char *)R_PrintFinish;
			text_display.L_Confirm = (char *)E_Confirm;
			text_display.L_LaserCtol = (char *)R_LaserCtol;
			text_display.L_LaserMove = (char *)R_LaserMove;
			text_display.L_Zero = (char *)R_Zero;
			text_display.L_Ctrl = (char *)R_Ctol;
			text_display.L_Cancel = (char *)E_Cancel;
			text_display.L_filament = (char *)R_Filament;
			text_display.L_Leveling = (char *)E_Leveling;
			text_display.L_Fast = (char *)E_Fast;
			text_display.L_Normal = (char *)E_Normal;
			text_display.L_Slow = (char *)E_Slow;
			text_display.L_autoleveling = (char *)E_autoleveling;
			text_display.L_offset = (char *)E_offset;
			text_display.L_Continue = (char *)E_Continue;
			text_display.L_3DPrintModeSelect = (char *)R_3DPrintModeSelect;
			text_display.L_3dPrintExtruder1 = (char *)R_3dPrintExtruder1;
			text_display.L_3dPrintExtruder2 = (char *)R_3dPrintExtruder2;
			text_display.L_3dPrintExtruder12 = (char *)R_3dPrintExtruder12;
			text_display.L_3dPrintHotEndReady = (char *)E_3dPrintHotEndReady;
			text_display.L_3dPrintHotEndTemperature = (char *)E_3dPrintHotEndTemperature;
			text_display.L_3dPrintHotEndStat1 = (char *)E_3dPrintHotEndStat1; 
			text_display.L_3dPrintHotEndStat2 = (char *)E_3dPrintHotEndStat2;
			text_display.L_ZOffset = (char *)E_ZOffset;
			text_display.L_Homing = (char *)E_Homing;
			break;
		case Italian:
			text_display.L_Print = (char *)I_Print;
			text_display.L_Ctol = (char *)I_Ctol;
			text_display.L_Set = (char *)I_Set;
			text_display.L_Preheat = (char *)I_Preheat;
			text_display.L_Move = (char *)I_Move;
			text_display.L_Extrusion = (char *)I_Extrusion;
			text_display.L_Fan = (char *)I_Fan;
			text_display.L_About = (char *)I_About;
			text_display.L_Language = (char *)I_Language;
			text_display.L_Status = (char *)I_Status;
			text_display.L_PrintName = (char *)I_PrintName;
			text_display.L_Pause = (char *)I_Pause;
			text_display.L_Pursue = (char *)I_Pursue;
			text_display.L_Stop = (char *)I_Stop;
			text_display.L_Tempertuare = (char *)I_Tempertuare;
			text_display.L_Speed = (char *)I_Speed;
			text_display.L_PrintCtrl = (char *)I_PrintCtrl;
			text_display.L_PrintSpeed = (char *)I_PrintSpeed;
			text_display.L_SDPrint = (char *)I_SDPrint;
			text_display.L_Back = (char *)I_Back;
			text_display.L_PrintFinish = (char *)I_PrintFinish;
			text_display.L_Confirm = (char *)E_Confirm;
			text_display.L_LaserCtol = (char *)I_LaserCtol;
			text_display.L_LaserMove = (char *)I_LaserMove;
			text_display.L_Zero = (char *)I_Zero;
			text_display.L_Ctrl = (char *)I_Ctol;
			text_display.L_Cancel = (char *)E_Cancel;
			text_display.L_filament = (char *)I_Filament;
			text_display.L_StopPrint = (char *)I_StopPrint;
			text_display.L_Leveling = (char *)I_Leveling;
			text_display.L_Adjust = (char *)I_Adjust;
			text_display.L_Load = (char *)I_Load;
			text_display.L_Unload = (char *)I_Unload;
			text_display.L_Fast = (char *)I_Fast;
			text_display.L_Normal = (char *)I_Normal;
			text_display.L_Slow = (char *)I_Slow;
			text_display.L_autoleveling = (char *)I_autoleveling;
			text_display.L_offset = (char *)I_offset;
			text_display.L_Continue = (char *)I_Continue;
			text_display.L_3DPrintModeSelect = (char *)I_3DPrintModeSelect;
			text_display.L_3dPrintExtruder1 = (char *)I_3dPrintExtruder1;
			text_display.L_3dPrintExtruder2 = (char *)I_3dPrintExtruder2;
			text_display.L_3dPrintExtruder12 = (char *)I_3dPrintExtruder12;
			text_display.L_3dPrintHotEndReady = (char *)E_3dPrintHotEndReady;
			text_display.L_3dPrintHotEndTemperature = (char *)E_3dPrintHotEndTemperature;
			text_display.L_3dPrintHotEndStat1 = (char *)E_3dPrintHotEndStat1; 
			text_display.L_3dPrintHotEndStat2 = (char *)E_3dPrintHotEndStat2;
			text_display.L_ZOffset = (char *)E_ZOffset;
			text_display.L_Homing = (char *)E_Homing;
			break;
		default:
			break;
	}
}

uint8_t enc_get_utf8_size(uint8_t* utf8)
{
	uint8_t temp = utf8[0];
	uint8_t res=0;
	uint8_t i;
	while(res < 6)	
	{
		if(temp&0x80)
		{
			res++;
			temp = temp<<1;
		}
		else
			break;
	}
	i=1;
	while(i<res)		
	{
		temp = utf8[i] ^ 0x80;
		temp = temp>>6;
		if(temp == 0)
			i++;
		else
			break;
	}
	if(i==res)
		return res;
	else if(res==0)
		return 1;
		
	else
		return 0xff;
}

int enc_utf8_to_unicode_one(const uint8_t* pInput, uint16_t* Unic,uint8_t utfbytes)
{
  
    char b1, b2, b3, b4, b5, b6;
  
    *Unic = 0x0; 
    unsigned char *pOutput = (unsigned char *) Unic;
  
    switch ( utfbytes )
    {
        case 1:
			*(pOutput+1) = *pInput;
            *pOutput     = 0x00;
            //utfbytes    += 1;
            break;
        case 2:
            b1 = *pInput;
            b2 = *(pInput + 1);
            if ( (b2 & 0xC0) != 0x80 )
                return 0;
            *(pOutput+1)     = (b1 << 6) + (b2 & 0x3F);
            *pOutput = (b1 >> 2) & 0x07;
            break;
        case 3:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80) )
                return 0;
            *(pOutput+1)     = (b2 << 6) + (b3 & 0x3F);
            *pOutput = (b1 << 4) + ((b2 >> 2) & 0x0F);
            break;
        case 4:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            b4 = *(pInput + 3);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
                    || ((b4 & 0xC0) != 0x80) )
                return 0;
            *(pOutput+2)     = (b3 << 6) + (b4 & 0x3F);
            *(pOutput+1) = (b2 << 4) + ((b3 >> 2) & 0x0F);
            *pOutput = ((b1 << 2) & 0x1C)  + ((b2 >> 4) & 0x03);
            break;
        case 5:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            b4 = *(pInput + 3);
            b5 = *(pInput + 4);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
                    || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80) )
                return 0;
            *(pOutput+3)     = (b4 << 6) + (b5 & 0x3F);
            *(pOutput+2) = (b3 << 4) + ((b4 >> 2) & 0x0F);
            *(pOutput+1) = (b2 << 2) + ((b3 >> 4) & 0x03);
            *pOutput = (b1 << 6);
            break;
        case 6:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            b4 = *(pInput + 3);
            b5 = *(pInput + 4);
            b6 = *(pInput + 5);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
                    || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80)
                    || ((b6 & 0xC0) != 0x80) )
                return 0;
            *(pOutput+3)     = (b5 << 6) + (b6 & 0x3F);
            *(pOutput+2) = (b5 << 4) + ((b6 >> 2) & 0x0F);
            *(pOutput+1) = (b3 << 2) + ((b4 >> 4) & 0x03);
            *pOutput = ((b1 << 6) & 0x40) + (b2 & 0x3F);
            break;
        default:
            return 0;
    }
  
    return utfbytes;
}

int enc_utf8_to_unicode(const uint8_t* pInput, uint8_t* Unic)
{
	uint8_t i;
	uint8_t len, num=0;
	i = 0;
	while(pInput[i] != '\0')
	{
		len = enc_get_utf8_size((uint8_t *)&pInput[i]);
		if(len == 0xff)
			return -1;
		enc_utf8_to_unicode_one((const uint8_t *)&pInput[i],(uint16_t *)&Unic[num * 2],len);
		i += len;
		num++;
	}
	return num;
}

void DGUSScreenHandler::DGUSLCD_TextDisplay(uint16_t adr, const void *values, uint8_t valueslen)
{
  uint8_t txbuf[128] = {0};
  uint8_t WordNum;
  WordNum = enc_utf8_to_unicode((uint8_t *)values, txbuf);
  txbuf[WordNum * 2] = 0xFF;
	txbuf[WordNum * 2 + 1] = 0xFF;
  dgusdisplay.WriteVariable(adr, txbuf, WordNum * 2 + 2, false);
}

void DGUSScreenHandler::DGUSLCD_TextDisplayPGM(uint16_t adr, const void *values, uint8_t valueslen)
{
  uint8_t txbuf[128] = {0};
  uint8_t WordNum;
  WordNum = enc_utf8_to_unicode((uint8_t *)values, txbuf);
  txbuf[WordNum * 2] = 0xFF;
	txbuf[WordNum * 2 + 1] = 0xFF;
  dgusdisplay.WriteVariablePGM(adr, txbuf, WordNum * 2 + 2, false);
}

void DGUSScreenHandler::DGUSLCD_IconDisplay(uint16_t adr, uint16_t value)
{
  dgusdisplay.WriteVariable(adr, value);
}

void DGUSScreenHandler::DGUSLCD_SetIcon(void)
{
	DGUSLCD_IconDisplay(0x1080, printmode.get_printmode());
	//DGUSLCD_IconDisplay(0x120a, LevelMode);
	DGUSLCD_IconDisplay(ICON_5BMP_A, 0);
	DGUSLCD_IconDisplay(ICON_5BMP_C, 1);
	DGUSLCD_IconDisplay(ICON_5BMP_E, 1);
	
	DGUSLCD_IconDisplay(ICON_6BMP_A, 1);
	DGUSLCD_IconDisplay(ICON_6BMP_C, 1);
	DGUSLCD_IconDisplay(ICON_28BMP_A, 1);
	DGUSLCD_IconDisplay(ICON_19BMP_A, 1);
	DGUSLCD_IconDisplay(ICON_29BMP_A, 1);
	DGUSLCD_IconDisplay(ICON_30BMP_A, 1);
	DGUSLCD_IconDisplay(ICON_30BMP_C, 1);
	DGUSLCD_IconDisplay(ICON_30BMP_F, 1);
	DGUSLCD_IconDisplay(ICON_11BMP_A, 0);
	DGUSLCD_IconDisplay(ICON_11BMP_B, 1);
	DGUSLCD_IconDisplay(ICON_11BMP_C, 0);
	DGUSLCD_IconDisplay(ICON_11BMP_D, 1);
	DGUSLCD_IconDisplay(ICON_45BMP_A, 1);
	DGUSLCD_IconDisplay(0x1c01, text_display.language_choice);
}

void DGUSScreenHandler::DIWEN_SetText(void)
{
	uint8_t i=0;
	uint16_t addr,value;
	char temp[40];

	set_display_language();
	switch(text_display.language_choice)
	{
		case Chinese:
		case Japanese:
			for(i=0;i<3;i++)
			{
				addr = DP_START_TEXT + i*DP_LENTH_TEXT;
				value = (uint16_t)0x1700;												// The location of the font is at 23
				dgusdisplay.WriteVariable(addr + 0x09, (uint8_t *)(&value), 2, false);	// set the location of the font
				value = (uint16_t)0x1010;												// fontsize 16x16
				dgusdisplay.WriteVariable(addr + 0x0a, (uint8_t* )(&value), 2, false);	// set the size of font
			}
		break;
		case English:
		case French:
		case German:
		case Spanish:
		case Portuguese:
		case Italian:
			for(i=0;i<3;i++)
			{
				addr = DP_START_TEXT + i*DP_LENTH_TEXT;
				value = (uint8_t)0x1f00;												// The location of the font is at 31
				dgusdisplay.WriteVariable(addr + 0x09, (uint8_t *)(&value), 2, false);	// set the location of the font
				value = (uint8_t)0x1008;												// fontsize 16x8
				dgusdisplay.WriteVariable(addr + 0x0a, (uint8_t *)(&value), 2, false);	// set the size of font
			}
		break;
		default:
		break;
	}

	// printmode 
	if(IS_PRINT3D_PRINTMODE()){
		if(text_display.language_choice == 0){
			sprintf(temp, "3D打印");
		} else {
			sprintf(temp, "3D Print");
		}
	} else if(IS_LASER_PRINTMODE()) {
		if(text_display.language_choice == 0){
			sprintf(temp, "激光雕刻");
		} else {
			sprintf(temp, "Laser Carve");
		}
	}
	DGUSLCD_TextDisplay(0x1070, temp, strlen(temp));

  	// STATE page Current mode 
	if(IS_PRINT3D_PRINTMODE()){
		if(text_display.language_choice == 0){
			sprintf(temp, "当前模式： 3D打印");
		} else {
			sprintf(temp, "Current Mode: 3D Print");
		}
	} else if(IS_LASER_PRINTMODE()) {
		if(text_display.language_choice == 0){
			sprintf(temp, "当前模式： 激光雕刻");
		} else {
			sprintf(temp, "Current Mode：Laser Carve");
		}
	}
  	DGUSLCD_TextDisplay(0x1db3, temp, strlen(temp));

	DGUSLCD_TextDisplay(0x1010, text_display.L_Print, strlen(text_display.L_Print));
	delay(100);
	DGUSLCD_TextDisplay(0x1030, text_display.L_Ctol, strlen(text_display.L_Ctol));
	delay(100);
	DGUSLCD_TextDisplay(0x1050, text_display.L_Set, strlen(text_display.L_Set));
	// PRINT page
	DGUSLCD_TextDisplay(0x1110, text_display.L_Back, strlen(text_display.L_Back));
	DGUSLCD_TextDisplay(0x1120, text_display.L_Print, strlen(text_display.L_Print));
	// CTRL page
	DGUSLCD_TextDisplay(0x1210, text_display.L_Back, strlen(text_display.L_Back));
	DGUSLCD_TextDisplay(0x1220, text_display.L_Ctol, strlen(text_display.L_Ctol));
	DGUSLCD_TextDisplay(0x1230, text_display.L_Preheat, strlen(text_display.L_Preheat));
	DGUSLCD_TextDisplay(0x1240, text_display.L_Move, strlen(text_display.L_Move));
	DGUSLCD_TextDisplay(0x1250, text_display.L_Zero, strlen(text_display.L_Zero));
	DGUSLCD_TextDisplay(0x1260, text_display.L_Extrusion, strlen(text_display.L_Extrusion));
#if 0 
	if(LevelMode==0)
		DGUSLCD_TextDisplay(0x1270, text_display.L_Leveling, strlen(text_display.L_Leveling));
	else
		DGUSLCD_TextDisplay(0x1270, text_display.L_autoleveling, strlen(text_display.L_autoleveling));
#else
  DGUSLCD_TextDisplay(0x1270, text_display.L_autoleveling, strlen(text_display.L_autoleveling));
#endif
	DGUSLCD_TextDisplay(0x1280, text_display.L_Fan, strlen(text_display.L_Fan));
	DGUSLCD_TextDisplay(0x1290, text_display.L_Stop, strlen(text_display.L_Stop));
	// SETTING page
	DGUSLCD_TextDisplay(0x1310, text_display.L_Back, strlen(text_display.L_Back));
	DGUSLCD_TextDisplay(0x1320, text_display.L_Set, strlen(text_display.L_Set));
	DGUSLCD_TextDisplay(0x1330, text_display.L_About, strlen(text_display.L_About));
	DGUSLCD_TextDisplay(0x1340, text_display.L_Language, strlen(text_display.L_Language));
	DGUSLCD_TextDisplay(0x1350, text_display.L_Status, strlen(text_display.L_Status));
	DGUSLCD_TextDisplay(0x1360, text_display.L_Continue, strlen(text_display.L_Continue));
	// PREHEAT page
	DGUSLCD_TextDisplay(0x1410, text_display.L_Back, strlen(text_display.L_Back));
	DGUSLCD_TextDisplay(0x1420, text_display.L_Preheat, strlen(text_display.L_Preheat));
	// 3D MOVE page
	DGUSLCD_TextDisplay(0x1510, text_display.L_Back, strlen(text_display.L_Back));
	DGUSLCD_TextDisplay(0x1520, text_display.L_Move, strlen(text_display.L_Move));
	// ZERO page
	DGUSLCD_TextDisplay(0x1610, text_display.L_Back, strlen(text_display.L_Back));
	DGUSLCD_TextDisplay(0x1620, text_display.L_Zero, strlen(text_display.L_Zero));
	// EXTRUDER page
	DGUSLCD_TextDisplay(0x1710, text_display.L_Back, strlen(text_display.L_Back));
	DGUSLCD_TextDisplay(0x1720, text_display.L_Extrusion, strlen(text_display.L_Extrusion));
	DGUSLCD_TextDisplay(0x1750, text_display.L_Slow, strlen(text_display.L_Slow));
	DGUSLCD_TextDisplay(0x1760, text_display.L_Normal, strlen(text_display.L_Normal));
	DGUSLCD_TextDisplay(0x1770, text_display.L_Fast, strlen(text_display.L_Fast));
	DGUSLCD_TextDisplay(0x1780, text_display.L_Load, strlen(text_display.L_Load));
	DGUSLCD_TextDisplay(0x1790, text_display.L_Unload, strlen(text_display.L_Unload));
	// MNAUAL LEVELING page
	DGUSLCD_TextDisplay(0x1810, text_display.L_Back, strlen(text_display.L_Back));
	DGUSLCD_TextDisplay(0x1820, text_display.L_Leveling, strlen(text_display.L_Leveling));
	// AUTO LEVELING page
	DGUSLCD_TextDisplay(0x1910, text_display.L_Back, strlen(text_display.L_Back));
	DGUSLCD_TextDisplay(0x1920, text_display.L_Leveling, strlen(text_display.L_Leveling));
	// FAN page
	DGUSLCD_TextDisplay(0x1a10, text_display.L_Back, strlen(text_display.L_Back));
	DGUSLCD_TextDisplay(0x1a20, text_display.L_Fan, strlen(text_display.L_Fan));
	// ABOUT page
	DGUSLCD_TextDisplay(0x1b10, text_display.L_Back, strlen(text_display.L_Back));
	DGUSLCD_TextDisplay(0x1b20, text_display.L_About, strlen(text_display.L_About));
	DGUSLCD_TextDisplay(0x1b30, COM_PILE_DATE, sizeof(COM_PILE_DATE));
	DGUSLCD_TextDisplay(0x1b50, SW_VERSION, sizeof(SW_VERSION));
	DGUSLCD_TextDisplay(0x1b70, CO_TD_INFO, sizeof(CO_TD_INFO));
	// LANGUEGA SECELT page
	DGUSLCD_TextDisplay(0x1c10, text_display.L_Back, strlen(text_display.L_Back));
	DGUSLCD_TextDisplay(0x1c20, text_display.L_Language, strlen(text_display.L_Language));
	// STATE page
	DGUSLCD_TextDisplay(0x1d10, text_display.L_Back, strlen(text_display.L_Back));
	DGUSLCD_TextDisplay(0x1d20, text_display.L_Status, strlen(text_display.L_Status));
	// 3D PRINT page
	DGUSLCD_TextDisplay(0x1ea0, text_display.L_Ctol, strlen(text_display.L_Ctol));
	DGUSLCD_TextDisplay(0x1ec0, text_display.L_Stop, strlen(text_display.L_Stop));
	DGUSLCD_TextDisplay(VP_PRINT_PAUSE_TEXT, text_display.L_Pursue, strlen(text_display.L_Pursue));
	DGUSLCD_TextDisplay(VP_PRINT_STOP_TEXT, text_display.L_StopPrint, strlen(text_display.L_StopPrint));
	// PRINT CTRL page
	DGUSLCD_TextDisplay(0x1f10, text_display.L_Back, strlen(text_display.L_Back));
	DGUSLCD_TextDisplay(0x1f20, text_display.L_PrintCtrl, strlen(text_display.L_PrintCtrl));
	DGUSLCD_TextDisplay(0x1fb0, text_display.L_Tempertuare, strlen(text_display.L_Tempertuare));	
	DGUSLCD_TextDisplay(0x1fc0, text_display.L_Fan, strlen(text_display.L_Fan));
	DGUSLCD_TextDisplay(0x1fd0, text_display.L_Speed, strlen(text_display.L_Speed));
	DGUSLCD_TextDisplay(0x1fe0, text_display.L_Extrusion, strlen(text_display.L_Extrusion));
	DGUSLCD_TextDisplay(0x1ff0, text_display.L_ZOffset, strlen(text_display.L_ZOffset));
	// LASER PRINT page
	//DGUSLCD_TextDisplay(0x2080, text_display.L_Stop, strlen(text_display.L_Language));
	DGUSLCD_TextDisplay(VP_PRINT_LASER_PAUSE_TEXT, text_display.L_Pursue, strlen(text_display.L_Pursue));
	// LSAER CTRL page
	DGUSLCD_TextDisplay(0x2110, text_display.L_Back, strlen(text_display.L_Back));
	DGUSLCD_TextDisplay(0x2120, text_display.L_LaserMove, strlen(text_display.L_LaserMove));
	// PRINT CTRL FAN page
	DGUSLCD_TextDisplay(0x2210, text_display.L_Back, strlen(text_display.L_Back));
	DGUSLCD_TextDisplay(0x2220, text_display.L_Fan, strlen(text_display.L_Fan));
	// SELECT page
	DGUSLCD_TextDisplay(0x2310, text_display.L_Cancel, strlen(text_display.L_Cancel));
	DGUSLCD_TextDisplay(0x2320, text_display.L_Confirm, strlen(text_display.L_Confirm));
	// PRINT STOP page
	DGUSLCD_TextDisplay(0x2410, text_display.L_Cancel, strlen(text_display.L_Cancel));
	DGUSLCD_TextDisplay(0x2420, text_display.L_Confirm, strlen(text_display.L_Confirm));
	// FILAMENT RUNOUT page
	DGUSLCD_TextDisplay(0x2510, text_display.L_Confirm, strlen(text_display.L_Confirm));
	DGUSLCD_TextDisplay(0x2520, text_display.L_filament, strlen(text_display.L_filament));
	// PRINT SPEED page
	DGUSLCD_TextDisplay(0x2210, text_display.L_Back, strlen(text_display.L_Back));
	DGUSLCD_TextDisplay(0x2220, text_display.L_Speed, strlen(text_display.L_Speed));
	// EXTRUDER page
	DGUSLCD_TextDisplay(0x2610, text_display.L_Back, strlen(text_display.L_Back));
	DGUSLCD_TextDisplay(0x2620, text_display.L_Extrusion, strlen(text_display.L_Extrusion));
	// PRINT COMPLETE page
	DGUSLCD_TextDisplay(VP_PRINT_COMPLETE_TEXT, text_display.L_PrintFinish, strlen(text_display.L_PrintFinish));
	DGUSLCD_TextDisplay(0x2710,"Print Time:", sizeof("Print Time:"));
	if(text_display.language_choice == 0)
	{
		DGUSLCD_TextDisplay(0x2710,"打印时间:", sizeof("打印时间:"));
	}
	else
	{
		DGUSLCD_TextDisplay(0x2710,"Print Time:", sizeof("打印时间:"));
	}
	DGUSLCD_TextDisplay(0x2810, text_display.L_3DPrintModeSelect, strlen(text_display.L_3DPrintModeSelect));
	DGUSLCD_TextDisplay(0x2830, text_display.L_3dPrintExtruder1, strlen(text_display.L_3dPrintExtruder1));
	DGUSLCD_TextDisplay(0x2860, text_display.L_3dPrintExtruder2, strlen(text_display.L_3dPrintExtruder2));
	DGUSLCD_TextDisplay(0x2890, text_display.L_3dPrintExtruder12, strlen(text_display.L_3dPrintExtruder12));
	DGUSLCD_TextDisplay(0x2A70, text_display.L_3dPrintHotEndReady, strlen(text_display.L_3dPrintHotEndReady));

	DGUSLCD_TextDisplay(0x2b10, text_display.L_Back, strlen(text_display.L_Back));
	DGUSLCD_TextDisplay(0x2b20, text_display.L_ZOffset, strlen(text_display.L_ZOffset));
	DGUSLCD_TextDisplay(VP_FILEPATH_TEXT, "", strlen(""));
	DGUSLCD_TextDisplay(VP_HOMING_TEXT, text_display.L_Homing, strlen(text_display.L_Homing));
}

void DGUSScreenHandler::init(){
  uint8_t powerOn[4] = {0x55,0xaa,0x5a,0xa5};
  dgusdisplay.WriteVariable(VP_Reset, powerOn, sizeof(powerOn), false);
  text_display.language_choice = Chinese;
}

void DGUSScreenHandler::SDCardPrintFinished()
{
	char filename[FILENAME_LENGTH];

	strncpy(filename, card.filename, FILENAME_LENGTH);
	//  change all auto file name  to AUTO0.G.
	//  eg:AUTO#.g --> AUTO0.g.
	filename[4] = '0';
	//  Avoid the printing completion page caused by running the auto. g file when power on.
	if(strncmp(filename, "AUTO0.G", FILENAME_LENGTH)){
		GotoScreen(LOTMAXXLCD_SCREEN_PRINT_COMPLTED);
	}
}

#endif
