#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Definición de pines utilizados para conectar el módulo NRF24L01
#define CE_PIN  16
#define CSN_PIN 10

// Pines utilizados para la comunicación SPI
#define SPI_SCK  12
#define SPI_MISO 13
#define SPI_MOSI 11

// Se crea el objeto radio para controlar el módulo NRF24L01
RF24 radio(CE_PIN, CSN_PIN);

// Dirección de comunicación que debe coincidir con el transmisor
const byte address[6] = "00001";

void setup() {

  // Inicialización de la comunicación serial para monitorear el funcionamiento
  Serial.begin(115200);
  delay(300);
  Serial.println("\nReceptor ESP32-S3 - Iniciando...");

  // Inicialización del bus SPI para la comunicación con el módulo
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

  // Inicializa el módulo NRF24L01 y verifica si responde correctamente
  if (!radio.begin()) {
    Serial.println("ERROR: radio.begin() falló → revisa cables y 3.3V");
    while (true) delay(5000);
  }

  Serial.println("radio.begin() OK");

  // Verificación de que el chip NRF24L01 esté conectado
  if (radio.isChipConnected()) {
    Serial.println("Chip NRF conectado ✓");
  } else {
    Serial.println("Chip NO detectado → problema SPI o módulo");
  }

  // Configuración de los parámetros de comunicación del módulo
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(76);

  // Se abre un canal de recepción y se coloca el módulo en modo escucha
  radio.openReadingPipe(1, address);
  radio.startListening();

  Serial.println("Escuchando...");
}

void loop() {

  // Se verifica si hay datos disponibles para recibir
  if (radio.available()) {

    // Se lee el mensaje recibido desde el transmisor
    char texto[32] = "";
    radio.read(&texto, sizeof(texto));

    // Se muestra el mensaje recibido en el monitor serial
    Serial.print("RECIBIDO: ");
    Serial.println(texto);
  }

  // Pequeño retardo para estabilidad del programa
  delay(50);
}