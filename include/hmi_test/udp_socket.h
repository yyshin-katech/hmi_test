#ifndef _UDP_SOCKET_H_
#define _UDP_SOKCET_H_

#include <fstream>
#include <iostream>
#include <vector>
#include <cstdio>
#include <iostream>
#include <stdio.h>
#include <cerrno>
#include <cstring>
#include <sstream>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ros/ros.h>

#include <hmi_test/chassis_msg.h>
#include <hmi_test/localization2D_msg_bus.h>

#define UDP_PORT 13000
#define DEST_IP "192.168.100.255"
#define PACKET_SIZE 100

typedef struct sforHMI{
    uint8_t     sync[2];
    uint8_t     Drive_Mode;
    double      Bus_Speed;
    double      Bus_Position_East;
    double      Bus_Position_North;
} __attribute__((packed)) for_HMI;

static int sock;

class HMI_Test{

    public:
        HMI_Test();
        ~HMI_Test();

        hmi_test::chassis_msg msg_chassis;
        hmi_test::localization2D_msg_bus msg_localization;

        for_HMI for_HMI_Packet;

        struct sockaddr_in DestInfo;
        struct sockaddr_in SrcInfo;

        uint8_t Buffer[PACKET_SIZE] = {0, };

        void loop();

        static void end(int sig);

        void callback_chassis(const hmi_test::chassis_msg& data);
        void callback_local(const hmi_test::localization2D_msg_bus& data);


};

#endif