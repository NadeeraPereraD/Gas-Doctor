#include "HX711.h"
#include<LiquidCrystal.h>
#include <Wire.h>
#include "RTClib.h"
#define DS3231_I2C_ADDRESS 0x68
#include <EEPROM.h>
//#include <SPI.h>
//#include <SD.h>
//File myFile;

int oldTime=0;
int newTime=oldTime+2;

RTC_DS1307 rtc;
LiquidCrystal lcd(2,3,6,7,8,9); // RS,EN,D4,D5,D6,D7

const byte menuButton = A0;
const byte escButton = A1;
const byte upButton = A2;
const byte downButton = A3;

byte test = 0;
byte menu=1;

#define LOADCELL_DOUT_PIN  4
#define LOADCELL_SCK_PIN  5

//#define RTCAddress 0x68

HX711 scale;

float calibration_factor = 373.55;
byte a = 0; /// Starting Position
byte x = 0; /// Keypad Position
byte up = 0;
byte down = 0;


boolean tare; //// if high tare is yes; if low tare is no


byte ct=EEPROM.read(0);
byte tValue = EEPROM.read(1);
byte tYear= EEPROM.read(2);
byte tMonth= EEPROM.read(3);
byte tDate= EEPROM.read(4);
byte tHour= EEPROM.read(5);
byte tMinute= EEPROM.read(6);
byte tSecond= EEPROM.read(7);




//byte ct = 1; /// Cylinder type
float ctValue = 0.0; //// Cylinder type value


byte second,  minute,  hour,day, date,month, dayOfWeek ;
byte year = 21;

byte counter;

/*int day;
int month;
int year;

int second;
int minute;
int hour;*/


void setup() {
  Serial.begin(9600);
  Wire.begin();
  readDS3231time(&second,
&minute,
&hour,
&dayOfWeek,
&date,
&month,
&year);

///////////////////SD Card///////////////////////////////
/*Serial.print("Initializing SD card...");
if (!SD.begin(10)) {
Serial.println("initialization failed!");
//while (1);
}
Serial.println("initialization done.");
// open the file. note that only one file can be open at a time,
// so you have to close this one before opening another.
//////////////////////myFile = SD.open("test.txt", FILE_WRITE);
// if the file opened okay, write to it:
/*if (myFile) {
Serial.print("Writing Data");
myFile.println("This is a test file :)");*/
/////////////////////**************************************************************
/*
myFile.print(date);
myFile.print("/");
myFile.print(month);
myFile.print("/");
myFile.print(year);
myFile.print(",");
myFile.print(second);
myFile.print("/");
myFile.print(minute);
myFile.print("/");
myFile.print(hour);
myFile.print(",");
myFile.print(value);
*/
//////////////////////*************************************************************
// close the file:
/*myFile.close();
Serial.println("done.");
} else {
// if the file didn't open, print an error:
Serial.println("error opening test.txt");
}*/
  
/////////// LoadcellStart////////////////////////////  
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale();
  scale.tare(); //Reset the scale to 0      ***************************
  //float inVal = scale.get_units();
  //inVal = 0.00;
  long zero_factor = scale.read_average(); 

/////////// LoadcellFinish////////////////////////////   

//////////////LCD_Start////////////////////////
  lcd.begin(20,4); // col,row
  lcd.clear();

//////////////LCD_Finish//////////////////////

////////////Buttons Start/////////////////////

  pinMode(menuButton,INPUT_PULLUP);
  pinMode(escButton,INPUT_PULLUP);
  pinMode(upButton,INPUT_PULLUP);
  pinMode(downButton,INPUT_PULLUP);
  
  

/////////////Buttons Finish//////////////

/////////////RTC Start//////////////////
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
  //setDS3231time(50,23,21,1,10,01,21);
//////////////////////////// Cylinder Type Save //////////////////////////////
    if(ct==1){
      ctValue = 37.5;
      //a=1;
    }
    if(ct==2){
      ctValue = 12.5;
      //a=1;
    }
    if(ct==3){
      ctValue = 05.0;
      //a=1;
    }
    if(ct==4){
      ctValue = 02.3;
      //a=1;
    }
  
  }

////////////RTC Finish/////////////////


void display(String a,String b,String c,String d){
  //lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(a);
  lcd.setCursor(0,1);
  lcd.print(b);
  lcd.setCursor(0,2);
  lcd.print(c);
  lcd.setCursor(0,3);
  lcd.print(d);
  if(test==0){
    for(int i=12;i<20;i++){
      lcd.setCursor(i,3);
      lcd.print(".");
      delay(500);
      test++;
    }
    lcd.clear();
  }
}



/*void valueDisplay(int i,int j, String a){
  lcd.setCursor(i,j);
  lcd.print(a);
}*/


void keypad(){
  if (digitalRead(menuButton)==LOW && a==1){
    //Serial.println("Menu");
    lcd.clear();
    a=2;
    delay(250);
  }

  if (digitalRead(menuButton)==LOW && x==1 && a!=7 && a!=13 && a!=14 && a!=8 && a!=15 && a!=16){
    //Serial.println("Menu");
    lcd.clear();
    a=6;
    delay(250);
  }

  if (digitalRead(menuButton)==LOW && x==2){
    //Serial.println("Menu");
    lcd.clear();
    a=11;
    delay(250);
  }

  if (digitalRead(menuButton)==LOW && x==3 && a!=7 && a!=13 && a!=14 && a!=8 && a!=15 && a!=16){
    //Serial.println("Menu");
    lcd.clear();
    a=12;
    delay(250);
  }  

  if (digitalRead(escButton)==LOW){
    //Serial.println("Esc");
    lcd.clear();
    a=3;
    delay(250);
  }

  if (digitalRead(upButton)==LOW && a!=7 && a!=13 && a!=14 && a!=8 && a!=15 && a!=16){
    //Serial.println("Up");
    lcd.clear();
    a=4;
    if(x!=2){
    menu--;
    if(menu<1){
      menu=5;
    }
    }
    if(x==3){
      ct--;
      if(ct<1){
        ct=4;
      }
    }
    delay(250);
  }

  if (digitalRead(downButton)==LOW && a!=7 && a!=13 && a!=14 && a!=8 && a!=15 && a!=16){
    //Serial.println("Down");
    lcd.clear();
    a=5;
    if(x!=2){
    menu++;
    if(menu>5){
      menu=1;
    }
    }
    if(x==3){
      ct++;
      if(ct>4){
        ct=1;
      }
    }    
    delay(250);
  }
/////////////////////////// Adjust Date ////////////////////////////////////
  if(digitalRead(upButton)==LOW && a==7){
    lcd.clear();
    counter++;
    date = date + counter;
    if(date > 31){
      date=1;
    }
    delay(10);
    displaySetDate();
    counter = 0;
    delay(240);
    /*Serial.println(a);
    Serial.println(date);*/
    
  }

  if(digitalRead(downButton)==LOW && a==7){
    lcd.clear();
    counter++;
    date = date - counter;
    if(date < 1){
      date=31;
    }
    delay(10);
    displaySetDate();
    counter = 0;
    delay(240);
    /*Serial.println(a);
    Serial.println(date);*/
    
  }

  if(digitalRead(menuButton)==LOW && a==7){
    setDS3231time(second, minute, hour, dayOfWeek, date, month, year);
    delay(240);
    //Serial.println(date);
    a = 13;
    //Serial.println(a);
  }

/////////////////////////// Adjust Month ////////////////////////////////////
  if(digitalRead(upButton)==LOW && a==13){
    lcd.clear();
    counter++;
    month = month + counter;
    if(month > 12){
      month=1;
    }
    delay(10);
    displaySetDate();
    counter = 0;
    delay(240);
    /*Serial.println(a);
    Serial.println(month);*/
    
  }

  if(digitalRead(downButton)==LOW && a==13){
    lcd.clear();
    counter++;
    month = month - counter;
    if(month < 1){
      month=12;
    }
    delay(10);
    displaySetDate();
    counter = 0;
    delay(240);
    /*Serial.println(a);
    Serial.println(month);*/
    
  }

  if(digitalRead(menuButton)==LOW && a==13){
    setDS3231time(second, minute, hour, dayOfWeek, date, month, year);
    //Serial.println(date);
    delay(240);
    a = 14;
    //Serial.println(a);
  }

/////////////////////////// Adjust Year ////////////////////////////////////
  if(digitalRead(upButton)==LOW && a==14){
    lcd.clear();
    counter++;
    year = year + counter;
    /*if(year > 12){
      year=1;
    }*/
    delay(10);
    displaySetDate();
    counter = 0;
    delay(240);
    /*Serial.println(a);
    Serial.println(year);*/
    
  }

  if(digitalRead(downButton)==LOW && a==14){
    lcd.clear();
    counter++;
    year = year - counter;
    /*if(month < 1){
      month=12;
    }*/
    delay(10);
    displaySetDate();
    counter = 0;
    delay(240);
    /*Serial.println(a);
    Serial.println(year);*/
    
  }

  if(digitalRead(menuButton)==LOW && a==14){
    setDS3231time(second, minute, hour, dayOfWeek, date, month, year);
    //Serial.println(date);
    delay(240);
    a = 1;
    menu = 1;
    //menuButton=LOW;
    //Serial.println(a);
  }

/////////////////////////// Adjust Second ////////////////////////////////////
  if(digitalRead(upButton)==LOW && a==8){
    lcd.clear();
    counter++;
    second = second + counter;
    if(second > 59){
      second=0;
    }
    delay(10);
    displaySetTime();
    counter = 0;
    delay(240);
    
  }

  if(digitalRead(downButton)==LOW && a==8){
    lcd.clear();
    counter++;
    second = second - counter;
    if(second < 0){
      second=59;
    }
    delay(10);
    displaySetTime();
    counter = 0;
    delay(240);
    /*Serial.println(a);
    Serial.println(date);*/
    
  }

  if(digitalRead(menuButton)==LOW && a==8){
    setDS3231time(second, minute, hour, dayOfWeek, date, month, year);
    delay(240);
    //Serial.println(date);
    a = 15;
    //Serial.println(a);
  }

/////////////////////////// Adjust Minute ////////////////////////////////////
  if(digitalRead(upButton)==LOW && a==15){
    lcd.clear();
    counter++;
    minute = minute + counter;
    if(minute > 59){
      minute=0;
    }
    delay(10);
    displaySetTime();
    counter = 0;
    delay(240);
    /*Serial.println(a);
    Serial.println(month);*/
    
  }

  if(digitalRead(downButton)==LOW && a==15){
    lcd.clear();
    counter++;
    minute = minute - counter;
    if(minute < 0){
      month=59;
    }
    delay(10);
    displaySetTime();
    counter = 0;
    delay(240);
    /*Serial.println(a);
    Serial.println(month);*/
    
  }

  if(digitalRead(menuButton)==LOW && a==15){
    setDS3231time(second, minute, hour, dayOfWeek, date, month, year);
    //Serial.println(date);
    delay(240);
    a = 16;
    //Serial.println(a);
  }

/////////////////////////// Adjust Hour ////////////////////////////////////
  if(digitalRead(upButton)==LOW && a==16){
    lcd.clear();
    counter++;
    hour = hour + counter;
    if(hour > 23){
      hour=0;
    }
    delay(10);
    displaySetTime();
    counter = 0;
    delay(240);
    /*Serial.println(a);
    Serial.println(year);*/
    
  }

  if(digitalRead(downButton)==LOW && a==16){
    lcd.clear();
    counter++;
    hour = hour - counter;
    if(hour < 0){
      hour=23;
    }
    delay(10);
    displaySetTime();
    counter = 0;
    delay(240);
    /*Serial.println(a);
    Serial.println(year);*/
    
 }

  if(digitalRead(menuButton)==LOW && a==16){
    setDS3231time(second, minute, hour, dayOfWeek, date, month, year);
    //Serial.println(date);
    delay(240);
    a = 1;
    menu = 1;
    //menuButton=LOW;
    //Serial.println(a);
  }

  /*if(a==7 && digitalRead(upButton)==LOW){
    date++;
  }*/
}

void updateMenu(){
    if (menu==1){
      //lcd.clear();
      display("Menu","1.>Set Date","2. Set Time","3. Tare");
    }
    if (menu==2){
      //lcd.clear();
      display("Menu","1. Set Date","2.>Set Time","3. Tare");
    }
    if (menu==3){
      //lcd.clear();
      display("Menu","1. Set Date","2. Set Time","3.>Tare");
    }             
    if (menu==4){
      //lcd.clear();
      display("Menu","2. Set Time","3. Tare","4.>Cylinder Type");
    } 
    if (menu==5){
      //lcd.clear();
      display("Menu","3. Tare","4. Cylinder Type","5.>Esc");
    }
}

void updateCT(){
    if (ct==1){
      //lcd.clear();
      display("Cylinder Type",">37.5 kg"," 12.5kg"," 5 kg");
    }
    if (ct==2){
      //lcd.clear();
      display("Cylinder Type"," 37.5 kg",">12.5kg"," 5 kg");
    }
    if (ct==3){
      //lcd.clear();
      display("Cylinder Type"," 37.5 kg"," 12.5kg",">5 kg");
    }             
    if (ct==4){
      //lcd.clear();
      display("Cylinder Type"," 12.5kg"," 5 kg",">2.3 kg");
    }  
}

//RTC FUNCTIONS............................
byte decToBcd(byte val){
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val){
  return( (val/16*10) + (val%16) );
}
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
date, byte month, byte year){
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(date)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}
void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *date,
byte *month,
byte *year){
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *date = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());

}
/*void displayTime(){
  byte second, minute, hour, dayOfWeek, date, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &date, &month,
  &year);
  // send it to the serial monitor
  Serial.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  if (minute<10){
    Serial.print("0");
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second<10){
    Serial.print("0");
  }
  Serial.print(second);
  Serial.print(" ");
  Serial.print(date, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.println(year, DEC);
}*/


void displaySetDate(){
  char bufferDate[20];
  String sDate = String(date);
  sprintf(bufferDate,"Date : %s", sDate.c_str());

  char bufferMonth[20];
  String sMonth = String(month);
  sprintf(bufferMonth,"Month : %s", sMonth.c_str());

  char bufferYear[20];
  String sYear = String(year);
  sprintf(bufferYear,"Year : %s", sYear.c_str());
    
  display("Set Date",bufferDate,bufferMonth,bufferYear);
  Serial.println("Display Set time");
}

void displaySetTime(){
  
  char bufferSec[20];
  String sSec = String(second);
  sprintf(bufferSec,"Second : %s", sSec.c_str());

  char bufferMin[20];
  String sMin = String(minute);
  sprintf(bufferMin,"Minute : %s", sMin.c_str());

  char bufferHo[20];
  String sHo = String(hour);
  sprintf(bufferHo,"Hour : %s", sHo.c_str());
    
  display("Set Time",bufferSec,bufferMin,bufferHo);  
}

/*void SD_cardWrite(){
  
  myFile = SD.open("test.txt", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile) {
  Serial.print("Writing Data");
  myFile.println("This is a test file :)");
  /////////////////////**************************************************************
  
  myFile.print(date);
  myFile.print("/");
  myFile.print(month);
  myFile.print("/");
  myFile.print(year);
  myFile.print(",");
  myFile.print(second);
  myFile.print("/");
  myFile.print(minute);
  myFile.print("/");
  myFile.print(hour);
  myFile.print(",");
  //myFile.print(value);
  
  //////////////////////*************************************************************
  // close the file:
  myFile.close();
  Serial.println("done.");
  } else {
  // if the file didn't open, print an error:
  Serial.println("error opening test.txt");
  }

}*/


void loop() {

  keypad();

  String nullValue = "                    "; 
  
  scale.set_scale(calibration_factor);
  
  if(a==0){
    display("Gas Doctor","EA 3050","18sea018","Initializing");
    a=1;
  }
 // float value = 95.9;
  float value = scale.get_units()/1000.0;
  if (value<0){
    value=0.00;
  }
  float ftValue= tValue;
  float Remaining = value-(ftValue/50)+ctValue;
  
  
  int remPercentage = Remaining *100 / ctValue;

  if (remPercentage<0){
    remPercentage=0.0;
  }
  ///////////////////////////////////////////////Prediction//////////////////////////
  float uTime =(minute-tMinute)+(float(second-tSecond)/60);
  Serial.println(tMinute);
  Serial.println(uTime);
  
  int  Prediction=(Remaining*uTime/(float(tValue)/50-value)) ;

   if (Prediction<0){
    Prediction=0;
  }
                                                                            // << Display 
  
  
  String sValue = String(value);
  String sRemaining= String(remPercentage);
  String sctValue = String(ctValue);
  String sPrediction=String(Prediction);

  //String Testt="Api Harak"+sValue+sRemaining;
//  Serial.println(Testt);

//sprintf(BufferA,"Total weight:%0.3skg",sValue.c_str()); 
  
 /* sprintf(AAA,"Total weight:%0.4s kg",sValue.c_str());
  
  sprintf(BBB,"Remaining Gas:%0.3s",sRemaining.c_str()) ;
  
  sprintf(CCC,"Gas Cylinder:%0.4s kg",sctValue.c_str());
  
  sprintf(DDD,"Finish after: %0.4s day", sPrediction.c_str()); */

  String ABC= "Total weight:"+sValue+" kg" ;
  String ABD="Remaining Gas:"+sRemaining +"%"  ;
  String ABE="Gas Cylinder:"+sctValue+" kg" ;
  String ABF= "Finish after:"+sPrediction+" day";

  //Serial.println(ABC);
 // Serial.println(ABD);
 // Serial.println(ABE);
 // Serial.println(ABF);

  if(a==1){
    //valueDisplay(13,0,sValue);
    //display(buffer,buffer1,ctBuffer,buffer3);
   // display(BufferA,"Remaining Gas: ",ctBuffer,"Finish after FFFday");
    //display("",BBB,CCC,DDD);
    display(ABC,ABD,ABE,ABF);
    //display(BufferA,"BufferB","test03","BufferD");
  }
  
  //delay(1000);


  /*Serial.print(date); 
  Serial.print(":");
  Serial.println(month);
  /*Serial.println(x);
  Serial.print("ct :");
  Serial.println(ctBuffer);*/
  
  if(a==2){
    display("Menu","1.>Set Date","2. Set Time","3. Tare");
    x = 1;
    //Serial.println("Menu");
  }

  if(a==6){
    //display("Menu","1.>Set Date","2. Set Time","3. Tare");
    //x = 1;
    if(menu==5){
      a=1;
      x=0;
    }

    if(menu==1){
      a=7;
                                                                                //**********
      
        //setDS3231time(50,23,21,1,10,01,22);
        readDS3231time(&second, &minute, &hour, &day, &date, &month, &year);
        
        displaySetDate();
        //Serial.println(date); 
      
    }
    if(menu==2){
      a=8;
                                                                        // *************
      readDS3231time(&second, &minute, &hour, &day, &date, &month, &year); 
      displaySetTime(); 
      //Serial.println(second); Serial.println(minute);Serial.println(hour);    
    }
    if(menu==3){
      a=9;
      x=2;
      display("Tare","Now total weight:","Do you want Tare ",">Yes");
      tare = HIGH;      
    }
    if(menu==4){
      a=10;
      x=3;
      display("Cylinder Type",">37.5 kg"," 12.5kg"," 5 kg");        
    }
  }

  if(a==3){ ////////// Esc Button
    //Serial.println("Esc");
    a=1;
    x=0;
    menu = 1;
  }

  if(a==4 && x==1){ ///////// Menu Up Button
    //Serial.println("Up");
    updateMenu();    
  }

  if(a==5 && x==1){ ////Menu Down Button
    //Serial.println("Down");
    updateMenu();      
  }

  if(a==5 && menu==3 && x==2){
    display("Tare","Do you want Tare "," Yes",">No");
    tare = LOW;
  }
  if(a==4 && menu==3 && x==2){
    display("Tare","Do you want Tare ",">Yes"," No");
    tare = HIGH;
    Serial.println(tValue);
    float fTValue = tValue;
    Serial.println(fTValue/50);
  }

  if (a==11 && tare==HIGH){
    display("Yes","New tare weight is",sValue,"Please press Esc");          //<< Tare **********
    tValue = value*50;
    tYear = year; tMonth = month; tDate = date; tHour = hour; tMinute = minute; tSecond=second;
    EEPROM.write(1,tValue);
    EEPROM.write(2,tYear);
    EEPROM.write(3,tMonth);
    EEPROM.write(4,tDate);
    EEPROM.write(5,tHour);
    EEPROM.write(6,tMinute);
    EEPROM.write(7,tSecond);
    
  }

  if (a==11 && tare==LOW){
    a=2;
  }

  /*if(a==5 && menu==4 && x==3){
    display("Tare","Do you want Tare "," Yes",">No");
    tare = LOW;
  } */ 

  if(a==4 && x==3){ ///////// CT Up Button
    //Serial.println("Up");
    updateCT();    
  }

  if(a==5 && x==3){ ////CT Down Button
    //Serial.println("Down");
    updateCT();      
  }

  if(a==12){
    if(ct==1){
      ctValue = 37.5;
      a=1;
    }
    if(ct==2){
      ctValue = 12.5;
      a=1;
    }
    if(ct==3){
      ctValue = 05.0;
      a=1;
    }
    if(ct==4){
      ctValue = 02.3;
      a=1;
    }
    EEPROM.write(0,ct);
  }

//readDS3231time(&second, &minute, &hour, &day, &date, &month, &year);
 // SD_cardWrite();
/*Serial.print("a = ");
Serial.println(a);
Serial.print("x = ");
Serial.println(x);
Serial.print("menu = ");
Serial.println(menu);*/

  //displayTime(); // display the real-time clock data on the Serial Monitor,
  //delay(200); // every second 
  readDS3231time(&second,
&minute,
&hour,
&dayOfWeek,
&date,
&month,
&year);
}
