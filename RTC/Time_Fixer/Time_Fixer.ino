#include <Wire.h>

#define ADDR 0x68

const int year_delta = 0;
const int month_delta = 0;
const int day_delta = 0;
const int h_delta = 0;
const int m_delta = 0;
const int s_delta = 0;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  pinMode(3, INPUT_PULLUP);

  uint8_t time_data[7];

  while(!digitalRead(3));
  Serial.println("press button to change time");
  while(digitalRead(3));

  for(int x=0; x<7; x++) {
    Wire.beginTransmission(ADDR);
    Wire.write(x);
    Wire.endTransmission();
    Wire.requestFrom(ADDR, 1);
    time_data[x] = Wire.read();
  }

  int s = (time_data[0]&0xF) + 10*(time_data[0]>>4);
  int m_carry = 0;
  s += s_delta;
  if(s >= 60) {
    s -= 60;
    m_carry = 1;
  }
  if(s < 0) {
    s += 60;
    m_carry = -1;
  }
  time_data[0] = ((s/10)<<4) | (s%10);

  int m = (time_data[1]&0xF) + 10*(time_data[1]>>4);
  int h_carry = 0;
  m += m_delta + m_carry;
  if(m >= 60) {
    m -= 60;
    h_carry = 1;
  }
  if(m < 0) {
    m += 60;
    h_carry = -1;
  }
  time_data[1] = ((m/10)<<4) | (m%10);

  int h;
  int day_carry = 0;
  if(time_data[2] & (1<<6)) {
    // 12 hour mode
    Serial.println("12 hour mode");
    h = (time_data[2]&0xF) + ((time_data[2]>>4)&1)*10;
    h += h_delta + h_carry;
    while(h > 12) {
      h -= 12;
      time_data[2] ^= (1<<5);
      day_carry++;
    }
    while(h < 1) {
      h += 12;
      time_data[2] ^= (1<<5);
      day_carry--;
    }
    day_carry /= 2;
    time_data[2] = ((time_data[2]&0b01100000)|((h/10)<<4)) | (h%10);
  } else {
    // 24 hour mode
    Serial.println("24 hour mode");
    h = (time_data[2]&0xF) + ((time_data[2]>>4)&0b11)*10;
    h += h_delta + h_carry;
    while(h > 23) {
      h -= 23;
      day_carry++;
    }
    while(h < 0) {
      h += 23;
      day_carry++;
    }
    time_data[2] = ((h/10)<<4) | (h%10);
  }

  time_data[3] = ((time_data[3] - 1 + day_delta) % 6) + 1;
  int day = (time_data[4]&0xF) + 10*(time_data[4]>>4);
  day += day_delta + day_carry;
  while(day > 31) {
    Serial.println("I don't feel like writing day carry; try again");
    while(1);
  }
  time_data[4] = ((day/10)<<4) | (day%10);

  int month = (time_data[5]&0xF) + 10*(time_data[5]>>4);
  month += month_delta;
  if(month > 12) {
    Serial.println("I don't feel like writing month carry; try again");
    while(1);
  }
  time_data[5] = ((month/10)<<4) | (month%10);

  int year = (time_data[6]&0xF) + 10*(time_data[6]>>4);
  year += year_delta;
  if(year > 99) {
    Serial.println("We're not Y2K compliant, OK?");
    while(1);
  }
  time_data[6] = ((year/10)<<4) | (year%10);

  for(int x=0; x<7; x++) {
    Wire.beginTransmission(ADDR);
    Wire.write(x);
    Wire.write(time_data[x]);
    Wire.endTransmission();
  }

  Serial.println("successfully updated time");
}

void loop() {
  
}
