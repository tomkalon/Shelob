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

// UART
uint8_t usart2data; 					// znaki odbierane przez UART'a

// TIM2
volatile uint16_t 	encoderCount;				 	// aktualne polozenie enkodera
volatile uint16_t 	encoderCountPrev 	= 16000; 	// poprzednie polozenie enkodera

// TIM6
volatile uint8_t 	pressBtnCounter		= 0;	// licznik czasu wcisniecia przycisku
const 	 uint8_t	PRESS_BTN_TIME		= 15;	// dlugosc wcisniecia przycisku do zapisania danych

// TIM7

// MENU PRZYGOTOWANIA I KONFIGURACJI PROJEKTU
extern volatile uint8_t workStep;		// Wskazuje aktualny krok w ustawieniach
extern volatile uint8_t projectSelect;  // Wskazuje aktualnie wybrany projekt w menu wyboru projektow ( step 1)

#ifndef PROJECT_COUNT
#define PROJECT_COUNT 3
#endif

// GLOWNE ZMIENNE
volatile uint8_t width_MAIN, turns_MAIN, diameter_MAIN, speed_MAIN;

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
  /* USER CODE BEGIN 2 */
  // USART & TIMERS
  HAL_UART_Receive_IT(&huart2, &usart2data, 1);
  printUART((unsigned char*)"Running...");
  // display initialization
  SSD1306_Init();
  SSD1306_SetContrast(0);
  setTheme();
  __HAL_TIM_SET_COUNTER(&htim2, 32000);
  HAL_TIM_Encoder_Start_IT(&htim2, TIM_CHANNEL_ALL);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
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
/* UART */
// =========================================================================================
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART2)
	{
		if(usart2data == '\r')
		{
			printUART((uint8_t *) "Connected");
		}
		HAL_UART_Receive_IT(&huart2, &usart2data, 1);
	}
}

// =========================================================================================
/* TIMERS - ENCODER */
// =========================================================================================
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2)
	{
		uint8_t tim7BusyFlag = HAL_TIM_Base_GetState(&htim7);
		uint8_t encoderBusyFlag = HAL_TIM_Encoder_GetState(&htim7);
		if(tim7BusyFlag == 1)
		{
			if(encoderBusyFlag == 1)
			{
				HAL_TIM_Base_Start_IT(&htim7);
			}
		}
	}
}

// =========================================================================================
/* TIMERS - COUNTING */
// =========================================================================================
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	// ========================
	/* TIM6 - CLICK BUFFOR */
	// ========================
	if(htim->Instance == TIM6)
	{
		bool btnBusyFlag = HAL_GPIO_ReadPin(GPIOA, SET_BTN_Pin);
		HAL_TIM_Base_Stop_IT(&htim6);

		if(!btnBusyFlag)
		{
			HAL_TIM_Base_Start_IT(&htim6);
			switch(workStep)
			{
				case 2: // step 2
					pressBtnCounter++;
					if(pressBtnCounter > PRESS_BTN_TIME)
					{
						width_MAIN = arrayToInt_chVal();
						workStep = 3;
						pressBtnCounter = 0;
						HAL_TIM_Base_Stop_IT(&htim6);
						clearSettings();
						setTheme();
					}
				break;
				case 3: // step 3
					pressBtnCounter++;
					if(pressBtnCounter > PRESS_BTN_TIME)
					{
						turns_MAIN = arrayToInt_chVal();
						workStep = 4;
						pressBtnCounter = 0;
						HAL_TIM_Base_Stop_IT(&htim6);
						clearSettings();
						setTheme();
					}
				break;
				case 4: // step 4
					pressBtnCounter++;
					if(pressBtnCounter > PRESS_BTN_TIME)
					{
						diameter_MAIN = arrayToInt_chVal();
						workStep = 5;
						pressBtnCounter = 0;
						HAL_TIM_Base_Stop_IT(&htim6);
						clearSettings();
						setTheme();
					}
				break;
				case 5: // step 5
					pressBtnCounter++;
					if(pressBtnCounter > PRESS_BTN_TIME)
					{
						speed_MAIN = arrayToInt_chVal();
						workStep = 6;
						pressBtnCounter = 0;
						HAL_TIM_Base_Stop_IT(&htim6);
						clearSettings();
						setTheme();
					}
				break;
			}
		}
		if(btnBusyFlag)
		{
			pressBtnCounter = 0;
			switch(workStep)
			{
				case 1: // step 1
					if(projectSelect == 0)
					{
						workStep = 2;
					}
					else
					{
						workStep = 11;
					}
				setTheme();
				break;
				case 11: // step 11
					workStep = 1;
					setTheme();
				break;
				case 2: // step 2
					showValueScreen(CARCASS_WIDTH, VALUE_NO_CHANGING, 0, CONTI_RUN);
				break;
				case 3: // step 3
					showValueScreen(CARCASS_COIL_TURNS, VALUE_NO_CHANGING, 0, CONTI_RUN);
				break;
				case 4: // step 4
					showValueScreen(WINDING_DIAMETER, VALUE_NO_CHANGING, 0, CONTI_RUN);
				break;
				case 5: // step 5
					showValueScreen(WINDING_SPEED, VALUE_NO_CHANGING, 0, CONTI_RUN);
				break;
			}
		}
	}

	// ========================
	/* TIM7 - BUFFOR LEFT/RIGHT */
	// ========================
	if(htim->Instance == TIM7)
	{
		HAL_TIM_Base_Stop_IT(&htim7);
		encoderCount = (__HAL_TIM_GET_COUNTER(&htim2) / 2);
		bool direction;
		if(encoderCount != encoderCountPrev)
		{
			if(encoderCount > encoderCountPrev)
			{
				direction = 1;
			}
			else
			{
				direction = 0;
			}
			encoderCountPrev = encoderCount;
			switch(workStep)
			{
				case 1: // step 1
					if(!direction)
					{
						if(projectSelect > 0)
						{
							projectSelect--;
						}
					}
					else
					{
						if((projectSelect) < PROJECT_COUNT)
						{
							projectSelect++;
						}
					}
					setTheme();
				break;
				case 2: // step 2
					showValueScreen(CARCASS_WIDTH, VALUE_CHANGING, direction, CONTI_RUN);
				break;
				case 3: // step 3
					showValueScreen(CARCASS_COIL_TURNS, VALUE_CHANGING, direction, CONTI_RUN);
				break;
				case 4: // step 4
					showValueScreen(WINDING_DIAMETER, VALUE_CHANGING, direction, CONTI_RUN);
				break;
				case 5: // step 4
					showValueScreen(WINDING_SPEED, VALUE_CHANGING, direction, CONTI_RUN);
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
	uint8_t tim6BusyFlag = HAL_TIM_Base_GetState(&htim7);
	if(tim6BusyFlag == 1)
	{
		if(GPIO_Pin == SET_BTN_Pin)
		{
			HAL_TIM_Base_Start_IT(&htim6);
		}
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
