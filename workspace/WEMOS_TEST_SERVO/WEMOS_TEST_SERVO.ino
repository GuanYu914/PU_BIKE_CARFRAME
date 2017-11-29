#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <ESP8266WiFi.h>
#include <Servo.h>
            
#define SERVO_MAX_PULSE_WIDTH      1380
#define SERVO_MIN_PULSE_WIDTH       700
#define SERVO_DELAY                  10              

/*-------------SERVO----------------*/
/*GPIO04 --> D4*/
#define SERVER_MOTOR_PIN           4
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
    for(int i = SERVO_MIN_PULSE_WIDTH ;i < SERVO_MAX_PULSE_WIDTH; i+=10)
    {
        delay(SERVO_DELAY);
        car_frame_servo_motor.writeMicroseconds(i);
        Serial.println(i);
    }
    for(int i = SERVO_MAX_PULSE_WIDTH ;i > SERVO_MIN_PULSE_WIDTH; i-=10)
    {
        delay(SERVO_DELAY);
        car_frame_servo_motor.writeMicroseconds(i);
        Serial.println(i);
    }
        
}
