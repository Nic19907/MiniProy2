// Adafruit IO Publish & Subscribe Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/

// this int will hold the current count for our sketch
//int count = 0;

int sensor;

char temporal_semaforo;
unsigned char e_sem;
int e_send = 0;

//estado de las LEDS
unsigned char e_ledV;
unsigned char e_ledA;
unsigned char e_ledR;

int pic_val;

// Track time of last published messages and limit feed->save events to once
// every IO_LOOP_DELAY milliseconds.
//
// Because this sketch is publishing AND subscribing, we can't use a long
// delay() function call in the main loop since that would prevent io.run()
// from being called often enough to receive all incoming messages.
//
// Instead, we can use the millis() function to get the current time in
// milliseconds and avoid publishing until IO_LOOP_DELAY milliseconds have
// passed.
#define IO_LOOP_DELAY 5000
unsigned long lastUpdate = 0;

// set up the 'counter' feed
//AdafruitIO_Feed *counter = io.feed("counter");
//PIC
AdafruitIO_Feed *senFeed = io.feed("sensor");

//adafruit
//BOTONES
AdafruitIO_Feed *botV_F = io.feed("boton1");
AdafruitIO_Feed *botA_F = io.feed("boton2");
AdafruitIO_Feed *botR_F = io.feed("boton3");

//LEDS
AdafruitIO_Feed *ledV_F = io.feed("led1");
AdafruitIO_Feed *ledA_F = io.feed("led2");
AdafruitIO_Feed *ledR_F = io.feed("led3");

//asignar otro para el valor del led
void setup() {
  pinMode(2,OUTPUT);
  // start the serial connection
  Serial.begin(115200); //wifi
  Serial2.begin(115200); //pic

  // wait for serial monitor to open
  //while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  botV_F->onMessage(handleMessage);
  botA_F->onMessage(handleMessage);
  botR_F->onMessage(handleMessage);

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  botV_F->get();
  botA_F->get();
  botR_F->get();
}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  //enviar
  if (millis() > (lastUpdate + IO_LOOP_DELAY)) {
    // save count to the 'counter' feed on Adafruit IO

    


    while(Serial2.available()>0){
      pic_val = Serial2.read(); //leer el valor que proviene del pic, aka el sensor
    }

    

    //enviar valores al adafruit
    //se hace un multiplexeo de los datos enviados para no saturar el ADAFRUIT
    switch (e_send) {
      case 0: //luz verde
        Serial.print("sending -> ");
        Serial.println(e_ledV);
        ledV_F->save(e_ledV);
        e_send++;
        break;

      case 1: //luz amarilla
        Serial.print("sending -> ");
        Serial.println(e_ledA);
        ledA_F->save(e_ledA);
        e_send++;
        break;

      case 2: //luz roja
        Serial.print("sending -> ");
        Serial.println(e_ledR);
        ledR_F->save(e_ledR);
        e_send++;
        break;
      
      case 3: //sensor de temperatura
        //sensor = random(0,50);//en lugar de random hacer que sea el valor del PIC
        switch (pic_val){
          case '0':
            sensor = 15;
            break;
      
          case '1':
            sensor = 16;
            break;
      
          case '2':
            sensor = 17;
            break;
      
          case '3':
            sensor = 18;
            break;
      
          case '4':
            sensor = 19;
            break;
      
          case '5':
            sensor = 20;
            break;
      
          case '6':
            sensor = 21;
            break;
      
          case '7':
            sensor = 22;
            break;
      
          case '8':
            sensor = 23;
            break;
      
          case '9':
            sensor = 24;
            break;
      
          case '!':
            sensor = 25;
            break;
      
          case '@':
            sensor = 26;
            break;
      
          case '#':
            sensor = 27;
            break;
      
          case '$':
            sensor = 28;
            break;
      
          case '%':
            sensor = 29;
            break;
      
          case '^':
            sensor = 30;
            break;
      
          case '&':
            sensor = 31;
            break;
      
          case '*':
            sensor = 32;
            break;
      
          case '<':
            sensor = 33;
            break;
      
          case '>':
            sensor = 34;
            break;
      
          default:
            sensor = 40;
            break;
        }
        Serial.print("sending -> ");
        Serial.println(sensor);
        senFeed->save(sensor);
        e_send = 0;
        break;

      default:
        e_send = 0;
        break;
    }
    
    
     

    // increment the count by 1

    // after publishing, store the current time
    lastUpdate = millis();
  }
}

// is received from Adafruit IO. it was attached to
// the counter feed in the setup() function above.

//Recepcion
void handleMessage(AdafruitIO_Data *data) {
  //adafruit
  //Serial.print("received <- ");
  //Serial.println(data->value());

  e_sem = *data ->value();

  //Serial2.print(botR_F); 


  switch (e_sem) {
    case '1'://luz verde
      Serial.print("received <- ");
      temporal_semaforo = '1';
      Serial2.print(temporal_semaforo);
      e_ledV = 1;
      e_ledA = 0;
      e_ledR = 0;
      Serial.println("Verde");
      
      break;

    case '2'://luz amarilla
      Serial.print("received <- ");
      temporal_semaforo = '2';
      Serial2.print(temporal_semaforo);
      e_ledV = 0;
      e_ledA = 1;
      e_ledR = 0;
      Serial.println("Amarillo");
      break;

    case '4'://luz roja
      Serial.print("received <- ");
      temporal_semaforo = '4';
      Serial2.print(temporal_semaforo);
      e_ledV = 0;
      e_ledA = 0;
      e_ledR = 1;
      Serial.println("Rojo");
      break;

    default:
      break;
  }

  //aqui puedo hacer codigo
}

void sensor_map (){
  switch (pic_val){
    case '0':
      sensor = 15;
      break;

    case '1':
      sensor = 16;
      break;

    case '2':
      sensor = 17;
      break;

    case '3':
      sensor = 18;
      break;

    case '4':
      sensor = 19;
      break;

    case '5':
      sensor = 20;
      break;

    case '6':
      sensor = 21;
      break;

    case '7':
      sensor = 22;
      break;

    case '8':
      sensor = 23;
      break;

    case '9':
      sensor = 24;
      break;

    case '!':
      sensor = 25;
      break;

    case '@':
      sensor = 26;
      break;

    case '#':
      sensor = 27;
      break;

    case '$':
      sensor = 28;
      break;

    case '%':
      sensor = 29;
      break;

    case '^':
      sensor = 30;
      break;

    case '&':
      sensor = 31;
      break;

    case '*':
      sensor = 32;
      break;

    case '<':
      sensor = 33;
      break;

    case '>':
      sensor = 34;
      break;

    default:
      sensor = 40;
      break;
  }
}

//tomar el valor, guardarlo, x10, sumarlo al segundo valor
