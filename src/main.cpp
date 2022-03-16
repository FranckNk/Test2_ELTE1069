/*

TITRE          : Control Servo Moteur avec Detecteur de Rotation. Test2 de ELTE1069
AUTEUR         : Franck Nkeubou Awougang
DATE           : 16/03/2022
DESCRIPTION    : Programme pour le Test de ELTE permettant de controler un système de fuite
					d'eau dans un sous-sol avec un capteur (SEN0223) et un relai pour actionner la
					pompe d'évacuation d'eau.
VERSION        : 0.0.1

*/
#include <Arduino.h>
#include "WIFIConnector_MKR1000.h"
#include "MQTTConnector.h"
#include "Timer.h"

// Déclaration des constantes.
const int PIN_RELAI = 2;
const int PIN_SENSOR_SWITCH = 4;

// Déclaration des variables.
bool EtatPompe = false; // La pompe d'évacuation est fermée.
bool EtatRelai = false;

Timer DelaiEnvoi;
unsigned long DelaiAttente = 5000;
// Déclaration des fonctions.
void ActivationPompe();

void setup() {
	//Initialisation du wifi.
	wifiConnect();                  //Branchement au réseau WIFI
	MQTTConnect();                  //Branchement au broker MQTT
	// initialisation du moniteur serie.
	Serial.begin(9600);
	// configuration du mode des broches.
	pinMode(PIN_RELAI, OUTPUT);
	pinMode(PIN_SENSOR_SWITCH, INPUT);
	attachInterrupt(digitalPinToInterrupt(PIN_SENSOR_SWITCH), ActivationPompe, CHANGE);
	DelaiEnvoi.startTimer(DelaiAttente);
	EtatPompe = digitalRead(PIN_SENSOR_SWITCH); // L'état par défaut de la pompe est la valeur lue par le capteur.
}

void loop() {
	// put your main code here, to run repeatedly:
	if (EtatPompe)
	{
		digitalWrite(PIN_RELAI, HIGH);	
		EtatRelai = true;	
	}
	else{
		digitalWrite(PIN_RELAI, LOW);
		EtatRelai = false;
	}
	if (DelaiEnvoi.isTimerReady())
	{
		appendPayload("EtatPompe", EtatPompe);
		appendPayload("EtatRelai", EtatRelai);
		sendPayload();
		DelaiEnvoi.startTimer(DelaiAttente);
	}
}

void ActivationPompe(){
	EtatPompe = !EtatPompe;
}
