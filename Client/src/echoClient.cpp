#include <stdlib.h>
#include <connectionHandler.h>
#include <task.h>
#include <encoderDecoder.h>
#include <boost/filesystem.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/thread.hpp>
#include "../include/logoutStatus.h"
#include "../include/task.h"

LogoutStatus logoutStatus(false, false);

int main (int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
        return -1;
    }
    std::string host = argv[1];
    short port = atoi(argv[2]);
    
    ConnectionHandler connectionHandler(host, port);
    if (!connectionHandler.connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }

   // LogoutStatus logoutStatus = LogoutStatus();
   // logoutStatus = false;
    EncoderDecoder encdec(connectionHandler);
    std::mutex mutex;
    SocketListener taskListener(mutex, encdec);
    InputReader taskInputReader(mutex, encdec);

    boost::thread threadListener(&SocketListener::run, &taskListener);
    boost::thread threadInputReader(&InputReader::run, &taskInputReader);
    threadListener.join();
    //threadInputReader.interruption_requested();
    //taskInputReader.terminate();
    //taskInputReader.receivedLogoutResponse = true;
    threadInputReader.join();

    return 0;
}