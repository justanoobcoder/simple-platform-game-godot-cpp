#include "player.hpp"

#include <cmath>
#include <numbers>

#include "godot_cpp/classes/animation_player.hpp"
#include "godot_cpp/classes/input.hpp"
#include "godot_cpp/classes/sprite2d.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/math.hpp"
#include "godot_cpp/core/object.hpp"
#include "godot_cpp/variant/variant.hpp"
#include "godot_cpp/variant/vector2.hpp"

using godot::AnimationPlayer;
using godot::ClassDB;
using godot::D_METHOD;
using godot::Input;
using godot::PropertyInfo;
using godot::Sprite2D;
using godot::Variant;
using godot::Vector2;

void Player::_bind_methods() {
  ClassDB::bind_method(D_METHOD("set_speed", "speed"), &Player::SetSpeed);
  ClassDB::bind_method(D_METHOD("get_speed"), &Player::GetSpeed);
  ClassDB::bind_method(D_METHOD("set_jump_strength", "jump_strength"), &Player::SetJumpStrength);
  ClassDB::bind_method(D_METHOD("get_jump_strength"), &Player::GetJumpStrength);

  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "jump_strength"), "set_jump_strength",
               "get_jump_strength");

  ADD_SIGNAL(godot::MethodInfo("shoot", PropertyInfo(Variant::VECTOR2, "pos"),
                               PropertyInfo(Variant::VECTOR2, "dir")));
}

void Player::_ready() {
  vel_ = get_velocity();
  upper_frame_ = get_node<Sprite2D>("Body/UpperFrame");
  lower_frame_ = get_node<Sprite2D>("Body/LowerFrame");
  lower_body_animation_ = get_node<AnimationPlayer>("Body/LowerAnimation");
  cross_hair_ = get_node<Sprite2D>("Crosshair");
}

void Player::_physics_process(double delta) {
  if (is_knocked_back_) {
    vel_.x = godot::Math::move_toward(vel_.x, 0.0F, 600.0F * static_cast<float>(delta));
    if (vel_.x == 0.0F) {
      is_knocked_back_ = false;
    }
  } else {
    HandleInput();
    vel_.x = direction_x_ * speed_;
  }
  if (!is_on_floor()) {
    vel_ += get_gravity() * delta;
  }
  UpdateAnimation();
  set_velocity(vel_);
  move_and_slide();
}

void Player::HandleInput() {
  auto* input = Input::get_singleton();
  direction_x_ = input->get_axis("left", "right");
  if (input->is_action_just_pressed("jump") && is_on_floor()) {
    vel_.y = -jump_strength_;
  }
  if (input->is_action_just_pressed("shoot")) {
    emit_signal("shoot", get_position(), get_local_mouse_position().normalized());
    if (cross_hair_) {
      auto tween = create_tween();
      tween->tween_property(cross_hair_, "scale", Vector2(0.1F, 0.1F), 0.2F);
      tween->tween_property(cross_hair_, "scale", Vector2(0.4F, 0.4F), 0.2F);
    }
  }
}

void Player::UpdateAnimation() {
  cross_hair_->set_position(get_local_mouse_position().normalized() * 30);

  // voodoo magic
  const auto aim = get_local_mouse_position();
  upper_frame_->set_frame(godot::Math::posmod(std::round(aim.angle() / (std::numbers::pi / 4)), 8));

  if (!is_on_floor()) {
    lower_body_animation_->play("jump");
    return;
  }

  lower_body_animation_->play(direction_x_ != 0.0F ? "run" : "idle");
  lower_frame_->set_flip_h(direction_x_ < 0.0F);
}

void Player::KnockBack(Vector2 dir) {
  if (is_knocked_back_) return;
  is_knocked_back_ = true;
  direction_x_ = 0.0F;
  vel_ = Vector2(get_position().x > dir.x ? -150 : 150, -150);
}
