cmake_minimum_required(VERSION 3.5)
project(behavior_tree_extras)

if(CATKIN_DEVEL_PREFIX OR CATKIN_BUILD_BINARY_PACKAGE)
  set(catkin_FOUND 1)
endif()

# Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++17) add_compile_options(-Wall -Werror)

include(GoogleTest REQUIRED)

set(${PROJECT_NAME}_SRCS
    src/BTExecutionResult.cpp
    src/BehaviorTreeBuilder.cpp
    src/BehaviorTreeManager.cpp
    src/modular_loggers/ModularFileLogger.cpp
    src/modular_loggers/ModularMinitraceLogger.cpp
    src/modular_loggers/ModularStdCoutLogger.cpp)

file(GLOB fast_test_files_list tests/fast_tests_*.cpp)

if(catkin_FOUND)

  find_package(catkin REQUIRED COMPONENTS behaviortree_cpp_v3 roscpp roslint
                                          rostest)

  catkin_package(
    INCLUDE_DIRS
    include
    LIBRARIES
    ${PROJECT_NAME}
    CATKIN_DEPENDS
    behaviortree_cpp_v3
    roscpp
    roslint)

  #
  # Build

  include_directories(include ${catkin_INCLUDE_DIRS} ${EIGEN_INCLUDE_DIRS})
  add_library(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})
  add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS}
                   ${catkin_EXPORTED_TARGETS})
  target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES})

  #
  # Install

  install(
    TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION})
  install(DIRECTORY include/ DESTINATION ${CATKIN_GLOBAL_INCLUDE_DESTINATION})

  #
  # Testing

  if(CATKIN_ENABLE_TESTING)

    set(fast_test_target_name ${PROJECT_NAME}-fast-tests)
    catkin_add_gmock(${fast_test_target_name} tests/runner.cpp
                     ${fast_test_files_list})
    target_link_libraries(${fast_test_target_name} ${PROJECT_NAME}
                          ${catkin_LIBRARIES})

    set(ROSLINT_CPP_OPTS --extensions=h,hpp,cpp)
    roslint_cpp()

    roslint_custom(catkin_lint -W2 ${PROJECT_SOURCE_DIR})
    roslint_add_test()
  endif()

else()

  option(${PROJECT_NAME}_ENABLE_TESTING "Build tests" ON)

  # Add our local modules folder to the search path for both <pkg>.cmake and
  # Find<pkg>.cmake files
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/packages")
  list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/packages")

  find_package(BehaviorTree)

  #
  # Build

  add_library(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})
  target_include_directories(${PROJECT_NAME} PUBLIC include)
  target_link_libraries(${PROJECT_NAME} behaviortree_cpp_v3)

  #
  # Install

  install(
    TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)

  #
  # Testing

  if(${PROJECT_NAME}_ENABLE_TESTING)
    include(CTest)
    include(GoogleTest REQUIRED)

    set(fast_test_target_name ${PROJECT_NAME}-islated-unit-tests)
    add_executable(${fast_test_target_name} tests/runner.cpp
                                            ${fast_test_files_list})
    target_link_libraries(${fast_test_target_name} ${PROJECT_NAME}
                          behaviortree_cpp_v3 gmock)
    gtest_discover_tests(${fast_test_target_name})

  endif()

endif()
