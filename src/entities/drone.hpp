#ifndef ENTITIES_DRONE_HPP
#define ENTITIES_DRONE_HPP

#include <godot_cpp/classes/character_body2d.hpp>

#include "components/back_and_forth_mover.hpp"
#include "godot_cpp/classes/animated_sprite2d.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/sprite2d.hpp"
#include "godot_cpp/classes/tween.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/vector2.hpp"

class Drone : public godot::CharacterBody2D {
  GDCLASS(Drone, godot::CharacterBody2D)  // NOLINT

 public:
  void _ready() override;
  void _physics_process(double delta) override;

  void StartCamBlink();
  void TakeDamage(godot::Vector2 dir);
  void Explode();
  void ChainExplode();

  void OnPlayerInAttackRangeEntered(godot::Node2D* body);
  void OnPlayerInAttackRangeExited(godot::Node2D* body);
  void OnPlayerInExplodeRangeEntered(godot::Node2D* body);
  void OnExplodeAnimationFinished(const godot::StringName&) { queue_free(); }

 protected:
  static void _bind_methods();

 private:
  godot::AnimatedSprite2D* sprite_{};
  godot::CharacterBody2D* player_{};
  godot::Sprite2D* cam_light_{};
  godot::Ref<godot::Tween> cam_blink_tween_{};

  BackAndForthMover* mover_{};

  int health_ = 3;
  float speed_ = 50.0F;
  godot::Vector2 direction_{};
  bool player_in_range_ = false;
  bool has_exploded_ = false;
};

#endif  // ENTITIES_DRONE_HPP
