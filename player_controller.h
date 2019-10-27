#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <Godot.hpp>
#include <KinematicBody.hpp>
#include <Camera.hpp>
#include <Input.hpp>
#include <Animation.hpp>
#include <AnimationPlayer.hpp>
#include <AnimationTree.hpp>
#include <ResourceLoader.hpp>
#include <PackedScene.hpp>

namespace godot {
    class PlayerController : public KinematicBody {
        GODOT_CLASS(PlayerController, KinematicBody)
    private:
    float time_passed;
    public:

        // Variables
        int turn_speed = 40;
        const int ANIM_FLOOR = 0;
        const int ANIM_AIR_UP = 1;
        const int ANIM_AIR_DOWN = 2;

        const float SHOOT_TIME = 1.5;
        const int SHOOT_SCALE = 2;
        AnimationPlayer* animation_player;
        AnimationTree *animation_tree;
        float animation_tree_value = 0.0;

        Vector3 facing_dir = Vector3(1, 0, 0);
        Vector3 movement_dir = Vector3();

        Vector3 init_translation;
        Vector3 init_rotation;

        bool jumping = false;

        bool keep_jump_inertia = true;
        bool air_idle_deaccel = false;
        float accel = 19.0;
        float deaccel = 14.0;
        int sharp_turn_threshold = 140;

        float max_speed = 3.1;
        bool prev_shoot = false;
        Vector3 linear_velocity = Vector3();

        float shoot_blend = 0.0;

        static void _register_methods();
        PlayerController();
        ~PlayerController();
        void _init();
        Vector3 adjust_facing(Vector3 p_facing, Vector3 p_target, float p_step, float p_adjust_rate, Vector3 current_gn);
        void _physics_process(float delta);
        void _ready();
        void play_animation(String animation);
        void spawn_bullet(float speed);
    };
}

#endif