#include "pitches.h";
//TODO make a simon mode so pattern extends instead of randomizing


int STARTING_PIN_OUT = 2;
int STARTING_PIN_IN = 6;
int AMOUNT_LEDS = 4;
int HERTZ_MUL = 120;
int PLAY_DELAY = 600;
int PAUSE_DELAY = 200;
int SPEAKER_PIN = 13;

int SUCCESS_SOUND[14] = {NOTE_G3, NOTE_G3, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_E4, NOTE_D4};
int SUCCESS_DELAY[14] = {200, 200, 200, 200, 200, 200, 400};

int FAILURE_SOUND[48] = {
                        NOTE_G3, 
                        NOTE_C4, NOTE_G3, NOTE_A3, NOTE_B3, NOTE_E3, NOTE_E3, 
                        NOTE_A3, NOTE_G3, NOTE_F3, NOTE_G3, NOTE_C3, NOTE_C3, 
                        NOTE_D3, NOTE_D3, NOTE_E3, NOTE_F3, NOTE_F3, NOTE_G3,
                        NOTE_A3, NOTE_B3, NOTE_C4, NOTE_D4,
                        NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_B3, NOTE_G3,
                        NOTE_C4, NOTE_B3, NOTE_A3, NOTE_B3, NOTE_E3, NOTE_E3,
                        NOTE_A3, NOTE_G3, NOTE_F3, NOTE_G3, NOTE_C3, NOTE_C3,
                        NOTE_C4, NOTE_B3, NOTE_A3, NOTE_G3, NOTE_B3, NOTE_C4, NOTE_D4
                        };
int FAILURE_DELAY[48] = {
                        400, 
                        800, 600, 200, 800, 400, 400, 
                        800, 600, 200, 800, 400, 400,
                        800, 400, 400, 800, 400, 400,
                        800, 400, 400, 1600,
                        800, 600, 200, 800, 400, 400,
                        800, 600, 200, 800, 400, 400,
                        800, 600, 200, 800, 600, 200,
                        800, 600, 200, 400, 400, 400, 1200
                        };

//TODO make used play delay dependent on holding button
int USER_PLAY_DELAY = 500;

int ROUND_END_DELAY = 1000;

int roundCounter = 0;
int *vals;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(11)); // nothing connected to 11 so read sees noise

  //init in- and output pins
  for(int i = 0; i < AMOUNT_LEDS; i++) {
    pinMode(i + STARTING_PIN_OUT, OUTPUT);
    pinMode(i + STARTING_PIN_IN, INPUT);
  }
  pinMode(SPEAKER_PIN, OUTPUT);
}

void loop() {
  roundCounter += 1;
  vals = generateSimonVals(vals);
  (void)playVals(vals);
  (void)userPlays(vals);

  //round finished, delay a bit
  delay(ROUND_END_DELAY);
}

int* generateSimonVals(int *vals) {
  int *tempVals = (int*)malloc(roundCounter * sizeof(int));
  if(roundCounter == 1) {
    tempVals[0] = random(0, AMOUNT_LEDS);
  } else {
    for(int i = 0; i < roundCounter-1; i++) {
      tempVals[i] = vals[i];
    }
    tempVals[roundCounter-1] = random(0, AMOUNT_LEDS);
  }
  //free malloced array with vals
  free(vals);
  return tempVals;
}

//fill play array with vals corresponding to an LED and a note
int* generateVals() {
  int *vals = (int*)malloc(roundCounter * sizeof(int));
  for(int i = 0; i < roundCounter; i++) {
    vals[i] = random(0, AMOUNT_LEDS);
  }
  return vals;
}

//play notes and light up LEDs consecutively with a delay
void playVals(int *vals) {
  for(int i = 0; i < roundCounter; i++) {
    tone(SPEAKER_PIN, (vals[i] + 1) * HERTZ_MUL);
    digitalWrite(vals[i] + STARTING_PIN_OUT, HIGH);
    delay(PLAY_DELAY);
    //check and see if this is necessary
    noTone(SPEAKER_PIN);
    digitalWrite(vals[i] + STARTING_PIN_OUT, LOW);
    delay(PAUSE_DELAY);
  }
}

//handle user turn
void userPlays(int *vals) {
  if(checkUserSuccess(vals)) {
    //user has succeeded => play success sound
    //get number of sounds
    int successSoundsAmount = sizeof(SUCCESS_SOUND)/sizeof(SUCCESS_SOUND[0]);
    //play success notes
    for(int i = 0; i < successSoundsAmount; i++) {
      tone(SPEAKER_PIN, SUCCESS_SOUND[i]);
      delay(SUCCESS_DELAY[i]);
      noTone(SPEAKER_PIN);
    }
  } else {
    //user has failed => play fail sound
    //get number of sounds
    int failSoundsAmount = sizeof(FAILURE_SOUND)/sizeof(FAILURE_SOUND[0]);
    //play fail notes
    for(int i = 0; i < failSoundsAmount; i++) {
      tone(SPEAKER_PIN, FAILURE_SOUND[i]);
      delay(FAILURE_DELAY[i]/2);
      noTone(SPEAKER_PIN);
    }
    //restart game
    roundCounter = 0;
  }
}

int checkUserSuccess(int *vals) {
  //go through every play val
  for(int i = 0; i < roundCounter; i++) {
    int correctPress = 0;
    int pressed = 0;
    int pressedButton = 1;
    //wait for button to be pressed
    while(!pressed) {
      //check HIGH/LOW for every button
      for(int j = 0; j < AMOUNT_LEDS && !pressed; j++) {
        if(digitalRead(j + STARTING_PIN_IN) == HIGH){
          //button was pressed
          if(j == vals[i]) {
            //button was correct
            correctPress = 1;
          }
          pressed = 1;
          pressedButton = j;
          //exit for and while loop
          continue;
        }
      }
    }
    //play notes and light up LEDs depending on what user pressed
    tone(SPEAKER_PIN, (pressedButton + 1) * HERTZ_MUL);
    digitalWrite(pressedButton + STARTING_PIN_OUT, HIGH);
    while(digitalRead(pressedButton + STARTING_PIN_IN) == HIGH){}
    noTone(SPEAKER_PIN);
    digitalWrite(pressedButton + STARTING_PIN_OUT, LOW);

    if(!correctPress) {
      return 0;
    }
  }
  return 1;
}

