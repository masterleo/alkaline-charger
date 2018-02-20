/*

 .'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''.
 .                                                                   .
 .       ________________ _________                      __          .
 .      /  _  \__    ___//   _____/ _____ _____ ________/  |_        .
 .     /  /_\  \|    |   \_____  \ /     \\__  \\_  __ \   __\       .
 .    /    |    \    |   /        \  Y Y  \/ __ \|  | \/|  |         .
 .    \____|__  /____|  /_______  /__|_|  (____  /__|   |__|         .
 .       \/               \/      \/     \/                          .
 .            __                                                     .
 .      ____ |  |__ _____ _______  ____   ___________                .
 .     / ___\|  |  \\__  \\_  __ \/ ___\_/ __ \_  __ \               .
 .    \  \___|   Y  \/ __ \|  | \/ /_/  >  ___/|  | \/               .
 .     \___  >___|  (____  /__|  \___  / \___  >__|                  .
 .         \/     \/     \/     /_____/      \/                      .
 .                                                                   .
 ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
 
 // This asci art is made with => http://patorjk.com/software/taag/
 
 DO NOT REMOVE THIS COPYRIGHT TERMS: 
 This example code is liscenced under Creative Commons BY SA NC. 
 Learn more about this liscence here :
 https://creativecommons.org/licenses/by-nc-sa/3.0/
 
 // Modification History Beggin
 Created 25 Oct 2014
 by Leo Etchaas
 
 Modified TODAY?
 by YOU?
 
 Modified TODAY?
 by YOUR_SELF?
 /// Modification History End
 
 
 
 TO_DO : ----------------------------------------------------------------------
 -) Add USB serial support .
 -> more about usb and ATTiny85 here : http://vusb.wikidot.com/hardware
 
 -) Program a Linux and Mac app to check the charge voltage and more
 (does not support windobe.) 
 
 -) Sound feedback with buzzer and fancy 8 bit sounds.
 ->mabe with that chip  ? :https://www.adafruit.com/product/2400
 
 -) Self insuling when panic mode triggle
 ,self insulation from evrything connected to :battery PW supply
 
 /TO_DO -----------------------------------------------------------------------
 
 More infos : 
 Neopixels are 0.18W each and that makes 18 lumens so : 
 1 meter @60LED is 1000 lumen Bright
 Question for you: How bright are those 5 leds ? 
 
 One day I had big problems, if your compiler don't compile this but compile
 smaller programs for ATTINY85 try to carfuly folow the steps  in this README 
 here :
 https://github.com/TCWORLD/ATTinyCore/tree/master/PCREL%20Patch%20for%20GCC
 
 What inspired me to create this :
 (to read if you are curious and mabe even bored:)
 http://fritzing.org/projects/smart-nimh-battery-power-charger-with-attiny85
 http://goo.gl/IJ1TcZ // if dead link try this one =>  http://bit.ly/1w20AAI
 
 **/


/* This represent how to connect the wires to witch pins **/

#include <SendOnlySoftwareSerial.h> // Definitions
SendOnlySoftwareSerial mySerial (4);  // Tx pin DigitalIO 3 is 
//phisical pin 3 near to GND that is 4

// NEOPIXELS START
// Leds are used with the LEDs to display the program states and stuff
// We use neopixel code :
// released under the GPLv3 license to match the rest of 
// the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            2

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS     5

// When we setup the NeoPixel library, we tell it how many pixels,
//and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the 
//third parameter--
//see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
//NEOPIXELS END INCLude


// Analog input pin that the potentiometer is attached to
// Usefull to read the voltage
const int analogInPin = A3 ;  

//Pulse Width Modulation that the battery(s) are atached to
//Usefull to provide power to charge the battery(s)
const int 
pwmOuPinBuzzer = 1,
pwmOutPinBatterie = 0 ;


/* \this is the end of the section "how to connect the wires to witch pins" **/


/* all the variables that I use for my program not for the pins connections are below **/
//for the leds :
int redValue = 0, // value to write to the red LED
greenValue = 0, // value to write to the green LED
blueValue = 0, // value to write to the blue LED 
PWM_OutPutValueBuzzer = 0, //TMP TEST
PWM_OutputValue = 0;  // value output to the PWM (analog out)
//200 is good for chargin the alkaline battery

int
programState = 0;

float 
voltage  = 0 ;
/* all the variables that I use for my program not for the pins connections **/



/********************************* setup !! : ) ********************************/
void setup() {

  // This is for Trinket 5V 16MHz, 
  //you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code
  pixels.begin(); // This initializes the NeoPixel library.


  pinMode(pwmOutPinBatterie, OUTPUT);
  pinMode(pwmOuPinBuzzer, OUTPUT);


  pinMode(4, OUTPUT);
  mySerial.begin(115200); // setup the monitor talk speed

  for(int j =0; j<2;j+=1){
    //data to light up  the leds :
    redValue = 255; // value to write to the red LED
    greenValue = 255; // value to write to the green LED
    blueValue = 255; // value to write to the blue LED

    // Try out all the leds :    
    for(int i=0;i<NUMPIXELS;i++){

      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(redValue,greenValue,blueValue));
      // Moderately bright green color.
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(150); // Delay for a period of time (in milliseconds).
    }
    //data to light up  the leds :
    redValue = 0; // value to write to the red LED
    greenValue = 0; // value to write to the green LED
    blueValue = 0; // value to write to the blue LED

    for(int i=0;i<NUMPIXELS;i++){
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(redValue,greenValue,blueValue));
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(150); // Delay for a period of time (in milliseconds).
    }
  }
}  /********************************* \ setup !! : ) *****************************************/











void loop() {/************************** loop begin ****************************/

    if ( ! (millis() % 10)){
      
        analogWrite(pwmOutPinBatterie,0  );  // chargin with PWM duty  cycle
voltage = readInputBattery(analogInPin);
    }
  /****************** DETERMINE STATE   ***********************/
  if ( voltage < (0.1) ) { // mabe ground (0v)? so absolutly no power, means no battery
    programState = 1;
  }
  else {
    if (voltage < 0.40 ){  // the thing inserted is very weak ! 
      programState = 2;
    }
    else  {
      if (voltage < 1.60 ){  // the thing inserted is around 1/4
        programState = 3;
      } 
      else {
        if (voltage < 2.40 ){  // the thing inserted is around 2/4
          programState = 4;
        }
        else {
          if (voltage < 3.2){  // the thing inserted is around 3/4
            programState = 5;
          }
          else {
            if ( voltage < 3.4){  // the thing inserted is around 4/4
              programState = 6;
            }
            else {
              if (voltage < 666 ){ // At this point the ATTINY sould be 
              //melted/burned or what ever 
                // so why sould i care about the above value ?
                programState = 7;
              }
              else{
                programState = 0; 
              }
            }
          }
        } 
      }
    }
  }

  /****************** /DETERMINE STATE  ***********************/
  /******************  PROCESS ALL THAT SHIT  ***********************/
  if(  programState == 0 ){  // panic
    pixels.setPixelColor(1, pixels.Color(155,1,155));
    pixels.setPixelColor(2, pixels.Color(155,155,1));
    pixels.setPixelColor(3, pixels.Color(155,1,155));
    pixels.setPixelColor(4, pixels.Color(155,155,1));

    PWM_OutputValue = 0; // the battery  charging power   
  } 
  else{
    if ( programState == 1 ) { // wating battery
      pixels.setPixelColor(1, pixels.Color(1,1,10));
      pixels.setPixelColor(2, pixels.Color(1,1,10));
      pixels.setPixelColor(3, pixels.Color(1,1,10));
      pixels.setPixelColor(4, pixels.Color(1,1,10));

      PWM_OutputValue = 0; // the battery  charging power   

    }
    else {
      if (  programState == 2 ){      // state 2: battery is dead                                 

       // heartbeat(0,0,0,255);
        pixels.setPixelColor(1, pixels.Color(5,0,0));
        pixels.setPixelColor(2, pixels.Color(5,0,0));
        pixels.setPixelColor(3, pixels.Color(5,0,0));
        pixels.setPixelColor(4, pixels.Color(100,0,0));

        PWM_OutputValue = 20; // the battery  charging power   
      } 
      else  {
        if ( programState == 3 ){       // state 3: batterry is less  30% full  

          //heartbeat(0,0,0,255);
          pixels.setPixelColor(1, pixels.Color(5,0,0));
          pixels.setPixelColor(2, pixels.Color(5,0,0));
          pixels.setPixelColor(3, pixels.Color(5,0,0));
          pixels.setPixelColor(4, pixels.Color(0,250,0));
          PWM_OutputValue = 200; // the battery  charging power   
        } 
        else {                 
          if ( programState == 4){             // state 4 : battery is less  60% full  
           // heartbeat(0,0,0,255);
            pixels.setPixelColor(1, pixels.Color(0,0,0));
            pixels.setPixelColor(2, pixels.Color(0,0,0));
            pixels.setPixelColor(3, pixels.Color(0,250,0));
            pixels.setPixelColor(4, pixels.Color(0,250,0));

            PWM_OutputValue = 200; // the battery  charging power   
          }
          else {
            if ( programState == 5 ){     // state 5: battery is less 100% full    
            //  heartbeat(0,0,0,255);
              pixels.setPixelColor(1, pixels.Color(0,0,0));
              pixels.setPixelColor(2, pixels.Color(0,250,0));
              pixels.setPixelColor(3, pixels.Color(0,250,0));
              pixels.setPixelColor(4, pixels.Color(0,250,0));
              PWM_OutputValue = 180; // the battery  charging power   

            }
            else {
              if ( programState == 6 ){     // state 5: battery is less 100% full    
               // heartbeat(0,0,0,255);
                pixels.setPixelColor(1, pixels.Color(0,250,0));
                pixels.setPixelColor(2, pixels.Color(0,250,0));
                pixels.setPixelColor(3, pixels.Color(0,250,0));
                pixels.setPixelColor(4, pixels.Color(0,250,0));
                PWM_OutputValue = 1; // the battery  charging power   

              }
              else {  
                if ( programState == 7 ){ 
                  //heartbeat(0,0,0,255);
                  pixels.setPixelColor(1, pixels.Color(255,0,0));
                  pixels.setPixelColor(2, pixels.Color(255,0,0));
                  pixels.setPixelColor(3, pixels.Color(255,0,0));
                  pixels.setPixelColor(4, pixels.Color(255,0,0));
                  PWM_OutPutValueBuzzer = 50;
                  PWM_OutputValue = 0; // the battery  charging power   

                }
              }
            }
          } 
        }
      }
    }
  }
  
  /****************** \ PROCESS ALL THAT SHIT  ***********************/


  /****************** WRITE  THE OUTPUTS   ***********************/
heartbeat(0,255,255,255);
  pixels.show(); // This sends the updated pixel color to the hardware.
  analogWrite(pwmOutPinBatterie,PWM_OutputValue);  // chargin with PWM duty  cycle
  analogWrite(pwmOuPinBuzzer,PWM_OutPutValueBuzzer);  // chargin with PWM duty  cycle

  //if( ! (millis() % 1000 )) {

  // state 0: Panic !                                           
  // ( ORANGE LED BLINK The user should call Leo etcheverry if this case is possible )
  // state 1: no battery is detected                            
  // ( GREEN LED FADE please insert one )
  // state 2: battery is dead                                  
  //  ( RED LED BLINK ? or FADE put another one in the charger )
  // state 3: batterry is less 30% full                       
  //( 1 green led, 1 green led blinking)
  // state 4: battery is less 70% full                        
  //( 1 green led, 1 green led blinking)
  // state 5: battery is less 100% full                               
  //( GREEN LED keep charge mode is activated take it and enjoy  ) 
  // state 6: battery is 100% full                               
  //( GREEN LED keep charge mode is activated take it and enjoy  )
  // state 7: Battery is  OVER POWER (more 100%  full  )                  
  //( RED LED FAST BLINK  charger may be damaged. Disconnect the battery )

  /**
   * mySerial.println("\n=----------------------------------="); 
   * // display a separation line
   * mySerial.println("\n millis    ");
   * // display the data in your computer !!! 
   * mySerial.println(millis(), DEC); 
   * // display the data in your computer !!! 
   * mySerial.println("\n programState    "); 
   * // display the data in your computer !!! 
   * mySerial.println(programState); 
   * // display the data in your computer !!! 
   * mySerial.println("\n Smooth volatage, {float voltage}   "); 
   * // display the data in your computer !!! 
   * mySerial.println(voltage);
   * // display the data in your computer !!! 
   * mySerial.println("\n Actualvoltage, {float sensorValue}    "); 
   * // display the data in your computer !!! 
   * mySerial.println(sensorValue); 
   * // display the data in your computer !!! 
   * 
   * mySerial.println("RED value :   "); 
   * // display the data in your computer !!! 
   * mySerial.println(redValue); 
   * // display the data in your computer !!! 
   * mySerial.println("GreenValue ");
   * // display the data in your computer !!! 
   * mySerial.println(greenValue);
   * // display the data in your computer !!! 
   * mySerial.println("\n Actualvoltage, {float sensorValue}    ");
   * // display the data in your computer !!! 
   * mySerial.println(sensorValue); 
   * // display the data in your computer !!! 
   * mySerial.println("\n Actualvoltage, {float sensorValue}    "); 
   * // display the data in your computer !!! 
   * mySerial.println(sensorValue);
   * // display the data in your computer !!! 
   * mySerial.println("\n=----------------------------------=");
   * // display a separation line
   ***/
  // Udate the led values 

  // }
  /****************** \ WRITE THE OUTPUTS  ***********************/

}

/************************** \ loop (end) ************************************/


/****************** READ THE INPUTS   ***********************/
/* RTI **/// this reads and calculate an average voltage of the batterry 
//every 500 ms 
unsigned long // time is always unsigned long
RTI_WakeTime; 
float 
RTI_voltage =0,       // value read from the analog pin
LastTenVolagesValues[20];
uint8_t RTI_AVERAGE_SIZE = 10;
float readInputBattery(uint8_t ANALOG_PIN){
    RTI_voltage = analogRead(ANALOG_PIN) / 1023.0 * 5.0  ; 
    LastTenVolagesValues[RTI_AVERAGE_SIZE] = RTI_voltage ; 
    RTI_voltage =0;
    for (int i=0; i<RTI_AVERAGE_SIZE; i+=1 ){  
      LastTenVolagesValues[i]=LastTenVolagesValues[i+1];
      RTI_voltage += LastTenVolagesValues[i];
    }
   return  RTI_voltage / RTI_AVERAGE_SIZE ;
  }

/****************** \ READ THE INPUTS  ***********************/


/******************  HB  ***********************/

/* HB **/// this provides a heartbeat on a pin ,
// so you can tell the software is running for example.
// "uint8_t" type is the same as unsigned char wich holds a 0-255 value ,
//we just use this inted of char to clearly show it is holding a number 
//and not a character 
uint8_t hbVal=128;
int8_t hbDelta=8;
unsigned long hbWakeTime;
uint8_t heartbeat(uint8_t LED_PIN,uint8_t redValue,uint8_t greenValue,uint8_t blueValue) {
  if(! hbWakeTime){                               // License Creative Commons BY SA NC.
    hbWakeTime = millis() + 30 ;
 if (hbVal > 192) hbDelta = -hbDelta;
  if (hbVal < 32) hbDelta = -hbDelta;
    hbVal += hbDelta;
    if( redValue || greenValue || blueValue)
    pixels.setPixelColor(LED_PIN, pixels.Color
    (redValue/hbVal*10,greenValue/hbVal*10,blueValue/hbVal*10)); 


else return hbVal;
  } 
  else if (hbWakeTime < millis()) {
    hbWakeTime = 0 ;
  }
}
/* /HB **/// License Creative Commons BY SA NC.
/****************** \ HB  ***********************/


/**
 * DO NOT REMOVE THIS COPYRIGHT TERMS: 
 * This code is liscenced under Creative Commons BY SA NC. 
 * Learn more about this liscence here :
 * https://creativecommons.org/licenses/by-nc-sa/3.0/
 * 
 ***/











