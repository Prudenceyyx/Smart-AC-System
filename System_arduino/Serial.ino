//String msg[2] = {"D1", "10"};
//read_input for the state system to adjust sensor values
void read_input() {
  /*Read from Display Screen*/
  String command = msg[0];
  int value = msg[1].toInt();
  if(value == 0){
    return;
  }
  if (command == "SP") {
    set_point += value;
  } else if (command == "T1") {
    T1_value = constrain(T1_value + value, -10, 80);
  } else if (command == "T2") {
    T2_value = constrain(T2_value + value, -10, 80);
  } else if (command == "T3") {
    T3_value = constrain(T3_value + value, -10, 80);
  } else if (command == "T4") {
    T4_value = constrain(T4_value + value, -10, 80);
  } else if (command == "P1") {
    P1_value = constrain(P1_value + value, 0, 1.6);
  } else if (command == "P2") {
    P2_value = constrain(P2_value + value, 0, 1.6);
  } else if (command == "D1") {
    DPs[0] += value;
//    setTarget("DP1", value);
  } else if (command == "D2") {
    DPs[1] += value;
//    setTarget("DP2", value);
  } else if (command == "D3") {
    DPs[2] += value;
//    setTarget("DP3", value);
  }

}


//If send to Processing through Serial
//void read_state() {
//  String dataArrary[2];
//  if ( Serial.available() > 0 ) {
//    String input = Serial.readString();
//    String command = input.substring(0, 2);
//    String content = input.substring(3, 4);
//
//    if (command == "ST") {
//      int income = content.toInt();
//      if (income >= 0 and income <= 4) {
//        if (STATE != income) init_state(income);
//      }
//
//    } else if (command == "SP" ) {
//      //      STATE = MANM;
//      if (content == "0") set_point -= 1;
//      else if (content == "1") set_point += 1;
//      //and (STATE == MANM)
//
//    } else if (command == "FA") {
//      if (content == "0") setTarget("DP3", -10);
//      else if (content == "1") setTarget("DP3", 10);
//    }
//    else if (command == "D1") {
//      if (content == "0") setTarget("DP1", -1);
//      else if (content == "1") setTarget("DP1", 1);
//      else if (content == "2") setTarget("DP1", -10);
//      else if (content == "3") setTarget("DP1", 10);
//      //storeValue(DP1.getCS(), DP1.getINC(), DP1.getUD());
//
//    } else if (command == "D2") {
//      if (content == "0") setTarget("DP2", -1);
//      else if (content == "1") setTarget("DP2", 1);
//      else if (content == "2") setTarget("DP2", -10);
//      else if (content == "3") setTarget("DP2", 10);
//      //      storeValue(DP2.getCS(), DP2.getINC(), DP2.getUD());
//
//    } else if (command == "D3") {
//      if (content == "0") setTarget("DP3", -1);
//      else if (content == "1") setTarget("DP3", 1);
//      else if (content == "2") setTarget("DP3", -10);
//      else if (content == "3") setTarget("DP3", 10);
//
//    } else {
//
//      int value = 0;
//      if (content == "0") value = -1;
//      else if (content == "1") value = 1;
//      else if (content == "2") value = -10;
//      else if (content == "3") value = 10;
//
//      if (command == "T1") T1_value = constrain(T1_value + value, -10, 80);
//      else if (command == "T2") T2_value = constrain(T2_value + value, -10, 80);
//      else if (command == "T3") T3_value = constrain(T3_value + value, -10, 80);
//      else if (command == "T4") T4_value = constrain(T4_value + value, -10, 80);
//      else if (command == "P1") P1_value = constrain(P1_value + value, 0, 1.6);
//      else if (command == "P2") P2_value = constrain(P2_value + value, 0, 1.6);
//
//    }
//  }
//}



