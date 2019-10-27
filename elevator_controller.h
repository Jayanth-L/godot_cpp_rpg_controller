#ifndef ELEVATOR_CONTROLLER_H
#define ELEVATOR_CONTROLLER_H

#include <Godot.hpp>
#include <KinematicBody.hpp>
#include <KinematicCollision.hpp>

namespace godot {
    class ElevatorController : public KinematicBody {
        GODOT_CLASS(ElevatorController, KinematicBody);
    private:
        /* data */
    public:

        /* Variables Declarations */
        float height = 0.0;
        float speed = 0.0;
        bool is_forward = true;
        Vector3 init_translation;
        /* Functions Declarations */
        ElevatorController();
        ~ElevatorController();
        static void _register_methods();
        void _init();
        void _ready();
        void _physics_process(float delta);
    };
}

#endif