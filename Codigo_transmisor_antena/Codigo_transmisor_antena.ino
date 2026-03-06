#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Pines para ESP32 (ajusta si usas otros)
#define CE_PIN   17
#define CSN_PIN  5
#define SPI_SCK  18
#define SPI_MOSI 23
#define SPI_MISO 19

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println("\nTransmisor ESP32 - Iniciando...");

  // Inicializa SPI con pines explícitos
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

  if (!radio.begin()) {
    Serial.println("ERROR: radio.begin() falló → revisa cables y alimentación");
    Serial.println("→ Condensador 10-100µF entre VCC y GND del nRF?");
    while (true) delay(5000);
  }

  Serial.println("radio.begin() OK ✓");

  radio.setPALevel(RF24_PA_MIN);     // Empieza con MIN para menos consumo
  radio.setDataRate(RF24_250KBPS);   // Más estable
  radio.setChannel(76);
  radio.openWritingPipe(address);
  radio.stopListening();

  Serial.println("Configuración completa. Enviando...");
  radio.printDetails();  // Muestra info del chip (copia esto al probar)
}

void loop() {
  const char texto[] = "Hola desde ESP32 TX";
  bool ok = radio.write(&texto, sizeof(texto));

  Serial.print("Enviado: ");
  Serial.print(texto);
  Serial.print(" → ");
  Serial.println(ok ? "OK (en FIFO)" : "FALLO");

  delay(1000);
}