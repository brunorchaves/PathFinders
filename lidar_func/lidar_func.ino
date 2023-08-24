# include <HardwareSerial.h>

HardwareSerial SerialPort(0);

char pkt = ' ';
char led = 2; //Led interno esp32-wroom

int RXD = 3;
int TXD = 1;

static const uint8_t CrcTable[256]
={ 0x00, 0x4d, 0x9a, 0xd7, 0x79, 0x34,0xe3,
0xae, 0xf2, 0xbf, 0x68, 0x25, 0x8b, 0xc6, 0x11, 0x5c, 0xa9, 0xe4, 0x33,
0x7e, 0xd0, 0x9d, 0x4a, 0x07, 0x5b, 0x16, 0xc1, 0x8c, 0x22, 0x6f, 0xb8,
0xf5, 0x1f, 0x52, 0x85, 0xc8, 0x66, 0x2b, 0xfc, 0xb1, 0xed, 0xa0, 0x77,
0x3a, 0x94, 0xd9, 0x0e, 0x43, 0xb6, 0xfb, 0x2c, 0x61, 0xcf, 0x82, 0x55,
0x18, 0x44, 0x09, 0xde, 0x93, 0x3d, 0x70, 0xa7, 0xea, 0x3e, 0x73, 0xa4,
0xe9, 0x47, 0x0a, 0xdd, 0x90, 0xcc, 0x81, 0x56, 0x1b, 0xb5, 0xf8, 0x2f,
0x62, 0x97, 0xda, 0x0d, 0x40, 0xee, 0xa3, 0x74, 0x39, 0x65, 0x28, 0xff,
0xb2, 0x1c, 0x51, 0x86, 0xcb, 0x21, 0x6c, 0xbb, 0xf6, 0x58, 0x15, 0xc2,
0x5f, 0xf1, 0xbc, 0x6b, 0x26, 0x7a, 0x37, 0xe0, 0xad, 0x03, 0x4e, 0x99,
0xd4, 0x7c, 0x31, 0xe6, 0xab, 0x05, 0x48, 0x9f, 0xd2, 0x8e, 0xc3, 0x14,
0x59, 0xf7, 0xba, 0x6d, 0x20, 0xd5, 0x98, 0x4f, 0x02, 0xac, 0xe1, 0x36,
0x7b, 0x27, 0x6a, 0xbd, 0xf0, 0x5e, 0x13, 0xc4, 0x89, 0x63, 0x2e, 0xf9,
0xb4, 0x1a, 0x57, 0x80, 0xcd, 0x91, 0xdc, 0x0b, 0x46, 0xe8, 0xa5, 0x72,
0x3f, 0xca, 0x87, 0x50, 0x1d, 0xb3, 0xfe, 0x29, 0x64, 0x38, 0x75, 0xa2,
0xef, 0x41, 0x0c, 0xdb, 0x96, 0x42, 0x0f, 0xd8, 0x95, 0x3b, 0x76, 0xa1,
0xec, 0xb0, 0xfd, 0x2a, 0x67, 0xc9, 0x84, 0x53, 0x1e, 0xeb, 0xa6, 0x71,
0x3c, 0x92, 0xdf, 0x08, 0x45, 0x19, 0x54, 0x83, 0xce, 0x60, 0x2d, 0xfa,
0xb7, 0x5d, 0x10, 0xc7, 0x8a, 0x24, 0x69, 0xbe, 0xf3, 0xaf, 0xe2, 0x35,
0x78, 0xd6, 0x9b, 0x4c, 0x01, 0xf4, 0xb9, 0x6e, 0x23, 0x8d, 0xc0, 0x17,
0x5a, 0x06, 0x4b, 0x9c, 0xd1, 0x7f, 0x32, 0xe5, 0xa8
};

uint8_t CalCRC8(uint8_t *p, uint8_t len)
{ 
uint8_t crc = 0;
uint16_t i;

  for (i = 0; i < len; i++)
  {
  crc = CrcTable[(crc ^ *p++) & 0xff];
  }

return crc;
}



struct ponto
{
  int dist,forca;
};

struct dados
{
  int vel,s_ang,e_ang,times,med,conf;
  struct ponto pontos[];
};


void setup() {
 SerialPort.begin(230400, SERIAL_8N1,RXD,TXD); //UART 2
 typedef struct dados Struct;
}

void loop() 
{
  typedef struct dados Struct;
  Struct s;

    if(SerialPort.available())
    {
    int ver_len = 0;
    int cont = 0;
    int val;
    int media = 0;
    int med_cont = 0;
    int conf = 0;

    cont = 0;
    media = 0;
    med_cont = 0;
    conf = 0;

    char msg[50];
    char speed[2];
    char start_ang[2];
    char data[3][12];
    char end_ang[2];
    char timestamp[2];

    union 
    {
      struct
      {
        uint8_t   byte1;
        uint8_t   byte2;
      };
      uint16_t    word;
    } speedByte;

    union 
    {
      struct
      {
        uint8_t   byte1;
        uint8_t   byte2;
      };
      uint16_t    word;
    } startangByte;

    union 
    {
      struct
        {
          uint8_t   byte1;
          uint8_t   byte2;
        };
      uint16_t    word;
    } dataByte;

    union 
    {
      struct
        {
          uint8_t   byte1;
          uint8_t   byte2;
        };
      uint16_t    word;
    } endangByte;

    union 
    {
      struct
        {
          uint8_t   byte1;
          uint8_t   byte2;
        };
      uint16_t    word;
    } tstampByte;

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

      speed[1] = msg[2];
      speed[0] = msg[3];
      speedByte.byte1 = speed[1];
      speedByte.byte2 = speed[0];

      start_ang[1] = msg[4];
      start_ang[0] = msg[5];
      startangByte.byte1 = start_ang[1];
      startangByte.byte2 = start_ang[0];

      cont = 6;

      for(int i = 0; i<ver_len ; i++)
      {
          dataByte.byte1 = msg[cont];
          cont++;
          dataByte.byte2 = msg[cont];
          cont++;
          data[2][i] = msg[cont];
          
          s.pontos[i].dist = dataByte.word;
          s.pontos[i].forca = msg[cont];

          cont++;
          if (dataByte.word != 0)
          {
            media = media + dataByte.word;
            conf = conf + msg[cont - 1];
            med_cont++;
          }
      }

      if(med_cont != 0)
      {
        s.med = (media/med_cont);
        s.conf = (conf/med_cont);
      }
      else
      {
        s.med = 0;
        s.conf = 0;
      }

      end_ang[1] = msg[cont];
      cont++;
      end_ang[0] = msg[cont];
      cont++;
      endangByte.byte1 = end_ang[1];
      endangByte.byte2 = end_ang[0];

      timestamp[0] = msg[cont];
      cont++;
      timestamp[1] = msg[cont];
      cont++;
      tstampByte.byte1 = timestamp[0];
      tstampByte.byte2 = timestamp[1];

      s.vel = speedByte.word;
      s.s_ang = startangByte.word;
      s.e_ang = endangByte.word;
      s.times = tstampByte.word;
      }
    }
  
  if((s.med <= 500) && (s.conf >= 200))
  {
    if(s.s_ang >= 9000 && s.s_ang <= 15000)
    {
      Serial.println("Obstaculo esquerda");
    }
    else if(s.s_ang >= 15000 && s.s_ang <= 21000)
    {
      Serial.println("Obstaculo frente");
    }
    else if(s.s_ang >= 21000 && s.s_ang <= 27000)
    {
      Serial.println("Obstaculo direita");
    }
  }
}






