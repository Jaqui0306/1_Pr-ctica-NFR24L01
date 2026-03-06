#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Definición de pines del ESP32 utilizados para conectar el módulo NRF24L01
#define CE_PIN   17
#define CSN_PIN  5
#define SPI_SCK  18
#define SPI_MOSI 23
#define SPI_MISO 19

// Creación del objeto radio para controlar el módulo
RF24 radio(CE_PIN, CSN_PIN);

// Dirección de comunicación que debe coincidir con la del receptor
const byte address[6] = "00001";

void setup() {

  // Inicializa la comunicación serial para monitorear el funcionamiento
  Serial.begin(115200);
  delay(300);
  Serial.println("\nTransmisor ESP32 - Iniciando...");

  // Inicializa el bus SPI con los pines especificados
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

  // Inicializa el módulo NRF24L01 y verifica que funcione correctamente
  if (!radio.begin()) {
    Serial.println("ERROR: radio.begin() falló → revisa cables y alimentación");
    Serial.println("→ Condensador 10-100µF entre VCC y GND del nRF?");
    while (true) delay(5000);
  }

  Serial.println("radio.begin() OK ✓");

  // Configuración de parámetros de comunicación del módulo
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(76);

  // Se define la dirección a la que se enviarán los datos
  radio.openWritingPipe(address);

  // Se coloca el módulo en modo transmisión
  radio.stopListening();

  Serial.println("Configuración completa. Enviando...");

  // Muestra en el monitor serial la configuración interna del módulo
  radio.printDetails();
}

void loop() {

  // Mensaje que será enviado al receptor
  const char texto[] = "Hola desde ESP32 TX";

  // Se envía el mensaje mediante el módulo NRF24L01
  bool ok = radio.write(&texto, sizeof(texto));

  // Se muestra en el monitor serial si el envío fue exitoso o falló
  Serial.print("Enviado: ");
  Serial.print(texto);
  Serial.print(" → ");
  Serial.println(ok ? "OK (en FIFO)" : "FALLO");

  // Espera antes de volver a enviar el mensaje
  delay(1000);
}