#include <Adafruit_NeoPixel.h>

// Definición del pin del botón y LEDs
#define PIN_NEO_PIXEL 9  
#define NUM_PIXELS 12     
const int botonPin = 10;
const int buzzer = 8;

Adafruit_NeoPixel NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

// Variables para el estado del botón
bool estadoActual = LOW;
bool estadoAnterior = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(botonPin, INPUT_PULLDOWN);
  pinMode(buzzer, OUTPUT);
  NeoPixel.begin();  
  NeoPixel.clear();  
  rojo(); // Encender los LEDs en rojo al iniciar
}

void loop() {
  estadoActual = digitalRead(botonPin);

  // Detectar cuando el usuario suelta el botón
  if (estadoAnterior == HIGH && estadoActual == LOW) {
    Serial.println("Botón liberado");
    // Aquí puedes agregar la acción que desees cuando se suelte el botón
    // Por ejemplo, cambiar el color de los LEDs
    cambiarColor();
  }

  estadoAnterior = estadoActual;
}

void cambiarColor() {
  static int colorIndex = 0;

  switch (colorIndex) {
    case 0:
      rojo();
      break;
    case 1:
      verde();
      break;
    case 2:
      azul();
      break;
    case 3:
      amarillo();
      break;
    case 4:
      cyan();
      break;
    case 5:
      magenta();
      break;
    case 6:
      blanco();
      break;
    case 7:
      naranja();
      break;
    case 8:
      rosa();
      break;
    case 9:
      violeta();
      break;
    default:
      off();
      break;
  }

  colorIndex = (colorIndex + 1) % 10; // Hay 10 colores disponibles
}

void off() {
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {          
    NeoPixel.setPixelColor(pixel, NeoPixel.Color(0, 0, 0));  // Apagar
  }
  NeoPixel.show();  // Actualiza todos los píxeles al mismo tiempo
}

void rojo() {
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {          
    NeoPixel.setPixelColor(pixel, NeoPixel.Color(10, 0, 0));  // Rojo puro
  }
  NeoPixel.show();  // Actualiza todos los píxeles al mismo tiempo
  tone(buzzer, 392);  // Hacer sonar el buzzer a 1000 Hz
  delay(500);          // Duración del sonido
  noTone(buzzer);     // Apagar el buzzer
}

void verde() {
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {          
    NeoPixel.setPixelColor(pixel, NeoPixel.Color(0, 10, 0));  // Verde puro
  }
  NeoPixel.show();  // Actualiza todos los píxeles al mismo tiempo
}

void azul() {
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {          
    NeoPixel.setPixelColor(pixel, NeoPixel.Color(0, 0, 10));  // Azul puro
  }
  NeoPixel.show(); // Actualiza todos los píxeles al mismo tiempo
  
}

void amarillo() {
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {          
    NeoPixel.setPixelColor(pixel, NeoPixel.Color(10, 10, 0));  // Amarillo
  }
  NeoPixel.show();  // Actualiza todos los píxeles al mismo tiempo
}

void cyan() {
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {          
    NeoPixel.setPixelColor(pixel, NeoPixel.Color(0, 10, 10));  // Cian
  }
  NeoPixel.show();  // Actualiza todos los píxeles al mismo tiempo
}

void magenta() {
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {          
    NeoPixel.setPixelColor(pixel, NeoPixel.Color(10, 0, 10));  // Magenta
  }
  NeoPixel.show();  // Actualiza todos los píxeles al mismo tiempo
}

void blanco() {
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {          
    NeoPixel.setPixelColor(pixel, NeoPixel.Color(10, 10, 10));  // Blanco
  }
  NeoPixel.show();  // Actualiza todos los píxeles al mismo tiempo
}

void naranja() {
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {          
    NeoPixel.setPixelColor(pixel, NeoPixel.Color(10, 5, 0));  // Naranja
  }
  NeoPixel.show();  // Actualiza todos los píxeles al mismo tiempo
}

void rosa() {
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {          
    NeoPixel.setPixelColor(pixel, NeoPixel.Color(10, 0, 5));  // Rosa
  }
  NeoPixel.show();  // Actualiza todos los píxeles al mismo tiempo
}

void violeta() {
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {          
    NeoPixel.setPixelColor(pixel, NeoPixel.Color(5, 0, 10));  // Violeta
  }
  NeoPixel.show();  // Actualiza todos los píxeles al mismo tiempo
}
