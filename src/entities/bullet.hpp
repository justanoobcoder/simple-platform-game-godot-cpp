#ifndef ENTITIES_BULLET_HPP
#define ENTITIES_BULLET_HPP

#include <godot_cpp/classes/area2d.hpp>

#include "godot_cpp/classes/node2d.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/vector2.hpp"

class Bullet : public godot::Area2D {
 public:
  [[nodiscard]] float GetSpeed() const { return speed_; }
  void SetSpeed(float speed) { speed_ = speed; }

  void _ready() override;
  void _process(double delta) override;

  void Setup(godot::Vector2 pos, godot::Vector2 dir);

  void OnHitDrone(godot::Node2D* body);
  void OnScreenExited();

 protected:
  static void _bind_methods();

 private:
  GDCLASS(Bullet, Area2D);  // NOLINT

  float speed_ = 200.0F;
  godot::Vector2 direction_;
};

#endif  // ENTITIES_BULLET_HPP
