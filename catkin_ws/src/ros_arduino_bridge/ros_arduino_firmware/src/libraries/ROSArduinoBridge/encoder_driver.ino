/* *************************************************************
   Encoder definitions
   
   Add an "#ifdef" block to this file to include support for
   a particular encoder board or library. Then add the appropriate
   #define near the top of the main ROSArduinoBridge.ino file.
   
   ************************************************************ */
   
#ifdef USE_BASE

#ifdef ROBOGAIA
  /* The Robogaia Mega Encoder shield */
  #include "MegaEncoderCounter.h"

  /* Create the encoder shield object */
  MegaEncoderCounter encoders = MegaEncoderCounter(4); // Initializes the Mega Encoder Counter in the 4X Count mode
  
  /* Wrap the encoder reading function */
  long readEncoder(int i) {
    if (i == LEFT) return encoders.YAxisGetCount();
    else return encoders.XAxisGetCount();
  }

  /* Wrap the encoder reset function */
  void resetEncoder(int i) {
    if (i == LEFT) return encoders.YAxisReset();
    else return encoders.XAxisReset();
  }
#elif defined(ARDUINO_ENC_COUNTER)
  volatile long left_enc_pos = 0L;
  volatile long right_enc_pos = 0L;
  static const int8_t ENC_STATES [] = {0,1,-1,0,-1,0,0,1,1,0,0,-1,0,-1,1,0};  //encoder lookup table
    
  /* Interrupt routine for LEFT encoder, taking care of actual counting */
  ISR (PCINT2_vect){
  	static uint8_t enc_last=0;
        
	enc_last <<=2; //shift previous state two places
	enc_last |= (PIND & (3 << 2)) >> 2; //read the current state into lowest 2 bits
  
  	left_enc_pos += ENC_STATES[(enc_last & 0x0f)];
  }
  
  /* Interrupt routine for RIGHT encoder, taking care of actual counting */
  ISR (PCINT1_vect){
        static uint8_t enc_last=0;
          	
	enc_last <<=2; //shift previous state two places
	enc_last |= (PINC & (3 << 4)) >> 4; //read the current state into lowest 2 bits
  
  	right_enc_pos += ENC_STATES[(enc_last & 0x0f)];
  }
  
  /* Wrap the encoder reading function */
  long readEncoder(int i) {
    if (i == LEFT) return left_enc_pos;
    else return right_enc_pos;
  }

  /* Wrap the encoder reset function */
  void resetEncoder(int i) {
    if (i == LEFT){
      left_enc_pos=0L;
      return;
    } else { 
      right_enc_pos=0L;
      return;
    }
  }

#elif defined ARDUINO_MY_COUNTER
  //功能左右电机的脉冲计数
  //计数器
  volatile long left_count = 0L;
  volatile long right_count = 0L;
  //初始化
  void initEncoder(){
    pinMode(LEFT_A,INPUT);
    pinMode(LEFT_B,INPUT);
    pinMode(RIGHT_A,INPUT);
    pinMode(RIGHT_B,INPUT);

    //添加中断
    attachInterrupt(5,leftEncoderEvent,CHANGE);
    //attachInterrupt(4,leftEncoderEvent,CHANGE);
    attachInterrupt(2,rightEncoderEvent,CHANGE);
    //attachInterrupt(1,rightEncoderEvent,CHANGE);
  }
  //编写中断的回调函数
   void leftEncoderEvent(){
    if(digitalRead(LEFT_A) == HIGH){
      if(digitalRead(LEFT_B) == HIGH){
        left_count++;
      } else {
        left_count--;
      }
    } else {
      if(digitalRead(LEFT_B) == LOW){
        left_count++;
      } else {
        left_count--;
      }
    }
   }
   void rightEncoderEvent(){
    if(digitalRead(RIGHT_A) == HIGH){
      if(digitalRead(RIGHT_B) == HIGH){
        right_count++;
      } else {
        right_count--;
      }
    } else {
      if(digitalRead(RIGHT_B) == LOW){
        right_count++;
      } else {
        right_count--;
      }
    }  
   }
  //实现编码器数据读和重置 
  long readEncoder(int i){
    if (i == LEFT)
      return left_count;
    if (i == RIGHT)
      return right_count;
  }
  void resetEncoder(int i){
    if (i == LEFT){
      left_count = 0L;
      return;
    } else { 
      right_count = 0L;
      return;
    }
  }
#else
  #error A encoder driver must be selected!
#endif

/* Wrap the encoder reset function */
void resetEncoders() {
  resetEncoder(LEFT);
  resetEncoder(RIGHT);
}

#endif
