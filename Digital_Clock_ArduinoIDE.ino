#include <LiquidCrystal_I2C.h>
#include <RtcDS1302.h>
#include <Keypad.h>

const byte ROWS = 4; // Four rows
const byte COLS = 3; // Three columns

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};  // Row pinouts
byte colPins[COLS] = {5, 4, 3};     // Column pinouts

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD Object

ThreeWire myWire(11, 10, 12);        // DAT, CLK, RST
RtcDS1302<ThreeWire> Rtc(myWire);    // RTC Object

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();

  Rtc.Begin();

  if (!Rtc.GetIsRunning()) {  // Only set if RTC is not running
    Rtc.SetDateTime(RtcDateTime(2025, 2, 13, 12, 0, 0));  // Set a default date & time
  }
}

void loop() {
  RtcDateTime now = Rtc.GetDateTime();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Date: ");
  lcd.print(now.Day());
  lcd.print("/");
  lcd.print(now.Month());
  lcd.print("/");
  lcd.print(now.Year());

  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  lcd.print(now.Hour());
  lcd.print(":");
  lcd.print(now.Minute());
  lcd.print(":");
  lcd.print(now.Second());

  char c = customKeypad.getKey();
  if (c == '*') {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Year: ");
    int year = getData();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Month: ");
    int month = getData();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Day: ");
    int day = getData();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Hours: ");
    int hour = getData();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Minutes: ");
    int minute = getData();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Seconds: ");
    int second = getData();

    RtcDateTime newTime(year, month, day, hour, minute, second);
    Rtc.SetDateTime(newTime);
  }

  delay(500);
}

int getData() {
  String container = "";
  lcd.setCursor(0, 1);

  while (true) {
    char c = customKeypad.getKey();
    if (c == '#') {
      break;
    } else if (c != NO_KEY && isDigit(c)) {
      container += c;
      lcd.print(c);
    }
  }

  return container.toInt();
}