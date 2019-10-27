#include "elevator_controller.h"

using namespace godot;

void ElevatorController::_register_methods() {
    register_method("_ready", &ElevatorController::_ready);
    register_method("_physics_process", &ElevatorController::_physics_process);
    register_property("speed", &ElevatorController::speed, 0.0f);
    register_property("height", &ElevatorController::height, 0.0f);
}

ElevatorController::ElevatorController() {}
ElevatorController::~ElevatorController() {}

void ElevatorController::_init() {}

void ElevatorController::_ready() {
    init_translation = get_translation();
}

void ElevatorController::_physics_process(float delta) {
    if (is_forward) {
        if (get_translation().y < (init_translation.y + height)) {
            //move_and_collide(Vector3(0, 1, 0) * speed * delta);
            set_translation(Vector3(init_translation.x, get_translation().y + speed * delta, init_translation.z));
        } else {
            is_forward = false;
        }
    } else if (!is_forward) {
        if(get_translation().y > init_translation.y) {
            //move_and_collide(Vector3(0, -1, 0) * speed * delta);
            set_translation(Vector3(init_translation.x, get_translation().y - speed * delta, init_translation.z));
        } else {
            is_forward = true;
        }
    }
}
