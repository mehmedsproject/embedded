#include <pin_magic.h>
#include <registers.h>
#include <SPFD5408_Adafruit_GFX.h>
#include <SPFD5408_Adafruit_TFTLCD.h>
#include <SPFD5408_TouchScreen.h>
#include <SPI.h>
#include <SD.h>
#include <math.h>

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7
// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).

// For Arduino Mega
//  connect the SD card with CS going to pin 53, DI going to pin 51, DO going to pin 50 and SCK going to pin 52 (standard)

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

#define SD_CS 53     // Set the chip select line to whatever you use

#else

// For Arduino Uno/Duemilanove, etc
//  connect the SD card with DI going to pin 11, DO going to pin 12 and SCK going to pin 13 (standard)
//  Then pin 10 goes to CS (or whatever you have set up)

#define SD_CS 10     // Set the chip select line to whatever you use (10 doesnt conflict with the library)

#endif

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

// Calibrate values
#define TS_MINX 170
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 860

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin
// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define kutu 40
#define PENRADIUS 3

#define MINPRESSURE 1
#define MAXPRESSURE 1000


/* --------------------------------------------------- */
bool DokunmaAlani(uint16_t, uint16_t, uint16_t, uint16_t);
void dokunmatik_ayar(TSPoint);
void Sicaklik_grafigi(float, float);
float DegerAyarla(int, String, String, int, String, int*);
void ButonEkle(uint16_t,uint16_t,uint16_t,uint16_t,uint8_t,String,uint16_t,uint16_t,uint16_t,uint16_t);

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Adafruit_TFTLCD tft;
float x = 0, t = 0;
int dakika[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int derece[18] ={20,40,60,80,100,120,140,160,180,200,220,240,260,280,300,320,340,360}; 
int profil[4] ={1,2,3,4}; 
bool manuel_flag;
int bolum;
uint8_t Profil;
int saniye = 0,dk = 0, art = 0;
TSPoint p;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  tft.reset();
  uint16_t identifier = tft.readID();

  if(identifier == 0x9325) {
    progmemPrintln(PSTR("Found ILI9325 LCD driver"));
  } else if(identifier == 0x9328) {
    progmemPrintln(PSTR("Found ILI9328 LCD driver"));
  } else if(identifier == 0x7575) {
    progmemPrintln(PSTR("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
    progmemPrintln(PSTR("Found SPFD5408 LCD driver"));
  }
    else {
    progmemPrint(PSTR("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    progmemPrintln(PSTR("If using the Adafruit 2.8\" TFT Arduino shield, the line:"));
    progmemPrintln(PSTR("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
    progmemPrintln(PSTR("should appear in the library header (Adafruit_TFT.h)."));
    progmemPrintln(PSTR("If using the breakout board, it should NOT be #defined!"));
    progmemPrintln(PSTR("Also if using the breakout, double-check that all wiring"));
    progmemPrintln(PSTR("matches the tutorial."));
    return;
  }
  progmemPrint(PSTR("Initializing SD card..."));
  if (!SD.begin(SD_CS)) {
    progmemPrintln(PSTR("failed!"));
    return;
  }
  progmemPrintln(PSTR("OK!"));
  tft.begin(0x9341); // SDFP5408
  tft.setRotation(1); // Need for the Mega, please changed for your choice or rotation initial
  //  Acilis_Ekrani();
  /*Touchscreen setup*/
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  p = ts.getPoint();
  digitalWrite(13, LOW);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  /*Timer ayarları*/
  
  cli();
  /* Ayarlamaların yapılabilmesi için öncelikle kesmeler durduruldu */

  /* Timer1 kesmesi saniyede bir çalışacak şekilde ayarlanacaktır (1 Hz)*/
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 15624;
  /* Bir saniye aralıklar için zaman sayıcısı ayarlandı */
  TCCR1B |= (1 << WGM12);
  /* Adımlar arasında geçen süre kristal hızının 1024'e bölümü olarak ayarlandı */
  TCCR1B |= (1 << CS12) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);
  /* Timer1 kesmesi aktif hale getirildi */

  
  /* Timer1 kesmesinin çalışabilmesi için tüm kesmeler aktif hale getirildi */
/*-----------------------------------*/
  /*end of setup*/
 // DenemeAlani();
  dokunmatik_ayar(p);
//  Acilis_Ekrani();
 tft.fillScreen(WHITE);
  //ButonEkle(105,177,108, 40, 2,"iLERi",YELLOW,MAGENTA,RED,CYAN); // 105 ve 177 butonun x ye y başlangıç noktası,108 ve 40 dikdörtgenin en ve boy değerleridir. 2 Yazı boyutudur.
  AnaEkran();
  Sicaklik_grafigi(x,t);
//  SicaklikProfili(x,t,manuel_flag);
  sei();
}

void loop() {
 //while(DokunmaAlani(0, 0, 0, 0) == true);
}
void ArctanProfili(float X_StretchCompress,float Y_StretchCompress,uint8_t SabitlemeDegeri,int8_t X_Position,int8_t Y_Position,uint16_t Renk)
{
  uint16_t i;
  for(i = 1; i < 159; i++)
  {
   tft.drawPixel(i + 159 + X_Position,Y_Position + 119 + Y_StretchCompress*75*-atan(X_StretchCompress*i*(360.0/318.0)*3.1416/180.0),Renk);
   tft.drawPixel(160 - i + X_Position,Y_Position + 119 + Y_StretchCompress*75*atan(X_StretchCompress*i*(360.0/318.0)*3.1416/180.0),Renk);
   delay(10);
  }
}
void LineerProfili(float X_StretchCompress,float Y_StretchCompress,uint8_t SabitlemeDegeri,int8_t X_Position,int8_t Y_Position,uint16_t Renk)
{
  uint16_t i,j;
  for(i = 1,j = 239; i < 319,j > 0; i++,j--)
  {
   tft.drawPixel(X_Position + i * X_StretchCompress,Y_Position + j * Y_StretchCompress ,Renk);
   delay(10);
  }
}
void LogaritmaProfili(float X_StretchCompress,float Y_StretchCompress,uint8_t SabitlemeDegeri,int8_t X_Position,int8_t Y_Position,uint16_t Renk)
{
  uint16_t i,j;
  for(i = 1,j = 239; i < 319,j > 0; i++,j--)
  {
   tft.drawPixel(X_Position + i * 1.3,Y_Position + 189 - Y_StretchCompress * 85 * log(0.5 + i * X_StretchCompress * (10.0/319.0)) ,Renk);   
   delay(10);
  }
}
void DenemeAlani()
{
  while(1){
  uint16_t i,j;
  tft.fillScreen(WHITE);
 /* for(i = 1; i < 318; i++)
  {
   tft.drawPixel(i,119 + 95*-sin(i*(360.0/318.0)*3.1416/180.0),BLUE);
   tft.drawPixel(i,119 + 95*-cos(i*(360.0/318.0)*3.1416/180.0),RED);
   tft.drawPixel(i + 158,119 + 75*-atan(i*(360.0/318.0)*3.1416/180.0),MAGENTA);
   if(i < 159) tft.drawPixel(159 - i,119 + 75*atan(i*(360.0/318.0)*3.1416/180.0),MAGENTA);
   delay(100);
  }*/
  ArctanProfili(1,1,0,50,-20,MAGENTA);
  LineerProfili(1.333,1,0,0,0,RED);
  LogaritmaProfili(0.8,0.4,0,50,-50,RED);
  }
}
/* Arduino otomatik olarak her saniye aşağıdaki fonksiyonu çalıştıracaktır */
ISR(TIMER1_COMPA_vect){
  tft.setTextColor(BLACK); tft.setTextSize(1);
  tft.fillRect(28+((60/t)*art), 222, 30, 10, WHITE);
  if(dk < t)
  {
  tft.setCursor(28+((60/t)*art),223);
  tft.print("|");tft.print(dk); tft.print(":"); if(saniye < 10) tft.print("0"); tft.print(saniye);
 // tft.fillRect(28, 222, (60/t)*art, 10, YELLOW);
  if(art > 0)
   tft.fillRect(28+((60/t)*(art-1)), 222, (60/t), 10, YELLOW);
  saniye++;
  if((saniye % 15) == 0) art++;
  if(saniye == 60) {saniye = 0; dk++;}
  }
  else if(dk >= t)
  {
    tft.setCursor(28+((60/t)*art),223);
    tft.print("|");tft.print(dk); tft.print(":"); if(saniye < 10) tft.print("0"); tft.print(saniye);
    tft.fillRect(28, 222, (60/t)*art, 10, YELLOW);
    dk = 0;
    for(;;);
  }
  
}

void AnaEkran()
{
 // bmpDraw("house.bmp", 0, 0);
  ButonEkle(20,50,128, 40, 2,"LEHiMLEME",YELLOW,MAGENTA,RED,CYAN);
  ButonEkle(20,155,128, 40, 2,"TOP DiZME",YELLOW,MAGENTA,RED,CYAN);
  ButonEkle(170,50,128, 40, 2,"SOKME",YELLOW,MAGENTA,RED,CYAN);
  ButonEkle(170,155,128, 40, 2,"MANUEL",YELLOW,MAGENTA,RED,CYAN);
  if(DokunmaAlani(20, 148, 50, 90) == true) // (DokunmaAlani( ekran_x, ekran_x + dikdörtgen_eni, ekran_y, ekran_y + dikdörtgen_boyu) == true)
  {
   manuel_flag = false;
   ButonAnimasyonu(20,50,128, 40, 2,"LEHiMLEME",YELLOW,MAGENTA,RED,CYAN); // ButonAnimasyonu(ekran_x,ekran_y,dikdörtgen_eni, dikdörtgen_boyu, yazı_boyutu,"  LEHiMLEME",YELLOW,MAGENTA,RED,CYAN);
   x = 200; t = 6; // 200 derece
  // bmpDraw("1.bmp", 0, 0);
  }
  else if(DokunmaAlani(20, 148, 155, 195) == true)
  {
   manuel_flag = false;
   ButonAnimasyonu(20,155,128, 40, 2,"TOP DiZME",YELLOW,MAGENTA,RED,CYAN);
   x = 200; t = 6; // 200 derece
//   bmpDraw("3.bmp", 0, 0);
  }
  else if(DokunmaAlani(170, 298, 50, 90) == true)
  {
   manuel_flag = false;
   ButonAnimasyonu(170,50,128, 40, 2,"SOKME",YELLOW,MAGENTA,RED,CYAN);
   x = 240; t = 6; //230 derecede
 //  bmpDraw("2.bmp", 0, 0);
  }
  else if(DokunmaAlani(170, 298, 155, 195) == true)
  {
   ButonAnimasyonu(170,155,128, 40, 2,"MANUEL",YELLOW,MAGENTA,RED,CYAN);
   t = DegerAyarla(10,"SICAKLIK SURESiNi \n\n        BELiRLEYiN","10",0,"DAK",dakika);
   SicaklikDegeri_GeriDon:
   x = DegerAyarla(18,"SICAKLIK DEGERiNi \n\n        BELiRLEYiN","0  ",0,"'C",derece);
   if(x == 0)
   {
    tft.fillScreen(WHITE);
    tft.setCursor(50, 40);
    tft.setTextColor(RED); 
    tft.setTextSize(2);
    tft.println(" SICAKLIK DEGERiNi \n\n     0 'C SECEMEZSiNiZ!");
    tft.drawRect(105, 177, 2 * kutu + 28, kutu, RED);
    tft.setCursor(120, 191); tft.print("<= GERi");
    while(DokunmaAlani(103, 225, 190, 225) == false);
    tft.drawRect(110, 182, 2 * kutu + 18, kutu - 10, RED);
    tft.fillRect(110, 182, 2 * kutu + 18, kutu - 10, CYAN);
    delay(150);
    tft.drawRect(110, 182, 2 * kutu + 18, kutu - 10, RED);
    tft.setCursor(120, 191); tft.print("<= GERi");
    goto SicaklikDegeri_GeriDon;
   }
   Profil = ProfilSecimi();
   
   manuel_flag = true;
//   bmpDraw("4.bmp", 0, 0);
  }
  else
   AnaEkran();
}
void Acilis_Ekrani()
{
  tft.fillScreen(WHITE);
  tft.setCursor(60, 60);
  tft.setTextColor(RED); tft.setTextSize(3);
  tft.println("LCD EKRANLI");
  tft.setTextSize(3);
  tft.setCursor(20, 130);
  tft.println("REWORK iSTASYONU");
  tft.setCursor(50, 200); tft.setTextColor(BLUE); tft.setTextSize(2);
  tft.drawLine(100, 214, 200, 214, BLUE);
  tft.println("     TASARIM:\n      Muhammed Aslan");
  delay(3500);
  //while(true);
}
uint8_t ProfilSecimi()
{
  uint8_t x = 0;
  ProfilSecimi_GeriDon: 
  tft.fillScreen(WHITE);
  tft.setCursor(60, 40);
  tft.setTextColor(RED); 
  tft.setTextSize(2);
  tft.println("SICAKLIK PROFiLi \n\n        BELiRLEYiN");
  bmpDraw("arctan.bmp", 25, 105);
  bmpDraw("log.bmp", 95, 105);
  bmpDraw("linear.bmp", 165, 105);
  bmpDraw("ozel.bmp", 235, 105);
  tft.drawRect(105, 177, 2 * kutu + 28, kutu, RED);
  tft.setCursor(130, 191); tft.print("iLERi");
 
  while(DokunmaAlani(103, 225, 190, 225) == false)
  {
   if(DokunmaAlani(18, 85, 110, 163) == true)
   {
    tft.drawRect(24, 105, 65, 53, RED);
    tft.drawRect(94, 105, 65, 53, WHITE);
    tft.drawRect(164, 105, 65, 53, WHITE);
    tft.drawRect(234, 105, 65, 53, WHITE);
    x = 1;
   }
   else if(DokunmaAlani(88, 155, 110, 163) == true)
   {
    tft.drawRect(24, 105, 65, 53, WHITE);
    tft.drawRect(94, 105, 65, 53, RED);
    tft.drawRect(164, 105, 65, 53, WHITE);
    tft.drawRect(234, 105, 65, 53, WHITE);
    x = 2;
   }
   else if(DokunmaAlani(157, 224, 110, 163) == true)
   {
    tft.drawRect(24, 105, 65, 53, WHITE);
    tft.drawRect(94, 105, 65, 53, WHITE);
    tft.drawRect(164, 105, 65, 53, RED);
    tft.drawRect(234, 105, 65, 53, WHITE);
    x = 3;
   }
   else if(DokunmaAlani(227, 294, 110, 163) == true)
   {
    tft.drawRect(24, 105, 65, 53, WHITE);
    tft.drawRect(94, 105, 65, 53, WHITE);
    tft.drawRect(164, 105, 65, 53, WHITE);
    tft.drawRect(234, 105, 65, 53, RED);
    x = 4;
   }
  }
  tft.drawRect(110, 182, 2 * kutu + 18, kutu - 10, RED);
  tft.fillRect(110, 182, 2 * kutu + 18, kutu - 10, CYAN);
  delay(150);
  tft.drawRect(110, 182, 2 * kutu + 18, kutu - 10, RED);
  tft.setCursor(130, 191); tft.print("iLERi");
  delay(150);
  if(x == 0)
  {
    tft.fillScreen(WHITE);
    tft.setCursor(60, 40);
    tft.setTextColor(RED); 
    tft.setTextSize(2);
    tft.println("SICAKLIK PROFiLi \n\n       SECMEDiNiZ !");
    tft.drawRect(105, 177, 2 * kutu + 28, kutu, RED);
    tft.setCursor(120, 191); tft.print("<= GERi");
    while(DokunmaAlani(103, 225, 190, 225) == false);
    tft.drawRect(110, 182, 2 * kutu + 18, kutu - 10, RED);
    tft.fillRect(110, 182, 2 * kutu + 18, kutu - 10, CYAN);
    delay(150);
    tft.drawRect(110, 182, 2 * kutu + 18, kutu - 10, RED);
    tft.setCursor(120, 191); tft.print("<= GERi");
    goto ProfilSecimi_GeriDon;
  }
  return x;
}
float DegerAyarla(int sayi,String str1,String str2, int bosluk_sayisi,String str3,int* deger){
  float x = 0;
  int i = 0, j = sayi - 1;
  tft.fillScreen(WHITE);
  tft.setCursor(60, 40);
  tft.setTextColor(RED); 
  tft.setTextSize(2);
  tft.println(str1);
  tft.setCursor(105, 130);
  tft.print("<   ");  tft.print("    >"); x = str2.toInt();
  tft.setCursor(130, 130); tft.print(str2); tft.print(str3);
  tft.drawRect(105, 117, 2 * kutu + 28, kutu, RED);
  tft.drawRect(105, 177, 2 * kutu + 28, kutu, RED);
  tft.setCursor(130, 191); tft.print("iLERi");
 
  while(DokunmaAlani(103, 225, 190, 225) == false){
    if(i >= sayi) i = 0;
    if(j <= -1) j = sayi - 1;
    if (DokunmaAlani(103, 115, 125, 165) == true) {
      tft.setCursor(105, 130); tft.setTextColor(CYAN); tft.setTextSize(2); tft.print("<");
      delay(150); if(j <= -1) j = sayi - 1;
      tft.setCursor(105, 130); tft.setTextColor(RED); tft.setTextSize(2); tft.print("< ");
      tft.fillRect(125, 130, 70, 15, WHITE); tft.setCursor(130, 130); tft.print(deger[j], DEC); for(int s = 0; s < bosluk_sayisi; s++) tft.print(" "); tft.print(str3); x = deger[j];
      i = j + 1; j--;
      Serial.print("i ve j değerleri: "); Serial.print(i); Serial.print(" , "); Serial.println(j);
    }

    if (DokunmaAlani(210, 222, 125, 165) == true) {
      tft.setCursor(201, 130); tft.setTextColor(CYAN); tft.setTextSize(2); tft.print(">");
      delay(150); if(i >= sayi) i = 0;
      tft.setCursor(201, 130); tft.setTextColor(RED); tft.setTextSize(2); tft.print(">");
      tft.fillRect(125, 130, 70, 15, WHITE); tft.setCursor(130, 130); tft.print(deger[i], DEC); for(int s = 0; s < bosluk_sayisi; s++) tft.print(" "); tft.print(str3); x = deger[i];
      j = i - 1; i++;
      Serial.print("i ve j değerleri: "); Serial.print(i); Serial.print(" , "); Serial.println(j);
    }
 }
  tft.drawRect(110, 182, 2 * kutu + 18, kutu - 10, RED);
  tft.fillRect(110, 182, 2 * kutu + 18, kutu - 10, CYAN);
  delay(150);
  tft.drawRect(110, 182, 2 * kutu + 18, kutu - 10, RED);
  tft.setCursor(130, 191); tft.print("iLERi");
  delay(150);

  return x;
}

void ButonEkle(uint16_t en,uint16_t boy,uint16_t genislik,uint16_t yukseklik,uint8_t YaziBoyutu,String ButonYazisi,uint16_t ArkaPlanRengi,uint16_t YaziRengi,uint16_t KenarRengi,uint16_t ButonIcRengi)
{
  int YaziUzunlugu = ButonYazisi.length();
  tft.fillRect(en, boy, genislik, yukseklik, ArkaPlanRengi);
  tft.drawRect(en, boy, genislik, yukseklik, KenarRengi);
  
  tft.setTextSize(YaziBoyutu);
  tft.setCursor((en + 65) - (YaziUzunlugu * 6), (boy + 12));
  tft.setTextColor(YaziRengi); tft.print(ButonYazisi);

}

void ButonAnimasyonu(uint16_t en,uint16_t boy,uint16_t genislik,uint16_t yukseklik,uint8_t YaziBoyutu,String ButonYazisi,uint16_t ArkaPlanRengi,uint16_t YaziRengi,uint16_t KenarRengi,uint16_t ButonIcRengi)
{
  int YaziUzunlugu = ButonYazisi.length();
  tft.setTextSize(YaziBoyutu);
  
   tft.drawRect(en + 5, boy + 5, genislik - 10, yukseklik - 10, KenarRengi);
   tft.fillRect(en + 5, boy + 5, genislik - 10, yukseklik - 10, ButonIcRengi);
   delay(150);
   tft.drawRect(en + 5, boy + 5, genislik - 10, yukseklik - 10, KenarRengi);
   tft.setCursor((en + 65) - (YaziUzunlugu * 6), (boy + 12)); tft.print(ButonYazisi);
   delay(150);
}

void Sicaklik_grafigi(float x, float t)
{
  char  *str_dizi;

  tft.fillScreen(WHITE);
  tft.setCursor(10, 10); tft.setTextColor(BLACK); tft.setTextSize(1);
  tft.print("SICAKLIK('C)");
  tft.setCursor(270, 232); tft.setTextColor(BLACK); tft.setTextSize(1);
  tft.print("SURE(DK)");
  tft.setCursor(30, 30);
  tft.setTextColor(BLACK); tft.setTextSize(1);
  tft.fillRect(30, 30, 260, 180, CYAN);
  tft.drawLine(30, (tft.height() - 30), (tft.width() - 30), (tft.height() - 30), BLACK);
  tft.drawLine(30, 20, 30, (tft.height() - 30), BLACK);
 
  /*grafiğin zaman ekseni*/
  //çizgileri
 if(t == 7) bolum = (56.0 / t);
 else if(t == 9) bolum = (54.0 / t);
 else  bolum = (60.0 / t);
  for (int i = 28; i < 288; i = i + bolum)
  {
    tft.setCursor(i, tft.height() - 33);
    tft.print("|");
  }
  //zaman değerleri 
  for (int i = 28, l = 0; i < 288, l < t + 1; i = i + (240.0 / t), l++)
  {
    tft.setCursor(i, tft.height() - 25);
    tft.print(l);
  }
  /*grafiğin derece ekseni*/
  //çizgileri
  for (int j = 27; j < 207; j = j + (3600.0 / x)) { // j = j + 10
    tft.setCursor(28, j);
    tft.print("-");
  }
  //derece değerleri
  for (int j = 207, k = 0; j < 17, k <= x; j = j - (3600.0 / x), k = k + 20) //  k = k + 20  j = j - 10
  {
    tft.setCursor(10, j);
    tft.print(int(k));
  }
  /*kesişim noktaları*/
  for (int i = 28; i < 288; i = i + (240.0 / t)) //i = i + 10
  {
    for (int j = 26; j < 205; j = j + (3600.0 / x)) { //i = i + 10
      tft.setCursor(i, j);
      tft.print("o");
    }
  }
 switch(Profil)
 {
  case 1:
   ArctanProfili(2,0.8,0,29,-5,RED);
   break;
  case 2:
   LogaritmaProfili(1.6,0.6,0,29,-25,RED);
   break;
  case 3:
   LineerProfili(1.3,1,0,29,-55,RED);
   break;
  default:
   break;
 }
}
void SicaklikProfili(float x, float t, bool man_flag)
{
  int i;
  if(man_flag == false){
  if(x == 200 && t == 6)
  {
    SicaklikProfiliEkle(1,0,0,0,0,0,0,0,0,RED,RED,BLUE);  
    //StretchX,StretchY,MoveDecreasingXinside,MoveDecreasingY,MoveIncreasingX,IncreaseLineX,MoveLineX,MoveDecreasingX,MoveLineY,Colour1,Colour2,Colour3
  }
  else if(x == 240 && t == 6)
  {
    SicaklikProfiliEkle(0.833,0,-0.62,7,16,-6,22,16,7,RED,RED,BLUE);
  }
  }
  else
  {
   SicaklikProfiliEkle((200*t/(x*6)),0,0,0,0,0,0,0,0,RED,RED,BLUE);
  }
}

void SicaklikProfiliEkle(float StretchX,float StretchY,float MoveDecreasingXinside,float MoveDecreasingY,int8_t MoveIncreasingX,int8_t IncreaseLineX,int8_t MoveLineX,int8_t MoveDecreasingX ,int8_t MoveLineY,uint16_t Colour1,uint16_t Colour2,uint16_t Colour3)
{
  int i;
  for(i = 30; i < (215 + MoveIncreasingX); i++)
  {
   tft.drawPixel(i,(115) + (-(atan((((i*1.13207)*3.141592653589)/180) * StretchX - (2.1))*(75+StretchY))),Colour1);
   delay(2);
  }
  for(i = (215 + IncreaseLineX); i < (230 + IncreaseLineX); i++)
  {
   tft.drawPixel((i + MoveLineX), (30 + MoveLineY), Colour2);
   delay(2);
  }
  for(i = (230 + MoveDecreasingX); i < (290); i++)
  {
   tft.drawPixel(i,(110 + MoveDecreasingY) + ((cos(((((i+40)*1.13207)*3.141592653589)/180) * StretchX - (2 + MoveDecreasingXinside))*(80+StretchY))),Colour3);
   delay(2);
  }
}

// Wait one touch

TSPoint waitOneTouch() {

  // wait 1 touch to exit function

  //TSPoint p;

  do {
    p = ts.getPoint();

    pinMode(XM, OUTPUT); //Pins configures again for TFT control
    pinMode(YP, OUTPUT);

  } while ((p.z < MINPRESSURE ) || (p.z > MAXPRESSURE));

  return p;
}
bool DokunmaAlani(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2) {
  uint16_t ekran_x, ekran_y;
  //TSPoint p;

  do {
    
    p = ts.getPoint();
        
    pinMode(XM, OUTPUT); //Pins configures again for TFT control
    pinMode(YP, OUTPUT);

  } while ((p.z < MINPRESSURE ) || (p.z > MAXPRESSURE));
  ekran_y = (p.x - 180); ekran_y = int(ekran_y / 2.95833); ekran_x = (p.y - 130); ekran_x = int(ekran_x / 2.08823); 
  
  Serial.println("Dokunma alani :");
  Serial.println(x1); Serial.println(x2); Serial.println(y1); Serial.println(y2);
  
  Serial.print("Dokunulan yerin x ve y degerleri :"); Serial.print(ekran_x, DEC); Serial.print(" "); Serial.println(ekran_y, DEC);
  //delay(200);
  
  if (ekran_x > x1 && ekran_x < x2 && ekran_y > y1 && ekran_y < y2)
  {
    Serial.println("Dokundun");
    return 1;
  }

  return 0;
}
/*
double normalCDF(double value)
{
  return 0.5 * erf(-value * M_SQRT1_2);
}
*/
void dokunmatik_ayar(TSPoint p)
{
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    /*
      Serial.print("X = "); Serial.print(p.x);
      Serial.print("\tY = "); Serial.print(p.y);
      Serial.print("\tPressure = "); Serial.println(p.z);
    */
    // scale from 0->1023 to tft.width

    // *** SPFD5408 change -- Begin
    // Bug in in original code
    //p.x = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    // *** SPFD5408 change -- End
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());;
  }
}
struct yazi
{
     char *ismi;
     uint8_t boyutu;
     uint16_t rengi;
     uint16_t x_konumu;
     uint16_t y_konumu;
};
struct sekil
{
     char *ismi;
     uint8_t boyutu;
     uint16_t rengi;
     uint16_t x_konumu;
     uint16_t y_konumu;
     uint8_t yaricap;
};
struct sekil_yazi
{
     char *ismi;
     uint8_t boyutu;
     uint16_t rengi;
     uint16_t x_konumu;
     uint16_t y_konumu;
     uint8_t yaricap;
};

void TrigonometrikSinyaller()
{
  uint16_t i,j;
  tft.fillScreen(WHITE);
  for(i = 1; i < 318; i++)
  {
   tft.drawPixel(i,119 + (sin(((i*1.13207)*3.141592653589)/180 -7.3 + 180)*95),BLUE);
   delay(10);
   tft.drawPixel(i,119 + (-(cos(((i*1.13207)*3.141592653589)/180)*95)),RED);
   delay(10);
   tft.drawPixel(i,119 + (-(atan(((i*1.13207)*3.141592653589)/180 - 3.2)*95)),MAGENTA);
   delay(10);
  }
  /* Daha derli toplusu 
   *  
  for(i = 1; i < 318; i++)
  {
   tft.drawPixel(i,119+(-sin(((i*1.132)*3.14)/180)*(90-(i / 100))),RED);
   tft.drawPixel(i,119+(-cos(((i*1.132)*3.14)/180)*(90-(i / 100))),BLUE);
   tft.drawPixel(i,119+(-tan(((i*1.132)*3.14)/180)*(90-(i / 100))),MAGENTA);   
   delay(2);
  }
   yada

   for(i = 1; i < 318; i++)
  {
   tft.drawPixel(i,119 + 95*-sin(i*(360.0/318.0)*3.1416/180.0),BLUE);
   tft.drawPixel(i,119 + 95*-cos(i*(360.0/318.0)*3.1416/180.0),RED);
   tft.drawPixel(i + 158,119 + 75*-atan(i*(360.0/318.0)*3.1416/180.0),MAGENTA);
   if(i < 159) tft.drawPixel(159 - i,119 + 75*atan(i*(360.0/318.0)*3.1416/180.0),MAGENTA);
   delay(100);
  }
   */
}

// This function opens a Windows Bitmap (BMP) file and
// displays it at the given coordinates.  It's sped up
// by reading many pixels worth of data at a time
// (rather than pixel by pixel).  Increasing the buffer
// size takes more of the Arduino's precious RAM but
// makes loading a little faster.  20 pixels seems a
// good balance.

#define BUFFPIXEL 20

void bmpDraw(char *filename, int x, int y) {

  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel in buffer (R+G+B per pixel)
  uint16_t lcdbuffer[BUFFPIXEL];  // pixel out buffer (16-bit per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();
  uint8_t  lcdidx = 0;
  boolean  first = true;

  if((x >= tft.width()) || (y >= tft.height())) return;

  Serial.println();
  progmemPrint(PSTR("Loading image '"));
  Serial.print(filename);
  Serial.println('\'');
  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) {
    progmemPrintln(PSTR("File not found"));
    return;
  }

  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) { // BMP signature
    progmemPrint(PSTR("File size: ")); Serial.println(read32(bmpFile));
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    progmemPrint(PSTR("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    progmemPrint(PSTR("Header size: ")); Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if(read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      progmemPrint(PSTR("Bit Depth: ")); Serial.println(bmpDepth);
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        progmemPrint(PSTR("Image size: "));
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if((x+w-1) >= tft.width())  w = tft.width()  - x;
        if((y+h-1) >= tft.height()) h = tft.height() - y;

        // Set TFT address window to clipped image bounds
        tft.setAddrWindow(x, y, x+w-1, y+h-1);

        for (row=0; row<h; row++) { // For each scanline...
          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if(bmpFile.position() != pos) { // Need seek?
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          for (col=0; col<w; col++) { // For each column...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              // Push LCD buffer to the display first
              if(lcdidx > 0) {
                tft.pushColors(lcdbuffer, lcdidx, first);
                lcdidx = 0;
                first  = false;
              }
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }

            // Convert pixel from BMP to TFT format
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            lcdbuffer[lcdidx++] = tft.color565(r,g,b);
          } // end pixel
        } // end scanline
        // Write any remaining data to LCD
        if(lcdidx > 0) {
          tft.pushColors(lcdbuffer, lcdidx, first);
        } 
        progmemPrint(PSTR("Loaded in "));
        Serial.print(millis() - startTime);
        Serial.println(" ms");
      } // end goodBmp
    }
  }

  bmpFile.close();
  if(!goodBmp) progmemPrintln(PSTR("BMP format not recognized."));
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(File f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}

// Copy string from flash to serial port
// Source string MUST be inside a PSTR() declaration!
void progmemPrint(const char *str) {
  char c;
  while(c = pgm_read_byte(str++)) Serial.print(c);
}

// Same as above, with trailing newline
void progmemPrintln(const char *str) {
  progmemPrint(str);
  Serial.println();
}
