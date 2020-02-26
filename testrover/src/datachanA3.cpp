/*
    Copyright (c) 2016 info@linux-projects.org
    All rights reserved.

    Redistribution and use in source and binary forms are permitted
    provided that the above copyright notice and this paragraph are
    duplicated in all such forms and that any documentation,
    advertising materials, and other materials related to such
    distribution and use acknowledge that the software was developed
    by the linux-projects.org. The name of the
    linux-projects.org may not be used to endorse or promote products derived
    from this software without specific prior written permission.
    THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
    IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/

/*
 * This is a simple echo server.
 * It creates to a unix domain socket of type SOCK_SEQPACKET specified by
 * command line, listens to it waiting for incoming messages from clients
 * (e.g. UV4L) and replies the received messages back to the senders.
 *
 * Example:
 *     $ ./datachannel_server /tmp/uv4l.socket
 *
 * To compile this program you need boost v1.60 or greater, for example:
 * g++ -Wall -I/path/to/boost/include/ -std=c++11 datachannel_server.cpp -L/path/to/boost/lib -l:libboost_coroutine.a -l:libboost_context.a -l:libboost_system.a -l:libboost_thread.a -pthread -o datachannel_server
 */

#ifndef BOOST_COROUTINES_NO_DEPRECATION_WARNING
#define BOOST_COROUTINES_NO_DEPRECATION_WARNING
#endif

#include <boost/asio/io_service.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio.hpp>
#include <memory>
#include <cstdio>
#include <array>
#include <functional>
#include <iostream>
#include <fstream> 
#include <string> 

#include <stdio.h>
#include <unistd.h>
#include "rs232.h"

#define BUF_SIZE 128
    #include <cstring>
    using std::string;


#if !defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
#error Local sockets not available on this platform.
#endif

constexpr std::size_t MAX_PACKET_SIZE = 1024 * 16;

namespace seqpacket {

    struct seqpacket_protocol {

        int type() const {
            return SOCK_SEQPACKET;
        }

        int protocol() const {
            return 0;
        }

        int family() const {
            return AF_UNIX;
        }

        using endpoint = boost::asio::local::basic_endpoint<seqpacket_protocol>;
        using socket = boost::asio::generic::seq_packet_protocol::socket;
        using acceptor = boost::asio::basic_socket_acceptor<seqpacket_protocol>;

#if !defined(BOOST_ASIO_NO_IOSTREAM)
        /// The UNIX domain iostream type.
        using iostream = boost::asio::basic_socket_iostream<seqpacket_protocol>;
#endif
    };
}

using seqpacket::seqpacket_protocol;

struct session : public std::enable_shared_from_this<session> {
    explicit session(seqpacket_protocol::socket socket) : socket_(std::move(socket)) {}

    ~session() {
        //std::cerr << "session closed\n";
    }
        

    void echo(boost::asio::yield_context yield) {
        auto self = shared_from_this();
        try {
            for (;;) {
                seqpacket_protocol::socket::message_flags in_flags = MSG_WAITALL, out_flags = MSG_WAITALL | MSG_EOR;
                                    //com arduino
            const char *device ="ttyUSB0";
                    int cport_nr= 24;//RS232_GetPortnr(device); /* /dev/ttyUSB0 */
                    int bdrate=9600; /* 9600 baud */

                    char mode[]={'8','N','1',0}; // 8 data bits, no parity, 1 stop bit
                    char str_send[2][BUF_SIZE]; // send data buffer
                    unsigned char str_recv[BUF_SIZE]; // recv data buffer
            printf("Dans for\n");
                    if(RS232_OpenComport(cport_nr, bdrate, mode))
                    {
                        printf("Can not open comport\n");
                    }

                    // usleep(2000000);  /* waits 2000ms for stable condition */
                    // while(1)
                    //{

                    // Wait for the message from the client
                    auto bytes_transferred = socket_.async_receive(boost::asio::buffer(data_), in_flags, yield);
                    std::string recu;
                    for(int i=0 ; data_[i] != '\0' ; ++i)
                    {
                        recu += string(1,data_[i]);
                    }
              if (recu == "{\"keycodes\":[44]}"){
                 recu = "z";
              }
              else if (recu == "{\"keycodes\":[31]}"){
                 recu = "s";
              }
              else if (recu == "{\"keycodes\":[16]}"){
                 recu = "q";
              }
              else if (recu == "{\"keycodes\":[32]}"){
                 recu = "d";
              }
              else if (recu == "{\"keycodes\":[18]}"){
                 recu = "e";
              }
              else if (recu == "{\"keycodes\":[30]}"){
                 recu = "a";
              }
              else if ((recu == "{\"keycodes\":[33]}") || (recu == "stop")){
                 recu = "f";
              }
              else if (recu == "{\"keycodes\":[105]}") {
                 recu = "V1";
              }
              else {
                  recu = "n";
              } 
            std::cout << "recu :" << recu << std::endl;
            recu = "";
                    //strcpy(str_send[0], recu.c_str());
                    //strcpy(str_send[1], "puc");
                    RS232_cputs(cport_nr, recu.c_str()); // sends string on serial
                    printf("Sent to Arduino: '%s'\n", recu.c_str());

               

/////////////////////////////////////lecture GPS////////////////////////////
                std::ifstream file( "bufferGPS" ); 
                if ( !file ) 
                { 
                    std::cerr << "Erreur d'ouverture\n"; 
                    return; 
                } 
      
                std::string line; 
                if ( ! ( file >> line ) ) 
                { 
                    std::cerr << "Erreur de lecture\n"; 
                    return; 
                } 
                std::cout << "Ligne lue : " << line; 
                std::string dataToSend;
                dataToSend = line;
                // Write the same message back to the client
                socket_.async_send(boost::asio::buffer(dataToSend, dataToSend.size()), out_flags, yield);
            }
        } catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
            socket_.close();
        }
    }

    void go() {
        boost::asio::spawn(socket_.get_io_service(), std::bind(&session::echo, this, std::placeholders::_1));
    }

private:
    seqpacket_protocol::socket socket_;
    std::array<char, MAX_PACKET_SIZE> data_;
};

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: datachannel_server <file> (e.g. /tmp/uv4l.socket)\n";
            std::cerr << "*** WARNING: existing file is removed ***\n";
            return EXIT_FAILURE;
        }

        boost::asio::io_service io_service;

        std::remove(argv[1]);

        boost::asio::spawn(io_service, [&](boost::asio::yield_context yield) {
                    seqpacket_protocol::acceptor acceptor_(io_service, seqpacket_protocol::endpoint(argv[1]));
                    for (;;) {
                        boost::system::error_code ec;
                        seqpacket_protocol::socket socket_(io_service);
                        acceptor_.async_accept(socket_, yield[ec]);
                        if (!ec)
                            std::make_shared<session>(std::move(socket_))->go();
                    }
                });

        io_service.run();

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
}
