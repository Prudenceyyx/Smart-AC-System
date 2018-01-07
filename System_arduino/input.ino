
//Read sensor values
void update_input() {
  noInterrupts();

  if (STATE == AUTO or STATE == MANM or STATE == VENT) {

    T1_value = read_temp(1);
    T2_value = read_temp(2);
    T3_value = read_temp(3);
    T4_value = read_temp(4);

    P1_value = map(analogRead(P1_pin), 0, 1024, 0, 1.6);
    P2_value = map(analogRead(P2_pin), 0, 1024, 0, 1.6);

    DP1_value = readValue(DP1.getWiper());
    DP2_value = readValue(DP2.getWiper());

  }
  
  if (STATE == AUTO) {
    DP3_value = readValue(DP3.getWiper());
  } else if (STATE == TEST) {
    DP1_value = readValue(DP1.getWiper());
    DP2_value = readValue(DP2.getWiper());
    DP3_value = readValue(DP3.getWiper());

  }

  interrupts();

}

//Calculate expected DP values according to formula
void calculate_output() {
  if (STATE == AUTO or STATE == MANM) {
    DPs[0] = int(update_DP1());
    DPs[1] = int(update_DP2());
    DPs[2] = int(update_DP3());
  }
}

//Adjust DP to its expected value
void update_dps() {
  if (STATE == AUTO or STATE == MANM or STATE == VENT or  STATE == TEST) {
    if (DPs[0] != DP1_value) {
      int i = 0;
      while (DPs[0] > DP1_value && i < 10) {
        increValue(DP1.getCS(), DP1.getINC(), DP1.getUD(), true);
        DP1_value = DP1.readValue();
        i += 1;
      }
      while (DPs[0] < DP1_value && i < 10) {
        increValue(DP1.getCS(), DP1.getINC(), DP1.getUD(), false);
        DP1_value = DP1.readValue();
        i += 1;
      }

      storeValue(DP1.getCS(), DP1.getINC(), DP1.getUD());
    }

    if (DPs[1] != DP2_value) {
      int i = 0;
      while (DPs[1] > DP2_value && i < 5) {
        increValue(DP2.getCS(), DP2.getINC(), DP2.getUD(), true);
        DP2_value = DP2.readValue();
        i += 1;
      }
      while (DPs[1] < DP2_value && i < 5) {
        increValue(DP2.getCS(), DP2.getINC(), DP2.getUD(), false);
        DP2_value = DP2.readValue();
        i += 1;
      }

      storeValue(DP2.getCS(), DP2.getINC(), DP2.getUD());
    }
    //    Serial.println(DPs[1]);
  }

  //  if (STATE == AUTO) {
  if (DPs[2] != DP3_value) {
    int i = 0;
    while (DPs[2] > DP3_value && i < 10) {
      increValue(DP3.getCS(), DP3.getINC(), DP3.getUD(), true);
      DP3_value = DP3.readValue();
      i += 1;
    }
    while (DPs[2] < DP3_value && i < 10) {
      increValue(DP3.getCS(), DP3.getINC(), DP3.getUD(), false);
      DP3_value = DP3.readValue();
      i += 1;
    }
    storeValue(DP3.getCS(), DP3.getINC(), DP3.getUD());

  }

}


//Save all value to the array for storage
//And judge the Abnormal condition based on the values
void update_output() {

  DP1_value = readValue(DP1.getWiper());
  DP2_value = readValue(DP2.getWiper());
  DP3_value = readValue(DP3.getWiper());

  data[0] = T1_value;
  data[1] = T2_value;
  data[2] = T3_value;
  data[3] = T4_value;

  data[4] = P1_value;
  data[5] = P2_value;

  data[6] = DP1_value;
  data[7] = DP2_value;
  data[8] = DP3_value;

  overtemp_validation();
  overpressure_validation();
  leaking_validation();
  hx_validation();

  data[9] = OverTemp;
  data[10] = OverPressure;
  data[11] = Leaking;
  data[12] = HX_Clogged;
  data[13] = set_point;
  data[14] = STATE;
}


//Return the target value of DP
//accordign to Temperature and pressure sensors
int update_DP1() {
  if (overtemp_validation() or overpressure_validation()) {
    return 100;
  }
  if (STATE == AUTO or STATE == MANM) {
    float DP1_target = T1toDP1(T1_value);
    return DP1_target;
    //TODO: 5%/s

  } else if (STATE == VENT) {
    return 100;
  }
}

double T1toDP1(float T1) {
  int DP1_target;
  if (T1 < set_point - 2) {
    DP1_target = 100;
  } else if (T1 < set_point - 1) {
    DP1_target = 50 * set_point - 50 * T1;
  } else if (T1 < set_point) {
    DP1_target = 50;
  } else if (T1 < set_point + 2) {
    DP1_target = 25 * set_point + 50 - 25 * T1;
  } else if (T1 >= set_point + 2) {
    DP1_target = 0;
  }

  return DP1_target;
}

int update_DP2() {
  if (overtemp_validation() or overpressure_validation()) {
    return 0;
  }
  if (STATE == AUTO or STATE == MANM) {

    if (DP1_value == 0) return 0;
    float DP2_target = T1toDP2(T1_value);
    return DP2_target;
    //TODO: 5%/s

  } else if (STATE == VENT) {
    return 100;
  }
}


double T1toDP2(float T1) {
  int DP2_target;
  if (T1 < set_point - 2) {
    DP2_target = 100;
  } else if (T1 < set_point) {
    DP2_target = 50 * set_point - 50 * T1;
  } else if (T1 >= set_point) {
    DP2_target = 0;
  }
  return DP2_target;
}


int update_DP3() {
  if (STATE == AUTO) {
    return T1toDP3(T1_value);
  } else if (STATE == MANM) {
    return DPs[2];
  } else if (STATE == VENT) {
    //      return 50;
    return DPs[2];
  } else {
    return DPs[2];
  }
}

double T1toDP3(float T1) {
  int DP3_target;
  if (T1 < set_point - 2) {
    DP3_target = 100;
  } else if (T1 < set_point) {
    DP3_target = 50 * set_point - 50 * T1;
  } else if (T1 >= set_point) {
    DP3_target = 0;
  }
  return DP3_target;
}


void DPset(int CS, int INC, int UD) {
  // CS LOW is active
  pinMode(CS, OUTPUT);
  digitalWrite(CS, LOW); // ON

  // INC - LOW is active
  pinMode(INC, OUTPUT);
  digitalWrite(INC, HIGH); // OFF

  // UP is high, DOWN is low
  pinMode(UD, OUTPUT);

  delay(5);
  Serial.println("Setup completed");
}

void increValue(int CS, int INC, int UD, boolean direction) {
  // Set the direction, up/down
  digitalWrite(UD, direction);
  // Increment or Decrement as per the UD value
  digitalWrite(INC, LOW);
  delay(1);
  // Done!
  digitalWrite(INC, HIGH);

  delay(1);
}

int readValue(int pin) {
  return map(analogRead(pin), 0, 1023, 0, 100);
}

void storeValue(int CS, int INC, int UD) {
  // Ensure INC is HIGH then move CS HIGH and back to LOW
  digitalWrite(INC, HIGH);
  delayMicroseconds(1);
  digitalWrite(CS, HIGH);
  delay(5);
  digitalWrite(CS, LOW);
}

void setTarget(String name, int incre_val) {
  if (name == "DP3") {
    DP3_value = readValue(DP3.getWiper());
    int target_val = DP3_value + incre_val;
    int attemp = abs(incre_val) + 5;
    int trytimes = 0;
    while (DP3_value != target_val && trytimes < attemp) {
      increValue(DP3.getCS(), DP3.getINC(), DP3.getUD(), incre_val > 0);
      DP3_value = readValue(DP3.getWiper());
      trytimes += 1;
    }
    storeValue(DP3.getCS(), DP3.getINC(), DP3.getUD());

  }
  else if (name == "DP2") {
    DP2_value = readValue(DP2.getWiper());
    int target_val = DP2_value + incre_val;
    int attemp = abs(incre_val) + 5;
    int trytimes = 0;
    while (DP2_value != target_val && trytimes < attemp) {
      increValue(DP2.getCS(), DP2.getINC(), DP2.getUD(), incre_val > 0);
      DP2_value = readValue(DP2.getWiper());
      trytimes += 1;
    }
    storeValue(DP2.getCS(), DP2.getINC(), DP2.getUD());

  }
  else if (name == "DP1") {
    DP1_value = readValue(DP1.getWiper());
    int target_val = DP1_value + incre_val;
    int attemp = abs(incre_val) + 5;
    int trytimes = 0;
    while (DP1_value != target_val && trytimes < attemp) {
      increValue(DP1.getCS(), DP1.getINC(), DP1.getUD(), incre_val > 0);
      DP1_value = readValue(DP1.getWiper());
      trytimes += 1;
    }
    storeValue(DP1.getCS(), DP1.getINC(), DP1.getUD());

  }

}

float read_temp(int sensor) {
  OneWire* ds;
  if (sensor == 1) {
    ds = &T1;
  } else if (sensor == 2) {
    ds = &T2;
  } else if (sensor == 3) {
    ds = &T3;
  } else if (sensor == 4) {
    ds = &T4;
  } else {
    return -100;
  }
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addrs[8];
  float celsius, fahrenheit;

  if ( !ds->search(addrs)) {
    //    Serial.println("No more addrsesses.");
    //    Serial.println();
    ds->reset_search();
//    delay(250);
    return -100;
  }

  //  Serial.print("ROM =");
  //  for ( i = 0; i < 8; i++) {
  //    Serial.write(' ');
  //    Serial.print(addrs[i], HEX);
  //  }

  if (OneWire::crc8(addrs, 7) != addrs[7]) {
    //    Serial.println("CRC is not valid!");
    return -100;
  }
  //  Serial.println();

  // the first ROM byte indicates which chip
  switch (addrs[0]) {
    case 0x10:
      //      Serial.println("  Chip = DS18S20");  // or old DS1820
      type_s = 1;
      break;
    case 0x28:
      //      Serial.println("  Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
      //      Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    default:
      //      Serial.println("Device is not a DS18x20 family device.");
      return -100;
  }

  ds->reset();
  ds->select(addrs);
  ds->write(0x44, 1);        // start conversion, with parasite power on at the end

  delay(10);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds->reset();
  ds->select(addrs);
  ds->write(0xBE);         // Read Scratchpad

  //  Serial.print("  Data = ");
  //  Serial.print(present, HEX);
  //  Serial.print(" ");
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds->read();
    //    Serial.print(data[i], HEX);
    //    Serial.print(" ");
  }
  //  Serial.print(" CRC=");
  //  Serial.print(OneWire::crc8(data, 8), HEX);
  //  Serial.println();

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  ds->reset_search();
  celsius = (float)raw / 16.0;
  return celsius;
}


