#ifndef COMPONENTS_BACK_AND_FORTH_MOVER
#define COMPONENTS_BACK_AND_FORTH_MOVER

#include "godot_cpp/classes/node2d.hpp"
#include "godot_cpp/variant/vector2.hpp"

class BackAndForthMover : public godot::Node2D {
  GDCLASS(BackAndForthMover, godot::Node2D)  // NOLINT

 public:
  void _ready() override;
  void _physics_process(double delta) override;

  godot::Vector2 GetDirection(double delta);

  void SetSpeed(float s) { speed_ = s; }
  [[nodiscard]] float GetSpeed() const { return speed_; }
  void SetWaitTime(float t) { wait_time_ = t; }
  [[nodiscard]] float GetWaitTime() const { return wait_time_; }
  void SetEnabled(bool e) { enabled_ = e; }
  [[nodiscard]] bool IsEnabled() const { return enabled_; }
  void SetDriveOwner(bool d);
  [[nodiscard]] bool IsDriveOwner() const { return drive_owner_; }
  void SetStartDistance(float d) { start_distance_ = d; }
  [[nodiscard]] float GetStartDistance() const { return start_distance_; }
  void SetRandomStart(bool r) { random_start_ = r; }
  [[nodiscard]] bool IsRandomStart() const { return random_start_; }

 protected:
  static void _bind_methods();

 private:
  godot::Vector2 Target() const { return going_to_b_ ? b_ : a_; }

  godot::Node2D* body_ = nullptr;

  godot::Vector2 a_, b_;
  float speed_ = 60.0F;
  float wait_time_ = 1.0F;
  float arrive_radius_ = 2.0F;
  double wait_left_ = 0.0;
  bool active_ = false;
  bool enabled_ = true;
  bool drive_owner_ = true;
  bool going_to_b_ = true;
  float start_distance_ = 0.0F;
  bool random_start_ = false;
};

#endif  // COMPONENTS_BACK_AND_FORTH_MOVER
