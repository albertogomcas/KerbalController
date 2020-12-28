void clearLCD ()
{
//  //clear the LCD by writing all spaces
//  jumpToStart();
//  mySerial.write("                ");
//  mySerial.write("                ");
//  jumpToStart();
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(SSD1306_WHITE); 
display.setCursor(0, 0);  
}

void jumpToStart ()
{
  //jump to the start of the first line on the LCD
  display.setCursor(0, 0);     // Start at top-left corner
  display.display();
}

void jumpToLineTwo ()
{
  //jump to the start of the second line on the LCD
  display.setCursor(0, 16);     // Start at top-left corner
}

void writeLCD (char myText[])
{
  display.print(myText);
  display.display();

}
