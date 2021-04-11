#include <KerbalSimpit.h>
#include <KerbalSimpitMessageTypes.h>
#include <PayloadStructs.h>


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);




#define RXMID 535
#define RYMID 520
#define RZMID  49

#define RXDEAD 25
#define RYDEAD 25
#define RZDEAD 15

#define TXMID 534
#define TYMID 515
#define TZMID 34

#define TXDEAD 25
#define TYDEAD 25
#define TZDEAD 15


KerbalSimpit simpit(Serial);

//analog pins
const int pTHROTTLE = A0 ; //slide pot
const int pTX = A1;       //translation x-axis
const int pTY = A2;       //translation y-axis
const int pTZ = A3;       //translation z-axis
const int pRX = A4;       //rotation x-axis
const int pRY = A5;       //rotation y-axis
const int pRZ = A6;       //rotation z-axis

//digital pins
const int pTB = 3;          //translation joystick button
const int pRB = 4;          //rotation joystick button
const int latchPin = 31;     //ST_CP - green / was 8
const int dataPin = 33;     //DS - yellow / was 11
const int clockPin = 35;    //SH_CP - blue / was 12

const int pLCDUP = 23;
const int pLCDDOWN = 27;
const int pSYNCBT = 25;
const int pSYNCLED = 29;


const int pSAS = 26;        //SAS switch
const int pRCS = 31;        //RCS switch
const int pARMABORT = 10;      //Abort switch (safety switch, active high)
const int pABORTLED = 9;
const int pABORT = 8;
const int pARM = 13;        //Arm switch (safety switch, active high) / was 30
const int pSTAGE = 11;      //Stage button // was 32
const int pSTAGELED = 12;   //Stage button LED/ was 33


const int pCHUTES = 30;     //Chutes button (action group 4)
const int pGEARS = 32;      //Gears button
const int pLIGHTS = 34;     //Lights button
const int pSOLAR = 36;      //Solar button (action group 5)
const int pBRAKES = 38;     //Brakes button
const int pLADDER = 40;     //Ladder button (action group 6)
const int pACTION1 = 42;    //Action Group 1 button
const int pACTION2 = 44;    //Action Group 2 button
const int pACTION3 = 46;    //Action Group 3 button


bool stage_on = false;
bool abort_on = false;


//toggle button states
bool tb_on = true;
bool rb_on = true;

//previous button state
bool tb_prev = false;
bool rb_prev = false;


//input value variables
int throttle_value;
int tx_value;
int ty_value;
int tz_value;
int rx_value;
int ry_value;
int rz_value;

//variables used for fuel guages
byte inputBytes[7];
int vSF, vLF, vOX, vEL, vMP, SF, LF, OX, EL, MP;

//debug variable
bool debug_flag = false;
int debug_screen = 0;

//timing
const int IDLETIMER = 20000;        //if no message received from KSP for more than 20s, go idle (default 2000)
const int CONTROLREFRESH = 20;      //send control packet every 10 ms (default 25)
const int stageLedBlinkTime = 500;  //blink staging LED when armed every 500 ms
const int abortLedBlinkTime = 500;

//variables used in timing
unsigned long deadtime, deadtimeOld, controlTime, controlTimeOld;
unsigned long now;

//variables used in serial communication
boolean Connected = false;
byte id;


// This boolean tracks the desired LED state.
bool state = false;

// A timestamp of the last time we sent an echo packet.
unsigned long lastSent = 0;
// How often to send echo packets (in ms)
unsigned int sendInterval = 50;


void clearLCD ()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
}

void LCDline(int line)
{
  display.setCursor(0, (line - 1) * 12);
}

void writeLCD (char myText[])
{
  display.print(myText);
}

void refreshLCD()
{
  display.display();
}


void stick() {
  int maxval = 32700;
  int TX, TY, TZ, yaw, pitch, roll;
  int rx, ry, rz, tx, ty, tz;
  static int rz_prev = 0;
  float exp_weight = 0.1;

  rotationMessage rotation;

  //rotation joystick button toggles flight control modes
  if (!digitalRead(pRB) && !rb_prev) {
    rb_on = !rb_on;
    rb_prev = true;
  }
  if (digitalRead(pRB) && rb_prev) {
    rb_prev = false;
  }

  rx = analogRead(pRX);
  ry = analogRead(pRY);
  rz = analogRead(pRZ);

  rz_prev = rz * exp_weight + (1 - exp_weight) * rz_prev;

  tx = analogRead(pTX);
  ty = analogRead(pTY);
  tz = analogRead(pTZ);

  //rocket mode
  yaw = map(rx, 0, 1023, -maxval, maxval);
  if (abs(rx - RXMID) < RXDEAD) yaw = 0;

  pitch = map(ry, 0, 1023, -maxval, maxval);
  if (abs(ry - RYMID) < RYDEAD) pitch = 0;

  roll = map(constrain(rz_prev - RZMID + 500, 0, 1023), 0, 1023, -maxval, maxval);
  if (abs(rz - RZMID) < RZDEAD) roll = 0;

  TX = map(tx, 0, 1023, -maxval, maxval);
  if (abs(tx - TXMID) < TXDEAD) TX = 0;

  TY = map(ty, 0, 1023, -maxval, maxval);
  if (abs(ty - TYMID) < TYDEAD) TY = 0;

  //TZ = map(tz, 0, 1023, -maxval, maxval);
  //TZ is shot in my HW
  TZ = 0;
  //if (abs(tz - TZMID) < TZDEAD) TZ = 0;

  translationMessage translation;
  translation.mask = 1 | 2 | 4;
  translation.X = TX;
  translation.Y = TY;
  translation.Z = TZ;
  simpit.send(TRANSLATION_MESSAGE, translation);
  
  rotation.mask = 1 | 2 | 4;
  rotation.pitch = pitch;
  rotation.roll = roll;
  rotation.yaw = yaw;
  simpit.send(ROTATION_MESSAGE, rotation);
}


  void debug() {
    //Debug mode does not communicate with KSP, but simply displays the button states on the LCD.
    //Select analog input using LCDx/y/z. Xyz = Throttle. xYz = Translation. xyZ = Rotation.

    //previous state tracking only used in debug
    bool stage_prev = false;
    bool abort_prev = false;

    if (digitalRead(pLCDUP)) debug_screen++;
    if (digitalRead(pLCDDOWN)) debug_screen--;

    if (debug_screen < 0) debug_screen = 2;
    if (debug_screen > 2) debug_screen = 0;

    //clear the LCD
    clearLCD();

    LCDline(1);
    //toggle switches
    if (!digitalRead(pSAS)) {
      writeLCD("S");
    } else {
      writeLCD("s");
    }
    if (!digitalRead(pRCS)) {
      writeLCD("R");
    } else {
      writeLCD("r");
    }
    if (digitalRead(pARMABORT)) {
      writeLCD("A"); //note abort switch is active high
    } else {
      writeLCD("a");
    }
    if (digitalRead(pARM)) {
      writeLCD("A"); //note arm switch is active high
    } else {
      writeLCD("a");
    }

    //in debug mode, handle all buttons as toggle buttons

    if (!digitalRead(pSTAGE) && !stage_prev) {
      stage_on = !stage_on;
      stage_prev = true;
    }
    if (digitalRead(pSTAGE) && stage_prev) {
      stage_prev = false;
    }
    if (stage_on) {
      writeLCD("S");
    } else {
      writeLCD("s");
    }
    digitalWrite(pSTAGELED, stage_on);

    if (!digitalRead(pABORT) && !abort_prev) {
      abort_on = !abort_on;
      abort_prev = true;
    }
    if (digitalRead(pABORT) && abort_prev) {
      abort_prev = false;
    }
    if (abort_on) {
      writeLCD("A");
    } else {
      writeLCD("a");
    }
    digitalWrite(pABORTLED, abort_on);


    LCDline(2);
    if (digitalRead(pCHUTES))writeLCD("C"); else writeLCD("c");
    if (digitalRead(pGEARS))writeLCD("G"); else writeLCD("g");
    if (digitalRead(pLIGHTS))writeLCD("L"); else writeLCD("l");
    if (digitalRead(pSOLAR))writeLCD("S"); else writeLCD("s");
    if (digitalRead(pBRAKES))writeLCD("B"); else writeLCD("b");
    if (digitalRead(pLADDER))writeLCD("L"); else writeLCD("l");
    if (digitalRead(pACTION1))writeLCD("A"); else writeLCD("a");
    if (digitalRead(pACTION2))writeLCD("A"); else writeLCD("a");
    if (digitalRead(pACTION3))writeLCD("A"); else writeLCD("a");


    if (!digitalRead(pTB) && !tb_prev) {
      tb_on = !tb_on;
      tb_prev = true;
    }
    if (digitalRead(pTB) && tb_prev) {
      tb_prev = false;
    }
    if (tb_on) {
      writeLCD("T");
    } else {
      writeLCD("t");
    }

    if (!digitalRead(pRB) && !rb_prev) {
      rb_on = !rb_on;
      rb_prev = true;
    }
    if (digitalRead(pRB) && rb_prev) {
      rb_prev = false;
    }
    if (rb_on) {
      writeLCD("R");
    } else {
      writeLCD("r");
    }

    //analog inputs
    LCDline(3);
    if (debug_screen == 0) {
      throttle_value = analogRead(pTHROTTLE);
      char throttle_char[5];
      itoa(throttle_value, throttle_char, 10);
      writeLCD("Throttle ");
      writeLCD(throttle_char);
    }
    if (debug_screen == 1) {
      tx_value = analogRead(pTX);
      char tx_char[5];
      itoa(tx_value, tx_char, 10);
      writeLCD("Trans ");
      writeLCD(tx_char);
      writeLCD(" ");
      ty_value = analogRead(pTY);
      char ty_char[5];
      itoa(ty_value, ty_char, 10);
      writeLCD(ty_char);
      writeLCD(" ");
      tz_value = analogRead(pTZ);
      char tz_char[5];
      itoa(tz_value, tz_char, 10);
      writeLCD(tz_char);
      writeLCD(" ");
    }

    if (debug_screen == 2) {
      rx_value = analogRead(pRX);
      char rx_char[5];
      itoa(rx_value, rx_char, 10);
      writeLCD("Rot ");
      writeLCD(rx_char);
      writeLCD(" ");
      ry_value = analogRead(pRY);
      char ry_char[5];
      itoa(ry_value, ry_char, 10);
      writeLCD(ry_char);
      writeLCD(" ");
      rz_value = analogRead(pRZ);
      char rz_char[5];
      itoa(rz_value, rz_char, 10);
      writeLCD(rz_char);
    }
    refreshLCD();
    delay(100);
    //end of debug mode
  }


void messageHandler(byte messageType, byte msg[], byte msgSize) {
  // We are only interested in echo response packets.
  if (messageType == ECHO_RESP_MESSAGE) {
    // The message payload will be either "low" or "high".
    // We use the strcmp function to check what the string payload
    // is, and set the LED status accordingly.
    if (strcmp(msg, "low")) {
      digitalWrite(LED_BUILTIN, LOW);
    } else {
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }
}

  void setup() {
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    while (!simpit.init()) {
      delay(100);
    }

    // Turn off the built-in LED to indicate handshaking is complete.
    digitalWrite(LED_BUILTIN, LOW);
    simpit.inboundHandler(messageHandler);
  }


  void check_debug() {
    //check mode
    if (!digitalRead(pSYNCBT) && !digitalRead(pLCDUP))
    {
      debug_flag = !debug_flag;
      delay(1000); // long wait to prevent bounces
      clearLCD();
      writeLCD("DEBUG");
      LCDline(2);

      if (debug_flag) writeLCD("ON");
      else writeLCD("OFF");

      refreshLCD();
      delay(100);
    }
  }

  void loop() {
    unsigned long now = millis();
    //check_debug();
    //if (debug_flag) debug();


  if (now - lastSent >= sendInterval) {   
    stick();
    simpit.update();
  }
  }
