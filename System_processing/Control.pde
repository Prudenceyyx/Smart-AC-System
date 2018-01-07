import controlP5.*;

ControlP5 cp5;
Group gp0, gp1, gp2, gp3, gp4, gp5, gp23, gp_td;

int s = 100;
int toggle_size = 50;

int interval = 20;

boolean POWER = false;
Toggle[] toggles = new Toggle[4]; //Buttons for State
Toggle[] toggles_adjust = new Toggle[9]; //Buttons for TEST

void initControl() {
  cp5 = new ControlP5(this);
  
  gp0 = cp5.addGroup("STDB")
    .setPosition(0, 10)
    .setWidth(x_upleft1)   
    .hideBar()
    ;
  gp1 = cp5.addGroup("AUTO")
    .setPosition(x_upleft1, 0)
    .setWidth(x_upleft2-x_upleft1)
    .hideBar()
    .open()
    ;

  gp2 = cp5.addGroup("MANM")
    .setPosition(x_upleft2, 0)
    .setWidth((width-x_upleft2)/3)
    .hideBar()
    ;

  gp3 = cp5.addGroup("VENT")
    .setPosition(x_upleft2+(width-x_upleft2)/3, 0)
    .setWidth((width-x_upleft2)/3)
    .hideBar()
    ;
  gp4 = cp5.addGroup("TEST")
    .setPosition(x_upleft2, y_midline)
    .setWidth(width-x_upleft2)
    .hideBar()
    ;

  gp5 = cp5.addGroup("Display")
    .setPosition(0, gp0.getHeight()+200)
    .setWidth(gp0.getWidth()+gp1.getWidth()+50)
    .hideBar()
    ;
    
  //The group of Buttons for TEST
  gp_td = cp5.addGroup("TEST_DISPLAY")
    .setPosition((width-x_upleft2)/3, 20)
    .setWidth((width-x_upleft2)/3*2)
    .hideBar()
    ;


  gp23 = cp5.addGroup("SP_FAN")
    .setPosition(width-(width-x_upleft2)/3, 0)
    .setWidth((width-x_upleft2)/3)
    .hideBar()
    ;

  //gp0.addCanvas(new TestCanvas(100, 100));
  //Buttons for the state
  Toggle powr = new Toggle(cp5, "POWER").setPosition(30, 40).setSize(s, s).setState(false).setLabel("POWR_MODE");
  Toggle auto = new Toggle(cp5, "AUTO_MODE").moveTo(gp1);
  Toggle manm = new Toggle(cp5, "MANM_MODE").moveTo(gp2);
  Toggle vent = new Toggle(cp5, "VENT_MODE").moveTo(gp3);
  Toggle test = new Toggle(cp5, "TEST_MODE").moveTo(gp4);

  toggles[0] = auto;
  toggles[1] = manm;
  toggles[2] = vent;
  toggles[3] = test;
 
 powr.getCaptionLabel().setSize(16).setPadding(5,-60);
 for(int i = 0;i<toggles.length;i++){
   toggles[i].setPosition(30, 30).setSize(s, s).setId(1);
   toggles[i].getCaptionLabel().setSize(14).align(ControlP5.CENTER,ControlP5.CENTER);
 }


  //Buttons for MANM and VENT
  //Set to gp23
  new Button(cp5, "SPplus").setLabel("SP+").setPosition(20, 30).setSize(toggle_size, toggle_size).moveTo(gp23);
  new Button(cp5, "SPminus").setLabel("SP-").setPosition(20, 30+toggle_size+interval).setSize(toggle_size, toggle_size).moveTo(gp23);
  new Button(cp5, "FANplus").setLabel("FAN+").setPosition(30+toggle_size, 30).setSize(toggle_size, toggle_size).moveTo(gp23);
  new Button(cp5, "FANminus").setLabel("FAN-").setPosition(30+toggle_size, 30+toggle_size+interval).setSize(toggle_size, toggle_size).moveTo(gp23);


  //Buttons for TEST
  //Set to gp_td, the group for TEST DISPLAY
  String[] toggles_output = {"D1", "D2", "D3"};
  String[] toggles_input = {"P1", "P2"};
  String[] toggles_temp = {"T1", "T2", "T3", "T4"};

  for (int i=0; i<3; i++) {
    toggles_adjust[i] = new Toggle(cp5, toggles_output[i])
      .setPosition(0, (interval+toggle_size)*(i+1)-toggle_size)
      .setSize(toggle_size, toggle_size).moveTo(gp_td).setId(3);
  }

  for (int i=0; i<2; i++) {
    toggles_adjust[3+i] = new Toggle(cp5, toggles_input[i])
      .setPosition(toggle_size+10, (interval+toggle_size)*(i+1)-toggle_size)
      .setSize(toggle_size, toggle_size).moveTo(gp_td).setId(3);
  }

  for (int i=0; i<4; i++) {
    toggles_adjust[5+i] = new Toggle(cp5, toggles_temp[i])
      .setPosition(toggle_size*2+20, (interval+toggle_size)*(i+1)-toggle_size)
      .setSize(toggle_size, toggle_size).moveTo(gp_td).setId(3);
  }

  for(int i = 0; i<toggles_adjust.length;i++){
    toggles_adjust[i].getCaptionLabel().setSize(12).align(ControlP5.CENTER,ControlP5.CENTER);
  }

//Button for Value change in TEST
//Set to gp_td, the group for TEST DISPLAY
  new Button(cp5, "plusten").setLabel("+10")
    .setPosition(toggle_size*3+30, (interval+s)*1-s)
    .setSize(toggle_size, toggle_size).moveTo(gp_td).setId(4);

  new Button(cp5, "minusten").setLabel("-10")
    .setPosition(toggle_size*3+30, (interval+s)*2-s)
    .setSize(toggle_size, toggle_size).moveTo(gp_td).setId(4);

  new Button(cp5, "plusone").setLabel("+1")
    .setPosition(toggle_size*4+40, (interval+s)*1-s)
    .setSize(toggle_size, toggle_size).moveTo(gp_td).setId(4);

  new Button(cp5, "minusone").setLabel("-1")
    .setPosition(toggle_size*4+40, (interval+s)*2-s)
    .setSize(toggle_size, toggle_size).moveTo(gp_td).setId(4);



  gp23.moveTo(gp0);
  gp5.moveTo(gp0);
  gp4.moveTo(gp0);
  gp3.moveTo(gp0);
  gp2.moveTo(gp0);
  gp1.moveTo(gp0);
  gp_td.moveTo(gp4);
  cp5.end();
}

void POWER(boolean theFlag) {
  if (theFlag) {
    gp0.open();
    toggles[0].setState(true);
  } else {
    gp0.close();
    changeState(0);
  }
}

void AUTO_MODE(boolean theFlag) {
  if (theFlag) {
    gp23.close();
    gp_td.close();
    changeState(1);
  } else {
    if (State == 1) toggles[0].setState(true);
  }
}

void MANM_MODE(boolean theFlag) {
  if (theFlag) {
    gp23.open();
    cp5.getController("SPplus").show();
    cp5.getController("SPminus").show();
    gp_td.close();
    changeState(2);
  } else {
    if (State == 2) toggles[1].setState(true);
  }
}

void VENT_MODE(boolean theFlag) {
  if (theFlag) {
    gp23.open();
    cp5.getController("SPplus").hide();
    cp5.getController("SPminus").hide();
    gp_td.close();
    changeState(3);
  } else {
    if (State == 3) toggles[2].setState(true);
  }
}

void TEST_MODE(boolean theFlag) {
  if (theFlag) {
    gp23.close();
    gp_td.open();
    changeState(4);
  } else {
    if (State == 4) toggles[3].setState(true);
  }
}

void changeState(int target) {
  String s = "ST "+target;
  State = target;
  println(s);  
  arduino.write(s);
  delay(10);
}


void controlEvent(ControlEvent theEvent) {
  //Test_Mode: choose only one toggles on
  //It sets the rest toggles off
  //println("Event "+theEvent.getController());

  if (theEvent.getController().getId() == 3) {//If toggles
    Toggle th = (Toggle)theEvent.getController();
    if (th.getState()==true) {//It this is turned on
      for (int i =0; i<9; i++) {//Turn off the rest
        if (th!= toggles_adjust[i]) {
          toggles_adjust[i].setState(false);
        }//End if
      }//End for
    }//End if
  }//End if

  else if ( theEvent.getController().getId() == 1 ) {
    Toggle th = (Toggle)theEvent.getController();
    if (th.getState()==true) {//It this is turned on
      for (int i =0; i<4; i++) {//Turn off the rest
        if (th!= toggles[i]) {
          toggles[i].setState(false);
        }//End if
      }//End for
    }//End if
    else if (th.getState()==false) {
    }
  }
  //Test_Mode +-10 +-1 Buttons
  else if (theEvent.getController().getId() == 4) {
    //arduino.write("ST 4");
    Button bt = (Button)theEvent.getController();
    //Toggle on;
    String name = "";
    for (int i = 0; i<9; i++) {
      if (toggles_adjust[i].getState() == true) {
        name = toggles_adjust[i].getName();
        break;
      }
    }

    if (name != "") {
      String message = name;
      message += " ";
      if (bt.getName() == "plusone") message += "1";
      else if (bt.getName() == "minusone") message += "0";
      else if (bt.getName() == "plusten") message += "3";
      else if (bt.getName() == "minusten") message += "2";
      println(message);
      arduino.write(message);
    }
  } else {
    String name = theEvent.getController().getName();
    String message = "";
    if (name == "SPplus") message = "SP 1"; 
    else if (name == "SPminus")  message = "SP 0";
    else if (name == "FANplus") message = "FA 1";
    else if (name == "FANminus") message = "FA 0";

    println(message);   
    arduino.write(message);
  }
}