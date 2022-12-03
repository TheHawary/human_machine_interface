#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"
#include "ADC.h"

// Function Prototypes
void init();

int main(void){

  init();     // Call Initialization Function
  unsigned char buffer_1[4] ;   // temp_C  (temperature in Celsius)
  unsigned char buffer_2[4] ;   // temp_F  (temperature in FAhrenhait)
  unsigned char buffer_3[4] ;   // limit_C (limit in Celsius)
  unsigned char buffer_4[4] ;   // limit_F (limit in Fahrenhait)

  int temp_unit = 0;        // 0 --> Celsius and 1 --> Fahrenhait
  int limit = 250;          // Limit (room temperature)
  PORTD = PORTD &~ (1<<3);  // LED 1 stays off
  
  while (1)
  {
    int command = ADC_Read(0);        // Reads pin A0 to know which button is pressed
    int temp_C = (ADC_Read(1))/10;    // Reads pin A1 from the temperature sensor and coverts it to celsius
    int temp_F = (temp_C*1.8) + 32;   // Conversion to fahrenhait
    int limit_C = (limit)/10;         // Limit of temperature before taking action accordingly
    int limit_F = (limit_C*1.8) + 32; // Limit in Fahrenhait
    
    // itoa functions to change integers to strings to be shown on LCD screen
    itoa(temp_C, buffer_1, 10);
    itoa(temp_F, buffer_2, 10);
    itoa(limit_C, buffer_3, 10);
    itoa(limit_F, buffer_4, 10);

    // Functions of buttons:
    // No buttons pressed
    if(command > 1013){    
      LCD_String ("Temperature:");      
    } 
    
    // Select button
    else if (command  > 715){  
      if(temp_unit == 0){
        LCD_String_xy(0,0, "Limit:");
        LCD_String_xy(1,0,buffer_3);  
        LCD_String_xy(1, 4,"Celsius");
        _delay_ms(700);      
      }
      else{
        LCD_String_xy(0,0, "Limit:");
        LCD_String_xy(1,0,buffer_4);  
        LCD_String_xy(1, 4,"Fahrenhait");
        _delay_ms(700);
      } 
    }
    
    // Left button
    else if(command > 450){
      limit += 2*10.23;
      LCD_String_xy(0, 1,"+2 to limit");
      _delay_ms(500);
    } 

    // Down button
    else if(command > 290){
      LCD_String ("-1 to limit");
      _delay_ms(500);
      limit -= 10.23;
    } 

    // Up button
    else if(command > 110){
      LCD_String ("+1 to limit");
      _delay_ms(500);
      limit += 10.23;
    } 

    //Right button
    else{
      LCD_String ("Loading....");
      if(temp_unit == 0){
        temp_unit = 1;
      }
      else{
        temp_unit = 0;
      }
      _delay_ms(600);
    } 
    
    // Displaying "Celsius" or "Fahrenhait" according to temp_unit
    if(temp_unit == 0){ 
      LCD_String_xy(1, 0,buffer_1);
      LCD_String_xy(1, 4,"Celsius");
    }
    else{
      LCD_String_xy(1, 0,buffer_2);
      LCD_String_xy(1, 4,"Fahrenhait");
    }

    // Controlling LEDs and buzzers according to temperature
    // Lower range  (COLD)
    if((temp_C < 15)){
    PORTD = PORTD &~ (1<<3);       // LED 1 stays off (red)
    PORTD = PORTD | (1<<2);        // LED 2 stays on (blue)
    PORTB = PORTB &~ (1<<3);       // Buzzer stays off
    }
    // Upper Range (HOT)
    else if ((temp_C > limit_C)){ 
    PORTD = PORTD | (1<<3);        // LED 1 stays on (red)
    PORTD = PORTD &~ (1<<2);       // LED 2 stays off (blue)
    PORTB = PORTB | (1<<3);        // Buzzer turns on
    }
    // In Range 
    else if(temp_C > 15){
      PORTD = PORTD &~ (1<<2);     //LED 2 stays off
      PORTD = PORTD &~ (1<<3);     //LED 1 stays off 
      PORTB = PORTB &~ (1<<3);     // Buzzer stays off
    }
    _delay_ms(100);
    LCD_Clear();
  }
  return 0; 
}

void init(){

  ADC_Init();        // Initializing ADC library
  LCD_Init ();       // Initializing LCD library
  
  
  // Setting pins  
  DDRD |= (1<<3);    // Setting LED 1 pin to output
  DDRD |= (1<<2);    // Setting LED 2 pin to output
  DDRB |= (1<<3);    // Setting Buzzer pin to output 
}