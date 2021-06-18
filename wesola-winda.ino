// pins configuration
// pins 0-1 are for serial communication
const int pinLiftUp = 2;
const int pinLiftDown = 3;
const int pinLiftPower = 4;
const int pinCallUp = 5;
const int pinCallDown = 6;
const int pinBtnUp = 7;
const int pinBtnDown = 8;
const int pinLedCallUp = 9;
const int pinLedCallDown = 10;
const int pinLedBtnUp = 11;
const int pinLedBtnDown = 12;
// pin 13 is for builtin LED
const int pinStopUp = 14;
const int pinStopDown = 15;

// zmienne na potrzeby rozróżniania stanów
//czy jedzie
bool liftRunningUp = false;
bool liftRunningDown = false;

// czy jest na górze / dole
bool liftUp = false;
bool liftDown = false;

// "blokadki" przycisków - true gdy obsłużyliśmy wciśnięcie i jeszcze nie został puszczony
bool lockPressBtnUp = false;
bool lockPressBtnDown = false;

// in lift btn states (reverse logic)
int stateBtnUp = HIGH;
int stateBtnDown = HIGH;

int lastStateBtnUp = HIGH;
int lastStateBtnDown = HIGH;

unsigned long lastDebounceTimeBtnUp = 0;
unsigned long lastDebounceTimeBtnDown = 0;
unsigned long debounceDelay = 100;

const int ledsBlinkingInterval = 800;
bool ledsBlinking = false;
int ledsCallState = LOW;
unsigned long ledsLastTime;


void setup() {
  pinMode(pinLiftUp, OUTPUT);
  pinMode(pinLiftDown, OUTPUT);
  pinMode(pinLiftPower, OUTPUT);
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
  stopLift();
  setLiftPosition();
}

void loop() {
  if (liftRunningUp) {
    if (reachedStopUp()) {
      stopLift();
      setLiftUp();
    }
    else if (manualStop()) {
      stopLift();
      setLiftPosition();
    }
  }
  else if (liftRunningDown) {
    if (reachedStopDown()) {
      stopLift();
      setLiftDown();
    }
    else if (manualStop()) {
      stopLift();
      setLiftPosition();
    }
  }
  else { // lift stopped
    if (!liftUp && (calledUp() || pressedBtnUp())) {
      startLiftUp();
    }
    else if (!liftDown && (calledDown() || pressedBtnDown())) {
      startLiftDown();
    }
  }

  if (ledsBlinking) {
    processLedsBlinking();
  }

  processLiftButtons(); // debouncing in lift buttons
}

void setLiftPosition() {
  if (reachedStopUp()) {
    setLiftUp();
  }
  else if (reachedStopDown()) {
    setLiftDown();
  }
  else {
    setLiftMiddle();
  }
}

bool reachedStopUp() {
  return digitalRead(pinStopUp) == HIGH;
}

bool reachedStopDown() {
  return digitalRead(pinStopDown) == HIGH;
}

bool manualStop() {
  return pressedBtnUp() || pressedBtnDown();
}

void stopLift() {
  digitalWrite(pinLiftUp, LOW);
  digitalWrite(pinLiftDown, LOW);
  delay(500);
  digitalWrite(pinLiftPower, LOW);
  liftRunningDown = false;
  liftRunningUp = false;

  stopLedsBlinking();

  waitAndSignalStop();
}

void waitAndSignalStop() {
  // wait 1s ignoring all input

  for (int i=0;i<2;i++) {
    digitalWrite(pinLedCallUp, HIGH);
    digitalWrite(pinLedCallDown, HIGH);
    digitalWrite(pinLedBtnUp, HIGH);
    digitalWrite(pinLedBtnDown, HIGH);
    delay(250);
    digitalWrite(pinLedCallUp, LOW);
    digitalWrite(pinLedCallDown, LOW);
    digitalWrite(pinLedBtnUp, LOW);
    digitalWrite(pinLedBtnDown, LOW);
    delay(250);
  }
}

void setLiftUp() {
  liftUp = true;
  liftDown = false;
}

void setLiftDown() {
  liftUp = false;
  liftDown = true;
}

void setLiftMiddle() {
  liftUp = false;
  liftDown = false;
}

// call and in-lift buttons working on reverse logic

bool calledUp() {
  return digitalRead(pinCallUp) == LOW;
}

bool calledDown() {
  return digitalRead(pinCallDown) == LOW;
}

void processLiftButtons() {
  int readingUp = digitalRead(pinBtnUp);
  if (readingUp != lastStateBtnUp) {
    lastDebounceTimeBtnUp = millis();
  }
  if ((millis() - lastDebounceTimeBtnUp) > debounceDelay) {
    stateBtnUp = readingUp;
  }
  lastStateBtnUp = readingUp;

  int readingDown = digitalRead(pinBtnDown);
  if (readingDown != lastStateBtnDown) {
    lastDebounceTimeBtnDown = millis();
  }
  if ((millis() - lastDebounceTimeBtnDown) > debounceDelay) {
    stateBtnDown = readingDown;
  }
  lastStateBtnDown = readingDown;
}

bool pressedBtnUp() {
  if (!lockPressBtnUp && stateBtnUp == LOW) {
    lockPressBtnUp = true;
    return true;
  }
  else if (lockPressBtnUp && stateBtnUp == HIGH) { // press locked but btn not pressed - so unlocking
    lockPressBtnUp = false;
    return false;
  }
  return false;
}

bool pressedBtnDown() {
  if (!lockPressBtnDown && stateBtnDown == LOW) {
    lockPressBtnDown = true;
    return true;
  }
  else if (lockPressBtnDown && stateBtnDown == HIGH) { // press locked but btn not pressed - so unlocking
    lockPressBtnDown = false;
    return false;
  }
  return false;
}

bool canStartUp() {
  return !liftUp && !liftRunningUp && !liftRunningDown && !reachedStopUp();
}

bool canStartDown() {
  return !liftDown && !liftRunningDown && !liftRunningUp && !reachedStopDown();
}

void startLiftUp() {
  if (canStartUp()) {
    liftRunningUp = true;
    liftRunningDown = false;
    digitalWrite(pinLiftDown, LOW);
    digitalWrite(pinLiftPower, HIGH);
    delay(500);
    digitalWrite(pinLiftUp, HIGH);
    

    ledsBlinking = true;
    digitalWrite(pinLedBtnUp, HIGH);
  }
}

void startLiftDown() {
  if (canStartDown()) {
    liftRunningUp = false;
    liftRunningDown = true;
    digitalWrite(pinLiftUp, LOW);
    digitalWrite(pinLiftPower, HIGH);
    delay(500);
    digitalWrite(pinLiftDown, HIGH);
    

    ledsBlinking = true;
    digitalWrite(pinLedBtnDown, HIGH);
  }
}

void processLedsBlinking() {
  unsigned long currentTime = millis();
  if (currentTime - ledsLastTime >= ledsBlinkingInterval) {
    ledsLastTime = currentTime;

    if (ledsCallState == LOW) {
      ledsCallState = HIGH;
    }
    else {
      ledsCallState = LOW;
    }
    digitalWrite(pinLedCallUp, ledsCallState);
    digitalWrite(pinLedCallDown, ledsCallState);
  }
}

void stopLedsBlinking() {
  ledsBlinking = false;
  ledsCallState = LOW;
  digitalWrite(pinLedCallUp, ledsCallState);
  digitalWrite(pinLedCallDown, ledsCallState);
}

/*
 * @todo:
 * - winda ruszyła w dół/górę, ktoś zatrzymał przyciskiem w windzie (manualStop) ale winda przejechała tak niewiele, że krańcówka nadal aktywna, a stan już się ustawił jako middle; można by sprawdzać krańcówki ustawiając windę jako middle
 */
