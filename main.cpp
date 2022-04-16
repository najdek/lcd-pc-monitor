#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
/*
byte fanA[] = {
  B00000,
  B11000,
  B00101,
  B01101,
  B10110,
  B10100,
  B00011,
  B00000
};
byte fanB[] = {
  B00010,
  B01001,
  B10101,
  B00110,
  B01100,
  B10101,
  B10010,
  B01000
};
*/
byte fanA[] = {
    B00000,
    B00000,
    B10110,
    B11100,
    B01110,
    B11010,
    B00000,
    B00000};
byte fanB[] = {
    B00000,
    B00000,
    B01000,
    B01110,
    B11100,
    B00100,
    B00000,
    B00000};
byte temp[] = {
    B00100,
    B01010,
    B01010,
    B01110,
    B01110,
    B11111,
    B11111,
    B01110};
byte tempC[] = {
    B11000,
    B11000,
    B00000,
    B00111,
    B00100,
    B00100,
    B00111,
    B00000};

byte netUP[] = {
    B00100,
    B01110,
    B11111,
    B01110,
    B01110,
    B01110,
    B01110,
    B00000};
byte netDOWN[] = {
    B01110,
    B01110,
    B01110,
    B01110,
    B11111,
    B01110,
    B00100,
    B00000};
/*
byte gpuMEM[] = {
    B11011,
    B11011,
    B00000,
    B11011,
    B11011,
    B00000,
    B11011,
    B11011};
*/
byte gpuMEM[] = {
    B00000,
    B11011,
    B11011,
    B00000,
    B11011,
    B11011,
    B00000,
    B00000};

byte gpuCORE[] = {
    B00000,
    B01010,
    B11111,
    B01110,
    B11111,
    B01010,
    B00000,
    B00000};

byte ohno1[] = {
    B00111,
    B01000,
    B10000,
    B10101,
    B10010,
    B10101,
    B10000,
    B10000};

byte ohno2[] = {
    B11111,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000};

byte ohno3[] = {
    B11100,
    B00010,
    B00001,
    B10101,
    B01001,
    B10101,
    B00001,
    B00001};

byte ohno4[] = {
    B10000,
    B10000,
    B10001,
    B10010,
    B10010,
    B01000,
    B00111,
    B00000};

byte ohno5[] = {
    B00000,
    B11111,
    B00000,
    B00000,
    B00000,
    B00000,
    B11111,
    B00000};

byte ohno6[] = {
    B00001,
    B00001,
    B10001,
    B01001,
    B01001,
    B00010,
    B11100,
    B00000};

void setup()
{
  Serial.begin(115200);
  Wire.begin();     //Use predefined PINS consts
  lcd.begin(20, 4); // The begin call takes the width and height. This
  // Should match the number provided to the constructor.
  lcd.createChar(0, fanA);
  lcd.createChar(1, fanB);
  lcd.createChar(2, temp);
  lcd.createChar(3, tempC);
  lcd.createChar(4, netUP);
  lcd.createChar(5, netDOWN);
  lcd.createChar(6, gpuCORE);
  lcd.createChar(7, gpuMEM);

  lcd.backlight(); // Turn on the backlight.

  lcd.home();
}

String x;
int serialUnavailableTimer = 0;
int serialTimeout = 5;

void loop()
{

  while (!Serial.available())
  {
    if (serialUnavailableTimer <= serialTimeout)
    {
      serialUnavailableTimer++;
      delay(1000);
    }
    if (serialUnavailableTimer == serialTimeout)
    {
      lcd.clear();
      lcd.noBacklight();
      lcd.createChar(0, ohno1);
      lcd.createChar(1, ohno2);
      lcd.createChar(2, ohno3);
      lcd.createChar(3, ohno4);
      lcd.createChar(4, ohno5);
      lcd.createChar(5, ohno6);
      lcd.setCursor(2, 1);
      lcd.write(0);
      lcd.write(1);
      lcd.write(2);
      lcd.setCursor(2, 2);
      lcd.write(3);
      lcd.write(4);
      lcd.write(5);
      lcd.setCursor(6, 1);
      lcd.print("BRAK");
      lcd.setCursor(6, 2);
      lcd.print("POLACZENIA...");
    }
  };
  if (serialUnavailableTimer >= serialTimeout)
  {
    lcd.clear();
    lcd.backlight();
    lcd.createChar(0, fanA);
    lcd.createChar(1, fanB);
    lcd.createChar(2, temp);
    lcd.createChar(3, tempC);

    lcd.createChar(4, netUP);
    lcd.createChar(5, netDOWN);
    lcd.createChar(6, gpuCORE);
    lcd.createChar(7, gpuMEM);
  }
  serialUnavailableTimer = 0;
  x = Serial.readStringUntil(';');
  String command = x.substring(0, 1);
  if (command == "w")
  {
    int cpu_usage = x.substring(1, 4).toInt();
    int cpu_fan_icon = x.substring(4, 5).toInt();
    int cpu_fan = x.substring(5, 8).toInt();
    int cpu_tmp = x.substring(8, 11).toInt();
    int gpu_usage = x.substring(11, 14).toInt();
    int gpu_fan_icon = x.substring(14, 15).toInt();
    int gpu_fan = x.substring(15, 18).toInt();
    int gpu_tmp_type = x.substring(18, 19).toInt();
    int gpu_tmp = x.substring(19, 22).toInt();
    int ram_usage = x.substring(22, 25).toInt();
    long net_up = x.substring(25, 31).toInt();
    long net_dn = x.substring(31, 37).toInt();
    long hashrate = x.substring(37, 40).toInt();

    //lcd.clear();

    lcd.setCursor(1, 0);
    lcd.print("CPU");
    lcd.setCursor(1, 1);
    //lcd.write(0x7e);
    lcd.print(cpu_usage);
    lcd.print("%");
    for (int i = String(cpu_usage).length(); i < 3; i++)
      lcd.print(" ");
    lcd.setCursor(0, 2);
    if (cpu_fan_icon == 0)
    {
      lcd.write(0);
    }
    else
    {
      lcd.write(1);
    }
    lcd.print(cpu_fan);
    if (cpu_fan < 100)
    {
      lcd.print("%");
    }

    for (int i = String(cpu_fan).length(); i < 2; i++)
      lcd.print(" ");
    lcd.setCursor(0, 3);
    lcd.write(2);
    lcd.print(cpu_tmp);
    if (cpu_tmp < 100)
    {
      lcd.write(3);
    }
    for (int i = String(cpu_tmp).length(); i < 2; i++)
      lcd.print(" ");

    lcd.setCursor(7, 0);
    lcd.print("GPU");
    lcd.setCursor(7, 1);
    //lcd.write(0x7e);
    if (hashrate > 0) {
      lcd.print((float)hashrate / float(10), 1);
      lcd.print("MH/s");
    } else {
      lcd.print(gpu_usage);
      lcd.print("%");
    }

    for (int i = String(gpu_usage).length(); i < 7; i++)
      lcd.print(" ");
    lcd.setCursor(6, 2);
    if (gpu_fan_icon == 0)
    {
      lcd.write(0);
    }
    else
    {
      lcd.write(1);
    }
    lcd.print(gpu_fan);
    lcd.print("%");
    for (int i = String(gpu_fan).length(); i < 3; i++)
      lcd.print(" ");

    // GPU TEMP
    lcd.setCursor(6, 3);
    if (gpu_tmp_type == 0)
    {
      lcd.write(6);
    }
    else
    {
      lcd.write(7);
    }
    //lcd.write(2);
    lcd.print(gpu_tmp);
    lcd.write(3);
    for (int i = String(gpu_tmp).length(); i < 3; i++)
      lcd.print(" ");

if (String(ram_usage).length() <= 2) {
    lcd.setCursor(13, 0);
    lcd.print("RAM:");
    lcd.print(ram_usage);
    lcd.print("%");
    for (int i = String(ram_usage).length(); i < 2; i++)
      lcd.print(" ");
} else {
    lcd.setCursor(12, 0);
    lcd.print("RAM:");
    lcd.print(ram_usage);
    lcd.print("%");
    for (int i = String(ram_usage).length(); i < 3; i++)
      lcd.print(" ");
}


    lcd.setCursor(13, 2);
    lcd.write(4);

    if (net_up < 10240)
    {
      lcd.print((float)net_up / float(1024), 2);
      lcd.print("MB");
    }
    else if (net_up < 102400)
    {
      lcd.print((float)net_up / float(1024), 1);
      lcd.print("MB ");
    }
    else
    {
      lcd.print((float)net_up / float(1024), 1);
      lcd.print("M");
    }

    lcd.setCursor(13, 3);
    lcd.write(5);
    if (net_dn < 10240)
    {
      lcd.print((float)net_dn / float(1024), 2);
      lcd.print("MB");
    }
    else if (net_dn < 102400)
    {
      lcd.print((float)net_dn / float(1024), 1);
      lcd.print("MB ");
    }
    else
    {
      lcd.print((float)net_dn / float(1024), 1);
      lcd.print("M");
    }

  }
}
