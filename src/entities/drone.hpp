#ifndef ENTITIES_DRONE_HPP
#define ENTITIES_DRONE_HPP

#include <godot_cpp/classes/character_body2d.hpp>

#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/vector2.hpp"

class Drone : public godot::CharacterBody2D {
 public:
  void _ready() override;
  void _physics_process(double delta) override;

  void TakeDamage(godot::Vector2 dir);
  void Explode();

  void OnPlayerInAttackRangeEntered(godot::Node2D* body);
  void OnPlayerInAttackRangeExited(godot::Node2D* body);
  void OnPlayerInExplodeRangeEntered(godot::Node2D* body);
  void OnExplodeAnimationFinished(const godot::StringName&);

 protected:
  static void _bind_methods();

 private:
  GDCLASS(Drone, godot::CharacterBody2D);  // NOLINT

  int health_ = 3;
  float speed_ = 50.0F;
  godot::Vector2 direction_{};
  bool player_in_range_ = false;
  bool has_exploded_ = false;
  godot::CharacterBody2D* player_{};
};

#endif  // ENTITIES_DRONE_HPP
