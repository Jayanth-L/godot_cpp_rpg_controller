#include "player_controller.h"

using namespace godot;

void PlayerController::_register_methods() {
    register_method("_physics_process", &PlayerController::_physics_process);
    register_method("_ready", &PlayerController::_ready);
}

PlayerController::PlayerController() {

}

PlayerController::~PlayerController() {

}

/**
 * Init
 */

void PlayerController::_init() {
    time_passed = 0.0;
    animation_tree_value = 0.0;
}

Vector3 PlayerController::adjust_facing(Vector3 p_facing, Vector3 p_target, float p_step, float p_adjust_rate, Vector3 current_gn) {
    Vector3 n = p_target;
    Vector3 t = n.cross(current_gn).normalized();
    float x = n.dot(p_facing);
    float y = t.dot(p_facing);

    int ang = atan2(y, x);
    if (abs(ang) < 0.001) {
        return p_facing;
    }


    int s;
    if (ang > 0) {
        s = 1;
    } else if(ang == 0) {
        s = 0;
    } else {
        s = -1;
    }
    ang = ang * s;
    int turn = ang * p_adjust_rate * p_step;

    int a;
    if (ang < turn) {
        a = ang;
    } else {
        a = turn;
    }
    ang = (ang - a) * s;

    return (n * cos(ang) + t * sin(ang)) * p_facing.length();
}

void PlayerController::_ready() {
    init_translation = get_translation();
    init_rotation = get_rotation();

    animation_player = (AnimationPlayer *) get_node("AnimationPlayer");
    animation_tree = (AnimationTree *) get_node("AnimationTree");
    animation_tree -> set("parameters/blend_position", animation_tree_value);
    // play_animation("idle");
}

void PlayerController::_physics_process(float delta) {
    Vector3 lv =  linear_velocity;
    Vector3 g = Vector3(0, -9.8, 0);

    lv += g * delta;

    int anim = ANIM_FLOOR;
    Vector3 up = -g.normalized();
    float vv = up.dot(lv);
    Vector3 hv = lv - up * vv;

    Vector3 hdir = hv.normalized();
    float hspeed = hv.length();

    Vector3 dir = Vector3();
    /* 
    Spatial *camera = (Spatial *) get_node("target/Camera");
    Transform cam_xform = camera -> get_global_transform();*/

    
    Input* input = Input::get_singleton();
    bool up_input = input -> is_action_pressed("ui_up");
    bool down_input = input -> is_action_pressed("ui_down");
    bool right_input = input -> is_action_pressed("ui_right");
    bool left_input = input -> is_action_pressed("ui_left");

    /*
    bool right_input_just = input -> is_action_just_pressed("ui_right");
    bool left_input_just = input -> is_action_just_pressed("ui_left");

    if(right_input_just) {
        Vector3 current_rot = get_rotation();
        set_rotation(Vector3(0, current_rot.y - M_PI/2, 0));
    } else if(left_input_just) {
        Vector3 current_rot = get_rotation();
        set_rotation(Vector3(0, current_rot.y + M_PI/2, 0));
    }*/

    if (up_input) {
        // dir += -cam_xform.basis[2];
        dir += get_transform().basis.z.normalized() * 2;
    }
    if (down_input) {
        // dir += cam_xform.basis[2];
        dir += -get_transform().basis.z.normalized() * 2;
    }
    if (left_input) {
        // dir += -cam_xform.basis[0];
        dir += get_transform().basis.x.normalized() * 2;
    }
    if (right_input) {
        dir += -get_transform().basis.x.normalized() * 2;
        // dir += cam_xform.basis[0];
    }

    if (down_input || up_input || left_input || right_input) {
        if (animation_tree_value < 1.0) {
            animation_tree_value += 0.04;        
        }
        // play_animation("walking");
    } else {
        // play_animation("idle");
        if(animation_tree_value > 0.0) {
            animation_tree_value -= 0.04;
        }
    }

    animation_tree -> set("parameters/blend_position", animation_tree_value);


    bool jump_attempt = input -> is_action_pressed("jump");
    bool shoot_attempt = input -> is_action_pressed("shoot");

    Vector3 target_dir = (dir - up * dir.dot(up)).normalized();

    if (is_on_floor()) {
        bool sharp_turn = hspeed > 0.1 and (acos(target_dir.dot(hdir))) * 180/M_PI > sharp_turn_threshold;

        if (dir.length() > 0.1 and !sharp_turn) {
            if(hspeed > 0.001) {
                hdir = adjust_facing(hdir, target_dir, delta, 1.0/hspeed * turn_speed, up);
                facing_dir = hdir;
            } else {
                hdir = target_dir;
            }

            if (hspeed < max_speed) {
                hspeed += accel * delta;
            } 
        } else {
            hspeed -= deaccel * delta;
            if (hspeed < 0) {
                hspeed = 0;
            }
        }

        hv = hdir * hspeed;
        
        if (!jumping && jump_attempt) {
            vv = 7.0;
            jumping = true;
        }
    } else {
        if (vv > 0) {
            anim = ANIM_AIR_UP;
        } else {
            anim = ANIM_AIR_DOWN;
        }

        if (dir.length() > 0.1) {
            hv += target_dir * (accel * 0.2) * delta;
            if (hv.length() > max_speed) {
                hv = hv.normalized() * max_speed;
            }
        } else {
            if (air_idle_deaccel) {
                hspeed = hspeed - (deaccel * 0.2) * delta;
                if (hspeed < 0) {
                    hspeed = 0;
                }
                hv = hdir * hspeed;
            }
        }
    }

    if (jumping and vv) {
        jumping = false;
    }

    lv = hv + up * vv;

    if (is_on_floor()) {
        movement_dir = lv;
    }

    linear_velocity = move_and_slide(lv, -g.normalized());
}

void PlayerController::play_animation(String animation) {
    if (animation == "walking") {
        animation_player -> play("lp_guy|Run");
    } else if(animation == "idle") {
        animation_player -> play("lp_guy|Idle");
    }
}

void PlayerController::spawn_bullet(float speed) {
    ResourceLoader *resource_loader = ResourceLoader::get_singleton();
    Ref<PackedScene> res = resource_loader->load("res://ModelScenes/Bullet.tscn");
    KinematicBody* body = (KinematicBody *) res -> instance();
    body -> set_translation(get_transform().origin + Vector3(1.409, 1.255, 0));
    body -> set("bullet_direction", Vector3(1, 0, 0));
    body -> set("bullet_speed", speed);
    body -> set_scale(Vector3(0.1, 0.1, 0.1));
    get_parent() ->add_child(body);
}