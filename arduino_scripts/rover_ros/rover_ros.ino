#include <EEPROM.h>
#include <fuzzy_table.h>
#include <PID_Beta6.h>
#include <PinChangeInt.h>
#include <PinChangeIntConfig.h>
#include <MotorWheel.h>
#include <Omni3WD.h>
#include <ros.h>
#include <std_msgs/Float64.h>


// モーター制御
// motor 1 = right
#define PWM1 9
#define DIR1 8
#define MOTOR1A 6
#define MOTOR1B 7
// motor 2 = left
#define PWM2 10
#define DIR2 11
#define MOTOR2A 14
#define MOTOR2B 15
// motor 3 = back
#define PWM3 3
#define DIR3 2
#define MOTOR3A 4
#define MOTOR3B 5

float cmd_left = 0.0;
float cmd_back = 0.0;
float cmd_right = 0.0;

void read_lcmd(const std_msgs::Float64& cmd_msg){
  cmd_left = cmd_msg.data;
}

void read_bcmd(const std_msgs::Float64& cmd_msg){
  cmd_back = cmd_msg.data;
}

void read_rcmd(const std_msgs::Float64& cmd_msg){
  cmd_right = cmd_msg.data;
}

void speed_motor(int PWM,float cmd){
  float speed;
  if(cmd != 0.0){
    speed = (abs(cmd) * 85);
    if (speed > 250){
      speed = 250;
    }
    analogWrite(PWM,speed);
  }
  else{
    analogWrite(PWM,0);
  }
}

void speed_rover(){
  speed_motor(PWM1,cmd_right);
  speed_motor(PWM2,cmd_left);
  speed_motor(PWM3,cmd_back);
}

ros::Subscriber<std_msgs::Float64> sub_lcmd("/rover/joint_controller0/command", read_lcmd);
ros::Subscriber<std_msgs::Float64> sub_bcmd("/rover/joint_controller1/command", read_bcmd);
ros::Subscriber<std_msgs::Float64> sub_rcmd("/rover/joint_controller2/command", read_rcmd);
ros::NodeHandle node_handle;

std_msgs::Float64 rover_odm;
ros::Publisher pub_odm("/rover_odom", &rover_odm);

void setup(){
  TCCR1B=TCCR1B&0xf8|0x01;    // Pin9,Pin10 PWM 31250Hz
  TCCR2B=TCCR2B&0xf8|0x01;    // Pin3,Pin11 PWM 31250Hz
  pinMode(MOTOR1A,OUTPUT); //信号用ピン
  pinMode(MOTOR1B,OUTPUT); //信号用ピン
  pinMode(PWM1,OUTPUT); //信号用ピン
  pinMode(DIR1,OUTPUT); //信号用ピン
  pinMode(MOTOR2A,OUTPUT); //信号用ピン
  pinMode(MOTOR2B,OUTPUT); //信号用ピン
  pinMode(PWM2,OUTPUT); //信号用ピン
  pinMode(DIR2,OUTPUT); //信号用ピン
  pinMode(MOTOR3A,OUTPUT); //信号用ピン
  pinMode(MOTOR3B,OUTPUT); //信号用ピン
  pinMode(PWM3,OUTPUT); //信号用ピン
  pinMode(DIR3,OUTPUT); //信号用ピン
  
    
  //node_handle.getHardware()->setBaud(19200);
  node_handle.getHardware()->setBaud(115200);
  node_handle.initNode();
  node_handle.subscribe(sub_lcmd);
  node_handle.subscribe(sub_bcmd);
  node_handle.subscribe(sub_rcmd);
  node_handle.advertise(pub_odm);

}

void move_motor(int PWM,int DIR,int A,int B,float cmd_motor){
  if(cmd_motor > 0){  
    digitalWrite(PWM,HIGH);
    digitalWrite(A,HIGH);
    digitalWrite(B,LOW);
    digitalWrite(DIR,LOW);
  }
  else if(cmd_motor < 0){
    digitalWrite(PWM,HIGH);
    digitalWrite(A,LOW);
    digitalWrite(B,HIGH);
    digitalWrite(DIR,HIGH);    
  }
  else{
    digitalWrite(PWM,LOW);
    digitalWrite(A,LOW);
    digitalWrite(B,LOW);    
  }
}

void move_rover(){
  move_motor(PWM1,DIR1,MOTOR1A,MOTOR1B,cmd_right);
  move_motor(PWM2,DIR2,MOTOR2A,MOTOR2B,cmd_left);
  move_motor(PWM3,DIR3,MOTOR3A,MOTOR3B,cmd_back);
}

void demo(){
  digitalWrite(PWM1,LOW);
  digitalWrite(MOTOR1A,LOW);
  digitalWrite(MOTOR1B,LOW);
  delay(1000);
  digitalWrite(PWM1,HIGH);
  digitalWrite(MOTOR1A,HIGH);
  digitalWrite(MOTOR1B,LOW);
  digitalWrite(DIR1,LOW);
  analogWrite(PWM1,100); 
  delay(3000);
  digitalWrite(PWM1,LOW);
  digitalWrite(MOTOR1A,LOW);
  digitalWrite(MOTOR1B,LOW);
  delay(1000);
  digitalWrite(PWM1,HIGH);
  digitalWrite(MOTOR1A,LOW);
  digitalWrite(MOTOR1B,HIGH);
  digitalWrite(DIR1,HIGH);
  analogWrite(PWM1,10); 
  delay(3000);  
}

void loop(){
    
  //demo();
  move_rover();
  speed_rover();
  delay(10);
  //pub_odm.publish(&rover_odm);
  node_handle.spinOnce();
}
