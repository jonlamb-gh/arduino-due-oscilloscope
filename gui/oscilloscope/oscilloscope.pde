/**
 * @file oscilloscope.pde
 * @brief Oscilloscope GUI Application.
 *
 * Processing: 3.4
 *
 */

import processing.serial.*;

Serial port;
int val[];
int channels = 5;
int[][] values;
int[] palette = new int[]{#ffffff,#ff0000,#00ff00,#0000ff,#888800,#888888};
void setup() 
{
  size(640, 480);
  String[] ttys = Serial.list();
  println("BORKLET: " + ttys.length);
  port = new Serial(this, ttys[0], 115200);
  values = new int[channels][width];
  val=new int[channels];
  smooth();
}

int getY(int val) {
  return (int)(val / 1023.0f * height) - 1;
}
void draw()
{
  
  while (port.available() >= 4) {
    if (port.read() == 0xff) {
      int channel =port.read();
      //val[channel] = (port.read() << 8) | (port.read());
      val[channel] = port.read() | (port.read() << 8);
    }
  }
  background(0);
    
  for (int c=0; c< channels; c++){
    int delta = values[c][width-1] - val[c];
    for (int i=0; i<width-1; i++)
      values[c][i] = values[c][i+1];
    values[c][width-1] = val[c];
    
    stroke(palette[c]);
    
    text("C"+c+": "+val[c], 15,20+30*c,150,70);
    text("D: "+ delta, 150,20+30*c,150,70);
    for (int x=1; x<width; x++) {
      line(width-x,   height-1-getY(values[c][x-1]), 
           width-1-x, height-1-getY(values[c][x]));
    }
  }
}
