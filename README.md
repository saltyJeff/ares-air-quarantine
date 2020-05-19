# Ares Air Quarantine Edition
## Hardware utilized
| Component | Model Number | Serial Protocol | Update Rate (hz) |
|---|---|---|---|
| MCU | Arduino BLE Sense 33 | N/A | N/A |
| IMU (9-DOF) | ST LSM9DS1 | I2C | 80 |
| Atmosphere Sensor | ST HTS221 | I2C | 12.5 |
| Barometer | ST LPS22HB | I2C | 75 |
| GPS | uBlox Neo | UART | 5 |
| Radio | RFD 900X | UART | N/A |
| SD Card | the really cheap ones you get at Microcenter | SPI | 4ms flush every 1 block (512 bytes) written |

## Packet Format
We use a variant of the HDLC framing scheme.

Escaping is performed as follows:
* If the character C to be encoded is 0x7E or 0x7D:
    * Write the escape character 0x7D
    * Write C ^ 0x20
* Else, just write the original character C

Byte order is little endian

### On the wire layout
* 0x7E
* Escaped Data
* Escaped Checksum (calculated over the data (without the 0x7E's) before escaping happens)
* 0x7E

The data is just a packed struct pointer that has been casted to a string. Its definition can be found in [TelemData.h](include/TelemData.h)

Subfield definitions can be found in [commondefs.h](include/commondefs.h)

## Supported Commands (send over radio or USB)
| Command | Short Desc | Description |
|---|---|---|
| R | Report | return the status and performance statistics of each component |
| B | Benchmark | benchmark each component and return their status/performance reports |
| D | Dump | force a transmission of all the sensor data current stored |