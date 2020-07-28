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
      setLiftMiddle();
    }
  }
  else if (liftRunningDown) {
    if (reachedStopDown()) {
      stopLift();
      setLiftDown();
    }
    else if (manualStop()) {
      stopLift();
      setLiftMiddle();
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

  // @todo: obsłużyć ledy i reakcje na przyciski "call" gdy winda jedzie - a dokładnie brak tej reakcji (czyli tylko migać ledami)
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
  liftRunningDown = false;
  liftRunningUp = false;
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

// call and lift buttons working on reverse logic

bool calledUp() {
  return digitalRead(pinCallUp) == LOW;
}

bool calledDown() {
  return digitalRead(pinCallDown) == LOW;
}

bool pressedBtnUp() {
  return digitalRead(pinBtnUp) == LOW;
}

bool pressedBtnDown() {
  return digitalRead(pinBtnDown) == LOW;
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
    digitalWrite(pinLiftUp, HIGH);
  }
}

void startLiftDown() {
  if (canStartDown()) {
    liftRunningUp = false;
    liftRunningDown = true;
    digitalWrite(pinLiftUp, LOW);
    digitalWrite(pinLiftDown, HIGH);
  }
}
