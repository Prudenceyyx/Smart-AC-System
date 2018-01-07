import processing.serial.*;
Serial arduino;

String T1, T2, T3, T4, P1, P2, DP1, DP2, DP3;
boolean OverTemp, OverPressure, Leaking, HX_Clogged;
int SP = 24;
int State;

String input="";
boolean started=false;
boolean ended = false;
String[] data;

void setup() {
  size(1024, 600);
  init_parameters();
  printArray(Serial.list());
  String arduinoName = Serial.list()[2];

  println(arduinoName);
  arduino = new Serial(this, arduinoName, 9600);
  T1=T2=T3=T4=P1=P2=DP1=DP2="";
  State = 0;

  initControl();
}

void draw() {
  draw_background();

  //display_inputfield();
  //display_button();
  if(State!=0)
  update_input();

  display();
  delay(100);
}