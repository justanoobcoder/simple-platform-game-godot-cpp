#include "drone.hpp"

#include "godot_cpp/classes/animated_sprite2d.hpp"
#include "godot_cpp/classes/animation_player.hpp"
#include "godot_cpp/classes/collision_shape2d.hpp"
#include "godot_cpp/classes/sprite2d.hpp"
#include "godot_cpp/variant/callable_method_pointer.hpp"
#include "godot_cpp/variant/color.hpp"
#include "godot_cpp/variant/string_name.hpp"
#include "godot_cpp/variant/vector2.hpp"

void Drone::_bind_methods() {}

void Drone::_ready() {
  auto* attack_range = get_node_or_null("AttackRange");
  if (attack_range) {
    attack_range->connect("body_entered", callable_mp(this, &Drone::OnPlayerInAttackRangeEntered));
    attack_range->connect("body_exited", callable_mp(this, &Drone::OnPlayerInAttackRangeExited));
  }
  auto* explode_range = get_node_or_null("Explosion/ExplodeRange");
  if (explode_range) {
    explode_range->connect("body_entered",
                           callable_mp(this, &Drone::OnPlayerInExplodeRangeEntered));
  }
}

void Drone::_physics_process(double) {
  if (player_) {
    direction_ = (player_->get_position() - get_position()).normalized();
  }
  set_velocity(direction_ * speed_);
  move_and_slide();
}

void Drone::Explode() {
  // stop tracking and following player after exploded
  player_ = nullptr;
  direction_ = godot::Vector2();

  auto* collision_shape = get_node<godot::CollisionShape2D>("CollisionShape2D");
  if (collision_shape) {
    collision_shape->set_deferred("disabled", true);
  }

  auto* drone_frame = get_node<godot::AnimatedSprite2D>("AnimatedSprite2D");
  auto* explode_frame = get_node<godot::Sprite2D>("Explosion/ExplodeFrame");
  auto* explode_animation = get_node<godot::AnimationPlayer>("Explosion/ExplodeAnimation");
  if (drone_frame && explode_frame && explode_animation) {
    drone_frame->hide();
    explode_frame->show();
    explode_animation->play("explode");
    explode_animation->connect("animation_finished",
                               callable_mp(this, &Drone::OnExplodeAnimationFinished));
  }
}

void Drone::TakeDamage(godot::Vector2 dir) {
  auto* drone_frame = get_node<godot::AnimatedSprite2D>("AnimatedSprite2D");
  if (drone_frame) {
    auto tween = create_tween();
    tween->tween_property(drone_frame, "modulate", godot::Color(1.0F, 0.29F, 0.204F), 0.2F);
    tween->tween_property(drone_frame, "modulate", godot::Color(1.0F, 1.0F, 1.0F), 0.2F);
  }

  health_ -= 1;
  move_and_collide(dir * 5);
  if (health_ == 0) {
    Explode();
  }
}

void Drone::OnPlayerInAttackRangeEntered(godot::Node2D* body) {
  player_ = godot::Object::cast_to<CharacterBody2D>(body);
}

void Drone::OnPlayerInAttackRangeExited(godot::Node2D*) {
  player_ = nullptr;
  direction_ = godot::Vector2();
}

void Drone::OnPlayerInExplodeRangeEntered(godot::Node2D*) { Explode(); }

void Drone::OnExplodeAnimationFinished(const godot::StringName&) { queue_free(); }
