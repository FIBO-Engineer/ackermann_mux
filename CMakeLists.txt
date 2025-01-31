cmake_minimum_required(VERSION 3.0.2)
project(ackermann_mux)

find_package(catkin REQUIRED
    COMPONENTS
      roscpp
      std_msgs
      geometry_msgs
      visualization_msgs
      diagnostic_updater)

catkin_package(
    INCLUDE_DIRS include
    CATKIN_DEPENDS
      roscpp
      std_msgs
      geometry_msgs
      visualization_msgs
      diagnostic_updater)

include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(ackermann_mux
    src/ackermann_mux.cpp
    src/ackermann_mux_node.cpp
    src/ackermann_mux_diagnostics.cpp)
target_link_libraries(ackermann_mux ${catkin_LIBRARIES})

add_executable(ackermann_marker
    src/ackermann_marker.cpp)
target_link_libraries(ackermann_marker ${catkin_LIBRARIES})

install(TARGETS ackermann_mux ackermann_marker
    RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

catkin_install_python(PROGRAMS scripts/joystick_relay.py
    DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

foreach(dir launch config)
    install(DIRECTORY ${dir}/
        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/${dir})
endforeach()

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)

if (CATKIN_ENABLE_TESTING)
    find_package(rostest REQUIRED)

    add_rostest(test/system.test)
endif()
