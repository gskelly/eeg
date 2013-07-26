#include "bbspi.h"

/*Return a file descriptor that corresponds to the selected SPI.*/
int bbspi_init(char *spidev, struct spi_ioc_transfer *ptr,
               void *TXbuffer, void * RXbuffer, uint32_t length){
    /*Creates file descriptor*/
    int fd = open(spidev, O_RDWR);
    if (fd < 0)
        perror("file open");

    /*Set attributes to user inputs*/
    ptr->tx_buf = (unsigned long)TXbuffer;
    ptr->rx_buf = (unsigned long)RXbuffer;
    ptr->len = length;

    /*Arbitrary default values. They can be changed by direct access or by ioctl().
    Check "linux/spi/spidev.h" and spidev documentation for more information*/
    ptr->delay_usecs = 0;
    ptr->speed_hz = 0;
    ptr->bits_per_word = 0;

    /*Default mode is 1 (CPOL = 0; CPHA = 1)*/
    ioctl(fd, SPI_IOC_WR_MODE, SPI_MODE_1);

    return fd;
}

/*Send and receive data*/
void bbspi_transfer(int fd, struct spi_ioc_transfer *ptr){
    int ret = ioctl(fd, SPI_IOC_MESSAGE(1), ptr);

    if (ret < 0) {
        perror("SPI data_transfer");
    }
}

/*Check if SPI is ready to transfer data*/
int bbspi_ready(int fd){
    int mode;
    int ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);

    if (ret < 0)
        perror("SPI SPI_ready");

    return (mode&SPI_READY == SPI_READY);
}

