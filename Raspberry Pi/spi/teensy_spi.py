import spidev

spi_bus = 0
spi_device = 0

spi = spidev.SpiDev()
spi.open(spi_bus, spi_device)
spi.max_speed_hz = 1000000


# Send a null byte to check for value
send_byte = 0x80
rcv_byte = spi.xfer2([send_byte])

# repeat to check for a response
rcv_byte = spi.xfer2([send_byte])

data_recv = rcv_byte[0]

if (data_recv != 0x80):
    print ("Unable to communicate with Arduino "+str(data_recv))
    quit()

