void init_displaysystem() {
  init_fsm();
  init_screen();
  init_parameters();
  display_system = new DisplaySystem();
  init_timer();
  init_toggles();

  draw_background();

  Serial.print("Status: ");
  Serial.println(tft.readStatus(), HEX);
  Serial.println("Waiting for touch events ...");
}


void init_screen() {
  Serial.println("RA8875 start");
  Wire.begin();        // join i2c bus (address optional for master)

  readOriginValues();
  pinMode     (FT5206_WAKE, INPUT);
  digitalWrite(FT5206_WAKE, HIGH );
  writeFT5206TouchRegister(0, eNORMAL); // device mode = Normal

  uint8_t periodMonitor = readFT5206TouchRegister(0x89);

  Serial.print("periodMonitor = ");
  Serial.println( periodMonitor, HEX );

  uint8_t  lenLibVersion = readFT5206TouchAddr(0x0a1, buf, 2 );
  if (lenLibVersion) {
    uint16_t libVersion = (buf[0] << 8) | buf[1];
    Serial.print("lib version = ");
    Serial.println( libVersion, HEX);
  }
  else {
    Serial.println("lib version length is zero");
  }

  uint8_t firmwareId = readFT5206TouchRegister( 0xa6 );

  //  Serial.print("firmware ID = ");
  //  Serial.println( firmwareId);

  while (!tft.begin(RA8875_800x480)) {
    Serial.println("RA8875 Not Found!");
    delay(100);
  }

  Serial.println("Found RA8875");

  tft.displayOn(true);
  tft.GPIOX(true);      // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255);

  tft.touchEnable(false);

}


void init_parameters() {
  y_midline = height / 3;
  x_upleft1 = width / 6;
  x_upleft2 = width / 2;
  x_downleft1 = width / 2;
  y_downline = height / 5 * 4;

  input_width =  int(x_downleft1 / 2 * 0.7);
  input_height = int((height - y_midline) / 5 * 0.6);

  button_radius = int(x_upleft1 * 0.8);
}

void init_toggles() {
  int s = int(y_midline * 0.6);
  float w = (x_upleft1 - strokeWeight) * 0.2;
  float h = (y_midline - strokeWeight) * 0.2;
  int side = s * 0.5;

  //  toggles_manm = new Toggles(4);
  fill(RA8875_C1);
  toggles_manm.add_toogle(0, "SP+", x_upleft2 + w + s * 2 + strokeWeight * 5, h, side);
  toggles_manm.add_toogle(1, "SP-", x_upleft2 + w + s * 2 + strokeWeight * 5, h + side + strokeWeight, side);
  toggles_manm.add_toogle(2, "FAN+", x_upleft2 + w + s * 2 + strokeWeight * 6 + side, h, side);
  toggles_manm.add_toogle(3, "FAN-", x_upleft2 + w + s * 2 + strokeWeight * 6 + side, h + side + strokeWeight, side);

  //  toggles_vent = new Toggles(2);
  toggles_vent.add_toogle(0, "FAN+", x_upleft2 + w + s * 2 + strokeWeight * 6 + side, h, side);
  toggles_vent.add_toogle(1, "FAN-", x_upleft2 + w + s * 2 + strokeWeight * 6 + side, h + side + strokeWeight, side);
  h = (y_midline - strokeWeight) * 0.1;
  //  toggles_test = new Toggles(9);
  toggles_test.add_toogle(0, "D1", x_upleft2 + w + s + strokeWeight * 2, y_midline + h, side);
  toggles_test.add_toogle(1, "D2", x_upleft2 + w + s + strokeWeight * 2, y_midline + h + side + strokeWeight, side);
  toggles_test.add_toogle(2, "D3", x_upleft2 + w + s + strokeWeight * 2, y_midline + h + side * 2 + strokeWeight * 2, side);
  toggles_test.add_toogle(3, "P1", x_upleft2 + w + s + strokeWeight * 3 + side, y_midline + h, side);
  toggles_test.add_toogle(4, "P2", x_upleft2 + w + s + strokeWeight * 3 + side, y_midline + h + side + strokeWeight, side);
  toggles_test.add_toogle(5, "T1", x_upleft2 + w + s + strokeWeight * 4 + side * 2, y_midline + h, side);
  toggles_test.add_toogle(6, "T2", x_upleft2 + w + s + strokeWeight * 4 + side * 2, y_midline + h + side + strokeWeight, side);
  toggles_test.add_toogle(7, "T3", x_upleft2 + w + s + strokeWeight * 4 + side * 2, y_midline + h + side * 2 + strokeWeight * 2, side);
  toggles_test.add_toogle(8, "T4", x_upleft2 + w + s + strokeWeight * 4 + side * 2, y_midline + h + side * 3 + strokeWeight * 3, side);

  //  toggles_test_control = new Toggles(2);
  toggles_test_control.add_toogle(0, "+1", x_upleft2 + w + s + strokeWeight * 6 + side * 3, y_midline + h, side);
  toggles_test_control.add_toogle(1, "-1", x_upleft2 + w + s + strokeWeight * 6 + side * 3, y_midline + h + side + strokeWeight, side);
  toggles_test_control.add_toogle(2, "+10", x_upleft2 + w + s + strokeWeight * 7 + side * 4, y_midline + h, side);
  toggles_test_control.add_toogle(3, "-10", x_upleft2 + w + s + strokeWeight * 7 + side * 4, y_midline + h + side + strokeWeight, side);
}
