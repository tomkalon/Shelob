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
#include <stdbool.h>
#include <stdio.h>

/* STRUCT */
//=============================
typedef struct {
	char		*fullName;
	char		*shortName;
	char		*desc1;
	char		*desc2;
	char		*desc1F;
	char		*desc2F;
	uint16_t	width;
	uint16_t	coil[10][2];
} Project;

/* ENUM */
//=============================

// SELECT TYPE OF VALUE
typedef enum {
	CARCASS_WIDTH			= 0,	// ust. szer. karkasu
	CARCASS_COIL_TURNS 		= 1, 	// ust. ilości zwojów
	WINDING_DIAMETER 		= 2, 	// ust. ilości zwojów
	WINDING_SPEED			= 3
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
	FIRST_RUN 			= 1, 	// pierwsze uruchomienie
	CONTI_RUN			= 0		// kontunuacja
} RUN_FLAG;

/* CONST */
//=============================
#define DISP_PROJECT_LABEL		"Wybierz projekt:"
#define DISP_SET_WIDTH_LABEL	"Szer. karkasu:"
#define DISP_SET_TURNS_LABEL	"Liczba zwojow:"
#define DISP_SET_DIAMETER_LABEL	"Srednica uzw.:"
#define DISP_SET_SPEED_LABEL	"Szybkosc:"
#define DISP_SET_SUMMARY_LABEL	"Podsumowanie:"

/* project details */
#define WIDTH_LABEL  			"Szerokosc: "
#define TASK_NO_LABEL 			"L. zadan: "

//=============================
/* FUNCTIONS */

//-- MAIN DISPLAY FUNCION
void setTheme(); 													// wyswietla obraz w zaleznosci od wybranego kroku

//-- START PAGE
void showLogo(void); 												// START LOGO (0)

//-- PROJECT SELECT PAGE (1)
void showProjectElements(Project * project, uint8_t margin); 		// 5 - left; 68 - right // wyswietla kompletne menu wyboru projektu (1)
void showProjectSelectMenu(void); 									// pobiera strukture projektu i dobiera marginesy
void newTaskElement(void); 											// kafelek odpowiedzialny za wybór nowego zadania

//-- PROJECT DETAILS (11)
void showProjectDetails(Project * project);							// wyswietla szczegoly projektu

//-- VALUE SETTINGS (2++)
void showValueScreen(VALUE_TYPE type, uint8_t runMode, bool direction, bool first);	// menu ustawien vartosci
void setMarkerPosition(uint8_t divider); 							// rysuje wskaznik/trojkat pod ustawiana wartoscia
void moveMarker(uint8_t range);										// zwieksza markerPosition 0-4
void changeValue(bool set, uint8_t position, uint16_t min, uint16_t max); // zwieksza lub zmniejsza cyfre 0-9
uint16_t arrayToInt_chVal(void);									// zamienia arrayToken[] na integer
void intToArray_chVal(uint16_t value);								// zamienia integer na arrayToken[]
void drawMarker(uint8_t width, uint8_t height); 					// rysuje trójkąt - wspołrzedne wskazuja wierzchołek
void clearMarker(void); 											// usuwa wskaznik/trojkat
void clearValue(void);												// usuwa liczbe/wartosci
void clearSettings(void);											// przygotowuje pod nowe ustawienia / czysci wczesniejsze wartosci

//-- UNIVERSAL FUNCTIONS
void showLabelBar(char* label); 									// wyswietla pasek z tytulem strony
void clearContent(void); 											// czyści na wyswietlaczu obszar ponizej paska postepu
void paginationBar(uint8_t pageBarWidth, uint8_t pageNo); 			// wyswietla pasek postepu - szerokosc pojedynczej strony, aktualna strona
Project getProjectStructByID(uint8_t id); 							// zwraca strukture w zaleznosci od wybranego id
uint8_t countArray(Project * project); 								// zlicza ilosc tablic pierwszego wymiaru

#endif /* INC_SSD1306_ADDS_H_ */
