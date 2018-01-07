void draw_background() {
  tft.graphicsMode();
  tft.fillScreen(RA8875_LIGHTGREY);

  //Background Area
  fill(RA8875_WHITE);
  rect(strokeWeight, strokeWeight, x_upleft1, y_midline);
  rect(x_upleft1 + strokeWeight, strokeWeight, x_upleft2, y_midline);
  rect(x_upleft2 + strokeWeight, strokeWeight, width - strokeWeight, y_midline);
  rect(strokeWeight, y_midline + strokeWeight, x_downleft1, height - strokeWeight);
  rect(x_downleft1 + strokeWeight, y_midline + strokeWeight, width - strokeWeight, y_downline);
  rect(x_downleft1 + strokeWeight, y_downline + strokeWeight, width - strokeWeight, height - strokeWeight);

  //Input Value Area
  tft.textMode();
  textFill(RA8875_BLACK, RA8875_WHITE);
  text("T1", (x_downleft1 / 2 - input_width) / 2, x_downleft1 / 2 - 20);
  text("T2", (x_downleft1 / 2 - input_width) / 2, x_downleft1 / 2 - 20 + (height - y_midline) / 5);
  text("T3", (x_downleft1 / 2 - input_width) / 2, x_downleft1 / 2 - 20 + (height - y_midline) / 5 * 2);
  text("T4", (x_downleft1 / 2 - input_width) / 2, x_downleft1 / 2 - 20 + (height - y_midline) / 5 * 3);

  text("P1", (x_downleft1 / 2 - input_width) / 2 + x_downleft1 / 2 - 20, x_downleft1 / 2 - 20);
  text("P2", (x_downleft1 / 2 - input_width) / 2 + x_downleft1 / 2 - 20, x_downleft1 / 2 - 20 + (height - y_midline) / 5);
  text("DP1", (x_downleft1 / 2 - input_width) / 2 + x_downleft1 / 2 - 20, x_downleft1 / 2 - 20 + (height - y_midline) / 5 * 2);
  text("DP2", (x_downleft1 / 2 - input_width) / 2 + x_downleft1 / 2 - 20, x_downleft1 / 2 - 20 + (height - y_midline) / 5 * 3);
  text("DP3", (x_downleft1 / 2 - input_width) / 2 + x_downleft1 / 2 - 20, x_downleft1 / 2 - 20 + (height - y_midline) / 5 * 4);

  text("SP: ", x_upleft1 + button_radius + 20, y_midline / 2);


  display_system->draw();
  draw_toggles();

}


//Draw round rectange in corner mode
void fill(uint16_t newcolor) {
  color = newcolor;
}

void textFill(uint16_t newcolor, uint16_t bgcolor) {
  textColor = newcolor;
  backColor = bgcolor;
}

void rect(int x1, int y1, int x2, int y2) {
  tft.fillRoundRect(x1, y1, x2 - x1, y2 - y1, 10, color);
}

void text(const char* content, float x, float y) {
  tft.textMode();
  tft.textSetCursor(int(x), int(y));
  tft.textColor(textColor, backColor);
  tft.textWrite(content);

}

void display_input() {

  //T1
  text(T1_value, (x_downleft1 / 2 - input_width) / 2 + 40, x_downleft1 / 2 - 20);
  //   text(String(T1_value), (x_downleft1 / 2 - input_width) / 2 + 40, x_downleft1 / 2 - 20);
  text(T2_value, (x_downleft1 / 2 - input_width) / 2 + 40, x_downleft1 / 2 - 20 + (height - y_midline) / 5);
  text(T3_value, (x_downleft1 / 2 - input_width) / 2 + 40, x_downleft1 / 2 - 20 + (height - y_midline) / 5 * 2);
  text(T4_value, (x_downleft1 / 2 - input_width) / 2 + 40, x_downleft1 / 2 - 20 + (height - y_midline) / 5 * 3);

  //  //DP1
  text(P1_value, (x_downleft1 / 2 - input_width) / 2 + x_downleft1 / 2 + 40, x_downleft1 / 2 - 20);
  text(P2_value, (x_downleft1 / 2 - input_width) / 2 + x_downleft1 / 2 + 40, x_downleft1 / 2 - 20 + (height - y_midline) / 5);
  text(DP1_value, (x_downleft1 / 2 - input_width) / 2 + x_downleft1 / 2 + 40, x_downleft1 / 2 - 20 + (height - y_midline) / 5 * 2);
  text(DP2_value, (x_downleft1 / 2 - input_width) / 2 + x_downleft1 / 2 + 40, x_downleft1 / 2 - 20 + (height - y_midline) / 5 * 3);
  text(DP3_value, (x_downleft1 / 2 - input_width) / 2 + x_downleft1 / 2 + 40, x_downleft1 / 2 - 20 + (height - y_midline) / 5 * 4);

  text(set_point, x_upleft1 + button_radius + 50, y_midline / 2);
  //
  textFill(RA8875_RED, RA8875_WHITE);
  if (OverTemp) {
    text("OverTemp", x_downleft1 + 20, y_downline + 5);
  } else {
    fill(RA8875_WHITE);
    //    rect(strokeWeight, strokeWeight, x_upleft1, y_midline);
    tft.fillRoundRect( x_downleft1 + 20, y_downline + 5, 100, 20, 10, RA8875_WHITE);
  }
  
  if (OverPressure) {
    text("OverPressure", x_downleft1 + 20, y_downline + 5 + 20 * 1);
  } else {
    fill(RA8875_WHITE);
    tft.fillRoundRect(  x_downleft1 + 20, y_downline + 5 + 20 * 1, 100, 20, 10, RA8875_WHITE);
  }
  
  if (Leaking) {
    text("Leaking", x_downleft1 + 20, y_downline + 5 + 20 * 2);
  } else {
    fill(RA8875_WHITE);
    tft.fillRoundRect(  x_downleft1 + 20, y_downline + 5 + 20 * 2, 100, 20, 10, RA8875_WHITE);
  }
  
  if (HX_Clogged) {
    text("HX_Clogged", x_downleft1 + 20, y_downline + 5 + 20 * 3);
  } else {
    fill(RA8875_WHITE);
    tft.fillRoundRect(  x_downleft1 + 20, y_downline + 5 + 20 * 3, 100, 20, 10, RA8875_WHITE);
  }
}

