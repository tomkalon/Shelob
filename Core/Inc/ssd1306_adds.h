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

#include "ssd1306.h"
#include "misc.h"

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

// SELECT TYPE OF VALUE
typedef enum {
	CARCASS_WIDTH			= 0,	// ust. szer. karkasu
	CARCASS_COIL_TURNS 		= 1, 	// ust. ilości zwojów
	WINDING_DIAMETER 		= 2, 	// ust. ilości zwojów
	WINDING_SPEED			= 3		// ust. predkosci nawijania
} VALUE_TYPE;

// SET VALUE
typedef enum {
	CARCASS_MIN_WIDTH 		= 50, 	// 5mm - minimalna szer. karkasu
	CARCASS_MAX_WIDTH 		= 1250, // 12,5cm - maksymalna szerokosc karkasu
	CARCASS_MIN_TURNS		= 10,	// minimalna liczba zwojow
	CARCASS_MAX_TURNS		= 2500, // maksymalna liczba zwojow
	WINDING_MIN_DIAMETER	= 1,	// 0,01 - minimalna srednica uzwojenia
	WINDING_MAX_DIAMETER	= 250,	// 2,5mm - maksymalna srednica uzwojenia
	WINDING_MIN_SPEED		= 1,	// min szybkość nawijania
	WINDING_MAX_SPEED		= 9,	// max szybkość nawijania
} VALUE_RANGE;

// CHANGE VALUE
typedef enum {
	VALUE_NO_CHANGING 	= 0, 	// nie zmieniaj wartosci
	VALUE_CHANGING 		= 1  	// zmieniaj wartosc
} VALUE_CHANGE_FLAG;

// FIRST RUN
typedef enum {
	EDIT_RUN			= 2,
	FIRST_RUN 			= 1, 	// pierwsze uruchomienie
	CONTI_RUN			= 0		// kontunuacja
} RUN_FLAG;

/* CONST */
//=============================
#define DISP_PROJECT_LABEL		"Wybierz projekt:"
#define DISP_SET_WIDTH_LABEL	"Szer. karkasu:"
#define DISP_SET_TURNS_LABEL	"Liczba zwojow:"
#define DISP_SET_DIAMETER_LABEL	"Srednica uzw.:"
#define DISP_SET_SPEED_LABEL	"Predkosc nawij.:"
#define DISP_SET_SUMMARY_LABEL	"Podsumowanie:"
#define DISP_CORRECTNESS_QUERY		"Chcesz poprawic?"

/* project details */
#define WIDTH_LABEL  			"Szerokosc: "
#define TURNS_LABEL				"Ilosc: "
#define DIAMETER_LABEL			"Srednica: "
#define SPEED_LABEL				"Predkosc:"
#define TASK_NO_LABEL 			"L. zadan: "

//=============================
/* FUNCTIONS */

//-- LOAD STRUCT VALUES
void structInit(void);

//-- MAIN DISPLAY FUNCION
void setTheme(); 													// wyswietla obraz w zaleznosci od wybranego kroku

//-- PROJECT SELECT PAGE (1)
void showProjectElements(ProjectManager * details, uint8_t margin); // 5 - left; 68 - right // wyswietla kompletne menu wyboru projektu (1)
void showProjectSelectMenu(void); 									// pobiera strukture projektu i dobiera marginesy
void newTaskElement(void); 											// kafelek odpowiedzialny za wybór nowego zadania

//-- PROJECT DETAILS (11)
void showProjectDetails(ProjectManager * details);					// wyswietla szczegoly projektu

//-- VALUE SETTINGS (2++)
void showValueScreen(VALUE_TYPE type, uint8_t runMode, bool direction, uint8_t runCount);	// menu ustawien vartosci
void setMarkerPosition(uint8_t divider); 							// ustawia marker/wskaznik na okreslone miejsce, w zaleznosci od polozenia kropki "divider"
void moveMarker(uint8_t range);										// zwieksza markerPosition 0-4
void changeValue(bool set, uint8_t position, uint16_t min, uint16_t max); // zwieksza lub zmniejsza cyfre 0-9
uint16_t arrayToInt_chVal(void);									// zamienia arrayToken[] na integer
void intToArray_chVal(uint16_t value);								// zamienia integer na arrayToken[]
void drawMarker(uint8_t width, uint8_t height); 					// rysuje trójkąt - wspołrzedne wskazuja wierzchołek
void clearMarker(void); 											// usuwa wskaznik/trojkat
void clearValue(void);

//-- SUMMARY (6)
void showSummary(void);												// podsumowanie ustawień

//-- CORRECTNESS QUERY (61)
void correctnessQuery(void);										// zatwierdzenie ustawien

//-- UNIVERSAL FUNCTIONS
void showLabelBar(char* label); 									// wyswietla pasek z tytulem strony
void clearContent(void); 											// czyści na wyswietlaczu obszar ponizej paska postepu
void paginationBar(uint8_t pageBarWidth, uint8_t pageNo); 			// wyswietla pasek postepu - szerokosc pojedynczej strony, aktualna strona
uint8_t countArray(ProjectManager * details); 						// określa ilość zadań w danym projekcie

#endif /* INC_SSD1306_ADDS_H_ */
