#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <ESP8266WiFi.h>
#include <Servo.h>
            
/*-------------SERVO----------------*/
/*GPIO04 --> D4*/
#define SERVER_MOTOR_PIN           4
#define TIME_LEVEL                5
#define SERIAL_BUAD_RATE      115200

Servo car_frame_servo_motor;

void setup()
{
        Serial.begin(SERIAL_BUAD_RATE);
        pinMode(SERVER_MOTOR_PIN, OUTPUT);
        car_frame_servo_motor.attach(SERVER_MOTOR_PIN);
        Serial.println("Initialize completed.");
}

void loop()
{
    for(int i = 550 ;i < 1700; i+=10)
    {
        delay(TIME_LEVEL);
        car_frame_servo_motor.writeMicroseconds(i);
        Serial.println(i);
    }
    for(int i = 1700 ;i > 550; i-=10)
    {
        delay(TIME_LEVEL);
        car_frame_servo_motor.writeMicroseconds(i);
        Serial.println(i);
    }
        
}
