/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include <stdio.h>
#include "fonts.h"
#include <string.h>
#include "st7735.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
TELA_INICIO,
TELA_SENHA,
TELA_MENU,
TELA_PRESENCA,
TELA_SAIDA,
TELA_BLOQUEADO,
TELA_RELATORIO
} Tela;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define INICIAR_PROFESSOR_PORT GPIOA
#define INICIAR_PROFESSOR_PIN GPIO_PIN_9

#define RETORNO_ALUNO_PORT GPIOA
#define RETORNO_ALUNO_PIN GPIO_PIN_10

#define SAIDA_ALUNO_PORT GPIOA
#define SAIDA_ALUNO_PIN GPIO_PIN_11

#define PRESENCA_PORT GPIOA
#define PRESENCA_PIN GPIO_PIN_12

#define READ_BTN(port, pin) HAL_GPIO_ReadPin(port, pin)

#define MAX_FORA 3
#define MENU_TOTAL 4
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */
GPIO_PinState estado_anterior = GPIO_PIN_RESET;
GPIO_PinState est_inicio_ant = GPIO_PIN_RESET;
GPIO_PinState est_presenca_ant = GPIO_PIN_RESET;
GPIO_PinState est_saida_ant = GPIO_PIN_RESET;
GPIO_PinState est_retorno_ant = GPIO_PIN_RESET;

char buffer[30];

uint8_t index_senha = 0;
char senha_correta[] = "1234";
char senha_digitada[5];

int tentativas = 0;
int tentativas_max = 3;

uint8_t indice = 0; //controla a posição da string a proxima tecla é slv
uint32_t tempo_inicio = 0;
uint32_t tempo = 0;
uint8_t bloqueado = 0;
uint8_t tempo_bloqueado = 30;
uint8_t tempo_inicio_bloqueio = 0;

uint8_t alunos_presentes = 0;
uint8_t alunos_fora = 0;
uint8_t alunos_maximo = 40;

Tela tela_atual = TELA_INICIO;

uint8_t menu_index = 0;
static uint8_t tela_anterior = 255;
uint16_t total_saidas = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
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
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  ST7735_Init();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  GPIO_PinState INICIAR_PROFESSOR = HAL_GPIO_ReadPin(INICIAR_PROFESSOR_PORT, INICIAR_PROFESSOR_PIN);
	  GPIO_PinState PRESENCA = HAL_GPIO_ReadPin(PRESENCA_PORT, PRESENCA_PIN);
	  GPIO_PinState SAIDA_ALUNO = HAL_GPIO_ReadPin(SAIDA_ALUNO_PORT, SAIDA_ALUNO_PIN);
	  GPIO_PinState RETORNO_ALUNO = HAL_GPIO_ReadPin(RETORNO_ALUNO_PORT, RETORNO_ALUNO_PIN);

	  if (tela_atual != tela_anterior)
	  {
	      ST7735_FillScreen(BLACK);
	      tela_anterior = tela_atual;
	  }

	  switch (tela_atual){

	  // =========================
	  case TELA_INICIO:
	// =========================

	  ST7735_WriteString(10, 20, "Pressione o botao para INICIAR", Font_7x10, WHITE, BLACK);

	  if (INICIAR_PROFESSOR == GPIO_PIN_RESET ){

		  HAL_Delay(200);

		  tela_atual = TELA_SENHA;

	  }
	  break;


	  case TELA_SENHA:

	      ST7735_WriteString(10, 10, "Digite a senha", Font_7x10, WHITE, BLACK);

	      // Mostra exatamente os dígitos digitados
	      sprintf(buffer, "Senha: %s", senha_digitada);
	      ST7735_WriteString(10, 30, buffer, Font_7x10, WHITE, BLACK);

	      // Só permite até 4 dígitos
	      if(index_senha < 4)
	      {
	          // BOTAO 1
	          if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == GPIO_PIN_RESET)
	          {
	              HAL_Delay(50);

	              if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == GPIO_PIN_RESET)
	              {
	                  senha_digitada[index_senha++] = '1';
	                  senha_digitada[index_senha] = '\0';

	                  HAL_Delay(200);
	              }
	          }

	          // BOTAO 2
	          if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == GPIO_PIN_RESET)
	          {
	              HAL_Delay(50);

	              if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == GPIO_PIN_RESET)
	              {
	                  senha_digitada[index_senha++] = '2';
	                  senha_digitada[index_senha] = '\0';

	                  HAL_Delay(200);
	              }
	          }

	          // BOTAO 3
	          if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == GPIO_PIN_RESET)
	          {
	              HAL_Delay(50);

	              if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == GPIO_PIN_RESET)
	              {
	                  senha_digitada[index_senha++] = '3';
	                  senha_digitada[index_senha] = '\0';

	                  HAL_Delay(200);
	              }
	          }

	          // BOTAO 4
	          if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_RESET)
	          {
	              HAL_Delay(50);

	              if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_RESET)
	              {
	                  senha_digitada[index_senha++] = '4';
	                  senha_digitada[index_senha] = '\0';

	                  HAL_Delay(200);
	              }
	          }
	      }

      // Verifica SOMENTE quando tiver 4 dígitos
	      if(index_senha == 4)
	      {
	          HAL_Delay(300);

	          if(strcmp(senha_digitada, senha_correta) == 0)
	          {
	              // senha correta
	              tentativas = 0;

	              tela_atual = TELA_MENU;
	          }
	          else
	          {
	              // Senha errada
	              tentativas++;

	              ST7735_FillRectangle(10, 50, 120, 20, BLACK);
	              ST7735_WriteString(10, 50, "Senha incorreta", Font_7x10, WHITE, BLACK);

	              HAL_Delay(1000);

	              if(tentativas >= tentativas_max)
	              {
	                  bloqueado = 1;

	                  tempo_inicio = HAL_GetTick();

	                  tela_atual = TELA_BLOQUEADO;
	              }
	          }

	          // Limpa senha para nova tentativa
	          memset(senha_digitada, 0, sizeof(senha_digitada));
	          index_senha = 0;

	          // Limpa linha da senha na tela
	          ST7735_FillRectangle(10, 30, 120, 20, BLACK);
	      }

	      break;

	  case TELA_BLOQUEADO:

	 	      tempo = (HAL_GetTick() - tempo_inicio) / 1000;

	 	      ST7735_WriteString(10, 10, "SISTEMA BLOQUEADO", Font_7x10, WHITE, BLACK);

	 	      sprintf(buffer, "Tempo: %lds", tempo_bloqueado - tempo);

	 	      ST7735_WriteString(10, 40,  buffer, Font_7x10, WHITE, BLACK);


	 	      // Quando acabar o tempo
	 	      if (tempo >= tempo_bloqueado)
	 	      {
	 	          bloqueado = 0;

	 	          tentativas = 0;

	 	          tela_atual = TELA_INICIO;
	 	      }

	 	      break;

	  ST7735_WriteString(10, 30, "PRESENCA = entrar", Font_7x10, WHITE, BLACK);

	  if (PRESENCA == GPIO_PIN_SET && est_presenca_ant == GPIO_PIN_RESET)
	  {
	      tela_atual = TELA_MENU;
	      HAL_Delay(300);
	  }
	  est_presenca_ant = PRESENCA;

	  break;

	  case TELA_MENU:

		  ST7735_WriteString(20, 5, "MENU PRINCIPAL", Font_7x10, WHITE, BLACK);

		    sprintf(buffer, "Presentes: %d", alunos_presentes);
		    ST7735_WriteString(10, 30, buffer, Font_7x10, WHITE, BLACK);

		    sprintf(buffer, "Fora da sala: %d", alunos_fora);
		    ST7735_WriteString(10, 50, buffer, Font_7x10, WHITE, BLACK);

		    if(alunos_fora > 0)
		    {
		        ST7735_WriteString(10, 70, "Saidas: OCUPADO", Font_7x10, RED, BLACK);
		    }
		    else
		    {
		        ST7735_WriteString(10, 70, "Saidas: LIVRE", Font_7x10, GREEN, BLACK);
		    }

	      // BOTAO ENTRADA
	      if(PRESENCA == GPIO_PIN_RESET && est_presenca_ant == GPIO_PIN_SET)
	      {
	          tela_atual = TELA_PRESENCA;
	          HAL_Delay(200);
	      }
	      est_presenca_ant = PRESENCA;

	      // BOTAO SAIDA
	      if(SAIDA_ALUNO == GPIO_PIN_RESET && est_saida_ant == GPIO_PIN_SET)
	      {
	          tela_atual = TELA_SAIDA;
	          HAL_Delay(200);
	      }
	      est_saida_ant = SAIDA_ALUNO;

	      // BOTAO RETORNO
	      if(RETORNO_ALUNO == GPIO_PIN_RESET && est_retorno_ant == GPIO_PIN_SET)
	      {
	          if(alunos_fora > 0)
	          {
	              alunos_fora--;
	              alunos_presentes++;
	          }

	          HAL_Delay(200);
	      }
	      est_retorno_ant = RETORNO_ALUNO;

	      // RELATORIO
	      if(INICIAR_PROFESSOR == GPIO_PIN_RESET && est_inicio_ant == GPIO_PIN_SET)
	      {
	          tela_atual = TELA_RELATORIO;
	          HAL_Delay(200);
	      }
	      est_inicio_ant = INICIAR_PROFESSOR;

	      break;

	      case TELA_PRESENCA:

	          // limpa a tela inteira
	          ST7735_FillScreen(BLACK);

	          //simula leitura facial
	          int matricula = 2345;

	          //verifica limite da sala
	          if(alunos_presentes < alunos_maximo)
	          {
	              alunos_presentes++;

	              ST7735_WriteString(10, 10, "ENTRADA LIBERADA", Font_7x10, WHITE, BLACK);

	              sprintf(buffer, "Matricula: %d", matricula);
	              ST7735_WriteString(10, 35, buffer, Font_7x10, WHITE, BLACK);

	              sprintf(buffer, "Presentes: %d", alunos_presentes);
	              ST7735_WriteString(10, 55, buffer, Font_7x10, WHITE, BLACK);
	          }
	          else
	          {
	              ST7735_WriteString(10, 20, "SALA LOTADA", Font_7x10, WHITE, BLACK);
	          }


	          HAL_Delay(100);

	          tela_atual = TELA_MENU;

	          break;


	  case TELA_SAIDA:

		  if(alunos_fora < MAX_FORA && alunos_presentes > 0)
	  {
	      alunos_fora++;
	      alunos_presentes--;
	      total_saidas++;

	      ST7735_WriteString(10, 10, "Saida liberada", Font_7x10, WHITE, BLACK);

	      sprintf(buffer, "fora: %d", alunos_fora);
	     ST7735_WriteString(5,30, buffer, Font_7x10, WHITE, BLACK);
	  }
		  else
	  {
	      ST7735_WriteString(10, 10, "Limite atingido", Font_7x10, WHITE, BLACK);
	  }

	  HAL_Delay(1000);
	  tela_atual = TELA_MENU;
	  break;

	  case TELA_RELATORIO:

	      ST7735_WriteString(15, 5, "RELATORIO FINAL", Font_7x10, WHITE, BLACK);

	      // ttotal regis
	      sprintf(buffer, "Registrados: %d", alunos_presentes + alunos_fora);
	      ST7735_WriteString(10, 25, buffer, Font_7x10, WHITE, BLACK);

	      // saidas totais
	      sprintf(buffer, "Saidas: %d", total_saidas);
	      ST7735_WriteString(10, 45, buffer, Font_7x10, WHITE, BLACK);

	      // aln fora
	      sprintf(buffer, "Fora sala: %d", alunos_fora);
	      ST7735_WriteString(10, 65, buffer, Font_7x10, WHITE, BLACK);

	      // volta
	      ST7735_WriteString(10, 95, "RET = Encerrar", Font_7x10, WHITE, BLACK);

	      // bt retormp
	      if(RETORNO_ALUNO == GPIO_PIN_RESET && INICIAR_PROFESSOR == GPIO_PIN_RESET)
	      {
	          alunos_presentes = 0;
	          alunos_fora = 0;
	          total_saidas = 0;

	          tela_atual = TELA_INICIO;

	          HAL_Delay(200);
	      }

	      est_retorno_ant = RETORNO_ALUNO;


	      break;
	  }
}
}

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  /* USER CODE END 3 */


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_1LINE;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, ST7735_DC_Pin|ST7735_RES_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : ST7735_CS_Pin */
  GPIO_InitStruct.Pin = ST7735_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ST7735_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ST7735_DC_Pin ST7735_RES_Pin */
  GPIO_InitStruct.Pin = ST7735_DC_Pin|ST7735_RES_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : INICIAR_PROFESSOR_Pin retorno_aluno_Pin saida_aluno_Pin Presenca_Pin */
  GPIO_InitStruct.Pin = INICIAR_PROFESSOR_PIN|retorno_aluno_Pin|saida_aluno_Pin|Presenca_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
