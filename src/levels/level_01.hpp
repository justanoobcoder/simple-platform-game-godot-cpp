#ifndef LEVELS_LEVEL_01
#define LEVELS_LEVEL_01

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>

#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/vector2.hpp"

class Level01 : public godot::Node2D {
 public:
  void _ready() override;

  void OnPlayerShoot(godot::Vector2 pos, godot::Vector2 dir);

 protected:
  static void _bind_methods();

 private:
  GDCLASS(Level01, godot::Node2D);  // NOLINT

  godot::Ref<godot::PackedScene> bullet_scene_;
};

#endif  // LEVELS_LEVEL_01
