#include <RTClib.h>
#include <ezButton.h>
#include <LiquidCrystal_I2C.h>
RTC_DS1307 rtc;
LiquidCrystal_I2C lcd(0x27,20,4);
byte clk = 2;
byte dt = 3;
byte sw = 4;
ezButton button(sw);
int counter = 0;
int previous_counter = -1;
int flag = -1;
int year = 0;
int day = 0;
int month = 0;
int hour = 0;
int minute = 0;
int second = 0;
String rtc_date="";
String rtc_time="";

void lcd_print(int x,int y,String message){
  lcd.setCursor(x,y);
  lcd.print(message);
}

void current_time(){
  DateTime current = rtc.now();
  year =current.year();
  month =current.month();
  day =current.day();
  hour =current.hour();
  minute =current.minute();
  second =current.second();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.clear();
  lcd_print(0,0,"rtc found");
  delay(1000);
  lcd_print(0,0,"I am Niyati");
  delay(2000);
  if(!rtc.begin()){
    while(true);
  }
  attachInterrupt(digitalPinToInterrupt(clk),encoder,FALLING);
  button.setDebounceTime(25);
}
void encoder(){
  previous_counter = counter;
  if(digitalRead(dt)==HIGH){counter++;}
  else{counter--;}
  counter = constrain(counter,0,3);
  flag=1;
}
void mode_selector(){
  if(previous_counter!=counter&&flag==1){
    if(counter==0){
      lcd.clear();
      lcd_print(0,0,"date and time");
    }
    else if(counter==1){ 
      lcd.clear();
      lcd_print(0,0,"set alarm");
    }
    else if(counter==2){
      lcd.clear();
      lcd_print(0,0,"Stop watch");
    }
    else if(counter==3){
      lcd.clear();
      lcd_print(0,0,"count down time");
    }
    flag=0;
  }
}
void loop() {
  // put your main code here, to run repeatedly:
  button.loop();
  if(button.isPressed()){
    select_mode();
  }
  mode_selector();
  delay(10);

}

String get_date(DateTime current) {

  int year = current.year();
  int month = current.month();
  int day = current.day();

  String current_date = "Date : " + String(day) + "/" + String(month) +
                        "/" + String(year);

  return current_date;
}
void get_time(){
  while(true){
    button.loop();

    current_time();
    String current_date = "Date:"+String(day)+"/"+String(month)+
                         "/" + String(year);
    String current_time = "Time : " + String(hour) + ":" + String(minute) +
                        ":" + String(second);
    lcd_print(0,0,"Date and time");
    lcd_print(0,1, current_date);
    lcd_print(0,2, current_time);
  }
}
void set_alarm(){

}
void stopwatch() {

  int stopwatch_hour, stopwatch_minute, stopwatch_second = 0;
  int last_second = 0;

  while (true) {
    //  breaking loop if button pressed
    button.loop();
    if (button.isPressed()) {
      prev_counter = -1;
      flag = 1;

      delay(5000);
      lcd.clear();
      break;
    }

    //  tracking current time to get the 'seconds' variable
    current_time();

    //  stopwatch algo
    if (abs(second - last_second) >= 1) {
      last_second = second;
      String stopwatch_time = String(stopwatch_hour) +
                              " : " + String(stopwatch_minute) +
                              " : " + String(stopwatch_second);
      lcd.clear();
      lcd_print(0, 0, "Stopwatch");
      lcd_print(0, 1, String(stopwatch_time));
      stopwatch_second++;
    }

    //  condition check
    if (stopwatch_second > 59) {
      stopwatch_second = 0;
      stopwatch_minute++;
    }
    else if (stopwatch_minute > 59) {
      stopwatch_minute = 0;
      stopwatch_hour++;
    }
    else if (stopwatch_hour > 24) {
      stopwatch_second, stopwatch_minute, stopwatch_hour = 0;
    }
  }
}

void count_down(){

}
void select_mode() {
  if (counter  ==  0)get_time();
  if (counter  ==  1)set_alarm();
  if (counter  ==  2)stop_watch();
  if (counter  ==  3)count_down();
  }