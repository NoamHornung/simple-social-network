#ifndef TASK_H
#define TASK_H

#include <iostream>
#include <mutex>
#include <thread>
#include <ctime>
#include <boost/thread.hpp>
#include "encoderDecoder.h"
#include "logoutStatus.h"

extern LogoutStatus logoutStatus;

class Task{
public:
    std::mutex &_mutex;
    bool shouldTerminate;
    EncoderDecoder encdec;
    Task(std::mutex& mutex, EncoderDecoder& encoderDecoder);
    virtual void run() = 0;
    void terminate();
    virtual ~Task()=0;
};

class SocketListener : public Task{
public:
    SocketListener(std::mutex& mutex, EncoderDecoder& encoderDecoder);
    void run();
    ~SocketListener();
};

class InputReader : public Task{
public:
    InputReader(std::mutex& mutex, EncoderDecoder& encoderDecoder);
    void run();
    bool buildCommandAndSend(char (&command)[1024]);
    ~InputReader();
};

#endif
