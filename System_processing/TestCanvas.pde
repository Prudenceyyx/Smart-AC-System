color b = #4A89DC;
color m = #37BC9B;
color color_bk = #CCD1D9; //grey
color color_line = #AAB2BD;//dark grey
color color_border = #656D78;

int strokeWeight=5;

int y_midline;
int x_upleft1, x_upleft2;
int x_downleft1;
int y_downline;

int input_width;
int input_height;

int button_radius;


void init_parameters() {
  y_midline = height/3;
  x_upleft1 = width/6;
  x_upleft2 = width/2;
  x_downleft1 = width/2;
  y_downline = height/5*4;

  input_width =  int(x_downleft1/2*0.7);
  input_height = int((height-y_midline)/5*0.6);

  button_radius = int(x_upleft1*0.8);
}



void draw_background() {
  background(color_bk);
  stroke(color_line);
  strokeWeight(strokeWeight);
  noFill();
  rectMode(CORNERS);
  rect(0, 0, x_upleft1, y_midline);
  rect(x_upleft1, 0, x_upleft2, y_midline);
  rect(x_upleft2, 0, width, y_midline);
  rect(0, y_midline, x_downleft1, height-strokeWeight);
  rect(x_downleft1, y_midline, width, y_downline);
  rect(x_downleft1, y_downline, width, height-strokeWeight);

  fill(50);
  textSize(22);

  text("T1", (x_downleft1/2-input_width)/2, x_downleft1/2-20);
  text("T2", (x_downleft1/2-input_width)/2, x_downleft1/2-20+(height - y_midline)/5);
  text("T3", (x_downleft1/2-input_width)/2, x_downleft1/2-20+(height - y_midline)/5*2);
  text("T4", (x_downleft1/2-input_width)/2, x_downleft1/2-20+(height - y_midline)/5*3);

  text("P1", (x_downleft1/2-input_width)/2+x_downleft1/2-20, x_downleft1/2-20);
  text("P2", (x_downleft1/2-input_width)/2+x_downleft1/2-20, x_downleft1/2-20+(height - y_midline)/5);
  text("DP1", (x_downleft1/2-input_width)/2+x_downleft1/2-20, x_downleft1/2-20+(height - y_midline)/5*2);
  text("DP2", (x_downleft1/2-input_width)/2+x_downleft1/2-20, x_downleft1/2-20+(height - y_midline)/5*3);
  text("DP3", (x_downleft1/2-input_width)/2+x_downleft1/2-20, x_downleft1/2-20+(height - y_midline)/5*4);

  text("SP: ", x_upleft1+button_radius+20, y_midline/2);
}


void display_inputfield() {
  rectMode(CORNER);
  stroke(color_border);
  strokeWeight(2);
  for (int i = 0; i < 4; i++) {
    int leftx = (x_downleft1/2-input_width)/2;
    int lefty = y_midline+((height - y_midline)/5-input_height)/2+(height - y_midline)/5*i;
    rect(leftx, lefty, input_width, input_height);
  }
  for (int i = 0; i < 5; i++) {
    int leftx = (x_downleft1/2-input_width)/2 + x_downleft1/2;
    int lefty = y_midline+((height - y_midline)/5-input_height)/2+(height - y_midline)/5*i;
    rect(leftx, lefty, input_width, input_height);
  }
}

void display_button() {
  ellipseMode(CENTER);
  noStroke();
  fill(b);
  ellipse(x_upleft1/2, y_midline/2, button_radius, button_radius);
  ellipse(x_upleft1/2+x_upleft1, y_midline/2, button_radius, button_radius);
  ellipse(x_upleft1/2+x_upleft2, y_midline/2, button_radius, button_radius);
  ellipse(x_upleft1/2+x_downleft1, y_midline/2+y_midline, button_radius, button_radius);
}