#define REMOTEXY_MODE__ESP32CORE_BLUETOOTH
#include <BluetoothSerial.h>
#include <RemoteXY.h>

// Conexion SD:
// VCC: 5V
// CS:   PIN D12
// SCK:  PIN D18
// MOSI: PIN D23
// MISO: PIN D19

int8_t X_a;
int8_t Y_a;

int x = 0;
int y = 0;

int IN1 = 2;
int IN2 = 4;
int IN3 = 16; // Input3 conectada al pin 5
int IN4 = 5;  // Input4 conectada al pin 4
int ENA = 18; // ENB conectada al pin 3 de Arduino
int ENB = 19; // ENB conectada al pin 3 de Arduino

bool sent_vert = true; // PARA EL SENTIDO ADELANTE O ATRAS
bool sent_horz = true; // PARA EL SENTIDO DE GIRO
int vel = 0;           // PARA LA VELOCIDAD DE 0 A 255
// Nombre y Contraseña:
#define REMOTEXY_BLUETOOTH_NAME "SEGO"
#define REMOTEXY_ACCESS_PASSWORD ""

#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
    {255, 2, 0, 0, 0, 32, 0, 16, 77, 1, 5, 32, 2, 17, 60, 60, 2, 26, 31, 129,
     0, 19, 80, 27, 5, 26, 79, 114, 105, 101, 110, 116, 97, 99, 105, 195, 179, 110, 0};

struct
{
  int8_t data_x;
  int8_t data_y;
  uint8_t connect_flag;

} RemoteXY;
#pragma pack(pop)

void sentido(bool sentido);
void velocidad(int vert, int horz);
void setup()
{
  RemoteXY_Init();
  Serial.begin(9600);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop()
{
  RemoteXY_Handler();
  X_a = RemoteXY.data_x;
  Y_a = RemoteXY.data_y;
  int X = static_cast<int>(X_a);
  int Y = static_cast<int>(Y_a);

  // variables:

  if (Y >= 0)
  {
    sent_vert = 1;
  }
  if (Y < 0)
  {
    sent_vert = 0;
  }
  if (X >= 0)
  {
    sent_horz = 1;
  }
  if (X < 0)
  {
    sent_horz = 1;
  }
  sentido(sent_vert);
  velocidad(Y, X);

  Serial.print("  COORDENADAS X-Y:  ");
  Serial.print("    X:  ");
  Serial.print(RemoteXY.data_x);
  Serial.print("    Y:  ");
  Serial.println(RemoteXY.data_y);

  delay(500);
}

void sentido(bool sentido)
{
  if (sentido)
  {
    // MOTOR1 ADELANTE
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    // MOTOR2 ADELANTE
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
  else
  {
    // MOTOR1 ATRAS
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    // MOTOR2 ATRAS
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
}

void velocidad(int vert, int horz) // vert: sentido vertical de las ruedas, horz: sentido horizontal de las ruedas
{
  // PARA QUE SE MUEVA HACIA ATRAS O ADELANTE
  if (abs(vert) <= 5)
  {
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
  }
  if (abs(vert) > 5)
  {
    analogWrite(ENA, 55);
    analogWrite(ENB, 55);
  }
  if (abs(vert) > 20)
  {
    analogWrite(ENA, 100);
    analogWrite(ENB, 100);
  }
  if (abs(vert) > 50)
  {
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
  }
  if (abs(vert) > 80)
  {
    analogWrite(ENA, 200);
    analogWrite(ENB, 200);
  }
  if (abs(vert) == 100)
  {
    analogWrite(ENA, 250);
    analogWrite(ENB, 250);
  }
  // PARA QUE SE MUEVA A LA DERECHA
  if ((horz) > 0)
  {
    analogWrite(ENA, 10);
    analogWrite(ENB, 30);
  }
  if ((horz) > 20)
  {
    analogWrite(ENA, 30);
    analogWrite(ENB, 80);
  }
  if ((horz) > 50)
  {
    analogWrite(ENA, 50);
    analogWrite(ENB, 100);
  }
  if ((horz) > 80)
  {
    analogWrite(ENA, 80);
    analogWrite(ENB, 200);
  }
  if ((horz) == 100)
  {
    analogWrite(ENA, 0);
    analogWrite(ENB, 250);
  }
  // PARA QUE SE MUEVA HACIA LA IZQUIERDA
  if ((horz) < 0)
  {
    analogWrite(ENA, 30);
    analogWrite(ENB, 10);
  }
  if ((horz) < -20)
  {
    analogWrite(ENA, 80);
    analogWrite(ENB, 30);
  }
  if ((horz) < -50)
  {
    analogWrite(ENA, 100);
    analogWrite(ENB, 50);
  }
  if ((horz) < -80)
  {
    analogWrite(ENA, 200);
    analogWrite(ENB, 80);
  }
  if ((horz) == -100)
  {
    analogWrite(ENA, 250);
    analogWrite(ENB, 0);
  }
}