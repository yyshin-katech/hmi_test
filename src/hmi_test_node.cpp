#include <hmi_test/udp_socket.h>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "HMI_Test");
    ros::NodeHandle node("~");

    HMI_Test hmi_test;

    ros::AsyncSpinner spinner(0);
    spinner.start();

    ros::Subscriber sub1 = node.subscribe("/sensors/chassis", 1, &HMI_Test::callback_chassis, &hmi_test);
    ros::Subscriber sub2 = node.subscribe("/udp/localization", 1, &HMI_Test::callback_local, &hmi_test);

    ros::waitForShutdown();
}