#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_LSM303_U.h>

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
Adafruit_L3GD20_Unified gyro = Adafruit_L3GD20_Unified(20);
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

/* Update this next line with the current SLP for better results      */
float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;

void displaySensorDetails(void)
{
  sensor_t bpmsensor;
  bmp.getSensor(&bmpsensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(bmpsensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(bmpsensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(bmpsensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(bmpsensor.max_value); Serial.println(" hPa");
  Serial.print  ("Min Value:    "); Serial.print(bmpsensor.min_value); Serial.println(" hPa");
  Serial.print  ("Resolution:   "); Serial.print(bmpsensor.resolution); Serial.println(" hPa");  
  Serial.println("------------------------------------");
  Serial.println("");

  sensor_t gyrosensor;
  gyro.getSensor(&gyrosensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(gyrosensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(gyrosensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(gyrosensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(gyrosensor.max_value); Serial.println(" rad/s");
  Serial.print  ("Min Value:    "); Serial.print(gyrosensor.min_value); Serial.println(" rad/s");
  Serial.print  ("Resolution:   "); Serial.print(gyrosensor.resolution); Serial.println(" rad/s");  
  Serial.println("------------------------------------");
  Serial.println("");
  
  sensor_t accelsensor;
  accel.getSensor(&accelsensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(accelsensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(accelsensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(accelsensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(accelsensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(accelsensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(accelsensor.resolution); Serial.println(" m/s^2");  
  Serial.println("------------------------------------");
  Serial.println("");
  
  sensor_t magsensor;
  mag.getSensor(&magsensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(magsensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(magsensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(magsensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(magsensor.max_value); Serial.println(" uT");
  Serial.print  ("Min Value:    "); Serial.print(magsensor.min_value); Serial.println(" uT");
  Serial.print  ("Resolution:   "); Serial.print(magsensor.resolution); Serial.println(" uT");  
  Serial.println("------------------------------------");
  Serial.println("");
  
  delay(500);
}

void setup(void) 
{

#ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif

  Serial.begin(9600);

  /* BAROMETER SETUP */
  Serial.println("Pressure Sensor Test"); Serial.println("");
  if(!bmp.begin())
  {
    Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  /* GYROSCOPE SETUP */
  Serial.println("Gyroscope Test"); Serial.println("");
  /* Enable auto-ranging */
  gyro.enableAutoRange(true);
  if(!gyro.begin())
  {
    Serial.println("Ooops, no L3GD20 detected ... Check your wiring!");
    while(1);
  }
  
  /* ACCELEROMETER SETUP */
  Serial.println("Accelerometer Test"); Serial.println("");
  if(!accel.begin())
  {
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }

  /* MAGNETOMETER SETUP */
  Serial.println("Magnetometer Test"); Serial.println("");
  /* Enable auto-gain */
  mag.enableAutoRange(true);
  if(!mag.begin())
  {
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  
  displaySensorDetails();
}

void loop(void) 
{
  /* BAROMETER LOOP */
  sensors_event_t bmpevent;
  bmp.getEvent(&bmpevent);
  if (bmpevent.pressure)
  {
    /* Display atmospheric pressue in hPa */
    Serial.print("Pressure:    ");
    Serial.print(bmpevent.pressure);
    Serial.println(" hPa");
    float temperature;
    bmp.getTemperature(&temperature);
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");
    Serial.print("Altitude:    "); 
    Serial.print(bmp.pressureToAltitude(seaLevelPressure,
                                        bmpevent.pressure)); 
    Serial.println(" m");
    Serial.println("");
  }
  else
  {
    Serial.println("Sensor error");
  }
  
  /* GYROSCOPE LOOP */
  sensors_event_t gyroevent; 
  gyro.getEvent(&gyroevent);
  /* Display the results (speed is measured in rad/s) */
  Serial.print("X: "); Serial.print(gyroevent.gyro.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(gyroevent.gyro.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(gyroevent.gyro.z); Serial.print("  ");
  Serial.println("rad/s ");
  
  /* ACCELEROMETER LOOP */
  sensors_event_t accelevent; 
  accel.getEvent(&accelevent);
  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: "); Serial.print(accelevent.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(accelevent.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(accelevent.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");

  /* MAGNETOMETER LOOP */
  sensors_event_t magevent; 
  mag.getEvent(&magevent);
  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  Serial.print("X: "); Serial.print(magevent.magnetic.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(magevent.magnetic.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(magevent.magnetic.z); Serial.print("  ");Serial.println("uT");
 
  delay(500);
}
