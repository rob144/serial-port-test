#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/select.h>
#include <time.h>

typedef unsigned char byte_t;

/* ****************************************************
* Function: open_port
* ****************************************************/
int open_port(char* tty_path){
    
    int fd;

    printf("Attempting to open port: %s\n", tty_path);

    fd = open(tty_path, O_RDWR | O_NOCTTY | O_NDELAY);
    
    if(fd == -1){
        printf("Unable to open port.\n");
    } else {
        fcntl(fd, F_SETFL, 0);
        printf("Port is open.\n");
    }
    
    return fd;
}

/* ****************************************************
* Function: configure_port
* ****************************************************/
int configure_port(int fd){

    struct termios portSett;

    cfsetispeed(&portSett, B115200);
    cfsetospeed(&portSett, B115200);
    
    portSett.c_cflag &= ~PARENB;
    portSett.c_cflag &= ~CSTOPB;
    portSett.c_cflag &= ~CSIZE;
    portSett.c_cflag |= ~CS8;

    tcsetattr(fd, TCSANOW, &portSett);
}

/* ****************************************************
* Function: send_data
* ****************************************************/
int send_data(int fd){

    fd_set rdfs;
    struct timeval timeout;

    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    
    byte_t cmdResetReceiver = { 0xA };
    byte_t cmdStopReceiverAndSensor = { 0x0 };
    
    //Write the output to the port
    write(fd, cmdResetReceiver, 1);
    printf("Sent cmdResetReceiver.\n");
    usleep(200000);

    write(fd, cmdStopReceiverAndSensor, 1);
    printf("Sent cmdStopReceiverAndSensor.\n");
    usleep(200000);

    //Read the response (if any) from the port
    char outBuff[8];
    int result = -1;
    result = read(fd, outBuff, sizeof(outBuff));
    printf("Result: %d\n\n", result);

    return 0;
}

/* ****************************************************
* Function: main
* ****************************************************/
main()
{
    char tty_path[10] = "/dev/ttyS2";
    int fd = open_port(tty_path);

    printf("Running serial port test.\n");
    printf("FD: %d\n", fd);

    configure_port(fd);
    send_data(fd);
}
