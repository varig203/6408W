# VEX V5RC 6408W | High Stakes 2024-2025
## Libraries/Framework
### Versions of all included software
- **PROS**: 4.1.1
- **LemLib**: 0.5.5
- **LVGL**: 8.3.8
- **Robodash**: 2.3.0
### Reasons for the libraries
- **PROS**: The framework used for nearly all robot-related functionality, providing motor control, sensor management, and multitasking support.
- **LemLib**: is used for simplifying the programming by adding pre-made functions for common tasks like sensors, drivetrain, and PID.
- **Robodash**: Facilitates auton selection, debugging via a console, and image uploading without requiring an SD card, enabling more efficient development
## Buttons
- **R1**: Intake/Bring Lady Brown back down (2M Intake)
- **R2**: Prepare Lady Brown (1M Intake, 1M LB)
- **L2**: Use Lady Brown (1M Intake, 1M LB)
- **B**: Clamp
- **A**: Doinker
- **Left Stick**: Throttle
- **Right Stick**: Turning
---
## Project Structure Overview
- **`src/`**: Contains the main robot code.
  - **`hardware.hpp` & `hardware.cpp`**: Hardware definitions (motors, sensors, pneumatics, etc.).
  - **`autons.hpp` & `autons.cpp`**: Autonomous code and routines.
  - **`gearbox.hpp` & `gearbox.cpp`**: Everything to do with the gearbox mechanism for intake and wall stake mech.
- **`include/`**: Contains the header files `hpp` or `h`
- **`firmware/`**: DO NOT TOUCH UNLESS YOU KNOW WHAT YOU ARE DOING.
## Git Workflow
- **Commit small, functional changes**: Avoid large "feature dumps." Each commit should do one thing and do it well.
- **Test often**: Ensure code is tested before committing.
