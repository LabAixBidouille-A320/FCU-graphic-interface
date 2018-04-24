#include <SPI.h>
#include "LedControl.h"
#include <MCP23S17.h>

#define CSIn 10 // broche CS MCP23S17
#define BR A0 // broche Potentiomètre Brightness
#define BL 6  // broche LEDs Back Light

byte inputByte[15]; // Recevra les lumières à allumer depuis l'interface graphique
int bright = 15;

// Variables qui représentent les switchs et boutons du FCU
MCP swGauche0(0, CSIn);
MCP swGauche1(1, CSIn);
MCP swCentre0(2, CSIn);
MCP swCentre1(3, CSIn);
MCP swDroite0(4, CSIn);
MCP swDroite1(5, CSIn);

byte swGauche0A = 0;
byte swGauche0B = 0;
byte swGauche1A = 0;
byte swGauche1B = 0;
byte swCentre0A = 0;
byte swCentre0B = 0;
byte swCentre1A = 0;
byte swCentre1B = 0;
byte swDroite0A = 0;
byte swDroite0B = 0;
byte swDroite1A = 0;
byte swDroite1B = 0;

//Recevront les états des switchs et boutons de chaque partie du FCU 
byte Gauche[6];
byte Centre[6];
byte Droite[6];

// Variables qui représentent les afficheurs 7 segments du FCU
byte aff7segG1 = 0x0;
byte aff7segG2 = 0x0;
byte aff7segG3 = 0x0;
byte aff7segG4 = 0x0;
byte aff7segC1 = 0x0;
byte aff7segC2 = 0x0;
byte aff7segC3 = 0x0;
byte aff7segC4 = 0x0;
byte aff7segC5 = 0x0;
byte aff7segC6 = 0x0;
byte aff7segC7 = 0x0;
byte aff7segC8 = 0x0;
byte aff7segC9 = 0x0;
byte aff7segC10 = 0x0;
byte aff7segC11 = 0x0;
byte aff7segC12 = 0x0;
byte aff7segC13 = 0x0;
byte aff7segC14 = 0x0;
byte aff7segC15 = 0x0;
byte aff7segC16 = 0x0;
byte aff7segD1 = 0x0;
byte aff7segD2 = 0x0;
byte aff7segD3 = 0x0;
byte aff7segD4 = 0x0;

LedControl lcGauche = LedControl(11, 13, 4, 1);
LedControl lcCentre = LedControl(11, 13, 8, 3);
LedControl lcDroite = LedControl(11, 13, 7, 1);

void setup() {
  pinMode (BL, OUTPUT);
  digitalWrite (BL, HIGH);
  pinMode(CSIn, OUTPUT);
  Serial.begin(9600); 
  establishContact(); // Synchronise l'état de la carte avec l'état de l'interface graphique
}

void loop() {
  // Récupère l'état des switchs et boutons du FCU
  refreshGauche(); 
  refreshCentre();
  refreshDroite();

  // Envoie l'état des switchs et boutons du FCU à l'interface graphique
  Serial.write(Gauche, 6);
  Serial.write(Centre, 6);
  Serial.write(Droite, 6);
}

void establishContact(){
  
  while(Serial.available()<=0){
      Serial.write("Contact");
      delay(1000);
  }
  if(Serial.read() != 'a') establishContact();
}

//Cette fonction est appelée à chaque fois que du contenu se trouve sur la lecture du port série
//C'est donc là où vont se retrouver les données que l'interface graphique envoie  à la carte
void serialEvent() {
  while (Serial.available()) {
    Serial.readBytesUntil(' ', inputByte, 15 );
     
     for(int i = 0; i < 8; i++){
      if(bitRead(inputByte[0], i)) // LEDs de Gauche
        lcGauche.setLed(0,4,i,true);
      else
        lcGauche.setLed(0,4,i,false); 

      if(bitRead(inputByte[2], i)) // LEDs de Droite
        lcDroite.setLed(0,4,i,true);
      else
        lcDroite.setLed(0,4,i,false);

     if(i == 0 || i == 7) continue;
      if(bitRead(inputByte[1], i)) // LEDs du Centre
        lcCentre.setLed(1,5,i,true);
      else
        lcCentre.setLed(1,5,i,false);
    }
    refreshAff7seg(); // Met à jour les afficheurs 7 segments du FCU
  }
}

void refreshAff7seg(){
    aff7segG2 = bitRead(inputByte[3], 0) + bitRead(inputByte[3], 1)*2 + bitRead(inputByte[3], 2)*4 + bitRead(inputByte[3], 3)*8;
    aff7segG1 = bitRead(inputByte[3], 4) + bitRead(inputByte[3], 5)*2 + bitRead(inputByte[3], 6)*4 + bitRead(inputByte[3], 7)*8;
    aff7segG4 = bitRead(inputByte[4], 0) + bitRead(inputByte[4], 1)*2 + bitRead(inputByte[4], 2)*4 + bitRead(inputByte[4], 3)*8;
    aff7segG3 = bitRead(inputByte[4], 4) + bitRead(inputByte[4], 5)*2 + bitRead(inputByte[4], 6)*4 + bitRead(inputByte[4], 7)*8;
    
    aff7segD2 = bitRead(inputByte[5], 0) + bitRead(inputByte[5], 1)*2 + bitRead(inputByte[5], 2)*4 + bitRead(inputByte[5], 3)*8;
    aff7segD1 = bitRead(inputByte[5], 4) + bitRead(inputByte[5], 5)*2 + bitRead(inputByte[5], 6)*4 + bitRead(inputByte[5], 7)*8;
    aff7segD4 = bitRead(inputByte[6], 0) + bitRead(inputByte[6], 1)*2 + bitRead(inputByte[6], 2)*4 + bitRead(inputByte[6], 3)*8;
    aff7segD3 = bitRead(inputByte[6], 4) + bitRead(inputByte[6], 5)*2 + bitRead(inputByte[6], 6)*4 + bitRead(inputByte[6], 7)*8;

    aff7segC2 = bitRead(inputByte[7], 0) + bitRead(inputByte[7], 1)*2 + bitRead(inputByte[7], 2)*4 + bitRead(inputByte[7], 3)*8;
    aff7segC1 = bitRead(inputByte[7], 4) + bitRead(inputByte[7], 5)*2 + bitRead(inputByte[7], 6)*4 + bitRead(inputByte[7], 7)*8;
    aff7segC4 = bitRead(inputByte[8], 0) + bitRead(inputByte[8], 1)*2 + bitRead(inputByte[8], 2)*4 + bitRead(inputByte[8], 3)*8;
    aff7segC3 = bitRead(inputByte[8], 4) + bitRead(inputByte[8], 5)*2 + bitRead(inputByte[8], 6)*4 + bitRead(inputByte[8], 7)*8;
    aff7segC6 = bitRead(inputByte[9], 0) + bitRead(inputByte[9], 1)*2 + bitRead(inputByte[9], 2)*4 + bitRead(inputByte[9], 3)*8;
    aff7segC5 = bitRead(inputByte[9], 4) + bitRead(inputByte[9], 5)*2 + bitRead(inputByte[9], 6)*4 + bitRead(inputByte[9], 7)*8;
    aff7segC8 = bitRead(inputByte[10], 0) + bitRead(inputByte[10], 1)*2 + bitRead(inputByte[10], 2)*4 + bitRead(inputByte[10], 3)*8;
    aff7segC7 = bitRead(inputByte[10], 4) + bitRead(inputByte[10], 5)*2 + bitRead(inputByte[10], 6)*4 + bitRead(inputByte[10], 7)*8;
    aff7segC10 = bitRead(inputByte[11], 0) + bitRead(inputByte[11], 1)*2 + bitRead(inputByte[11], 2)*4 + bitRead(inputByte[11], 3)*8;
    aff7segC9 = bitRead(inputByte[11], 4) + bitRead(inputByte[11], 5)*2 + bitRead(inputByte[11], 6)*4 + bitRead(inputByte[11], 7)*8;
    aff7segC12 = bitRead(inputByte[12], 0) + bitRead(inputByte[12], 1)*2 + bitRead(inputByte[12], 2)*4 + bitRead(inputByte[12], 3)*8;
    aff7segC11 = bitRead(inputByte[12], 4) + bitRead(inputByte[12], 5)*2 + bitRead(inputByte[12], 6)*4 + bitRead(inputByte[12], 7)*8;
    aff7segC14 = bitRead(inputByte[13], 0) + bitRead(inputByte[13], 1)*2 + bitRead(inputByte[13], 2)*4 + bitRead(inputByte[13], 3)*8;
    aff7segC13 = bitRead(inputByte[13], 4) + bitRead(inputByte[13], 5)*2 + bitRead(inputByte[13], 6)*4 + bitRead(inputByte[13], 7)*8;
    aff7segC16 = bitRead(inputByte[14], 0) + bitRead(inputByte[14], 1)*2 + bitRead(inputByte[14], 2)*4 + bitRead(inputByte[14], 3)*8;
    aff7segC15 = bitRead(inputByte[14], 4) + bitRead(inputByte[14], 5)*2 + bitRead(inputByte[14], 6)*4 + bitRead(inputByte[14], 7)*8;

    lcGauche.setChar(0, 0, aff7segG1, false);
    lcGauche.setChar(0, 1, aff7segG2, false);
    lcGauche.setChar(0, 2, aff7segG3, false);
    lcGauche.setChar(0, 3, aff7segG4, false);

    lcDroite.setChar(0, 0, aff7segD1, false);
    lcDroite.setChar(0, 1, aff7segD2, false);
    lcDroite.setChar(0, 2, aff7segD3, false);
    lcDroite.setChar(0, 3, aff7segD4, false);

    lcCentre.setChar(0, 0, aff7segC1, false);
    lcCentre.setChar(0, 1, aff7segC2, false);
    lcCentre.setChar(0, 2, aff7segC3, false);
    lcCentre.setChar(0, 3, aff7segC4, false); 
    lcCentre.setChar(0, 4, aff7segC5, false); 
    lcCentre.setChar(0, 5, aff7segC6, false); 
    lcCentre.setChar(1, 0, aff7segC7, false);
    lcCentre.setChar(1, 1, aff7segC8, false);
    lcCentre.setChar(1, 2, aff7segC9, false);
    lcCentre.setChar(1, 3, aff7segC10, false); 
    lcCentre.setChar(1, 4, aff7segC11, false); 
    lcCentre.setChar(2, 0, aff7segC12, false);
    lcCentre.setChar(2, 1, aff7segC13, false);
    lcCentre.setChar(2, 2, aff7segC14, false);
    lcCentre.setChar(2, 3, aff7segC15, false); 
    lcCentre.setChar(2, 4, aff7segC16, false); 
}

void refreshGauche(){

  swGauche0.begin(); 
  swGauche0.byteWrite(IODIRA, 0xFF);
  swGauche0.byteWrite(IODIRB, 0xFF);
  swGauche0.byteWrite(GPPUA, 0xFF);
  swGauche0.byteWrite(GPPUB, 0xFF);
  swGauche0A = swGauche0.byteRead(GPIOA);
  swGauche0B = swGauche0.byteRead(GPIOB);
  SPI.end();

  swGauche1.begin();
  swGauche1.byteWrite(IODIRA, 0xFF);
  swGauche1.byteWrite(IODIRB, 0xFF);
  swGauche1.byteWrite(GPPUA, 0xFF);
  swGauche1.byteWrite(GPPUB, 0xFF);
  swGauche1A = swGauche1.byteRead(GPIOA);
  swGauche1B = swGauche1.byteRead(GPIOB);
  SPI.end();

  Gauche[1] = bitRead(swGauche1B, 5)*128 + bitRead(swGauche1B, 4)*64 + bitRead(swGauche1A, 7)*32 + bitRead(swGauche1B, 7)*16 +
             bitRead(swGauche1A, 6)*8 + bitRead(swGauche1A, 5)*4 + bitRead(swGauche1A, 4)*2 + bitRead(swGauche1A, 3);

  Gauche[2] = bitRead(swGauche0B, 0)*128 + bitRead(swGauche0B, 1)*64 + bitRead(swGauche0B, 2)*32 + bitRead(swGauche0B, 3)*16 +
             bitRead(swGauche0B, 4)*8 + bitRead(swGauche1A, 0)*4 + bitRead(swGauche0A, 5)*2 + bitRead(swGauche0A, 7);

  Gauche[3] = bitRead(swGauche0A, 6)*128 + bitRead(swGauche0A, 4)*64 + bitRead(swGauche0A, 2)*32 + bitRead(swGauche0B, 7)*16 +
             bitRead(swGauche0B, 6)*8 + bitRead(swGauche0B, 5)*4 + bitRead(swGauche0A, 1)*2 + bitRead(swGauche0A, 0);

  Gauche[4] = bitRead(swGauche1A, 2)*128 + bitRead(swGauche1A, 1)*64 ;
  
  Gauche[0] = 0x00;
  Gauche[5] = 0xFF;

  LedControl lcGauche = LedControl(11, 13, 4, 1);
  lcGauche.shutdown(0, false); // activation module Gauche (U9)
  lcGauche.setIntensity(0, bright); 
  
}

void refreshCentre(){
  
  swCentre0.begin(); 
  swCentre0.byteWrite(IODIRA, 0xFF);
  swCentre0.byteWrite(IODIRB, 0xFF);
  swCentre0.byteWrite(GPPUA, 0xFF);
  swCentre0.byteWrite(GPPUB, 0xFF);
  swCentre0A = swCentre0.byteRead(GPIOA);
  swCentre0B = swCentre0.byteRead(GPIOB);
  SPI.end();

  swCentre1.begin();
  swCentre1.byteWrite(IODIRA, 0xFF);
  swCentre1.byteWrite(IODIRB, 0xFF);
  swCentre1.byteWrite(GPPUA, 0xFF);
  swCentre1.byteWrite(GPPUB, 0xFF);
  swCentre1A = swCentre1.byteRead(GPIOA);
  swCentre1B = swCentre1.byteRead(GPIOB);
  SPI.end();

  Centre[1] = bitRead(swCentre0A, 2)*128 + bitRead(swCentre0A, 3)*64 + bitRead(swCentre0A, 1)*32 + bitRead(swCentre0A, 0)*16 +
             bitRead(swCentre0A, 6)*8 + bitRead(swCentre0A, 7)*4 + bitRead(swCentre0A, 5)*2 + bitRead(swCentre0A, 4);

  Centre[2] = bitRead(swCentre0B, 3)*128 + bitRead(swCentre0B, 1)*64 + bitRead(swCentre0B, 4)*32 + bitRead(swCentre0B, 5)*16 +
             bitRead(swCentre0B, 2)*8 + bitRead(swCentre1A, 1)*4 + bitRead(swCentre1B, 1)*2 + bitRead(swCentre1B, 0);

  Centre[3] = bitRead(swCentre1B, 7)*128 + bitRead(swCentre1B, 6)*64 + bitRead(swCentre1B, 5)*32 + bitRead(swCentre1B, 4)*16 +
             bitRead(swCentre1A, 6)*8 + bitRead(swCentre1A, 7)*4 + bitRead(swCentre1A, 5)*2 + bitRead(swCentre1A, 4);

  Centre[4] = bitRead(swCentre1A, 3)*128 + bitRead(swCentre1A, 2)*64 + bitRead(swCentre0B, 7)*32 + bitRead(swCentre0B, 6)*16 +
             bitRead(swCentre1B, 2)*8 + bitRead(swCentre1B, 3)*4;
             
  Centre[0] = 0x01;
  Centre[5] = 0xFE;

  LedControl lcCentre = LedControl(11, 13, 8, 3);
  lcCentre.shutdown(0, false); // activation module Centre (U4)
  lcCentre.shutdown(1, false); // activation module Centre (U5)
  lcCentre.shutdown(2, false); // activation module Centre (U6)
  lcCentre.setIntensity(0, bright); 
  lcCentre.setIntensity(1, bright);
  lcCentre.setIntensity(2, bright);
}

void refreshDroite(){
  
  swDroite0.begin(); 
  swDroite0.byteWrite(IODIRA, 0xFF);
  swDroite0.byteWrite(IODIRB, 0xFF);
  swDroite0.byteWrite(GPPUA, 0xFF);
  swDroite0.byteWrite(GPPUB, 0xFF);
  swDroite0A = swDroite0.byteRead(GPIOA);
  swDroite0B = swDroite0.byteRead(GPIOB);
  SPI.end();

  swDroite1.begin();
  swDroite1.byteWrite(IODIRA, 0xFF);
  swDroite1.byteWrite(IODIRB, 0xFF);
  swDroite1.byteWrite(GPPUA, 0xFF);
  swDroite1.byteWrite(GPPUB, 0xFF);
  swDroite1A = swDroite1.byteRead(GPIOA);
  swDroite1B = swDroite1.byteRead(GPIOB);
  SPI.end();

  Droite[1] = bitRead(swDroite1B, 5)*128 + bitRead(swDroite1B, 4)*64 + bitRead(swDroite1A, 1)*32 + bitRead(swDroite1B, 7)*16 +
             bitRead(swDroite1A, 3)*8 + bitRead(swDroite1A, 2)*4 + bitRead(swDroite1A, 4)*2 + bitRead(swDroite1A, 5);

  Droite[2] = bitRead(swDroite0B, 4)*128 + bitRead(swDroite0B, 3)*64 + bitRead(swDroite0B, 2)*32 + bitRead(swDroite0B, 1)*16 +
             bitRead(swDroite0B, 0)*8 + bitRead(swDroite0A, 1)*4 + bitRead(swDroite0B, 7)*2 + bitRead(swDroite0B, 6);

  Droite[3] = bitRead(swDroite0B, 5)*128 + bitRead(swDroite0A, 0)*64 + bitRead(swDroite0A, 2)*32 + bitRead(swDroite0A, 5)*16 +
             bitRead(swDroite0A, 7)*8 + bitRead(swDroite0A, 6)*4 + bitRead(swDroite0A, 4)*2 + bitRead(swDroite0A, 3);

  Droite[4] = bitRead(swDroite1B, 1)*128 + bitRead(swDroite1B, 2)*64;
             
  Droite[0] = 0x02;
  Droite[5] = 0xFD;

  LedControl lcDroite = LedControl(11, 13, 7, 1);
  lcDroite.shutdown(0, false); // activation module Centre (U12)
  lcDroite.setIntensity(0, bright); 
}

