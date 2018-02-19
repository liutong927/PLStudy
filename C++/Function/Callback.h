//**************************************************************
//         a simple callback system built by std::bind and std::function
//**************************************************************

#ifndef CALLBACK_H
#define CALLBACK_H

#include <functional>
#include <iostream>
#include <string>

using namespace std;

class OperatingSystem
{
public:
    OperatingSystem() :m_callback(nullptr)
    {
    }

    ~OperatingSystem()
    {
    }

    template<typename T1,typename T2>
    void Register(T1 memfunc, T2 pThis)
    {
        m_callback = bind(memfunc, pThis, _1, _2);
    }

    void Notify()
    {
        if (m_callback != nullptr)
        {
            m_callback(m_jobName, m_jobMessage);
        }
    }

    void SetJobName(string& name)
    {
        m_jobName = name;
    }

    void SetJobMessage(string& msg)
    {
        m_jobMessage = msg;
    }

private:
    using NotifyFunc = std::function<void(string, string)>;
    NotifyFunc m_callback;
    string m_jobName;
    string m_jobMessage;
};

class Job
{
public:
    Job(string& jobName, string& jobMsg) :jobName(jobName), jobMsg(jobMsg)
    {
    }

    ~Job()
    {
    }

    void AddJob(OperatingSystem& os)
    {
        os.Register(&Job::SendMsg, this);
        os.SetJobName(jobName);
        os.SetJobMessage(jobMsg);
        os.Notify();
    }

private:
    void SendMsg(string& name, string& msg)
    {
        cout << "(local)" << name << ": " << msg << endl;
    }

private:
    string jobName;
    string jobMsg;
};

void TestCallback()
{
    OperatingSystem os;

    Job job1(string("Job1"), string("Job1 started."));
    job1.AddJob(os);

    Job job2(string("Job2"), string("Job2 started."));
    job2.AddJob(os);
}



#endif
