#include"DC.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

enum{REGULAR,INCREASE,MODIFY,STOPWATCH} state;

extern volatile short second = 0, minute = 0, hour = 0;
extern volatile short wSecond = 0, wMinute = 0, wMili = 0;
extern volatile uint32_t interval = 1000;
extern volatile bool isDisplay = true;
extern volatile bool pressed1 = 0;
extern volatile bool pressed2 = 0, pressedOne2 = 0; 
extern volatile bool firstRead1 = 0, secondRead1 = 0;
extern volatile bool firstRead2 = 0, secondRead2 = 0;
extern volatile bool checkOne = true; 
extern volatile bool checkPressed = true;
extern volatile bool checkPressed2 = true;
extern volatile bool StopwatchFlag = false;
extern volatile bool accessRegular = true, accessIncrease = false, accessStopwatch = false;
extern volatile int count2 = 0, countOne2 = 0;
extern volatile int count = 0;
extern volatile short modeModify = 0;

void ISR(){
   count++;
   if(count >= (1000 / interval)){
      second++;
      count = 0;
   }
   if(modeModify == 4 && StopwatchFlag)wMili++;
   displayLCD();
}

void readButtons(){
    // Read and store signal of BUTTON1
   secondRead1 = firstRead1;
   firstRead1 = digitalRead(BUTTON1);

   
   // Read and store signal of BUTTON2
   secondRead2 = firstRead2;
   firstRead2 = digitalRead(BUTTON2);

   if((firstRead1 == secondRead1) && (firstRead1 == 0)){
      pressed1 = 1;
   }else{
      pressed1 = 0;
   }


   if((firstRead2 == secondRead2) && (firstRead2 == 0) && (pressed1 == 0)){
      count2++;
      pressed2 = 1;
      if(count2 >= 50){
         pressedOne2 = 1;
         if(countOne2 >= 10){
            countOne2 = 0;
         }
         countOne2++;
      }   
   }else{
      count2 = 0;
      pressed2 = 0;
      pressedOne2 = 0;
      countOne2 = 0;
   }
   
   timer0_write(ESP.getCycleCount()+1600000);
}

void initialize(){
    initializeLCD();
    initializeTimer01();
}

void initializeLCD(){
   Wire.begin();
   lcd.begin(16, 2);
   lcd.init();
   lcd.backlight();
}

void initializeTimer01(){
   noInterrupts();
   timer1_isr_init();
   timer1_attachInterrupt(ISR);
   timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
   timer1_write(5000 * interval); 

   timer0_isr_init();
   timer0_attachInterrupt(readButtons);
   timer0_write(ESP.getCycleCount()+1600000);
   interrupts();
}

void FSM(){
    switch(state){
      case REGULAR:
         if(accessRegular){
            interval = 1000;
            timer1_write(5000 * interval);
            accessRegular = false;
         }
         if(pressed1){
            if(checkPressed){
               state = INCREASE;
               modeModify = 1;
               accessIncrease = true;
               checkPressed = false;   
            }
         }else checkPressed = true;
         break;
      case INCREASE:
         if(accessIncrease){
            interval = 250;
            timer1_write(5000 * interval);
            accessIncrease = false;
         }
         if(pressed1){
            if(checkPressed){
               modeModify++;
               if(modeModify == 4){
                  state = STOPWATCH;
                  accessStopwatch = true;
               }
               checkPressed = false;   
            }
         }else checkPressed = true;
         if(pressed2){
            switch(modeModify){
               case 1:
                  hour++;
                  if(hour >= 24) hour = 0;
                  break;
               case 2:
                  minute++;
                  if(minute >= 60) minute = 0;
                  break;
               case 3:
                  second++;
                  if(second >= 60) second = 0;
                  break;
            }
            state = MODIFY;
         }
         break;
      case MODIFY:
         if(!pressed2) state = INCREASE;
         if(pressedOne2){
            if(countOne2 == 10){
               if(checkOne){
                  checkOne = false;
                  switch(modeModify){
                     case 1:
                        hour++;
                        break;
                     case 2:
                        minute++;
                        break;
                     case 3:
                        second++;
                        break;
                  }
               }
            }else checkOne = true;
         }
         break;
      case STOPWATCH:
         if(accessStopwatch){
            interval = 100;
            timer1_write(5000 * interval);
            accessStopwatch = false;
         }
         if(pressed2){
            if(checkPressed2){
               checkPressed2 = false;
               StopwatchFlag = !StopwatchFlag;
            }
         }else checkPressed2 = true;
         if(pressed1){
            if(checkPressed){
               state = REGULAR;
               modeModify = 0;
               wMili = wSecond = wMinute = 0;
               accessRegular = true;
               checkPressed = false;   
            }
         }else checkPressed = true;
         break;
   }
}

void displayLCD(){
   isDisplay = !isDisplay;
   lcd.clear();
   lcd.setCursor(4,0);
   if(second >= 60){
      minute++;  
      second = 0;
   }
   if(minute >= 60){
      hour++;
      minute = 0;
   }
   if(hour >= 24){
      hour = 0;
   }

   if(modeModify == 4){
      if(wMili == 10){
      wSecond++;  
      wMili = 0;
      }
      if(wSecond == 60){
         wMinute++;
         wSecond = 0;
      }
      if(wMinute == 60){
         wMinute = 0;
      }
   }

   switch(modeModify){
      case 0:
         if(hour < 10){
            lcd.print("0");
            lcd.print(hour);
         }else lcd.print(hour);
         lcd.print(":");
         if(minute < 10){
            lcd.print("0");
            lcd.print(minute);
         }else lcd.print(minute);
         lcd.print(":");
         if(second < 10){
            lcd.print("0");
            lcd.print(second);
         }else lcd.print(second);
         break;
      case 1:
         if(isDisplay){
            if(hour < 10){
               lcd.print("0");
               lcd.print(hour);
            }else lcd.print(hour);
            lcd.print(":");
         }else lcd.print("  :");
         if(minute < 10){
            lcd.print("0");
            lcd.print(minute);
         }else lcd.print(minute);
         lcd.print(":");
         if(second < 10){
            lcd.print("0");
            lcd.print(second);
         }else lcd.print(second);
         break;
      case 2:
         if(hour < 10){
            lcd.print("0");
            lcd.print(hour);
         }else lcd.print(hour);
         lcd.print(":");
         if(isDisplay){
            if(minute < 10){
               lcd.print("0");
               lcd.print(minute);
            }else lcd.print(minute);
            lcd.print(":");
         }else lcd.print("  :");
         if(second < 10){
            lcd.print("0");
            lcd.print(second);
         }else lcd.print(second);
         break;
      case 3:
         if(hour < 10){
            lcd.print("0");
            lcd.print(hour);
         }else lcd.print(hour);
         lcd.print(":");
         if(minute < 10){
            lcd.print("0");
            lcd.print(minute);
         }else lcd.print(minute);
         lcd.print(":");
         if(isDisplay){
            if(second < 10){
               lcd.print("0");
               lcd.print(second);
            }else lcd.print(second);
         }else lcd.print("  ");
         break;
      case 4:
         if(wMinute < 10){
            lcd.print("0");
            lcd.print(wMinute);
         }else lcd.print(wMinute);
         lcd.print(":");
         if(wSecond < 10){
            lcd.print("0");
            lcd.print(wSecond);
         }else lcd.print(wSecond);
         lcd.print(":");
         lcd.print(wMili);
         break;
   }
} 
