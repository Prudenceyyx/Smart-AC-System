
void check_toggles() {
  toggles_manm.check_toggle();
  toggles_vent.check_toggle();
  toggles_test.check_toggle();
  toggles_test_control.check_toggle();
}
void draw_toggles() {
  toggles_manm.draw();
  toggles_vent.draw();
  toggles_test.draw();
  toggles_test_control.draw();
}

void touch_event() {
  attention = digitalRead(FT5206_INT);
  /* Wait around for touch events */
  if (!attention && oldAttention ) {

    uint8_t count = readFT5206TouchLocation( touchLocations, 1 );

    if (count) {
      thisTouchTime = millis();
      static TouchLocation lastTouch = touchLocations[0];

      //If no touch: clear up
      if (((thisTouchTime - lastTouchTime) > 1000) && sameLoc( touchLocations[0], lastTouch ) )
      {
        lastTouchTime = thisTouchTime;
      }

      if ( (thisTouchTime - lastTouchTime) > 50
           && (dist(touchLocations[0], lastTouch) >= 15)
         ) {
        //      Serial.print("Time delta = ");
        //      Serial.print(thisTouchTime - lastTouchTime);
        //      Serial.print(", dist = ");
        //      Serial.println( dist(touchLocations[0], lastTouch) );
        lastTouch = touchLocations[0];

        Serial.print("Locations: ");
        Serial.print(touchLocations[0].x);
        Serial.print(", ");
        Serial.print(touchLocations[0].y);
        Serial.println(".");

        display_system->check_button();
        check_toggles();
      }
    }
  }

  oldAttention = attention;
}
