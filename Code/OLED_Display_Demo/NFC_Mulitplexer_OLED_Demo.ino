#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define MUX_ENA   2
#define MUX_ENB   3
#define MUX_A0    4
#define MUX_A1    5

#define RST_PIN   9
#define SS_PIN    10

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

MFRC522 mfrc522(SS_PIN, RST_PIN); // Instance of the class

// Declaration for SSD1306 display connected using I2C
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int counter = 0;

int foundArray[] = {0, 0, 0, 0, 0, 0, 0, 0}; 
int xValArray[] = {69, 46, 23, 0, 69, 46, 23, 0}; 
int yValArray[] = {39, 39, 39, 39, 16, 16, 16, 16};

unsigned long myTime;

//----------------------------------------------------
void setup() {

  myTime = millis();

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

  // initialize the OLED object
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
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

  // Clear the buffer and screen.
  display.clearDisplay();
  display.display();

  display.setTextSize(1); //height=9 pixels, width=5 pixels at textsize 1
  display.setTextColor(WHITE);
  display.setCursor(15, 100);
  display.setRotation(3); //rotates text on OLED 1=90 degrees, 2=180 degrees
  display.print("PCB");

  display.setRotation(0); 
  display.drawRect(0, 16, 24, 24, WHITE); //B4
  display.drawRect(0, 39, 24, 24, WHITE); //A4
  display.drawRect(23, 16, 24, 24, WHITE); //B3
  display.drawRect(23, 39, 24, 24, WHITE); //A3
  display.drawRect(46, 16, 24, 24, WHITE); //B2
  display.drawRect(46, 39, 24, 24, WHITE); //A2
  display.drawRect(69, 16, 24, 24, WHITE); //B1
  display.drawRect(69, 39, 24, 24, WHITE); //A1
  display.drawRect(92, 16, 24, 47, WHITE);
  display.display();
}


//----------------------------------------------------
void loop() {

  if(counter == 8){

    display.clearDisplay();
    display.setTextSize(1); //height=9 pixels, width=5 pixels at textsize 1
    display.setTextColor(WHITE);
    display.setCursor(15, 100);
    display.setRotation(3); //rotates text on OLED 1=90 degrees, 2=180 degrees
    display.print("PCB");
    display.setRotation(0);
    
    counter = 1;
    for (int j = 0; j < 8; j++){
      if(foundArray[j] == 0){
          display.drawRect(xValArray[j], yValArray[j], 24, 24, WHITE);  
      }else{
          display.fillRect(xValArray[j], yValArray[j], 24, 24, WHITE);
      }
      display.drawRect(92, 16, 24, 47, WHITE);
      display.display();
      foundArray[j] = 0;
    }
    
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

      foundArray[counter-1] = 1;

      mfrc522.PICC_HaltA();       // Halt PICC
      mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
  
      mfrc522.PCD_Reset();        // Test soft reset
      mfrc522.PCD_Init();
  }

}


//----------------------------------------------------
// ADDITIONAL FUNCTIONS SECTION
void dump_byte_array(byte * buffer, byte bufferSize) {          // Helper function which steps through each of the bytes in the tags ID and prints them to the terminal
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
