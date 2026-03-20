# ackermann_mux

A ROS 2 velocity command multiplexer for Ackermann-drive robots. It subscribes to multiple
[ackermann_msgs/AckermannDrive](https://index.ros.org/p/ackermann_msgs/) command topics,
prioritizes them, and republishes the highest-priority active command. Inputs can be
suppressed via [std_msgs/Bool](https://docs.ros.org/en/rolling/p/std_msgs/) lock topics.

## Features

- Multiplexes multiple `ackermann_msgs/AckermannDrive` velocity command topics
- Priority-based selection (0–255; higher value = higher priority)
- Configurable per-topic timeout: stale messages are discarded automatically
- Lock topics (`std_msgs/Bool`) block all lower-priority velocity sources
- Diagnostics output via `diagnostic_updater`

## Dependencies

- ROS 2 (ament_cmake)
- `rclcpp`
- `ackermann_msgs`
- `std_msgs`
- `visualization_msgs`
- `diagnostic_updater`

## Building

```bash
cd ~/ros_ws
colcon build --packages-select ackermann_mux
source install/setup.bash
```

## Usage

### Launch

```bash
ros2 launch ackermann_mux twist_mux_launch.py
```

Launch arguments:

| Argument | Default | Description |
|---|---|---|
| `config_topics` | `config/twist_mux_topics.yaml` | Velocity topic definitions |
| `config_locks` | `config/twist_mux_locks.yaml` | Lock topic definitions |
| `ackermann_vel_out` | `ackermann_mux/ackermann_vel` | Output topic name |
| `use_sim_time` | `False` | Use simulation clock |

### Topic configuration (`config/twist_mux_topics.yaml`)

```yaml
twist_mux:
  ros__parameters:
    topics:
      navigation:
        topic   : nav_vel
        timeout : 0.5
        priority: 10
      joystick:
        topic   : joy_vel
        timeout : 0.5
        priority: 100
```

Each entry requires:
- `topic` — the input topic name (`ackermann_msgs/AckermannDrive`)
- `timeout` — seconds before a silent publisher is considered inactive
- `priority` — integer in `[0, 255]`; the active topic with the highest priority is forwarded

### Lock configuration (`config/twist_mux_locks.yaml`)

```yaml
twist_mux:
  ros__parameters:
    locks:
      pause:
        topic   : pause_navigation
        timeout : 0.0
        priority: 100
```

When a lock topic publishes `True`, all velocity sources with a priority **lower than or equal to** the lock's priority are suppressed. A `timeout > 0` treats a silent lock publisher as an active lock.

## Published topics

| Topic | Type | Description |
|---|---|---|
| `ackermann_vel_out` (remappable) | `ackermann_msgs/AckermannDrive` | Multiplexed output command |

## Authors

- Enrique Fernandez
- Siegfried-A. Gevatter Pujals
- Jeremie Deray
- Brighten Lee
- Bence Magyar
- Norawit Nangsue

## License

Apache License 2.0