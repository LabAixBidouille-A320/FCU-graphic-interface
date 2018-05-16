/*
 * fcu_functions.c
 *
 *  Created on: 7 mai 2018
 *      Author: Yael
 */

#include "fcu_functions.h"

uint8_t no_op = 0x00;

/**
 * Transmettre une donnée aux afficheurs de la partie gauche du FCU
 * Paramètres : reg = registre où envoyer la donnée (Digits 1-4, leds 5, indics 6)
 * 			    data = valeur à envoyer au registre ( defines dans fcu_functions.h )
 */
void FCU_Transmit_G(uint8_t reg, uint8_t data)
{
	HAL_GPIO_WritePin(NSS_AffG_GPIO_Port, NSS_AffG_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3, &reg , 1, 1000);
	HAL_SPI_Transmit(&hspi3, &data , 1, 1000);
	HAL_GPIO_WritePin(NSS_AffG_GPIO_Port, NSS_AffG_Pin, GPIO_PIN_SET);
}

/**
 * Transmettre une donnée aux afficheurs de la partie droite du FCU
 * Paramètres : reg = registre où envoyer la donnée (Digits 1-4, leds 5, indics 8)
 * 			    data = valeur à envoyer au registre ( defines dans fcu_functions.h )
 */
void FCU_Transmit_D(uint8_t reg, uint8_t data)
{
	HAL_GPIO_WritePin(NSS_AffD_GPIO_Port, NSS_AffD_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3, &reg , 1, 1000);
	HAL_SPI_Transmit(&hspi3, &data , 1, 1000);
	HAL_GPIO_WritePin(NSS_AffD_GPIO_Port, NSS_AffD_Pin, GPIO_PIN_SET);
}

/**
 * Transmettre une donnée aux afficheurs de la partie centrale du FCU
 * Paramètres : addr = addresse de la sous partie à controler de 1 à 3 ( 0 = toutes )
 * 				reg = registre où envoyer la donnée (Digits 1-5 (6 pour addr = 1), leds & indics 6-7 (7-8 pour addr = 1))
 * 			    data = valeur à envoyer au registre ( defines dans fcu_functions.h )
 */
void FCU_Transmit_C(uint8_t addr, uint8_t reg, uint8_t data)
{
	HAL_GPIO_WritePin(NSS_AffC_GPIO_Port, NSS_AffC_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3, (addr == 3 || addr == 0 ? &reg : &no_op) , 1, 1000);
	HAL_SPI_Transmit(&hspi3, &data , 1, 1000);
	HAL_SPI_Transmit(&hspi3, (addr == 2 || addr == 0 ? &reg : &no_op) , 1, 1000);
	HAL_SPI_Transmit(&hspi3, &data , 1, 1000);
	HAL_SPI_Transmit(&hspi3, (addr == 1 || addr == 0 ? &reg : &no_op) , 1, 1000);
	HAL_SPI_Transmit(&hspi3, &data , 1, 1000);
	HAL_GPIO_WritePin(NSS_AffC_GPIO_Port, NSS_AffC_Pin, GPIO_PIN_SET);
}

/**
 * Effacer et éteindre tous les digits, leds, et voyants du FCU
 */
void FCU_Affich_Clear()
{
	for(uint8_t i = 0; i < 7; i++)
	{
		FCU_Transmit_G(i, DIGIT_BLANK);
		FCU_Transmit_D(i, DIGIT_BLANK);
		FCU_Transmit_C(0, i, DIGIT_BLANK);
	}
	FCU_Transmit_G(5, 0x00);
	FCU_Transmit_G(6, 0x00);
	FCU_Transmit_C(1, 7, 0x00);
	FCU_Transmit_C(1, 8, 0x00);
	FCU_Transmit_C(2, 6, 0x00);
	FCU_Transmit_C(2, 7, 0x00);
	FCU_Transmit_C(3, 6, 0x00);
	FCU_Transmit_C(3, 7, 0x00);
	FCU_Transmit_D(5, 0x00);
	FCU_Transmit_D(6, 0x00);
}

/**
 * Teste l'adressage et les registres de chaque partie du FCU en les allumant à tour de rôle
 */
void FCU_Affich_Init()
{
	  FCU_Affich_Clear();
	  FCU_Transmit_G(SHUTDOWN_MODE, 0x01); // No shutdown
	  FCU_Transmit_G(SCAN_LIMIT, 0x05); // Scan Limit 0-5
	  FCU_Transmit_G(DECODE_MODE, 0x0F); // Decode Mode, digits 3-0
	  FCU_Transmit_G(INTENSITY, 0x0E); // Intensité élevée

	  FCU_Transmit_G(TEST_DISPLAY, 0x01);
	  HAL_Delay(500);
	  FCU_Transmit_G(TEST_DISPLAY, 0x00);

	  FCU_Transmit_C(0, SHUTDOWN_MODE, 0x01); // No shutdown
	  FCU_Transmit_C(0, SCAN_LIMIT, 0x06); // Scan Limit 0-6
	  FCU_Transmit_C(1, SCAN_LIMIT, 0x07); // Scan Limit 0-7
	  FCU_Transmit_C(0, DECODE_MODE, 0x1F); // Decode Mode, digits 4-0
	  FCU_Transmit_C(1, DECODE_MODE, 0x3F); // Decode Mode, digits 5-0
	  FCU_Transmit_C(0, INTENSITY, 0x0E); // Intensité élevée
	  FCU_Transmit_C(1, TEST_DISPLAY, 0x01);
	  HAL_Delay(500);
	  FCU_Transmit_C(1, TEST_DISPLAY, 0x00);
	  FCU_Transmit_C(2, TEST_DISPLAY, 0x01);
	  HAL_Delay(500);
	  FCU_Transmit_C(2, TEST_DISPLAY, 0x00);
	  FCU_Transmit_C(3, TEST_DISPLAY, 0x01);
	  HAL_Delay(500);
	  FCU_Transmit_C(3, TEST_DISPLAY, 0x00);
	  FCU_Transmit_C(0, TEST_DISPLAY, 0x01);
	  HAL_Delay(500);
	  FCU_Transmit_C(0, TEST_DISPLAY, 0x00);

	  FCU_Transmit_D(SHUTDOWN_MODE, 0x01); // No shutdown
	  FCU_Transmit_D(SCAN_LIMIT, 0x05); // Scan Limit 0-5
	  FCU_Transmit_D(DECODE_MODE, 0x0F); // Decode Mode, digits 3-0
	  FCU_Transmit_D(INTENSITY, 0x0E); // Intensité élevée
	  FCU_Transmit_D(TEST_DISPLAY, 0x01);
	  HAL_Delay(500);
	  FCU_Transmit_D(TEST_DISPLAY, 0x00);
}

/**
 * Transmettre une donnée aux registres des switchs
 * Paramètres : addr = addresse physique de la puce (0-5)
 * 				reg = registre où envoyer la donnée (defines dans fcu_functions.h)
 * 			    data = valeur à envoyer au registre
 */
void FCU_TransmitSW(uint8_t addr, uint8_t reg, uint8_t data)
{
	uint8_t final_addr = OPCODEW | (addr << 1u);
	HAL_GPIO_WritePin(NSS_Switch_GPIO_Port, NSS_Switch_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3, &final_addr , 1, 1000);
	HAL_SPI_Transmit(&hspi3, &reg , 1, 1000);
	HAL_SPI_Transmit(&hspi3, &data , 1, 1000);
	HAL_GPIO_WritePin(NSS_Switch_GPIO_Port, NSS_Switch_Pin, GPIO_PIN_SET);
}

/**
 * Recevoir une donnée des registres des switchs
 * Paramètres : addr = addresse physique de la puce (0-5)
 * 				reg = registre d'où recevoir la donnée (defines dans fcu_functions.h)
 */
uint8_t FCU_ReceiveSW(uint8_t addr, uint8_t reg)
{
	uint8_t value = 0;
	uint8_t final_addr = OPCODER | (addr << 1u);
	HAL_GPIO_WritePin(NSS_Switch_GPIO_Port, NSS_Switch_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3, &final_addr , 1, 1000);
	HAL_SPI_Transmit(&hspi3, &reg , 1, 1000);
	HAL_SPI_TransmitReceive(&hspi3, &value , &value, 1, 1000);
	HAL_GPIO_WritePin(NSS_Switch_GPIO_Port, NSS_Switch_Pin, GPIO_PIN_SET);
	return value;
}

/**
 * Initialise les registres des switchs
 */
void FCU_Switchs_Init()
{
	for(int ad = 0; ad < 6; ad++)
	{
		FCU_TransmitSW(ad, IOCON, 0x08);
		FCU_TransmitSW(ad, IODIRA, 0xFF);
		FCU_TransmitSW(ad, IODIRB, 0xFF);
		FCU_TransmitSW(ad, GPPUA, 0xFF);
		FCU_TransmitSW(ad, GPPUB, 0xFF);
	}
}
