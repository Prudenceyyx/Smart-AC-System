void init_fsm() {
  states[0] = &state_stdb;
  states[1] = &state_auto;
  states[2] = &state_manm;
  states[3] = &state_vent;
  states[4] = &state_test;

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      if (i == j) continue;
      fsm.add_transition(states[j], states[i], i, NULL);
    }
  }
}

void timerCallback(Timer* timer) {
  /* Print some values of this timer */
  //  Serial.print("Timer call -> Action: ");
  //  Serial.print(timer->getAction());
  //  Serial.print(", Current Time: ");
  //  Serial.println(millis());
  int t = timer->getAction();
  int state = t / 10;
  int function = t % 10;
  if (state == MANM) {
    toggles_manm.setOff(function);
  } else if (state == VENT) {
    toggles_vent.setOff(function);
  } else if (state == TEST) {
    toggles_test_control.setOff(function);
  }
}



void init_timer() {
  stensTimer = StensTimer::getInstance();
  stensTimer->setStaticCallback(timerCallback);

}


void state0() {
  Serial.println("State 0");
}
void state1() {
  Serial.println("State 1");
}

void state2() {
  Serial.println("State 2");
  toggles_manm.show();
}

void state2_leave() {
  Serial.println("State 2 Leaving");
  toggles_manm.hide();
}

void state3() {
  Serial.println("State 3");
  toggles_vent.show();
}

void state3_leave() {
  Serial.println("State 3 Leaving");
  toggles_vent.hide();

}

void state4() {
  Serial.println("State 4");
  toggles_test.show();
  toggles_test_control.show();
}

void state4_leave() {
  Serial.println("State 4 Leaving");
  toggles_test.hide();
  toggles_test_control.hide();

}


