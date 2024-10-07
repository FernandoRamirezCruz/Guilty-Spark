// Código del Maestro
#include <WiFi.h>
#include <vector>
#include <Adafruit_NeoPixel.h>
// Configuración del Access Point
const char* ssid = "ESP32_AP";       // Nombre de la red que creará el maestro
const char* password = "12345678";   // Contraseña de la red (mínimo 8 caracteres)
int conectados=0;
bool star=false;
WiFiServer server(1234); // Puerto en el que el servidor escuchará
bool x = true;
// Estructura para almacenar información de cada esclavo
struct Slave {
  String id;
  WiFiClient client;
};
String letrasColores[] = {
  "ROJO",
  "VERDE",
  "AZUL",
  "AMARILLO",
  "CYAN",
  "MAGENTA",
  "BLANCO",
  "NARANJA",
  "ROSA",
  "VIOLETA"
};

std::vector<Slave> slaves;
//////////////////////////////ESTRUCTURA DE NODOS PARA SIMON DICE//////////////
// Definición de la estructura del nodo
struct Node {


  int slave;           // Identificador del esclavo
  String color;        // Color asociado al esclavo
  Node* next;          // Puntero al siguiente nodo

  // Constructor para facilitar la creación de nodos
  Node(int s, String c) : slave(s), color(c), next(NULL) {}

};
// Clase para manejar la cola simplemente enlazada
class Queue {
  private:
    Node* front;    // Puntero al frente de la cola
    Node* rear;     // Puntero al final de la cola

  public:
    // Constructor: Inicializa la cola vacía
    Queue() : front(NULL), rear(NULL) {}

    // Destructor: Limpia toda la cola al destruir el objeto
    ~Queue() {
      while (!isEmpty()) {
        dequeue();
      }
    }

    // Función para verificar si la cola está vacía
    bool isEmpty() {
      return front == NULL;
    }

    // Función para añadir un nuevo nodo al final de la cola
    void enqueue(int slave, String color) {
      Node* newNode = new Node(slave, color); // Crear un nuevo nodo
      if (isEmpty()) {
        // Si la cola está vacía, el nuevo nodo es tanto el frente como el final
        front = rear = newNode;
      } else {
        // Añadir el nuevo nodo al final y actualizar el puntero rear
        rear->next = newNode;
        rear = newNode;
      }
      Serial.print("Enqueued: Slave = ");
      Serial.print(slave);
      Serial.print(", Color = ");
      Serial.println(color);
    }

    // Función para eliminar el nodo del frente de la cola
    void dequeue() {
      if (isEmpty()) {
        Serial.println("La cola está vacía. No se puede realizar dequeue.");
        return;
      }

      Node* temp = front; // Nodo a eliminar
      front = front->next; // Actualizar el frente

      // Si después de eliminar, la cola está vacía, rear también debe ser NULL
      if (front == NULL) {
        rear = NULL;
      }

      Serial.print("Dequeued: Slave = ");
      Serial.print(temp->slave);
      Serial.print(", Color = ");
      Serial.println(temp->color);

      delete temp; // Liberar la memoria del nodo eliminado
    }

    // Función para recorrer y mostrar todos los nodos de la cola
    void traverse() {
      if (isEmpty()) {
        Serial.println("La cola está vacía.");
        return;
      }

      Serial.println("Contenido de la cola:");
      Node* current = front;
      while (current != NULL) {
        Serial.print("Slave = ");
        Serial.print(current->slave);
        Serial.print(", Color = ");
        Serial.println(current->color);
        current = current->next;
      }
    }
};
////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);

  // Iniciar el modo Access Point
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("IP del Access Point: ");
  Serial.println(IP);

  // Iniciar el servidor
  server.begin();
  Serial.println("Servidor iniciado");
  Serial.println("El maestro está listo para aceptar conexiones de esclavos.");
  
}

void loop() {

  aceptarNuevosEsclavos();
  gestionarEsclavos();
  if(star){



  }
  
  
}

void simonDice(){



}

void reflejos(){

int intentos=0;
bool correcto = true;
bool push= false;
int prevNumber = 0;     // Almacena el número anterior, inicializado fuera del rango 1-3
int currentNumber = 0;  // Almacena el número actual

int respuesta;
  do{
    intentos++;
    do{
        push=false;
        currentNumber = random(1, 4);
    }while (currentNumber == prevNumber);
    
    push=botonPresionado();
    respuesta = leerBoton(); 
    if(push){
      if((respuesta == currentNumber)){
        intentos++;
      }
      else{
        correcto=false;
      }
    }


  }while(intentos<10 && correcto );
    if(correcto){
        GANAR();
    }else{
      PERDER();
    }

}

bool botonPresionado(){
//Si se a presionado algun boton de los slaver retornara un true, de lo contraio (no esten presionando nada) retornara un false  
}
int leerBoton(){
//Me va a salir de que numero de slaver sale, por ejemplo del slaver 1 retornara un 1
}




void INICIO(){
  enviarComando("Slave1", "BLANCO");
  delay(2000);
  star=true;
  enviarComando("Slave1", "OFF");
}
void GANAR(){
  enviarComando("Slave1", "WIN");
  enviarComando("Slave2", "WIN");
  enviarComando("Slave3", "WIN");
}
void PERDER(){
  enviarComando("Slave1", "LOSE");
  enviarComando("Slave2", "LOSE");
  enviarComando("Slave3", "LOSE");
}
void aceptarNuevosEsclavos() {
  if (server.hasClient()) {
    WiFiClient newClient = server.available();
    if (newClient) {
      Serial.println("Nuevo esclavo conectado");
      x=true;
      // Leer el identificador del esclavo.
      String slaveId = newClient.readStringUntil('\n');
      slaveId.trim();

      // Agregar el nuevo esclavo a la lista
      Slave newSlave;
      newSlave.id = slaveId;
      newSlave.client = newClient;
      slaves.push_back(newSlave);

      Serial.print("Esclavo agregado: ");
      Serial.println(slaveId);
      conectados++;
      
      if(conectados == 1){
        INICIO();
      }

    }
  }
}

void gestionarEsclavos() {
  for (auto it = slaves.begin(); it != slaves.end(); ) {
    if (it->client.connected()) {
      if (it->client.available()) {
        String mensaje = it->client.readStringUntil('\n');
        mensaje.trim();

        if (mensaje == "BOTON_PRESIONADO") {
          Serial.println("Botón del " + it->id + " presionado");

        } else {
          Serial.print("Mensaje de ");
          Serial.print(it->id);
          Serial.print(": ");
          Serial.println(mensaje);
        }
      }
      ++it;
    } else {
      Serial.print("Esclavo desconectado: ");
      Serial.println(it->id);
      it = slaves.erase(it);
    }
  }
}

void enviarComando(const String& slaveId, const String& command) {
  bool found = false;
  for (auto& slave : slaves) {
    if (slave.id == slaveId) {
      if (slave.client.connected()) {
        slave.client.println(command);
        Serial.print("Comando enviado a ");
        Serial.print(slaveId);
        Serial.print(": ");
        Serial.println(command);
      } else {
        Serial.print("El esclavo ");
        Serial.print(slaveId);
        Serial.println(" no está conectado.");
      }
      found = true;
      break;
    }
  }
  if (!found) {
    Serial.print("Esclavo no encontrado: ");
    Serial.println(slaveId);
  }
}
