String determin_state(int state) {
  switch (state) {
    case 0: return "STDB";
    case 1: return "AUTO";
    case 2: return "MANM";
    case 3: return "VENT";
    case 4: return "TEST";
    default: {
        Serial.println("determin_state input error: " + state);
        return "AUTO";
      }
  }//End of switch
}

//Evalute the abnormal condition based on formulas
boolean overtemp_validation() {
  //  OverTemp = false;//DP1 = 100;
  if (T3_value > 70 & DP1_value == 0 & DP2_value == 0) {
    OverTemp = true;
  } else if (T3_value < 45) {
    OverTemp = false;
  }
  return OverTemp;
}

boolean overpressure_validation() {
  if (P1_value > 1) { //DP1 = 100;
    OverPressure = true;
  } else {
    OverPressure = false;
  }
  return OverPressure;
}


void hx_validation() {
  if (DP1_value == 0 & DP2_value == 0 & T3_value - T2_value < 5) {
    if (millis() - Hx_timestamp > 1000) {
      HX_Clogged = true;
    }
  } else {
    Hx_timestamp = millis();
    HX_Clogged = false;
  }

}

void leaking_validation() {
  if (P1_value - P2_value < 0.2 & DP1_value == 0) {
    if (millis() - Leaking_timestamp > 1000) {
      Leaking = true;
    } else {
      Leaking_timestamp = millis();
      Leaking = false;
    }

  }
}




