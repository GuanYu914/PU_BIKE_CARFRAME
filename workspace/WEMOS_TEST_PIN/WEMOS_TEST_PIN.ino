/*-------------RGBLED---------------*/
/*GPIO14 --> D13*/
#define R_LED_PIN                   14
/*GPIO13 --> D11*/
#define G_LED_PIN                   13
/*GPIO12 --> D12*/
#define B_LED_PIN                   12
/*GPIO5 --> D3*/
#define BUZZER_PIN                   5
#define Time                      1000

void setup() 
{
	Serial.begin(115200);
	pinMode(R_LED_PIN, OUTPUT);
	pinMode(G_LED_PIN, OUTPUT);
	pinMode(B_LED_PIN, OUTPUT);
	pinMode(BUZZER_PIN, OUTPUT);
}

void loop() 
{
	LED_turnoff();
	digitalWrite(R_LED_PIN, HIGH);
	delay(500);
	LED_turnoff();
	digitalWrite(G_LED_PIN, HIGH);
	delay(500);
	LED_turnoff();
	digitalWrite(B_LED_PIN, HIGH);
	delay(500);
}

void LED_turnoff(void)
{
	digitalWrite(R_LED_PIN, LOW);
	digitalWrite(G_LED_PIN, LOW);
	digitalWrite(B_LED_PIN, LOW);
}