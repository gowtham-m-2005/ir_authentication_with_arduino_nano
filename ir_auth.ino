#include <Crypto.h>
#include <SHA256.h>
#include<IRremote.h>

#define SHA256_BLOCK_SIZE 32
uint8_t computed_hash[SHA256_BLOCK_SIZE];
uint8_t stored_hash[SHA256_BLOCK_SIZE] = {
    0x05, 0x81, 0x80, 0x2F, 0xFD, 0x34, 0x50, 0x93,
    0xE0, 0x45, 0xBF, 0xE4, 0x92, 0x04, 0xFF, 0x5D,
    0xCF, 0x38, 0x87, 0x89, 0xF4, 0x7E, 0x4D, 0xEB,
    0xD0, 0x5A, 0x60, 0x63, 0xA3, 0x84, 0x00, 0xD8
};

bool entered_passcode = false;
bool is_hashed = false;
bool printed_stored_hash = false;
bool auth_check_done = false;

const int IR_RECV_PIN = 2;

void setup() {
  Serial.begin(300);
  IrReceiver.begin(IR_RECV_PIN, DISABLE_LED_FEEDBACK);
}

void loop() {
  String passcode;
  SHA256 sha256;
  //Getting the passcode from IR Receiver
  if(entered_passcode != true)
    passcode = get_passcode();

  //computed_hashing with sha256
  if(is_hashed != true && passcode != "")
    hash_with_sha256(passcode, sha256);

  if(is_hashed && printed_stored_hash == false){
    printed_stored_hash = true;
    Serial.println("Stored Hash");
    print_hash(stored_hash);
    Serial.println();
  }

  if(is_hashed && printed_stored_hash && auth_check_done == false){
    auth_check_done = true;
    if(compare_hashes(computed_hash,stored_hash) == true)
      Serial.println("Access Granted!");
    else{
    Serial.println("Access Denied!");
  }
  }
}

String get_passcode(){
  String passcode = "";

  Serial.println("Enter your Passcode");
  while(entered_passcode == false){
    if((IrReceiver.decode()) && (IrReceiver.decodedIRData.decodedRawData != 0x0)){
      if(IrReceiver.decodedIRData.decodedRawData == 0xFCABFFBD){
        entered_passcode = true;
        continue;
      }
      passcode = passcode + String(IrReceiver.decodedIRData.decodedRawData,HEX);
      Serial.println(IrReceiver.decodedIRData.decodedRawData,HEX);
      //IrReceiver.printIRResultShort(&Serial); // Print complete received data in one line
      //IrReceiver.printIRSendUsage(&Serial);   // Print the statement required to send this data
      IrReceiver.resume();
    }
  }
  Serial.println(passcode);
  return passcode;
}

void hash_with_sha256(String passcode, SHA256 sha256){
  Serial.println("Validation Passcode,Please Wait...");
  
  sha256.update(passcode.c_str(), passcode.length());
  sha256.finalize(computed_hash, SHA256_BLOCK_SIZE);
  is_hashed = true;

  Serial.println("Computed Hash");
  print_hash(computed_hash);
  Serial.println();
}

void print_hash(uint8_t* hash){
  for (int i = 0; i < SHA256_BLOCK_SIZE; ++i){
      if (hash[i] < 0x10){
        Serial.print("0");
      }
      Serial.print(hash[i], HEX);
  }
}

bool compare_hashes(uint8_t* hash_1,uint8_t* hash_2){
  for (int i = 0; i < SHA256_BLOCK_SIZE; ++i){
      if(hash_1[i] == hash_2[i])
        return true;
      return false;
  }
}