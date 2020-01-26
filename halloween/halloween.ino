
//#include <ESP8266WiFi.h>
//#include <LiquidCrystal.h>
#include <LiquidCrystal595.h>
int d7 = 16, d6 = 5, d5 = 4, d4 = 0, rs = 14, en = 2;
//int d7 = 12, d6 = 11, d5 = 10, d4 = 9, rs = 7, en = 8;

LiquidCrystal595 lcd(12,13, 15);
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//LiquidCrystal lcd(D0, D1, D2, D3, D4, D5);
unsigned long currentMillis = 0;
unsigned long cycles = 0;
byte toggle_pin = 12;
byte rst_pin = 13;
bool toggle_state = false;

// variables will change:
int buttonState = 0;
int previousBState = 0;

#define CHANGE_MESSAGE 5000
#define MAX_CYCLES 6

//Declarations
extern byte ghost_top_left[];
extern byte ghost_top_right[];
extern byte ghost_bot_left[];
extern byte ghost_bot_right[];
extern byte pumpkin_top_left[];
extern byte pumpkin_top_right[];
extern byte pumpkin_bot_left[];
extern byte pumpkin_bot_right[];

extern const char* msg[];
extern size_t msg_count;

typedef unsigned int size_t;

void setup() {
  // put your setup code here, to run once:
  //Init random
  initRandom();
  // initialize the pushbutton pin as an input:
  pinMode(toggle_pin, INPUT_PULLUP);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.clear();
  // Print a message to the LCD. 
  lcd.setCursor(2, 0);
  lcd.print("Hello, World!");
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  Serial.begin(115200);
  currentMillis = CHANGE_MESSAGE - 30;
  pinMode(rst_pin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  // read the state of the pushbutton value:
    //lcd.clear();
   if(previousBState == HIGH && buttonState == LOW)
   {
      toggle_state = !toggle_state;
      currentMillis = CHANGE_MESSAGE;
      lcd.clear();
   }
   previousBState = buttonState;
   buttonState = digitalRead(toggle_pin);

   
   showBling();
   //lcd.setCursor(0, 0);
    //lcd.print("Hello, World!");
   if(!toggle_state)
    showRoutineMessages();
   else
    showUnavailableMsg();

   addTick();
   if(cycles >= MAX_CYCLES)
    {
      pinMode(rst_pin, OUTPUT);
      digitalWrite(rst_pin, HIGH);
    }
}

void addTick()
{
  delay(1);
  currentMillis++;
}

void showRoutineMessages()
{
  size_t strval = 0;
  if(currentMillis >= CHANGE_MESSAGE)
  {
    strval = random(msg_count - 1);
    lcd.clear();
    currentMillis = 0;
    cycles++;
  }
  else
    return;
  char* line1 = lineTok(msg[strval], '#', false);
  char* line2 = lineTok(msg[strval], '#', true);
  Serial.println(line1);
  if(line1)
  {
    //Return cursor to beginning of text
    lcd.setCursor(2, 0);
    lcd.print(line1);
    Serial.println(line1);
    delete line1;
  }
  if(line2)
  {
    //Return cursor to beginning of text
    lcd.setCursor(2, 1);
    lcd.print(line2);
    Serial.println(line2);
    delete line2;
  }
}

char* lineTok(const char* str, const char token, bool second)
{
  size_t s = 0;
  size_t startSeg = 0;
  size_t endSeg = 0;
  Serial.println(str);
  if(second)
  {
    while(str[startSeg] != '\n' &&  str[startSeg] != token)
    {
      startSeg++;
    }
    startSeg++;
    endSeg = startSeg;
  }
  while(str[endSeg] != '\n' &&  str[endSeg] != token)
  {
    endSeg++;
  }
  s = endSeg - startSeg;
  if(s <= 0)
    return NULL;

  char* p = new char[s + 1];
  Serial.println("his");
  for(size_t i = 0; i < s; i++)
  {
    p[i] = str[startSeg + i];
  }
  p[s] = 0;
  return p;
}

void showUnavailableMsg()
{
  //Return cursor to beginning of text
    lcd.setCursor(2, 0);
    lcd.print("No Candy!");
    lcd.setCursor(2, 1);
    lcd.print("Go Away! XD");
}

void showBling()
{
  //Create char
  //Ghost
  lcd.createChar(0, ghost_top_left);
  lcd.createChar(1, ghost_top_right);
  lcd.createChar(2, ghost_bot_left);
  lcd.createChar(3, ghost_bot_right);
  //Pumpkin
  lcd.createChar(4, pumpkin_top_left);
  lcd.createChar(5, pumpkin_top_right);
  lcd.createChar(6, pumpkin_bot_left);
  lcd.createChar(7, pumpkin_bot_right);

  //Display characters
  //Ghost
  lcd.setCursor(0, 0);
  lcd.write(byte(0));
  lcd.setCursor(1, 0);
  lcd.write(1);
  lcd.setCursor(0, 1);
  lcd.write(2);
  lcd.setCursor(1, 1);
  lcd.write(3);
  //Pumpkin
  lcd.setCursor(14, 0);
  lcd.write(4);
  lcd.setCursor(15, 0);
  lcd.write(5);
  lcd.setCursor(14, 1);
  lcd.write(6);
  lcd.setCursor(15, 1);
  lcd.write(7);
  //Return cursor to beginning of text
  lcd.setCursor(2, 0);
  
}


void initRandom()
{
  int r = 0;
  for( int i=1; i<=16; i++)
    r += analogRead(i);
  randomSeed(r);
}
