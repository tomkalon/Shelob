/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ssd1306_adds.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

// NUMBER OF SAVED PROJECTS
extern const uint8_t G_PROJECT_COUNT;

// MAIN INTERFACE VAR's
extern volatile uint8_t g_workStep;  		// Wskazuje aktualny krok w ustawieniach
extern volatile uint8_t g_projectSelect;  	// Wskazuje aktualnie wybrany projekt w menu wyboru projektow ( step 1)
extern volatile uint8_t g_selector;			// zmienna do wskazania określonej opcji w menu wyboru
extern volatile bool g_correctionFlag;		// informuje, czy ustawienia są wpisywane, czy poprawiane

// TIM2 / ENCODER - LEFT/RIGHT
volatile uint16_t encoderCount				= 0;		// aktualne polozenie enkodera
volatile uint16_t encoderCountPrev 			= 16000; 	// poprzednie polozenie enkodera

// TIM6 / BUTTON
static volatile uint8_t pressBtnCounter		= 0;	// licznik czasu wcisniecia przycisku

// MAIN VAR
volatile uint16_t g_width_MAIN, g_turns_MAIN, g_diameter_MAIN, g_speed_MAIN;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_TIM21_Init();
  MX_TIM22_Init();
  /* USER CODE BEGIN 2 */

  // structures initialization
  Structures_Init();

  // display initialization
  SSD1306_Init();
  SSD1306_SetContrast(0);
  Set_Theme();

  // timer TIM2 Encoder
  __HAL_TIM_SET_COUNTER(&TIMER_ENCODER, 32000);
  HAL_TIM_Encoder_Start_IT(&TIMER_ENCODER, TIM_CHANNEL_ALL);

  bool workStart = 0; 					// określa zakończenie konfiguracji wstępnej i start pracy

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if (workStart)
	  {
		  //Steppers_Control_Init(g_width_MAIN, g_turns_MAIN, g_diameter_MAIN, g_speed_MAIN);
	  }

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_SYSCLK;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* **********************************************IRQ************************************* */
// =========================================================================================
/* TIMERS - ENCODER */
// =========================================================================================
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM2)
	{
		uint8_t tim7BusyFlag = HAL_TIM_Base_GetState(&TIMER_ENCODER_BUFFER);
		uint8_t encoderBusyFlag = HAL_TIM_Encoder_GetState(&TIMER_ENCODER);

		if (tim7BusyFlag == 1 && encoderBusyFlag == 1) HAL_TIM_Base_Start_IT(&TIMER_ENCODER_BUFFER);
	}
}

// =========================================================================================
/* TIMERS - COUNTING */
// =========================================================================================
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	// ========================
	/* TIM21 */
	// ========================
	if (htim->Instance == TIM21)
	{
	}
	// ========================
	/* TIM22 */
	// ========================
	if (htim->Instance == TIM22)
	{
		HAL_GPIO_TogglePin(C_LED_GPIO_Port, C_LED_Pin);
	}
	// ========================
	/* TIM6 - CLICK/BTN BUFFER */
	// ========================
	if (htim->Instance == TIM6)
	{
		bool btnBusyFlag = HAL_GPIO_ReadPin(GPIOA, MAIN_BTN_Pin);

		HAL_TIM_Base_Stop_IT(&TIMER_BUTTON_BUFFER);
		if (!btnBusyFlag)
		{
			HAL_TIM_Base_Start_IT(&TIMER_BUTTON_BUFFER);
			if ((g_workStep >= 2 && g_workStep <=61)) pressBtnCounter++;
			if (pressBtnCounter > PRESS_BTN_TIME)
			{
				HAL_TIM_Base_Stop_IT(&TIMER_BUTTON_BUFFER);
				bool exception = 0;
				switch (g_workStep)
				{
					case STEP_PROJECT_DETAILS:
						exception = 1;
						g_workStep = STEP_PROJECT_TASKS_LIST;
						break;
					case STEP_PROJECT_TASKS_LIST:
						exception = 1;
						g_workStep = STEP_SELECT_PROJECT;
						break;
					case STEP_WIDTH_SET:
						g_width_MAIN = Array_To_Int_Change_Value();
						Save_Set_Value(g_width_MAIN);
						break;
					case STEP_TURNS_SET:
						g_turns_MAIN = Array_To_Int_Change_Value();
						Save_Set_Value(g_turns_MAIN);
						break;
					case STEP_DIAMETER_SET:
						g_diameter_MAIN = Array_To_Int_Change_Value();
						Save_Set_Value(g_diameter_MAIN);
						break;
					case STEP_SPEED_SET:
						g_speed_MAIN = Array_To_Int_Change_Value();
						Save_Set_Value(g_speed_MAIN);
						break;
					case STEP_CORRECTNES_QUERY:
						exception = 1;
						if (!g_selector) g_workStep = STEP_START_POSITION_SET;
						else
						{
							g_workStep = STEP_WIDTH_SET;
							g_correctionFlag = 1;
						}
						g_selector = 0;
						break;
				}
				if (!exception)
				{
					g_workStep++;

				}
				pressBtnCounter = 0;
				Set_Theme();
			}
		}
		if (btnBusyFlag)
		{
			pressBtnCounter = 0;
			switch (g_workStep)
			{
				case STEP_SELECT_PROJECT:
					if (g_projectSelect == 0) g_workStep = STEP_WIDTH_SET;
					else g_workStep = STEP_PROJECT_DETAILS;
					Set_Theme();
					break;
				case STEP_PROJECT_DETAILS:
					g_workStep = STEP_SELECT_PROJECT;
					Set_Theme();
					break;
				case STEP_WIDTH_SET:
					Show_Value_Screen(VAL_TYPE_CARCASS_WIDTH, VALUE_CHANGE_FLAG_NO, 0, RUN_FLAG_CONTI);
					break;
				case STEP_TURNS_SET:
					Show_Value_Screen(VAL_TYPE_CARCASS_COIL_TURNS, VALUE_CHANGE_FLAG_NO, 0, RUN_FLAG_CONTI);
					break;
				case STEP_DIAMETER_SET:
					Show_Value_Screen(VAL_TYPE_WINDING_DIAMETER, VALUE_CHANGE_FLAG_NO, 0, RUN_FLAG_CONTI);
					break;
				case STEP_SPEED_SET:
					Show_Value_Screen(VAL_TYPE_WINDING_SPEED, VALUE_CHANGE_FLAG_NO, 0, RUN_FLAG_CONTI);
					break;
				case STEP_SUMMARY:
					g_workStep = STEP_CORRECTNES_QUERY;
					Set_Theme();
					break;
			}
		}
	}

	// ========================
	/* TIM7 - BUFFER LEFT/RIGHT */
	// ========================
	if (htim->Instance == TIM7)
	{
		bool direction;

		HAL_TIM_Base_Stop_IT(&TIMER_ENCODER_BUFFER);
		encoderCount = (__HAL_TIM_GET_COUNTER(&TIMER_ENCODER) / 2);
		if (encoderCount != encoderCountPrev)
		{
			if (encoderCount > encoderCountPrev) direction = 1;
			else direction = 0;
			encoderCountPrev = encoderCount;
			switch (g_workStep)
			{
				case STEP_SELECT_PROJECT:
					if (!direction)
					{
						if (g_projectSelect > 0) g_projectSelect--;
					}
					else
					{
						if ((g_projectSelect) < G_PROJECT_COUNT) g_projectSelect++;
					}
					Set_Theme();
					break;
				case STEP_PROJECT_TASKS_LIST:
					break;
				case STEP_WIDTH_SET:
					Show_Value_Screen(VAL_TYPE_CARCASS_WIDTH, VALUE_CHANGE_FLAG_YES, direction, RUN_FLAG_CONTI);
					break;
				case STEP_TURNS_SET:
					Show_Value_Screen(VAL_TYPE_CARCASS_COIL_TURNS, VALUE_CHANGE_FLAG_YES, direction, RUN_FLAG_CONTI);
					break;
				case STEP_DIAMETER_SET:
					Show_Value_Screen(VAL_TYPE_WINDING_DIAMETER, VALUE_CHANGE_FLAG_YES, direction, RUN_FLAG_CONTI);
					break;
				case STEP_SPEED_SET:
					Show_Value_Screen(VAL_TYPE_WINDING_SPEED, VALUE_CHANGE_FLAG_YES, direction, RUN_FLAG_CONTI);
					break;
				case STEP_CORRECTNES_QUERY:
					Show_Summary_Correctness_Query(direction, RUN_FLAG_CONTI);
					break;

			}
		}
	}
}

// =========================================================================================
/* GPIO */
// =========================================================================================
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	uint8_t tim6BusyFlag = HAL_TIM_Base_GetState(&TIMER_BUTTON_BUFFER);
	uint8_t tim7BusyFlag = HAL_TIM_Base_GetState(&TIMER_ENCODER_BUFFER);
	if ((tim6BusyFlag == 1) && (tim7BusyFlag == 1))
	{
		if (GPIO_Pin == MAIN_BTN_Pin) HAL_TIM_Base_Start_IT(&TIMER_BUTTON_BUFFER);
	}
}
// =========================================================================================
// -----------------------------------------------------------------------------------------


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
