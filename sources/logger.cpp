#include "logger.h"
#include <iostream>
using namespace std;

void Logger::run()
{
    m_logThreadPtr = std::make_unique<thread>(&Logger::writeToTxtFile, this);
   // m_logThreadPtr->detach();
}

void Logger::stop()
{
    m_isStopFlag = true;
    m_condition.notify_one();
    m_logThreadPtr->join();
}

void Logger::addLogQueue(std::string &str)
{
    unique_lock<mutex> lock(m_mtx);
    m_logQueue.emplace(str);
    m_condition.notify_one();
}

void Logger::writeToTxtFile()
{
    auto now = chrono::system_clock::now();
    time_t ctime = chrono::system_clock::to_time_t(now);
    tm* localTime = localtime(&ctime);
    stringstream ss;
    ss<<put_time(localTime, "%Y-%m-%d_%H-%M-%S");
    string fileName = m_filePath + ss.str() + ".txt";
    ofstream outFile(fileName, std::ios::app);
    if (!outFile.is_open())
    {
        cerr<<"can not open logFile"<<endl;
    }
    while (!m_isStopFlag)
    {
        unique_lock<mutex> lock(m_mtx);
        m_condition.wait(lock, [this](){return !m_logQueue.empty() || m_isStopFlag;});
        if (m_logQueue.empty() && m_isStopFlag == true)
            break;
        while (!m_logQueue.empty())
        {
            outFile<<m_logQueue.front();
            m_logQueue.pop();
        }
        outFile.flush();
    }
    outFile.close();
    cout<<"logThread quit"<<endl;
}

