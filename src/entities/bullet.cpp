#include "bullet.hpp"

#include "entities/drone.hpp"
#include "godot_cpp/classes/input.hpp"
#include "godot_cpp/classes/node2d.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/sprite2d.hpp"
#include "godot_cpp/classes/tween.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/object.hpp"
#include "godot_cpp/variant/callable.hpp"
#include "godot_cpp/variant/callable_method_pointer.hpp"

using godot::ClassDB;
using godot::D_METHOD;
using godot::PropertyInfo;
using godot::Variant;
using godot::Vector2;

void Bullet::_bind_methods() {
  ClassDB::bind_method(D_METHOD("set_speed", "speed"), &Bullet::SetSpeed);
  ClassDB::bind_method(D_METHOD("get_speed"), &Bullet::GetSpeed);

  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");
}

void Bullet::_ready() {
  connect("body_entered", callable_mp(this, &Bullet::OnHitDrone));

  auto* notifier = get_node_or_null("VisibleOnScreenNotifier2D");
  if (notifier) {
    notifier->connect("screen_exited", callable_mp(this, &Bullet::OnScreenExited));
  }

  auto* sprite_node = get_node<godot::Sprite2D>("Sprite2D");
  if (sprite_node) {
    sprite_node->set_scale(Vector2(0.0F, 0.0F));
    auto tween = create_tween();
    tween->tween_property(sprite_node, "scale", Vector2(1.0F, 1.0F), 0.2);
  }
}

void Bullet::Setup(Vector2 pos, Vector2 dir) {
  set_position(pos + dir * 15);
  direction_ = dir;
  set_rotation(dir.angle());
}

void Bullet::OnHitDrone(godot::Node2D* body) {
  auto* drone = godot::Object::cast_to<Drone>(body);
  if (drone) {
    drone->TakeDamage(direction_);
  }
  queue_free();
}

void Bullet::OnScreenExited() { queue_free(); }

void Bullet::_process(double delta) { set_position(get_position() + speed_ * direction_ * delta); }
