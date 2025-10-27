// #include <Arduino.h>

// #define STEP_PIN 7
// #define DIR_PIN 23

// #define STEPS_PER_REV 200 // ajusta según tu driver/microstepping

// void stepMotor(int dir, int steps, int delay_us) {
//   digitalWrite(DIR_PIN, dir);
//   for (int i = 0; i < steps; i++) {
//     digitalWrite(STEP_PIN, HIGH);
//     delayMicroseconds(delay_us);
//     digitalWrite(STEP_PIN, LOW);
//     delayMicroseconds(delay_us);
//   }
// }

// void setup() {
//   pinMode(STEP_PIN, OUTPUT);
//   pinMode(DIR_PIN, OUTPUT);
// }

// void loop() {
//   // CW - lento
//   stepMotor(HIGH, STEPS_PER_REV, 3000);
//   delay(3000);

//   // // CCW - lento
//   // stepMotor(LOW, STEPS_PER_REV, 3000);
//   // delay(1000);
// }





// #include <Arduino.h>
// #include "AccelStepper.h"

// // Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver:
// #define dirPin 23
// #define stepPin 7
// #define motorInterfaceType 1

// // Create a new instance of the AccelStepper class:
// AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

// void setup() {
//   // Set the maximum speed in steps per second:
//   stepper.setMaxSpeed(1500);
// }

// void loop() {
//   // Set the speed in steps per second:
//   stepper.setSpeed(1500);
//   stepper.move(3000);
//   // while (stepper.distanceToGo() != 0) {
//   //   stepper.run();
//   // }
//   // // Step the motor with a constant speed as set by setSpeed():
//   // stepper.stop();

//   delay(3000);
// }









// #include <Arduino.h>
// // Pines del driver
// #define DIR_PIN 23
// #define STEP_PIN 7

// // Configuración de movimiento
// #define STEP_DELAY 1800  // microsegundos entre pasos (controla la velocidad)
// #define STEPS_PER_REV 200  // pasos por revolución (NEMA 17 típico)

// // Variables
// int direction = 1;  // 1 = adelante, -1 = atrás

// void moveSteps(int steps) {
//   for (int i = 0; i < steps; i++) {
//     digitalWrite(STEP_PIN, HIGH);
//     delayMicroseconds(STEP_DELAY);
//     digitalWrite(STEP_PIN, LOW);
//     delayMicroseconds(STEP_DELAY);
//   }
// }

// void setup() {
//   pinMode(DIR_PIN, OUTPUT);
//   pinMode(STEP_PIN, OUTPUT);

//   digitalWrite(DIR_PIN, LOW);
//   Serial.begin(9600);
//   Serial.println("Motor listo");
// }

// void loop() {
//   // Mueve una vuelta completa hacia un lado
//   moveSteps(STEPS_PER_REV);
//   delay(2000);

//   // Cambia la dirección
//   direction *= -1;
//   digitalWrite(DIR_PIN, (direction == 1) ? HIGH : LOW);

//   // Mueve otra vuelta al otro lado
//   moveSteps(STEPS_PER_REV);
//   delay(2000);
// }






// #include <Arduino.h>
// #include <Stepper.h>
// #include <util/atomic.h>

// #define OUTPUT1   23                
// #define OUTPUT2   7               

// const int stepsPerRotation = 1000;  
// Stepper myStepper(stepsPerRotation, OUTPUT1, OUTPUT2);  


// void rotateStepper(int steps, int speed) {
//   myStepper.setSpeed(speed);
//   myStepper.step(steps);
//   delay(1000);
// }

// void setup() {
//   Serial.begin(9600);

//   Serial.println("Motores listos.");
//   myStepper.setSpeed(15);  // 15 RPM
// }

// void loop() {
//   rotateStepper(stepsPerRotation, 15);  // Rotación en un sentido
//   delay(2000);
// }












// #include <Arduino.h>
// #include <AccelStepper.h>

// // Pines (ajusta según tu conexión)
// const uint8_t PIN_STEP = 7;   // STEP -> driver
// const uint8_t PIN_DIR  = 23;   // DIR  -> driver

// // Crear motor (interface DRIVER usa STEP/DIR)
// AccelStepper motor(AccelStepper::DRIVER, PIN_STEP, PIN_DIR);

// void setup() {
//   // Configuración del motor
//   motor.setMaxSpeed(2000.0);     // pasos/segundo (ajústalo a tu motor)
//   motor.setAcceleration(1000.0); // pasos/segundo^2

//   Serial.begin(115200);
//   Serial.println("TMC2208 + AccelStepper sin ENABLE listo");
// }

// long targetPosition = 0;

// void loop() {
//   // Movimiento simple de ida y vuelta
//   if (!motor.isRunning()) {
//     if (targetPosition == 0) {
//       targetPosition = 2000;       // mueve 2000 pasos adelante
//       motor.moveTo(targetPosition);
//       Serial.println("Mover a +2000");
//     } else {
//       targetPosition = 0;          // vuelve a 0
//       motor.moveTo(targetPosition);
//       Serial.println("Mover a 0");
//     }
//   }

//   // Ejecutar movimiento
//   motor.run();
// }






#include <Arduino.h>
#include <AccelStepper.h>

// Tipo de driver: DRIVER = 1 (usa pines STEP y DIR)
#define MOTOR_INTERFACE_TYPE 1

// Pines del driver
#define STEP_PIN 7
#define DIR_PIN 23

// Crear el objeto del motor
AccelStepper stepper(MOTOR_INTERFACE_TYPE, STEP_PIN, DIR_PIN);

void setup() {
  // Configurar velocidad máxima y aceleración
  stepper.setMaxSpeed(1000);      // pasos por segundo
  stepper.setAcceleration(1000);  // pasos por segundo^2

  // Posición inicial
  stepper.setCurrentPosition(0);
}

void loop() {
  // Mover 1000 pasos hacia adelante
  stepper.moveTo(1000);
  stepper.runToPosition();  // se bloquea hasta llegar

  delay(1000);

  // Mover 1000 pasos hacia atrás
  stepper.moveTo(0);
  stepper.runToPosition();

  delay(1000);
}