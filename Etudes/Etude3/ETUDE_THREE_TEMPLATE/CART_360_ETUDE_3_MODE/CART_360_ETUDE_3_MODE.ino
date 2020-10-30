/**********************ETUDE 3 CART 360 2020*******************************
 * WELCOME! 
 * THE PURPOSE OF THIS EXERCISE IS TO DESIGN A VERY SIMPLE KEYBOARD (4 KEYS)
 * WHICH ALLOWS YOU TO PLAY LIVE, RECORD, AND PLAYBACK SINGLE NOTES (NO CHORDS OR TIMING). 
 * THERE WILL BE A BUTTON WHICH WHEN PRESSED WILL TAKE THE USER TO THE NEXT MODE:
 * THERE ARE 5 POSSIBLE MODES
 * 0 ==  reset    ==   led off == also resets
 * 1 ==  live     ==   led BLUE
 * 2 ==  record   ==   led RED
 * 3 ==  play     ==   led GREEN
 * 4 ==  looper   ==   led PURPLE
 * 
 * ADDITIONALLY - WHEN THE USER CHANGES MODE, 
 * THE RGB LED WILL CHANGE COLOR (ACCORDING TO THE SPECIFICATIONS)

 * PLEASE FOLLOW THE INSTRUCTIONS IN THE COMMENTS:
 * DO NOT ADD ANY MORE FUNCTION DEFINITIONS 
 * ONLY IMPLEMENT THE FUNCTION DEFINITIONS SUPPLIED
 * THERE IS NO NEED TO ADD ANY NEW VARIABLES OR CONSTANTS
 * PROVIDE COMMENTS FOR ANY OF THE CODE IMPLEMENTED
 * GOOD LUCK!
 */
/**** CONSTANTS ********************************************************/
#define BUTTON_MODE_PIN 2 // Button to change the mode

// constants for RGB LED
#define LED_PIN_R 9  // B PIN
#define LED_PIN_G 10 // G PIN
#define LED_PIN_B 11 // R PIN

// constant for note in (button-resistor ladder on breadboard)
# define NOTE_IN_PIN A0

//constant for max number of notes in array
#define MAX_NOTES 16

// duration for a single played note
const int duration = 200;

// constant for pin to output for buzzer 
#define BUZZER_PIN 3 // PWM

/******** VARIABLES *****************************************************/
// counter for how many notes we have
int countNotes = 0;
int mode = 0; // start at off
// array to hold the notes played (for record/play mode)
int notes [MAX_NOTES]; 

/*********VARIABALES OF THE LOOP FUNCTION*******************************************************/
// The array which would be used to check if the new generated random number is repetitve or not
int randomNumberChecker [MAX_NOTES];
// The array which stores the verified generated random numbers.
int mainSetOfRandNum[MAX_NOTES];
// The variable in while loop which makes sure all 16 elements of randomNumber array has been filled
int k = 1;

/**********************SETUP() DO NOT CHANGE*******************************/
// Declare pin mode for the single digital input
void setup()
{  
  pinMode(BUTTON_MODE_PIN, INPUT); // Receive input from the button
  // Display the output through the RGB LED and Piezo Buzzer
  pinMode(LED_PIN_R, OUTPUT); 
  pinMode(LED_PIN_G, OUTPUT);
  pinMode(LED_PIN_B, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin (115200);
  while(!Serial);  
}
/**********************LOOP() DO NOT CHANGE *******************************
 * INSTRUCTIONS: 
 * There is NO NEED to change the loop - it establishes the flow of the program
 * We call here 3 functions repeatedly:
 * 1: chooseMode(): this function will determine the mode that your program is in 
 * based on if the button (linked to the BUTTON_MODE_PIN) was pressed
 * 2: setRGB(): will set the color of the RGB LED based on the value of the mode variable
 * 3: selectMode(): will determine which function to call based on the value of the mode variable
**************************************************************************/
void loop()
{  
  chooseMode();
  setRGB(mode);
  selectMode();
}
/******************CHOOSEMODE(): IMPLEMENT *********************************
 * INSTRUCTIONS:
 * Read the value from the Button (linked to the BUTTON_MODE_PIN) and 
 * if is being pressed then change the mode variable.
 * REMEMBER:
 * mode == 0 is reset 
 * mode == 1 is live == B
 * mode == 2 is record == R
 * mode == 3 is play == G
 * mode == 4 is loopMode == P
 * Every time the user presses the button, the program will go to the next mode,
 * once it reaches 4, it should go back to mode == 0. 
 * (i.e. if mode ==2 and we press, then mode ==3) ...
**************************************************************************/
void chooseMode()
{  
  // Check if the button is pressed..
  if (digitalRead(BUTTON_MODE_PIN) == HIGH) {
     // If the mode's value is < 4:
     if (mode < 4) {
        // Add one to it
        mode ++; 
     }
     // If the mode's value is >= 4: 
     else  { 
        // Reset it to its initial value
        mode = 0;
     }
     // This delay allows the system to run the code only once
     // every time the button is pressed.
     delay (duration*5); 
  }
}
/******************SETRGB(): IMPLEMENT *********************************
 * INSTRUCTIONS:
 * Depending on the value of the mode variable:
 * if the mode is 0 - RGB LED IS OFF
 * if the mode is 1 - RGB LED IS BLUE
 * if mode is 2 - RGB LED IS RED
 * if mode is 3 - RGB LED IS GREEN
 * if mode is 4 - RGB LED iS PURPLE
 * YOU MUST USE A SWITCH CASE CONSTRUCT (NOT A SERIES OF IF / ELSE STATEMENTS
**************************************************************************/
void setRGB(int modeColor)
{ 
  // Specify the color of each mode based on the mode variable value
  switch (modeColor) {
     case 0:
        digitalWrite(LED_PIN_R, 0);
        digitalWrite(LED_PIN_G, 0);
        digitalWrite(LED_PIN_B, 0);
        break;
     case 1:
        digitalWrite(LED_PIN_R, 255);
        digitalWrite(LED_PIN_G, 0);
        digitalWrite(LED_PIN_B, 0);
        break;
     case 2:
        digitalWrite(LED_PIN_R, 0);
        digitalWrite(LED_PIN_G, 0);
        digitalWrite(LED_PIN_B, 255);
        break;
     case 3:
        digitalWrite(LED_PIN_R, 0);
        digitalWrite(LED_PIN_G, 255);
        digitalWrite(LED_PIN_B, 0);
        break;
     case 4:
        digitalWrite(LED_PIN_R, 102);
        digitalWrite(LED_PIN_G, 51);
        digitalWrite(LED_PIN_B, 153);
        break;
  }
}
/**********************SELECTMODE() DO NOT CHANGE *******************************
 * INSTRUCTIONS: 
 * There is NO NEED to change this function - it selects WHICH function should run based on the mode variable
 * There are 4 possibilities
 * 1: reset(): this function will reset any arrays etc, and will do nothing else
 * 2: live(): this function will play the corresponding notes 
 * to the user pressing the respective buttons. 
 * NOTE:: the notes played are NOT stored
 * 3: record(): this function will play the corresponding notes 
 * to the user pressing the respective buttons
 * AND will STORE up to 16 consecutive notes in an array.
 * 4: play(): this function will playback any notes stored in the array that were recorded
 * in the previous mode
 * 5: loopMode(): this function will playback any notes stored in the array that were recorded, 
 * BUT unlike the previous mode, you can choose in which sequence the notes are played.
 * REQUIRED: only play notes from the array (no live stuff)
******************************************************************************/
void selectMode()
{
  if(mode == 0) { 
    reset();
  }
  else if(mode == 1) {
    live();
  }
  else if(mode == 2) {
    record();
  }
  
  else if(mode == 3) {
    play();
  }
   
   else if(mode == 4) {
    looper();
  }
}
/******************RESET(): IMPLEMENT **************************************
 * INSTRUCTIONS:
 * this function should ensure that any notes recorded are no longer available
**************************************************************************/
void reset()
{
  // To reset the notes array, 
  // assign zero to all of the array's elements respectively
  for (int i = 0; i < MAX_NOTES; i++){
      notes[i] = 0;
      // Also resets the countNotes and mode value
      countNotes = 0;
      mode = 0;
  }  
}
/******************LIVE(): IMPLEMENT **************************************
 * INSTRUCTIONS:
 * this function will play the corresponding notes 
 * to the user pressing the respective buttons
 * NOTE:: the notes played are NOT stored
 * SO: you need read in the input from the analog input (linked to the button-resistor ladder combo)
 * THEN - output the note to the buzzer using the tone() function
**************************************************************************/
void live()
{
  // Duration variable value lets the compiler to have enough time to read the tones 
  // being played respectively, and returns them as audio through the buzzer 
  tone( BUZZER_PIN, analogRead(NOTE_IN_PIN), duration);
}
/******************RECORD(): IMPLEMENT **********************************
 * INSTRUCTIONS:
 * this function will play the corresponding notes 
 * to the user pressing the respective buttons
 * AND will STORE up to 16 consecutive notes in an array.
 * SO:you need read in the input from the analog input (linked to the button-resistor ladder combo)
 * AND - output the note to the buzzer using the tone() function
 * THEN store that note in the array  - BE CAREFUL - you can only allow for up to MAX_NOTES to be stored
**************************************************************************/
void record()
{   
  // While the number of notes being played is < total number of notes:
  while (countNotes < MAX_NOTES) {
      // First, give the system enough time to store the code in the correct array element
      // Then, receive the next note and play it
      delay(2000);
      // Play the note through the buzzer
      tone(BUZZER_PIN, analogRead(NOTE_IN_PIN), duration);
      // Store the note that has just been played in the array
      notes[countNotes] = {analogRead(NOTE_IN_PIN)};
      
      // Go for the next note
      countNotes += 1;
  }
}
/******************PLAY(): IMPLEMENT ************************************
 * INSTRUCTIONS:
 * this function will playback any notes stored in the array that were recorded
 * in the previous mode
 * SO: you need to go through the array of values (be careful - the user may not have put in MAX_NOTES)
 * READ each value IN ORDER
 * AND output each note to the buzzer using the tone() function
 * ALSO: as long as we are in this mode, the notes are played over and over again
 * BE CAREFUL: make sure you allow for the user to get to another mode from the mode button...
**************************************************************************/
void play()
{ 
  // Play the notes stored in the notes array
  for (int j = 0; j < MAX_NOTES; j++){
     tone( BUZZER_PIN, notes[j], duration);
     delay(duration);
  }
  delay(duration*2);
  // If the button is pressed, stop the loop
  if (digitalRead(BUTTON_MODE_PIN) == HIGH) {
     noTone(BUZZER_PIN);
     // Reset the countNotes value to zero
     countNotes = 0;
  }
}
/******************LOOPMODE(): IMPLEMENT *********************************
 * INSTRUCTIONS:
 * this function will playback any notes stored in the array that were recorded
 * in the previous mode
 * SO: you need to go through the array of values (be careful - the user may not have put in MAX_NOTES)
 * READ values IN ANY ORDERING (You MUST use the array - but you can determine your own sequence)
 * AND output each note to the buzzer using the tone() function
 * ALSO: as long as we are in this mode, the notes are played over and over again
 * BE CAREFUL: make sure you allow for the user to get to another mode from the mode button...
**************************************************************************/
void looper()
{   
  // Make sure if new random number already exists in our randomNumberChecker array or not
  boolean alreadyExist = false;
  
  // The code which randomizes the order of notes array elements
  while (k < MAX_NOTES) {
     // The variable which stores the new generated random number in each round
     int random1 = random(1, 16);

     // The condition which checks if this new random number matches any of the previous ones:
     for (int l= 1; l < MAX_NOTES; l++){
         if (random1 == randomNumberChecker[l]) {
            // If the random number already exists, make the boolean value true
            alreadyExist = true;
            // Break the loop
            break;
         }
     }
     // If the random number doesn't exists already:
     if (!alreadyExist) {
        // Assign this new random number to the main array which is collecting the correct set of random numbers
        mainSetOfRandNum[k] = random1;
        // Also, add this new random number to the randomNumberChecker array which is used to check repetitiveness
        randomNumberChecker[k] = random1;
        // Go for the next element of the main array () 
        k++;
     }
     // Reset the value of the boolean variable to it's initial value
     alreadyExist = false;
  }

  // Play the recorded notes based on the new order of the array elements
  for (int m = 0; m < MAX_NOTES; m++) {
      // make sure the code will skip the empty (0) elements of the notes array
      if ( notes[mainSetOfRandNum[m]] != 0) {
         tone( BUZZER_PIN, notes[mainSetOfRandNum[m]], duration);
         delay(duration);
      }
  }
  delay(duration);    
} 
/**************PART TWO: SHORT ANSWER*********************************************
* Through my observation and experimentation on the LoopyLooper circuit, I realized that 
* the resistor ladder(keyboard), is a series circuit. The resistence is constant for all
* buttons and is the same, since they all possesses same type of resistor. Also, 
* the current is constant, as the buttons are connected to each other through the resistors and
* and share the same flow of current.
* 
* The rate of voltage is
*********************************************************************************/
