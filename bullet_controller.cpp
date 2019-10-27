#include "bullet_controller.h"

using namespace godot;

void BulletController::_register_methods() {
    register_method("_ready", &BulletController::_ready);
    register_method("_physics_process", &BulletController::_physics_process);
    register_property("bullet_speed", &BulletController::bullet_speed, 0);
    register_property("bullet_direction", &BulletController::bullet_direction, Vector3(0, 0, 0));
}

BulletController::BulletController() {}

BulletController::~BulletController() {}

void BulletController::_init() {

}

void BulletController::_ready() {
    
}

void BulletController::_physics_process(float delta) {
    move_and_collide(bullet_direction * bullet_speed * delta);
}