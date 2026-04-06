#include <esp_now.h>
#include <WiFi.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

const int ledPin = 2;  // GPIO2 for built-in LED

// Structure to receive data (must match transmitter)
typedef struct {
  int xValue;
  int yValue;
  int buttonState;
  int messageId;
} JoystickData;

JoystickData receivedData;
bool newDataAvailable = false;

// FreeRTOS handles
TaskHandle_t receiveTaskHandle = NULL;
TaskHandle_t printTaskHandle = NULL;

// Queue for thread-safe communication
QueueHandle_t dataQueue;

// ESP-NOW receive callback
//typedef void (*esp_now_recv_cb_t)(const esp_now_recv_info_t * esp_now_info, const uint8_t *data, int data_len);
void OnDataRecv(const esp_now_recv_info_t *esp_now_info, const uint8_t *incomingData, int len) {
  if (len == sizeof(JoystickData)) {
    JoystickData tempData;
    memcpy(&tempData, incomingData, sizeof(tempData));

    // Send to queue (non-blocking from ISR)
    xQueueSendFromISR(dataQueue, &tempData, NULL);

    // // Print sender MAC
    // Serial.print("📡 Received from: ");
    // for(int i = 0; i < 6; i++) {
    //   Serial.printf("%02X", mac[i]);
    //   if(i < 5) Serial.print(":");
    // }
    Serial.print("Received bytes: ");
    Serial.println(len);
  }
}

// A4:F0:0F:69:7F:5C for TRANSMITTOR mac adress
// F4:2D:C9:59:8D:AC for RECEIVER 

// Task to receive data from queue
void receiveTask(void *pvParameters) {
  JoystickData tempData;

  while (1) {
    if (xQueueReceive(dataQueue, &tempData, portMAX_DELAY) == pdTRUE) {
      receivedData = tempData;
      newDataAvailable = true;

      // Notify print task
      xTaskNotifyGive(printTaskHandle);
    }
  }
}

// Task to print received data
void printTask(void *pvParameters) {
  while (1) {
    // Wait for notification from receive task
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    if (newDataAvailable) 
    {
      digitalWrite(ledPin, HIGH);
      Serial.println("=================================");
      Serial.print("📦 Message ID: ");
      Serial.println(receivedData.messageId);
      Serial.print("🕹️  X-Axis: ");
      Serial.println(receivedData.xValue);
      Serial.print("🕹️  Y-Axis: ");
      Serial.println(receivedData.yValue);
      Serial.print("🔘 Button: ");
      Serial.println(receivedData.buttonState == LOW ? "PRESSED" : "released");

      // Add joystick position interpretation
      if (receivedData.xValue < 1000) {
        Serial.println("⬅️  Moving LEFT");
      } else if (receivedData.xValue > 3000) {
        Serial.println("➡️  Moving RIGHT");
      }

      if (receivedData.yValue < 1000) {
        Serial.println("⬇️  Moving DOWN");
      } else if (receivedData.yValue > 3000) {
        Serial.println("⬆️  Moving UP");
      }

      Serial.println("=================================");
      Serial.println();

      newDataAvailable = false;
      vTaskDelay(pdMS_TO_TICKS(25));  
    }
    digitalWrite(ledPin, LOW);
  }
}

void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.println("🔧 ESP32 Receiver Starting...");

  // Create queue for thread-safe communication
  dataQueue = xQueueCreate(10, sizeof(JoystickData));

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

  // Register receive callback
  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("✅ ESP-NOW initialized");
  Serial.print("📡 Receiver MAC: ");
  Serial.println(WiFi.macAddress());
  Serial.println("Waiting for data...");
  Serial.println();

  // Create FreeRTOS tasks
  xTaskCreatePinnedToCore(
    receiveTask,
    "Receive Task",
    4096,
    NULL,
    2,
    &receiveTaskHandle,
    0);

  xTaskCreatePinnedToCore(
    printTask,
    "Print Task",
    4096,
    NULL,
    1,
    &printTaskHandle,
    1);

  Serial.println("✅ FreeRTOS tasks created");
}

void loop() {
  // Empty - FreeRTOS handles everything
  vTaskDelay(pdMS_TO_TICKS(1000));
}