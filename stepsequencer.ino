//create an array for LEDs
int ledArray[4] = {20, 19, 18, 17};
//create an array for the buttons
int buttonArray[4] = {3, 4, 5, 6};
//initialize all pots and switches

//set analog pin for tempo
int potPin = A7;
//initialize tempo
int tempo = 0;
int togSwitch = 1;

//initialize variabls for steps
int currentStep = 0;
unsigned long lastSteptime = 0;

//create an array of 4 items for the button state and on functions
boolean lastButtonState[4] = {LOW, LOW, LOW, LOW};
boolean buttonState[4] = {LOW, LOW, LOW, LOW};
boolean on[4] = {false, false, false, false};

//put MIDI notes to play in a list
int playNote[4] = {60, 62, 64, 66}; 

// set each pin in the array as an output
// set each button as an input
void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(ledArray[i], OUTPUT);
    pinMode(buttonArray[i], INPUT);
  }
  pinMode(togSwitch, INPUT);
}


void loop() {
  sequence();
  checkButtons();
  doLeds();
}


//have sequence include the back and forward function
void      sequence() {

  tempo = analogRead(potPin);
  if (digitalRead(togSwitch) == HIGH) {
    stepLed();
  } else {
    backLed();
  }
}


//have the tempo effect the speed of incrementation
//use currentstep to increment
//if the LED is on and the button is pressed, the note should be played
//note must start turned off
void      stepLed() {

  if (millis() > lastSteptime + tempo) {

  usbMIDI.sendNoteOff(playNote[currentStep], 0, 1);
  

    //increment
    currentStep = currentStep + 1;
    if (currentStep > 3) {
      currentStep = 0;
    }
    lastSteptime = millis();


    if (on[currentStep] == true){
      usbMIDI.sendNoteOn(playNote[currentStep], 127, 1);
    }
  }
}


// do the same steps as the forward function but count down from 3 to 0
//have the tempo effect the speed of incrementation
//if the LED is on and the button is pressed, the note should be played
//note must start turned off
void      backLed() {
  if (millis() > lastSteptime + tempo) {

    usbMIDI.sendNoteOff(playNote[currentStep], 0, 1);

    //increment down
    currentStep = currentStep - 1;
    if (currentStep < 0) {
      currentStep = 3;
    }
    lastSteptime = millis();

    if (on[currentStep] == true){
      usbMIDI.sendNoteOn(playNote[currentStep], 127, 1);
    }
  }
}


//iterate through the boolean variables
//make the button on when its pressed and off when its pressed again
void checkButtons() {

  for (int i = 0; i < 4; i++) {

    lastButtonState[i] = buttonState[i];
    buttonState[i] = digitalRead(buttonArray[i]);

    if (buttonState[i] == HIGH && lastButtonState[i] == LOW) {
      if (on[i] == false) {
        on[i] = true;
      } else if (on[i] == true) {
        on[i] = false;
      }
    }
  }
}

//this step iterates through the LEDs and will turn them on in the sequence or if the button is pressed
void doLeds() {

  for (int i = 0; i < 4; i++) {

    if (on[i] == true or i == currentStep) {
      digitalWrite(ledArray[i], HIGH);
    } else if (on[i] == false) {
      digitalWrite(ledArray[i], LOW);
    }
  }
}
