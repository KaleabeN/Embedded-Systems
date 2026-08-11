#include <LiquidCrystal.h>
#include <Servo.h>
#include <Adafruit_Keypad.h>  // Using Adafruit Keypad library

Servo myservo;
int pos = 0; // position of servo motor
LiquidCrystal lcd(A4, A5, A3, A2, A1, A0);

// Keypad configuration for Adafruit Keypad library
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {0, 1, 2, 3};    // Connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 5, 6};       // Connect to the column pinouts of the keypad

// Create the Keypad
Adafruit_Keypad customKeypad = Adafruit_Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

char* password = "0123";  // Default password
int currentposition = 0;

void setup() {
  displayscreen();
  myservo.attach(9); // Servo motor connection
  lcd.begin(16, 2);
  customKeypad.begin();  // Initialize the keypad
}

void loop() {
  customKeypad.tick();  // Tick the keypad to check for presses
  
  while (customKeypad.available()) {
    keypadEvent e = customKeypad.read();
    if (e.bit.EVENT == KEY_JUST_PRESSED) {
      char key = (char)e.bit.KEY;
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PASSWORD:");
      lcd.setCursor(7, 1);
      lcd.print(" ");
      lcd.setCursor(7, 1);
      
      // Display asterisks for each entered character
      for (int l = 0; l <= currentposition; l++) {
        lcd.print("*");
      }
      
      if (key == password[currentposition]) {
        currentposition++;
        if (currentposition == 4) {
          unlockdoor();
          currentposition = 0;
        }
      } else {
        incorrect();
        currentposition = 0;
      }
    }
  }
  
  if (currentposition == 0) {
    displayscreen();
  }
}

//------------------ Function 1- OPEN THE DOOR--------------//
void unlockdoor() {
  delay(900);
  
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Access Granted");
  lcd.setCursor(4, 1);
  lcd.print("WELCOME!!");
  
  // Open the door
  for (pos = 180; pos >= 0; pos -= 5) {
    myservo.write(pos); 
    delay(5); 
  }
  
  delay(2000);
  counterbeep();
  delay(1000);
  
  // Close the door
  for (pos = 0; pos <= 180; pos += 5) {
    myservo.write(pos); 
    delay(15);
  }
  
  currentposition = 0;
  lcd.clear();
  displayscreen();
}

//--------------------Function 2- Wrong code--------------//
void incorrect() {
  delay(500);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("CODE");
  lcd.setCursor(6, 0);
  lcd.print("INCORRECT");
  lcd.setCursor(4, 1);
  lcd.print("GET AWAY!!!");
  
  delay(3000);
  lcd.clear();
  displayscreen();
}

//------------Function 3 - DISPLAY FUNCTION--------------------//
void displayscreen() {
  lcd.setCursor(0, 0);
  lcd.print("*ENTER THE CODE*");
  lcd.setCursor(1, 1);
  lcd.print("TO OPEN DOOR!!");
}

//--------------Function 4 - Count down------------------//
void counterbeep() {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("GET IN WITHIN:");
  
  for (int i = 5; i > 0; i--) {
    lcd.setCursor(4, 1);
    lcd.print(i);
    lcd.print(" ");
    delay(1000);
  }
  
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("RE-LOCKING");
  for (int i = 0; i < 3; i++) {
    lcd.print(".");
    delay(500);
  }
  
  delay(400);
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("LOCKED!");
  delay(440);
}
