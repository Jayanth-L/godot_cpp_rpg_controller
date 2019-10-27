#ifndef COIN_CONTROLLER_H
#define COIN_CONTROLLER_H

#include <Godot.hpp>
#include <Area.hpp>
#include <CollisionShape.hpp>
#include <KinematicBody.hpp>

namespace godot {
    class CoinController : public Area {
        GODOT_CLASS(CoinController, Area);
        private:
        public:
            int rotate_speed = 200;
            static void _register_methods();
            CoinController();
            ~CoinController();
            void _ready();
            void del_ready();
            void _init();
            void _physics_process(float delta);
            void coin_entered(KinematicBody *body);
    };
}

#endif