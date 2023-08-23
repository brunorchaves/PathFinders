# include <HardwareSerial.h>

HardwareSerial SerialPort(2);

char pkt = ' ';
char led = 2; //Led interno esp32-wroom

int RXD = 16;
int TXD = 17;

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
 SerialPort.begin(230400, SERIAL_8N1,16,17); //UART 2
 Serial.begin(230400);
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






