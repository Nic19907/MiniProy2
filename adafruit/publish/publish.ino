#include "config.h"

void setup() {
  for (int i=0; i<=4;i++){
     cadena[i] = '0'; 
    }
  //pinMode(LED, OUTPUT);
  //pinMode(SLIDER, INPUT);


  Serial.begin(115200);
  Serial2.begin(115200);
  
  while(! Serial);


  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.

  //nombre cosa -> onMessage(handleMessage1)

  
  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // Because Adafruit IO doesn't support the MQTT retain flag, we can use the
  // get() function to ask IO to resend the last value for this feed to just
  // this MQTT client after the io client is connected.

  //nombre_cosa->get


  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {
  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  // save count to the 'counter' feed on Adafruit IO
   
   /*El switch es necesario para que la recepcion sea lo 
     mas rapida posible. Si se realizan las escrituras 
     una tras una, la recepcion tarda todo el tiempo de 
     envio*/ 
  switch(count){
    case 1: // Verde
      Serial.print("sending -> ");
      Serial.println(AVerde);
      VerPICfeed->save(AVerde);
      count++;
      break;
      
    case 2: // Amarillo
      Serial.print("sending -> ");
      Serial.println(AAmarillo);
      AmaPICfeed->save(AAmarillo);
      count++;
      break;
      
    case 3: // Rojo
      Serial.print("sending -> ");
      Serial.println(ARojo);
      RojPICfeed->save(ARojo);
      count++;
      break; 
      
    case 4: // Altura
      Serial.print("sending -> ");
      Serial.println(cadena);
      AlturaFeed->save(cadena);
      count=1;
      break; 
      
    default:
      count = 1;
      break;
  }
  
  // Disponibilidad en el serial2
  while(Serial2.available()>0){
    // Guardar la cadena
    static char mensaje[5];
    static unsigned int pos_Mensaje = 0;

    // Bit a bit
    char bit_Mensaje = Serial2.read();  

    // Recepcion de mensaje
    if(bit_Mensaje != '\n' && pos_Mensaje <5){
      mensaje[pos_Mensaje] = bit_Mensaje;
      pos_Mensaje++; // Recorrey el array
    }else{
      //mensaje[pos_Mensaje]='\0';
      Serial2.print(mensaje);
      pos_Mensaje = 0;
    }

    for (int i=0; i<=4;i++){
     cadena[i] = mensaje[i]; 
    }
  }
  //Serial.print("Altura:");
  //Serial.println(cadena);
  delay(3000);
}

// RECEPCION
// this function is called whenever a 'counter' message
// is received from Adafruit IO. it was attached to
// the counter feed in the setup() function above.
void handleMessage1(AdafruitIO_Data *data) {
  Serial.print("received <- ");
  Serial.println(data->value());
  Semaforo = *data->value();
  
