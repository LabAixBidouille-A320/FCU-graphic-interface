
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  * Cet exemple teste l'affichage de la partie gauche du FCU
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l4xx_hal.h"
#include "fcu_functions.h"

/* USER CODE BEGIN Includes */

#define LOW 0
#define HIGH 1

#define swGauche0 0
#define swGauche1 1
#define swCentre0 2
#define swCentre1 3
#define swDroite0 4
#define swDroite1 5

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

int data_receive;
char Rx_indx, Rx_Buffer[100], buffer[100], inputByte[15];
uint8_t noOp = 0; len = 0, Transfer_cplt = 0, Rx_data[2], spidata[16], dataSwitch[6];

int swGauche0A = 0; swGauche0B = 0; swGauche1A = 0; swGauche1B = 0; swCentre0A = 0; swCentre0B = 0;
int swCentre1A = 0; swCentre1B = 0; swDroite0A = 0; swDroite0B = 0; swDroite1A = 0; swDroite1B = 0;
uint8_t Gauche = 0; uint8_t Centre = 0; uint8_t Droite = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_SPI3_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

// Afficher sur le port série
void Serial_Transmit_Str(char* str)
{
	HAL_UART_Transmit(&huart2, str, strlen(str), 1000);
}

void Serial_Transmit_Int(uint8_t i[])
{
	HAL_UART_Transmit(&huart2, i, strlen(i), 1000);
}

void Serial_Transmit_I(int * i)
{
	HAL_UART_Transmit(&huart2, i, 1, 1000);
}

// Recevoir une donnée du port série par interruption
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	int i;

	if(huart->Instance == USART2)
	{
		if(Rx_indx==0) {for(i=0; i<100;i++) Rx_Buffer[i]=0;}

		Rx_Buffer[Rx_indx++]=Rx_data[0];
		Transfer_cplt = 1;


		//HAL_UART_Receive_IT(&huart2, Rx_data, 1);
	}
}

// Changer la polarité de l'horloge (Elle doit être à HIGH pour l'afficheur et à LOW pour les switchs)
int setClockPolarity(int cpol)
{
	if(cpol)
		hspi3.Init.CLKPolarity = SPI_POLARITY_HIGH;
	else
		hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;

	if (HAL_SPI_Init(&hspi3) != HAL_OK)
	{
		return 0;
	}
	return 1;
}

int bitRead(uint8_t byte, int nb)
{
	if(nb > 8 || nb < 0)
		return -1;
	return (byte >> nb) & 1;
}

int bitSet(uint8_t byte, int nb)
{
	if(nb > 8 || nb < 0)
		return -1;
	return byte | (1 << nb);
}

int bitClear(uint8_t byte, int nb)
{
	if(nb > 8 || nb < 0)
		return -1;
	return byte & ~(1 << nb);
}

void refreshGauche()
{
	swGauche0A = FCU_ReceiveSW(swGauche0, RGPIOA);
	swGauche0B = FCU_ReceiveSW(swGauche0, RGPIOB);
	swGauche1A = FCU_ReceiveSW(swGauche1, RGPIOA);
	swGauche1B = FCU_ReceiveSW(swGauche1, RGPIOB);
	dataSwitch[0] = 0;
	dataSwitch[5] = 255;
	dataSwitch[1] = bitRead(swGauche1B, 5)*128 + bitRead(swGauche1B, 4)*64 + bitRead(swGauche1A, 7)*32 + bitRead(swGauche1B, 7)*16 +
			bitRead(swGauche1A, 6)*8 + bitRead(swGauche1A, 5)*4 + bitRead(swGauche1A, 4)*2 + bitRead(swGauche1A, 3);

	dataSwitch[2] = bitRead(swGauche0B, 0)*128 + bitRead(swGauche0B, 1)*64 + bitRead(swGauche0B, 2)*32 + bitRead(swGauche0B, 3)*16 +
			bitRead(swGauche0B, 4)*8 + bitRead(swGauche1A, 0)*4 + bitRead(swGauche0A, 5)*2 + bitRead(swGauche0A, 7);

	dataSwitch[3] = bitRead(swGauche0A, 6)*128 + bitRead(swGauche0A, 4)*64 + bitRead(swGauche0A, 2)*32 + bitRead(swGauche0B, 7)*16 +
			bitRead(swGauche0B, 6)*8 + bitRead(swGauche0B, 5)*4 + bitRead(swGauche0A, 1)*2 + bitRead(swGauche0A, 0);

	dataSwitch[4] = bitRead(swGauche1A, 2)*128 + bitRead(swGauche1A, 1)*64;
	for(int i = 0; i < 6; i++)
	{
		Serial_Transmit_I(&dataSwitch[i]);
	}
}

void refreshCentre()
{
	swCentre0A = FCU_ReceiveSW(swCentre0, RGPIOA);
	swCentre0B = FCU_ReceiveSW(swCentre0, RGPIOB);
	swCentre1A = FCU_ReceiveSW(swCentre1, RGPIOA);
	swCentre1B = FCU_ReceiveSW(swCentre1, RGPIOB);
	dataSwitch[0] = 1;
	dataSwitch[5] = 254;
	dataSwitch[1] = bitRead(swCentre0A, 2)*128 + bitRead(swCentre0A, 3)*64 + bitRead(swCentre0A, 1)*32 + bitRead(swCentre0A, 0)*16 +
	             bitRead(swCentre0A, 6)*8 + bitRead(swCentre0A, 7)*4 + bitRead(swCentre0A, 5)*2 + bitRead(swCentre0A, 4);

	dataSwitch[2] = bitRead(swCentre0B, 3)*128 + bitRead(swCentre0B, 1)*64 + bitRead(swCentre0B, 4)*32 + bitRead(swCentre0B, 5)*16 +
				 bitRead(swCentre0B, 2)*8 + bitRead(swCentre1A, 1)*4 + bitRead(swCentre1B, 1)*2 + bitRead(swCentre1B, 0);

	dataSwitch[3] = bitRead(swCentre1B, 7)*128 + bitRead(swCentre1B, 6)*64 + bitRead(swCentre1B, 5)*32 + bitRead(swCentre1B, 4)*16 +
				 bitRead(swCentre1A, 6)*8 + bitRead(swCentre1A, 7)*4 + bitRead(swCentre1A, 5)*2 + bitRead(swCentre1A, 4);

	dataSwitch[4] = bitRead(swCentre1A, 3)*128 + bitRead(swCentre1A, 2)*64 + bitRead(swCentre0B, 7)*32 + bitRead(swCentre0B, 6)*16 +
				 bitRead(swCentre1B, 2)*8 + bitRead(swCentre1B, 3)*4;
	for(int i = 0; i < 6; i++)
	{
		Serial_Transmit_I(&dataSwitch[i]);
	}
}

void refreshDroite()
{
	swDroite0A = FCU_ReceiveSW(swDroite0, RGPIOA);
	swDroite0B = FCU_ReceiveSW(swDroite0, RGPIOB);
	swDroite1A = FCU_ReceiveSW(swDroite1, RGPIOA);
	swDroite1B = FCU_ReceiveSW(swDroite1, RGPIOB);
	dataSwitch[0] = 2;
	dataSwitch[5] = 253;
	dataSwitch[1] = bitRead(swDroite1B, 5)*128 + bitRead(swDroite1B, 4)*64 + bitRead(swDroite1A, 1)*32 + bitRead(swDroite1B, 7)*16 +
	             bitRead(swDroite1A, 3)*8 + bitRead(swDroite1A, 2)*4 + bitRead(swDroite1A, 4)*2 + bitRead(swDroite1A, 5);

	dataSwitch[2] = bitRead(swDroite0B, 4)*128 + bitRead(swDroite0B, 3)*64 + bitRead(swDroite0B, 2)*32 + bitRead(swDroite0B, 1)*16 +
	             bitRead(swDroite0B, 0)*8 + bitRead(swDroite0A, 1)*4 + bitRead(swDroite0B, 7)*2 + bitRead(swDroite0B, 6);

	dataSwitch[3] = bitRead(swDroite0B, 5)*128 + bitRead(swDroite0A, 0)*64 + bitRead(swDroite0A, 2)*32 + bitRead(swDroite0A, 5)*16 +
	             bitRead(swDroite0A, 7)*8 + bitRead(swDroite0A, 6)*4 + bitRead(swDroite0A, 4)*2 + bitRead(swDroite0A, 3);

	dataSwitch[4] = bitRead(swDroite1B, 1)*128 + bitRead(swDroite1B, 2)*64;
	for(int i = 0; i < 6; i++)
	{
		Serial_Transmit_I(&dataSwitch[i]);
	}
}



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_SPI3_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart2, Rx_data, 1);
  HAL_GPIO_WritePin(NSS_Reset_GPIO_Port, NSS_Reset_Pin, GPIO_PIN_RESET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(NSS_Reset_GPIO_Port, NSS_Reset_Pin, GPIO_PIN_SET);
  FCU_Affich_Init();

  while (!Transfer_cplt)
  {
	  Serial_Transmit_Str("Contact");
	  HAL_Delay(100);
  }
  Transfer_cplt = 0;
  Rx_indx = 0;

  setClockPolarity(LOW);
  FCU_Switchs_Init();

/* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  refreshGauche();
	  HAL_UART_Receive(&huart2, inputByte, 15, 1000);
	  refreshCentre();
	  HAL_UART_Receive(&huart2, inputByte, 15, 1000);
	  refreshDroite();
	  HAL_UART_Receive(&huart2, inputByte, 15, 1000);

	  for(int i = 0; i < 8; i++)
		{

		  if(bitRead(inputByte[0], 7-i)) // LEDs de Gauche
			  Gauche = bitSet(Gauche, i);
		  else
			  Gauche = bitClear(Gauche, i);

		  if(bitRead(inputByte[2], 7-i)) // LEDs de Droite
			  Droite = bitSet(Droite, i);
		  else
			  Droite = bitClear(Droite, i);

		  if(i == 0 || i == 7) continue;
		  if(bitRead(inputByte[1], 7-i)) // LEDs du Centre
			  Centre = bitSet(Centre, i);
		  else
			  Centre = bitClear(Centre, i);
		}
		FCU_Transmit_G(5, Gauche);
		FCU_Transmit_D(5, Droite);
		FCU_Transmit_C(2, 6, Centre);

		FCU_Transmit_G(1, bitRead(inputByte[3], 4) + bitRead(inputByte[3], 5)*2 + bitRead(inputByte[3], 6)*4 + bitRead(inputByte[3], 7)*8);
		FCU_Transmit_G(2, bitRead(inputByte[3], 0) + bitRead(inputByte[3], 1)*2 + bitRead(inputByte[3], 2)*4 + bitRead(inputByte[3], 3)*8);
		FCU_Transmit_G(3, bitRead(inputByte[4], 4) + bitRead(inputByte[4], 5)*2 + bitRead(inputByte[4], 6)*4 + bitRead(inputByte[4], 7)*8);
		FCU_Transmit_G(4, bitRead(inputByte[4], 0) + bitRead(inputByte[4], 1)*2 + bitRead(inputByte[4], 2)*4 + bitRead(inputByte[4], 3)*8);

		FCU_Transmit_D(1, bitRead(inputByte[5], 4) + bitRead(inputByte[5], 5)*2 + bitRead(inputByte[5], 6)*4 + bitRead(inputByte[5], 7)*8);
		FCU_Transmit_D(2, bitRead(inputByte[5], 0) + bitRead(inputByte[5], 1)*2 + bitRead(inputByte[5], 2)*4 + bitRead(inputByte[5], 3)*8);
		FCU_Transmit_D(3, bitRead(inputByte[6], 4) + bitRead(inputByte[6], 5)*2 + bitRead(inputByte[6], 6)*4 + bitRead(inputByte[6], 7)*8);
		FCU_Transmit_D(4, bitRead(inputByte[6], 0) + bitRead(inputByte[6], 1)*2 + bitRead(inputByte[6], 2)*4 + bitRead(inputByte[6], 3)*8);

		FCU_Transmit_C(1, 1, bitRead(inputByte[7], 4) + bitRead(inputByte[7], 5)*2 + bitRead(inputByte[7], 6)*4 + bitRead(inputByte[7], 7)*8);
		FCU_Transmit_C(1, 2, bitRead(inputByte[7], 0) + bitRead(inputByte[7], 1)*2 + bitRead(inputByte[7], 2)*4 + bitRead(inputByte[7], 3)*8);
		FCU_Transmit_C(1, 3, bitRead(inputByte[8], 4) + bitRead(inputByte[8], 5)*2 + bitRead(inputByte[8], 6)*4 + bitRead(inputByte[8], 7)*8);
		FCU_Transmit_C(1, 4, bitRead(inputByte[8], 0) + bitRead(inputByte[8], 1)*2 + bitRead(inputByte[8], 2)*4 + bitRead(inputByte[8], 3)*8);
		FCU_Transmit_C(1, 5, bitRead(inputByte[9], 4) + bitRead(inputByte[9], 5)*2 + bitRead(inputByte[9], 6)*4 + bitRead(inputByte[9], 7)*8);
		FCU_Transmit_C(1, 6, bitRead(inputByte[9], 0) + bitRead(inputByte[9], 1)*2 + bitRead(inputByte[9], 2)*4 + bitRead(inputByte[9], 3)*8);

		FCU_Transmit_C(2, 1, bitRead(inputByte[10], 4) + bitRead(inputByte[10], 5)*2 + bitRead(inputByte[10], 6)*4 + bitRead(inputByte[10], 7)*8);
		FCU_Transmit_C(2, 2, bitRead(inputByte[10], 0) + bitRead(inputByte[10], 1)*2 + bitRead(inputByte[10], 2)*4 + bitRead(inputByte[10], 3)*8);
		FCU_Transmit_C(2, 3, bitRead(inputByte[11], 4) + bitRead(inputByte[11], 5)*2 + bitRead(inputByte[11], 6)*4 + bitRead(inputByte[11], 7)*8);
		FCU_Transmit_C(2, 4, bitRead(inputByte[11], 0) + bitRead(inputByte[11], 1)*2 + bitRead(inputByte[11], 2)*4 + bitRead(inputByte[11], 3)*8);
		FCU_Transmit_C(2, 5, bitRead(inputByte[12], 4) + bitRead(inputByte[12], 5)*2 + bitRead(inputByte[12], 6)*4 + bitRead(inputByte[12], 7)*8);

		FCU_Transmit_C(3, 1, bitRead(inputByte[12], 0) + bitRead(inputByte[12], 1)*2 + bitRead(inputByte[12], 2)*4 + bitRead(inputByte[12], 3)*8);
		FCU_Transmit_C(3, 2, bitRead(inputByte[13], 4) + bitRead(inputByte[13], 5)*2 + bitRead(inputByte[13], 6)*4 + bitRead(inputByte[13], 7)*8);
		FCU_Transmit_C(3, 3, bitRead(inputByte[13], 0) + bitRead(inputByte[13], 1)*2 + bitRead(inputByte[13], 2)*4 + bitRead(inputByte[13], 3)*8);
		FCU_Transmit_C(3, 4, bitRead(inputByte[14], 4) + bitRead(inputByte[14], 5)*2 + bitRead(inputByte[14], 6)*4 + bitRead(inputByte[14], 7)*8);
		FCU_Transmit_C(3, 5, bitRead(inputByte[14], 0) + bitRead(inputByte[14], 1)*2 + bitRead(inputByte[14], 2)*4 + bitRead(inputByte[14], 3)*8);
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

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the main internal regulator output voltage 
    */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* SPI3 init function */
static void MX_SPI3_Init(void)
{

  /* SPI3 parameter configuration*/
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 7;
  hspi3.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi3.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5|NSS_Reset_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, NSS_Switch_Pin|NSS_BL_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(NSS_AffD_GPIO_Port, NSS_AffD_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, NSS_AffG_Pin|NSS_AffC_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : POT_INT_Pin */
  GPIO_InitStruct.Pin = POT_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(POT_INT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PA5 NSS_Reset_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_5|NSS_Reset_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : NSS_Switch_Pin NSS_BL_Pin */
  GPIO_InitStruct.Pin = NSS_Switch_Pin|NSS_BL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : NSS_AffD_Pin */
  GPIO_InitStruct.Pin = NSS_AffD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(NSS_AffD_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : NSS_AffG_Pin NSS_AffC_Pin */
  GPIO_InitStruct.Pin = NSS_AffG_Pin|NSS_AffC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
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
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
