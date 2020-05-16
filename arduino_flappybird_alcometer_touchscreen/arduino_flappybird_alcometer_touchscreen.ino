#include <Adafruit_GFX.h>   
#include <Adafruit_TFTLCD.h> 
#include <TouchScreen.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;        // hard-wired for UNO shields anyway.
#include <EEPROM.h>       // memory
#if defined(__SAM3X8E__)
#undef __FlashStringHelper::F(string_literal)
#define F(string_literal) string_literal
#endif

#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define MINPRESSURE 10
#define MAXPRESSURE 150000

TSPoint tp;
uint8_t YP = A1;  // must be an analog pin, use "An" notation!
uint8_t XM = A2;  // must be an analog pin, use "An" notation!
uint8_t YM = 7;   // can be a digital pin
uint8_t XP = 6;   // can be a digital pin
int16_t BOXSIZE;
int16_t PENRADIUS = 3;
uint16_t identifier, oldcolor, currentcolor;
//uint16_t TS_LEFT = 950;
//uint16_t TS_RIGHT  = 180;
//uint16_t TS_TOP = 170;
//uint16_t TS_BOT = 880;
uint16_t TS_LEFT = 0;
uint16_t TS_RIGHT  = 1000;
uint16_t TS_TOP = 0;
uint16_t TS_BOT = 1000;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 260);
int wing;
bool running = false;
bool menu = true;
bool scrPress = false; // screen pressed
long nextDrawLoopRunTime;

int fx = 50; // default 50
int fy = 125; //default 125
int fallRate = 0; // default -1
int pillarPos = 200;
int gapheight = 90; // default 90
int gapPos = 60;
bool crashed = false;
int score = 0;
int highScore = 0;
float alkoHiScore;

#include <avr/pgmspace.h>
int Mqpin = 5;
const float muunnos[]PROGMEM {
-12.578,-12.472,-12.367,-12.262,-12.158,-12.054,-11.952,-11.850,-11.748,-11.648,-11.548,-11.448,-11.350,-11.252,-11.155,-11.058,-10.962,-10.867,-10.772,-10.678,-10.585,-10.492,-10.400,-10.309,-10.218,-10.128,-10.039,-9.950,-9.862,-9.774,-9.687,-9.601,-9.515,-9.430,-9.345,-9.261,-9.178,-9.095,-9.013,-8.931,-8.850,-8.770,
-8.690,-8.611,-8.532,-8.454,-8.377,-8.300,-8.223,-8.147,-8.072,-7.997,-7.923,-7.849,-7.776,-7.704,-7.632,-7.560,-7.489,-7.419,-7.349,-7.279,-7.210,-7.142,-7.074,-7.007,-6.940,-6.874,-6.808,-6.742,-6.678,-6.613,-6.549,-6.486,-6.423,-6.361,-6.299,-6.237,-6.176,-6.116,-6.056,-5.996,-5.937,-5.878,-5.820,-5.763,-5.705,-5.648,
-5.592,-5.536,-5.481,-5.426,-5.371,-5.317,-5.263,-5.210,-5.157,-5.104,-5.052,-5.001,-4.949,-4.899,-4.848,-4.798,-4.749,-4.699,-4.651,-4.602,-4.554,-4.507,-4.459,-4.413,-4.366,-4.320,-4.275,-4.229,-4.184,-4.140,-4.096,-4.052,-4.009,-3.966,-3.923,-3.881,-3.839,-3.797,-3.756,-3.715,-3.674,-3.634,-3.594,-3.555,-3.516,-3.477,
-3.438,-3.400,-3.362,-3.325,-3.288,-3.251,-3.214,-3.178,-3.142,-3.107,-3.072,-3.037,-3.002,-2.968,-2.934,-2.900,-2.867,-2.834,-2.801,-2.768,-2.736,-2.704,-2.673,-2.641,-2.610,-2.580,-2.549,-2.519,-2.489,-2.459,-2.430,-2.401,-2.372,-2.344,-2.316,-2.288,-2.260,-2.232,-2.205,-2.178,-2.152,-2.125,-2.099,-2.073,-2.047,-2.022,
-1.997,-1.972,-1.947,-1.923,-1.898,-1.874,-1.851,-1.827,-1.804,-1.781,-1.758,-1.735,-1.713,-1.691,-1.669,-1.647,-1.626,-1.605,-1.584,-1.563,-1.542,-1.522,-1.502,-1.482,-1.462,-1.442,-1.423,-1.404,-1.385,-1.366,-1.347,-1.329,-1.311,-1.293,-1.275,-1.257,-1.240,-1.223,-1.206,-1.189,-1.172,-1.156,-1.139,-1.123,-1.107,-1.091,
-1.076,-1.060,-1.045,-1.030,-1.015,-1.000,-0.986,-0.971,-0.957,-0.943,-0.929,-0.915,-0.901,-0.888,-0.875,-0.861,-0.848,-0.836,-0.823,-0.810,-0.798,-0.785,-0.773,-0.761,-0.749,-0.738,-0.726,-0.715,-0.703,-0.692,-0.681,-0.670,-0.660,-0.649,-0.638,-0.628,-0.618,-0.608,-0.598,-0.588,-0.578,-0.568,-0.559,-0.549,-0.540,-0.531,
-0.522,-0.513,-0.504,-0.495,-0.487,-0.478,-0.470,-0.462,-0.454,-0.446,-0.438,-0.430,-0.422,-0.414,-0.407,-0.400,-0.392,-0.385,-0.378,-0.371,-0.364,-0.357,-0.350,-0.344,-0.337,-0.331,-0.324,-0.318,-0.312,-0.306,-0.300,-0.294,-0.288,-0.282,-0.276,-0.271,-0.265,-0.260,-0.254,-0.249,-0.244,-0.239,-0.234,-0.229,-0.224,-0.219,
-0.214,-0.209,-0.205,-0.200,-0.196,-0.191,-0.187,-0.183,-0.179,-0.174,-0.170,-0.166,-0.162,-0.159,-0.155,-0.151,-0.147,-0.144,-0.140,-0.136,-0.133,-0.130,-0.126,-0.123,-0.120,-0.117,-0.113,-0.110,-0.107,-0.104,-0.101,-0.099,-0.096,-0.093,-0.090,-0.088,-0.085,-0.082,-0.080,-0.077,-0.075,-0.072,-0.070,-0.068,-0.066,-0.063,
-0.061,-0.059,-0.057,-0.055,-0.053,-0.051,-0.049,-0.047,-0.045,-0.043,-0.041,-0.040,-0.038,-0.036,-0.035,-0.033,-0.031,-0.030,-0.028,-0.027,-0.025,-0.024,-0.022,-0.021,-0.020,-0.018,-0.017,-0.016,-0.015,-0.013,-0.012,-0.011,-0.010,-0.009,-0.008,-0.007,-0.006,-0.005,-0.004,-0.003,-0.002,-0.001,0.000,0.001,0.002,0.002,0.003,
0.004,0.005,0.006,0.006,0.007,0.008,0.008,0.009,0.010,0.010,0.011,0.011,0.012,0.013,0.013,0.014,0.014,0.015,0.015,0.016,0.016,0.017,0.017,0.017,0.018,0.018,0.019,0.019,0.019,0.020,0.020,0.020,0.021,0.021,0.021,0.022,0.022,0.022,0.022,0.023,0.023,0.023,0.023,0.024,0.024,0.024,0.024,0.024,0.024,0.025,0.025,0.025,0.025,0.025,
0.025,0.026,0.026,0.026,0.026,0.026,0.026,0.026,0.026,0.027,0.027,0.027,0.027,0.027,0.027,0.027,0.027,0.027,0.027,0.027,0.027,0.027,0.028,0.028,0.028,0.028,0.028,0.028,0.028,0.028,0.028,0.028,0.028,0.028,0.028,0.028,0.028,0.028,0.028,0.028,0.028,0.028,0.029,0.029,0.029,0.029,0.029,0.029,0.029,0.029,0.029,0.029,0.029,0.029,
0.029,0.029,0.029,0.029,0.029,0.029,0.029,0.029,0.029,0.029,0.029,0.030,0.030,0.030,0.030,0.030,0.030,0.030,0.030,0.030,0.030,0.030,0.030,0.030,0.030,0.030,0.030,0.031,0.031,0.031,0.031,0.031,0.031,0.031,0.031,0.031,0.031,0.031,0.031,0.032,0.032,0.032,0.032,0.032,0.032,0.032,0.032,0.032,0.032,0.033,0.033,0.033,0.033,0.033,
0.033,0.033,0.033,0.033,0.034,0.034,0.034,0.034,0.034,0.034,0.034,0.035,0.035,0.035,0.035,0.035,0.035,0.035,0.035,0.036,0.036,0.036,0.036,0.036,0.036,0.037,0.037,0.037,0.037,0.037,0.037,0.037,0.038,0.038,0.038,0.038,0.038,0.038,0.039,0.039,0.039,0.039,0.039,0.039,0.040,0.040,0.040,0.040,0.040,0.040,0.041,0.041,0.041,0.041,
0.041,0.041,0.042,0.042,0.042,0.042,0.042,0.043,0.043,0.043,0.043,0.043,0.043,0.044,0.044,0.044,0.044,0.044,0.044,0.045,0.045,0.045,0.045,0.045,0.046,0.046,0.046,0.046,0.046,0.046,0.047,0.047,0.047,0.047,0.047,0.048,0.048,0.048,0.048,0.048,0.048,0.049,0.049,0.049,0.049,0.049,0.050,0.050,0.050,0.050,0.050,0.050,0.051,0.051,
0.051,0.051,0.051,0.052,0.052,0.052,0.052,0.052,0.052,0.053,0.053,0.053,0.053,0.053,0.054,0.054,0.054,0.054,0.054,0.054,0.055,0.055,0.055,0.055,0.055,0.056,0.056,0.056,0.056,0.056,0.057,0.057,0.057,0.057,0.058,0.058,0.058,0.058,0.058,0.059,0.059,0.059,0.059,0.060,0.060,0.060,0.060,0.061,0.061,0.061,0.061,0.062,0.062,0.062,
0.062,0.063,0.063,0.063,0.064,0.064,0.064,0.064,0.065,0.065,0.065,0.066,0.066,0.066,0.067,0.067,0.067,0.068,0.068,0.069,0.069,0.069,0.070,0.070,0.071,0.071,0.071,0.072,0.072,0.073,0.073,0.074,0.074,0.075,0.075,0.076,0.076,0.077,0.077,0.078,0.078,0.079,0.080,0.080,0.081,0.081,0.082,0.083,0.083,0.084,0.085,0.085,0.086,0.087,
0.088,0.088,0.089,0.090,0.091,0.092,0.093,0.093,0.094,0.095,0.096,0.097,0.098,0.099,0.100,0.101,0.102,0.103,0.104,0.105,0.106,0.107,0.109,0.110,0.111,0.112,0.113,0.115,0.116,0.117,0.119,0.120,0.121,0.123,0.124,0.126,0.127,0.129,0.130,0.132,0.134,0.135,0.137,0.139,0.140,0.142,0.144,0.146,0.148,0.150,0.151,0.153,0.155,0.157,
0.160,0.162,0.164,0.166,0.168,0.170,0.173,0.175,0.177,0.180,0.182,0.185,0.187,0.190,0.193,0.195,0.198,0.201,0.204,0.206,0.209,0.212,0.215,0.218,0.221,0.225,0.228,0.231,0.234,0.238,0.241,0.245,0.248,0.252,0.255,0.259,0.263,0.266,0.270,0.274,0.278,0.282,0.286,0.290,0.295,0.299,0.303,0.308,0.312,0.317,0.321,0.326,0.331,0.335,
0.340,0.345,0.350,0.355,0.360,0.366,0.371,0.376,0.382,0.387,0.393,0.399,0.404,0.410,0.416,0.422,0.428,0.435,0.441,0.447,0.454,0.460,0.467,0.473,0.480,0.487,0.494,0.501,0.508,0.516,0.523,0.530,0.538,0.545,0.553,0.561,0.569,0.577,0.585,0.593,0.602,0.610,0.619,0.628,0.636,0.645,0.654,0.663,0.673,0.682,0.691,0.701,0.711,0.720,
0.730,0.740,0.751,0.761,0.771,0.782,0.793,0.803,0.814,0.825,0.837,0.848,0.859,0.871,0.883,0.895,0.907,0.919,0.931,0.944,0.956,0.969,0.982,0.995,1.008,1.021,1.035,1.048,1.062,1.076,1.090,1.104,1.119,1.133,1.148,1.163,1.178,1.193,1.208,1.224,1.240,1.256,1.272,1.288,1.304,1.321,1.338,1.354,1.372,1.389,1.406,1.424,1.442,1.460,
1.478,1.497,1.515,1.534,1.553,1.572,1.592,1.611,1.631,1.651,1.671,1.692,1.712,1.733,1.754,1.775,1.797,1.818,1.830
};

void setup(void){
    uint16_t tmp;
    tft.begin(9600);
    tft.reset();
    identifier = tft.readID();
    Serial.begin(9600);
    tft.begin(identifier);
    tft.setRotation(3);
    BOXSIZE = tft.width() / 2;
    highScore = EEPROM.read(0);
    EEPROM.get(1, alkoHiScore);
    drawMenu();
    startGame();
}

void drawMenu() {
    BOXSIZE = tft.width() / 6;
    tft.fillScreen(RED);
             // (x start, y start, x end, y end, color)
    tft.fillRect(0,         0,     400,     100, BLACK);
          // (x start, y start, x end, height, color)
    tft.drawRect(0,  100,          400,   170,  BLACK);
    tft.drawRect(1,  101,          398,   168,  BLACK);
    //tft.drawRect(0,  170,          400,    70,  BLACK);
    //tft.drawRect(1,  171,          398,    68,  BLACK);
    tft.drawRect(200,  100,        200,   170,  BLACK);
    tft.drawRect(201,  101,        198,   168,  BLACK);
    
    tft.setTextColor(WHITE);
    tft.setTextSize(3);
    tft.setCursor(8, 125);
    tft.print("Henkiarvio");
    tft.setCursor(217, 125);
    tft.print("Laepsy-");
    tft.setCursor(217, 155);
    tft.print("lintu");
    tft.setTextSize(2);
    tft.setTextColor(BLACK);
    tft.setCursor(5, 200);
    tft.print("Pohjat: ");
    tft.print(alkoHiScore);// high score for alcometre
    tft.print("%.");
    tft.setCursor(217, 200);
    tft.print("Pohjat: ");
    tft.print(highScore); // high score for flappy bird
}

void startGame() {
  if (menu == false){
    tft.fillScreen(BLUE); 
    int ty = 230;
    for (int tx = 0; tx <= 400; tx +=20) {
      tft.fillTriangle(tx,ty, tx+10,ty, tx,ty+10, GREEN);
      tft.fillTriangle(tx+10,ty+10, tx+10,ty, tx,ty+10, YELLOW);
      tft.fillTriangle(tx+10,ty, tx+20,ty, tx+10,ty+10, YELLOW);
      tft.fillTriangle(tx+20,ty+10, tx+20,ty, tx+10,ty+10, GREEN);
    }
    tft.setTextColor(BLACK); tft.setTextSize(1);
    tft.setCursor(5, 231);
    tft.print("HIGH-SCORE: ");
    tft.print(highScore);
    pillarPos = 200;
  }
}
void drawLoop() {
  // clear moving items
  clearPillar(pillarPos, gapPos);
  clearFlappy(fx, fy);

  // move items
  if (running) {
    fy += fallRate;
    fallRate++;
    pillarPos -= 5;
    if (pillarPos == 0) {
      score++;
    } else if (pillarPos < -50) {
      pillarPos = 400;
      gapPos = random(20, 120);
    }
  }
  // draw moving items & animate
  drawPillar(pillarPos, gapPos);
  drawFlappy(fx, fy);
}
void loop(void) {
  uint16_t xpos, ypos;  //screen coordinates
  TSPoint p = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  pinMode(XP, OUTPUT);
  pinMode(YM, OUTPUT);
  if (menu) {
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE) { // exit menu
      xpos = map(p.x, TS_LEFT, TS_RIGHT, tft.width(), 0);
      ypos = map(p.y, TS_TOP, TS_BOT, tft.height(), 0);
      if (xpos < 125 && ypos > 90) {// start flappy
        menu = false;
        startGame();
      }
      else if (xpos < 125 && ypos < 90) {// analyze breath
        alcomeasure();
      }
    }
  }
  else{
    if (millis() > nextDrawLoopRunTime) {
      drawLoop();
      checkCollision();
      nextDrawLoopRunTime += 50;
    }
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
      if (!running) {
        // clear text & start scrolling
        tft.fillRect(0,0,400,80,BLUE);
        running = true;
        //startGame();
        
      } else {
        // fly up
        fallRate = -5; // default -11
        scrPress = true;
      }
    } else if (p.z == 0 && scrPress) {
      scrPress = false;
    }
  }
}

void alcomeasure(){
  gapheight = 90;
  tft.fillRect(0, 0,400,100, BLACK); // clear screen
  tft.setTextColor(WHITE);
  tft.setTextSize(5);
  tft.setCursor(100, 20);
  tft.print("Puhalla!");
  tft.setTextSize(2);
  tft.setCursor(90, 75);
  tft.print("Arvioidaan henkea...");
  
  int Alcovalue;
  int maxvalue = 407;
  for(int i = 0; i < 140; i++){
     Alcovalue = analogRead(Mqpin);
     if (Alcovalue > maxvalue){
        maxvalue = Alcovalue;
     }
     delay(50);
  }
  tft.fillRect(0, 0, 400, 100, BLACK); // clear screen
  //float& value = maxvalue; 
  Serial.println(maxvalue);
  
  float converted = pgm_read_float(&muunnos[maxvalue]);
  //Serial.println(converted, 2);
  
  if (converted > alkoHiScore){
    alkoHiScore = converted;
    EEPROM.put(1, alkoHiScore);
  }
  tft.setTextColor(WHITE);
  tft.setTextSize(5);
  tft.setCursor(100, 20);
  tft.print(converted, 2);
  tft.setCursor(250, 20);
  tft.print("%.");
  gapheight += static_cast<int>(converted*100);
}

void checkCollision() {
  // Collision with ground
  if (fy > 206) crashed = true;
  
  // Collision with pillar
  if (fx + 34 > pillarPos && fx < pillarPos + 50)
    if (fy < gapPos || fy + 24 > gapPos + gapheight)
      crashed = true;
  
  if (crashed) {
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.setCursor(75, 75);
    tft.print("Havisit pelin!");
    tft.setCursor(75, 125);
    tft.print("Tulos:");
    tft.setCursor(150, 125);
    tft.print(score);
    delay(700);
    tft.setCursor(20, 175);
    tft.print("Takasin menuun...");
    delay(700);
    tft.setCursor(275, 175);
    tft.print("3");
    delay(700);
    tft.setCursor(300, 175);
    tft.print("2");
    delay(700);
    tft.setCursor(325, 175);
    tft.print("1");
    delay(700);
    
    if (score > highScore){
      highScore = score;
      EEPROM.write(0, highScore);
    }
    fx = 50; fy = 125; fallRate = 0;
    pillarPos = 400; gapPos = 60;
    crashed = false; score = 0;
    running = false;
    menu = true;
    drawMenu();
    startGame();
  }
}

void drawBorder () {
  uint16_t width = tft.width() - 1;
  uint16_t height = tft.height() - 1;
  uint8_t border = 10;
  tft.fillScreen(BLUE);
  tft.fillRect(border, border, (width - border * 2), (height - border * 2), WHITE);
}

void drawFlappy(int x, int y) {
  // Upper & lower body
  tft.fillRect(x+2, y+8, 2, 10, BLACK);
  tft.fillRect(x+4, y+6, 2, 2, BLACK);
  tft.fillRect(x+6, y+4, 2, 2, BLACK);
  tft.fillRect(x+8, y+2, 4, 2, BLACK);
  tft.fillRect(x+12, y, 12, 2, BLACK);
  tft.fillRect(x+24, y+2, 2, 2, BLACK);
  tft.fillRect(x+26, y+4, 2, 2, BLACK);
  tft.fillRect(x+28, y+6, 2, 6, BLACK);
  tft.fillRect(x+10, y+22, 10, 2, BLACK);
  tft.fillRect(x+4, y+18, 2, 2, BLACK);
  tft.fillRect(x+6, y+20, 4, 2, BLACK);
  
  // Body fill
  tft.fillRect(x+12, y+2, 6, 2, YELLOW);
  tft.fillRect(x+8, y+4, 8, 2, YELLOW);
  tft.fillRect(x+6, y+6, 10, 2, YELLOW);
  tft.fillRect(x+4, y+8, 12, 2, YELLOW);
  tft.fillRect(x+4, y+10, 14, 2, YELLOW);
  tft.fillRect(x+4, y+12, 16, 2, YELLOW);
  tft.fillRect(x+4, y+14, 14, 2, YELLOW);
  tft.fillRect(x+4, y+16, 12, 2, YELLOW);
  tft.fillRect(x+6, y+18, 12, 2, YELLOW);
  tft.fillRect(x+10, y+20, 10, 2, YELLOW);
  
  // Eye
  tft.fillRect(x+18, y+2, 2, 2, BLACK);
  tft.fillRect(x+16, y+4, 2, 6, BLACK);
  tft.fillRect(x+18, y+10, 2, 2, BLACK);
  tft.fillRect(x+18, y+4, 2, 6, WHITE);
  tft.fillRect(x+20, y+2, 4, 10, WHITE);
  tft.fillRect(x+24, y+4, 2, 8, WHITE);
  tft.fillRect(x+26, y+6, 2, 6, WHITE);
  tft.fillRect(x+24, y+6, 2, 4, BLACK);
  
  // Beak
  tft.fillRect(x+20, y+12, 12, 2, BLACK);
  tft.fillRect(x+18, y+14, 2, 2, BLACK);
  tft.fillRect(x+20, y+14, 12, 2, RED);
  tft.fillRect(x+32, y+14, 2, 2, BLACK);
  tft.fillRect(x+16, y+16, 2, 2, BLACK);
  tft.fillRect(x+18, y+16, 2, 2, RED);
  tft.fillRect(x+20, y+16, 12, 2, BLACK);
  tft.fillRect(x+18, y+18, 2, 2, BLACK);
  tft.fillRect(x+20, y+18, 10, 2, RED);
  tft.fillRect(x+30, y+18, 2, 2, BLACK);
  tft.fillRect(x+20, y+20, 10, 2, BLACK);
}

void drawPillar(int x, int gapPosition) {
  // upper pillar fill with green
           // (x start, y start, x end,         y end,  color)
  tft.fillRect(x+2,           2,    46, gapPosition-4,  GREEN);

  // lower pillar fill with green
  tft.fillRect(x+2, gapPosition+gapheight+2, 46, 226-gapheight-gapPosition, GREEN);

  // write score in pillar
  if (x < 350){
    tft.setTextColor(BLACK);
    tft.setTextSize(1);
    tft.setCursor(x+5, gapPosition-25);
    tft.print("Tulos: ");
    tft.setCursor(x+20, gapPosition-15);
    tft.print(score);
  }

  // draw a double rectangle with black as the edge of the pillar
  tft.drawRect(x,0,50,gapPosition,BLACK);     // upper
  tft.drawRect(x+1,1,48,gapPosition-2,BLACK);
  tft.drawRect(x, gapPosition+gapheight, 50, 230-gapheight-gapPosition, BLACK); // lower
  tft.drawRect(x+1,gapPosition+gapheight+1 ,48, 228-gapheight-gapPosition, BLACK);
}

void clearPillar(int x, int gapPosition) {
  tft.fillRect(x+45, 0, 5, gapPosition, BLUE);
  tft.fillRect(x+45, gapPosition+gapheight, 5, 230-gapheight-gapPosition, BLUE);
}

void clearFlappy(int x, int y) {
 tft.fillRect(x, y, 34, 24, BLUE); 
}
