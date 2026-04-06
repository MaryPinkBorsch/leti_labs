#include <esp_now.h>
#include <WiFi.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// Joystick pins
const int xPin = 32;
const int yPin = 33;
const int swPin = 25;
const int ledPin = 2;  // GPIO2 for built-in LED

const int dead_zone = 200;
const int center = 2048;
bool new_data = false;

// MAC address of Receiver ESP32 (replace with your receiver's MAC)

// A4:F0:0F:69:7F:5C for TRANSMITTOR
// F4:2D:C9:59:8D:AC for RECEIVER
uint8_t receiverMac[] = { 0xF4, 0x2D, 0xC9, 0x59, 0x8D, 0xAC };

// Structure to send data
typedef struct
{
  int xValue;
  int yValue;
  int buttonState;
  int messageId;
} JoystickData;

JoystickData joystickData;
int messageCounter = 0;

// FreeRTOS handles
TaskHandle_t joystickTaskHandle = NULL;
TaskHandle_t networkTaskHandle = NULL;

// // ESP-NOW send callback
// void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
//   Serial.print("Send Status: ");
//   Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
// }

// Task 1: Read joystick values
void joystickTask(void *pvParameters) {
  while (1) {
    // Read joystick
    joystickData.xValue = analogRead(xPin);
    joystickData.yValue = analogRead(yPin);
    joystickData.buttonState = digitalRead(swPin);



    if ((joystickData.xValue < center - dead_zone || joystickData.xValue > center + dead_zone)
        || (joystickData.yValue < center - dead_zone || joystickData.yValue > center + dead_zone))

    {
      new_data = true;
      joystickData.messageId = messageCounter++;
      digitalWrite(ledPin, HIGH);
      // Print local readings
      Serial.print("📊 X: ");
      Serial.print(joystickData.xValue);
      Serial.print(" | Y: ");
      Serial.print(joystickData.yValue);
      Serial.print(" | Button: ");
      Serial.println(joystickData.buttonState == LOW ? "Pressed" : "Released");

      Serial.println("NEW DATA");
    }

    vTaskDelay(pdMS_TO_TICKS(50));  // Read every 50ms
    digitalWrite(ledPin, LOW);
  }
}

// Task 2: Send data via ESP-NOW
void networkTask(void *pvParameters) {
  while (1) {
    if (new_data) {
      // Send joystick data
      esp_err_t result = esp_now_send(receiverMac, (uint8_t *)&joystickData, sizeof(joystickData));

      if (result == ESP_OK) {
        Serial.println("📤 Data sent successfully");
      } else {
        Serial.println("❌ Error sending data");
      }
      new_data = false;
    }

    vTaskDelay(pdMS_TO_TICKS(100));  // Send every 100ms
  }
}

void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Initialize joystick pins
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(swPin, INPUT_PULLUP);

  Serial.println("🔧 ESP32 Transmitter Starting...");

  // Set ESP32 as Wi-Fi Station
  WiFi.mode(WIFI_STA);
  delay(500);  // Wait for WiFi radio to fully power up
  // CRITICAL: Actually start WiFi (this powers on the radio)
  WiFi.begin();  // Starts WiFi without connecting to any network
  delay(500);    // Wait for WiFi radio to fully power up

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("❌ Error initializing ESP-NOW");
    return;
  }
  Serial.println("✅ ESP-NOW initialized");
  Serial.print("📡 Transmitter MAC: ");
  Serial.println(WiFi.macAddress());
  // Register send callback
  // esp_now_register_send_cb(OnDataSent);

  // Add peer (receiver)
  esp_now_peer_info_t peerInfo;

  memset(&peerInfo, 0, sizeof(peerInfo));  // Clear the structure first
  // CRITICAL: Set the interface (ESP_IF_WIFI_STA = 0 for station mode)
  peerInfo.ifidx = WIFI_IF_STA;
  memcpy(peerInfo.peer_addr, receiverMac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("❌ Failed to add peer");
    return;
  }



  // Create FreeRTOS tasks
  xTaskCreatePinnedToCore(
    joystickTask,         // Task function
    "Joystick Task",      // Task name
    2048,                 // Stack size
    NULL,                 // Parameters
    1,                    // Priority
    &joystickTaskHandle,  // Task handle
    0                     // Core 0
  );

  xTaskCreatePinnedToCore(
    networkTask,         // Task function
    "Network Task",      // Task name
    4096,                // Stack size (larger for network)
    NULL,                // Parameters
    2,                   // Higher priority
    &networkTaskHandle,  // Task handle
    1                    // Core 1
  );

  Serial.println("✅ FreeRTOS tasks created");
}

void loop() {
  // Empty - FreeRTOS handles everything
  vTaskDelay(pdMS_TO_TICKS(1000));
}