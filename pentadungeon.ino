#include <FastLED.h>
#include "Enemy.h"
#include "Hero.h"


#define LED_PIN     5
#define NUM_LEDS    51
#define BRIGHTNESS  10
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];


CRGB heroColor = CRGB(102, 255, 0);
CRGB heroBlastColor = CRGB(0, 85, 255);


CRGB enemyColor = CRGB(255, 0, 0);
CRGB enemyBlastColor = CRGB(200, 50, 5);

CRGB goldLootColor = CRGB::Yellow;
CRGB diamondLootColor = CRGB::Cyan;
CRGB portalColor = CRGB(255, 42, 127);


int heroPos;  //current position of the player

// Arduino pin numbers
const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output

int TopMoveThreshold = 510;
int BottomMoveThreshold = 490;


int STAGE=1;
int S1_start=0;
int S2_start=9;
int S3_start=15;
int S4_start=21;
int S4_end=25;
int S5_start=26;
int S6_start=29;
int S7_start=33;
int S8_start=38;
int S9_start=44;
int S9_end=50;

int Portal = S4_end;
int enemySpeed = 1;

Enemy e = Enemy();
Enemy e2 = Enemy();


Hero hero = Hero();

void setup() {
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  
  FastLED.addLeds<LED_TYPE, LED_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
  
  heroPos=S1_start;
  
  e.Spawn(S2_start+2, S2_start, S3_start, 1, enemySpeed);
  e2.Spawn(S8_start, S8_start, S9_start, -1, enemySpeed);

  hero.Spawn(0);
  
  Serial.begin(115200);
  
}

bool inRange(int val, int minimum, int maximum)
{
  return ((minimum <= val) && (val < maximum));
}

void loop() {  
  FastLED.clear();

  int X_pos = analogRead(X_pin);
  delay(10);
  int Y_pos = analogRead(Y_pin);
  delay(10);
  int attack = digitalRead(SW_pin);
  delay(10);

  if(attack==0 && !hero._is_attacking){
    hero.Attack();
  }

  if(attack==1 && !hero._is_attacking && STAGE==1){
    
    // bottom to up
    if(inRange(hero._pos, S1_start, S2_start) && X_pos > TopMoveThreshold) {
      hero._pos+=1;
    }
    
    if(inRange(hero._pos, S1_start, S2_start+1) && X_pos < BottomMoveThreshold) {
      hero._pos-=1;
    }
    
    // left to right
    if(inRange(hero._pos, S2_start, S3_start) && Y_pos > TopMoveThreshold) {
      hero._pos+=1;
    }
    
    if(inRange(hero._pos, S2_start+1, S3_start+1) && Y_pos < BottomMoveThreshold) {
      hero._pos-=1;
    }
    
    
    //bottom to down
    if(inRange(hero._pos, S3_start, S4_start) && X_pos < BottomMoveThreshold) {
      hero._pos+=1;
    }
    
    if(inRange(hero._pos, S3_start+1, S4_start+1) && X_pos > TopMoveThreshold) {
      hero._pos-=1;
    }
    
    //right to left
    if(inRange(hero._pos, S4_start, S5_start) && Y_pos < BottomMoveThreshold) {
      hero._pos+=1;
    }
    
    if(inRange(hero._pos, S4_start+1, S5_start+1) && Y_pos > TopMoveThreshold) {
      hero._pos-=1;
    }

  }
  
  
  //levevl 2
  if(attack==1 && !hero._is_attacking && STAGE==2){
    
    //left to right
    if(inRange(hero._pos, S5_start, S6_start) && Y_pos > TopMoveThreshold) {
      hero._pos+=1;
    }
    
    if(inRange(hero._pos, S5_start+1, S6_start+1) && Y_pos < BottomMoveThreshold) {
      hero._pos-=1;
    }
    
    // bottom to up
    if(inRange(hero._pos, S6_start, S7_start) && X_pos > TopMoveThreshold) {
      hero._pos+=1;
    }
    
    if(inRange(hero._pos, S6_start+1, S7_start+1) && X_pos < BottomMoveThreshold) {
      hero._pos-=1;
    }
    
    //right to left
    if(inRange(hero._pos, S7_start, S8_start) && Y_pos < BottomMoveThreshold) {
      hero._pos+=1;
    }
    
    if(inRange(hero._pos, S7_start+1, S8_start+1) && Y_pos > TopMoveThreshold) {
      hero._pos-=1;
    }
    
    // up to bottom
    if(inRange(hero._pos, S8_start, S9_start) && X_pos < BottomMoveThreshold) {
      hero._pos+=1;
    }
    
    if(inRange(hero._pos, S8_start+1, S9_start+1) && X_pos > TopMoveThreshold) {
      hero._pos-=1;
    }
    
    //left to right
    if(inRange(hero._pos, S9_start, S9_end) && Y_pos > TopMoveThreshold) {
      hero._pos+=1;
    }
    
    if(inRange(hero._pos, S9_start+1, S9_end+1) && Y_pos < BottomMoveThreshold) {
      hero._pos-=1;
    }
  }
  
  // from directions
  if(hero._pos<0 || hero._pos>NUM_LEDS ){
    hero._pos=0;
  }
  
  if((hero._pos == e._pos && e.Alive())){
    hero.Die();
  }
  
  if((hero._pos == e2._pos && e2.Alive())){
    hero.Die();
  }
  
  if(e.Alive()){
    leds[e._pos] = enemyColor;
  }
  
  if(e2.Alive()){
    leds[e2._pos] = enemyColor;
  }
  

  if(hero._is_attacking){
    hero.Attack();
    
    if(STAGE==1){
        if(!(hero._a_down <= S1_start && hero._pos == S1_start)) leds[hero._a_down] = heroBlastColor;
    }
    
    if(STAGE==2){
        if(!(hero._a_down <= S5_start && hero._pos == S5_start)) leds[hero._a_down] = heroBlastColor;
        if(hero._a_down < S5_start) leds[hero._a_down]= CRGB::Black;
    }

    leds[hero._a_up] = heroBlastColor;
    
    if(inRange(e._pos,hero._a_down, hero._a_up+1) && e.Alive()){
        e.GetHit();
    }
    
    if(inRange(e2._pos,hero._a_down, hero._a_up+1) && e2.Alive()){
        e2.GetHit();
    }
  }

  if(e._loot_dropped) { 
    leds[e._loot_pos] = goldLootColor;
    
    if(hero._pos == e._loot_pos){
      hero.LevelUp();
      e.Unloot();
    }
  }
  
  if(e2._loot_dropped) { 
    leds[e2._loot_pos] = diamondLootColor;
    
    if(hero._pos == e2._loot_pos){
      hero.LevelUp();
      e2.Unloot();
    }
  }

  leds[hero._pos] = heroColor;

  
  if(hero._pos==Portal and STAGE==1){
    leds[Portal] = CRGB::Green;
    hero._pos = S5_start;
    hero.SaveCheckpoint(S5_start);
    Portal=S9_end;
    STAGE=2;
  }else if(hero._pos==Portal and STAGE==2){
    leds[Portal] = CRGB::Green;
    hero._pos = S1_start;
    hero.SaveCheckpoint(S1_start);
    Portal=S4_end;
    STAGE=1;
    
    enemySpeed=2*enemySpeed;
    if(enemySpeed>4) enemySpeed=4;
    
    e.Revive(enemySpeed);
    e2.Revive(enemySpeed);

  }else{
    leds[Portal] = portalColor;
  }
  
  
  FastLED.show();
  e.Move();
  e2.Move();
  delay(30);
}
