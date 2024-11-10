
//need to install DHT11 Lib by Dhruba Saha
#include <DHT11.h> 

#define SECONDS_IN_HOUR     3600
#define WATER_MAX_COUNT     60

int pwm_red = 4;
int pwm_green= 2;
int pwm_blue = 15;

int gpio_1wire = 26;
int gpio_waterpump = 13;
int gpio_atomizer = 12;
int gpio_fan = 14;
int gpio_buzzer = 17;

int adc_soilmoist = 36;

float temp = 0;
float humidity = 0;
float soilmoisture = 0;
int fan = 0;
int atomizer = 0;
int buzzer = 0;
int waterpump = 0;
int color_preset = 0;
int color_red = 0;
int color_green = 0;
int color_blue = 0;

float conf_temp = 30;
float conf_humid = 40;
float conf_soilmoist = 30;
int conf_red = 0;
int conf_green = 0;
int conf_blue = 0;

int seconds_counter = 0;
int water_counter = 0;

DHT11 dht11(gpio_1wire);

void read_1wire(){
  temp = dht11.readTemperature();
	humidity = dht11.readHumidity();
}

void update_status(){
	read_1wire();
	soilmoisture = analogRead(adc_soilmoist);
}		

void update_output(){
  digitalWrite(gpio_waterpump, waterpump);
  digitalWrite(gpio_atomizer, atomizer);
  digitalWrite(gpio_fan, fan);
  digitalWrite(gpio_buzzer, buzzer);
}

void setup() {
		  pinMode(pwm_red, OUTPUT);
      pinMode(pwm_green, OUTPUT);
      pinMode(pwm_blue, OUTPUT);

      pinMode(gpio_waterpump, OUTPUT);
      pinMode(gpio_atomizer, OUTPUT);
      pinMode(gpio_fan, OUTPUT);
      pinMode(gpio_buzzer, OUTPUT);
}

void loop() {
	update_status();

	if(temp > conf_temp)
	{
		fan = 1;
	}
	else if(temp < conf_temp)
	{
		fan = 0;
	}
	else
	{
		fan = 0;
	}

  if(humidity > conf_humid)
  {
    atomizer = 0;
  }
  else if(humidity < conf_humid)
  {
    atomizer = 1;
  }
  else
  {
    atomizer = 0;
  }
  
  if(soilmoisture > conf_soilmoist)
  {
    waterpump = 0;
    water_counter = 0;
    buzzer = 0;
  }
  else if(soilmoisture < conf_soilmoist)
  {
    waterpump = 1;
    if(water_counter > WATER_MAX_COUNT)
      buzzer = 1;
  }
  else
  {
    waterpump = 0;
    water_counter = 0;
    buzzer = 0;
  }

  if(seconds_counter < (SECONDS_IN_HOUR * 24))
    seconds_counter++;
  else
    seconds_counter = 0;

  update_output();

  analogWrite(pwm_red, conf_red);
  analogWrite(pwm_green, conf_green);
  analogWrite(pwm_blue, conf_blue);

	delay(1000);
}
