// #include <Arduino.h>
// #include <AccelStepper.h>

// // Tipo de driver: DRIVER = 1 (usa pines STEP y DIR)
// #define MOTOR_INTERFACE_TYPE 1

// // Pines del driver
// #define STEP_PIN 7
// #define DIR_PIN 23

// // Crear el objeto del motor
// AccelStepper stepper(MOTOR_INTERFACE_TYPE, STEP_PIN, DIR_PIN);

// void setup() {
//   // Configurar velocidad máxima y aceleración
//   stepper.setMaxSpeed(1000);      // pasos por segundo
//   stepper.setAcceleration(1000);  // pasos por segundo^2

//   // Posición inicial
//   stepper.setCurrentPosition(0);
// }

// void loop() {
//   // Mover 1000 pasos hacia adelante
//   stepper.moveTo(1000);
//   stepper.runToPosition();  // se bloquea hasta llegar

//   delay(1000);

//   // Mover 1000 pasos hacia atrás
//   stepper.moveTo(0);
//   stepper.runToPosition();

//   delay(1000);
// }





#include <Arduino.h>
#include <Wire.h>
#include "../robot/robot_instances.h"

void setup()
{
  Serial.begin(9600);
  sorter_.begin();
}

void loop()
{
  sorter_.moveToState(Sorter::SorterState::MATURE_POS_1);
  sorter_.update();
  delay(3000);
  
  sorter_.moveToState(Sorter::SorterState::MATURE_POS_2);
  sorter_.update();
  delay(3000);

  sorter_.moveToState(Sorter::SorterState::MATURE_POS_3);
  sorter_.update();
  delay(3000);

  sorter_.moveToState(Sorter::SorterState::OVERMATURE_POS_1); 
  sorter_.update();
  delay(3000);

  sorter_.moveToState(Sorter::SorterState::OVERMATURE_POS_2);
  sorter_.update();
  delay(3000);

  sorter_.moveToState(Sorter::SorterState::OVERMATURE_POS_3);
  sorter_.update();
  delay(3000);
}