# Line Following Robot

This repository contains the code and details for building a line-following robot using an Arduino Nano and various components.

---

## **Hardware Used**
1. **Arduino Nano** - Microcontroller for processing.
2. **L298 Motor Driver** - Controls the motors.
3. **IR Array (14-sensor array)** - Detects the line to follow.
4. **MUX (16-channel)** - Expands input channels for sensor readings.
5. **Buck Converter** - Regulates power supply.
6. **Battery** - Powers the robot.
7. **Gear Motor** - Drives the wheels.
8. **Push Buttons** - For mode selection and starting operations.
9. **Switches, Wires, etc.** - Additional components for connections.

---

## **Connection Diagram**
To understand the wiring and connections:
- ![Basic LFR schematic](https://github.com/user-attachments/assets/cd00b360-109a-41d9-8068-3b55f9f4eef9)
- [Click here for detailed circuit](https://example.com/detailed-circuit](https://app.cirkitdesigner.com/project/8df65339-e6b2-4c45-b0c9-fccbe6c56bb0))

---

## **Code Breakdown**
The code is structured into the following steps:
1. **Defining Variables**: Initializes all required variables and constants.
2. **I/O Pins Setup**: Configures input/output pins for sensors, motors, and buttons.
3. **Calibration Mode**: Calibrates the robot to adapt to the track.
4. **Run Mode**: Executes the line-following operation.

---

## **Purpose of Different Functions**
### 1. **`calibration()`**
   - **Purpose**: Runs before operating the robot on a new track.
   - **Details**: Records the minimum and maximum analog values of the track. Black surface values are near `1023`, and white surface values are near `0`. These values are used to calculate the robot's position on the line.

### 2. **`printCalibration()`**
   - **Purpose**: Prints the calibrated values after the calibration process.

### 3. **`saveCalibration()`**
   - **Purpose**: Saves the calibrated values to EEPROM for persistent use.

### 4. **`readLine()`**
   - **Purpose**: Reads sensor data during operation.
   - **Details**: Uses calibrated values to map sensor readings and determine the robot's position.

### 5. **`Mux_Addr(channel_no)`**
   - **Purpose**: Sets the appropriate MUX channel. In our case, we used 14 channels sequentialy.
   - **Details**: Toggles pins based on the `channel_address_table[14][4]` array.

### 6. **`printReading()`**
   - **Purpose**: Displays real-time sensor readings to detect potential sensor faults.

### 7. **`drive(left_motor_speed, right_motor_speed)`**
   - **Purpose**: Controls motor speeds.
   - **Details**: Accepts PWM values for left and right motors.

### 8. **`push(button_pin)`**
   - **Purpose**: Handles button presses while avoiding false triggers due to debounce effects.
   - **Details**: Two buttons are used:
     1. **Calibration Mode Button**
     2. **Run Mode Button**

### 9. **`lineFollow(position, base_speed, max_speed, min_speed, Kp, Kd, Ki)`**
   - **Purpose**: Implements PID control for line following.
   - **Details**: 
     - Calculates positional error and adjusts motor speeds accordingly.
     - Detects a "finish box" where all sensors read black, stopping the motors.
     - To resume, place the robot on a valid line and press the "Run" button.

### 10. **`analogReadSetup()`**: 
   - **Purpose**: Initializes the register bits for custom analog read function.
   - **Details**: Always call this function inside the setup. Refer to ATMega328P datasheet for better understanding of the registers.

### 11. **`analogRd(uint8_t pin_no)`**
   - **Purpose**: This a customized function for taking analog readings.
   - **Details**: Direct register manipulation has been done to enhance reading speed. Send the analog pin numbers as arguement. (**N.B.**: Do not send A0, A1. Send only numbers. E.g.: `analogRd(0);` ==> It will read the A0 pin.)
---

## **How to Use**
1. Connect the hardware as per the connection diagram.
2. Upload the code to the Arduino Nano using the Arduino IDE.
3. Use the **Calibration Mode Button** to calibrate the robot for a new track.
4. Press the **Run Mode Button** to start line following.

---

## **Features**
- **Calibration and Run Modes**: Easily switch between modes for seamless operation.
- **Fault Detection**: Check sensor readings in serial monitor in real time.
- **Finish Box Detection**: Automatically stops when a finish box is detected.


