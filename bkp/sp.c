#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/select.h>

int open_port(){
    
    int fd;
    fd = open("/dev/ttyS2", O_RDWR | O_NOCTTY | O_NDELAY);
    
    if(fd == -1){
        printf("Unable to open port.\n");
    } else {
        fcntl(fd, F_SETFL, 0);
        printf("Port is open.\n");
    }
    
    return fd;
}

int configure_port(int fd){

    struct termios psett;

    cfsetispeed(&psett, B115200);
    cfsetospeed(&psett, B115200);
    
    psett.c_cflag &= ~PARENB;
    psett.c_cflag &= ~CSTOPB;
    psett.c_cflag &= ~CSIZE;
    psett.c_cflag |= ~CS8;

    tcsetattr(fd, TCSANOW, &psett);
}

int send_data(){

    fd_set rdfs;
    struct timeval timeout;

    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    
    unsigned char send_bytes[] = { 0x00 };

    return 0;
}

main()
{
    printf("Running serial port test.\n");
    int fd = open_port();
    configure_port(fd);
}
