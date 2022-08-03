/*
 * ssd1306_adds.h
 *
 *  Created on: Apr 22, 2022
 *      Author: Tomasz Kaliński
 */

#ifndef INC_SSD1306_ADDS_H_
#define INC_SSD1306_ADDS_H_

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

#include <stdbool.h>
#include <stdio.h>
#include "ssd1306.h"

/* STRUCT */
//=============================
typedef struct {
	char		*fullName;
	char		*shortName;
	char		*descShort_1;
	char		*descShort_2;
	char		*descFull_1;
	char		*descFull_2;
	uint16_t	width;
	uint16_t	turns[10];
	uint16_t	diameter[10];
	uint8_t		taskCount;
} ProjectManager;

typedef struct {
	uint16_t	minValue;
	uint16_t	setValue;
	uint16_t	maxValue;
	uint8_t		digitsCount;
	uint8_t 	dotPosition;
} Step;

/* ENUM */
//=============================

// MENU/SETTINGS/STEPS LABELS
typedef enum {
	STEP_LOGO_DISP			= 0,
	STEP_SELECT_PROJECT 	= 1,
	STEP_PROJECT_DETAILS	= 11,
	STEP_PROJECT_TASKS_LIST	= 12,
	STEP_WIDTH_SET			= 2,
	STEP_TURNS_SET			= 3,
	STEP_DIAMETER_SET		= 4,
	STEP_SPEED_SET			= 5,
	STEP_SUMMARY			= 6,
	STEP_CORRECTNES_QUERY	= 61,
	STEP_START_POSITION_SET = 7
} STEPS_LABELS;

// SELECT TYPE OF VALUE
typedef enum {
	VAL_TYPE_CARCASS_WIDTH		= 0,	// ust. szer. karkasu
	VAL_TYPE_CARCASS_COIL_TURNS = 1, 	// ust. ilości zwojów
	VAL_TYPE_WINDING_DIAMETER 	= 2, 	// ust. ilości zwojów
	VAL_TYPE_WINDING_SPEED		= 3		// ust. predkosci nawijania
} VALUE_TYPE;

// SET VALUE
/* VAL_RNG_WINDING -> if changed - add/remove switch case in stepper.c || FUNC: Steppers_Control_Init */
typedef enum {
	VAL_RNG_CARCASS_MIN_WIDTH 		= 50, 	// 5mm - minimalna szer. karkasu
	VAL_RNG_CARCASS_MAX_WIDTH 		= 1299, // 12,5cm - maksymalna szerokosc karkasu
	VAL_RNG_CARCASS_MIN_TURNS		= 10,	// minimalna liczba zwojow
	VAL_RNG_CARCASS_MAX_TURNS		= 2999, // maksymalna liczba zwojow
	VAL_RNG_WINDING_MIN_DIAMETER	= 1,	// 0,01 - minimalna srednica uzwojenia
	VAL_RNG_WINDING_MAX_DIAMETER	= 199,	// 1.99mm - maksymalna srednica uzwojenia
	VAL_RNG_WINDING_MIN_SPEED		= 1,	// min szybkość nawijania
	VAL_RNG_WINDING_MAX_SPEED		= 5,	// max szybkość nawijania
} VALUE_RANGE;

// CHANGE VALUE
typedef enum {
	VALUE_CHANGE_FLAG_NO 	= 0, 	// nie zmieniaj wartosci
	VALUE_CHANGE_FLAG_YES 	= 1  	// zmieniaj wartosc
} VALUE_CHANGE_FLAG;

// FIRST RUN
typedef enum {
	RUN_FLAG_EDIT			= 2,
	RUN_FLAG_FIRST			= 1, 	// pierwsze uruchomienie
	RUN_FLAG_CONTI			= 0		// kontunuacja
} RUN_FLAG;

// BOX_POSITION
typedef enum {
	ALIGN_LEFT			= 5,	// LEFT
	ALIGN_RIGHT			= 68 	// RIGHT

} ALIGN;

/* CONST */
//=============================

/* main/top labels */
#define DISP_PROJECT_LABEL		"Wybierz projekt:"
#define DISP_SET_WIDTH_LABEL	"Szer. karkasu:"
#define DISP_SET_TURNS_LABEL	"Liczba zwojow:"
#define DISP_SET_DIAMETER_LABEL	"Srednica uzw.:"
#define DISP_SET_SPEED_LABEL	"Predkosc nawij.:"
#define DISP_SET_SUMMARY_LABEL	"Podsumowanie:"
#define DISP_CORRECTNESS_QUERY	"Poprawne?"

/* details labels */
#define DISP_DETAIL_WIDTH_LABEL  		"Szerokosc: "
#define DISP_DETAIL_TURNS_LABEL			"Ilosc: "
#define DISP_DETAIL_DIAMETER_LABEL		"Srednica: "
#define DISP_DETAIL_SPEED_LABEL			"Predkosc:"
#define DISP_DETAIL_TASK_COUNT_LABEL 	"L. zadan: "

/* selector */
#define DISP_TEXT_YES		"TAK"
#define DISP_TEXT_NO		"NIE"
#define DISP_TEXT_OK		"OK"
#define DISP_TEXT_CORRECT	"POPRAW"
#define DISP_TEXT_RETURN	"POWROT"

//=============================
/* FUNCTIONS */

//-- LOAD STRUCT VALUES
void Structures_Init(void); 	// przyposuje określone wartości do struktur

//-- MAIN DISPLAY FUNCION
void Set_Theme(); 				// wyswietla obraz w zaleznosci od wybranego kroku

//-- PROJECT SELECT PAGE (1)
void Show_Project_Elements(ProjectManager * details, uint8_t margin); 	// 5 - left; 68 - right // wyswietla kompletne menu wyboru projektu (1)
void Show_Project_Select_Menu(void); 									// pobiera strukture projektu i dobiera marginesy
void New_Task_Element(void); 											// kafelek odpowiedzialny za wybór nowego zadania

//-- PROJECT DETAILS (11)
void Show_Project_Details(ProjectManager * details, bool list);		// wyswietla szczegoly projektu
void Show_Project_Tasks(ProjectManager * details);					// wyswietla zadanai danego projektu

//-- VALUE SETTINGS (2++)
void Show_Value_Screen(VALUE_TYPE type, uint8_t runMode, bool direction, uint8_t runCount);		// menu ustawien vartosci
void Set_Marker_Position(uint8_t divider); 														// ustawia marker/wskaznik na okreslone miejsce, w zaleznosci od polozenia kropki "divider"
void Move_Marker(uint8_t range);																// zwieksza markerPosition 0-4
void Change_Value(bool set, uint8_t position, uint16_t min, uint16_t max); 						// zwieksza lub zmniejsza cyfre 0-9
uint16_t Array_To_Int_Change_Value(void);														// zamienia arrayToken[] na integer
void Int_To_Array_Change_Value(uint16_t value);													// zamienia integer na arrayToken[]
void Save_Set_Value(uint16_t value);															// zapamietuje wybrane dane na wypadek koniecznosci poprawki
void Draw_Changing_Value_Marker(uint8_t width, uint8_t height); 								// rysuje trójkąt - wspołrzedne wskazuja wierzchołek

//-- CLEAR DISPLAY
void Clear_Changing_Value_Marker(void); 	// usuwa wskaznik/trojkat
void Clear_Value(void);						// usuwa wyświetlaną, zmienianą wartość
void Clear_Content(void); 					// czyści na wyswietlaczu obszar ponizej paska postepu

//-- SUMMARY (6)
void Show_Summary(void);					// podsumowanie ustawień

//-- CORRECTNESS QUERY (61)
void Show_Summary_Correctness_Query(bool direction, uint8_t runCount);	// zatwierdzenie ustawien
bool Show_Select_Boxes(uint8_t margin, uint8_t pointer);				// okienko wyboru

//-- UNIVERSAL FUNCTION
void Show_Label_Bar(char* label); 							// wyswietla pasek z tytulem strony
void Pagination_Bar(uint8_t pageBarWidth, uint8_t pageNo); 	// wyswietla pasek postepu - szerokosc pojedynczej strony, aktualna strona
uint8_t Count_Array(ProjectManager * details); 				// określa ilość zadań w danym projekcie

#endif /* INC_SSD1306_ADDS_H_ */
