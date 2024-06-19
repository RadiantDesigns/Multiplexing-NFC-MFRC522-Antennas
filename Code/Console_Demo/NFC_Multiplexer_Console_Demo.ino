#include <SPI.h>
#include <MFRC522.h>

#define MUX_ENA   2
#define MUX_ENB   3
#define MUX_A0    4
#define MUX_A1    5

#define RST_PIN   9
#define SS_PIN    10

MFRC522 mfrc522(SS_PIN, RST_PIN); // Instance of the class

int counter = 0;

char *TileNameArray[] = {"A1", "A2", "A3", "A4", "B1", "B2", "B3", "B4"};

int foundArray[] = {0, 0, 0, 0, 0, 0, 0, 0}; 

unsigned long myTime;

byte NUID_array[8][4] =           
  {
    {0x00, 0x00, 0x00, 0x00},   // 0x00 -> empty array
    {0x00, 0x00, 0x00, 0x00},       
    {0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00}   
  };


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



//----------------------------------------------------
void loop() {

  if(counter == 8){
    myTime = millis() - myTime;
    Serial.println();
    Serial.print("Time to loop over all 8 Tiles: ");
    Serial.println(myTime);
    
    counter = 1;
    for (int j = 0; j < 8; j++){
      Serial.println();
      Serial.print("Tile ");
      Serial.print(TileNameArray[j]);
      Serial.print(": ");
      for (byte i = 0; i < 4; i++) {
        Serial.print(NUID_array[j][i] < 0x10 ? " 0" : " ");
        Serial.print(NUID_array[j][i], HEX);
        NUID_array[j][i] = 0x00; 
      }
      foundArray[j] = 0;
    }

    Serial.println();
    Serial.println("Press a key to continue..");
    while (!Serial.available()){}
    while (Serial.available()){
      Serial.read();
    }

    myTime = millis();
    
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

      for (byte i = 0; i < 4; i++) {
          NUID_array[counter-1][i] = mfrc522.uid.uidByte[i];
      }

      foundArray[counter-1] = 1;
      
      mfrc522.PICC_HaltA();       // Halt PICC
      mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
  
      mfrc522.PCD_Reset();        
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
