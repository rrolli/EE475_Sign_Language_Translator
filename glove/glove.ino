/******************************************************************************
Flex_Sensor_Example.ino
Example sketch for SparkFun's flex sensors
  (https://www.sparkfun.com/products/10264)
Jim Lindblom @ SparkFun Electronics
April 28, 2016

Create a voltage divider circuit combining a flex sensor with a 47k resistor.
- The resistor should connect from A0 to GND.
- The flex sensor should connect from A0 to 3.3V
As the resistance of the flex sensor increases (meaning it's being bent), the
voltage at A0 should decrease.

Development environment specifics:
Arduino 1.6.7
******************************************************************************/
const int FLEX_PIN1 = A0; // Pin connected to voltage divider output
const int FLEX_PIN2 = A1; // Pin connected to voltage divider output
const int FLEX_PIN3 = A2; // Pin connected to voltage divider output
const int FLEX_PIN4 = A3; // Pin connected to voltage divider output
const int FLEX_PIN5 = A4; // Pin connected to voltage divider output


// Measure the voltage at 5V and the actual resistance of your
// 47k resistor, and enter them below:
const float VCC = 4.98; // Measured voltage of Ardunio 5V line
const float R_DIV = 47500.0; // Measured resistance of 3.3k resistor

// Upload the code, then try to adjust these values to more
// accurately calculate bend degree.
const float STRAIGHT_RESISTANCE [5] = {53523.38, 48154.52, 53945.73, 47407.23, 53314.31}; // Resistance when straight
const float BEND_RESISTANCE [5] = {99305.13, 139394.22, 123000.0, 126666.67, 136562.5}; // Resistance at 90 deg
  float calibrationResults[26][5];

float hiral[26][5] = {
  {156,  120,  120, 5, 6 }, //first three done A
  {34,  16,  30, 5, 6 }, //first three done B
  {3,  3,  4, 5, 6 },
  {4,  3,  4, 5, 6 },
  {5,  3,  4, 5, 6 },
  {6,  3,  4, 5, 6 },
  {7,  3,  4, 5, 6 },
  {8,  3,  4, 5, 6 },
  {9,  3,  4, 5, 6 },
  {10,  3,  4, 5, 6 },
  {11,  3,  4, 5, 6 },
  {12,  3,  4, 5, 6 },
  {13,  3,  4, 5, 6 },
  {14,  3,  4, 5, 6 },
  {15,  3,  4, 5, 6 },
  {16,  3,  4, 5, 6 },
  {17,  3,  4, 5, 6 },
  {18,  3,  4, 5, 6 },
  {19,  3,  4, 5, 6 },
  {20,  3,  4, 5, 6 },
  {21,  3,  4, 5, 6 },
  {22,  3,  4, 5, 6 },
  {23,  3,  4, 5, 6 },
  {24,  3,  4, 5, 6 },
  {25,  3,  4, 5, 6 },
  {26,  3,  4, 5, 6 }, 
};

float rebecca[26][5] = { //need to recalibrate
  {30,  112,  104, 105, 142}, //done A
  {54,  31,  32, 18, 29 }, //done B
  {23,  50,  60, 36, 45}, //done C
  {24,  30,  94, 82, 82}, //done D
  {51,  144, 116, 112, 111}, //E
  {26,  106,  32, 18, 31 }, //F
  {41,  48,  109, 122, 136}, //G
  {47,  49,  43, 97, 111}, //H
  {35,  158,  141, 90, 32}, //I
  {35,  158,  141, 90, 32}, //this is for I need to do J
  {27,  39, 39, 91, 112},//K
  {15,  42,  129, 116, 129}, //L
  {64,  140,  90, 79, 91}, //M
  {57,  113,  67, 82, 107}, //N
  {31,  97,  82, 71, 74}, //O
  {24,  27,  49, 78, 86}, //P
  {16,  32,  108, 110, 108}, //Q
  {18,  3,  4, 5, 6 },
  {19,  3,  4, 5, 6 },
  {20,  3,  4, 5, 6 },
  {21,  3,  4, 5, 6 },
  {22,  3,  4, 5, 6 },
  {23,  3,  4, 5, 6 },
  {24,  3,  4, 5, 6 },
  {25,  3,  4, 5, 6 },
  {26,  3,  4, 5, 6 }, 
};

float pinMatrix[26][5] = {
  {19.00, 97.00, 89.00, 90.00, 95.00},  //A
  {52.00, 2.00, 0.00, 0.00, 3.00}, //B
  {22.00, 17.00, 26.00, 14.00, 4.00}, //C
  {15.00, 6.00, 44.00, 44.00, 39.00},  //D
  {75.00, 75.00, 86.00, 79.00, 64.00}, //E
  {13.00, 51.00, 6.00, 2.00, 5.00}, //F
  {3.00, 23.00, 99.00, 100.00, 104.00}, //G TODO: same as Q
  {26.00, 12.00, 6.00, 64.00, 57.00}, //H TODO: same as U and V
  {39.00, 87.00, 82.00, 79.00, 21.00}, //I TODO: same as J
  {39.00, 87.00, 82.00, 79.00, 21.00}, //J TODO: same as I
  {2.00, 6.00, 34.00, 78.00, 84.00}, //K
  {-7.00, 5.00, 100.00, 79.00, 90.00}, //L
  {59.00, 93.00, 60.00, 59.00, 66.00}, //M
  {36.00, 92.00, 58.00, 64.00, 67.00}, //N
  {14.00, 52.00, 39.00, 33.00, 24.00}, //O
  {0.00, 9.00, 24.00, 65.00, 72.00}, //P
  {3.00, 23.00, 99.00, 100.00, 104.00}, //Q TODO: same as G
  {32.00, 7.00, 4.00, 87.00, 84.00}, //R
  {43.00, 113.00, 114.00, 104.00, 114.00}, //S
  {21.00, 73.00, 76.00, 70.00, 88.00}, //T
  {30.00, 4.00, 1.00, 71.00, 64.00}, //U TODO: same as H and V
  {34.00, 7.00, 0.00, 62.00, 64.00}, //V TODO: same as H and U
  {39.00, 8.00, 4.00, 2.00, 49.00}, //W
  {33.00, 42.00, 104.00, 92.00, 86.00}, //X
  {-2.00, 97.00, 61.00, 66.00, 16.00}, //Y
  {33.00, 12.00, 91.00, 85.00, 102.00}, //Z
};

float error[26][5] = {
  {15, 15,  15, 15, 15}, //A
  {15, 15,  15, 15, 15}, //B
  {15, 15,  15, 15, 15}, //C
  {15, 15,  15, 15, 15}, //D
  {15, 15,  15, 15, 15}, //E
  {15, 15,  15, 15, 15}, //F
  {15, 15,  15, 15, 15}, //G
  {15, 15,  15, 15, 15}, //H
  {15, 15,  15, 15, 15}, //I
  {15, 15,  15, 15, 15}, //J
  {15, 15,  15, 15, 15}, //K
  {15, 15,  15, 15, 15}, //L
  {15, 15,  15, 15, 15}, //M
  {15, 15,  15, 15, 15}, //N
  {15, 15,  15, 15, 15}, //O
  {15, 15,  15, 15, 15}, //P
  {15, 15,  15, 15, 15}, //Q
  {15, 15,  15, 15, 15}, //R
  {15, 15,  15, 15, 15}, //S
  {15, 15,  15, 15, 15}, //T
  {15, 15,  15, 15, 15}, //U
  {15, 15,  15, 15, 15}, //V
  {15, 15,  15, 15, 15}, //W
  {15, 15,  15, 15, 15}, //X
  {15, 15,  15, 15, 15}, //Y
  {15, 15,  15, 15, 15}, //Z
};
//float pinMatrix[26][5] = allen[26][5];

// TODO: File file = SD.open("gloveData", FILE_WRITE);


void setup() 
{
  Serial.begin(9600);
  pinMode(FLEX_PIN1, INPUT);
  Serial.begin(9600);
  pinMode(FLEX_PIN2, INPUT);
  Serial.begin(9600);
  pinMode(FLEX_PIN3, INPUT);
  Serial.begin(9600);
  pinMode(FLEX_PIN4, INPUT);
  Serial.begin(9600);
  pinMode(FLEX_PIN5, INPUT);
  
  startCalibration();
}

void loop() 
{
  Serial.print("{");
  float angle1 = readFinger(FLEX_PIN1,1);
  Serial.print(", ");
  float angle2 = readFinger(FLEX_PIN2,2);
  Serial.print(", ");
  float angle3 = readFinger(FLEX_PIN3,3);
  Serial.print(", ");
  float angle4 = readFinger(FLEX_PIN4,4);
  Serial.print(", ");
  float angle5 = readFinger(FLEX_PIN5,5);
  Serial.print("}");
  Serial.print(", ");
  
  Serial.println();
  delay(500);
  String letter = "no letter found";
  for(int i = 0; i < 26; i++){
    int currDiff = 0;
    int lastDiff = 999999;
    float currAngle1 = pinMatrix[i][0];
    float currAngle2 = pinMatrix[i][1];
    float currAngle3 = pinMatrix[i][2];
    float currAngle4 = pinMatrix[i][3];
    float currAngle5 = pinMatrix[i][4];    //5 is the range. So value can be + or - 5 from actual to be correct, we can change this value to have a higher or lower tolerance
    if(angle1<=currAngle1+error[i][0] && angle1>= currAngle1-error[i][0]){
       currDiff += abs(currAngle1-angle1);
       if(angle2<=currAngle2+error[i][1] && angle2>= currAngle2-error[i][1]){
          currDiff += abs(currAngle2-angle2);
          if(angle3<=currAngle3+error[i][2] && angle3>= currAngle3-error[i][2]){
              currDiff += abs(currAngle3-angle3);
              if(angle4<=currAngle4+error[i][3]&& angle4>= currAngle4-error[i][3]){
                  currDiff += abs(currAngle4-angle4);
                  if(angle5<=currAngle5+error[i][4] && angle5>= currAngle5-error[i][4]){
                      currDiff += abs(currAngle5-angle5);
                      char currLetter;
                        if(currDiff < lastDiff){
                          currLetter = i + 65;
                          lastDiff = currDiff;
                          Serial.println("changed letter");
                        }
                         letter = "letter = ";
                         Serial.print(letter);
                         Serial.println(currLetter);
                          
                  }
             }
          }
       }
    }
  }

  Serial.println(letter);
}

int getFingerPin (int number) {

  
}

void startCalibration() {
  Serial.print("Calibrate glove (c) or use default values (d): ");
  //delay(200);
  while (!Serial.available());    // is a character available? 
  char rx_byte = Serial.read();
  Serial.println(rx_byte);
  if(rx_byte == 'd'){
    return;
  }
  
  Serial.println("Starting calibration");
  for(int i = 0; i < 26; i++){
    char curr = i + 65;
    Serial.print("Calibrating letter ");
    Serial.println(String(curr));
    delay(5000);
    Serial.print("{");
    float val1 = 0; 
    float val2 = 0;
    float val3 = 0;
    float val4 = 0; 
    float val5 = 0;
    for(int i = 0; i < 10; i++){
    val1 += readFinger(FLEX_PIN1,1);
    Serial.print(", ");
    val2 += readFinger(FLEX_PIN2,2);
    Serial.print(", ");
    val3 += readFinger(FLEX_PIN3,3);
    Serial.print(", ");
    val4 += readFinger(FLEX_PIN4,4);
    Serial.print(", ");
    val5 += readFinger(FLEX_PIN5,5);
    Serial.println("}");
    }
    pinMatrix[i][0] = val1/5;
    pinMatrix[i][1] = val2/5;
    pinMatrix[i][2] = val3/5;
    pinMatrix[i][3] = val4/5;
    pinMatrix[i][4] = val5/5;
    delay(500);
    
  }
}

float readFinger(const int FLEX_PIN, int fingerNumber){
  //Serial.print("finger number " + String(fingerNumber));
  int flexADC1 = analogRead(FLEX_PIN);
  float flexV1 = flexADC1 * VCC / 1023.0;
  float flexR1 = R_DIV * (VCC / flexV1 - 1.0);
  //Serial.print(" Resistance: " + String(flexR1) + " ohms");

  // Use the calculated resistance to estimate the sensor's
  // bend angle:
  float angle1 = map(flexR1, STRAIGHT_RESISTANCE[fingerNumber - 1], BEND_RESISTANCE[fingerNumber - 1],
                   0, 90.0);
  //Serial.print(" Bend: " + String(angle1) + " degrees");
    //Serial.println();
  Serial.print(String(angle1));
return angle1;
}
