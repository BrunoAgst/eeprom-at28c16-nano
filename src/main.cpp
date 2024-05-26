#include <Arduino.h>

#define SH_CP 10
#define ST_CP 11
#define DATA 12
#define WE 13
#define OE 14
#define CE 15

#define D2 2
#define D3 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7
#define D8 8
#define D9 9

void setData(unsigned char data);
void setAddress(unsigned int address);
void setupReadOrWrite(unsigned int select);
void getData();
void writeEEPROM(unsigned int address, unsigned char data);
void readEEPROM(unsigned int address);
void readAllEEPROM(unsigned int size);
void writeAllEEPROM(const unsigned char *array, unsigned int size);

void setup() {
    Serial.begin(9600);
    pinMode(SH_CP, OUTPUT);
    pinMode(ST_CP, OUTPUT);
    pinMode(DATA, OUTPUT);
    pinMode(WE, OUTPUT);
    pinMode(OE, OUTPUT);
    pinMode(CE, OUTPUT);
    digitalWrite(CE, HIGH);

    unsigned char array[10] = {
            0xE3, 0x1F,
            0x4B, 0xB8,
            0x2A, 0x9C,
            0xF0, 0x7D,
            0x56, 0x21
    };

    writeAllEEPROM(array, 10);

    readAllEEPROM(10);
}

void loop() {}

void getData(){
    unsigned int pin = 2;
    unsigned char byte = 0x00;
    for(int i = 7; i >= 0; i--){
        unsigned int bit = digitalRead(pin);
        byte = byte | (bit << i);
        pin++;
        delay(5);
    }
    Serial.print("0x");
    Serial.print(byte, HEX);
    Serial.print(" ");
}


void setData(unsigned char data){
    unsigned int pin = 2;
    for(int i = 7; i >= 0; i--){
        unsigned char bit = data & (1 << i) ? HIGH : LOW;
        digitalWrite(pin, bit);
        pin++;
        delay(1);
    }
}

void setupReadOrWrite(unsigned int select){
    pinMode(D2, select);
    pinMode(D3, select);
    pinMode(D4, select);
    pinMode(D5, select);
    pinMode(D6, select);
    pinMode(D7, select);
    pinMode(D8, select);
    pinMode(D9, select);
}

void setAddress(unsigned int address){
    digitalWrite(ST_CP, LOW);
    for(int i = 11; i >= 0; i--){
        digitalWrite(DATA, address & (1 << i) ? HIGH : LOW);
        digitalWrite(SH_CP, HIGH);
        delay(1);
        digitalWrite(SH_CP, LOW);
    }
    digitalWrite(ST_CP, HIGH);
}

void writeEEPROM(unsigned int address, unsigned char data){
    digitalWrite(CE, LOW);
    delay(1);
    setAddress(address);
    setupReadOrWrite(OUTPUT);
    setData(data);
    digitalWrite(OE, 1);
    delay(1);
    digitalWrite(WE, 0);
    delay(1);
    digitalWrite(WE, 1);
    delay(1);
    digitalWrite(CE, HIGH);
    delay(500);
}

void readEEPROM(unsigned int address){
    digitalWrite(CE, LOW);
    delay(1);
    setAddress(address);
    setupReadOrWrite(INPUT);
    delay(1);
    digitalWrite(OE, 0);
    delay(1);
    getData();
    digitalWrite(OE, 1);
    delay(1);
    digitalWrite(CE, HIGH);
    delay(500);
}

void readAllEEPROM(unsigned int size){
    int pln = 0;
    for(int i = 0; i < size; i++){
        if(pln == 20){
            pln = 0;
            Serial.println();
        }
        readEEPROM(i);
        delay(1);
        pln++;
    }
}

void writeAllEEPROM(const unsigned char *array, unsigned int size){
    for(int i = 0; i < size; i++){
        writeEEPROM(i, array[i]);
    }
}
