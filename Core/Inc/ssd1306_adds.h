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

//STRUCT
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

// CONST
#define DISP_PROJECT_LABEL		"Wybierz projekt:"
#define DISP_SET_WIDTH_LABEL	"Szer. karkasu:"

// project details
#define WIDTH_LABEL  			"Szerokosc: "
#define TASK_NO_LABEL 			"L. zadan: "

// FUNCTIONS

//--glowna funkcja zarzadzajaca wyswietlana trescia
void setTheme(); 												// wyswietla obraz w zaleznosci od wybranego kroku

//--strona startowa
void showLogo(void); 											// START LOGO (0)

//--wybor projektu (1)
void showProjectElements(Project * project, uint8_t margin); 	// 5 - left; 68 - right // wyswietla kompletne menu wyboru projektu (1)
void showProjectSelectMenu(void); 								// pobiera strukture projektu i dobiera marginesy
void newTaskElement(void); 										// kafelek odpowiedzialny za wybór nowego zadania

//--szczegoly projektu (11)
void showProjectDetails(Project * project);						// wyswietla szczegoly projektu

//--szerokosc karkasu (2)
void showWidthScreen(uint8_t runMode, bool direction);			// menu ustawien szerokosci karkasu

//--ustawianie wartosci
void setMarkerPosition(uint8_t divider); 						// rysuje wskaznik/trojkat pod ustawiana wartoscia

void moveMarker(void);											// zwieksza markerPosition 0-4
void changeValue(bool set, uint16_t min, uint16_t max); 		// zwieksza lub zmniejsza cyfre 0-9
void drawMarker(uint8_t width, uint8_t height); 				// rysuje trójkąt - wspołrzedne wskazuja wierzchołek
void clearMarker(void); 										// usuwa wskaznik/trojkat
void clearValue(void);											// usuwa liczbe/wartosci

//--uniwersalne
void showLabelBar(char* label); 								// wyswietla pasek z tytulem strony
void clearContent(void); 										// czyści na wyswietlaczu obszar ponizej paska postepu
void paginationBar(uint8_t pageBarWidth, uint8_t pageNo); 		// wyswietla pasek postepu - szerokosc pojedynczej strony, aktualna strona
Project getProjectStructByID(uint8_t id); 						// zwraca strukture w zaleznosci od wybranego id
uint8_t countArray(Project * project); 							// zlicza ilosc tablic pierwszego wymiaru

#endif /* INC_SSD1306_ADDS_H_ */
