#include <LiquidCrystal.h>

#define DELAY_LOOP 300

// Macro sensore ultrasuoni
#define SOUND_SPEED 0.0343

#define S1_D 20
#define S2_D 40
#define S3_D 80
#define S4_D 150

#define MAX_D 200

// Macro buzzer
#define TONE 2000

#define S1_P 50
#define S2_P 100
#define S3_P 300
#define S4_P 400

// Macro LED
#define RED 8
#define GREEN 9
#define BLUE 10

#define L_OFF 0
#define L_RED 1
#define L_GREEN 2
#define L_BLUE 3
#define L_MAGENTA 4
#define L_YELLOW 5
#define L_CYAN 6
#define L_WHITE 7


// Macro LCD
#define MAX_BLOCKS 16

/*

+-------+-----------+--------+
| STAGE | DISTANCE  | PERIOD |
|-------+-----------+--------+
|   1   |   0 - 20  |    50  |
|   2   |  20 - 40  |   100  |
|   3   |  40 - 80  |   300  |
|   4   |  80 - 150 |   400  |
|   5   | 150 - 200 |   ///  |
|  MAX  |   > 200   |   ///  |
+-------+-----------+--------+

*/

// Pin sensore ultrasuoni
const int transmitterPin = 6;
const int receiverPin = 7;

// Pin buzzer
const int buzzerPin = 13;
const int buttonPin = A0;

// Inizializzazione LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


// Altre variabili
float duration, distance;
bool isActive;
int buttonState;
int numBlocks;
bool firstActivation = false;


void setup() {

    // Inizializzazione PIN e Variabili
    pinMode(transmitterPin, OUTPUT);
    pinMode(receiverPin, INPUT);

    pinMode(buzzerPin, OUTPUT);

    pinMode(buttonPin, INPUT);

    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);

    distance = 0;
    isActive = false;

    lcd.begin(16, 2);
    delay(1000);

    // Intro & Welcome Message
    lcd.write("   Benvenuto!   ");
    delay(2000);
    lcd.clear();
    lcd.write(" Premi bottone ");
    lcd.setCursor(0, 1);
    lcd.write("  per attivare  ");

    Serial.begin(9600);
}


void loop() {

    // Entro nell'if solo se il bottone viene premuto
    if(analogRead(buttonPin) > 600){

        isActive = !isActive;

        // Accensione dispositivo
        if(isActive){
            light(L_GREEN);

            tone(buzzerPin, TONE);
            .
            .
            .
        }

        // Spegnimento dispositivo
        else{
            light(L_RED);

            lcd.clear();
            lcd.write(" Premi bottone ");
            lcd.setCursor(0, 1);
            lcd.write("  per attivare  ");
        }

        delay(DELAY_LOOP);
        
        light(L_OFF);
    }

    // Entro nell'if quando il dispositivo è acceso
    if(isActive){

        // Segnale impulso sensore ultrasuoni
        digitalWrite(transmitterPin, LOW);
        delayMicroseconds(2);
        digitalWrite(transmitterPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(transmitterPin, LOW);

        // Calcolo e stampa distanza
        duration = pulseIn(receiverPin, HIGH);
        distance = (duration * SOUND_SPEED) / 2;

        setDistance(distance);

        // Calcolo del numero di blocchi da far visualizzare
        numBlocks = getDistanceBlocks(distance);

        // Stampa blocchi nel display LCD
        for (int i = 0; i < numBlocks; i++) {
            lcd.setCursor(i, 1);
            lcd.write(255);
            delay(10);
        }


        // Gestione stage avvicinamento
        if(distance < S4_D){

            if(distance < S1_D){
                tone(buzzerPin, TONE);
                light(L_RED);

                delay(S1_P);
            }
            else if(distance < S2_D){
                tone(buzzerPin, 1750);
                light(L_YELLOW);

                delay(S2_P);
            }
            else if(distance < S3_D){
                tone(buzzerPin, 3000);
                light(L_GREEN);

                delay(S3_P);
            }
            else{
                tone(buzzerPin, 1000);
                light(L_WHITE);

                delay(S4_P);
            }

            noTone(buzzerPin);
        }
        else{
            light(L_OFF);

            delay(DELAY_LOOP);
        }
        
        resetCursor();
    }

    delay(DELAY_LOOP);
}


// Procedura che gestisce i possibili colori del LED
void light(int state){

    switch(state){

        case L_OFF:
            
            digitalWrite(RED, LOW);
            digitalWrite(GREEN, LOW);
            digitalWrite(BLUE, LOW);
            
            break;


        case L_RED:

            digitalWrite(RED, HIGH);
            digitalWrite(GREEN, LOW);
            digitalWrite(BLUE, LOW);
            
            break;


        case L_GREEN:

            digitalWrite(RED, LOW);
            digitalWrite(GREEN, HIGH);
            digitalWrite(BLUE, LOW);
            
            break;


        case L_BLUE:

            digitalWrite(RED, LOW);
            digitalWrite(GREEN, LOW);
            digitalWrite(BLUE, HIGH);
            
            break;


        case L_MAGENTA:

            digitalWrite(RED, HIGH);
            digitalWrite(GREEN, LOW);
            digitalWrite(BLUE, HIGH);
            
            break;


        case L_YELLOW:

            digitalWrite(RED, HIGH);
            digitalWrite(GREEN, HIGH);
            digitalWrite(BLUE, LOW);

            break;

            
        case L_CYAN:

            digitalWrite(RED, LOW);
            digitalWrite(GREEN, HIGH);
            digitalWrite(BLUE, HIGH);

            break;


        case L_WHITE:

            digitalWrite(RED, HIGH);
            digitalWrite(GREEN, HIGH);
            digitalWrite(BLUE, HIGH);

            break;
    }
}


// Procedura che pulisce il display ad ogni loop
void resetCursor(){

    for(int j = 0; j < MAX_BLOCKS; j++){
        lcd.setCursor(j, 1);
        lcd.print(" ");
    }

    for(int j = 10; j < MAX_BLOCKS; j++){
        lcd.setCursor(j, 0);
        lcd.print(" ");
    }

}

// Funzione che calcola il numero di blocchi da visualizzare
int getDistanceBlocks(float distance){
    
    int numBlocks = 0;

    numBlocks = (distance * MAX_BLOCKS) / MAX_D;

    return MAX_BLOCKS - numBlocks;

}

// Procedura che gestisce la stampa della distanza (centimetri o metri)
void setDistance(float distance){
    
    float newDistance = distance;

    if(distance < 1000){

        lcd.setCursor(10, 0);

        if(newDistance > 100){
            newDistance /= 100;
            lcd.print(newDistance, 2);
            lcd.print(" m");
        }
        else{
            lcd.print(distance, 0);
            lcd.print(" cm");
        }
    }
}