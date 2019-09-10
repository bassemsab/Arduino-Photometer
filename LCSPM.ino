//This sketch is meant to support the user controlling a servo motor
//to rotate the diffraction grating of a low cost spectrophotometer with a 1 degree precision
//it has 2 operation modes whereby the first the user can gradually sweep the angle by one degree
//10 degrees or jump immediately to pre-defined locations using the remote control enabled by this sketch
//in the second mode, the user can enter the desired diffraction angle of a 3 digits and then press OK
// if the angle is out of the range of 0-180 degrees the system will give an error message and return
#include "IRremote.h"
#include <LiquidCrystal.h>
#include <Servo.h>
char welcome[17]=" Group 2 ";
//RGB LED INITIALIZATIONS
int redPin = 5; // choose the pin for each of the LEDs
int greenPin =6 ;
int bluePin =7 ;
int R, G, B; // the Red Green and Blue color components
char curcolor[7]="";
//IR RECEIVER INITIALIZATIONS
int receiver = 3;
IRrecv irrecv(receiver); // create instance of 'irrecv'
decode_results results; // create instance of 'decode_results'
char IRCODE='&'; //a char that will hold remote control input
int input=0; // store the value of IR remote numric keys if pressed
//Servo motor INITIALIZATIONS
int servopin =9;

int i=69; //alpha
Servo myservo;
//BUZZER INITIALIZATIONS
int buzpin= 8;
//LCD INITIALIZATIONS
LiquidCrystal lcd(4, 2, 13, 12, 11, 10);
void setup() [
Serial.begin(9600); //start serial monitor
irrecv.enableIRIn(); // Start the IR receiver
//Bootup Beep
pinMode(buzpin,OUTPUT);//Single Beep
delay(250);
pinMode(buzpin,INPUT);
//initialize Servo
myservo.attach(servopin);
// myservo.write(0+10);
delay(500);//servo speed is 0.12 seconds for each 60 degrees , means it needs 360 uS at most to return from 180 to 0 degrees
//Bootup Screen//
lcd.begin(16, 2);
] //end of setup fuction

void loop() [
//welcome screen
lcd.clear();
lcd.setCursor(0, 0);
lcd.print(" LCSPM Project ");
lcd.setCursor(0, 1);
lcd.cursor();
for( int n=0; n < 16; n++)[
lcd.print(welcome[n]);
delay(100);]
displayBlink(3, 500);
lcd.noCursor();
SELECT_MENUE();
] //end of main loop
void SELECT_MENUE()[
irrecv.resume();
point0:
lcd.setCursor(0, 1);
lcd.print ("Press OK to start");
//Waiting for user input
delay(30);

if (irrecv.decode(&results)) //any buttons pressed ?
[
translateIR(&IRCODE); //a function that will change the content of "IRCODE" depending on key pressed
if (IRCODE=='K') [ // is it okay button ?
pinMode(buzpin,OUTPUT);//Single Beep
delay(100);
pinMode(buzpin,INPUT);
irrecv.resume();
SINGLE_COLOR();]
else [irrecv.resume();
goto point0;]
]//results were recived and recorded, action taken
else goto point0;
]
//signle color mode function
void SINGLE_COLOR()[
irrecv.resume();
char IRCODE='&';
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Single Color MODE");
delay(750);
lcd.clear();
lcd.setCursor(0, 0);

lcd.print("Select spectrum");
lcd.setCursor(0, 1);
lcd.print("sweeping method:");
delay (2000);
pointz:
lcd.noBlink();
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("1-Manual Sweep");
lcd.setCursor(0, 1);
lcd.print("2-Key in Angle");
delay(30);
point0:
if (irrecv.decode(&results)) //any buttons pressed ?
[
translateIR(&IRCODE); //a function that will change the content of "IRCODE" depending on key pressed
// is it #1 button ?
if (IRCODE=='1')[
pinMode(buzpin,OUTPUT);//Single Beep
delay(100);
pinMode(buzpin,INPUT);
MANUAL_SWEEP();
] //PROGRAM 1 GATE

else if (IRCODE=='2') [
pinMode(buzpin,OUTPUT);// sound single beep
delay(100);
pinMode(buzpin,INPUT);
SPECIFY_ANGLE();
]// PROGRAM 2 GATE
else if (IRCODE=='*') [
pinMode(buzpin,OUTPUT);//sound single beep
delay(100);
pinMode(buzpin,INPUT);
SELECT_MENUE();
]// Going back to previous screen option
else [
irrecv.resume();
goto point0;]
]//results were recived and recorded, action taken
else goto pointz;
]//SINGLE_COLOR MODE OVER

void MANUAL_SWEEP()[
lcd.noBlink();
myservo.write(i+10); // The grating holder has a 10 degrees Clock wise tilt at intial 0 degrees
//(initially @-10 degrees) so we always add 10 degrees whenever we write the angle to the servo.
settleangle();
delay(1000);
irrecv.resume();
char IRCODE=' ';
point0:
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Difraction Angle:");
// lcd.setCursor(7, 1);
// lcd.print("Deg.");
lcd.setCursor(5, 1);
lcd.write (B11011111);//display degree symbol
lcd.setCursor(1, 1);
lcd.print(i);
lcd.setCursor(7, 1);
lcd.print(";");
lcd.setCursor(9, 1);
lcd.print(curcolor);
RGB_LED_COLOR(i);
delay(200);

if (irrecv.decode(&results)) //any buttons pressed ?
[
translateIR(&IRCODE); //a function that will change the content of "IRCODE" depending on key pressed
if (IRCODE=='F')[
pinMode(buzpin,OUTPUT);//Single Beep
delay(50);
pinMode(buzpin,INPUT);
if (i<=170)[
i=i+10;
Serial.print(i);]
else [
lcd.clear();
lcd.setCursor(0,1);
lcd.print("Max Angle");
delay(500);
]
myservo.write(i+10);
delay(200);
irrecv.resume();
] //PROGRAM 1 GATE

else if (IRCODE=='B') [
pinMode(buzpin,OUTPUT);//Single Beep
delay(50);
pinMode(buzpin,INPUT);
if (i>=10)
i-=10;
else[
lcd.clear();
lcd.setCursor(0,1);
lcd.print ("Min Angle");
delay(50);
]
myservo.write(i+10);
delay(200);
irrecv.resume();
]// PROGRAM 2 GATE
else if (IRCODE=='R') [
pinMode(buzpin,OUTPUT);//single beep for a while
delay(50);
pinMode(buzpin,INPUT);
if (i<=179)
i+=1;
else[
lcd.clear();
lcd.setCursor(0,1);
lcd.print ("Max Angle");
delay(500);
]

myservo.write(i+10);
delay(100);
irrecv.resume();
]// PROGRAM 3 GATE
else if (IRCODE=='L') [
pinMode(buzpin,OUTPUT);//Single Beep
delay(50);
pinMode(buzpin,INPUT);
if (i>=1)
i-=1;
else[
lcd.clear();
lcd.setCursor(0,1);
lcd.print ("Min Angle");
delay(500);
]
myservo.write(i+10);
delay(100);
irrecv.resume();
]// PROGRAM 4 GATE

else if (IRCODE=='*') [
pinMode(buzpin,OUTPUT);//Single Beep
delay(100);
pinMode(buzpin,INPUT);
SINGLE_COLOR();
]// Previous Screen option
//violet
else if (IRCODE=='1') [
pinMode(buzpin,OUTPUT);//Single Beep
i=69;
myservo.write(i+10);
settleangle();
delay(1200);
pinMode(buzpin,INPUT);
irrecv.resume();
]
//Blue
else if (IRCODE=='2') [
pinMode(buzpin,OUTPUT);//Single Beep
i=72;
myservo.write(i+10);
settleangle();
delay(1200);
pinMode(buzpin,INPUT);
irrecv.resume();
]

//Cyan
else if (IRCODE=='3') [
pinMode(buzpin,OUTPUT);//Single Beep
i=75;
myservo.write(i+10);
settleangle();
delay(1200);
pinMode(buzpin,INPUT);
irrecv.resume();
]
//Green
else if (IRCODE=='4') [
pinMode(buzpin,OUTPUT);//Single Beep
i=77;
myservo.write(i+10);
settleangle();
delay(1200);
pinMode(buzpin,INPUT);
irrecv.resume();
]

//yellow
else if (IRCODE=='5') [
pinMode(buzpin,OUTPUT);//Single Beep
i=79;
myservo.write(i+10);
settleangle();
delay(1200);
pinMode(buzpin,INPUT);
irrecv.resume();
]
//orange
else if (IRCODE=='6') [
pinMode(buzpin,OUTPUT);//Single Beep
i=80;
myservo.write(i+10);
settleangle();
delay(1200);
pinMode(buzpin,INPUT);
irrecv.resume();
]

//Red
else if (IRCODE=='7') [
pinMode(buzpin,OUTPUT);//Single Beep
i=82;
myservo.write(i+10);
settleangle();
delay(1200);
pinMode(buzpin,INPUT);
irrecv.resume();
]
else [irrecv.resume();
goto point0;
]
]//results were recived and recorded, action taken
goto point0;
]//end of manual sweep mode
void SPECIFY_ANGLE()[
irrecv.resume();
myservo.write(0+10);
delay(500);
char IRCODE=' ';
int i=100;
lcd.clear();
lcd.setCursor(0, 0);
marquee("Enter Dif. angle then Press OK",0);

point4:
int destination=0;
int ret=0; //decision flag
lcd.noBlink();
point3:
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Key in &press OK");
lcd.setCursor(0, 1);
lcd.print("Angle:");
lcd.setCursor(6, 1);
lcd.print(destination);
lcd.setCursor(9, 1);
lcd.write (B11011111);//display degree symbol
lcd.setCursor(11, 1);
lcd.write (B01111111);//display backspace mark symbol
lcd.setCursor(12, 1);
lcd.print (":(#)");//
delay(200);
HERE:
if (ret==1)[ marquee("Press # to key in angle again",1);
delay(2000);]
if (irrecv.decode(&results))[
translateIR(&IRCODE);
pinMode(buzpin,OUTPUT);//Single Beep

delay(100);
pinMode(buzpin,INPUT);
switch (IRCODE)[
case 'O':
case 'X':
irrecv.resume();
if (ret==1) goto HERE;
goto point3;
break;
case '*':
SINGLE_COLOR();
break;
case '#':
irrecv.resume();
goto point4;
break;
case 'K':
lcd.noBlink();
if (ret==1) goto HERE;
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Rotating...");
displayBlink(3, 500);
//delay(1000);
//lcd.noDisplay();
myservo.write(destination+10);
pinMode(buzpin,OUTPUT);//Single Beep
RGB_LED_COLOR(destination);

delay(500);
pinMode(buzpin,INPUT);
delay(200);
pinMode(buzpin,OUTPUT);//Single Beep
delay(500);
pinMode(buzpin,INPUT);
settleangle();
//lcd.display();
//delay(700);
//lcd.noDisplay();
//delay(700);
//lcd.display();
//delay(700);
irrecv.resume();
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Motor InPosition");
ret=1;
lcd.setCursor(0, 1);
goto HERE;
break;
default:
irrecv.resume();
if (ret==1)
goto HERE;
break;
]//end of swtich

if (ret==1) goto HERE;
// this part will take each digit that the user keys in and shift it so that the final destination will have 3 digits
if (destination==0) [destination=input; irrecv.resume(); goto point3;]
else if (destination>0 && destination<10)[ destination=((destination*10)+input); irrecv.resume(); goto point3;]
else if (destination>=10 && destination<100)[
destination=destination*10+input;
if (destination>=180)[
for(int z=0;z<5;z++)[
pinMode(buzpin,OUTPUT);//Single Beep
RGB_LED_COLOR(127);
delay(500);
RGB_LED_COLOR(80);
pinMode(buzpin,INPUT);
]
RGB_LED_COLOR(myservo.read());
lcd.noBlink();
lcd.clear();
lcd.setCursor(2, 0);
lcd.print("Out of range");
delay(2000);
lcd.setCursor(2, 1);
lcd.print("Key in again");
delay(3000);
goto point4;
]

irrecv.resume();
goto point3;
]
else [ irrecv.resume(); goto point3;]
]// input was recorded and action was taken
if (ret==1) [
//lcd.scrollDisplayLeft();
//delay(350);
goto HERE;
]
if (destination==0) lcd.setCursor(6, 1);
else if (destination>0 && destination<10) lcd.setCursor(7, 1);
else if (destination>=10 && destination<100) lcd.setCursor(8, 1);
lcd.blink();
delay(100);
goto point3; // when there is no input
] //any buttons pressed ?

//IR remote buttons decoding function
void translateIR(char *IRCODE) // takes action based on IR code received
[
switch(results.value)
[
case 0xFF629D:
case 0x511DBB:
*IRCODE= 'F'; ; break;
case 0x52A3D41F:
case 0xFF22DD:
*IRCODE= 'L'; break;
case 0xD7E84B1B:
case 0xFF02FD:
*IRCODE= 'K'; ; break;
case 0x20FE4DBB:
case 0xFFC23D:
*IRCODE= 'R'; break;
case 0xA3C8EDDB:
case 0xFFA857:
*IRCODE= 'B'; break;
case 0xC101E57B:
case 0xFF6897:
input=1;
*IRCODE= '1'; break;

case 0x97483BFB:
case 0xFF9867:
input=2 ;
*IRCODE= '2'; break;
case 0xF0C41643:
case 0xFFB04F:
input=3;
*IRCODE= '3'; ; break;
case 0x9716BE3F:
case 0xFF30CF:
input=4;
*IRCODE= '4'; break;
case 0x3D9AE3F7:
case 0xFF18E7:
input=5;
*IRCODE= '5'; break;
case 0x6182021B:
case 0xFF7A85:
input=6;
*IRCODE= '6'; break;
case 0x8C22657B:
case 0xFF10EF:
input=7;
*IRCODE= '7'; break;

case 0x488F3CBB:
case 0xFF38C7:
input=8;
*IRCODE= '8'; break;
case 0x449E79F:
case 0xFF5AA5:
input=9;
*IRCODE= '9'; break;
case 0x32C6FDF7:
case 0xFF42BD:
*IRCODE= '*'; break;
case 0x1BC0157B:
case 0xFF4AB5:
input=0;
*IRCODE= '0'; break;
case 0x3EC3FC1B:
case 0xFF52AD:
*IRCODE= '#'; break;
case 0xFFFFFFFF:
*IRCODE= 'X'; break;
default:
*IRCODE= 'O';
]// End Case

delay(100); // Do not get immediate repeat
] // end of translateIR
void lcd_scroll(char disp[],int row)[
int length = strlen(disp);
if (length <= 16)
return;
int pos=1;
while(pos <= length - 16)
[
lcd.setCursor(0,row);
for( int i=0; i < 16; i++)
lcd.print(disp[pos+i]);
delay(225);
pos = pos + 1;
]
]
//function that will determine the current color and setup the color of the RGB LEDs
void RGB_LED_COLOR(int angle)[

// Violet 380-450nm
if (angle>=67 && angle<70)
[
strcpy(curcolor, "Violet");
R = 127;
G = 0;
B = 255;
Serial.println("Violet");
]
// Blue 450-495
else if(angle>=70 && angle<73)[
strcpy(curcolor, "Blue");
R = 0;
G = 0;
B = 255;
Serial.println("Blue");
]
// Azure 488nm
else if (angle>=73 && angle<74)[
strcpy(curcolor, "Azure");
R = 0;
G = 127;
B = 255;
Serial.println("Azure");
]

// cyan 490 -520
else if (angle>=74 && angle<76)[
strcpy(curcolor, "Cyan");
R = 0;
G = 255;
B = 255;
Serial.println("Cyan");
]
// green 495 -570
else if (angle>=76 && angle<78)[
strcpy(curcolor, "Green");
R = 0;
G = 255;
B = 0;
Serial.println("Green");
]
// Yellow 570 - 590
else if (angle>=78 && angle<80)[
strcpy(curcolor, "Yellow");
R = 255;
G = 255;
B = 0;
Serial.println("Yellow");
]

// Orange 590-620
else if(angle>=80 && angle<82) [
strcpy(curcolor, "Orange");
R = 255;
G = 127;
B = 0;
Serial.println("Orange");
]
// RED 620-740
else if(angle>=82 && angle<85) [
strcpy(curcolor, "Red");
R = 255;
G = 0;
B = 0;
Serial.println("RED");
]
else [
R = 0;
G = 0;
B = 0;
Serial.println("Out of VISIBLE SPECTRUM");
strcpy(curcolor, "NOT VIS");
]

// write the RGB values to the pins
Serial.println("PROJECTING ON LED NOW");
Serial.println(R);
Serial.println(G);
Serial.println(B);
analogWrite(redPin,R);
analogWrite(greenPin,G);
analogWrite(bluePin,B);
]
//function to ad a blinking effect to the screen
void displayBlink(int blinks, int duration)
[
while(blinks--)
[
lcd.noDisplay();
delay(duration);
lcd.display();
delay(duration);
]
]
//function to sweep the LCD text to the left when the size of the string is greater than 16
void marquee( char *text,int l)
[
int length = strlen(text); // the number of characters in the text
if(length < 16)
lcd.print(text);
else
[
int pos;
for( pos = 0; pos < 16; pos++)
lcd.print(text[pos]);

delay(1000); // allow time to read the first line before scrolling
pos=1;
while(pos <= length - 16)
[
lcd.setCursor(0,l);
for( int f=0; f < 16; f++)
lcd.print(text[pos+f]);
delay(200);
pos = pos + 1;
]
]
]
//function to improve the precision of servo, step up and then step down 5 degrees:
void settleangle()[
for (int t=0;t<5;t++)[
i++;
myservo.write(i+10);
delay(100);
]
for (int t=0;t<5;t++)[
i--;
myservo.write(i+10);
delay(100);
]
]