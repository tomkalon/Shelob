/*
 * ssd1306_adds.c
 *
 *  Created on: Apr 22, 2022
 *      Author: Tomasz Kaliński
 */

#include "ssd1306_adds.h"
#include "ssd1306_bitmaps.h"

// CONST
const uint8_t G_PROJECT_COUNT = 3; 		// liczba zapisanych projektów

// EXTERN
extern volatile uint16_t g_width_MAIN, g_turns_MAIN, g_diameter_MAIN, g_speed_MAIN;

// VAR
volatile uint8_t g_workStep			= 0;  	// Wskazuje aktualny krok w ustawieniach
volatile uint8_t g_projectSelect	= 0;  	// Wskazuje aktualnie wybrany projekt w menu wyboru projektow ( step 1)
volatile uint8_t g_correctionFlag 	= 0;	// informuje, czy dane edytowane w trybie poprawiania
volatile uint8_t g_taskStep			= 0;	// wybierane zadanie danego projektu

uint8_t progressBarWidth		= 0; 	// szerokosc wskaznika stron
uint8_t progressBarStep			= 0; 	// polozenie wskaznika stron

// CHANGE VALUE
int8_t markerPosition 			= 0; 			// polozenie wskaznika ustawianej wartosci
volatile int8_t arrayToken[4] 	= {0,0,0,0};	// ustawianie wartosci

// YES/NO QUERY
volatile uint8_t g_selector = 0;

// STRUCT
ProjectManager Details[4];
Step Settings[4];

void Structures_Init(void)
{
	uint8_t i = 0;

	// STEPS
	// ============================================================
	// STEP WIDTH
	Settings[i].minValue 	= VAL_RNG_CARCASS_MIN_WIDTH;
	Settings[i].setValue	= 0;
	Settings[i].maxValue 	= VAL_RNG_CARCASS_MAX_WIDTH;
	Settings[i].digitsCount = 4;
	Settings[i].dotPosition = 1;
	i++;

	// STEP TURNS
	Settings[i].minValue 	= VAL_RNG_CARCASS_MIN_TURNS;
	Settings[i].setValue 	= 0;
	Settings[i].maxValue	= VAL_RNG_CARCASS_MAX_TURNS;
	Settings[i].digitsCount = 4;
	Settings[i].dotPosition = 0;
	i++;

	// STEP DIAMETER
	Settings[i].minValue 	= VAL_RNG_WINDING_MIN_DIAMETER;
	Settings[i].setValue 	= 0;
	Settings[i].maxValue 	= VAL_RNG_WINDING_MAX_DIAMETER;
	Settings[i].digitsCount = 3;
	Settings[i].dotPosition = 2;
	i++;

	// STEP SPEED
	Settings[i].minValue 	= VAL_RNG_WINDING_MIN_SPEED;
	Settings[i].setValue 	= 0;
	Settings[i].maxValue 	= VAL_RNG_WINDING_MAX_SPEED;
	Settings[i].digitsCount = 1;
	Settings[i].dotPosition = 0;
	i=0;

	// PROJECT
	// ============================================================
	// PROJECT - PEAVEY CLASSIC 30 - MAIN TRANSFORMER
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
	Details[i].taskCount	= Count_Array(&Details[i]);
	i++;

	// PROJECT - PEAVEY CLASSIC 30 - SPEAKER TRANSFORMER
	Details[i].fullName		= "Peavey-C30-SPK";
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
	Details[i].taskCount	= Count_Array(&Details[i]);
	i++;

	// PROJECT - TEST
	Details[i].fullName		= "TEST-FULL";
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
	Details[i].taskCount	= Count_Array(&Details[i]);
}

void Set_Theme(void)
{
	Clear_Content();
	switch (g_workStep)
	{
		case STEP_LOGO_DISP:
			SSD1306_DrawBitmap(0, 0, IMG_LOGO, 128, 64, 1);
			SSD1306_UpdateScreen();
			HAL_Delay(1000);
			SSD1306_Clear();
			g_workStep++;
			Set_Theme();
			break;
		case STEP_SELECT_PROJECT:
			Show_Label_Bar(DISP_PROJECT_LABEL);
			progressBarWidth = (128 / ((G_PROJECT_COUNT + 1) / 2) + ((G_PROJECT_COUNT + 1) % 2));
			progressBarStep = g_projectSelect / 2;
			Pagination_Bar(progressBarWidth, progressBarStep);
			Show_Project_Select_Menu();
			break;
		case STEP_PROJECT_DETAILS:
			Show_Project_Details(&Details[g_projectSelect - 1], 0);
			break;
		case STEP_PROJECT_TASKS_LIST:
			Clear_Content();
			Show_Project_Details(&Details[g_projectSelect - 1], 1);
			break;
		case STEP_WIDTH_SET:
			Show_Label_Bar(DISP_SET_WIDTH_LABEL);
			if (!g_correctionFlag) Show_Value_Screen(VAL_TYPE_CARCASS_WIDTH, 0, 0, RUN_FLAG_FIRST);
			else Show_Value_Screen(VAL_TYPE_CARCASS_WIDTH, 0, 0, RUN_FLAG_EDIT);
			break;
		case STEP_TURNS_SET:
			Show_Label_Bar(DISP_SET_TURNS_LABEL);
			if (!g_correctionFlag) Show_Value_Screen(VAL_TYPE_CARCASS_COIL_TURNS, 0, 0, RUN_FLAG_FIRST);
			else Show_Value_Screen(VAL_TYPE_CARCASS_COIL_TURNS, 0, 0, RUN_FLAG_EDIT);
			break;
		case STEP_DIAMETER_SET:
			Show_Label_Bar(DISP_SET_DIAMETER_LABEL);
			if (!g_correctionFlag) Show_Value_Screen(VAL_TYPE_WINDING_DIAMETER, 0, 0, RUN_FLAG_FIRST);
			else Show_Value_Screen(VAL_TYPE_WINDING_DIAMETER, 0, 0, RUN_FLAG_EDIT);
			break;
		case STEP_SPEED_SET:
			Show_Label_Bar(DISP_SET_SPEED_LABEL);
			if (!g_correctionFlag) Show_Value_Screen(VAL_TYPE_WINDING_SPEED, 0, 0, RUN_FLAG_FIRST);
			else Show_Value_Screen(VAL_TYPE_WINDING_SPEED, 0, 0, RUN_FLAG_EDIT);
			break;
		case STEP_SUMMARY:
			Show_Label_Bar(DISP_SET_SUMMARY_LABEL);
			Show_Summary();
			break;
		case STEP_CORRECTNES_QUERY:
			Show_Label_Bar(DISP_CORRECTNESS_QUERY);
			Show_Summary_Correctness_Query(0, RUN_FLAG_FIRST);
			break;
	}
	SSD1306_UpdateScreen();
}

// wybór projektu - 1
// -------------------------------------------------------------------------------------
void Show_Project_Select_Menu(void)
{
	uint8_t leftMargin 		= ALIGN_LEFT; // left margin
	uint8_t renderingBlock 	= g_projectSelect - 1;
	uint8_t renderingStep 	= 0;

	if (g_projectSelect < 2)
	{
		New_Task_Element();
		ProjectManager ProjectToken = Details[0];
		Show_Project_Elements(&ProjectToken, 68);
	}
	else
	{
		while(renderingStep < 2)
		{
			if (!renderingStep)
			{
				if (g_projectSelect % 2) leftMargin = ALIGN_RIGHT;
				else leftMargin = ALIGN_LEFT;
				ProjectManager ProjectToken = Details[renderingBlock];
				Show_Project_Elements(&ProjectToken, leftMargin);
			}
			else
			{
				if (g_projectSelect % 2)
				{
					renderingBlock--;
					leftMargin = ALIGN_LEFT;
				}
				else
				{
					renderingBlock++;
					leftMargin = ALIGN_RIGHT;
				}
				ProjectManager ProjectToken = Details[renderingBlock];
				Show_Project_Elements(&ProjectToken, leftMargin);
			}
			renderingStep++;
		}
	}
}

void New_Task_Element(void)
{
	bool color = 0;

	if ((g_projectSelect + 3) % 2)
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

void Show_Project_Elements(ProjectManager * details, uint8_t margin)
{
	bool color = 0;

	color = Show_Select_Boxes(margin, g_projectSelect);
	margin += 4;
	SSD1306_GotoXY(margin, 29);
	SSD1306_Puts(details->shortName, &Font_7x10, color);
	SSD1306_GotoXY(margin, 40);
	SSD1306_Puts(details->descShort_1, &Font_7x10, color);
	SSD1306_GotoXY(margin, 51);
	SSD1306_Puts(details->descShort_2, &Font_7x10, color);
}

// project details - 11
// -------------------------------------------------------------------------------------

void Show_Project_Details(ProjectManager * details, bool list)
{
	char width[10];

	sprintf(width, "%i.%imm", details->width / 10, details->width % 10);
	Show_Label_Bar(details->fullName);
	SSD1306_GotoXY(0, 20);
	SSD1306_Puts(DISP_DETAIL_WIDTH_LABEL, &Font_7x10, 1);
	SSD1306_GotoXY(70, 20);
	SSD1306_Puts(width, &Font_7x10, 1);
	if (!list)
	{
		SSD1306_GotoXY(0, 31);
		SSD1306_Puts(DISP_DETAIL_TASK_COUNT_LABEL, &Font_7x10, 1);
		SSD1306_GotoXY(70, 31);
		uint8_t count = details->taskCount;
		sprintf(width, "%i", count);
		SSD1306_Puts(width, &Font_7x10, 1);
		SSD1306_GotoXY(0, 42);
		SSD1306_Puts(details->descFull_1, &Font_7x10, 1);
		SSD1306_GotoXY(0, 53);
		SSD1306_Puts(details->descFull_2, &Font_7x10, 1);
	}
	else Show_Project_Tasks(details);
}

// project tasks list - 12
// -------------------------------------------------------------------------------------

void Show_Project_Tasks(ProjectManager * details)
{
	char valueToken[10];

	SSD1306_GotoXY(0, 31);
	SSD1306_Puts(DISP_DETAIL_TURNS_LABEL, &Font_7x10, 1);
	SSD1306_GotoXY(70, 31);
	sprintf(valueToken, "%i", details->turns[g_taskStep]);
	SSD1306_Puts(valueToken, &Font_7x10, 1);
	SSD1306_GotoXY(0, 42);
	SSD1306_Puts(DISP_DETAIL_DIAMETER_LABEL, &Font_7x10, 1);
	SSD1306_GotoXY(70, 42);
	sprintf(valueToken, "%i", details->diameter[g_taskStep]);
	SSD1306_Puts(valueToken, &Font_7x10, 1);
	SSD1306_GotoXY(0, 53);
}

// ustawianie wartosci - 2++
// -------------------------------------------------------------------------------------
void Show_Value_Screen(VALUE_TYPE type, uint8_t runMode, bool direction, uint8_t runCount)
{
	char valueLettering[20];

	if (runCount)
	{
		markerPosition = 0;
		switch (type)
		{
			case VAL_TYPE_CARCASS_WIDTH: SSD1306_DrawBitmap(0, 0, IMG_WIDTH, 128, 64, 1);
				break;
			case VAL_TYPE_CARCASS_COIL_TURNS: SSD1306_DrawBitmap(0, 0, IMG_TURNS, 128, 64, 1);
				break;
			case VAL_TYPE_WINDING_DIAMETER: SSD1306_DrawBitmap(0, 0, IMG_DIAMETER, 128, 64, 1);
				break;
			case VAL_TYPE_WINDING_SPEED: SSD1306_DrawBitmap(0, 0, IMG_SPEED, 128, 64, 1);
				break;
		}
		if (runCount == RUN_FLAG_FIRST) Int_To_Array_Change_Value(Settings[type].minValue);
		else if (runCount == RUN_FLAG_EDIT) Int_To_Array_Change_Value(Settings[type].setValue);
	}
	else
	{
		if (runMode) Change_Value(direction, markerPosition, Settings[type].minValue, Settings[type].maxValue);
		else Move_Marker(Settings[type].digitsCount);
	}
	Set_Marker_Position(Settings[type].dotPosition);
	switch (type)
	{
		case VAL_TYPE_CARCASS_WIDTH: sprintf(valueLettering, "%i%i%i.%imm", arrayToken[3], arrayToken[2], arrayToken[1], arrayToken[0]);
			break;
		case VAL_TYPE_CARCASS_COIL_TURNS: sprintf(valueLettering, "%i%i%i%izw.", arrayToken[3], arrayToken[2], arrayToken[1], arrayToken[0]);
			break;
		case VAL_TYPE_WINDING_DIAMETER: sprintf(valueLettering, "~%i.%i%imm", arrayToken[2], arrayToken[1], arrayToken[0]);
			break;
		case VAL_TYPE_WINDING_SPEED: sprintf(valueLettering, "   %i", arrayToken[0]);
			break;
	}
	Clear_Value();
	SSD1306_GotoXY(25, 20);
	SSD1306_Puts(valueLettering, &Font_11x18, 1);
	SSD1306_UpdateScreen();
}

void Set_Marker_Position(uint8_t divider)
{
	uint8_t correction = 0;

	Clear_Changing_Value_Marker();
	if (markerPosition >= divider)
	{
		correction = 11;
	}
	char margin = (73 - ((markerPosition * 11) + correction));
	Draw_Changing_Value_Marker(margin, 39);
}

void Move_Marker(uint8_t range)
{
	markerPosition++;
	if(markerPosition >= range) {markerPosition = 0;}
}

void Change_Value(bool set, uint8_t position, uint16_t min, uint16_t max)
{
	uint16_t value;
	uint16_t valueToken = Array_To_Int_Change_Value();
	uint16_t expo 		= 1;

	for (uint8_t i = 0; i < position; i++) {expo *= 10;}
	uint8_t overflowFlag = arrayToken[position] = (valueToken / expo) % 10; // określa wartość cyfry nad markerem wyboru
	if (set)
	{
		if (overflowFlag >= 9) value = valueToken - (expo * 9);
		else value = valueToken + expo;
	}
	else
	{
		if (overflowFlag <= 0) value = valueToken + (expo * 9);
		else value = valueToken - expo;
	}
	if (value < min){value = valueToken;}
	if (value > max){value = valueToken;}
	Int_To_Array_Change_Value(value);
}

uint16_t Array_To_Int_Change_Value(void)
{
	volatile uint16_t expo;
	uint16_t value = 0;

	for (uint8_t i = 0; i <= 3; i++)
	{
		if (!i){expo = 1;}
		else {expo *= 10;}
		value = value + (expo * arrayToken[i]);
	}
	return value;
}

void Int_To_Array_Change_Value(uint16_t value)
{
	volatile uint16_t expo;

	for (uint8_t i = 0; i <= 3; i++)
	{
		if (!i){expo = 1;}
		else {expo *= 10;}
		arrayToken[i] = (value / expo) % 10;
	}
}

void Save_Set_Value(uint16_t value)
{
	Settings[g_workStep - 2].setValue = value;
}

void Draw_Changing_Value_Marker(uint8_t width, uint8_t height)
{
	for (uint8_t h = 0; h < 5; h++)
	{
		uint8_t w = 0;
		while (w <= (h * 2))
		{
			SSD1306_DrawPixel(((width - h) + w), height + h, 1);
			w++;
		}
	}
}
void Clear_Changing_Value_Marker(void)
{
	SSD1306_DrawFilledRectangle(20, 39, 80, 5, 0);
}
void Clear_Value(void)
{
	SSD1306_DrawFilledRectangle(20, 20, 100, 18, 0);
}

// summary - 6
// -------------------------------------------------------------------------------------
void Show_Summary(void)
{
	char width[10], turns[12],diameter[15], speed[10];
	uint8_t diameterArr[4];
	uint16_t expo = 0;

	for (uint8_t i = 0; i <= 3; i++)
	{
		if (!i) expo = 1;
		else expo *= 10;
		diameterArr[i] = (g_diameter_MAIN / expo) % 10;
	}
	sprintf(width, "%i.%i mm", g_width_MAIN / 10, g_width_MAIN % 10);
	sprintf(turns, " %i zw.", g_turns_MAIN);
	sprintf(diameter, " %i.%i%i mm", diameterArr[2], diameterArr[1], diameterArr[0]);
	sprintf(speed, " %i", g_speed_MAIN);
	SSD1306_GotoXY(0, 20);
	SSD1306_Puts(DISP_DETAIL_WIDTH_LABEL, &Font_7x10, 1);
	SSD1306_GotoXY(70, 20);
	SSD1306_Puts(width, &Font_7x10, 1);
	SSD1306_GotoXY(0, 31);
	SSD1306_Puts(DISP_DETAIL_TURNS_LABEL, &Font_7x10, 1);
	SSD1306_GotoXY(42, 31);
	SSD1306_Puts(turns, &Font_7x10, 1);
	SSD1306_GotoXY(0, 42);
	SSD1306_Puts(DISP_DETAIL_DIAMETER_LABEL, &Font_7x10, 1);
	SSD1306_GotoXY(63, 42);
	SSD1306_Puts(diameter, &Font_7x10, 1);
	SSD1306_GotoXY(0, 53);
	SSD1306_Puts(DISP_DETAIL_SPEED_LABEL, &Font_7x10, 1);
	SSD1306_GotoXY(63, 53);
	SSD1306_Puts(speed, &Font_7x10, 1);
}

// correctness query - 61
// -------------------------------------------------------------------------------------
void Show_Summary_Correctness_Query(bool direction, uint8_t runCount)
{
	bool color = 0;

	Clear_Content();
	if (runCount == RUN_FLAG_CONTI)
	{
		if (direction) g_selector++;
		else g_selector--;
		if (g_selector > 1 && g_selector < 10) g_selector = 1;
		if (g_selector > 10) g_selector = 0;
	}
	color = Show_Select_Boxes(ALIGN_LEFT, g_selector);
	SSD1306_GotoXY(18, 29);
	SSD1306_Puts(DISP_TEXT_YES, &Font_11x18, color);
	SSD1306_GotoXY(28, 50);
	SSD1306_Puts(DISP_TEXT_OK, &Font_7x10, color);
	color = Show_Select_Boxes(ALIGN_RIGHT, g_selector);
	SSD1306_GotoXY(80, 29);
	SSD1306_Puts(DISP_TEXT_NO, &Font_11x18, color);
	SSD1306_GotoXY(76, 50);
	SSD1306_Puts(DISP_TEXT_CORRECT, &Font_7x10, color);
	if (runCount == RUN_FLAG_CONTI) SSD1306_UpdateScreen();
}

bool Show_Select_Boxes(uint8_t margin, uint8_t pointer)
{
	pointer += 3;
	if (margin == ALIGN_LEFT)
	{
		if ((pointer) % 2)
		{
			SSD1306_DrawFilledRectangle(margin, 25, 56, 47, 1);
			return 0;
		}
		else
		{
			SSD1306_DrawRectangle(margin, 25, 56, 47, 1);
			return 1;
		}
	}
	else
	{
		if (pointer % 2)
		{

			SSD1306_DrawRectangle(margin, 25, 56, 47, 1);
			return 1;
		}
		else
		{
			SSD1306_DrawFilledRectangle(margin, 25, 56, 47, 1);
			return 0;
		}
	}
}

// uniwersalne
// -------------------------------------------------------------------------------------
void Show_Label_Bar(char* label)
{
	SSD1306_DrawFilledRectangle(0, 0, 128, 16, 1);
	SSD1306_GotoXY(4, 4);
	SSD1306_Puts(label, &Font_7x10, 0);
}


void Clear_Content(void)
{
	SSD1306_DrawFilledRectangle(0, 18, 128, 46, 0);
}

void Pagination_Bar(uint8_t pageBarWidth, uint8_t pageNo)
{
	uint8_t pageBarMargin = pageBarWidth * pageNo;

	SSD1306_DrawFilledRectangle(pageBarMargin, 18, pageBarWidth, 3, 1);
}

uint8_t Count_Array(ProjectManager * details)
{
	uint8_t count = 0;

	for (uint8_t i = 0; i<10; i++)
	{
		if (details->turns[i] > 0) count++;
	}
	return count;
}
