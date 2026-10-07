#include "register_types.h"

#include <gdextension_interface.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "components/back_and_forth_mover.hpp"
#include "entities/bullet.hpp"
#include "entities/drone.hpp"
#include "entities/player.hpp"
#include "levels/level_01.hpp"

using namespace godot;  // NOLINT

void initialize_gdextension_types(ModuleInitializationLevel level) {  // NOLINT
  if (level != MODULE_INITIALIZATION_LEVEL_SCENE) {
    return;
  }
  GDREGISTER_CLASS(BackAndForthMover);
  GDREGISTER_RUNTIME_CLASS(Level01);
  GDREGISTER_RUNTIME_CLASS(Player);
  GDREGISTER_RUNTIME_CLASS(Bullet);
  GDREGISTER_RUNTIME_CLASS(Drone);
}

void uninitialize_gdextension_types(ModuleInitializationLevel level) {  // NOLINT
  if (level != MODULE_INITIALIZATION_LEVEL_SCENE) {
    return;
  }
}

extern "C" {
// Initialization
GDExtensionBool GDE_EXPORT godotcpp_library_init(  // NOLINT
  GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library,
  GDExtensionInitialization* r_initialization) {
  GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
  init_obj.register_initializer(initialize_gdextension_types);
  init_obj.register_terminator(uninitialize_gdextension_types);
  init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

  return init_obj.init();
}
}
