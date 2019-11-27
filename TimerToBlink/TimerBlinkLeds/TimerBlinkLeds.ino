#define firstLED  D1
#define secondLED D0

volatile int countToThousand = 0;
void inline blinkLeds(){
  digitalWrite(firstLED,!digitalRead(firstLED));
  countToThousand++; // When timer1 reaches 1000 ticks(1 second): Toggle secondLED
  if(countToThousand == 1000){
    Serial.println("Yayyyyy!");
    digitalWrite(secondLED,!digitalRead(secondLED));
    countToThousand = 0;
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(firstLED,OUTPUT);
  pinMode(secondLED,OUTPUT);

  noInterrupts(); //Just to ensure there is no other interrupt at this moment; Can be removed if you want
  timer1_isr_init();
  timer1_attachInterrupt(blinkLeds); //Interrupt routine
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP); // Divide by 16: 5MHz; Edge Triggered; Auto enable Global Interrupt Bit
  timer1_write(5000); //5000 ticks at 5MHz <=> 1kHz
  interrupts(); // Enable Interrupt Bit

}

void loop() {
  // put your main code here, to run repeatedly:

}
