/**
 * LARC 2025
 *
 * @Roborregos
 */

#include <Arduino.h>
#include <Wire.h>

#include "robot/robot_instances.h"
#include "robot/StateMachine.hpp"
#include <SoftwareSerial.h>
#include "../../lib/controllers/PIDController.hpp"

// --- Bluetooth Setup ---
SoftwareSerial bluetooth(0, 1); // RX, TX pins for Bluetooth module

// State machine instance
StateMachine stateMachine(bluetooth);

// PID controllers for distance control
PIDController leftDistancePID(10, 0.00, 0.0, -150.0, 150.0);  // kp, ki, kd, min, max
PIDController rightDistancePID(10, 0.00, 0.0, -150.0, 150.0); // kp, ki, kd, min, max

// Target distance in cm
const float TARGET_DISTANCE = 16.0;
// Mode flag: when true, use ONLY left distance for control
const bool USE_LEFT_ONLY = true;

void setup()
{
  Serial.begin(9800);
  bluetooth.begin(9800);
  Serial.println("Starting...");
  Wire.begin();

  // All systems must begin after initializing the serial and as the code starts running
  drive_.begin();
  com_.begin();
  line_sensor_.begin();
  distance_sensor_.begin();
  drive_.setState(0);
  drive_.acceptHeadingInput(Rotation2D::fromDegrees(0));

  // Initialize state machine
  stateMachine.begin();
  // Wait for "ready" message from Bluetooth before continuing
  String btInput = "";
  while (true)
  {
    if (bluetooth.available())
    {
      char c = bluetooth.read();
      if (c == '\n' || c == '\r')
      {
        btInput.trim();
        if (btInput.equalsIgnoreCase("ready"))
        {
          Serial.println("Bluetooth ready received.");
          break;
        }
        btInput = "";
      }
      else
      {
        btInput += c;
      }
    }
  }
}

void loop()
{
  // Get current distance readings
  auto distanceValues = distance_sensor_.getArrayDistance();
  float leftDistance = distanceValues[0];
  float rightDistance = distanceValues[1];

  if (USE_LEFT_ONLY)
  {
    // LEFT-ONLY CONTROL: maintain 16cm to the left obstacle using lateral movement only
    float leftOnlyOutput = leftDistancePID.update(leftDistance, TARGET_DISTANCE);

    // Map PID to lateral (vx). Sign chosen consistent with forward sign flip
    float vx = 75.0;  // strafe left/right to correct left distance
    float vy = -leftOnlyOutput;              // no forward motion in left-only mode

    drive_.acceptInput(vx, vy, 0.0);

    // Debug
    bluetooth.print("[LEFT-ONLY] L: ");
    bluetooth.print(leftDistance);
    bluetooth.print("cm (out: ");
    bluetooth.print(leftOnlyOutput);
    bluetooth.println(")");
  }
  else
  {
    // DUAL-SENSOR CONTROL (existing): maintain ~16cm on both sensors
    // Calculate PID outputs for both sensors
    float leftOutput = leftDistancePID.update(leftDistance, TARGET_DISTANCE);
    float rightOutput = rightDistancePID.update(rightDistance, TARGET_DISTANCE);

    // Calculate average output for forward/backward movement
    float forwardOutput = (leftOutput + rightOutput) / 2.0 * -1;

    // Calculate difference for left/right movement (to center between obstacles)
    float lateralOutput = (rightDistance - leftDistance) * 0.1; // Simple proportional control for centering

    // Apply the control outputs to the drive system
    // forwardOutput controls forward/backward movement
    // lateralOutput controls left/right movement
    drive_.acceptInput(lateralOutput, forwardOutput, 0.0);

    // Debug output
    bluetooth.print("L: ");
    bluetooth.print(leftDistance);
    bluetooth.print("cm (");
    bluetooth.print(leftOutput);
    bluetooth.print(") R: ");
    bluetooth.print(rightDistance);
    bluetooth.print("cm (");
    bluetooth.print(rightOutput);
    bluetooth.print(") Fwd: ");
    bluetooth.print(forwardOutput);
    bluetooth.print(" Lat: ");
    bluetooth.println(lateralOutput);
  }

  // Update drive system
  drive_.update();

  delay(20); // Control loop frequency
}