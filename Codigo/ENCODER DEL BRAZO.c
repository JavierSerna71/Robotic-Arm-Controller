#INCLUDE <16f887.h>
#DEVICE ADC=10
#USE DELAY(internal=8MHz)
#FUSES XT,NOPROTECT,NOWDT,NOBROWNOUT,PUT,NOLVP
#include <stdio.h>
#include <stdlib.h>
#include <map_function.c>
#use RS232(BAUD=9600,BITS=8,PARITY=N,XMIT=PIN_C6,RCV=PIN_C7)
#define CLK input(pin_b0)
#define DT input(pin_b1)
int16 PWM,KP=2,KD=0;
signed long  LEYDECONTROL,PID_P,PID_D,PID_I,ERROR,ERROR1,set=500,POTE;
float INTEGRAL,KI=0.04;
void PID()
{
   ERROR=set-POTE; 
   PID_P=KP*ERROR;
   PID_D=(KD*((ERROR-ERROR1)));
    if(-175 < ERROR && ERROR < 175)
    { 
      INTEGRAL=INTEGRAL+(KI*ERROR);
    }
    else
    {
      INTEGRAL = 0;
    }
     if(-3 < ERROR && ERROR < 3)
    {
      INTEGRAL = 0;
      LEYDECONTROL=0;
    }
    
      PID_I=INTEGRAL;
      LEYDECONTROL= PID_P+PID_D+PID_I;
      
    if (LEYDECONTROL >= 718.0){      
     LEYDECONTROL = 718.0;
   }
    if (LEYDECONTROL <= 0){       
     LEYDECONTROL = LEYDECONTROL*-1;
    }
    if (LEYDECONTROL >= 718.0){      
     LEYDECONTROL = 718.0;
   }
   
      PWM=LEYDECONTROL;
      ERROR1=ERROR;
       
      if(set>POTE){
      OUTPUT_BIT(PIN_D0,1);
      OUTPUT_BIT(PIN_D1,0);
      }
      if(set<POTE){
      OUTPUT_BIT(PIN_D0,0);
      OUTPUT_BIT(PIN_D1,1);
      }
      set_pwm1_duty(PWM+280);
}
void mostrar()
{
printf("ERROR=%Lu*POTE=%Lu*\r\n",ERROR,POTE);
}

void main()
{
   setup_timer_2(t2_div_by_1,255,1);
   setup_ccp1(ccp_pwm);
   setup_adc_ports(sAN0);
   setup_adc(ADC_CLOCK_INTERNAL);
   while(TRUE)
   {
      set_adc_channel(0);
      DELAY_MS(1);
      POTE = read_adc();
      PID();
      mostrar();
      
   }

}
