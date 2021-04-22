#include <Arduino.h>
#include <arduino-timer.h>
#include "motors/L298N_Motor.h"
#include "tankdrive/TankDrive.h"
#include <bluefruit.h>
#include "utilities/utilities.h"
#include "math.h"

// motor pins
constexpr int leftMotorEnablePin = 2;
constexpr int leftMotorInA = 3;
constexpr int leftMotorInB = 31;
constexpr int rightMotorEnablePin = 20;
constexpr int rightMotorInA = 21;
constexpr int rightMotorInB = 22;

// led pins
constexpr int LED = 23;

double deadZone = 0.025;

constexpr int leftMotorDeadBand = 110;
constexpr int rightMotorDeadBand = 110;

uint16_t x;
uint16_t y;
uint8_t buttons;

auto timer = timer_create_default();

int ticks = 0;

motor::L298N_Motor leftMotor(leftMotorEnablePin, leftMotorInA, leftMotorInB);
motor::L298N_Motor rightMotor(rightMotorEnablePin, rightMotorInA, rightMotorInB);

tankdrive::TankDrive driveTrain(&leftMotor, &rightMotor);

BLEClientDis clientdis;
BLEClientUart clientuart;

int updateTicks(int currentTicks)
{
  if(currentTicks == 20)
  {
    return ticks++;
  }
}

void analyzePackage(BLEClientUart &uart_service)
{
  while (uart_service.available())
  {
    constexpr size_t bufferSize = 8;
    uint8_t buffer[bufferSize];
    uart_service.read(buffer, bufferSize);
    x = tu_ntohs(*(uint16_t *)&buffer[0]);
    y = tu_ntohs(*(uint16_t *)&buffer[2]);
    buttons = buffer[4];
    // Serial.printf("x: %d y: %d \n", x, y, buttons);
  }
}

double translateInput(double input)
{
  return pow(input, 3);
}

bool checkConnection(void *)
{
  if(!Bluefruit.connected())
  {    
    digitalWrite(LED, LOW);
  }
  else
  {
    digitalWrite(LED, HIGH);
  }
  return true;
}

bool driveTask(void *)
{
  if (Bluefruit.connected())
  {
    double calculatedX = utilities::mapDouble(static_cast<double>(x), 0, 940, -0.7, 0.7);
    calculatedX = translateInput(calculatedX);
    if (abs(calculatedX) < deadZone)
    {
      calculatedX = 0;
    }
    calculatedX = constrain(calculatedX, -0.5, 0.5);

    double calculatedY = utilities::mapDouble(static_cast<double>(y), 0, 940, -1.0, 1.0);
    calculatedY = translateInput(calculatedY);
    if (abs(calculatedY) < deadZone)
    {
      calculatedY = 0;
    }
    calculatedY = constrain(calculatedY, -1, 1);

    driveTrain.drive(calculatedX, calculatedY);
  }
  else
    driveTrain.drive(0, 0);
  return true;
}

void connectCallBack(uint16_t connHandle)
{
  Serial.println("Connected");
  Serial.print("Dicovering DIS ... ");
  if (clientdis.discover(connHandle))
  {
    Serial.println("Found it");
    char buffer[32 + 1];

    memset(buffer, 0, sizeof(buffer));
    if (clientdis.getManufacturer(buffer, sizeof(buffer)))
    {
      Serial.print("Manufacturer: ");
      Serial.println(buffer);
    }

    memset(buffer, 0, sizeof(buffer));
    if (clientdis.getModel(buffer, sizeof(buffer)))
    {
      Serial.print("Model: ");
      Serial.println(buffer);
    }

    Serial.println();
  }

  Serial.print("Discovering BLE Uart Service ... ");

  if (clientuart.discover(connHandle))
  {
    Serial.println("Found it");

    Serial.println("Enable TXD's notify");
    clientuart.enableTXD();

    Serial.println("Ready to receive from peripheral");
  }
  else
  {
    Serial.println("Found NONE");
    Bluefruit.disconnect(connHandle);
  }
}

void disconnectCallBack(uint16_t conn_handle, uint8_t reason)
{
  (void)conn_handle;
  (void)reason;

  Serial.println("Disconnected");
}

void scanCallBack(ble_gap_evt_adv_report_t *report)
{
  if (Bluefruit.Scanner.checkReportForService(report, clientuart))
  {
    Serial.print("BLE UART service detected. Connecting ... ");

    Bluefruit.Central.connect(report);
  }
  Bluefruit.Scanner.resume();
}

void setup()
{
  Serial.begin(9600);
  delay(3000);
  Bluefruit.begin(0, 1);
  Bluefruit.setName("Muggmaster");
  clientdis.begin();
  clientuart.begin();
  clientuart.setRxCallback(analyzePackage);
  Bluefruit.setConnLedInterval(250);
  Bluefruit.Central.setConnectCallback(connectCallBack);
  Bluefruit.Central.setDisconnectCallback(disconnectCallBack);
  Bluefruit.Scanner.setRxCallback(scanCallBack);
  Bluefruit.Scanner.restartOnDisconnect(true);
  Bluefruit.Scanner.setInterval(160, 80);
  Bluefruit.Scanner.useActiveScan(false);
  Bluefruit.Scanner.start(0);
  timer.every(20, driveTask);
  timer.every(20, checkConnection);
  leftMotor.setDeadBand(leftMotorDeadBand);
  rightMotor.setDeadBand(rightMotorDeadBand);
  pinMode(LED, OUTPUT);
}

void bluetoothLoop()
{
  if (Bluefruit.Central.connected())
  {
    // Not discovered yet
    if (clientuart.discovered())
    {
      if (Serial.available())
      {
        Serial.println("connected");
        delay(2); // delay a bit for all characters to arrive
        char str[20 + 1] = {0};

        Serial.readBytes(str, 20);

        clientuart.print(str);
      }
    }
  }
  else
    Serial.println("not connected");
}

void loop()
{
  timer.tick();
  auto currentTicks = timer.tick();
  updateTicks(currentTicks);
  Serial.printf("ticks: %d", ticks);
  bluetoothLoop();
  digitalWrite(LED, LOW);
}