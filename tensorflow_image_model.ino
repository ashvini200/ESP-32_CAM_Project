#include <esp_camera.h>
#include <WiFi.h>
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

// TensorFlow Lite Model
#include "face_eyeglass_model.h" // Include compiled TFLite model

// WiFi Credentials
const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

// Relay Pin
#define RELAY_PIN 12 // Replace with your actual relay pin

// TensorFlow Lite Globals
constexpr int kTensorArenaSize = 1024 * 10; // Adjust as per your model requirements
uint8_t tensor_arena[kTensorArenaSize];
tflite::MicroInterpreter* interpreter = nullptr;

void startCameraServer(); // Function prototype for camera server

void setup() {
  // Serial Communication Setup
  Serial.begin(115200);
  Serial.println("Starting program...");

  // Configure Camera
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // Initialize Camera
  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Camera initialization failed!");
    while (true); // Halt on failure
  }
  Serial.println("Camera initialized.");

  // WiFi Connection
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start Camera Server
  startCameraServer();
  Serial.println("Camera server ready.");

  // Configure Relay Pin
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Initialize relay as LOW

  // Load TensorFlow Lite Model
  static tflite::MicroErrorReporter error_reporter;
  const tflite::Model* model = tflite::GetModel(face_eyeglass_model);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    error_reporter.Report("Model schema version mismatch.");
    while (true); // Halt on failure
  }

  static tflite::MicroMutableOpResolver<5> resolver;
  resolver.AddBuiltin(tflite::BuiltinOperator_CONV_2D, tflite::ops::micro::Register_CONV_2D());
  resolver.AddBuiltin(tflite::BuiltinOperator_SOFTMAX, tflite::ops::micro::Register_SOFTMAX());

  static tflite::MicroInterpreter static_interpreter(model, resolver, tensor_arena, kTensorArenaSize, &error_reporter);
  interpreter = &static_interpreter;
  interpreter->AllocateTensors();
}

void loop() {
  // Capture image and process TensorFlow model
  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed.");
    return;
  }

  // Pass image data to TensorFlow Lite model
  TfLiteTensor* input = interpreter->input(0);
  memcpy(input->data.uint8, fb->buf, fb->len); // Resize/normalize image as needed

  if (interpreter->Invoke() != kTfLiteOk) {
    Serial.println("Error invoking TensorFlow Lite model.");
    esp_camera_fb_return(fb);
    return;
  }

  // Get the results
  TfLiteTensor* output = interpreter->output(0);
  bool matchFace = output->data.uint8[0] > 128; // Example threshold for face match
  bool eyeGlassDetected = output->data.uint8[1] > 128; // Example threshold for eyeglass detection

  // Relay control logic
  if (matchFace && eyeGlassDetected) {
    digitalWrite(RELAY_PIN, HIGH); // Turn on relay
    Serial.println("Face matched and eyeglass detected. Relay ON.");
  } else {
    digitalWrite(RELAY_PIN, LOW); // Turn off relay
    Serial.println("Face mismatch or eyeglass not detected. Relay OFF.");
  }

  esp_camera_fb_return(fb);
  delay(1000); // Delay for stability
}
