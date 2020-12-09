#include "Arduino.h"
#include "GPS_Air530.h"

void setup() {
    Serial.begin(115200);
    Air530.begin();

    Air530.setmode(MODE_GPS);

    //GPS.setNMEA(NMEA_GGA|NMEA_GSA|NMEA_RMC|NMEA_VTG);

    //GPS.setPPS(3,200);
}

void loop() {
    String NMEA = Air530.getNMEA();
    if(NMEA != "0")
    {
        Serial.println(NMEA);
    }
}
