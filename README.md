# ESP-32_CAM_Project
# Face and Eyeglass Detection with Relay Control using ESP32-CAM

This project implements face recognition and eyeglass detection on an **ESP32-CAM** microcontroller using **TensorFlow Lite for Microcontrollers**. The system toggles a relay based on recognition results, enabling automatic machine control for applications like smart switches or accessibility tools.

## Features

- **Face Recognition**: Identifies whether a face is detected.
- **Eyeglass Detection**: Determines if the detected face has eyeglasses.
- **Relay Control**: Activates or deactivates a relay based on recognition results.
- **Camera Integration**: Utilizes ESP32-CAM for image capturing and processing.
- **TensorFlow Lite Integration**: Runs a pre-trained TFLite model on the ESP32-CAM for real-time inference.
- **WiFi Connectivity**: Provides IP-based debugging or extension support.

---

## Components

### Hardware Requirements
- **ESP32-CAM**: Microcontroller with a built-in camera.
- **Relay Module**: Controls the connected machine or appliance.
- **Power Supply**: 5V 2A power supply for the ESP32-CAM.
- **Optional**: Breadboard, jumper wires, and connectors for prototyping.

### Software Requirements
- **Arduino IDE**: For programming and flashing the ESP32-CAM.
- **TensorFlow Lite Micro (TFLite)**: Lightweight models for on-device inference.
- **Libraries**:
  - `esp_camera.h` for camera handling.
  - `WiFi.h` for WiFi connectivity.
  - TensorFlow Lite libraries (`tensorflow/lite/micro`).

---

## Installation and Setup

### Step 1: Hardware Setup
1. Connect the ESP32-CAM to a relay module:
   - Connect the relay input pin to GPIO 12 (or the defined pin in the code).
   - Connect the relay's power and ground to the ESP32-CAM.
2. Ensure the ESP32-CAM is powered with a stable 5V 2A source.

### Step 2: Arduino IDE Setup
1. Install the **ESP32 Board Package**:
   - Go to **File** > **Preferences** and add this URL to the additional boards manager:
     ```
     https://dl.espressif.com/dl/package_esp32_index.json
     ```
   - Go to **Tools** > **Boards Manager**, search for "ESP32," and install it.
2. Install Required Libraries:
   - `esp32-camera`
   - `tensorflow/lite/micro`
3. Load the provided code into Arduino IDE:
   - Save the sketch as `Face_Eyeglass_Detection_RelayControl`.
   - Add the `face_eyeglass_model.tflite` file to the project directory.

### Step 3: Configure WiFi
In the Arduino sketch, update the following lines with your WiFi credentials:
```cpp
const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";
```

### Step 4: Upload the Code
1. Connect the ESP32-CAM to your computer using a USB-to-serial adapter.
2. Select the correct board and COM port in Arduino IDE:
   - Board: `AI Thinker ESP32-CAM`.
3. Compile and upload the code to the ESP32-CAM.

---

## Usage

1. Power on the ESP32-CAM.
2. Wait for the device to connect to WiFi. The IP address will be displayed in the serial monitor.
3. The system will start recognizing faces and eyeglasses.
4. The relay will be toggled based on the recognition results:
   - **Relay ON**: Face and eyeglasses detected.
   - **Relay OFF**: No face or no eyeglasses detected.
5. Connect your appliance or machine to the relay for automated control.

---

## Project Workflow

1. **Image Capture**: The ESP32-CAM captures frames via its onboard camera.
2. **Model Inference**:
   - The captured frame is passed to a TensorFlow Lite model.
   - The model predicts the presence of a face and eyeglasses.
3. **Relay Control**:
   - Based on the inference results, the relay is toggled to control the connected device.
4. **WiFi Debugging** (Optional): Monitor the status through serial output over WiFi.

---

## Applications

- **Smart Switches**: Automate device operation based on facial recognition.
- **Accessibility Tools**: Help individuals with disabilities control appliances.
- **Home Automation**: Integrate into IoT systems for advanced control.
- **Industrial Automation**: Use for operator-specific machine control.

---

## Files and Directories

- `Face_Eyeglass_Detection_RelayControl.ino`: Main Arduino sketch.
- `face_eyeglass_model.tflite`: Pre-trained TensorFlow Lite model for detection.
- `README.md`: Documentation for the project.

---

## Future Enhancements

- Add support for **voice commands** via Google Assistant or Alexa.
- Train a more robust model for detecting multiple objects or attributes.
- Extend to include gesture recognition or real-time emotion analysis.

---

## Acknowledgments

Special thanks to:
- **TensorFlow** for providing a lightweight framework for microcontrollers.
- **ESP32 Community** for extensive support and resources.

---

## License

This project is licensed under the [MIT License](LICENSE).
```

Let me know if you need further customization! 😊
