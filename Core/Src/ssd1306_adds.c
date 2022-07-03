/*
 * ssd1306_adds.c
 *
 *  Created on: Apr 22, 2022
 *      Author: Tomasz Kaliński
 */

#include "ssd1306_adds.h"
#include "ssd1306_bitmaps.h"

// CONST
#define PROJECT_COUNT 3 			// liczba zapisanych projektów

// VAR
volatile uint8_t workStep			= 0;
volatile uint8_t projectSelect 		= 0; 			// wybrany/wybierany projekt z listy projektow

volatile uint8_t progressBarWidth	= 0; 			// szerokosc wskaznika stron
volatile uint8_t progressBarStep	= 0; 			// polozenie wskaznika stron

//markery wyboru
volatile int8_t markerPosition 		= 0; 			// polozenie wskaznika ustawianej wartosci
volatile int8_t arrayToken[5] 		= {0,0,0,0,0};	// ustawianie wartosci

void setTheme(void)
{
	clearContent();
	switch (workStep)
	{
		case 0: // wyświetla logo
			showLogo();
			workStep++;
			SSD1306_UpdateScreen();
			HAL_Delay(1000);
			SSD1306_Clear();
			SSD1306_UpdateScreen();
			setTheme();
		break;
		case 1: // wybór projektu - nowy lub istniejacy
			showLabelBar(DISP_PROJECT_LABEL);
			progressBarWidth = (128 / ((PROJECT_COUNT + 1) / 2) + ((PROJECT_COUNT + 1) % 2));
			progressBarStep = projectSelect / 2;
			paginationBar(progressBarWidth, progressBarStep);
			showProjectSelectMenu();
		break;
		case 11:; // szczegoly projektu
			Project Handler = getProjectStructByID(projectSelect);
			showProjectDetails(&Handler);
		break;
		case 2: // ustawienie szerokości karkasu
			showLabelBar(DISP_SET_WIDTH_LABEL);
			showValueScreen(CARCASS_WIDTH, 0, 0, FIRST_RUN);
		break;
		case 3: // ustawienie ilosci zwojow
			showLabelBar(DISP_SET_TURNS_LABEL);
			showValueScreen(CARCASS_COIL_TURNS, 0, 0, FIRST_RUN);
		break;
		case 4: // srednica uzwojenia
			showLabelBar(DISP_SET_DIAMETER_LABEL);
			showValueScreen(WINDING_DIAMETER, 0, 0, FIRST_RUN);
		break;
		case 5: // szybkosc nawijania
			showLabelBar(DISP_SET_DIAMETER_LABEL);
			showValueScreen(WINDING_SPEED, 0, 0, FIRST_RUN);
		break;
		case 6: // podsumowanie
			showLabelBar(DISP_SET_SUMMARY_LABEL);
		break;
	}
	SSD1306_UpdateScreen();
}

// start - 0
// -------------------------------------------------------------------------------------
void showLogo(void)
{
	SSD1306_DrawBitmap(0, 0, logo, 128, 64, 1);
}

// wybór projektu - 1
// -------------------------------------------------------------------------------------
void showProjectSelectMenu(void)
{
	uint8_t side = 5;
	uint8_t display = projectSelect;
	uint8_t step = 0;

		if(projectSelect < 2)
		{
			newTaskElement();
			Project Handler = getProjectStructByID(1);
			showProjectElements(&Handler, 69);
		}
		else
		{
			while(step < 2)
			{
				if(!step)
				{
					if(projectSelect % 2)
					{
						side = 68;
					}
					else
					{
						side = 5;
					}
					Project Handler = getProjectStructByID(display);
					showProjectElements(&Handler, side);
				}
				else
				{
					if(projectSelect % 2)
					{
						display--;
						side = 5;
					}
					else
					{
						display++;
						side = 68;
					}
					Project Handler = getProjectStructByID(display);
					showProjectElements(&Handler, side);
				}
				step++;
			}
		}
}

void newTaskElement(void)
{
	bool color = 0;
	if((projectSelect + 3) % 2)
	{
		SSD1306_DrawFilledRectangle(5, 25, 56, 47, 1);
		color = 0;
	}
	else
	{
		SSD1306_DrawRectangle(5, 25, 56, 47, 1);
		color = 1;
	}

	SSD1306_GotoXY(18, 33);
	SSD1306_Puts("Nowe", &Font_7x10, color);
	SSD1306_GotoXY(9, 46);
	SSD1306_Puts("zadanie", &Font_7x10, color);
}

void showProjectElements(Project * project, uint8_t margin)
{
	bool color = 0;
	if(margin == 5)
	{
		if((projectSelect + 3) % 2)
		{
			SSD1306_DrawFilledRectangle(margin, 25, 56, 47, 1);
			color = 0;
		}
		else
		{
			SSD1306_DrawRectangle(margin, 25, 56, 47, 1);
			color = 1;
		}
	}
	else
	{
		if((projectSelect + 3) % 2)
		{

			SSD1306_DrawRectangle(margin, 25, 56, 47, 1);
			color = 1;
		}
		else
		{
			SSD1306_DrawFilledRectangle(margin, 25, 56, 47, 1);
			color = 0;
		}
	}
	margin += 4;
	SSD1306_GotoXY(margin, 29);
	SSD1306_Puts(project->shortName, &Font_7x10, color);
	SSD1306_GotoXY(margin, 40);
	SSD1306_Puts(project->desc1, &Font_7x10, color);
	SSD1306_GotoXY(margin, 51);
	SSD1306_Puts(project->desc2, &Font_7x10, color);
}

// szczegoly projektu - 11
// -------------------------------------------------------------------------------------

void showProjectDetails(Project * project)
{

	char width[10];
	sprintf(width, "%i.%imm", project->width / 10, project->width % 10);
	showLabelBar(project->fullName);
	SSD1306_GotoXY(0, 20);
	SSD1306_Puts(WIDTH_LABEL, &Font_7x10, 1);
	SSD1306_GotoXY(70, 20);
	SSD1306_Puts(width, &Font_7x10, 1);
	SSD1306_GotoXY(0, 31);
	SSD1306_Puts(TASK_NO_LABEL, &Font_7x10, 1);
	SSD1306_GotoXY(70, 31);
	uint8_t count = countArray(project);
	sprintf(width, "%i", count);
	SSD1306_Puts(width, &Font_7x10, 1);

	char * desc1;
	char * desc2;
	if(!project->desc1F)
	{
		desc1 = project->desc1;
	}
	else
	{
		desc1 = project->desc1F;
	}
	if(!project->desc2F)
	{
		desc2 = project->desc2;
	}
	else
	{
		desc2 = project->desc2F;
	}
	SSD1306_GotoXY(0, 42);
	SSD1306_Puts(desc1, &Font_7x10, 1);
	SSD1306_GotoXY(0, 53);
	SSD1306_Puts(desc2, &Font_7x10, 1);
}

// ustawianie wartosci - 2++
// -------------------------------------------------------------------------------------
void showValueScreen(VALUE_TYPE type, uint8_t runMode, bool direction, bool first)
{
	char valueLettering[10];
	uint8_t typeToken = type;

	switch (typeToken)
	{
		case 0:
			if(first) {
				SSD1306_DrawBitmap(0, 0, width, 128, 64, 1);
				intToArray_chVal(CARCASS_MIN_WIDTH);
			}
			else
			{
				if(runMode) {changeValue(direction, markerPosition, CARCASS_MIN_WIDTH, CARCASS_MAX_WIDTH);}
				else{moveMarker(4);}
			}
			setMarkerPosition(1);
			sprintf(valueLettering, "%i%i%i.%imm", arrayToken[3], arrayToken[2], arrayToken[1], arrayToken[0]);
		break;
		case 1:
			if(first) {
				SSD1306_DrawBitmap(0, 0, turns, 128, 64, 1);
				intToArray_chVal(CARCASS_MIN_TURNS);
			}
			else
			{
				if(runMode) {changeValue(direction, markerPosition, CARCASS_MIN_TURNS, CARCASS_MAX_TURNS);}
				else{moveMarker(4);}
			}
			setMarkerPosition(0);
			sprintf(valueLettering, "%i%i%i%izw.", arrayToken[3], arrayToken[2], arrayToken[1], arrayToken[0]);
		break;
		case 2:
			if(first) {
				SSD1306_DrawBitmap(0, 0, diameter, 128, 64, 1);
				intToArray_chVal(WINDING_MIN_DIAMETER);
			}
			else
			{
				if(runMode) {changeValue(direction, markerPosition, WINDING_MIN_DIAMETER, WINDING_MAX_DIAMETER);}
				else{moveMarker(3);}
			}
			setMarkerPosition(2);
			sprintf(valueLettering, "~%i.%i%imm", arrayToken[2], arrayToken[1], arrayToken[0]);
		break;
		case 3:
			if(first) {
				SSD1306_DrawBitmap(0, 0, speed, 128, 64, 1);
				intToArray_chVal(WINDING_MIN_SPEED);
			}
			else
			{
				if(runMode) {changeValue(direction, markerPosition, WINDING_MIN_SPEED, WINDING_MAX_SPEED);}
				else{moveMarker(1);}
			}
			setMarkerPosition(0);
			sprintf(valueLettering, "   %i", arrayToken[0]);
		break;
	}

	clearValue();
	SSD1306_GotoXY(25, 20);
	SSD1306_Puts(valueLettering, &Font_11x18, 1);
	SSD1306_UpdateScreen();
}

void setMarkerPosition(uint8_t divider)
{
	clearMarker();
	uint8_t correction = 0;
	if(markerPosition >= divider)
	{
		correction = 11;
	}
	char margin = (73 - ((markerPosition * 11) + correction));
	drawMarker(margin, 39);
}

void moveMarker(uint8_t range)
{
	markerPosition++;
	if(markerPosition >= range) {markerPosition = 0;}
}

void changeValue(bool set, uint8_t position, uint16_t min, uint16_t max)
{
	uint16_t value;
	uint16_t valueToken = arrayToInt_chVal();
	uint16_t expo 		= 1;
	for(uint8_t i = 0; i < position; i++)
	{
		expo *= 10;
	}

	uint8_t overflowFlag = arrayToken[position] = (valueToken / expo) % 10; // określa wartość cyfry nad markerem wyboru

	if(set)
	{
		if(overflowFlag >= 9)
		{
			value = valueToken - (expo * 9);
		}
		else
		{
			value = valueToken + expo;
		}
	}
	else
	{
		if(overflowFlag <= 0)
		{
			value = valueToken + (expo * 9);
		}
		else
		{
			value = valueToken - expo;
		}
	}
	if(value < min){value = valueToken;}
	if(value > max){value = valueToken;}
	intToArray_chVal(value);
}

uint16_t arrayToInt_chVal(void)
{
	uint16_t expo;
	uint16_t value = 0;

	for(uint8_t i = 0; i < 5; i++)
	{
		if(!i){expo = 1;}
		else{expo *= 10;}
		value = value + (expo * arrayToken[i]);
	}
	return value;
}

void intToArray_chVal(uint16_t value)
{
	uint16_t expo;
	for(uint8_t i = 0; i < 5; i++)
	{
		if(!i){expo = 1;}
		else{expo *= 10;}
		arrayToken[i] = (value / expo) % 10;
	}
}

void drawMarker(uint8_t width, uint8_t height)
{
	for(uint8_t h = 0; h < 5; h++)
	{
		uint8_t w = 0;
		while(w <= (h * 2))
		{
			SSD1306_DrawPixel(((width - h) + w), height + h, 1);
			w++;
		}
	}
}
void clearMarker(void)
{
	SSD1306_DrawFilledRectangle(20, 39, 80, 5, 0);
}
void clearValue(void)
{
	SSD1306_DrawFilledRectangle(20, 20, 100, 18, 0);
}

void clearSettings(void)
{
	intToArray_chVal(CARCASS_MIN_TURNS);
	markerPosition = 0;
}

// uniwersalne
// -------------------------------------------------------------------------------------
void showLabelBar(char* label)
{
	SSD1306_DrawFilledRectangle(0, 0, 128, 16, 1);
	SSD1306_GotoXY(4, 4);
	SSD1306_Puts(label, &Font_7x10, 0);
}


void clearContent(void)
{
	SSD1306_DrawFilledRectangle(0, 18, 128, 46, 0);
}

void paginationBar(uint8_t pageBarWidth, uint8_t pageNo)
{
	uint8_t pageBarMargin = pageBarWidth * pageNo;
	SSD1306_DrawFilledRectangle(pageBarMargin, 18, pageBarWidth, 3, 1);
}

uint8_t countArray(Project * project)
{
	uint8_t count = 0;
	for(uint8_t i = 0; i<10; i++)
	{
		if(project->coil[i][0] > 0)
		{
			count++;
		}
	}
	return count;
}

// struktury
// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------
Project PeaveyC30_MAIN =
{
	"Peavey-C30-Main",
	"P-C30-M",
	"230V",
	"270,30V",
	0,
	"270V, 30V",
	1155,
	{
		{1100, 0.1}, {500, 0.55},
	}
};
Project PeaveyC30_SPK =
{
	"Peavey-C30-SPK",
	"PC30-S",
	"4xEL84",
	"16 Ohm",
	0,
	0,
	65,
	{
		{1200, 0.2}, {600, 0.65}, {300, 0.8},
	}
};
Project Test_Trafo =
{
	"Test",
	"Test",
	"2xEL84",
	"4-8-16",
	0,
	"4-8-16 Ohm",
	100,
	{
		{1300, 0.3}, {700, 0.75},
	}
};

Project Empty =
{
	"",
	"",
	"",
	"",
	0,
	0,
	0,
	{{}}
};

Project getProjectStructByID(uint8_t id)
{
	switch(id)
	{
		case 1:
			return PeaveyC30_MAIN;
		break;
		case 2:
			return PeaveyC30_SPK;
		break;
		case 3:
			return Test_Trafo;
		break;
	}
	return Empty;
}
