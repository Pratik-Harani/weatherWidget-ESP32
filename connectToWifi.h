#include "secrets.h"
#include "WiFi.h"
#include "esp_eap_client.h"

WiFiClient client;


void print_wifi_info ()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.print("Connected to WiFi network: ");
    Serial.println(WiFi.SSID());
  }
  else
  {
    Serial.println("Wifi not connected :(");
  }
}


void waitOutForConnection(float timeout){
  unsigned long deadline = millis() + (unsigned long)(timeout * 1000);
  while ((WiFi.status() != WL_CONNECTED) && (millis() < deadline))
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
}


void connectToWifi (float timeout = 30)
{
 
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  
  Serial.printf("Connecting to eduroam as user %s\n", SECRET_EDUROAM_USER);
  esp_eap_client_set_identity((uint8_t *)SECRET_EDUROAM_USER, strlen(SECRET_EDUROAM_USER));
  esp_eap_client_set_username((uint8_t *)SECRET_EDUROAM_USER, strlen(SECRET_EDUROAM_USER));
  esp_eap_client_set_password((uint8_t *)SECRET_EDUROAM_PASS, strlen(SECRET_EDUROAM_PASS));
  esp_wifi_sta_enterprise_enable();
  WiFi.begin("eduroam");
  waitOutForConnection(timeout);
  

if (WiFi.status() != WL_CONNECTED){
  WiFi.disconnect(true);
  Serial.println("eduroam connection failed, connecting to backup network");
  Serial.printf("Connecting to %s\n", SECRET_BACKUP_WIFI_SSID);
  WiFi.begin(SECRET_BACKUP_WIFI_SSID, SECRET_BACKUP_WIFI_PASS);
  waitOutForConnection(timeout);
}
  
  Serial.println("");
  print_wifi_info();
}

