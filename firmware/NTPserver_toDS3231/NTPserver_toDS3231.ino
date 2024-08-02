#include <microDS3231.h>
#include <GyverNTP.h>

#define WIFI_SSID "Your Wi-Fi SSID"
#define WIFI_PASS "Your Wi-Fi Password"

GyverNTP ntp(3);
MicroDS3231 rtc;
String dateAsString;
String timeAsString;
int seconds, minutes, hour, day, month, year;

void setup() {
  Serial.begin(115200);
  //WiFi.setHostname("ESP-Wi-Fi test");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Подключено");
  Serial.println(WiFi.localIP());

  ntp.begin();
  //ntp.asyncMode(false);   // выключить асинхронный режим
  //ntp.ignorePing(true);   // не учитывать пинг до сервера
  pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {
  ntp.tick();
  DateTime now = rtc.getTime();

  timeAsString = ntp.timeToString();
  dateAsString = ntp.dateToString();

  seconds = timeAsString.substring(6).toInt();
  minutes = timeAsString.substring(3, 5).toInt();
  hour = timeAsString.substring(0, 2).toInt();
  day = dateAsString.substring(0, 2).toInt();
  month = dateAsString.substring(3, 5).toInt();
  year = dateAsString.substring(6).toInt();


  now.year = year;
  now.second = seconds + 1;
  now.minute = minutes;
  now.hour = hour;
  now.date = day;
  now.month = month;

  if (ntp.ms() == 999) {
    delay(1);
    rtc.setTime(now);
    digitalWrite(LED_BUILTIN, 0);
    Serial.println("Время записано в DS3231");
    delay(10000);
  }
}
