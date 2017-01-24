// Librerías utilizadas
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MLX90614.h>

// Variables de retardo para antirebote
volatile unsigned long last_micros;
long debouncing_time = 250; //Debouncing Time in Milliseconds

// Pin y Bandera de cambio de estado, para seleccionar Grados Celsius o Fahrenheit
const byte interruptPin = 2;
volatile byte select = LOW;
volatile byte measuring = HIGH;

// Creamos un cáracter personalizado
# define Circle 0
byte customChar[] = {
  B01110,
  B01010,
  B01110,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

// Se declara el objeto
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// Ajusta la dirección de la LCD a 0x27 para una pantalla de 16 caracteres y 2 lineas
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Pin de interrupcion para cambiar de grados Celsius a Farenheit
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), SelectTemp, LOW);

  // Inicializa la LCD y el MLX90614
  lcd.begin();
  mlx.begin();

  // Se crea el cáracter personalizado
  lcd.createChar(Circle, customChar);

  // Enciende la retroiluminación e imprime un mensaje.
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("MLX90614 Sensor");
  delay(800);
  lcd.setCursor(0, 1);
  lcd.print("Measuring...");
  delay(800);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("--- Celsius ---");
  delay(800);
}

void loop() {
  if (select == LOW) {
    if (measuring == HIGH) {
      lcd.setCursor(0, 0);
      lcd.print("Ambient: "); lcd.print(mlx.readAmbientTempC());
      lcd.write(Circle); lcd.print("C");
      lcd.setCursor(0, 1);
      lcd.print("Object:  "); lcd.print(mlx.readObjectTempC());
      lcd.write(Circle); lcd.print("C");
      delay(250);

    } else if (measuring == LOW) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("--- Celsius ---");
      delay(800);
      lcd.clear();
      measuring = HIGH;
    }
  } else if (select == HIGH) {
    if (measuring == HIGH) {
      lcd.setCursor(0, 0);
      lcd.print("Ambient: "); lcd.print(mlx.readAmbientTempF());
      lcd.write(Circle); lcd.print("F");
      lcd.setCursor(0, 1);
      lcd.print("Object:  "); lcd.print(mlx.readObjectTempF());
      lcd.write(Circle); lcd.print("F");
      delay(250);

    } else if (measuring == LOW) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("-- Fahrenheit --");
      delay(800);
      lcd.clear();
      measuring = HIGH;
    }
  }
}


// Rutina de interrupción para cambiar el antirebote y cambio de estado de las escalas
void SelectTemp() {
  if ((long)(micros() - last_micros) >= debouncing_time * 1000) {
    last_micros = micros();
    select = !select;
    measuring = LOW;
  }
}

