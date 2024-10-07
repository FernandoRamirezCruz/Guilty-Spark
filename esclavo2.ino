// Código del Esclavo 2

#include <WiFi.h>
#include <Adafruit_NeoPixel.h>

// Configuración de Wi-Fi
const char* ssid = "ESP32_AP";       // Nombre de la red creada por el maestro
const char* password = "12345678";   // Contraseña de la red
const char* host = "192.168.4.1";    // IP del Access Point del maestro
const int port = 1234;               // El mismo puerto que el servidor

// Identificador único del esclavo
const String slaveId = "Slave2";

// Definición del pin del botón y LEDs
#define PIN_NEO_PIXEL 9  
#define NUM_PIXELS 12     
const int botonPin = 10;

bool estadoActual = LOW;
bool estadoAnterior = LOW;

Adafruit_NeoPixel NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

// Cliente WiFi
WiFiClient client;

void setup() {
  Serial.begin(115200);
  pinMode(botonPin, INPUT_PULLDOWN);
  NeoPixel.begin();  
  NeoPixel.clear();  

  // Conectar al Access Point del maestro
  Serial.println("Conectando al Access Point...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConectado al Access Point");

  conectarAlMaestro();
}

void loop() {
  // Verificar si está conectado al maestro
  if (client.connected()) {
    // Lectura del estado del botón
    estadoActual = digitalRead(botonPin);

    if (estadoAnterior == LOW && estadoActual == HIGH) {
      Serial.println("Botón presionado");
      client.println("BOTON_PRESIONADO");

      // Control de NeoPixels al presionar el botón
      win();
      delay(1000);  // Mantener el color por 1 segundo
      off();        // Apagar los NeoPixels
    }
    estadoAnterior = estadoActual;

    // Procesar mensajes del maestro
    if (client.available()) {
      String mensaje = client.readStringUntil('\n');
      mensaje.trim();

      if (mensaje.equalsIgnoreCase("WIN")) {
        Serial.println("Comando 'WIN' recibido del maestro.");
        win();
        delay(1000);  // Mantener el color por 1 segundo
        off();
        client.println("WIN ejecutado");
      }
      // Puedes agregar más comandos aquí si lo deseas
    }

  } else {
    Serial.println("Desconectado del maestro. Intentando reconectar...");
    client.stop();
    delay(5000);
    conectarAlMaestro();
  }
}

void conectarAlMaestro() {
  if (client.connect(host, port)) {
    Serial.println("Conectado al maestro");
    client.println(slaveId);
  } else {
    Serial.println("Fallo al conectar al maestro. Reintentando en 5 segundos...");
    delay(5000);
  }
}

void win() {
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {          
    NeoPixel.setPixelColor(pixel, NeoPixel.Color(0, 10, 0));  // Verde puro
  }
  NeoPixel.show();  // Actualiza todos los píxeles al mismo tiempo
}

void off() {
  NeoPixel.clear();    // Apaga todos los píxeles
  NeoPixel.show();
}

void lose() {
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {          
    NeoPixel.setPixelColor(pixel, NeoPixel.Color(10, 0, 0));  // Rojo puro
  }
  NeoPixel.show();  // Actualiza todos los píxeles al mismo tiempo
}
