#include <hmi_test/udp_socket.h>

HMI_Test::HMI_Test()
{
    memset(&DestInfo, 0, sizeof(DestInfo));
    memset(&SrcInfo, 0, sizeof(SrcInfo));
    memset(&Buffer, 0, PACKET_SIZE);
    memset(&for_HMI_Packet, 0, sizeof(for_HMI_Packet));

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if(sock < 0)
    {
        perror("socket");
        exit(0);
    }

    const int broadcast_enable = 1;

    int ret = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable));
    
    if(ret)
    {
        ROS_ERROR("Error in setting broadcast option");
        close(sock);
        exit(0);
    }

    DestInfo.sin_family = AF_INET;
    DestInfo.sin_port = htons(UDP_PORT);

    // DestInfo.sin_addr.s_addr = htonl(INADDR_ANY);
    DestInfo.sin_addr.s_addr = inet_addr(DEST_IP);
    // DestInfo.sin_addr.s_addr = inet_addr("192.168.1.195");
    SrcInfo.sin_family = AF_INET;
    SrcInfo.sin_port = htons(UDP_PORT);
    SrcInfo.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sock, (struct sockaddr*)&SrcInfo, sizeof(SrcInfo)) < 0)
    {
        perror("bind");
        exit(0);
    }

    for_HMI_Packet.sync[0] = 0xAB;
    for_HMI_Packet.sync[1] = 0xCD;

}

HMI_Test::~HMI_Test()
{

}

void HMI_Test::end(int sig)
{
    if(sig == 2)
    {
        int test = shutdown(sock, SHUT_RDWR);
        test = close(sock);
        std::cout << "socket end : " << test << std::endl;
    }
}

void HMI_Test::callback_chassis(const hmi_test::chassis_msg& data)
{
    for_HMI_Packet.Bus_Speed = data.speed;
    for_HMI_Packet.Drive_Mode = data.autonomous_mode;
}

void HMI_Test::callback_local(const hmi_test::localization2D_msg_bus& data)
{
    int ret;

    for_HMI_Packet.Bus_Position_East = data.east;
    for_HMI_Packet.Bus_Position_North = data.north;

    memcpy(Buffer, (const void*)&for_HMI_Packet, sizeof(for_HMI_Packet));
    
    ret = sendto(sock, (const char*)&Buffer, sizeof(for_HMI_Packet), 0, (struct sockaddr *)&DestInfo, sizeof(DestInfo));

    ROS_INFO("LOCAL Callback ret: %d speed: %.2lf", ret, for_HMI_Packet.Bus_Speed);
}
        