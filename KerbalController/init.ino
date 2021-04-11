//void controlsInit() {
//  pinMode(pTHROTTLE, INPUT);
//  pinMode(pTX, INPUT);
//  pinMode(pTY, INPUT);
//  pinMode(pTZ, INPUT);
//  pinMode(pRX, INPUT);
//  pinMode(pRY, INPUT);
//  pinMode(pRZ, INPUT);
//  pinMode(pTB, INPUT_PULLUP);
//  pinMode(pRB, INPUT_PULLUP);
//
//  pinMode(pLCDUP, INPUT_PULLUP);
//  pinMode(pLCDDOWN, INPUT_PULLUP);
//  pinMode(pSYNCBT, INPUT_PULLUP);
//  pinMode(pSYNCLED, OUTPUT);
//   
//  pinMode(pSAS, INPUT_PULLUP);
//  pinMode(pRCS, INPUT_PULLUP);
//  pinMode(pARMABORT, INPUT);
//  pinMode(pABORT, INPUT_PULLUP);
//  pinMode(pABORTLED, OUTPUT);
//  pinMode(pARM, INPUT);
//  pinMode(pSTAGE, INPUT_PULLUP);
//  pinMode(pSTAGELED, OUTPUT);
//  pinMode(pLIGHTS, INPUT);
//  pinMode(pLADDER, INPUT);
//  pinMode(pSOLAR, INPUT);
//  pinMode(pCHUTES, INPUT);
//  pinMode(pGEARS, INPUT);
//  pinMode(pBRAKES, INPUT);
//  pinMode(pACTION1, INPUT);
//  pinMode(pACTION2, INPUT);
//  pinMode(pACTION3, INPUT);
//  pinMode(latchPin, OUTPUT);
//  pinMode(clockPin, OUTPUT);
//  pinMode(dataPin, OUTPUT);
//}
//
//void testLEDS(int testdelay){
//  digitalWrite(pSTAGELED,HIGH);
//  delay(testdelay);
//  digitalWrite(pSTAGELED,LOW);
//  digitalWrite(pABORTLED,HIGH);
//  delay(testdelay);
//  digitalWrite(pABORTLED,LOW);
//
//  digitalWrite(pSYNCLED,HIGH);
//  delay(testdelay);
//  digitalWrite(pSYNCLED,LOW);
// 
//  //prepare the shift register
//  digitalWrite(dataPin, LOW);
//  digitalWrite(clockPin, LOW);
//  digitalWrite(latchPin, LOW);
//
//  inputBytes[0] = B11111111;
//  inputBytes[1] = B11111111;
//  inputBytes[2] = B11111111;
//  inputBytes[3] = B11111111;
//  inputBytes[4] = B11111111;
//  inputBytes[5] = B11111111;
//  inputBytes[6] = B11111111;
//  //loop through the input bytes
//  for (int j=0; j<=6; j++){
//    byte inputByte = inputBytes[j];
//    Serial.println(inputByte);
//    shiftOut(dataPin, clockPin, MSBFIRST, inputByte);
//  }
//  
//  //latch the values in when done shifting
//  digitalWrite(latchPin, HIGH); 
//  
//  delay(testdelay);
//  
//  //prepare the shift register
//  digitalWrite(dataPin, LOW);
//  digitalWrite(clockPin, LOW);
//  digitalWrite(latchPin, LOW);
//
//  inputBytes[0] = B00000000;
//  inputBytes[1] = B00000000;
//  inputBytes[2] = B00000000;
//  inputBytes[3] = B00000000;
//  inputBytes[4] = B00000000;
//  inputBytes[5] = B00000000;
//  inputBytes[6] = B00000000;
//  //loop through the input bytes
//  for (int j=0; j<=6; j++){
//    byte inputByte = inputBytes[j];
//    Serial.println(inputByte);
//    shiftOut(dataPin, clockPin, MSBFIRST, inputByte);
//  }
//  
//  //latch the values in when done shifting
//  digitalWrite(latchPin, HIGH); 
//}
