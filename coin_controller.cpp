#include "coin_controller.h"

using namespace godot;

void CoinController::_register_methods() {
    register_method("_physics_process", &CoinController::_physics_process);
    register_method("_ready", &CoinController::_ready);
    register_method("del_ready", &CoinController::del_ready);
    register_method("coin_entered", &CoinController::coin_entered);
    register_property("rotate_speed", &CoinController::rotate_speed, 200);
}

CoinController::CoinController() {

}

CoinController::~CoinController() {

}

void CoinController::_init() {
    printf("Inited");
}

void CoinController::_physics_process(float delta) {
    rotate_y((rotate_speed * M_PI/180) * delta);
}

void CoinController::coin_entered(KinematicBody *body) {
    Godot::print("Area entered");
    Godot::print(body->get_name());
    queue_free();
    //Node *a = (Node *) area;
    //a -> queue_free();
}

void CoinController::_ready() {
    //connect("body_entered", this, "_on_Coin_area_entered");
}

void CoinController::del_ready() {
    Godot::print("Hurray");
}