#include <Arduino.h>
#include <arduino-timer.h>
#include "motors/L298N_Motor.h"
#include "tankdrive/TankDrive.h"
#include <bluefruit.h>
#include "utilities/utilities.h"
#include "math.h"
#include "motors/SparkMotor.h"

using namespace motor;

#define SPARKMOTOR_LOWER_DEADBAND_RIGHT 10000
#define SPARKMOTOR_UPPER_DEADBAND_RIGHT 14000

#define SPARKMOTOR_LOWER_DEADBAND_LEFT 10000
#define SPARKMOTOR_UPPER_DEADBAND_LEFT 14000

// leftMotor is driving to fast

// motor pins
constexpr int pwmMotorRight = 21;
constexpr int pwmMotorLeft = 22;

// led pins
constexpr int LED = 23;

double deadZone = 0.025;

uint16_t x;
uint16_t y;
uint8_t buttons;

volatile bool joystickUpdated = false;

auto timer = timer_create_default();

int ticks = 0;

HardwarePWM pwmModule(NRF_PWM0); 

SparkMotor leftMotor = SparkMotor(pwmMotorLeft, &pwmModule);
SparkMotor rightMotor = SparkMotor(pwmMotorRight, &pwmModule);

tankdrive::TankDrive driveTrain(&leftMotor, &rightMotor);

BLEClientDis clientdis;
BLEClientUart clientuart;

void analyzePackage(BLEClientUart &uart_service)
{
  while (uart_service.available() > 0)
  {
    constexpr size_t bufferSize = 8;
    uint8_t buffer[bufferSize];
    uart_service.read(buffer, bufferSize);
    x = tu_ntohs(*(uint16_t *)&buffer[0]);
    y = tu_ntohs(*(uint16_t *)&buffer[2]);
    buttons = buffer[4];
    joystickUpdated = true;
  }
}

double translateInput(double input)
{
  return pow(input, 3);
}

bool driveTask(void *)
{
  if (Bluefruit.connected())
  {
    if(joystickUpdated){
      double calculatedX = utilities::mapDouble(static_cast<double>(x), 0, 940, -0.8, 0.8);
      calculatedX = translateInput(calculatedX); 
      if (abs(calculatedX) < deadZone)
      {
        calculatedX = 0;
      }
      calculatedX = constrain(calculatedX, -0.5, 0.5);

      double calculatedY = utilities::mapDouble(static_cast<double>(y), 0, 940, -0.8, 0.8);
      calculatedY = translateInput(calculatedY);
      if (abs(calculatedY) < deadZone)
      {
        calculatedY = 0;
      }
      calculatedY = constrain(calculatedY, -1, 1);

      driveTrain.drive(calculatedY, -calculatedX);

      joystickUpdated = false;
    }
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

  // config bluetoothModule
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

  // initialize PWM pins
  pwmModule.setMaxValue(20000);
  pwmModule.setClockDiv(PWM_PRESCALER_PRESCALER_DIV_2);

  // config PWM pins
  pwmModule.addPin(pwmMotorRight);
  pwmModule.addPin(pwmMotorLeft);

  timer.every(20, driveTask);

  // initialize motors
  leftMotor.setLowerDeadBand(SPARKMOTOR_LOWER_DEADBAND_LEFT);
  leftMotor.setUpperDeadBand(SPARKMOTOR_UPPER_DEADBAND_LEFT);
  rightMotor.setLowerDeadBand(SPARKMOTOR_LOWER_DEADBAND_RIGHT);
  rightMotor.setUpperDeadBand(SPARKMOTOR_UPPER_DEADBAND_RIGHT);
  
  rightMotor.invert(true);
  leftMotor.invert(true);

  NRF_WDT -> CONFIG      = 0x01;
  NRF_WDT -> CRV         = 163839;
  NRF_WDT -> RREN        = 0x01;
  NRF_WDT -> TASKS_START = 1;
}

void loop()
{
  timer.tick();
  NRF_WDT->RR[0] = WDT_RR_RR_Reload;
}