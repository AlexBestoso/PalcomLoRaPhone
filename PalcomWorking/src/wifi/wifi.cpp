#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>

#include <TFT_eSPI.h>
#include <RadioLib.h>
#include <lvgl.h>
#include <string>
#include <cstdint>

#include <mbedtls/md.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/bignum.h>
#include <mbedtls/x509.h>
#include <mbedtls/rsa.h>
#include <mbedtls/aes.h>

#include "./wifi.h"

PalcomWifi::PalcomWifi(void){
	this->ssid = "";
	this->password = "";
	this->targetHost = "";
	targetPort = 0;
}

PalcomWifi::~PalcomWifi(){}


void PalcomWifi::init(void){
	if(this->ssid == "" && this->password == "") return;
	WiFi.begin(this->ssid, this->password);
}

void PalcomWifi::denit(void){
	WiFi.disconnect();
}
