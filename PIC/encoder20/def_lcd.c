#define RS RC3
#define RW RC4
#define E RC5
#define LCD PORTD

// CONST Char gd[4][21] = {"    Thap Chuong     ",
//                         "                    ",
//                         "Sensor 1:           ",
//                         "Sensor 2:           "};
char arrNumber[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

void ghima(int8 data)
{
  LCD = data;
  RS = 0;
  RW = 0;
  E = 1;
  E = 0;
  delay_us(1500);
}
void hienthi(int8 data)
{
  LCD = data;
  RS = 1;
  RW = 0;
  E = 1;
  E = 0;
  delay_us(1500);
}

void initLCD()
{
  TRISD = 0;
  TRISC3 = TRISC4 = TRISC5 = 0;

  ghima(0x38); //giao tiep 8bit, hien thi 2 dong, kich thuoc 5x7
  ghima(0x0C); //bat man hinh
  ghima(0x01); //xoa man hinh
  ghima(0x80); // dua con tro ve du dong 1

  // int8 i, j;
  // for (i = 0; i < 4; i++)
  //   for (j = 0; j < 20; j++)
  //   {
  //     hienthi(gd[i][j]);
  //     if ((j == 19) && (i == 0))
  //       ghima(0xc0);
  //     if ((j == 19) && (i == 1))
  //       ghima(0x94);
  //     if ((j == 19) && (i == 2))
  //       ghima(0xd4);
  //   }
}
