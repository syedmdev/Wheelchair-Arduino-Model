/*
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/  
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/

#include <esp_now.h>
#include <WiFi.h>

//variables
float static deadZone = 0.3;
float static deadZone2 = 0.15;

bool forward = false;
bool backward = false;
bool left = false;
bool right = false;

static int enableA = 12; //LOW HIGH FWD
static int input1 = 14; 
static int input2 = 27;

static int enableB =21;
static int input3 = 22; 
static int input4 = 23; 


// Structure example to receive data
// Must match the sender structure

typedef struct struct_message {
  char head[32];
  float RotX;
  float RotY;
  float RotZ;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  //Serial.print("Bytes received: ");
  //Serial.println(len);
  //Serial.print("Char: ");
  //Serial.print(myData.head);
  //Serial.print("Rotation X: ");
  Serial.print(myData.RotX); Serial.print(",");
  //Serial.print("Rotation Y: ");
  Serial.print(myData.RotY); Serial.print(",");
  //Serial.print("Rotation Z: ");
  Serial.print(myData.RotZ); Serial.print("\n");

  float RotX = myData.RotX; float RotZ = myData.RotZ;

  if(!forward && !backward && !left && !right){
    if (RotZ >= deadZone){
      forward = true;
    }
    else if (RotZ <= -deadZone){
      backward = true;
    }
    else if (RotX >= deadZone2){
      right = true;
    }
    else if (RotX <= -deadZone2){
      left = true;
    }
    }

  else if (forward){
    if (RotZ <= -deadZone){
      forward = false;
    }

  }
  else if (backward){
    if (RotZ >= deadZone){
      backward = false;
    }
  }
  else if (left){
    if (RotX >= deadZone2){
      left = false;
    }
  }
  else if (right){
    if (RotX <= -deadZone2){
      right = false;
    }
  }
  
} 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(921600);

  pinMode(enableA, OUTPUT); //LOW HIGH FWD
  pinMode(input1, OUTPUT);
  pinMode(input2, OUTPUT);

  pinMode(enableB, OUTPUT);
  pinMode(input3, OUTPUT);
  pinMode(input4, OUTPUT);

  digitalWrite(enableA, HIGH);
  digitalWrite(enableB, HIGH);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));}
 
void loop() {

  /*Serial.println("Forward" && forward);
  Serial.println("Backward" && backward);
  Serial.println("Right" && right);
  Serial.println("Left" && left);*/
  //delay(5000);
  Direction();
  }

void Direction(){
  if (!forward && !backward && !left && !right){
    digitalWrite(input1, LOW);
    digitalWrite(input2, LOW);
    digitalWrite(input3, LOW);
    digitalWrite(input4, LOW);
  }
  else if (forward){
    digitalWrite(input1, LOW);
    digitalWrite(input2, HIGH);
    digitalWrite(input3, LOW);
    digitalWrite(input4, HIGH);
  }
  else if (backward){
    digitalWrite(input1, HIGH);
    digitalWrite(input2, LOW);
    digitalWrite(input3, HIGH);
    digitalWrite(input4, LOW);
  }
  else if (right){
    digitalWrite(input1, HIGH);
    digitalWrite(input2, LOW);
    digitalWrite(input3, LOW);
    digitalWrite(input4, HIGH);
  }
  else if (left){
    digitalWrite(input1, LOW);
    digitalWrite(input2, HIGH);
    digitalWrite(input3, HIGH);
    digitalWrite(input4, LOW);
  }
}
