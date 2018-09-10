#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <EEPROM.h>

const uint8_t left=4;
const uint8_t right=5;

// game variables
uint8_t dim=8;
int pos_player=60;
uint8_t pos_bullet_x=50;
uint8_t pos_bullet_y=0;
uint8_t life;
bool in_game;
int points;
int high_score;
//Costructor argument is the number of display reset pin, -1 means no reset pin
Adafruit_SSD1306 display(-1);


void setup() {
  // set pin connected to the buttons as input
  pinMode(left,INPUT);
  pinMode(right,INPUT);
  
  //initialize screen
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  //clearDisplay method is called to erase screen memory buffer
  display.clearDisplay();
  // splash screen
  display.setCursor(32,30);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print("Catch");
  display.display();
  display.setTextSize(1);
  delay(1000);

  //initialize random seed
  randomSeed(analogRead(0));

  // read from EEPROM the highest score value
  EEPROM.get(0,high_score);
 

}

void loop() {
  // in game
  if(in_game)
  {
    display.clearDisplay();
    display.setCursor(3,5);
    display.print(points);
    display.setCursor(127-life*12,5);
    
      
      
    // print on screen life indicator
    for(int i=0;i<life;i++)
      display.print("<3");

    // update player position
    if(digitalRead(left)==HIGH && pos_player>=1 )
      {
        pos_player=pos_player-5-points/10*2;
        if(pos_player<=1)
          pos_player=2;
      }
    else if(digitalRead(right)==HIGH && pos_player+dim<=127 )
      {
        pos_player=pos_player+ 5+points/10*2;
        if(pos_player+dim>127)
          pos_player=127-dim;
      }
    // update bullet position. Bullet velocity increases every 10 bullet
    pos_bullet_y = pos_bullet_y+(points/10)+1;
    
    //check if player catch the bullet
    if(pos_bullet_y>=59)
      {
        if(pos_bullet_x>=pos_player && pos_bullet_x<=pos_player+dim)
          {
            points++;
            if(points%10==0 && life<3)
            {
              life++;
              dim=dim+2;
            }
          }
         else
          {
            life--;
          }
          pos_bullet_y=0;
          pos_bullet_x=random(1,127);
      }

    // draw on screen bullet
    display.drawPixel(pos_bullet_x,pos_bullet_y,WHITE);
    
    // draw on screen player
    for(int i=pos_player;i<pos_player+dim;i++)
      display.drawPixel(i,59,WHITE);
    
    // draw on screen margin
    for(int i=0;i<128;i++)
      {
        display.drawPixel(i,0,WHITE);
        display.drawPixel(i,63,WHITE);
      }
    for(int i=0;i<63;i++)
      {
        display.drawPixel(0,i,WHITE);
        display.drawPixel(127,i,WHITE);
      }
    display.display();

    // check if it is game over
    if(life==0)
    {
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(10,5);
      display.print("GAME OVER");
      display.setCursor(58,23);
      display.print(points);
      display.setTextSize(1);
      display.display();
      delay(2000);
      
      // if new high score update and save into EEPROM
      if(points>high_score)
      {
        display.setCursor(20,45);
        display.print("NEW HIGH SCORE!!");
        display.display();
        high_score=points;
        EEPROM.put(0,high_score);
      }
      //exit the game
      in_game=false;
      delay(2000);
    }
   }
  // out of the game
  else
  {
    //display main screen
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(40,10);
    display.print("PLAY");
    display.setTextSize(1);
    display.setCursor(15,30);
    display.print("Click any button");
    display.setCursor(10,50);
    display.print("Highest score: ");
    display.print(high_score);
    display.display();
    if(digitalRead(left)== HIGH || digitalRead(right)== HIGH)
    {
      //initialize all the variables
      in_game=true;
      points=0;
      life=3;
      dim=8;
      pos_player=60;
      pos_bullet_y=0;
      pos_bullet_x=random(1,127);
    }
   
  }
  

}
