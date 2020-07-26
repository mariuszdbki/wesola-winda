// konfiguracja pinow
const int pinLiftUp = 2;
const int pinLiftDown = 3;
const int pinStopUp = 4;
const int pinStopDown = 5;
const int pinCallUp = 6;
const int pinCallDown = 7;
const int pinBtnUp = 8;
const int pinBtnDown = 9;
const int pinLedCallUp = 10;
const int pinLedCallDown = 11;
const int pinLedBtnUp = 12;
const int pinLedBtnDown = 14;

// zmienne na potrzeby rozróżniania stanów
//czy jedzie
bool liftRunningUp = false;
bool liftRunningDown = false;

// czy zawołana
bool liftCalledUp = false;
bool liftCalledDown = false;

// czy jest na górze / dole
bool liftUp = false;
bool liftDown = false;

void setup() {
  pinMode(pinLiftUp, OUTPUT);
  pinMode(pinLiftDown, OUTPUT);
  pinMode(pinStopUp, INPUT);
  pinMode(pinStopDown, INPUT);
  pinMode(pinCallUp, INPUT_PULLUP);
  pinMode(pinCallDown, INPUT_PULLUP);
  pinMode(pinBtnUp, INPUT_PULLUP);
  pinMode(pinBtnDown, INPUT_PULLUP);
  pinMode(pinLedCallUp, OUTPUT);
  pinMode(pinLedCallDown, OUTPUT);
  pinMode(pinLedBtnUp, OUTPUT);
  pinMode(pinLedBtnDown, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

}
