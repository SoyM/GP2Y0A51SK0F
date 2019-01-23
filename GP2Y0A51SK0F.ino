void setup() {
  Serial.begin(115200);

}
// Maximum value of times_read_average:64  (2^16/1024)
unsigned int times_read_average = 32;
unsigned int sensorA_v = 0;
unsigned int sensorB_v = 0;
int sensor_diff = 0;

unsigned int sensorAB_distance = 10; //cm

// 2~15cm
//                      2     3    4    5    6    7   8     9   10   11   12   13   14   15
float table_voltage[] = {2.08,1.68,1.32,1.06,0.91,0.81,0.72,0.66,0.59,0.52,0.48,0.45,0.43,0.39};

typedef struct {
  float offset; //cm
  float distance; //cm,
  unsigned int adc; 
  float voltage; //v
  int pin;
}SHARP;
SHARP sensorA = {0,0,0,0,A0};
SHARP sensorB = {0,0,0,0,A3};

void loop() {
  
  for(int i=1;i<=times_read_average;i++){
    sensorA.adc = (sensorA.adc*(times_read_average-1) + analogRead(sensorA.pin))/times_read_average;
  }
  for(int i=1;i<=times_read_average;i++){
    sensorB.adc = (sensorB.adc*(times_read_average-1) + analogRead(sensorB.pin))/times_read_average;
  }
  
  sensorA.voltage = sensorA.adc * (5.0 / 1023.0);
  sensorB.voltage = sensorB.adc * (5.0 / 1023.0);

  for(int i=0;i<14;i++){
    if(sensorA.voltage<= table_voltage[i]){
        sensorA.distance =i+2;
    }
    if(sensorB.voltage<= table_voltage[i]){
        sensorB.distance =i+2;
    }
  }
  Serial.print(asin((sensorA.distance-sensorB.distance)/sensorAB_distance)*180);
  Serial.println("°");
  

  Serial.print("L: ");
  Serial.print(sensorB.adc);
  Serial.print(" , ");
  Serial.print(sensorB.voltage);
  Serial.print("v, | ");
  Serial.print(sensorB.distance);
  Serial.println("cm");
  
  Serial.print("R: ");
  Serial.print(sensorA.adc);
  Serial.print(" , ");
  Serial.print(sensorA.voltage);
  Serial.print("v, | ");
  Serial.print(sensorA.distance);
  Serial.println("cm");
  
  Serial.print("closer side: ");
  sensor_diff = sensorA.adc - sensorB.adc;
  Serial.print(sensor_diff);
  if(sensor_diff>5){
    Serial.println("               [ R ]");
  }else if(sensor_diff<-5){
    Serial.println("               [ L ]");
  }

  Serial.println("-----------------------------------");
  
  delay(100);
} 
