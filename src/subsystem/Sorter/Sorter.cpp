#include "Sorter.hpp"

Sorter::Sorter()
    : stepper{kMotorInterfaceType, Pins::kSorterStepPin, Pins::kSorterDirPin}
       {}

void Sorter::begin() {
    stepper.setMaxSpeed(kMaxStepperSpeed);
    stepper.setAcceleration(kStepperAcceleration);
    stepper.setCurrentPosition(0);
}
    
void Sorter::update() {
    stepper.runToPosition();
}

void Sorter::setState(int state) {
    moveToState(static_cast<Sorter::SorterState>(state));
}

// void Sorter::moveToState(Sorter::SorterState target_state) {
//     // Convertir el estado lógico (enum) a índice numérico
//     int target_index = static_cast<int>(target_state);

//     // Calcular la posición absoluta en pasos
//     long target_steps = static_cast<long>(target_index) * static_cast<long>(Sorter::kStepsPerPosition);

//     // Obtener la posición actual del stepper
//     long current_steps = stepper.currentPosition();
//     bool forward = (target_steps > current_steps);

//     // Configurar la dirección física
//     setDirection(forward);

//     // Comandar movimiento absoluto
//     stepper.moveTo(target_steps);

//     // Guardar el estado lógico solicitado
//     sorter_state_ = target_state;

//     // Si el motor no está corriendo, hacer un “hard brake” (opcional)
//     if (!isRunning()) {
//         stepperHardBrake();
//     }
// }

void Sorter::moveToState(Sorter::SorterState target_state) {
    int target_index = static_cast<int>(target_state);
    long target_steps = static_cast<long>(target_index) * static_cast<long>(Sorter::kStepsPerPosition);
    // schedule movement
    stepper.moveTo(target_steps);
    // store desired logical state (note: this is the requested state; the physical position will be reached asynchronously)
    sorter_state_ = target_state;

    if (!isRunning()) {
        stepperHardBrake();
    }
}

bool Sorter::isRunning() {
    return stepper.distanceToGo() != 0;
} 

void Sorter::stepperHardBrake() {
    stepper.stop();
    stepper.setCurrentPosition(stepper.currentPosition());
    stepper.move(0);
}

void Sorter::setDirection(bool forward) {
    digitalWrite(Pins::kSorterDirPin, forward ? HIGH : LOW);
    delayMicroseconds(5);
}
