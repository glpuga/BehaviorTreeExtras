include(FetchContent)
FetchContent_Declare(behavior_tree_v3
  GIT_REPOSITORY    https://github.com/glpuga/BehaviorTree.CPP.git
  GIT_TAG           40baf90b1e6786f5dfc0ef5d65926c7ad302de70    # branch: glpuga/ariac2022
  GIT_SHALLOW       true
  GIT_PROGRESS      true
)

option(BUILD_EXAMPLES   "Build tutorials and examples" OFF)
option(BUILD_SAMPLES    "Build sample nodes" OFF)
option(BUILD_UNIT_TESTS "Build the unit tests" OFF)
option(BUILD_TOOLS "Build commandline tools" OFF)
option(ENABLE_COROUTINES "Enable boost coroutines" OFF)

FetchContent_MakeAvailable(behavior_tree_v3)