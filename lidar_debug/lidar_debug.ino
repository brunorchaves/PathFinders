# include <HardwareSerial.h>

HardwareSerial SerialPort(2);

char pkt = ' ';
char led = 2; //Led interno esp32-wroom

int RXD = 16;
int TXD = 17;


void setup() {
 SerialPort.begin(230400, SERIAL_8N1,16,17); //UART 2
 Serial.begin(230400);
}

void loop() {
  while(SerialPort.available())
  {

  int ver_len = 0;
  int cont = 0;
  int val;
  int media = 0;
  int med_cont = 0;

  char msg[50];

  char speed[2];
  char start_ang[2];
  char data[3][12];
  char end_ang[2];
  char timestamp[2];
  char checksum;

  char data_rcvd = SerialPort.read();   // read one byte from serial buffer and save to data_rcvd

    if((data_rcvd == 0x54))
    {
      msg[0] = 0x54;

      for(int i = 1; i < 50; i++)
      {
          data_rcvd = SerialPort.read();
          msg[i] = data_rcvd;
      }


    switch(msg[1])
    {
      case (0x21):
        ver_len = 1;
        break;
      case (0x22):
        ver_len = 2;
        break;
      case (0x23):
        ver_len = 3;
        break;
      case (0x24):
        ver_len = 4;
        break;
      case (0x25):
        ver_len = 5;
        break;
      case (0x26):
        ver_len = 6;
        break;
      case (0x27):
        ver_len = 7;
        break;
      case (0x28):
        ver_len = 8;
        break;
      case (0x29):
        ver_len = 9;
        break;
      case (0x2A):
        ver_len = 10;
        break;
      case (0x2B):
        ver_len = 11;
        break;
      case (0x2C):
        ver_len = 12;
        break;
    }



    union {
      struct{
        uint8_t   byte1;
        uint8_t   byte2;
      };
      uint16_t    word;
    } speedByte;

    Serial.print("\n");
    speed[1] = msg[2];
    speed[0] = msg[3];
    speedByte.byte1 = speed[1];
    speedByte.byte2 = speed[0];
    Serial.print("Speed is: ");
    Serial.println(speedByte.word);

    union {
      struct{
        uint8_t   byte1;
        uint8_t   byte2;
      };
      uint16_t    word;
    } startangByte;


    start_ang[1] = msg[4];
    start_ang[0] = msg[5];
    startangByte.byte1 = start_ang[1];
    startangByte.byte2 = start_ang[0];
    Serial.print("Start angle: ");
    Serial.println(startangByte.word);

    union {
      struct{
        uint8_t   byte1;
        uint8_t   byte2;
      };
      uint16_t    word;
    } dataByte;

    cont = 6;

      for(int i = 0; i<ver_len ; i++)
      {
        dataByte.byte1 = msg[cont];
        cont++;
        dataByte.byte2 = msg[cont];
        cont++;
        data[2][i] = msg[cont];
        cont++;

        Serial.print("Distance : ");
        Serial.print(dataByte.word);
        Serial.print("mm | Strength: ");
        Serial.println(data[2][i], DEC);

        if (dataByte.word != 0)
        {
          media = media + dataByte.word;
          med_cont++;
        }
      }

      if(med_cont != 0)
      {
        Serial.print("Media : ");
        Serial.println(media/med_cont);
      }

      end_ang[1] = msg[cont];
      cont++;
      end_ang[0] = msg[cont];
      cont++;

    union {
      struct{
        uint8_t   byte1;
        uint8_t   byte2;
      };
      uint16_t    word;
    } endangByte;

    endangByte.byte1 = end_ang[1];
    endangByte.byte2 = end_ang[0];
    Serial.print("End angle: ");
    Serial.println(endangByte.word);

    timestamp[0] = msg[cont];
    cont++;
    timestamp[1] = msg[cont];
    cont++;

    union {
      struct{
        uint8_t   byte1;
        uint8_t   byte2;
      };
      uint16_t    word;

    } tstampByte;

    tstampByte.byte1 = timestamp[0];
    tstampByte.byte2 = timestamp[1];
    Serial.print("Timestamp : ");
    Serial.println(tstampByte.word);

    checksum = msg[cont];

    Serial.print("Checksum : ");
    Serial.print(checksum, DEC);
    Serial.print("\n");
    Serial.print("--------------------------------");

    cont = 0;
    media = 0;
    med_cont = 0;
    }
  }
}