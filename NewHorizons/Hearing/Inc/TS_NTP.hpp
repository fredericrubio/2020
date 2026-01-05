#ifndef __TIME_SERVICE_NTP_H__
#define __TIME_SERVICE_NTP_H__

class TS_NTP {
    public:
#ifdef ESP32_ADAFRUIT_FEATHER
    /*
     * pTimeZOne: time zone to set
     */
    static bool synchronize();

    /*
     *
     */
    static void getCurrentTime(const tm* pTimeInfo);

    /*
     *
     */
    static void getCurrentTime(const timeval* pTV);

    /*
     *
     */
    static long long getCurrentTimeInMs();

    /*
     * pTimeZOne: time zone to set
     */
    static bool  setTimeZone(const char* pTimeZone);
#endif
    /*
     *
     */
    static long long clockMS();

};
#endif
