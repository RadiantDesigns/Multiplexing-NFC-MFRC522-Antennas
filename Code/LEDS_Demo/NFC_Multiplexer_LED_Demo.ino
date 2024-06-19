#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_NeoPixel.h>

#define MUX_ENA   2
#define MUX_ENB   3
#define MUX_A0    4
#define MUX_A1    5

#define RST_PIN   9
#define SS_PIN    10

#define LED_PIN 6
#define LED_COUNT 1

// Setting up the NeoPixel library
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

MFRC522 mfrc522(SS_PIN, RST_PIN); // Instance of the class

//The The limited set of recognized unique IDs of the colored Tags I'm using
byte allowedID[6][4] =           
  {
    {0x90, 0x6A, 0xCF, 0x25},   // ID0 - BLUE
    {0x11, 0x7C, 0x5D, 0x26},   // ID1 - GREEN   
    {0x90, 0xD3, 0x11, 0x25},   // ID2 - RED
    {0x53, 0x40, 0x5D, 0x10},    // ID3 - BROWN
    {0xA3, 0xD3, 0x49, 0x30},    // ID4 - YELLOW
    {0x80, 0xF0, 0xC8, 0x25}    // ID5 - WHITE
  };   

// Colors matching the set of recognized unique IDs of the tags I'm using
uint32_t color_matching[6] =    
  {
    strip.Color(  0,   0, 255),     // Blue    
    strip.Color(  0,   255, 0),     // Green
    strip.Color(  255,   0, 0),     // Red
    strip.Color(  150,   75, 0),    // Brown
    strip.Color(  255,   255, 0),   // Yellow
    strip.Color(  255,   255, 255), // White
  };

int counter = 0;

void setup() {
  strip.begin();           // Initialize NeoPixel object
  strip.setBrightness(100); // Set BRIGHTNESS to about 40% (max = 255)
  strip.clear(); // Set all pixel colors to 'off'

  pinMode(MUX_ENA, OUTPUT);
  pinMode(MUX_ENB, OUTPUT);
  pinMode(MUX_A0, OUTPUT);
  pinMode(MUX_A1, OUTPUT);

  digitalWrite(MUX_ENA, LOW);
  digitalWrite(MUX_ENB, LOW);
  digitalWrite(MUX_A0, LOW);
  digitalWrite(MUX_A1, LOW);
  
  Serial.begin(9600);
  while (!Serial);    

  SPI.begin(); // Init SPI bus
  Serial.println();
  Serial.println("********************* NEW PROGRAM ***************************");
  Serial.println("<Arduino is ready>");
  Serial.println();
  
  mfrc522.PCD_Init();                 // Init MFRC522 
  delay(100);                         // Optional delay. Some board do need more time after init to be ready
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  
  Serial.println(F("<NFC reader is ready>"));
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
  Serial.println();
}

void loop() {

  if(counter == 8){
    counter = 1; 
  }else{
    counter++;  
  }

  // Tile A1
  if (counter == 1){
    digitalWrite(MUX_ENA, HIGH);
    digitalWrite(MUX_ENB, LOW);
    digitalWrite(MUX_A0, LOW);
    digitalWrite(MUX_A1, LOW);
  }

  // Tile A2
  if (counter == 2){
    digitalWrite(MUX_ENA, HIGH);
    digitalWrite(MUX_ENB, LOW);
    digitalWrite(MUX_A0, HIGH);
    digitalWrite(MUX_A1, LOW);
  }

  // Tile A3
  if (counter == 3){
    digitalWrite(MUX_ENA, HIGH);
    digitalWrite(MUX_ENB, LOW);
    digitalWrite(MUX_A0, LOW);
    digitalWrite(MUX_A1, HIGH);
  }

  // Tile A4
  if (counter == 4){
    digitalWrite(MUX_ENA, HIGH);
    digitalWrite(MUX_ENB, LOW);
    digitalWrite(MUX_A0, HIGH);
    digitalWrite(MUX_A1, HIGH);
  }

  // Tile B1
  if (counter == 5){
    digitalWrite(MUX_ENA, LOW);
    digitalWrite(MUX_ENB, HIGH);
    digitalWrite(MUX_A0, LOW);
    digitalWrite(MUX_A1, LOW);
  }

  // Tile B2
  if (counter == 6){
    digitalWrite(MUX_ENA, LOW);
    digitalWrite(MUX_ENB, HIGH);
    digitalWrite(MUX_A0, HIGH);
    digitalWrite(MUX_A1, LOW);
  }

  // Tile B3
  if (counter == 7){
    digitalWrite(MUX_ENA, LOW);
    digitalWrite(MUX_ENB, HIGH);
    digitalWrite(MUX_A0, LOW);
    digitalWrite(MUX_A1, HIGH);
  }

  // Tile B4
  if (counter == 8){
    digitalWrite(MUX_ENA, LOW);
    digitalWrite(MUX_ENB, HIGH);
    digitalWrite(MUX_A0, HIGH);
    digitalWrite(MUX_A1, HIGH);
  }

  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      
      strip.setPixelColor(counter-1, checkTagUID(mfrc522.uid.uidByte));
      strip.show();
      
      mfrc522.PICC_HaltA();       // Halt PICC
      mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
  
      mfrc522.PCD_Reset();        // Test soft reset
      mfrc522.PCD_Init();
  }else{
    strip.setPixelColor((counter-1),   0,   0,   0);
    strip.show();
  }
}

//----------------------------------------------------
// ADDITIONAL FUNCTIONS SECTION
uint32_t checkTagUID(byte * buffer){
  
  uint32_t color = strip.Color(  128,   128,   128);         // std color, as default return value for unrecognized tag
  for (int i = 0; i < 6; i++) {
    if (memcmp(buffer, allowedID[i], 4) == 0){
        color = color_matching[i];
        break;    
    }
  }
  return color;
}
