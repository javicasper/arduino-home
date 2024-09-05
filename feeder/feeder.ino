#include <IRremote.hpp>  // Biblioteca para manejar el receptor IR
#include <Servo.h>       // Biblioteca para manejar el servomotor

const int RECV_PIN = 2;   // Pin donde está conectado el receptor IR
const int SERVO_PIN = 9;  // Pin donde está conectado el servomotor

// Códigos de los botones del mando a distancia
const long BUTTON_ACTION_CODE = 0xC;      // Código del botón para acción automática
const long BUTTON_OPEN_CODE = 0x18;       // Código del botón para abrir
const long BUTTON_CLOSE_CODE = 0x5E;      // Código del botón para cerrar

// Ángulos para abrir y cerrar
const int CLOSE_ANGLE = 0;  // Ángulo cerrado
const int OPEN_ANGLE = 60;   // Ángulo abierto

Servo myservo; // Crear un objeto Servo

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);  // Inicia el receptor IR en el pin definido
  myservo.attach(SERVO_PIN);  // Conectar el servomotor al pin definido
  myservo.write(CLOSE_ANGLE);  // Inicializa el servo en la posición cerrada
  Serial.println("Listo para recibir señales IR");
}

// Función para abrir el tapón (mover el servo a la posición abierta)
void openServo(bool withDelay) {
  for (int pos = CLOSE_ANGLE; pos <= OPEN_ANGLE; pos++) {
    myservo.write(pos);
    if (withDelay) {
      delay(2);  // Ajusta este valor para cambiar la velocidad (menor valor = movimiento más rápido)
    }
  }
}

// Función para cerrar el tapón (mover el servo a la posición cerrada)
void closeServo(bool withDelay) {
  for (int pos = OPEN_ANGLE; pos >= CLOSE_ANGLE; pos--) {
    myservo.write(pos);
    if (withDelay) {
      delay(2);  // Ajusta este valor para cambiar la velocidad
    }
  }
}

// Función para la acción automática de abrir, esperar y cerrar
void automaticAction() {
  openServo(true);
  delay(200);  // Espera 1 segundo en la posición abierta
  closeServo(true);
}

void loop() {
  if (IrReceiver.decode()) {
    long receivedCode = IrReceiver.decodedIRData.command;

    if (receivedCode == BUTTON_ACTION_CODE) {
      automaticAction();  // Realizar la acción automática
    } else if (receivedCode == BUTTON_OPEN_CODE) {
      openServo(false);  // Abrir el tapón
    } else if (receivedCode == BUTTON_CLOSE_CODE) {
      closeServo(false);  // Cerrar el tapón
    }

    // Imprimir el valor del código recibido en formato hexadecimal
    Serial.print("Código recibido: 0x");
    Serial.println(receivedCode, HEX);

    // Volver a recibir el próximo valor
    IrReceiver.resume();  
  }
}
