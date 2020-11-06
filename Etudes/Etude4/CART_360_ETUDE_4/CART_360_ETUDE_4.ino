/********************** ETUDE 4 CART 360 2020 *******************************
   WELCOME! IN THIS ETUDE WE WILL USE TIMERS & COMPUTE A RUNNING AVERAGE FOR PHOTOCELL
   THE PURPOSE OF THIS EXERCISE IS EXTEND ETUDE_3 WITH THE FOLLOWING FUNCTIONALITY:
   1/ WE WILL CHANGE THE RECORD MODE SUCH THAT IT NOT ONLY RECORDS THE NOTE BUT THE DURATION THAT NOTE WAS PRESSED AS WELL
   WE WILL ALLOW A USER TO HOLD DOWN A BUTTON FOR A MAXIMUM OF 5 SECONDS
   2/ WE WILL ADD A PHOTOCELL TO INCORPORATE FREQUENCY MODULATION - WHEN YOU PRESS A BUTTON AND AFFECT THE PHOTOCELL,
   THE FREQUENCY WILL CHANGE
   3/ IN RECORD MODE -YOU WILL PLAY THE NOTE LIVE WITH THE LIVE VALUE OF THE PHOTOCELL, IN PLAYBACK YOU WILL USE THE AVERAGE
   PHOTOCELL VALUE AS AN OFFSET TO THE FREQUENCY
   4/ WE WILL HAVE A NEW PLAYMODE WHICH WILL PLAYBACK THE FREQUENCY+AVERAGE PHOTO VAL AND THE DURATION ASSOCIATED WITH THE NOTE
   5/ ADDITIONALLY, IN PLAYBACK, THE LED BRIGHTNESS & COLOUR WILL CHANGE ACCORDING TO THE NOTE VALUE
   6/ RE-IMPLEMENT THE LIVE MODE TO INCORPORATE THE PHOTOCELL VALUE
   PLEASE NOTE:: THE OTHER NEW FUNCTIONS(BESIDES 6/) TO IMPLEMENT ARE AT THE BOTTOM OF THE CODE...

   PLEASE FOLLOW THE INSTRUCTIONS IN THE COMMENTS:
   DO NOT ADD ANY MORE FUNCTION DEFINITIONS
   ONLY IMPLEMENT THE FUNCTION DEFINITIONS SUPPLIED
   THERE IS NO NEED TO ADD ANY NEW VARIABLES OR CONSTANTS
   PROVIDE COMMENTS FOR ANY OF THE CODE IMPLEMENTED
   GOOD LUCK!
*/
/**** CONSTANTS ********************************************************/

#define BUTTON_MODE_PIN 2 // Button to change the mode

// constants for RGB LED
#define LED_PIN_R 9 // R PIN
#define LED_PIN_G 10 // G PIN
#define LED_PIN_B 11 // B PIN

// constant for note in (button-resistor ladder on breadboard)
# define NOTE_IN_PIN A0

//constant for max number of notes in array
#define MAX_NOTES 16

// a constant for duration
const int duration = 200;

// constant for pin to output for buzzer
#define BUZZER_PIN 3 // PWM

// constant for input from photo cell
#define PHOTO_PIN A1

// constant for size of running samples array (for averging)
#define RUNNING_SAMPLES 16

// max pressing time
const int MAX_PRESS_TIME = 4096; //2^12 = 4096

/******** VARIABLES *****************************************************/
// counter for how many notes we have
int countNotes = 0;
// array to hold the notes played (for record/play mode)
int notes [MAX_NOTES];
int mode = 0; // start at off

/******** VARIABLES FOR ETUDE 4 *****************************************************/
// NEW FOR ETUDE 4: array to hold durations
// will be a parallel array to the notes array -
// meaning that at index 0 of notes[] is stored the 1st frequency, and at index 0 of
// the durations array  is stored the 1st duration for THAT note....
int durations [MAX_NOTES];

// boolean to determine if a note button was being pressed:
boolean activeNoteButton = false;

// keep track of the active frequency
int activeFrequency = 0;

// keep track of offset Freq generated by the photocell
int offsetFrequency = 0;

// keep track of average Freq generated by the photocell
int averageOffsetFreq = 0;

// timer vars
unsigned long timePassed;
unsigned long startTime;

// var to hold the note pressed
int testNote;

// vars for averageing
int runningAverageBuffer[RUNNING_SAMPLES];
int nextCount = 0;

/************Photocell Frequency + Note Frequency********************************************/
// Var to hold the combined values of the photo cell and the note frequency
int combinedValues;
// Var to hold the photo cell value 
int photocell_Value;

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
  /* DEBUG THE STATES */
  Serial.begin(57600);
  while (!Serial);
  pinMode(BUTTON_MODE_PIN, INPUT); // Receive input from the button
  // Display the output through the RGB LED and Piezo Buzzer
  pinMode(LED_PIN_R, OUTPUT);
  pinMode(LED_PIN_G, OUTPUT);
  pinMode(LED_PIN_B, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  // Photocell Input
  pinMode(PHOTO_PIN, INPUT);

}

/**********************LOOP() DO NOT CHANGE *******************************
   INSTRUCTIONS:
   There is NO NEED to change the loop - it establishes the flow of the program
   We call here 3 functions repeatedly:
   1: chooseMode(): this function will determine the mode that your program is in
   based on if the button (linked to the BUTTON_MODE_PIN) was pressed
   2: setRGB(): will set the color of the RGB LED based on the value of the mode variable
   3: selectMode(): will determine which function to call based on the value of the mode variable

**************************************************************************/
void loop()
{
  chooseMode();
  setRGB(mode);
  selectMode();
}
/******************CHOOSEMODE() *********************************
   INSTRUCTIONS:
   Read the value from the Button (linked to the BUTTON_MODE_PIN) and
   if is being pressed then change the mode variable.
   REMEMBER:
   mode == 0 is reset
   mode == 1 is live
   mode == 2 is record
   mode == 3 is play
   mode == 4 is loopMode
   Every time the user presses the button, the program will go to the next mode,
   once it reaches 4, it should go back to mode == 0.
   (i.e. if mode ==2 and we press, then mode ==3) ...
**************************************************************************/
void chooseMode() {
  if (digitalRead(BUTTON_MODE_PIN) == HIGH)
  {
    delay(1000);
    if (mode < 4) mode++;
    else mode = 0;
  }
}

/******************SETRGB()*********************************************
   INSTRUCTIONS:
   Depending on the value of the mode variable:
   if the mode is 0 - RGB LED IS OFF
   if the mode is 1 - RGB LED IS BLUE
   if mode is 2 - RGB LED IS RED
   if mode is 3 - RGB LED IS GREEN
   if mode is 4 - RGB LED iS PURPLE
   YOU MUST USE A SWITCH CASE CONSTRUCT (NOT A SERIES OF IF / ELSE STATEMENTS
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
      digitalWrite(LED_PIN_R, 152);
      digitalWrite(LED_PIN_G, 51);
      digitalWrite(LED_PIN_B, 153);
      break;
  }
}
/**********************SELECTMODE() *******************************
   INSTRUCTIONS:
   There is NO NEED to change this function - it selects WHICH function should run based on the mode variable
   There are 4 possibilities
   1: reset(): this function will reset any arrays etc, and will do nothing else
   2: live(): this function will play the corresponding notes
   to the user pressing the respective buttons.
   NOTE:: the notes played are NOT stored
   3: record(): this function will play the corresponding notes
   to the user pressing the respective buttons
   AND will STORE up to 16 consecutive notes in an array.
   4: play(): this function will playback any notes stored in the array that were recorded
   in the previous mode
   5: loopMode(): this function will playback any notes stored in the array that were recorded,
   BUT unlike the previous mode, you can choose in which sequence the notes are played.
   REQUIRED: only play notes from the array (no live stuff)

******************************************************************************/
void selectMode()
{
  if (mode == 0) {
    reset();
  }

  else if (mode == 1) {
    live();
  }

  else if (mode == 2) {
    recordWithDuration();
  }

  else if (mode == 3) {
    playWithDuration();
  }

  else if (mode == 4) {
    loopWithDuration();
  }
}
/******************RESET() **************************************
   INSTRUCTIONS:
   this function should ensure that any notes recorded are no longer available
**************************************************************************/
void reset()
{
  // To reset the notes, duration , and array,
  // assign zero to all of the array's elements respectively
  for (int i = 0; i < MAX_NOTES; i++) {
    notes[i] = 0;
    durations[i] = 0;
    runningAverageBuffer[i] = 0;
    // Also resets all the variables which stores a certain value
    countNotes = 0;
    nextCount = 0;
    mode = 0;
    activeNoteButton = false;
    activeFrequency = 0;
    offsetFrequency = 0;
    averageOffsetFreq = 0;
  }
}

/******************LIVE()  - RE-IMPLEMENT **************************************
   INSTRUCTIONS:
   this function will play the corresponding notes BUT NO TIMING INFO
   to the user pressing the respective buttons
   NOTE:: the notes played are NOT stored
   SO: you need read in the input from the analog input (linked to the button-resistor ladder combo)
   ALSO - get the value of the photo cell and combine it with the note frequency
   THEN - output the note to the buzzer using the tone() function, as 3rd argument
   make the duration value == to 100
**************************************************************************/
void live()
{ 
  // IMPLEMENT live()
  // Get and store the photo cell value
  offsetFrequency = getPhotoFrequency();
  // Get and store the note frequency
  int noteValue = analogRead(NOTE_IN_PIN);
  // Combine the two
  combinedValues = noteValue + offsetFrequency;

  // Output sound
  tone( BUZZER_PIN, combinedValues, duration / 2);
}

/******************RECORDWITHDURATION(): IMPLEMENT *********************************
   INSTRUCTIONS:
   this function will play the corresponding notes
   to the user pressing the respective buttons
   AND will STORE up to 16 consecutive notes in an array.
   It will ALSO use a timer to keep track of how long the current note button has been pressed
   It will ALSO sample the photocell value to be used live as well as well as calling the function to calculate
   the running average of the photocell value
   SO:I have implemented a basic framework for you to follow - the basic state machine.
   YOUR JOB is to implement the NEW functions that get called within the framework...
**************************************************************************/
void recordWithDuration()
{
  // to handle a little bit of noise
  int theAdjuster = 2; // Scale 2^2, 2^3, 2^4, 2^5 etc (Multiple of 2s)
  // check we have not stored more than 16 notes
  if (countNotes < MAX_NOTES)
  {
    // read in the value from button press
    testNote = analogRead(NOTE_IN_PIN);
    /*** STATE A::: *******************************************
        IF the testNote is > theAdjuster AND the timer has not reached 5 secs:
        This means we are pressing on a note button
    */
    if (testNote > theAdjuster && (timePassed < MAX_PRESS_TIME)) {
      /*** STATE AA::: *******************************************
          IF the boolean is false it means we have just started to press the button
          YOU now need to implement the function below which will set up all things
          required to have a new timer
      */
      if (activeNoteButton == false)
      {
        /*** FUNCTION TO IMPLEMENT ***/
        startUpTimer();
        // Set the boolean value to true to run the code written for the time the button is kept pressed for 5sec
        activeNoteButton = true;
      }
      /*** STATE AA::: *******************************************
           IF the boolean is true it means we are continuing to press the button
           YOU now need to implement the function below which will allow for
           1/ timer to be updated
           2/ get the photoCell Value
           3/ get the current running average
      */
      else
      {
        // update the timer
        updateTimer();
        // Get the photo cell value
        offsetFrequency = getPhotoFrequency();
        // Play the note that is currently being played
        playCurrentNote();
        // Get the average of photocell value to be used as an offset to the frequency
        averageOffsetFreq = getRunningAverage();
      }
    }
    /*** STATE B::: *******************************************
       IF the testNote is > theAdjuster AND the timer HAS reached 5 secs:
       then WRITE the code to turn OFF the buzzer.
    */
    else if (testNote > theAdjuster && (timePassed > MAX_PRESS_TIME))
    {
      // WRITE the code to turn OFF the buzzer
      noTone(BUZZER_PIN);
    }
    /*** STATE C::: *******************************************
       IF the testNote is < theAdjuster AND the boolean is true
       this means we have STOPPED pressing the button
       YOU: need to implement the function to update the arrays etc
    */
    else if ((testNote <= theAdjuster && activeNoteButton == true) ) {
      // Update the notes and durations arrays
      updateArraysWithNoteAndTimings();
    }
  }
}

/******************STARTUPTIMER(): IMPLEMENT *********************************
   INSTRUCTIONS:
   this function will set all required variables to reset the timer
**************************************************************************/
void startUpTimer()
{
  startTime = 1;
  timePassed = 0;
}

/******************UPDATETIMER(): IMPLEMENT *********************************
   INSTRUCTIONS:
   this function will update the current timer
**************************************************************************/
void updateTimer()
{
  timePassed += startTime;
  // Store the duration of the note being currently played in this array
  durations[countNotes] = timePassed;
}

/******************PLAYCURRENTNOTE(): IMPLEMENT *********************************
   INSTRUCTIONS:
   this function will play the current note being pressed in RecordDuration mode
   and the frequency should be note value + offset from photocell
   add as the 3rd parameter to the tone() a duration of _DURATION_OF_NOTE_
   tone( BUZZER_PIN , _FREQ_+_OFFSET , _DURATION_OF_NOTE_ );
   delay( _DURATION_OF_NOTE_ );
**************************************************************************/
void playCurrentNote()
{
  // Store the note value's to be used later in the playback function
  notes[countNotes] = testNote ;
  tone( BUZZER_PIN, notes[countNotes] + offsetFrequency, durations[countNotes]);
  delay(durations[countNotes]);
}
/******************UPDATEARRAYSWITHNOTEANDTIMINGS(): IMPLEMENT *********************************
   INSTRUCTIONS:
   this function will handle the case where the note is no longer being pressed:
   you need to update the arrays
   NOTE: please for the note value  - combine with the running average val from the photocell
   reset the necessary booleans
   reset any counters
**************************************************************************/
void updateArraysWithNoteAndTimings()
{
  countNotes += 1;
  activeNoteButton = false;
  timePassed = 0;
}

/******************GETPHOTOFREQUENCY(): IMPLEMENT *********************************
   INSTRUCTIONS:
   this function will get input from the photocell
   please manipulate to your liking and return the value accordingly
**************************************************************************/
int getPhotoFrequency()
{
  photocell_Value =  analogRead(PHOTO_PIN);
  return photocell_Value;
}

/******************GETRUNNINGAVERAGE(): IMPLEMENT *********************************
   INSTRUCTIONS:
   this function should compute the running average of the photcell value (returned from the
   getPhotoFrequency() function).
**************************************************************************/
int getRunningAverage()
{
// The code used to get the average of the photocell frequencies
//for(int i = 0; i< RUNNING_SAMPLES; i++)
//  {
//    runningAverageBuffer[i] = getPhotoFrequency();
//    nextCount+= runningAverageBuffer[i];
//  }
//  int averageOffsetFreq = nextCount/RUNNING_SAMPLES;
//  return averageOffsetFreq;
//  delay(100);
}
/******************COLORLED(): IMPLEMENT *********************************
   INSTRUCTIONS:
   this function will change the color of the led (in playback mode) to have the brightness
   adjusted according to the frequency of the note being played (range : 0-255)
**************************************************************************/
void colorLED(int col)
{
  //IMPLEMENT
}

/******************PLAYWITHDURATION(): IMPLEMENT *********************************
   INSTRUCTIONS:
   As in Etude 2 - This is the true play back loop of the original recording
   this function will got through the notes array and play back the note and
   its corresponding duration value.
   NOTE: we have not implemented any case/code for pauses  - this can be a fixed value
   ALSO: call the function to change the color of the LED using the correct argument.
   ALSO: note that the mode button will not be super responsive UNLESS you press the button
   continuously - that is ok for this etude.
**************************************************************************/
void playWithDuration()
{
  // Play the notes stored in the notes array
  for (int j = 0; j < MAX_NOTES; j++){
    // Duration is multiplied by 5 to make the noise recognizable.
     tone( BUZZER_PIN, notes[j], durations[j]*5);
     delay(durations[j]*5);
  }
  delay(duration*2);
  // If the button is pressed, turn off the buzzer
  if (digitalRead(BUTTON_MODE_PIN) == HIGH) {
     noTone(BUZZER_PIN);
  }
}
/******************LOOPWITHDURATION(): IMPLEMENT *********************************
   INSTRUCTIONS:
   As in Etude 2 - This is the custom (variant) play back loop of the original recording
   the function will got through the notes array and play back the note and
   its corresponding duration value.
   NOTE: we have not implemented any case/code for pauses - this can be a fixed value
   ALSO: call the function to change the color of the LED using the correct argument.
   ALSO: note that the mode button will not be super responsive UNLESS you press the button
   continuously - that is ok for this etude.
**************************************************************************/
void loopWithDuration()
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
      tone( BUZZER_PIN, notes[mainSetOfRandNum[m]], durations[m]*5);
      delay(durations[m]*5);
  }
  delay(duration);    
}
