
void initThreads() {
//  updateThread.onRun(update_input);
//  updateThread.setInterval(500);
  saveThread.onRun(saveFile);
  saveThread.setInterval(500);
  printThread.onRun(print_data);
  printThread.setInterval(1000);
}


void print_data() {
  //The main loop
  //1. update read + summerize
  //2. update expected dps
  //3. update output(ds) + summerize
  //4. send or display
  noInterrupts();
  if (STATE != STDB) {
    update_input();
    calculate_output();
    update_dps();
    update_output();
    send_data();
  }
  interrupts();

  noInterrupts();
  if (STATE == STDB) {
    textFill(RA8875_WHITE, RA8875_WHITE);
  } else {
    textFill(RA8875_BLACK, RA8875_WHITE);
  }
  display_input();
  interrupts();

}


//callback for printThread
void send_data() {

  //  String stdata[13];
  String st = "<";
  for (int i = 0; i < 6; i++) {
    //    stdata[i] = String(data[i], 2);
    st += String(data[i], 2);
    st += " ";
  }
  for (int i = 6; i < 15; i++) {
    st += String(int(data[i]));
    st += " ";
  }

  st += ">";
  Serial.println(st);

}



// callback for saveThread
void saveFile() {
  //Print hh:mm:ss
  time_message = "";
  time_message += __DATE__;
  time_message += ',';
  if (hh < 10) time_message += '0';
  time_message += hh;
  time_message += ':';
  if (mm < 10) time_message += '0';
  time_message += mm;
  time_message += ':';
  if (ss < 10) time_message += '0';
  time_message += ss;
  noInterrupts();
  writeFile(time_message);
  interrupts();

  //  Serial.print(__DATE__);
  //  Serial.print(',');
  //  if (hh < 10) Serial.print('0');
  //  Serial.print (hh);
  //  Serial.print (':');
  //  if (mm < 10) Serial.print('0');
  //  Serial.print (mm);
  //  Serial.print (':');
  //  if (ss < 10) Serial.print('0');
  //  Serial.println (ss);


  noInterrupts();
  ss++;
  if (ss == 60) {
    ss = 0;
    mm++;
    if (mm > 59) {
      mm = 0;
      hh++;
      if (hh > 23) hh = 0;
    }
  }
  interrupts();
}

