#ifndef __Geomtk_TimeManager__
#define __Geomtk_TimeManager__

#include "geomtk_commons.h"

namespace geomtk {

struct TimeUnit {
    static const int SECONDS = 1;
    static const int MINUTES = 60;
    static const int HOURS = 3600;
    static const int DAYS = 86400;
};

enum TimeStepUnit {
    YEAR, MONTH, DAY, HOUR, MINUTE, SECOND
};

/**
 * Time
 * This class describes the time point.
 */

class Time {
public:
    bool useLeap;
    int year, month, day, hour, minute;
    double second;

    Time();
    Time(double seconds);
    virtual ~Time();

    virtual void reset();

    double getTOD() const;

    /**
     * This method returns the seconds between this time and other time. The
     * sign is included to represent the time direction.
     */
    double getSeconds(const Time &other) const;

    double getMinutes(const Time &other) const;
    
    double getHours(const Time &other) const;
    
    double getDays(const Time &other) const;

    /**
     * This method returns the days included in the given month and year. If the
     * month and year are not given, use the ones in this object.
     */
    int getDaysOfMonth(int month = -1, int year = -1) const;
    
    /**
     * This method returns the days before the given month and in the given
     * year. If the month and year are not given, use the ones in this object.
     */
    int getDaysBeforeMonth(int month = -1, int year = -1) const;
    
    /**
     * This method returns the days after the given month and in the given
     * year. If the month and year are not given, use the ones in this object.
     */
    int getDaysAfterMonth(int month = -1, int year = -1) const;
    
    /**
     * This method returns the days included in the given year. If the year is
     * not given, use the one in this object.
     */
    int getDaysOfYear(int year = -1) const;
    
    /**
     * This method returns the previous month of the given month. If the month
     * is not given, use the one in this object.
     */
    int getPrevMonth(int month = -1) const;
    
    /**
     * This method returns the next month of the given month. If the month is
     * not given, use the one in this object.
     */
    int getNextMonth(int month = -1) const;

    const Time operator+(double seconds) const;

    Time& operator+=(double seconds);

    const Time operator-(double seconds) const;

    Time& operator=(const Time &other);

    Time& operator=(const string &other);

    Time& operator=(const char *other);

    bool operator==(const Time &other) const;

    bool operator>(const Time &other) const;

    bool operator>=(const Time &other) const;

    bool operator<(const Time &other) const;

    bool operator<=(const Time &other) const;

    string s(bool onlyDate = false) const;

    friend inline ostream& operator<<(ostream &os, Time &other) {
        os << other.s();
        return os;
    }
};

class TimeManager {
protected:
    bool useLeap;
    Time startTime, currTime, endTime;
    TimeStepUnit stepUnit;
    double stepSize;
    int numStep;
public:
    TimeManager();
    ~TimeManager();

    /**
     *  Initialize time manager.
     *
     *  @param startTime the start time.
     *  @param endTime   the end time.
     *  @param stepSize  the time step size in seconds.
     */
    void init(const Time &startTime, const Time &endTime, double stepSize);

    void init(const string &startTime, const string &endTime,
              const string &stepSize);

    /**
     *  Reset the current time.
     *
     *  @param time the current time.
     */
    void resetCurrentTime(Time time);

    /**
     *  Advance the time by one time step.
     *
     *  @param mute boolean to control whether print message or not.
     */
    void advance(bool mute = false);

    /**
     *  Check if the end time is reached.
     *
     *  @return Boolean true if the end time is reached, otherwise false.
     */
    bool isFinished() const { return currTime > endTime; }
    
    const Time& getStartTime() const { return startTime; }
    const Time& getCurrTime() const { return currTime; }
    const Time& getEndTime() const { return endTime; }
    double getStepSize() const { return stepSize; }
    int getNumStep() const { return numStep; }
    double getSeconds() const { return currTime.getSeconds(startTime); }
    double getMinutes() const { return currTime.getMinutes(startTime); }
    double getHours() const { return currTime.getHours(startTime); }
    double getDays() const { return currTime.getDays(startTime); }
};

}

#endif
