#include <FastLED.h>

#define NUM_LEDS 256

#define DATA_PIN 3


#define Right 1
#define Up 2
#define Left 3
#define Down 4

int del_time = 1000;


CRGB leds[NUM_LEDS];


#define Vx 14
#define Vy 15
unsigned long last_t;

//Параметры поля
int field[16][16];

//Координаты еды
int x_food;
int y_food;

int direct;


// Координаты головы
int x_h;
int y_h;
//Координаты хвоста
int x_t;
int y_t;

void ShowSnake();
void CreateFood();
void Loose();
//-------------------------------------------------------------

//Функции игры
void set_tail() {
  switch (field[y_t][x_t])
  {
  case 2:
    if (x_t == 15) { field[y_t][x_t] = 0; x_t = 0; }
    else {
      field[y_t][x_t] = 0;
      x_t += 1;
    }
     break;
  case 1:
    if (y_t == 0) { field[y_t][x_t] = 0; y_t = 15; }
    else {
      field[y_t][x_t] = 0;
      y_t -= 1;
    }
    break;
  case 4:
    if (x_t == 0) { field[y_t][x_t] = 0; x_t = 15;}
    else {
      field[y_t][x_t] = 0;
      x_t -= 1;
    }
    break;
  case 3:
    if (y_t == 15) { field[y_t][x_t] = 0; y_t = 0; }
    else {
      field[y_t][x_t] = 0;
      y_t += 1;
    }
    break;
  }
}


void clean() {
  for (int i = 0; i <= 15; i++) {
    for (int j=0; j <= 15; j++) {
      field[i][j] = 0;
    }
  }
}

void set_snake() {
  x_h = 8;
  y_h = 6;

  x_t = 6;
  y_t = 6;

  field[6][6] = 2;
  field[6][7] = 2;
  field[6][8] = 2;
}

void change_direction(int dr) {
  delay(50);
  switch (dr)
  {
  case 2:
    if (field[y_h][x_h] != 4) {
       if (field[y_h][x_h + 1] == 7) {   

          x_h += 1;
          field[y_h][x_h] = 2;
          x_h += 1;
          field[y_h][x_h] = 2;
          CreateFood();
        }
        else {
        field[y_h][x_h] = 2;
        x_h += 1;
        field[y_h][x_h] = 2;
        }
         set_tail();
      }
    break;
  case 1:
    if (field[y_h][x_h] != 3) {
      if (field[y_h - 1][x_h] == 7) {
          y_h -= 1;
          field[y_h][x_h] = 1;
          y_h -= 1;
          field[y_h][x_h]=1;
          CreateFood();
        }
        else {
      field[y_h][x_h] = 1;
      y_h -= 1;
      field[y_h][x_h] = 1;
        }
      set_tail();
    }
    break;
  case 4:
    if (field[y_h][x_h] != 2) {
       if (field[y_h][x_h - 1] == 7) {
          x_h -= 1;
          field[y_h][x_h] = 4;
          x_h -= 1;
          field[y_h][x_h] = 4;
          CreateFood();
        }
        else {
      field[y_h][x_h] = 4;
      x_h -= 1;
      field[y_h][x_h] = 4;
        }
      set_tail();
    }
    break;
  case 3:
    if (field[y_h][x_h] != 1) {
      if (field[y_h + 1][x_h] == 7) {
          y_h += 1;
          field[y_h][x_h] = 3;
          y_h += 1;
          field[y_h][x_h] = 3;
          CreateFood();
        }
        else {
      field[y_h][x_h] = 3;
      y_h += 1;
      field[y_h][x_h] = 3;
        }
      set_tail();
    }
    break;
  }
  ShowSnake();
  delay(50);

}
//-------------------------------------------------------------------------------------

int x;
int y;
bool clrY;
bool cllY;
bool clr;
bool cll;


void ShowSnake() {
  =

  for (int i = 0; i <= 15; i++) {
    for (int j = 0; j <= 14; j++) {
      
      if(field[i][j]==0) {leds[LedXY(i+1,j+1)]=CRGB::Black;}
      if(field[i][j] !=0) {
           if(field[i][j] == 7) {
            leds[LedXY(i+1,j+1)] = 0x005e00;
           }
           else {
            
           
        leds[LedXY(i+1,j+1)] = 0x3c0000;
           }
        }
      } 
    
     if(field[i][15]==0) {leds[LedXY(i+1,16)]=CRGB::Black;}
     if(field[i][15] !=0) {
      if(field[i][15] == 7) {
            leds[LedXY(i+1,16)] = 0x005e00;
           }
           else {
       
            leds[LedXY(i+1,16)] = 0x3c0000;
           }
     }
    }
    FastLED.show();
    delay(80);
  }
//-----------------------------------------------------------------------
void setup() {
  randomSeed(analogRead(18));
  //Инициализация матрицы
  delay(2000);
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
 //---------- 
 
 clean();
 
 set_snake();
 
 CreateFood();
 
 //Пины для джойстика
 pinMode(Vx, INPUT); 
 pinMode(Vy, INPUT);


 
 clr= false;
 cll = false;
 
 clrY = false;
 cllY = false;
 
 last_t = 0;
}
//------------------------------------------------------------------


//Цикл игры------------------------------------------------------------
void loop() {
   
 //Пауза-------------
 
 if (millis() - last_t > del_time) {
  last_t = millis();
  
   //Если не было нажатия кнопки то просто передвигаем змейку-----------------------------
 
 switch (field[y_h][x_h])
     {
      case 2:
        direct = field[y_h][x_h + 1];
        if (direct == 1 || direct == 2 || direct == 3 || direct == 4 ) {
           Loose();
        }
        if (x_h == 15) { x_h = -1; }
        if (field[y_h][x_h + 1] == 7) {   

          x_h += 1;
          field[y_h][x_h] = 2;
          x_h += 1;
          field[y_h][x_h] = 2;
          CreateFood();
        }
        else {
          x_h += 1;
          field[y_h][x_h] = 2;
        }
        break;
      case 1:
      direct = field[y_h-1][x_h];
        if (direct == 1 || direct == 2 || direct == 3 || direct == 4 ) {
           Loose();
        }
        if (y_h == 0) { y_h = 16; }
        if (field[y_h - 1][x_h] == 7) {
          y_h -= 1;
          field[y_h][x_h] = 1;
          y_h -= 1;
          field[y_h][x_h]=1;
          CreateFood();
        }
        else {

          y_h -= 1;
          field[y_h][x_h] = 1;
        }
        break;
      case 4:
      direct = field[y_h][x_h-1];
        if (direct == 1 || direct == 2 || direct == 3 || direct == 4 ) {
           Loose();
        }
        if (x_h == 0) { x_h = 16; }
        if (field[y_h][x_h - 1] == 7) {
          x_h -= 1;
          field[y_h][x_h] = 4;
          x_h -= 1;
          field[y_h][x_h] = 4;
          CreateFood();
        }
        else {
          x_h -= 1;
          field[y_h][x_h] = 4;
        }
        break;
      case 3:
      direct = field[y_h+1][x_h];
        if (direct == 1 || direct == 2 || direct == 3 || direct == 4 ) {
           Loose();
        }
        if (y_h == 15) { y_h = -1; }
        if (field[y_h + 1][x_h] == 7) {
          y_h += 1;
          field[y_h][x_h] = 3;
          y_h += 1;
          field[y_h][x_h] = 3;
          CreateFood();
        }
        else {
          y_h += 1;
          field[y_h][x_h] = 3;
        }
        break;
      }
      set_tail();
      ShowSnake();
   
 }


   x =  analogRead(Vx);
   y = analogRead(Vy);
 //Проверяем координату y
 if(y > 1000 && clrY == false) {
 change_direction(Down);
 clrY = true;
 }
 if(y< 1000 && clrY == true) {clrY = false;}
 
 if(y < 100 && cllY == false) {
  change_direction(Up);
  cllY=true;
 }
 
 if(y > 100 && cllY == true) {cllY=false;}
 
 //Проверяем координату x
 if(x > 1000 && clr == false) {
 change_direction(Left);
 clr = true;
 }
 if(x< 1000 && clr == true) {clr = false;}
 
 if(x < 100 && cll == false) {
  change_direction(Right);
  cll=true;
 }
 if(x > 100 && cll == true) {cll=false;}
 
}
//----------------------------------------------------------------------------

int LedXY(int x, int y) {
  int row;
  int led_num;
  int y_cur=y-1;
  int x_cur = x-1;
  if(x >16 || y >16) {
    return 1;
  }
  else {
    row = y_cur%2;
    if(row ==0) {
       led_num = y_cur*16+x_cur; 
    }
    else {
       x_cur=15-x_cur;
       led_num = y_cur*16+x_cur;
    }
  return led_num;
  }
}


void Loose() {
   for (int i = 0; i <= 15; i++) {
    for (int j = 0; j <= 15; j++) {
      leds[LedXY(i+1,j+1)] = 0x00005e;
      FastLED.show();
      delay(700);
      } 
   }
}

void CreateFood() {
  bool no_eat = true;
  do
  {
    x_food = random(16);
    y_food = random(16);
    if (field[y_food][x_food] == 0)
    {
      field[y_food][x_food] = 7;
      no_eat = false;
    }
  } while (no_eat);
  
}
