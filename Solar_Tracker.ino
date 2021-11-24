
#include <LiquidCrystal.h>
#include <Servo.h>
LiquidCrystal lcd{12,11,5,4,3,2}; // Conexiones{rs,eneble,d4,d5,d6,d7}

float Sensibilidad = 0.066;

// Servomotores
Servo ServoH;
int SH = 0;
int SHAltMax = 160;
int SHAltMin = 20;

Servo ServoV;
int SV = 0;
int SVAltMax = 160;
int SVAltMin = 50;

//Asignación de LDRs
int LDRsi = 2; //top left LDR
int LDRsd = 1; //top right LDR
int LDRii = 3; // bottom left LDR
int LDRid = 0; // bottom right LDR

void setup ()
{
  Serial.begin(9600);
  //pinMode(A5, INPUT);
  lcd.begin(16,2); //Se inicia interfaz del lcd
  ServoH.attach(10);
  ServoH.write(0);
  ServoV.attach(9);
  ServoV.write(0);
  delay(500);
}

void loop()
{
  
  float Acs712 = 0;
  float C = 0;
  float P = 0;
  float E = 0;
  float t = 1;
  
  Acs712 = analogRead(A5) * (5 / 1023.0); // Lectura de Voltaje del sensor
  C = (Acs712 - 2.5)/Sensibilidad;        // Cálculo de la corriente
  P = C * Acs712;                         // Cálculo de la potencia
  E = P * t;                              // Cálculo de la energía aprovechada en 1 seg
  
  lcd.clear(); //Se limpia pantalla
  lcd.setCursor(0,0); //Coloca cursor en columna 0 fila 0
  lcd.print("Corriente: "); // Imprime
  lcd.setCursor(10,0); //Coloca cursor en columna 0 fila 1
  lcd.print(C); 
  lcd.setCursor(15,0); 
  lcd.print("A"); 
  lcd.setCursor(0,1);
  lcd.print("Energía: "); 
  lcd.setCursor(10,1); 
  lcd.print(E); 
  lcd.setCursor(15,1); 
  lcd.print("J"); 

  
  SH = ServoH.read();
  SV = ServoV.read();

  //capturando valores analogicos de cada LDR
  int Si = analogRead(LDRsi);
  int Sd = analogRead(LDRsd);
  int Ii = analogRead(LDRii);
  int Id = analogRead(LDRid);

  // calculando el Promedio
  int Psup = (Si + Sd) / 2; //Promedio del top LDRs
  int Pinf = (Ii + Id) / 2; //Promedio del bottom LDRs
  int Pizq = (Si + Ii) / 2; //Promedio del left LDRs
  int Pder = (Sd + Id) / 2; //Promedio del right LDRs

  if (Psup < Pinf)
  {
    ServoV.write(SV + 1);
    if (SV > SVAltMax)
    {
      SV = SVAltMax;
    }
    delay(10);
  }
  else{ if (Pinf < Psup)
  {
    ServoV.write(SV - 1);
    if (SV < SVAltMin)
    {
      SV = SVAltMin;
    }
    delay(10);
  }
  else
  {
    ServoV.write(SV);
  }
  }
  if (Pizq > Pder)
  {
    ServoH.write(SH + 1);
    if (SH > SHAltMax)
    {
      SH = SHAltMax;
    }
    delay(10);
  }
  else{ if (Pder > Pizq)
  {
    ServoH.write(SH - 1);
    if (SH < SHAltMin)
    {
      SH = SHAltMin;
    }
    delay(10);
  }
  else
  {
    ServoH.write(SH);
  }
  }
  delay(50);

  delay(1000);
}


    
