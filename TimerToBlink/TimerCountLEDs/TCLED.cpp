#include"TCLED.h"

extern volatile bool firstRead1 = 0, secondRead1 = 0;
extern volatile bool firstRead2 = 0, secondRead2 = 0;
extern volatile bool pressed1 = 0, pressedOne1 = 0, pressedThree1 = 0;
extern volatile bool pressed2 = 0, pressedOne2 = 0, pressedThree2 = 0;
extern volatile int count1 = 0, countOne1 = 0, countThree1 = 0;
extern volatile int count2 = 0, countOne2 = 0, countThree2 = 0;
extern volatile bool checkOne1 = 1, checkThree1 = 1;
extern volatile bool checkOne2 = 1, checkThree2 = 1;
extern volatile int counter = 0;

LiquidCrystal_I2C lcd(0x27,16,2);
enum{READY, IN, IN_ONE, IN_THREE, DE, DE_ONE, DE_THREE} state;

void ISR(){
   // Read and store signal of button1
   secondRead1 = firstRead1;
   firstRead1 = digitalRead(button1);

   
   // Read and store signal of button2
   secondRead2 = firstRead2;
   firstRead2 = digitalRead(button2);

   if((firstRead1 == secondRead1) && (firstRead1 == 0)){
      count1++;
      pressed1 = 1;
      if(count1 >= ((pressRequire2 * 1000) / interval)){
         pressedThree1 = 1;
         if(countThree1 >= ((speed2 * 1000) / interval)){
            countThree1 = 0;
         }
         countThree1++;
      }
      else if((count1 >= ((pressRequire1 * 1000) / interval)) && (count1 < ((pressRequire2 * 1000) / interval))){
         pressedOne1 = 1;
         if(countOne1 >= ((speed1 * 1000) / interval)){
            countOne1 = 0;
         }
         countOne1++;
      }
          
   }else{
      count1 = 0;
      pressed1 = 0;
      pressedOne1 = 0;
      pressedThree1 = 0;
      countOne1 = 0;
      countThree1 = 0;
   }


   if((firstRead2 == secondRead2) && (firstRead2 == 0) && (pressed1 == 0)){
      count2++;
      pressed2 = 1;
      if(count2 >= ((pressRequire2 * 1000) / interval)){
         pressedThree2 = 1;
         if(countThree2 >= ((speed2 * 1000) / interval)){
            countThree2 = 0;
         }
         countThree2++;
      }
      else if((count2 >= ((pressRequire1 * 1000) / interval)) && (count2 < ((pressRequire2 * 1000) / interval))){
         pressedOne2 = 1;
         if(countOne2 >= ((speed1 * 1000) / interval)){
            countOne2 = 0;
         }
         countOne2++;
      }
          
   }else{
      count2 = 0;
      pressed2 = 0;
      pressedOne2 = 0;
      pressedThree2 = 0;
      countOne2 = 0;
      countThree2 = 0;
   }
}

void modCounter(bool up){
   if(up){
      if(counter == 127) counter = 0;
      else counter++;
      lcd.clear();
      lcd.print(counter);
   }
   else{
      if(counter == 0) counter = 127;
      else counter--;
      lcd.clear();
      lcd.print(counter);
   }
}

void initializeTimer1(){
   timer1_isr_init();
   timer1_attachInterrupt(ISR);
   timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
   timer1_write(5000 * interval); 
   interrupts();
}

void initializeLCD(){
   Wire.begin();
   lcd.begin(16,2);
   lcd.init();
   lcd.backlight();
   lcd.print("Waiting...");
   delay(500);
   lcd.clear();
}

void initialize(){
   pinMode(button1, INPUT_PULLUP);
   pinMode(button2, INPUT_PULLUP);
}

void updateStep(){
   switch(state){
      case READY:
         if(pressed1){
            modCounter(1);
            state = IN;
         } 
         if(pressed2){
            modCounter(0);
            state = DE;
         }
         break;
      case IN:
         if(!pressed1) state = READY;
         if(pressedOne1) state = IN_ONE;
         break;
      case DE:
         if(!pressed2) state = READY;
         if(pressedOne2) state = DE_ONE;
         break;
      case IN_ONE:
         if(!pressed1) state = READY;
         if(countOne1 == ((speed1 * 1000) / interval)){
            if(checkOne1){ // Create a latch 
               checkOne1 = 0;
               modCounter(1);
            }
         }else checkOne1 = 1;
         if(pressedThree1) state = IN_THREE;
         break;
      case DE_ONE:
         if(!pressed2) state = READY;
         if(countOne2 == ((speed1 * 1000) / interval)){
            if(checkOne2){
               checkOne2 = 0;
               modCounter(0);
            }
         }else checkOne2 = 1;
         if(pressedThree2) state = DE_THREE;
         break;
      case IN_THREE:
         if(!pressed1) state = READY;
         if(countThree1 == ((speed2 * 1000) / interval)){
            if(checkThree1){
               checkThree1 = 0;
               modCounter(1);
            }
         }else checkThree1 = 1;
         break;
      case DE_THREE:
         if(!pressed2) state = READY;
         if(countThree2 == ((speed2 * 1000) / interval)){
            if(checkThree2){
               checkThree2 = 0;
               modCounter(0);
            }
         }else checkThree2 = 1;
         break;
   }
}
