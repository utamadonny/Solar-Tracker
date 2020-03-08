//Power Meter	Tugas EBT 20 Oktober 2019
// Donny Prakarsa Utama

#include <Wire.h>   //inisialisasi dipendencies
#include <Adafruit_INA219.h> //library sensor
#include <Adafruit_SSD1306.h> // library OLED
#include <SPI.h>    // SPI 
#include "SdFat.h"  // SD card
SdFat SD;  // Ubah SdFat jadi SD

#define OLED_RESET 4 //pin OLED 4
Adafruit_SSD1306 display(OLED_RESET); 
Adafruit_INA219 ina219;

unsigned long previousMillis = 0;   // set timer dengan fungsi milis (milisecond) ms
unsigned long interval = 100;   // set timer untuk bekerja tiap intervar 100ms
const int chipSelect = 10;     // pin  sd card  CS
float shuntvoltage = 0;        // nilai awal
float busvoltage = 0;                // nilai awal
float current_mA = 0;       // nilai awal
float loadvoltage = 0;       // nilai awal
float energy = 0;       // nilai awal
File TimeFile;    //nama file yang mau disimpan
File VoltFile;   //nama file yang mau disimpan
File CurFile;   //nama file yang mau disimpan

void setup() {
  SD.begin(chipSelect);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  ina219.begin();
}

void loop() {
  unsigned long currentMillis = millis();       // timer dengan fungsi milis current time merupakan fungsi milis
  if (currentMillis - previousMillis >= interval) // ketika waktu currentMillis - previousMillis >= interval maka eksekusi penyimpanan file
  {
    previousMillis = currentMillis;    // ubah previousMillis=currentMillis supaya mereset nilai 
    ina219values();                     // melakukan kalkulasi tegangan dan arus

    TimeFile = SD.open("TIME.txt", FILE_WRITE);   // simpan file
    if (TimeFile) {
      TimeFile.println(currentMillis);
      TimeFile.close();
    }

    VoltFile = SD.open("VOLT.txt", FILE_WRITE);    //simpan file
    if (VoltFile) {
      VoltFile.println(loadvoltage);
      VoltFile.close();
    }

    CurFile = SD.open("CUR.txt", FILE_WRITE);      //simpan file
    if (CurFile) {
      CurFile.println(current_mA);
      CurFile.close();
    }
    displaydata();    //tampilkan pada layar OLED
  }
}

void displaydata() {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(loadvoltage);
  display.setCursor(35, 0);
  display.println("V");
  display.setCursor(50, 0);
  display.println(current_mA);
  display.setCursor(95, 0);
  display.println("mA");
  display.setCursor(0, 10);
  display.println(loadvoltage * current_mA);
  display.setCursor(65, 10);
  display.println("mW");
  display.setCursor(0, 20);
  display.println(energy);
  display.setCursor(65, 20);
  display.println("mWh");
  display.display();
}

void ina219values() {
  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  energy = energy + loadvoltage * current_mA / 3600;
}




