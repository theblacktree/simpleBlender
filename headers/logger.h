#ifndef LOGGER_H
#define LOGGER_H
#include <condition_variable>
#include <string.h>
#include <string>
#include <fstream>
#include <sstream>
#include <thread>
#include <atomic>
#include <memory>
#include <queue>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <mutex>
#include "singleton.h"
class Logger: public Singleton<Logger>
{
public:

    void run();
    void stop();
    void addLogQueue(std::string& str);

    void writeToTxtFile();
private:
    std::string m_filePath = "./log/";
    std::atomic<bool>m_isStopFlag = false;
    std::queue<std::string>m_logQueue;
    std::unique_ptr<std::thread>m_logThreadPtr;
    std::condition_variable m_condition;
    std::mutex m_mtx;
};

#endif // LOGGER_H
