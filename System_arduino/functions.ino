
//Time conversion
static uint8_t conv2d(const char* p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9') v = *p - '0';
  return 10 * v + *++p - '0';
}

int index(const String key) {
  for (int i = 0; i < 12; i++ ) {
    if (monthss[i].equals(key))  return i;
  }
  return -1;
}

String month2int(String m) {
  m.toLowerCase();
  int in = index(m) + 1;
  String result = String(in);
  if (in < 10) result = "0" + result;
  return result;
}

//*********

//TFT Touch
uint8_t readFT5206TouchRegister( uint8_t reg )
{
  Wire.beginTransmission(addr);
  Wire.write( reg );  // register 0
  uint8_t retVal = Wire.endTransmission();

  uint8_t returned = Wire.requestFrom(addr, uint8_t(1) );    // request 6 uint8_ts from slave device #2

  if (Wire.available())
  {
    retVal = Wire.read();
  }

  return retVal;
}

uint8_t readFT5206TouchAddr( uint8_t regAddr, uint8_t * pBuf, uint8_t len )
{
  Wire.beginTransmission(addr);
  Wire.write( regAddr );  // register 0
  uint8_t retVal = Wire.endTransmission();

  uint8_t returned = Wire.requestFrom(addr, len);    // request 1 bytes from slave device #2

  uint8_t i;
  for (i = 0; (i < len) && Wire.available(); i++)
  {
    pBuf[i] = Wire.read();
  }

  return i;
}

uint8_t readFT5206TouchLocation( TouchLocation * pLoc, uint8_t num )
{
  uint8_t retVal = 0;
  uint8_t i;
  uint8_t k;

  do
  {
    if (!pLoc) break; // must have a buffer
    if (!num)  break; // must be able to take at least one

    uint8_t status = readFT5206TouchRegister(2);

    static uint8_t tbuf[40];

    if ((status & 0x0f) == 0) break; // no points detected

    uint8_t hitPoints = status & 0x0f;

    //    Serial.print("number of hit points = ");
    //    Serial.println( hitPoints );

    readFT5206TouchAddr( 0x03, tbuf, hitPoints * 6);

    for (k = 0, i = 0; (i < hitPoints * 6) && (k < num); k++, i += 6)
    {
      pLoc[k].x = (tbuf[i + 0] & 0x0f) << 8 | tbuf[i + 1];
      pLoc[k].y = (tbuf[i + 2] & 0x0f) << 8 | tbuf[i + 3];
    }

    retVal = k;

  } while (0);

  return retVal;
}

void writeFT5206TouchRegister( uint8_t reg, uint8_t val)
{
  Wire.beginTransmission(addr);
  Wire.write( reg );  // register 0
  Wire.write( val );  // value

  uint8_t retVal = Wire.endTransmission();
}

void readOriginValues(void)
{
  writeFT5206TouchRegister(0, eTEST);
  delay(1);
  //uint8_t originLength = readFT5206TouchAddr(0x08, buf, 8 );
  uint8_t originXH = readFT5206TouchRegister(0x08);
  uint8_t originXL = readFT5206TouchRegister(0x09);
  uint8_t originYH = readFT5206TouchRegister(0x0a);
  uint8_t originYL = readFT5206TouchRegister(0x0b);

  uint8_t widthXH  = readFT5206TouchRegister(0x0c);
  uint8_t widthXL  = readFT5206TouchRegister(0x0d);
  uint8_t widthYH  = readFT5206TouchRegister(0x0e);
  uint8_t widthYL  = readFT5206TouchRegister(0x0f);

  //if (originLength)
  {
    Serial.print("Origin X,Y = ");
    Serial.print( uint16_t((originXH << 8) | originXL) );
    Serial.print(", ");
    Serial.println( uint16_t((originYH << 8) | originYL) );

    Serial.print("Width X,Y = ");
    Serial.print( uint16_t((widthXH << 8) | widthXL) );
    Serial.print(", ");
    Serial.println( uint16_t((widthYH << 8) | widthYL) );
  }

}

uint32_t dist(const TouchLocation & loc)
{
  uint32_t retVal = 0;

  uint32_t x = loc.x;
  uint32_t y = loc.y;

  retVal = x * x + y * y;

  return retVal;
}

uint32_t dist(const TouchLocation & loc1, const TouchLocation & loc2)
{
  uint32_t retVal = 0;

  uint32_t x = loc1.x - loc2.x;
  uint32_t y = loc1.y - loc2.y;

  retVal = sqrt(x * x + y * y);

  return retVal;
}

bool sameLoc( const TouchLocation & loc, const TouchLocation & loc2 )
{
  return dist(loc, loc2) < 50;
}

