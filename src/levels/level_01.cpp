#include "level_01.hpp"

#include "entities/bullet.hpp"
#include "godot_cpp/classes/resource_loader.hpp"
#include "godot_cpp/variant/callable.hpp"
#include "godot_cpp/variant/callable_method_pointer.hpp"

using godot::ResourceLoader;

void Level01::_bind_methods() {}

void Level01::_ready() {
  bullet_scene_ = ResourceLoader::get_singleton()->load("uid://dxgrup4qovhs7");

  auto* player_node = get_node_or_null("Player");
  if (player_node) {
    player_node->connect("shoot", callable_mp(this, &Level01::OnPlayerShoot));
  }
}

void Level01::OnPlayerShoot(godot::Vector2 pos, godot::Vector2 dir) {
  auto* node = bullet_scene_->instantiate();
  auto* bullet = godot::Object::cast_to<Bullet>(node);
  if (bullet) {
    bullet->Setup(pos, dir);
    auto* bullets = get_node_or_null("Bullets");
    if (bullets) {
      bullets->add_child(bullet);
    } else {
      add_child(bullet);
    }
  } else if (node) {
    node->queue_free();
  }
}
