#include "LedControl.h" //  need the library
#include <LiquidCrystal.h>
#define  scrie Serial.print
LedControl lc = LedControl( 12, 11, 10, 1); //DIN, CLK, LOAD, No. DRIVER
LiquidCrystal lcd(8 ,  7 ,  A3 ,  A2 , 4 , 13 );

#define noMaterial 5
#define matNameLength 10
#define partNameLength 20
#define noEachMat 4
#define LED_YELLOW 3
#define LED_BLUE 5
#define LED_RED 6
//#define LED_GREEN 9
byte matrix[ 30 ][ 23 ];
int8_t lifes = 3 ;
float fuel = 75 ;
float fuelMax = 75 ;
int8_t cargoMax = 5 ;
int8_t cargoActual;
int8_t healthMax = 2;
int8_t healthActual = 2;
int8_t moneyTotal;
int8_t moneyActual= 2;
int8_t noSteps;
int8_t dellay;
int8_t ledYellowIntensity ;
int8_t ledBlueIntensity;
int8_t ledRedIntensity;
int8_t ledGreenIntensity;
int8_t ledSpecialIntensity;
struct point
{
  int8_t col , row:6 ;
  point( int8_t c , int8_t r )
  {
    col = c;
    row = r;
  }
};
point begin_matrix( 0  , 0 ) ;//punctul de incepere al matricii 8x8 in matricea 50x20
point curr ( 1 , 1 );
void init_matrix()
{
 
  int i , j ;
  for ( i = 0 ; i < 2; i++)
    for ( j =  0 ; j < 22 ; j++ )
    matrix[i][j] = 1;
    matrix[1][8] = 0 ;//fuel
    matrix[1][15] = 0 ;//market
    matrix[1][18] = 0 ;//shop
  for ( i = 2 ; i < 8 ; i++ )
    for ( j = 0 ; j < 8 ; j++ )
      lc.setLed( 0 , j , i , !matrix[ i ][ j ] );

}
void endGame()
{
  for ( int i = 0 ; i < 8 ; i++ )
    for ( int j = 0 ; j < 8 ; j++ )
      if ( i==j || i == 8 - j - 1 )
        lc.setLed( 0 , j , i , true );
        else lc.setLed(0 , j , i , false ); 
}
void matrix8x8()
{
  int i , j ; 
   for ( i = begin_matrix.row ; i < begin_matrix.row + 8 ; i++ )
          for ( j = begin_matrix.col ; j < begin_matrix.col + 8 ; j++ )
            lc.setLed( 0 , j - begin_matrix.col , i - begin_matrix.row , !matrix[i][j]);
        //      if (millis() % 100 < 50  )
   // lc.setLed( 0 , curr.row , curr.col , false);
   // else lc.setLed( 0 , curr.row , curr.col , true);
}
void w_matrix()
{
  int8_t i , j ;
  for ( i  = 0 ; i < 10 ; i++ )
    {
      for ( j = 0 ; j < 20 ; j++ )
        Serial.print(matrix[ i ][ j ]);
        Serial.println("");
    }
  Serial.println("");

}

class material
{
  private:
  int8_t ledColorIntensity;
  char nameMat[ matNameLength ];
  int cargo;
  //int8_t quantity:4; trebuie sa fac un vector -> nu mai am memorie
  int8_t pricePerUnit:3;
  int8_t posX[ noEachMat ];
  int8_t posY[ noEachMat ];
  
 public:
  void setter( int8_t contorPos , int8_t row , int8_t col )
  {
    posX[ contorPos ] = col ;
    posY[ contorPos ] = row ;
  }
  void setLedColorIntensity( int8_t s )
  {
    ledColorIntensity = s;
  }
  int8_t getLedColorIntensity()
  {
    return  ledColorIntensity;
  }
  int8_t getX( int8_t contor )
  {
    return posX[contor];
  }
   int8_t getY( int8_t contor )
  {
    return posY[contor];
  }
  bool isMat( int8_t col , int8_t row )
  {
    int i , j;
      for ( j = 0 ; j < noEachMat ; j++ )//nr de obiecte pt fiecare element
        if ( col == this->getY( j ) && row == this->getX(j))
          return true;
    return false;
  }
  int8_t getCargo()
  {
    return cargo;
  }
  void setCargo(int8_t c )
  {
    cargo = c;
  }
  int8_t getPrice()
  {
    return pricePerUnit;
  }
  void setPrice( int8_t p )
  {
    pricePerUnit = p ;
  }

};

material mets[ noMaterial ];//bronz argint aur resSpeciala



int setYellowLedIntensity(int8_t poz , int8_t row , int8_t col )
{
  int ok = 0 ;
  for ( int i = 0 ; i < 1 ; i++ )
  {
    if ( mets[poz].getX(i) == col && mets[poz].getY(i) == row )
      {
        ok = 1;
        mets[poz].setter(i,-1,-1); 
        break;
      }
      else
      if ( row < 30 )
      {
       if ( mets[poz].getX(i) == (col -1) && mets[poz].getY(i) == (row - 1 ) ||
            mets[poz].getX(i) == (col ) && mets[poz].getY(i) == (row - 1 ) ||
            mets[poz].getX(i) == (col + 1) && mets[poz].getY(i) == (row - 1 ) ||
            mets[poz].getX(i) == (col -1) && mets[poz].getY(i) == (row  ) ||
            mets[poz].getX(i) == (col +1) && mets[poz].getY(i) == (row  ) ||
            mets[poz].getX(i) == (col -1) && mets[poz].getY(i) == (row + 1 ) ||
            mets[poz].getX(i) == (col ) && mets[poz].getY(i) == (row + 1 ) ||
            mets[poz].getX(i) == (col +1) && mets[poz].getY(i) == (row + 1 ) )
            ok = 6;
      }
  }
  if ( ok == 1 )
   
      mets[poz].setLedColorIntensity(250) ;
    
    else 
    if ( ok == 6 )
       mets[poz].setLedColorIntensity(1);
      else mets[poz].setLedColorIntensity(0);
      
      return ok;
}
int8_t money;
void setup()
{
  pinMode( 2 , INPUT );
  
//  lcd.setIntensity(2);
   lcd.begin(16, 2);

 pinMode(9, OUTPUT); // PWN in loc de POTENTIOMETRU
  analogWrite(9, 140);
  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, 1); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
    pinMode(LED_YELLOW , OUTPUT);
    pinMode( LED_BLUE ,OUTPUT );
     pinMode( LED_RED ,OUTPUT );
    //  pinMode( LED_GREEN ,OUTPUT );
  //pinMode(2, INPUT);
  /* for (int row = 0; row < 8; row++)
    {
    for (int col = 0; col < 8; col++)
    {
    lc.setLed(0, col, row, true); // turns on LED at col, row
    delay(25);
    }
    }
    lc.setLed(0,2,2,false);
    matrix[2][2] = 1 ;
  */
  // matrix[ begin_matrix.row ][ begin_matrix.col ] = 1 ;
  matrix[ curr.col ][ curr.row ]  = 1;
  init_matrix();
  //  move_up();
  // move_r();
  Serial.begin(9600);
  mets[1].setter(0 , 3 , 2 );
  mets[1].setter(1 , 4 , 6 );
  mets[1].setter(2 , 8 , 2 );
  mets[1].setter(3, 15 , 3 );
  mets[1].setCargo(2);
  mets[1].setPrice(1);
  mets[1].setLedColorIntensity(0);
  mets[2].setter(0 , 3 , 7);
  mets[2].setter(1 , 10 , 4 );
  mets[2].setCargo(4);
  mets[2].setPrice(2);
  mets[2].setLedColorIntensity(0);

  mets[3].setter(0 , 8 , 5 );
  mets[3].setCargo(6);
  mets[3].setPrice(3);
  mets[3].setLedColorIntensity(0);

  mets[0].setter(1 , 5 , 6 );
  mets[0].setCargo(10);
  mets[0].setPrice(6);
  mets[0].setLedColorIntensity(0);
 
}

void loop()
{
  pinMode(A4,INPUT);
  //lcd.clear();
  lcd.setCursor(0 , 0);
  lcd.print("  ");
   lcd.setCursor(2,0);
    lcd.print("Fuel : ");
    lcd.setCursor(7,0);
    lcd.print(String(fuel));
    lcd.setCursor(0,1);
    lcd.print("                ");
    
   //Serial.println(String(fuel));
  int trecut = 1;
  //  lcd.print(fuel);
  if ( lifes > 0 ){
        
   // Serial.println(fuel);
   //  lcd.begin(16, 2);
 //   lcd.print("plm");//print name


//delay(750);//delay of 0.75sec

//lcd.scrollDisplayLeft();//shifting data on LCD

//lcd.setCursor(0, 0);// set the cursor to column 0, line1
   analogWrite(LED_YELLOW,mets[1].getLedColorIntensity());//led invers. am conectat placa invers
    analogWrite(LED_BLUE,mets[2].getLedColorIntensity());//led invers. am conectat placa invers
    analogWrite(LED_RED, mets[3].getLedColorIntensity());//led invers. am conectat placa invers
   
   ///analogWrite( LED_BLUE , 20 );
  // analogWrite( LED_RED , 75 );
  // analogWrite( LED_GREEN , 250 );
  // delay(600);
  //Serial.println(sizeof(matrix));

  // matrix[ begin_matrix.row ][ begin_matrix.col ] = 1 ;
 int a1 , a2 , a3;
  int x_axe = analogRead(A1);
  int y_axe = analogRead(A0);
  bool pressed = false;
 
  if ( x_axe > 800 )
  {
    if(begin_matrix.col < 14 && curr.row > 3 )
    begin_matrix.col++;
    if( curr.row < 21)
    curr.row++ ;
    if ( matrix[ curr.col ][ curr.row ] == 0 )
    {
      fuel-=3.0;
    }
    else fuel-=2.0;
    //Serial.println("Cresc row");
  }
  else
  if(x_axe < 200 )
  {
    if ( begin_matrix.col > 0 )
    {
      ///Serial.println("Scad row");
      
      if(curr.row < 19)
      begin_matrix.col-- ;
      curr.row--;
      
    }
    else if (curr.row > 0 )
    {
      //Serial.print("prima coloana");
      curr.row--;
    }
    if ( matrix[ curr.col ][ curr.row ] == 0 )
    {
      fuel-=3.0;
    }
    else fuel-=2;
  }
  else if (y_axe > 800)
  {
    if(begin_matrix.row < 22 && curr.col > 3)
    
      begin_matrix.row++ ;
      if ( curr.col < 28 )
      curr.col++ ;
    //Serial.print(curr.col);
    //Serial.print(" ");
    //Serial.println(begin_matrix.row);
    
    //Serial.println("cresc col");
    if ( matrix[ curr.col ][ curr.row ] == 0 )
    {
      fuel-=4.0;
    }
    
  }
  else if ( y_axe < 200   && curr.col > 0 &&  matrix[ curr.col - 1 ][ curr.row ] == 1 )
  {
    if ( begin_matrix.row > 0 )
    {
      if ( curr.col < 26 )
      begin_matrix.row-- ;
      curr.col-- ;
    }
    else 
    {
      if (curr.col > 0 )
      curr.col--;
    }
    if ( matrix[ curr.col ][ curr.row ] == 1 )
    {
      fuel-=2.0;
    }
    
   // Serial.println("Scad col");
  }
  else pressed = true;
  if(pressed)
  fuel-=0.05;
  if ( !pressed ){
    if ( curr.col > 1 ){
 
 // Serial.println(curr.col);
 // Serial.println(curr.row);
 // Serial.println(
 // mets[1].isMat(curr.col , curr.row)) ;
  //Serial.println("");
  for ( int i = 0 ; i < noMaterial ; i++){
   // Serial.print("INTRAAAAAAAAAAAAAAAT");
   //if(mets[i].isMat(curr.col,curr.row))
   
  if ( mets[i].isMat(curr.col , curr.row) && matrix[curr.col][curr.col]==0) //DACA ESTE MATERIAL NEVIZITAT
  {
    //Serial.print("ghita prostu");
   // Serial.print(i);
   // Serial.print("cargo : ");
   // Serial.println(mets[i].getCargo());
 //   digitalWrite(4,LOW);
 //Serial.print("INTRAAAAT"),Serial.println(mets[i].getCargo());
    if(mets[i].getCargo() + cargoActual <= cargoMax )
    {
      
      cargoActual+=mets[i].getCargo();
      money+=mets[i].getPrice();
    }
   // else Serial.println("nu mai este spatiu");
   /// Serial.print("cargo : ");
    // Serial.println(cargoActual);
    
  }
  }

  
 
   matrix[ curr.col ][ curr.row ]  = 1; 
   
   a1=setYellowLedIntensity( 1 , curr.col , curr.row);
   a2=setYellowLedIntensity( 2 , curr.col , curr.row);
//Serial.println("");
   a3=setYellowLedIntensity( 3 , curr.col , curr.row);
 //  scrie(mets[3].getLedColorIntensity());
  // Serial.println("");
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  //  Serial.print("GAAASIT");
  //  mets[1].setter( 1 , -1 , -1 );
    }
  matrix8x8();
 
  }
  lc.setLed( 0 , curr.row - begin_matrix.col , curr.col - begin_matrix.row, true );
 delay(100);
lc.setLed( 0 , curr.row - begin_matrix.col , curr.col - begin_matrix.row, false );
    if(pressed != 1 )
      noSteps = 0;
      int trecut = 0;
    if ( curr.col < 47  && curr.col > 1 && pressed == 1)
    {
      trecut = 1;
      if ( matrix[curr.col + 1][curr.row] == 1 )
      {
        begin_matrix.row++;
          curr.col++;
        noSteps++;
          lc.setLed( 0 , curr.row - begin_matrix.col , curr.col - begin_matrix.row, true );
        delay(50);
          lc.setLed( 0 , curr.row - begin_matrix.col , curr.col - begin_matrix.row, false );
      
          delay(50);
         matrix8x8();
         delay(400);
          
          
          //matrix8x8();
        //  delay(100);
        
      }
    }
   delay(100);
   if ( trecut == 1 && healthActual < noSteps && matrix[curr.col+1][curr.row] == 0 && noSteps >0)
   lifes--,healthActual-=noSteps>4?noSteps-4:0,noSteps = 0;
  
   
//  delay(10);
  // lcd.clear();
   unsigned tms =0;
    
  if ( digitalRead(2) == HIGH )
  {
   
     if( curr.col == 1 && curr.row == 8 )//fuel
      {
        if( fuel <= 3 )
        {
          if ( (fuelMax - fuel)/10 > moneyActual)//nu am destui bani,dar ma grabesc
            {
              fuel =fuel +moneyActual*10; 
              moneyActual = 0;
              Serial.println("incarcat rapid");
            }
        }
        else
        {
          if(digitalRead(A4)==HIGH){//upgrade
          if(moneyActual >= 3 )
          {
            fuelMax +=20; 
            fuel = fuelMax;
            Serial.println("Fuel upgradat");
          }
          else Serial.println("Buget indisponibil");
        }
        else
        {
          tms = millis();
          if(tms > 3000 )
          {
            if(moneyActual > (fuelMax - fuel)/10){
            fuel = fuelMax;
            moneyActual -= (fuelMax - fuel)/10;
            Serial.print("fuel reincacat!");
            }
            else
            Serial.println("Buget indisponibil");
           
          }
        }
        
        }


        
      }
      else
      
        if( curr.col == 1 && curr.row == 15 )//selling
        {
        //  Serial.print("selling");
           tms = millis();
           if(tms>3000){ if(cargoActual>0)
         Serial.print("vandut");
        // else Serial.print("cargo empty");
         moneyActual += money;
         money = 0;
         cargoActual = 0;

        
         }
        // else Serial.print("wait");
          if(digitalRead(A4)==HIGH)
          {
            if(moneyActual >= 2)
            {
              moneyActual-=2;
              cargoMax+=2; Serial.println("Cargo upgraded");
            } 
         
          }
        
      }
      else
      if( curr.col == 1 && curr.row == 18 ){
          
           tms = millis();
           if (tms>3000)
           {
            if(moneyActual > 1)
            {
              healthActual = healthMax;
              moneyActual--;
            }
           }
           if(digitalRead(A4)==HIGH)
           {
            if(moneyActual>1){
              healthActual++;
              moneyActual--;
            }
           }
      }
      else{
        Serial.print("");
        Serial.println("-------------");
    Serial.print("Ora server:");
    Serial.println(millis());
    Serial.print("Fuel : ");
    Serial.print(fuel);
    Serial.print("/");
    Serial.println(fuelMax);
    Serial.print("Health : ");
    Serial.print(healthActual);
    Serial.print("/");
    Serial.println(healthMax);
    Serial.print("Cargo : ");
    Serial.print(cargoActual);
    Serial.print("/");
    Serial.println(cargoMax);
    Serial.print("Moneytotal : ");
    Serial.println(moneyActual);
    Serial.print("MoneyActual : ");
    Serial.println(money);
    Serial.print("Closest loot : ");
    if( a3 ==6)
    Serial.print("gold");
    else if(a2== 6)
    Serial.print("silver");
    else if(a1==6)
    Serial.print("bronz");
    else Serial.println("");
      }
  }
    if ( fuel <= 0 )
        {
           lifes--;
          if ( lifes)
          fuel = 50;
         lcd.clear();
          endGame();
          delay(500);
          matrix8x8();
        }
        
  }
  else endGame();

}
