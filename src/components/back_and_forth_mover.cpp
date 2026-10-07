#include "back_and_forth_mover.hpp"

#include <algorithm>

#include "godot_cpp/classes/marker2d.hpp"
#include "godot_cpp/variant/utility_functions.hpp"

using godot::ClassDB;
using godot::D_METHOD;
using godot::UtilityFunctions;

void BackAndForthMover::_bind_methods() {
  ClassDB::bind_method(D_METHOD("set_speed", "s"), &BackAndForthMover::SetSpeed);
  ClassDB::bind_method(D_METHOD("get_speed"), &BackAndForthMover::GetSpeed);
  ClassDB::bind_method(D_METHOD("set_wait_time", "t"), &BackAndForthMover::SetWaitTime);
  ClassDB::bind_method(D_METHOD("get_wait_time"), &BackAndForthMover::GetWaitTime);
  ClassDB::bind_method(D_METHOD("set_enabled", "e"), &BackAndForthMover::SetEnabled);
  ClassDB::bind_method(D_METHOD("is_enabled"), &BackAndForthMover::IsEnabled);
  ClassDB::bind_method(D_METHOD("set_drive_owner", "d"), &BackAndForthMover::SetDriveOwner);
  ClassDB::bind_method(D_METHOD("is_drive_owner"), &BackAndForthMover::IsDriveOwner);
  ClassDB::bind_method(D_METHOD("set_start_distance", "d"), &BackAndForthMover::SetStartDistance);
  ClassDB::bind_method(D_METHOD("get_start_distance"), &BackAndForthMover::GetStartDistance);
  ClassDB::bind_method(D_METHOD("set_random_start", "r"), &BackAndForthMover::SetRandomStart);
  ClassDB::bind_method(D_METHOD("is_random_start"), &BackAndForthMover::IsRandomStart);

  ADD_PROPERTY(godot::PropertyInfo(godot::Variant::FLOAT, "speed"), "set_speed", "get_speed");
  ADD_PROPERTY(godot::PropertyInfo(godot::Variant::FLOAT, "wait_time"), "set_wait_time",
               "get_wait_time");
  ADD_PROPERTY(godot::PropertyInfo(godot::Variant::BOOL, "enabled"), "set_enabled", "is_enabled");
  ADD_PROPERTY(godot::PropertyInfo(godot::Variant::BOOL, "drive_owner"), "set_drive_owner",
               "is_drive_owner");
  ADD_PROPERTY(godot::PropertyInfo(godot::Variant::FLOAT, "start_distance",
                                   godot::PROPERTY_HINT_RANGE, "0,2000,1,or_greater"),
               "set_start_distance", "get_start_distance");
  ADD_PROPERTY(godot::PropertyInfo(godot::Variant::BOOL, "random_start"), "set_random_start",
               "is_random_start");
}

void BackAndForthMover::SetDriveOwner(bool d) {
  drive_owner_ = d;
  set_physics_process(d);
}

void BackAndForthMover::_ready() {
  set_physics_process(drive_owner_);

  body_ = godot::Object::cast_to<godot::Node2D>(get_parent());
  auto* pa = get_node<godot::Marker2D>("PointA");
  auto* pb = get_node<godot::Marker2D>("PointB");
  if (!body_ || !pa || !pb) {
    UtilityFunctions::push_warning(
      "BackAndForthMover: needs a Node2D parent and PointA/PointB children");
    return;
  }
  a_ = pa->get_global_position();
  b_ = pb->get_global_position();
  active_ = a_.distance_to(b_) > arrive_radius_;
  if (active_) {
    const float len = a_.distance_to(b_);
    const float d = random_start_ ? godot::UtilityFunctions::randf() * len
                                  : std::clamp(start_distance_, 0.0F, len);
    body_->set_global_position(a_ + (b_ - a_).normalized() * d);
  }
}

godot::Vector2 BackAndForthMover::GetDirection(double delta) {
  if (!active_ || !enabled_) return {};

  if (wait_left_ > 0.0) {
    wait_left_ -= delta;
    return {};
  }

  const godot::Vector2 pos = body_->get_global_position();
  const godot::Vector2 target = Target();
  if (pos.distance_to(target) <= arrive_radius_) {
    going_to_b_ = !going_to_b_;
    wait_left_ = wait_time_;
    return {};
  }
  return (target - pos).normalized();
}

void BackAndForthMover::_physics_process(double delta) {
  if (!drive_owner_ || !active_ || !enabled_) return;

  const godot::Vector2 dir = GetDirection(delta);
  if (dir == godot::Vector2()) return;

  const godot::Vector2 pos = body_->get_global_position();
  // clamp so a fast mover never overshoots the point and oscillates
  const float step = std::min(speed_ * static_cast<float>(delta), pos.distance_to(Target()));
  body_->set_global_position(pos + dir * step);
}
