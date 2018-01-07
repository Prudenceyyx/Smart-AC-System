/*

The project is an intelligient Air Conditioner adjuster.
It makes the AC adjust the power of its motors according
to the environment's temperature and atmosphere pressure.


Sensor types:
Temperature ----------- DS18B20
Atmosphere pressure --- analog sensor

Output types:
Digital Potentiometer-- X9C102P, X9C104P
Capacitive Touchable Screen - RA8875(800 x 480) from Buydisplay.com

State Machine fields:
STATE: STDB --- to turn off the power
       AUTO --- to run automatically
       MANM --- to allow manual adjust of SP and DP3
       VENT --- to allow manual adjust of DP3
       TEST --- to allow manual adjust of all sensors and outputs, meanwhile suspend the auto update function
SP: a set_point as the formula's coefficient

Display System
One button for each state. Press the button to swtitch to the state.
Several groups of toggles for certain states. One group of toggles show only when the state they belong to is on.
Input Field at the left bottom corner keeps showing the update sensor value
Warning Field at the right bottom corner shows the abnormal evaluation only when there is such a condition.

It saves the data to SD card every second.
*/


#include <OneWire.h>
#include <X9C10XP.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Thread.h>
#include <StensTimer.h>
#include "Fsm.h"
#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"
#include <stdint.h>
uint8_t addr  = 0x38;

//Arduino DUE,Arduino mega2560,Arduino UNO
#define RA8875_INT 4
#define RA8875_CS 10
#define RA8875_RESET 9
#define FT5206_WAKE 6
#define FT5206_INT  7



String msg[2] = {};

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);
uint16_t tx, ty;

/*Touch parameters*****************/
enum {
  eNORMAL = 0,
  eTEST   = 0x04,
  eSYSTEM = 0x01
};

struct TouchLocation
{
  uint16_t x;
  uint16_t y;
};

TouchLocation touchLocations[5];
uint8_t attention;
static uint8_t oldAttention = 1;
uint32_t thisTouchTime = millis();
uint8_t i = 0;
static uint32_t lastTouchTime = thisTouchTime;

//Functions to be initialized later
uint8_t readFT5206TouchRegister( uint8_t reg );
uint8_t readFT5206TouchLocation( TouchLocation * pLoc, uint8_t num );
uint8_t readFT5206TouchAddr( uint8_t regAddr, uint8_t * pBuf, uint8_t len );
uint32_t dist(const TouchLocation & loc);
uint32_t dist(const TouchLocation & loc1, const TouchLocation & loc2);
bool sameLoc( const TouchLocation & loc, const TouchLocation & loc2 );
uint8_t buf[30];
/******************/

/*Display parameters*/
int width = 800;
int height = 480;
int y_midline;
int x_upleft1, x_upleft2;
int x_downleft1;
int y_downline;
int input_width;
int input_height;
int button_radius;
int strokeWeight = 5;

uint16_t color = RA8875_WHITE;
uint16_t textColor = RA8875_WHITE;
uint16_t backColor = RA8875_BLUE;
#define RA8875_LIGHTGREY 0xC618
#define RA8875_DARKGREY 0x7BEF
#define RA8875_DARKBLUE 0x0015
#define RA8875_C1 0x4E1D
#define RA8875_C2 0x3D7B

static uint16_t total = 256;
static uint16_t w = tft.width();
static uint16_t h = tft.height();

float xScale = 1024.0F / w;
float yScale = 1024.0F / h;

char* string2char(String command) {
  if (command.length() != 0) {
    char *p = const_cast<char*>(command.c_str());
    return p;
  }
}

//Functions to be initialized later
void textFill(uint16_t newcolor, uint16_t bgcolor);
void text(const char* content, float x, float y);
void text(String content, float x, float y) {
  text(string2char(content), x, y);
}
void text(int content, float x, float y) {
  char result[8]; // Buffer big enough for 7-character float
  dtostrf(content, 6, 2, result); // Leave room for too large numbers!
  text(result, x, y);
}

void text(float content, float x, float y) {
  char result[8]; // Buffer big enough for 7-character float
  dtostrf(content, 6, 2, result); // Leave room for too large numbers!
  text(result, x, y);
}
/******************/

/*Customized class Button
  for both mode buttons and functional buttons
*/
class Button {
  private:
    char* _title;
    float _x;
    float _y;
    float _w;
    //    float _h;
    bool _isOn;
    bool _isShow;
    int _color;
    int _fsm_state;
    int _isToggle;

  public:
    Button(char* title, float button_x, float button_y, float button_w, int button_state) {
      _title = title;
      _x = button_x;
      _y = button_y;
      _w = button_w;
      //_h = button_h;
      _isOn = false;
      _isShow = true;
      _color = color;
      _fsm_state = button_state;
    };

    void draw() {
      int tempc = _isOn ? RA8875_DARKBLUE : _color;
      tft.graphicsMode();
      if (_isShow) {
        tft.fillRoundRect(_x, _y, _w, _w, 10, tempc);
        tft.textMode();
        textFill(RA8875_WHITE, tempc);
        text(_title, _x + _w / 2 - 20, _y + _w / 2);
      } else {
        tft.fillRoundRect(_x, _y, _w, _w, 10, RA8875_WHITE);
      }
      return;
    }

    char* getTitle() {
      return _title;
    }

    void setToggle() {
      _isToggle = true;
    }

    void show() {
      _isShow = true;
      draw();
    }

    void hide() {
      _isShow = false;
      draw();
    }

    void setOff() {
      _isOn = false;
      draw();
    }

    void setOn() {
      _isOn = true;
      draw();
    }

    bool isTouched(float x, float y) {
      return _isShow && (x >= _x && x <= _x + _w) && (y >= _y && y <= _y + _w);
    }

    void callback() {
      if (_isOn) {
        setOff();
      } else {
        setOn();
      }
    }

    bool isPressed(float x, float y) {
      if (isTouched(x, y)) {
        return true;
      }
      return false;
    }

    bool isOn() {
      return _isOn;
    }

};
/******************/


const int STDB = 0;
const int AUTO = 1;
const int MANM = 2;
const int VENT = 3;
const int TEST = 4;

String input = "";
int STATE = AUTO;

void init_state(int i);
void init_state_stdb() {
  init_state(0);
}
void init_state_auto() {
  init_state(1);
}
void init_state_manm() {
  init_state(2);
}
void init_state_vent() {
  init_state(3);
}
void init_state_test() {
  init_state(4);
}

State state_stdb(&init_state_stdb, NULL, NULL);
State state_auto(&init_state_auto, NULL, NULL);
State state_manm(&init_state_manm, NULL, &leave_from_state);
State state_vent(&init_state_vent, NULL, &leave_from_state);
State state_test(&init_state_test, NULL, &leave_from_state);
State* states[5];

Fsm fsm(&state_auto);


class StateSystem {
    //  private:
    //  int current_state;
  public:
    StateSystem() {};

    void init() {
      states[0] = &state_stdb;
      states[1] = &state_auto;
      states[2] = &state_manm;
      states[3] = &state_vent;
      states[4] = &state_test;

      for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
          if (i == j) continue;
          fsm.add_transition(states[j], states[i], i, NULL);
        }

      }
    }
};


/*Customized class DisplaySystem
  connect between button touchevent and state machine callback functions
*/

class DisplaySystem {
  private:
    int _size;
    Button* _buttons[5];
    int _state = 0;

  public:
    DisplaySystem() {
      _size = 5;
      int s = int(y_midline * 0.6);
      float w = (x_upleft1 - strokeWeight) * 0.2;
      float h = (y_midline - strokeWeight) * 0.2;
      fill(RA8875_BLUE);
      _buttons[0] = new Button("STDB", w, h, s, 0);
      _buttons[1] = new Button("AUTO", x_upleft1 + w, h, s, 1);
      _buttons[2] = new Button("MANM", x_upleft2 + w, h, s, 2);
      _buttons[3] = new Button("VENT", x_upleft2 + w + s + strokeWeight, h, s, 3);
      _buttons[4] = new Button("TEST", x_upleft2 + w + strokeWeight, y_midline + h, s, 4);

      setOn(1);
      //      _buttons[1]->setOn();
    };

    bool setOn(int i) {
      if (_state != i) {
        _buttons[i]->setOn();
        _buttons[_state]->setOff();
        _state = i;
        return true;
      }
      return false;
    }
    void draw() {
      for (int i = 0; i < _size; i++) {
        _buttons[i]->draw();
      }
    }

    void check_button() {
      float x = touchLocations[0].x;
      float y = touchLocations[0].y;
      for (int i = 0; i < _size; i++) {
        if (_buttons[i]->isTouched(x, y)) {
          bool b = setOn(i);
          Serial.print(i);
          Serial.println(" button_pressed");
          if (b) {
            fsm.trigger(i);
            //            STATE = i;

          }
        }
      }
    }

    int get_state() {
      return _state;
    }

};
/******************/


/*Customized class Toggles
  build set of toggles based on buttons
*/
StensTimer* stensTimer; //A timer to turn off toggles based on time
DisplaySystem* display_system;
void sendmsg() {

  //sendmsg to statemachine
  Serial.print(msg[0]);
  Serial.print("  ");
  Serial.println(msg[1]);
  read_input();
  //  msg[0] = "";
  msg[1] = "0";
}

class Toggles {
  private:
    int _size;
    Button* _toggles[9];
    int _state = -1;
    int _panel = false;
    bool _isShow = false;

  public:
    Toggles(int size, bool ifPanel) {
      _size = size;
      _panel = ifPanel;
    };

    void set_callback(void(*u)()) {
      this->update = u;
    }

    void (*update)();

    void add_toogle(int i, char* title, float button_x, float button_y, float button_w) {
      _toggles[i] = new Button(title,  button_x,  button_y,  button_w, 0);
      _toggles[i]->hide();
    }

    void draw() {
      for (int i = 0; i < _size; i++) {
        _toggles[i]->draw();
      }
    }

    void show() {
      _isShow = true;
      for (int i = 0; i < _size; i++) {
        _toggles[i]->show();
      }
    }

    void hide() {
      _isShow = false;
      for (int i = 0; i < _size; i++) {
        _toggles[i]->hide();
      }
    }

    void setOff(int i) {
      _toggles[i]->setOff();
      _state = -1;
    }

    bool setOn(int i) {
      if (_state == -1) {
        _toggles[i]->setOn();
        _state = i;
        return true;
      }
      if (_panel) {
        if (_state == i) {
          setOff(_state);
        } else {
          setOff(_state);
          setOn(i);
        }
        return true;
      }
    }

    void check_toggle() {
      float x = touchLocations[0].x;
      float y = touchLocations[0].y;
      for (int i = 0; i < _size; i++) {
        if (_toggles[i]->isTouched(x, y)) {
          setOn(i);
          /*If it is button mode, set it off after 200 ms
            else if it is toggle mode, set if off if the same toggle is pressed twice*/
          if (pass2callback(_toggles[i]->getTitle())) {
            sendmsg();//sendmsg to statemachine
          }
          if (_panel == false) {
            stensTimer->setTimer(display_system->get_state() * 10 + i, 200);
          }
          Serial.print(i);
          Serial.println(" toggle pressed");
          //if (b) fsm.trigger(i);
        }
      }
    }
};
/******************/




Toggles toggles_manm(4, false);
Toggles toggles_vent(2, false);
Toggles toggles_test(9, true);
Toggles toggles_test_control(4, false);


//Input
float T1_value, T2_value, T3_value, T4_value;
float P1_value, P2_value;
//Output
int DP1_value, DP2_value, DP3_value;
int DPs[3];

//Input pin
int T1_pin = 41;
int T2_pin = 43;
int T3_pin = 45;
int T4_pin = 47;

OneWire  T1(T1_pin);
OneWire  T2(T2_pin);
OneWire  T3(T3_pin);
OneWire  T4(T4_pin);


int P1_pin = A4;
int P2_pin = A5;

//Output pin read
int DP1_pin = A1;
int DP2_pin = A2;
int DP3_pin = A3;
X9C10XP DP1(22, 24, 25, DP1_pin);
X9C10XP DP2(26, 26, 29, DP2_pin);
X9C10XP DP3(30, 32, 33, DP3_pin);
uint32_t targetTime = 0;// for next 1 second timeout
uint8_t hh, mm, ss; //containers for current time
//My simple Thread
Thread updateThread = Thread();
Thread saveThread = Thread();
Thread printThread = Thread();
String time_message;

//Validation
boolean OverTemp, OverPressure, Leaking, HX_Clogged = false;
//Validation timestamp
int Hx_timestamp;
int Leaking_timestamp;

//SP
int set_point = 24;

float data[15]; //6 input, 3output, 4 status, 1 set_point, 1state

String fileName;
File myFile;
int CS_pin = 5;

const String monthss[12] = {"jan", "feb", "mar", "apr", "may", "june", "july", "aug", "sep", "oct", "nov", "dec"};

void setup(void) {
  Serial.begin(9600);
  STATE = STDB;
  //Init validation
  Hx_timestamp = millis();
  Leaking_timestamp = millis();

  for (int i = 0; i < 14; i++) {
    data[i] = 0;
  }
  DP1.init();
  DP2.init();
  DP3.init();
  data[15] = 0;
  DPs[0] =  readValue(DP1.getWiper());
  DPs[1] =  readValue(DP2.getWiper());
  DPs[2] =  readValue(DP3.getWiper());

  hh = conv2d(__TIME__);
  mm = conv2d(__TIME__ + 3);
  ss = conv2d(__TIME__ + 6);
  targetTime = millis() + 1000;
  initSD();
  init_displaysystem();
  initThreads();

}

void loop(void) {
  //stensTimer for toggles
  stensTimer->run();
  //  //1.Read the state (touch)
  //  //  read_state();
  touch_event();
  //state fsm
  fsm.run_machine();
  //  T1_value = T1.Update();
  //  Serial.print(T1_value);
  //
  //  //  if (STATE != STDB) {
  if (printThread.shouldRun()) printThread.run();
  //  //2.Read input
  //  update_input();
  //
  //
  //  //3.Write output
  //  update_output();

  //  display_input(); // move to print Thread
  //4.Send to Processing/Display

  if (saveThread.shouldRun()) saveThread.run();

  //
  delay(10); //just here to slow down the output so it is easier to read
  //  //  }
}




void leave_from_state() {
  if (STATE == AUTO) {
    
  } else if (STATE == MANM) {
    toggles_manm.hide();
  } else if (STATE == VENT) {
    toggles_vent.hide();
  } else if (STATE == TEST) {
    toggles_test.hide();
    toggles_test_control.hide();
  }
}

void init_state(int i) {
  //  STATE = income;
  STATE = i;
  set_point = 24;
  if (STATE == AUTO) {
    set_point = 24;
  } else if (STATE == MANM) {
    set_point = 24;
    toggles_manm.show();
  } else if (STATE == VENT) {
    toggles_vent.show();
    //    DPs[2] = 50;
    DP3_value = readValue(DP3.getWiper());
    int attemp = 60;
    int trytimes = 0;
    while (DP3_value != 50 && trytimes < attemp) {
      increValue(DP3.getCS(), DP3.getINC(), DP3.getUD(), 50 > DP3_value);
      DP3_value = readValue(DP3.getWiper());
      trytimes += 1;
    }
    storeValue(DP3.getCS(), DP3.getINC(), DP3.getUD());

  } else if (STATE == TEST) {
    toggles_test.show();
    toggles_test_control.show();
  }
}
