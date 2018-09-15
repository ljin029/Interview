//Design Alarm Clock
typedef void (*TaskCallbackFunc)(void);
struct alarmClock
{
    const Time tm;
    TaskCallbackFunc dosth;
}

class Manager : public Task
{
public:
    static Manager& getInstance()
    {
        static Manager obj;
        return obj;
    }
    bool checkAlarm(const Time& current, alarmClock& alarm);
    void addAlarm(alarmClock& alarm);
    bool compare_ascending(const alarmClock& first, const alarmClock& second)
    {
        return (first.tm < second.tm);
    }
    
private:
    Manager();
    ~Manager();
    Manager(Manager const &) = delete;
    void operator=(Manager const &) = delete;
    list<alarm> curlist;
    Mutex m_mutex;
    static Time m_period;
    static Time cPeriod = 1000;
    static uint32_t cCycleTolerance = 5;
}

Time m_period = cPeriod;
void Manager::task()
{
    alarm alarmOn;
    Time current;
    for(;;)
    {
        alarm.dosth = NULL;
        current = getTime();
        if(checkAlarm(current, alarmOn))
        {
            if(NULL != alarmOn.dosth)
            {
                alarmOn.dosth();
            }
        }
        delay(m_period);
    }
}

void Manager::addAlarm(alarmClock& alarm)
{
    if(NULL != alarm.dosth)
    {
        Time current = getTime();
        m_mutex(lock);
        curlist.push_front(alarm);
        curlist.sort(compare_ascending);
        m_period = cPeriod.front().tm - current;
        m_mutex(unlock);
    }
}

bool Manager::checkAlarm(const Time& current, alarmClock& alarm)
{
    m_mutex(lock);
    bool ret = false;
    
         Time current = getTime();
   Time tm = curlist.front().tm;
        if((current - tm) <= cCycleTolerance)
        {
            ret = true;
            alarm = curlist.pop_front();
            m_period = curlist.front().tm - current;
        }     
        else
        {
            m_period = curlist.front().tm - current;
        }
    m_mutex(unlock);
    return ret;
}