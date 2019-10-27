#ifndef BULLET_CONTROLLER_H
#define BULLET_CONTROLLER_H

#include <Godot.hpp>
#include <KinematicBody.hpp>
#include <KinematicCollision.hpp>

namespace godot {
    class BulletController : public KinematicBody {
        GODOT_CLASS(BulletController, KinematicBody);
        private:
        public:
            int bullet_speed;
            Vector3 bullet_direction;
            Vector3 bullet_translation;
            static void _register_methods();
            BulletController();
            ~BulletController();
            void _init();
            void _ready();
            void _physics_process(float delta);
    };
}

#endif