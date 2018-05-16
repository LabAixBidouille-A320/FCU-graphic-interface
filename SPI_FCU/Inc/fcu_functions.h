#include "main.h"
#include "stm32l4xx_hal.h"
#include <stdint.h>

SPI_HandleTypeDef hspi3;

#define LOW 0
#define HIGH 1


/* ----- Valeurs nécessaires à l'affichage ----- */

// Modes de registres
#define NO_OP			0x00
#define SHUTDOWN_MODE	0x0C
#define SCAN_LIMIT 		0x0B
#define DECODE_MODE 	0x09
#define INTENSITY 		0x0A
#define TEST_DISPLAY 	0x0F

// Valeurs types
#define DIGIT_BLANK 0x0F
#define NO_LED 		0x00

// Valeurs pour la partie gauche à l'adresse 5

#define G_NDB 	0x40
#define G_ARPT 	0x20
#define G_CSTR 	0x10
#define G_VORD 	0x08
#define G_WPT 	0x04
#define G_ILS 	0x02
#define G_FD 	0x01

// Valeurs pour la partie gauche à l'adresse 6
#define G_QNH 0x60
#define G_QFE 0x03

// Valeurs pour la première partie centrale à l'adresse 7
#define C1_MACH 	0x60
#define C1_SPD 		0x0C
#define C1_HDG_H 	0x03
#define C1_POINT_G 	0x10

// Valeurs pour la première partie centrale à l'adresse 8
#define C1_TRK 		0x60
#define C1_LAT 		0x18
#define C1_HDG_V 	0x03
#define C1_POINT_D 	0x04

// Valeurs pour la deuxième partie centrale à l'adresse 6
#define C2_AP1 		0x40
#define C2_AP2 		0x10
#define C2_LOC 		0x20
#define C2_ATHR 	0x08
#define C2_EXPED 	0x04
#define C2_APPR 	0x02

// Valeurs pour la deuxième partie centrale à l'adresse 7
#define C2_VS 	0x42
#define C2_TRK 	0x30
#define C2_FPA 	0x0C

// Valeurs pour la troisième partie centrale à l'adresse 6
#define C3_LVLCH1 	0x4A
#define C3_FPA 		0x21
#define C3_VS 		0x14

// Valeurs pour la troisième partie centrale à l'adresse 7
#define C3_LVLCH2 	0x31
#define C3_ALT 		0x0C
#define C3_POINT 	0x02

// Valeurs pour la partie droite à l'adresse 5
#define D_WPT 	0x40
#define D_ARPT 	0x20
#define D_CSTR 	0x10
#define D_VORD 	0x08
#define D_NDB	0x04
#define D_ILS 	0x02
#define D_FD 	0x01

// Valeurs pour la partie droite à l'adresse 8
#define D_QFE 0x60
#define D_QNH 0x03

/* ----- Valeurs nécessaires aux switchs ----- */

#define OPCODEW (0b01000000)
#define OPCODER (0b01000001)

//Valeurs des registres

#define IOCON 	0x0A
#define IODIRA 	0x00
#define IODIRB 	0x01
#define GPPUA 	0x0C
#define GPPUB 	0x0D
#define RGPIOA 	0x12
#define RGPIOB 	0x13

/* ----- Fonctions ----- */

void FCU_Transmit_G(uint8_t reg, uint8_t data);
void FCU_Transmit_D(uint8_t reg, uint8_t data);
void FCU_Transmit_C(uint8_t addr, uint8_t reg, uint8_t data);
void FCU_Affich_Clear();
void FCU_Affich_Init();
void FCU_TransmitSW(uint8_t addr, uint8_t reg, uint8_t data);
uint8_t FCU_ReceiveSW(uint8_t addr, uint8_t reg);
void FCU_Switchs_Init();

