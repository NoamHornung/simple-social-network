#include "../include/task.h"

Task::Task(std::mutex &mutex, EncoderDecoder& encoderDecoder) : _mutex(mutex),
shouldTerminate(false),encdec(encoderDecoder){}

void Task::terminate() {
    shouldTerminate = true;
}

Task::~Task() = default;

SocketListener::SocketListener(std::mutex &mutex, EncoderDecoder& encoderDecoder) : Task(mutex, encoderDecoder) {}

void SocketListener::run() {
    while(!shouldTerminate){
            std::string response;

            bool isSuccessful = encdec.decode(response); //gets the response from the server and puts them in response
            if (!isSuccessful) {
                std::cout << "Disconnected. Exiting...\n" << std::endl;
                break;
            }
          //  std::lock_guard <std::mutex> lock(_mutex);
            std::cout << response << std::endl;

            if(response == "ACK 3"){ //ack for logout request todo how to stop the other thread?
                logoutStatus.gotAckResponse = true;
                logoutStatus.received = true;
                break;
            }else if(response == "ERROR 3") {
                logoutStatus.gotAckResponse = false;
                logoutStatus.received = true;
            }
    }
}
SocketListener::~SocketListener()= default ;


InputReader::InputReader(std::mutex &mutex, EncoderDecoder& encoderDecoder) : Task(mutex,encoderDecoder){}

void InputReader::run() {
    while(!shouldTerminate){
        const short bufsize = 1024;
        char buf[bufsize];

        // if(gotResponse) {
        // std::lock_guard<std::mutex> lock(_mutex);
        std::cin.getline(buf, bufsize, '\n'); //read user input

        char newCommand[1024];
        std::string opcodeStr;
        unsigned int i = 0;
        //get the command type (first word)
        while(buf[i] != ' ' && buf[i] != '\0'){
            opcodeStr += buf[i];
            i = i + 1;
        }
        bool sentSuccessfully = encdec.buildCommandAndSend(buf, newCommand,opcodeStr);

        //bool sentSuccessfully = encdec.buildCommandAndSend(buf); //sends the command as byte array
        //gotResponse = false;
        if (!sentSuccessfully) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }

        if(opcodeStr == "LOGOUT"){
            //boost::mutex::scoped_lock lock(_mutex);
            while(!logoutStatus.received){
                //wait
            }
            if(logoutStatus.received && logoutStatus.gotAckResponse){
                break;
            }else{
                logoutStatus.received = false;
            }
        }
    }
}
InputReader::~InputReader() = default;



