#ifndef ENTITIES_PLAYER_HPP
#define ENTITIES_PLAYER_HPP

#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/character_body2d.hpp>
#include <godot_cpp/classes/sprite2d.hpp>

#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/vector2.hpp"

class Player : public godot::CharacterBody2D {
 public:
  [[nodiscard]] float GetSpeed() const { return speed_; }
  void SetSpeed(float speed) { speed_ = speed; }
  [[nodiscard]] float GetJumpStrength() const { return jump_strength_; }
  void SetJumpStrength(float jump_strength) { jump_strength_ = jump_strength; }

  void _ready() override;
  void _physics_process(double delta) override;

  void HandleInput();
  void UpdateAnimation();

 protected:
  static void _bind_methods();

 private:
  GDCLASS(Player, godot::CharacterBody2D)  // NOLINT

  godot::Vector2 vel_;
  float speed_ = 100.0F;
  float jump_strength_ = 300.0F;
  float direction_x_;
  godot::Sprite2D* upper_frame_;
  godot::Sprite2D* lower_frame_;
  godot::AnimationPlayer* lower_body_animation_;
  godot::Sprite2D* cross_hair_;
};

#endif  // ENTITIES_PLAYER_HPP
