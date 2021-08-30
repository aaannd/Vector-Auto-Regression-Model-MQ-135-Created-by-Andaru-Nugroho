/*Library by Agam Sanjaya 
 * Dated : 31-01-2020
 * credit : B.Aswinth Raj/www.circuitdigest.com
 * Article : Davide Gironi
 * Mix n Match : Andaru Nugrahanto
 */
 
 #include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,4);//adrressing lcd 0x27,16X2 lcd

#include "DHT.h"
#define DHTPIN 2  
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);
//library disertakan di deskripsi

#define RL 10 //nilai RL =10 pada sensor
#define m -0.11 //hasil perhitungan gradien
#define b 0.11 //hasil perhitungan perpotongan
#define Ro 30 //hasil pengukuran RO
#define MQ_sensor A0 //definisi variabel

const int numReadings = 5;//nilai penambilan sample pembacaan sebesar 5 kali
float readings[numReadings];      
int readIndex = 0;              
float total = 0;                  
float average = 0;                

void setup() {
lcd.begin();
lcd.backlight();
  Serial.begin(9600);

////////////////////////////////////

  Serial.println(F("Bismillah Sukses Aamiin"));

  dht.begin();

//////////////////////////////

   for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
for (int times = 1; times <=3; times++) {
    lcd.setCursor(0,0);
    lcd.print("  pre heating");
    lcd.setCursor(3,1);
    lcd.print(times);
    lcd.print(" seconds");
    delay(1000);
  }
  lcd.clear();

}

void loop() {

  float VRL; 
  float RS;  
  float ratio; 
   
  VRL = analogRead(MQ_sensor)*(5/1023.0); //konversi analog ke tegangan
   RS =(5.0 /VRL-1)*10 ;//rumus untuk RS
  ratio = RS/Ro;  // rumus mencari ratio

  
  float ppm = pow(10, ((log10(ratio)-b)/m));//rumus mencari ppm

  
 total = total - readings[readIndex];

  readings[readIndex] = ppm;

  total = total + readings[readIndex];

  readIndex = readIndex + 1;


  if (readIndex >= numReadings) {

    readIndex = 0;
  }

  average = total / numReadings;

////////////////////////////////////////////
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
   // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);


  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F(" %"));
  Serial.print(F(" Temperature: "));

  Serial.print(hic);
  Serial.print(F("°C "));

  

//////////////////////////////////////////////


int analogSensor = analogRead(MQ_sensor);//pembacaansensor sebagai inpuanalo
Serial.print("CO2 =  ");
Serial.print(average);
Serial.print(" PPM");
Serial.println();

     lcd.setCursor(3,0);
  lcd.print("Air Quality");
 //menampilkan nilai rata-rata ppm setelah 5 kali pembacaan
  lcd.print(" CO2");
 lcd.setCursor(4,1);
 lcd.print(average);
 lcd.print(" PPM");
  lcd.print("  ");
  
   delay(1000);

}
