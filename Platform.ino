
#include <PS2X_lib.h>  //for v1.6




#include<stdio.h>

#include<math.h>

#include "Wire.h"

#include "I2Cdev.h"

#include "MPU6050.h"

MPU6050 accelgyro;







PS2X ps2x; // create PS2 Controller Class




//right now, the library does NOT support hot pluggable controllers, meaning

//you must always either restart your Arduino after you conect the controller,

//or call config_gamepad(pins) again after connecting the controller.

int error = 0;

byte type = 0;

byte vibrate = 0;







int lf = 6;

int lb = 9;

int rf = 5;

int rb = 3;




int lfv = 100;

int lbv = 100;

int rfv = 100;

int rbv = 100;




int blfv = 100;

int blbv = 100;

int brfv = 100;

int brbv = 100;




int bblfv = 100;

int bblbv = 100;

int bbrfv = 100;

int bbrbv = 100;




int16_t ax, ay, az;

int16_t gx, gy, gz;

int16_t bx, by, bz , bgx, bgy, bgz;

long double dx,dy,bdx, bdy;




void setup(){

    pinMode(lf,OUTPUT);

    pinMode(lb,OUTPUT);

    pinMode(rf,OUTPUT);

    pinMode(rb,OUTPUT);

   

    Wire.begin();




    accelgyro.initialize();




    accelgyro.getMotion6(&bx, &by, &bz, &bgx, &bgy, &bgz);




    bdx = atan2(bx,bz)*100;

    bdy = atan2(by,bz)*100;

   

    Serial.begin(38400);

 

    error = ps2x.config_gamepad(13,11,10,12, true, true);

    if(error == 0){

        Serial.println("Found Controller, configured successful");

        Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");

        Serial.println("holding L1 or R1 will print out the analog stick values.");

        Serial.println("Go to www.billporter.info for updates and to report bugs.");

    }

   

  else if(error == 1)

   Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

   

  else if(error == 2)

   Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

   

  else if(error == 3)

   Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

   

   //Serial.print(ps2x.Analog(1), HEX);

   

   type = ps2x.readType();

     switch(type) {

       case 0:

        Serial.println("Unknown Controller type");

       break;

       case 1:

        Serial.println("DualShock Controller Found");

       break;

       case 2:

         Serial.println("GuitarHero Controller Found");

       break;

     }

 

}




void loop(){

   /* You must Read Gamepad to get new values

   Read GamePad and set vibration values

   ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)

   if you don't enable the rumble, use ps2x.read_gamepad(); with no values

   

   you should call this at least once a second

   */

   

   accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    //Serial.print("a/g:\t");

    //Serial.print((ax-bx)*9.8/16384); Serial.print("m/s^2 \t");

    //Serial.print((ay-by)*9.8/16384); Serial.print("m/s^2 \t");

    //Serial.print((az-bz)*9.8/16384); Serial.print("m/s^2 \t");

    //Serial.print((gx)/131); Serial.print("d/s \t");

    //Serial.print((gy)/131); Serial.print("d/s \t");

    //Serial.print((gz)/131); Serial.println("d/s");

    dx = atan2(ax,az)*100 - bdx;

    dy = atan2(ay,az)*100 - bdy;

   

   

 if(error == 1) //skip loop if no controller found

  return;

 

 if(type == 2){ //Guitar Hero Controller

   

   ps2x.read_gamepad();          //read controller

   

   if(ps2x.ButtonPressed(GREEN_FRET))

     Serial.println("Green Fret Pressed");

   if(ps2x.ButtonPressed(RED_FRET))

     Serial.println("Red Fret Pressed");

   if(ps2x.ButtonPressed(YELLOW_FRET))

     Serial.println("Yellow Fret Pressed");

   if(ps2x.ButtonPressed(BLUE_FRET))

     Serial.println("Blue Fret Pressed");

   if(ps2x.ButtonPressed(ORANGE_FRET))

     Serial.println("Orange Fret Pressed");

     




    if(ps2x.ButtonPressed(STAR_POWER))

     Serial.println("Star Power Command");

   

    if(ps2x.Button(UP_STRUM))          //will be TRUE as long as button is pressed

     Serial.println("Up Strum");

    if(ps2x.Button(DOWN_STRUM))

     Serial.println("DOWN Strum");

 

 

    if(ps2x.Button(PSB_START))                   //will be TRUE as long as button is pressed

         Serial.println("Start is being held");

    if(ps2x.Button(PSB_SELECT))

         Serial.println("Select is being held");




   

    if(ps2x.Button(ORANGE_FRET)) // print stick value IF TRUE

    {

        Serial.print("Wammy Bar Position:");

        Serial.println(ps2x.Analog(WHAMMY_BAR), DEC);

    }

 }




 else { //DualShock Controller

 

    ps2x.read_gamepad(false, vibrate);          //read controller and set large motor to spin at 'vibrate' speed

   

    if(ps2x.Button(PSB_START))                   //will be TRUE as long as button is pressed

         Serial.println("Start is being held");

    if(ps2x.Button(PSB_SELECT))

         Serial.println("Select is being held");

         

         

     if(ps2x.Button(PSB_PAD_UP)) {         //will be TRUE as long as button is pressed

       Serial.print("Up held this hard: ");

       Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);

      }

      if(ps2x.Button(PSB_PAD_RIGHT)){

       Serial.print("Right held this hard: ");

        Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);

      }

      if(ps2x.Button(PSB_PAD_LEFT)){

       Serial.print("LEFT held this hard: ");

        Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);

      }

      if(ps2x.Button(PSB_PAD_DOWN)){

       Serial.print("DOWN held this hard: ");

     Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);

      }  

 

   

      vibrate = ps2x.Analog(PSAB_BLUE);        //this will set the large motor vibrate speed based on

                                              //how hard you press the blue (X) button    

   

    if (ps2x.NewButtonState())               //will be TRUE if any button changes state (on to off, or off to on)

    {

     

       

         

        if(ps2x.Button(PSB_L3))

         Serial.println("L3 pressed");

        if(ps2x.Button(PSB_R3))

         Serial.println("R3 pressed");

        if(ps2x.Button(PSB_L2))

         Serial.println("L2 pressed");

        if(ps2x.Button(PSB_R2))

         Serial.println("R2 pressed");

        if(ps2x.Button(PSB_GREEN))

         Serial.println("Triangle pressed");

         

    }  

         

   

    if(ps2x.ButtonPressed(PSB_RED))             //will be TRUE if button was JUST pressed

         Serial.println("Circle just pressed");

         

    if(ps2x.ButtonReleased(PSB_PINK))             //will be TRUE if button was JUST released

         Serial.println("Square just released");    

   

    if(ps2x.NewButtonState(PSB_BLUE))            //will be TRUE if button was JUST pressed OR released

         Serial.println("X just changed");    

   

   

    if(ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) // print stick values if either is TRUE

    {

        Serial.print("Stick Values:");

        Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX  

        Serial.print(",");

        Serial.print(ps2x.Analog(PSS_LX), DEC);

        Serial.print(",");

        Serial.print(ps2x.Analog(PSS_RY), DEC);

        Serial.print(",");

        Serial.println(ps2x.Analog(PSS_RX), DEC);




if ((ps2x.Analog(PSS_LY)==0)&&(ps2x.Analog(PSS_LX)==0)&&(ps2x.Analog(PSS_RY)==0)&&(ps2x.Analog(PSS_RX)==0)){}else{

        if (ps2x.Analog(PSS_LY)<120){

          lfv = lfv + (120-ps2x.Analog(PSS_LY))/5;

          if (lfv>255){

            lfv = 255;

          }

          lbv = lbv + (120-ps2x.Analog(PSS_LY))/5;

          if (lbv>255){

            lbv = 255;

          }

          rfv = rfv + (120-ps2x.Analog(PSS_LY))/5;

          if (rfv>255){

            rfv = 255;

          }

          rbv = rbv + (120-ps2x.Analog(PSS_LY))/5;

          if (rbv>255){

            rbv = 255;

          }

        }

       

        if (ps2x.Analog(PSS_LY)>130){

          lfv = lfv - (ps2x.Analog(PSS_LY)-130)/5;

          if (lfv<0){

            lfv = 0;

          }

          lbv = lbv - (ps2x.Analog(PSS_LY)-130)/5;

          if (lbv<0){

            lbv = 0;

          }

          rfv = rfv - (ps2x.Analog(PSS_LY)-130)/5;

          if (rfv<0){

            rfv = 0;

          }

          rbv = rbv - (ps2x.Analog(PSS_LY)-130)/5;

          if (rbv<0){

            rbv = 0;

          }

        }










       

        if (ps2x.Analog(PSS_RY)<120){

          lfv = lfv - (120-ps2x.Analog(PSS_RY))/10;

          if (lfv<0){

            lfv = 0;

          }

          lbv = lbv + (120-ps2x.Analog(PSS_RY))/10;

          if (lbv>255){

            lbv = 255;

          }

          rfv = rfv - (120-ps2x.Analog(PSS_RY))/10;

          if (rfv<0){

            rfv = 0;

          }

          rbv = rbv + (120-ps2x.Analog(PSS_RY))/10;

          if (rbv>255){

            rbv = 255;

          }

        }

       

        if (ps2x.Analog(PSS_RY)>130){

          lfv = lfv + (ps2x.Analog(PSS_RY)-130)/10;

          if (lfv>255){

            lfv = 255;

          }

          lbv = lbv - (ps2x.Analog(PSS_RY)-130)/10;

          if (lbv<0){

            lbv = 0;

          }

          rfv = rfv + (ps2x.Analog(PSS_RY)-130)/10;

          if (rfv>255){

            rfv = 255;

          }

          rbv = rbv - (ps2x.Analog(PSS_RY)-130)/10;

          if (rbv<0){

            rbv = 0;

          }

        }










       

        if (ps2x.Analog(PSS_RX)<120){

          lfv = lfv - (120-ps2x.Analog(PSS_RX))/10;

          if (lfv<0){

            lfv = 0;

          }

          lbv = lbv - (120-ps2x.Analog(PSS_RX))/10;

          if (lbv<0){

            lbv = 0;

          }

          rfv = rfv + (120-ps2x.Analog(PSS_RX))/10;

          if (rfv>255){

            rfv = 255;

          }

          rbv = rbv + (120-ps2x.Analog(PSS_RX))/10;

          if (rbv>255){

            rbv = 255;

          }

        }

       

        if (ps2x.Analog(PSS_RX)>130){

          lfv = lfv + (ps2x.Analog(PSS_RX)-130)/10;

          if (lfv>255){

            lfv = 255;

          }

          lbv = lbv + (ps2x.Analog(PSS_RX)-130)/10;

          if (lbv>255){

            lbv = 255;

          }

          rfv = rfv - (ps2x.Analog(PSS_RX)-130)/10;

          if (rfv<0){

            rfv = 0;

          }

          rbv = rbv - (ps2x.Analog(PSS_RX)-130)/10;

          if (rbv<0){

            rbv = 0;

          }

        }

    }

   

    }else{

   

 

  //----------------------------------Gra------------------

    if(dx>0){

      if (lbv<=245 && rbv<=245){

        lbv = lbv + 10;

        rbv = rbv + 10;

      }

      else{

        if (lfv>10 && rfv>10){

          lfv = lfv - 10;

          rfv = rfv - 10;

        }

      }

    }

    if(dy<0){

      if (lfv<=245 && rfv<=245){

        lfv = lfv + 10;

        rfv = rfv + 10;

      }

      else{

        if (lbv>10 && rbv>10){

          lbv = lbv - 10;

          rbv = rbv - 10;

        }

      }

    }

    if(dy<0){

      if (lbv<=245 && lfv<=245){

        lbv = lbv + 10;

        lfv = lfv + 10;

      }

      else{

        if (rbv>10 && rfv>10){

          rbv = rbv - 10;

          rfv = rfv - 10;

        }

      }

    }

    if(dy>0){

      if (rbv<=245 && rfv<=245){

        rbv = rbv + 10;

        rfv = rfv + 10;

      }

      else{

        if (lbv>10 && lfv>10){

          lbv = lbv - 10;

          lfv = lfv - 10;

        }

      }

    }

  //----------------------------------Gra------------------

 

    blfv = lfv;

    blbv = lbv;

    brfv = rfv;

    brbv = rbv;

    }

 }

 

    analogWrite(lf,lfv);

    analogWrite(lb,lbv);

    analogWrite(rf,rfv);

    analogWrite(rb,rbv);

 

 delay(50);

     

}
