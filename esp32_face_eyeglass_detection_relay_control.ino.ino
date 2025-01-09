#include "esp_camera.h"
#include <WiFi.h>
#include "FS.h"
#include "SPIFFS.h"
#include "TensorFlowLite.h"

// Camera configuration for AI Thinker ESP32-CAM
#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

// WiFi credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// Relay control pin
#define RELAY_PIN 12  // Change to your relay pin

// File path for the TensorFlow Lite model stored in SPIFFS
const char* tflite_model_file = "/eyeglasses_detection_model.tflite";

// Global variables for TensorFlow Lite model
std::unique_ptr<tflite::Interpreter> interpreter;
tflite::ops::micro::AllOpsResolver resolver;

void setup() {
  // Start Serial communication for debugging
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount SPIFFS");
    return;
  }

  // Initialize camera
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = 15;
  config.pin_d1 = 2;
  config.pin_d2 = 4;
  config.pin_d3 = 5;
  config.pin_d4 = 18;
  config.pin_d5 = 19;
  config.pin_d6 = 21;
  config.pin_d7 = 22;
  config.pin_xclk = 0;
  config.pin_pclk = 22;
  config.pin_vsync = 25;
  config.pin_href = 23;
  config.pin_sscb_sda = 26;
  config.pin_sscb_scl = 27;
  config.pin_pwdn = 32;
  config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  esp_camera_init(&config);

  // Load TensorFlow Lite model
  File modelFile = SPIFFS.open(tflite_model_file, "r");
  if (!modelFile) {
    Serial.println("Failed to open model file");
    return;
  }

  // Load the model into memory
  tflite::FlatBufferModel model(modelFile);
  tflite::InterpreterBuilder builder(model, resolver);
  builder(&interpreter);

  // Check if TensorFlow Lite model was initialized successfully
  if (interpreter == nullptr) {
    Serial.println("Failed to build interpreter");
    return;
  }

  // Set relay pin mode
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);  // Start with relay off
  Serial.println("Model loaded and relay initialized.");
}

void loop() {
  // Capture an image from the camera
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  // Preprocess image and feed it to TensorFlow Lite model
  // Resize and normalize image as per the model requirements
  // You can add code here to resize the image if needed

  // Set the input tensor (image) for the model
  interpreter->tensor(interpreter->inputs()[0])->data.f = fb->buf;

  // Run inference with TensorFlow Lite
  interpreter->Invoke();

  // Get the result of the classification (output tensor)
  float* output = interpreter->tensor(interpreter->outputs()[0])->data.f;

  // If output > threshold, turn on relay (machine on)
  if (output[0] > 0.5) {  // Adjust threshold based on your model's accuracy
    digitalWrite(RELAY_PIN, HIGH);  // Turn on machine
  } else {
    digitalWrite(RELAY_PIN, LOW);  // Turn off machine
  }

  // Return the frame buffer to free memory
  esp_camera_fb_return(fb);

  // Repeat after a small delay
  delay(1000);  // Adjust the delay as needed
}
