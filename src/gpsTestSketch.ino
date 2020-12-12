#include "Arduino.h"
#include "LoRaWan_APP.h"
#include "GPS_Air530.h"
#include "cubecell_SSD1306Wire.h"

#ifndef LoraWan_RGB
#define LoraWan_RGB 0
#endif

#define RF_FREQUENCY                                915000000 // Hz

#define TX_OUTPUT_POWER                             14        // dBm

#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       7         // [SF7..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false


#define RX_TIMEOUT_VALUE                            1000
#define BUFFER_SIZE                                 128 // Define the payload size here

char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];

static RadioEvents_t RadioEvents;

double txNumber;

int16_t rssi,rxSize;
void  DoubleToString( char *str, double double_num,unsigned int len);
int fracPart(double val, int n);


void setup() {
  Serial.begin(9600);
  Air530.begin();

  Air530.setmode(MODE_GPS);

  //GPS.setNMEA(NMEA_GGA|NMEA_GSA|NMEA_RMC|NMEA_VTG);

  //GPS.setPPS(3,200);txNumber=0;
  txNumber=0;
  rssi=0;

  Radio.Init( &RadioEvents );
  Radio.SetChannel( RF_FREQUENCY );
  Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                     LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                     LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                     true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );
}

void loop() {
  delay(1000);

  float lat = Air530.location.lat();
  uint8_t lat_frac = fracPart(lat, 6);
  float lng = Air530.location.lng();
  uint8_t lng_frac = fracPart(lng, 6);
  float altitude = Air530.altitude.meters();
  float speed = Air530.speed.mph();

  sprintf(txpacket, "Latitude: %.6f\nLongitude: %.6f\nAltitude: %.2f",
          lat, lng, altitude);
  Radio.Send( (uint8_t *)txpacket, strlen(txpacket) );

  Serial.write((uint8_t)lat);
  Serial.write(lat_frac);
  Serial.write((uint8_t)lng);
  Serial.write(lng_frac);
}

void  DoubleToString( char *str, double double_num,unsigned int len) {
  double fractpart, intpart;
  fractpart = modf(double_num, &intpart);
  fractpart = fractpart * (pow(10,len));
  sprintf(str + strlen(str),"%d", (int)(intpart)); //Integer part
  sprintf(str + strlen(str), ".%d", (int)(fractpart)); //Decimal part
}

int fracPart(double val, int n)
{
  return (int)((val - (int)(val))*pow(10,n));
}
