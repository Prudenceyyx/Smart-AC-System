void display() {
  try {
    fill(50);
    textSize(22);
    //T1
    text(T1, (x_downleft1/2-input_width)/2+40, x_downleft1/2-20);
    text(T2, (x_downleft1/2-input_width)/2+40, x_downleft1/2-20+(height - y_midline)/5);
    text(T3, (x_downleft1/2-input_width)/2+40, x_downleft1/2-20+(height - y_midline)/5*2);
    text(T4, (x_downleft1/2-input_width)/2+40, x_downleft1/2-20+(height - y_midline)/5*3);

    //DP1
    text(P1, (x_downleft1/2-input_width)/2+x_downleft1/2+40, x_downleft1/2-20);
    text(P2, (x_downleft1/2-input_width)/2+x_downleft1/2+40, x_downleft1/2-20+(height - y_midline)/5);
    text(DP1, (x_downleft1/2-input_width)/2+x_downleft1/2+40, x_downleft1/2-20+(height - y_midline)/5*2);
    text(DP2, (x_downleft1/2-input_width)/2+x_downleft1/2+40, x_downleft1/2-20+(height - y_midline)/5*3); 
    text(DP3, (x_downleft1/2-input_width)/2+x_downleft1/2+40, x_downleft1/2-20+(height - y_midline)/5*4);

    text(SP, x_upleft1+button_radius+50, y_midline/2);

    if (OverTemp) text("OverTemp", x_downleft1+20, y_downline+25);
    if (OverPressure) text("OverPressure", x_downleft1+20, y_downline+25*2);
    if (Leaking) text("Leaking", x_downleft1+20, y_downline+25*3);
    if (HX_Clogged) text("HX_Clogged", x_downleft1+20, y_downline+25*4);
  }
  catch(Exception e) {
    //println("Input data error");
  }
}

void update_input() {
  while ( arduino.available()>0) {
    char a = arduino.readChar();
    //print(a);
    if (a == '<') {
      started = true;
      ended = false;
    } else if (a == '>') {
      ended = true;
      break;
    } else if (started && !ended) {
      input+=a;
    }
  }
  if (started && ended) {
    //println(true);
    println(input);
    data = input.split(" ");
    input = "";
    //printArray(data);
    if (data.length==15) {

      T1 = data[0];
      T2 = data[1];
      T3 = data[2];
      T4 = data[3];
      P1 = data[4];
      P2 = data[5];
      DP1 = data[6];
      DP2 = data[7];
      DP3 = data[8];
      OverTemp = (int(data[9])!=0);
      OverPressure = (int(data[10])!=0);
      Leaking = (int(data[11])!=0);
      HX_Clogged = (int(data[12])!=0);
      SP = int(data[13]);
      if(State != int(data[14])) changeState(State);
    }
    started = false;
    ended = false;
  }
}