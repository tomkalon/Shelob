/*
 * ssd1306_adds.c
 *
 *  Created on: Apr 22, 2022
 *      Author: Tomasz Kaliński
 */

#include "ssd1306_adds.h"
#include "ssd1306_bitmaps.h"

// CONST
const uint8_t PROJECT_COUNT = 3; 		// liczba zapisanych projektów

// VAR
volatile uint8_t workStep		= 0;  	// Wskazuje aktualny krok w ustawieniach
volatile uint8_t projectSelect	= 0;  	// Wskazuje aktualnie wybrany projekt w menu wyboru projektow ( step 1)

uint8_t progressBarWidth		= 0; 	// szerokosc wskaznika stron
uint8_t progressBarStep			= 0; 	// polozenie wskaznika stron

//markery wyboru
int8_t markerPosition 				= 0; 			// polozenie wskaznika ustawianej wartosci
volatile int8_t arrayToken[5] 		= {0,0,0,0,0};	// ustawianie wartosci

// STRUCT
ProjectManager Details[4];
Step Settings[4];

void structInit(void)
{
	uint8_t i = 0;

	// STEP
	// ============================================================
	// STEP WIDTH
	Settings[i].minValue = CARCASS_MIN_WIDTH;
	Settings[i].setValue = 0;
	Settings[i].maxValue = CARCASS_MAX_WIDTH;
	Settings[i].digitsCount = 4;
	Settings[i].dotPosition = 1;
	i++;

	// STEP TURNS
	Settings[i].minValue = CARCASS_MIN_TURNS;
	Settings[i].setValue = 0;
	Settings[i].maxValue = CARCASS_MAX_TURNS;
	Settings[i].digitsCount = 4;
	Settings[i].dotPosition = 0;
	i++;

	// STEP DIAMETER
	Settings[i].minValue = WINDING_MIN_DIAMETER;
	Settings[i].setValue = 0;
	Settings[i].maxValue = WINDING_MAX_DIAMETER;
	Settings[i].digitsCount = 3;
	Settings[i].dotPosition = 2;
	i++;

	// STEP SPEED
	Settings[i].minValue = WINDING_MIN_SPEED;
	Settings[i].setValue = 0;
	Settings[i].maxValue = WINDING_MAX_SPEED;
	Settings[i].digitsCount = 1;
	Settings[i].dotPosition = 0;
	i=0;

	// PROJECT
	// ============================================================
	//PROJECT - PEAVEY CLASSIC 30 - MAIN TRANSFORMER
	Details[i].fullName		= "Peavey-C30-Main";
	Details[i].shortName	= "P-C30-M";
	Details[i].descShort_1	= "230V";
	Details[i].descShort_2	= "270,30V";
	Details[i].descFull_1	= "PRI: 230V";
	Details[i].descFull_2 	= "SEC: 230V, 30V";
	Details[i].width		= 1150;
	Details[i].turns[0]		= 1100;
	Details[i].diameter[0]	= 10;
	Details[i].turns[1]		= 900;
	Details[i].diameter[1]	= 15;
	Details[i].turns[2]		= 200;
	Details[i].diameter[2]	= 100;
	i++;

	//PROJECT - PEAVEY CLASSIC 30 - SPEAKER TRANSFORMER
	Details[i].fullName 	= "Peavey-C30-SPK";
	Details[i].shortName	= "P-C30-S";
	Details[i].descShort_1	= "4xEL84";
	Details[i].descShort_2	= "16 Ohms";
	Details[i].descFull_1	= "4xEL84 | 3.4k";
	Details[i].descFull_2 	= "OUT: 16 Ohms";
	Details[i].width		= 650;
	Details[i].turns[0]		= 900;
	Details[i].diameter[0]	= 20;
	Details[i].turns[1]		= 600;
	Details[i].diameter[1]	= 120;
	i++;

	//PROJECT - TEST
	Details[i].fullName 	= "TEST-FULL";
	Details[i].shortName	= "TEST-SH";
	Details[i].descShort_1	= "2XEL84";
	Details[i].descShort_2	= "4-8-16";
	Details[i].descFull_1	= "2XEL84 | 8.2k";
	Details[i].descFull_2 	= "OUT: 4-8-16 Ohms";
	Details[i].width		= 800;
	Details[i].turns[0]		= 2500;
	Details[i].diameter[0]	= 250;
	Details[i].turns[1]		= 10;
	Details[i].diameter[1]	= 1;
	Details[i].turns[2]		= 2500;
	Details[i].diameter[2]	= 1;
	Details[i].turns[3]		= 10;
	Details[i].diameter[3]	= 250;
}

void setTheme(void)
{
	clearContent();
	switch (workStep)
	{
		case 0: // wyświetla logo
			SSD1306_DrawBitmap(0, 0, IMG_LOGO, 128, 64, 1);
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
			ProjectManager Handler = Details[projectSelect - 1];
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
			showLabelBar(DISP_SET_SPEED_LABEL);
			showValueScreen(WINDING_SPEED, 0, 0, FIRST_RUN);
			break;
		case 6: // podsumowanie
			showLabelBar(DISP_SET_SUMMARY_LABEL);
			break;
	}
	SSD1306_UpdateScreen();
}

// wybór projektu - 1
// -------------------------------------------------------------------------------------
void showProjectSelectMenu(void)
{
	uint8_t leftMargin = 5; // left margin
	uint8_t renderingBlock = projectSelect - 1;
	uint8_t renderingStep = 0;

		if(projectSelect < 2)
		{
			newTaskElement();
			ProjectManager Handler = Details[0];
			showProjectElements(&Handler, 69);
		}
		else
		{
			while(renderingStep < 2)
			{
				if(!renderingStep)
				{
					if(projectSelect % 2)
					{
						leftMargin = 68;
					}
					else
					{
						leftMargin = 5;
					}
					ProjectManager Handler = Details[renderingBlock];
					showProjectElements(&Handler, leftMargin);
				}
				else
				{
					if(projectSelect % 2)
					{
						renderingBlock--;
						leftMargin = 5;
					}
					else
					{
						renderingBlock++;
						leftMargin = 68;
					}
					ProjectManager Handler = Details[renderingBlock];
					showProjectElements(&Handler, leftMargin);
				}
				renderingStep++;
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

void showProjectElements(ProjectManager * details, uint8_t margin)
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
	SSD1306_Puts(details->shortName, &Font_7x10, color);
	SSD1306_GotoXY(margin, 40);
	SSD1306_Puts(details->descShort_1, &Font_7x10, color);
	SSD1306_GotoXY(margin, 51);
	SSD1306_Puts(details->descShort_2, &Font_7x10, color);
}

// szczegoly projektu - 11
// -------------------------------------------------------------------------------------

void showProjectDetails(ProjectManager * details)
{

	char width[10];
	sprintf(width, "%i.%imm", details->width / 10, details->width % 10);
	showLabelBar(details->fullName);
	SSD1306_GotoXY(0, 20);
	SSD1306_Puts(WIDTH_LABEL, &Font_7x10, 1);
	SSD1306_GotoXY(70, 20);
	SSD1306_Puts(width, &Font_7x10, 1);
	SSD1306_GotoXY(0, 31);
	SSD1306_Puts(TASK_NO_LABEL, &Font_7x10, 1);
	SSD1306_GotoXY(70, 31);
	uint8_t count = countArray(details);
	sprintf(width, "%i", count);
	SSD1306_Puts(width, &Font_7x10, 1);
	SSD1306_GotoXY(0, 42);
	SSD1306_Puts(details->descFull_1, &Font_7x10, 1);
	SSD1306_GotoXY(0, 53);
	SSD1306_Puts(details->descFull_2, &Font_7x10, 1);
}

// ustawianie wartosci - 2++
// -------------------------------------------------------------------------------------
void showValueScreen(VALUE_TYPE type, uint8_t runMode, bool direction, uint8_t runCount)
{
	char valueLettering[10];
	if(runCount)
	{
		markerPosition = 0;
		switch (type)
		{
			case 0: SSD1306_DrawBitmap(0, 0, IMG_WIDTH, 128, 64, 1);
				break;
			case 1: SSD1306_DrawBitmap(0, 0, IMG_TURNS, 128, 64, 1);
				break;
			case 2: SSD1306_DrawBitmap(0, 0, IMG_DIAMETER, 128, 64, 1);
				break;
			case 3: SSD1306_DrawBitmap(0, 0, IMG_SPEED, 128, 64, 1);
				break;
		}
		if(runCount == FIRST_RUN) intToArray_chVal(Settings[type].minValue);
		else if(runCount == EDIT_RUN) intToArray_chVal(Settings[type].setValue);
	}
	else
	{
		if(runMode) changeValue(direction, markerPosition, Settings[type].minValue, Settings[type].maxValue);
		else moveMarker(Settings[type].digitsCount);
	}
	setMarkerPosition(Settings[type].dotPosition);
	switch (type)
	{
		case 0: sprintf(valueLettering, "%i%i%i.%imm", arrayToken[3], arrayToken[2], arrayToken[1], arrayToken[0]);
			break;
		case 1: sprintf(valueLettering, "%i%i%i%izw.", arrayToken[3], arrayToken[2], arrayToken[1], arrayToken[0]);
			break;
		case 2: sprintf(valueLettering, "~%i.%i%imm", arrayToken[2], arrayToken[1], arrayToken[0]);
			break;
		case 3: sprintf(valueLettering, "   %i", arrayToken[0]);
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

uint8_t countArray(ProjectManager * details)
{
	uint8_t count = 0;
	for(uint8_t i = 0; i<10; i++)
	{
		if(details->turns[i] > 0)
		{
			count++;
		}
	}
	return count;
}
