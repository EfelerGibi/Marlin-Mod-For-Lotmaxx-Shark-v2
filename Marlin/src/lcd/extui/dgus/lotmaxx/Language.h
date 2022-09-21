/*
*Function:
*Programed by:Ray_DK@163.com
*Modified by:Efeler_Gibi
*Complete date:
*Modified by:
*Modified date:
*Remarks:
*/
#ifndef __APP_LANGUAGE_H_
#define __APP_LANGUAGE_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "stm32f4xx.h"
#include "stdio.h"

#define COM_PILE_DATE							__DATE__     
#define COM_PILE_TIME							__TIME__   
#define SW_VERSION								"SC-10Shark Gibi Mod" 
#define CO_TD_INFO								"Shenzhen Senma Utrend Technology Co., Ltd & Efeler_Gibi"


#define	SK_NAME									"SKU-60-BASE"

typedef enum
{
	Chinese = 0,
	English,
	German,
	French,
	Spanish,
	Portuguese,
	Japanese,
	Russian,
	Italian,
}LANGUAGE_SELECT;

typedef struct
{
	char *L_Print;
	char *L_Ctol;
	char *L_Set;
	char *L_Preheat;
	char *L_Move;
	char *L_Extrusion;
	char *L_Fan;
	char *L_About;
	char *L_Language;
	char *L_Status;
	char *L_PrintName;
	char *L_Pause;
	char *L_Pursue;
	char *L_Stop;
	char *L_Tempertuare;
	char *L_Speed;
	char *L_PrintCtrl;
	char *L_PrintSpeed;
	char *L_SDPrint;
	char *L_Back;
	char *L_PrintFinish;
	char *L_Confirm;
	char *L_Cancel;
	char *L_LaserCtol;
	char *L_LaserMove;
	char *L_Zero;
	char *L_Ctrl;
	char *L_filament;
	char *L_StopPrint;
	char *L_Leveling;
	char *L_Adjust;
	char *L_Load;
	char *L_Unload;
	char *L_Fast;
	char *L_Normal;
	char *L_Slow;
	char *L_autoleveling;
	char *L_offset;
	char *L_Continue;
	char *L_3DPrintModeSelect;
	char *L_3dPrintExtruder1;	
	char *L_3dPrintExtruder2;
	char *L_3dPrintExtruder12;
	char *L_3dPrintHotEndReady;
	char *L_3dPrintHotEndTemperature;
	char *L_3dPrintHotEndStat1;
	char *L_3dPrintHotEndStat2;
	char *L_ZOffset;
	char *L_Homing;
	char language_choice;
}TextDisplay;

#define C_Print				"打印"
#define C_Ctol				"控制"
#define C_Set				"设置"
#define C_Preheat			"预热"
#define C_Move				"移动"
#define C_Extrusion			"挤出"
#define C_Fan				"风扇"
#define C_About				"关于"
#define C_Language			"语言"
#define C_Status			"状态"
#define	C_PrintName			"打印文件名"
#define	C_Pause				"暂停"
#define C_Pursue			"继续"
#define C_Stop				"停止"
#define C_Tempertuare		"温度"
#define	C_Speed				"速度"
#define	C_PrintCtrl			"打印控制"
#define	C_PrintSpeed		"打印速度"
#define	C_SDPrint			"SD卡打印"
#define C_Back				"返回"
#define C_PrintFinish		"打印完成"
#define	C_Confirm			"确定"
#define C_Cancel			"取消"	
#define C_Leveling			"调平"
#define C_Adjust			"屏幕校准"
#define C_Load				"进料"
#define C_Unload			"出料"
#define C_Fast				"快速"
#define	C_Normal			"中速"
#define C_Slow				"慢速"
#define C_Continue			"断点续打"

#define	C_LaserCtol			"激光雕刻控制"
#define C_LaserMove			"激光雕刻移动"
#define	C_Zero				"归零"
#define C_Filament			"耗材耗尽"
#define C_StopPrint			"停止打印？"
#define C_autoleveling   	"自动调平"
#define	C_offset         	"偏移"

#define C_3DPrintModeSelect	"请选择打印模式"		
#define C_3dPrintExtruder1	"挤出机1打印"
#define C_3dPrintExtruder2	"挤出机2打印"
#define C_3dPrintExtruder12	"双挤出机打印"
#define C_3dPrintHotEndReady "喷头准备中"
#define C_3dPrintHotEndTemperature	"喷头温度"
#define C_3dPrintHotEndStat1	"喷头加热中......"
#define C_3dPrintHotEndStat2	"耗材准备中......"
#define C_ZOffset			"Z轴偏移"
#define C_Homing			"归零中..."


#define E_Print				"Print"
#define E_Ctol				"Control"
#define E_Set				"Setting"
#define E_Preheat			"Preheat"
#define E_Move				"Move"
#define E_Extrusion			"Extrusion"
#define E_Fan				"Fan"
#define E_About				"About"
#define E_Language			"Language"
#define E_Status			"Status"
#define	E_PrintName			"PrintFileName"
#define	E_Pause				"Pause"
#define E_Pursue			"Pursue"
#define E_Stop				"Stop"
#define E_Tempertuare		"Temperature"
#define	E_Speed				"Speed"
#define	E_PrintCtrl			"PrintControl"
#define E_PrintSpeed		"PrintSpeed"
#define	E_SDPrint			"SD_Print"
#define E_Back				"Back"
#define E_PrintFinish		"PrintFinish"
#define	E_Confirm			"Confirm"
#define E_Cancel			"Cancel"
#define E_Zero				"Zero"
#define E_Ctrl				"Ctrl"
#define E_Leveling			"Leveling"
#define E_Adjust			"Adjust"
#define E_Load				"Load"
#define E_Unload			"Unload"
#define E_Fast				"Fast"
#define	E_Normal			"Normal"
#define E_Slow				"Slow"
#define E_Continue			"Continue"


#define	E_LaserCtol		"LaserControl"
#define E_LaserMove		"LaserMove"
#define E_Filament		"Filament run out"
#define E_StopPrint		"Stop Printting?"
#define E_autoleveling   "Auto leveling"
#define	E_offset         "offset"

#define E_3DPrintModeSelect	"Please select print mode"		
#define E_3dPrintExtruder1	"Select extruder 1 to print"
#define E_3dPrintExtruder2	"Select extruder 2 to print"
#define E_3dPrintExtruder12	"Double extruder printing"
#define E_3dPrintHotEndReady "The print head is in preparation"
#define E_3dPrintHotEndTemperature	"Nozzle temperature"
#define E_3dPrintHotEndStat1	"Nozzle is heating......."
#define E_3dPrintHotEndStat2	"Filament preparation......"
#define E_ZOffset			"ZOffset"
#define E_Homing			"Homing..."


//德文
#define G_Print				"Drucken"
#define G_Ctol				"Kontrolle"
#define G_Set				"Einstellen"
#define G_Preheat			"Vorwärmen"
#define G_Move				"Bewegen"
#define G_Extrusion			"ausdrücken"
#define G_Fan				"Gebläse"
#define G_About				"Über"
#define G_Language			"Sprache"
#define G_Status			"Zustand"
#define	G_PrintName			"gedrucktes Dokument"
#define	G_Pause				"Pause"
#define G_Pursue			"Weiter"
#define G_Stop				"Stopp"
#define G_Tempertuare		"Temperatur"
#define	G_Speed				"Geschwindigkeit"
#define	G_PrintCtrl			"Drucken-Kontrolle"
#define G_PrintSpeed		"Drucken-Geschwindigkeit"
#define	G_SDPrint			"Drucken aus SD Karte"
#define G_Back				"Zurück"
#define G_PrintFinish		"Vollenden"
#define	G_Confirm			"Bestätigen"
#define G_Cancel			"Stornieren"
#define G_Zero				"Nullstellen"
#define G_Ctrl				"Kontrolle"
#define G_Leveling			"Nivellierung"
#define G_Adjust			"Kalibrierung"
#define G_Load				"Belastung"
#define G_Unload			"Entladen"
#define G_Fast				"Schnell"
#define	G_Normal			"Normal"
#define G_Slow				"Langsam"
#define G_Continue			"Fortsetzen"


#define	G_LaserCtol			"Lasergravur-Kontrolle"
#define G_LaserMove			"Lasergravur-Bewegen"
#define G_Filament			"kein Verbrauchsmaterial"
#define G_StopPrint			"Drucken beenden?"
#define G_autoleveling   	"Auto Nivellierung"
#define	G_offset         	"Abweichung"

#define G_3DPrintModeSelect	"Please select print mode"		
#define G_3dPrintExtruder1	"Select extruder 1 to print"
#define G_3dPrintExtruder2	"Select extruder 2 to print"
#define G_3dPrintExtruder12	"Double extruder printing"


//French
#define F_Print				"Impression"
#define F_Ctol				"Contrôler"
#define F_Set				"Réglage"
#define F_Preheat			"Préchauffer"
#define F_Move				"Déplacer"
#define F_Extrusion			"Extrusion"
#define F_Fan				"Ventilateur"
#define F_About				"Sur"
#define F_Language			"Langage"
#define F_Status			"État"
#define	F_PrintName			"Nom de fichier de l'impression"
#define	F_Pause				"Pause"
#define F_Pursue			"Continuar"
#define F_Stop				"Parada"
#define F_Tempertuare		"Temperatura"
#define	F_Speed				"Velocidad"
#define	F_PrintCtrl			"Control de Impression"
#define F_PrintSpeed		"Vitesse de l'impression"
#define	F_SDPrint			"Impression de SD"
#define F_Back				"Retour"
#define F_PrintFinish		"Achèvement de l'impression"
#define	F_Confirm			"Confirmer"
#define F_Cancel			"Annuler"
#define F_Zero				"Remise à zéro"
#define F_Ctrl				"Contrôler"
#define F_Adjust			"Calibration"
#define F_Load				"Charge"
#define F_Unload			"Décharger"
#define F_Fast				"Rapide"
#define	F_Normal			"Normal"
#define F_Slow				"Lent"
#define F_Continue			"Continuer"

#define	F_LaserCtol			"Contrôle de gravure à laser"
#define F_LaserMove			"Déplacement de gravure à laser"
#define F_Filament			"Consommables épuisés"
#define F_StopPrint			"Arrêter l'impression?"
#define F_Leveling			"Nivellement"
#define F_autoleveling   	"Nivellement auto"
#define	F_offset         	"décalage"

#define F_3DPrintModeSelect	"Please select print mode"		
#define F_3dPrintExtruder1	"Select extruder 1 to print"
#define F_3dPrintExtruder2	"Select extruder 2 to print"
#define F_3dPrintExtruder12	"Double extruder printing"

//Spanish
#define S_Print				"Imprimir"
#define S_Ctol				"Controlar"
#define S_Set				"Configuración"
#define S_Preheat			"Precalentamiento"
#define S_Move				"Mover"
#define S_Extrusion			"Extrusión"
#define S_Fan				"Ventilador"
#define S_About				"Sobre"
#define S_Language			"Lengua"
#define S_Status			"Estado"
#define	S_PrintName			"Nombre del documento de  impresión"
#define	S_Pause				"Suspensión"
#define S_Pursue			"Continuar"
#define S_Stop				"Parada"
#define S_Tempertuare		"Temperatura"
#define	S_Speed				"Velocidad"
#define	S_PrintCtrl			"Control de impresión"
#define S_PrintSpeed		"Velocidad de impresión"
#define	S_SDPrint			"Imprimir de la tarjeta SD"
#define S_Back				"Regresar"
#define S_PrintFinish		"Terminación de impresión"
#define	S_Confirm			"OK"
#define S_Cancel			"Cancelar"
#define S_Zero				"Reducir a cero"
#define S_Ctrl				"Controlar"
#define S_Leveling			"Nivelación"
#define S_Adjust			"Calibración"
#define S_Load				"Carga"
#define S_Unload			"Descargar"
#define S_Fast				"Rápido"
#define	S_Normal			"Normal"
#define S_Slow				"Lento"
#define S_Continue			"Seguir"

#define	S_LaserCtol			"Control de grabado  láser"
#define S_LaserMove			"Mover de grabado  láser"
#define S_Filament			"No materiales para impresión"
#define S_StopPrint			"¿Dejar de imprimir?"
#define S_autoleveling   	"Nivelación auto"
#define	S_offset         	"compensar"

#define S_3DPrintModeSelect	"Please select print mode"		
#define S_3dPrintExtruder1	"Select extruder 1 to print"
#define S_3dPrintExtruder2	"Select extruder 2 to print"
#define S_3dPrintExtruder12	"Double extruder printing"

//Portuguese
#define P_Print				"Impressão"
#define P_Ctol				"Contrôle"
#define P_Set				"Configuração"
#define P_Preheat			"Pré-aqueça"
#define P_Move				"Movimento"
#define P_Extrusion			"Extrução"
#define P_Fan				"Ventilador"
#define P_About				"Sobre"
#define P_Language			"Língua"
#define P_Status			"Estado"
#define	P_PrintName			"Nome do documento imprimido"
#define	P_Pause				"Pausar"
#define P_Pursue			"Continuar"
#define P_Stop				"Parar"
#define P_Tempertuare		"Temperatura"
#define	P_Speed				"Velocidade"
#define	P_PrintCtrl			"Contrôle de impressão"
#define P_PrintSpeed		"Velocidade de impressão"
#define	P_SDPrint			"Impressão através do cartão SD"
#define P_Back				"Voltar"
#define P_PrintFinish		"Impressão terminada"
#define	P_Confirm			"OK"
#define P_Cancel			"Cancelar"
#define P_Zero				"Voltar a zero"
#define P_Ctrl				"Contrôle"
#define P_Adjust			"Calibração"
#define P_Load				"Carga"
#define P_Unload			"Descarregar"
#define P_Fast				"Rápido"
#define	P_Normal			"Normal"
#define P_Slow				"Lento"
#define P_Continue			"Continuar"

#define	P_LaserCtol			"Contrôle de gravação por laser"
#define P_LaserMove			"Movimento de gravação por laser"
#define P_Filament			"Esgotamento de consumíveis"
#define P_StopPrint			"Parar de imprimir?"	
#define P_Leveling			"Nivelamento"
#define P_autoleveling   	"Nivelamento auto"
#define	P_offset         	"Deslocamento"

#define P_3DPrintModeSelect	"Please select print mode"		
#define P_3dPrintExtruder1	"Select extruder 1 to print"
#define P_3dPrintExtruder2	"Select extruder 2 to print"
#define P_3dPrintExtruder12	"Double extruder printing"


//日语
#define J_Print				"印刷"
#define J_Ctol				"コントロール"
#define J_Set				"設定"
#define J_Preheat			"予熱"
#define J_Move				"移動"
#define J_Extrusion			"押出"
#define J_Fan				"ファン"
#define J_About				"について"
#define J_Language			"言語"
#define J_Status			"状態"
#define	J_PrintName			"印刷ファイル名"
#define	J_Pause				"一時停止"
#define J_Pursue			"続ける"
#define J_Stop				"停止"
#define J_Tempertuare		"温度"
#define	J_Speed				"スピード"
#define	J_PrintCtrl			"印刷コントロール"
#define J_PrintSpeed		"印刷スピード"
#define	J_SDPrint			"SDカード印刷"
#define J_Back				"戻る"
#define J_PrintFinish		"印刷完了"
#define	J_Confirm			"確定"
#define J_Cancel			"キャンセル"
#define J_Zero				"クリア"
#define J_Ctrl				"コントロール"
#define J_Leveling			"底板調整"
#define J_Adjust			"訂正"
#define J_Load				"負荷"
#define J_Unload			"降ろす"
#define J_Fast				"速い"
#define	J_Normal			"普通"
#define J_Slow				"スロー"
#define J_Continue			"継続する"

#define	J_LaserCtol			"レーザー彫刻コントロール"
#define J_LaserMove			"レーザー彫刻移動"
#define J_Filament			"材料が足りないです"	
#define J_StopPrint			"印刷を停止しますか? "
#define J_autoleveling   	"セルフレベリング"
#define	J_offset         	"オフセット"

#define J_3DPrintModeSelect	"Please select print mode"		
#define J_3dPrintExtruder1	"Select extruder 1 to print"
#define J_3dPrintExtruder2	"Select extruder 2 to print"
#define J_3dPrintExtruder12	"Double extruder printing"


//俄语
#define R_Print				"Печать"
#define R_Ctol				"Проверка"
#define R_Set				"Настройки"
#define R_Preheat			"Предварительный подогрев"
#define R_Move				"Перемещение"
#define R_Extrusion			"Экструзия"
#define R_Fan				"Вентилятор"
#define R_About				"Об"
#define R_Language			"Язык"
#define R_Status			"Состояние"
#define	R_PrintName			"Печать имени файла"
#define	R_Pause				"Пауза"
#define R_Pursue			"Продолжить"
#define R_Stop				"Стоп"
#define R_Tempertuare		"Температура"
#define	R_Speed				"Скорость"
#define	R_PrintCtrl			"Управление печатью"
#define R_PrintSpeed		"Скорость печати"
#define	R_SDPrint			"Печать SD-карты"
#define R_Back				"Назад"
#define R_PrintFinish		"Завершение печати"
#define	R_Confirm			"Да"
#define R_Cancel			"Отмена"
#define R_Zero				"Сброс"
#define R_Ctrl				"Проверка"
#define R_Adjust			"屏幕校准"
#define R_Continue			"Продолжить"

#define	R_LaserCtol			"Контроль лазерной гравировки"
#define R_LaserMove			"Перемещение лазерной гравировки"
#define R_Filament			"Расходные материалы исчерпаны"

#define R_3DPrintModeSelect	"Please select print mode"		
#define R_3dPrintExtruder1	"Select extruder 1 to print"
#define R_3dPrintExtruder2	"Select extruder 2 to print"
#define R_3dPrintExtruder12	"Double extruder printing"


//Italian
#define I_Print				"Stampa"
#define I_Ctol				"Controlla"
#define I_Set				"Impostazioni"
#define I_Preheat			"Preriscalda"
#define I_Move				"Mossa"
#define I_Extrusion			"Estrusione"
#define I_Fan				"Ventilatore"
#define I_About				"Su"
#define I_Language			"linguaggio"
#define I_Status			"Stato"
#define	I_PrintName			"Filenome stampatp"
#define	I_Pause				"Pausa"
#define I_Pursue			"Continua"
#define I_Stop				"Ferma"
#define I_Tempertuare		"Temperatura"
#define	I_Speed				"Velocità"
#define	I_PrintCtrl			"Controllo di stampa"
#define I_PrintSpeed		"Velocità di stampa"
#define	I_SDPrint			"Stampa da SD"
#define I_Back				"Indietro"
#define I_PrintFinish		"Fine stampa"
#define	I_Confirm			"Conferma"
#define I_Cancel			"Annulla"
#define I_Zero				"Azzera"
#define I_Ctrl				"Controlla"
#define I_Leveling			"livellamento"
#define I_Adjust			"correzione"
#define I_Load				"Caricare"
#define I_Unload			"Scaricare"
#define I_Fast				"Veloce"
#define	I_Normal			"Normale"
#define I_Slow				"Lento"
#define I_Continue			"Continua"

#define	I_LaserCtol			"Controllo dell'incisione laser"
#define I_LaserMove			"Incisione laser mobile"
#define I_Filament			"Materiale esaurito"
#define I_StopPrint			"Smetti di stampare?"	
#define I_autoleveling   "Livellamento auto"
#define	I_offset         "compensare"

#define I_3DPrintModeSelect	"Please select print mode"		
#define I_3dPrintExtruder1	"Select extruder 1 to print"
#define I_3dPrintExtruder2	"Select extruder 2 to print"
#define I_3dPrintExtruder12	"Double extruder printing"

#ifdef __cplusplus
}
#endif				//End of __cplusplus

#endif				//End of files








