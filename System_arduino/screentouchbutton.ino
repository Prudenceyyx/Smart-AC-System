//A converter of toggle's comman from string to the right form
bool pass2callback(String in) {
  //Return if it should send the msg to state machine
  bool result = true;
  if (in == "SP+") {
    callback("SP", 1);
  } else if (in == "SP-") {
    callback("SP", -1);
  } else if (in == "FAN+") {
    callback("D3", 10);
  } else if (in == "FAN-") {
    callback("D3", -10);
  } else if (in == "+10") {
    callback(10);
  } else if (in == "-10") {
    callback(-10);
  } else if (in == "+1") {
    callback(1);
  } else if (in == "-1") {
    callback(-1);
  } else {
    callback(in);
    result = false;
  }
  return result;
}

void callback(String sensor) {
  msg[0] = sensor;
}

void callback(int incre) {
  msg[1] = incre;
}
//The sensor to adjust, and the incre value
void callback(String sensor, int incre) {
  msg[0] = sensor;
  msg[1] = incre;
}

