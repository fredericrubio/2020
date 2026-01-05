#ifdef ESP32_ADAFRUIT_FEATHER
#include <Arduino.h>
#include <esp_sntp.h>
#include <WiFi.h>
#include <chrono>
#include "CX_Wifi.hpp"
using namespace std::chrono;
#else
#include <time.h>
#endif

#include "TS_NTP.hpp"

// #define _NTP_DEBUG_DISPLAY
#ifdef ESP32_ADAFRUIT_FEATHER
/*
 * 
 */
bool TS_NTP::synchronize() {

#ifdef _NTP_DEBUG_DISPLAY
        Serial.println("TS_NTP::synchronize - begin.");
        Serial.flush();
#endif        

    if (! CX_Wifi::isConnected()) {
        // try to connect
        if (! CX_Wifi::connect()) {
#ifdef _DEBUG_DISPLAY
        Serial.println("TS_NTP::synchronize - no wifi connection.");
#endif        
        return(false);
        }
    }
#ifdef _NTP_DEBUG_DISPLAY
    else {
        Serial.println("TS_NTP::synchronize - wifi already connected.");
        Serial.flush();
    }
#endif
    if (! sntp_enabled()) {
        sntp_set_sync_mode(SNTP_SYNC_MODE_IMMED);
        sntp_setoperatingmode(SNTP_OPMODE_POLL);
        sntp_setservername(0, "pool.ntp.org");
        sntp_init();

        int retry = 0;
        const int retry_count = 15;
        while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }
        if (retry > retry_count) {
#ifdef _DEBUG_DISPLAY
            Serial.println("TS_NTP::synchronize - failed (nb retry).");
            Serial.flush();
#endif        
            return(false);
        }
    }
    return(true);
}

/* 
 *
 */
void TS_NTP::getCurrentTime(const tm* pTimeInfo) {

    time_t now;
    time(&now);
    localtime_r(&now, (struct tm*) pTimeInfo);
#ifdef _DEBUG_DISPLAY
    char strftime_buf[64];
    strftime(strftime_buf, sizeof(strftime_buf), "%c", pTimeInfo);
    Serial.println("TS_NTP::getCurrentTime - Current date/time: " + String(strftime_buf));
#endif

}

/*
*
*/
void TS_NTP::getCurrentTime(const timeval* pTV) {

    gettimeofday((struct timeval*) pTV, NULL);

}

/*
 * pTimeZOne: time zone to set
*/
bool  TS_NTP::setTimeZone(const char* pTimeZone) {
    if (pTimeZone == NULL) {
        return -1;
    }
    // Set timezone to France Standard Time
    // setenv("TZ", "CET-1CEST", 1);
    int value = setenv("TZ", pTimeZone, 1);

        // Set timezone to France Standard Time
    if (value == 0) {
#ifdef _NTP_DEBUG_DISPLAY
        Serial.println("TS_NTP::setTimeZone - end.");
        Serial.flush();
#endif        
        tzset();
        return(true);
    }
#ifdef _DEBUG_DISPLAY
    Serial.println("TS_NTP::setTimeZone - failed (set tz).");
    Serial.flush();
#endif        
    return(false);
}

/*
 *
 */
long long TS_NTP::getCurrentTimeInMs() {
    milliseconds ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
    return ms.count();
}

#endif

/*
 *
 */
 long long TS_NTP::clockMS() {
#ifdef ESP32_ADAFRUIT_FEATHER
    return TS_NTP::getCurrentTimeInMs();
#else
    return clock();
#endif

}
