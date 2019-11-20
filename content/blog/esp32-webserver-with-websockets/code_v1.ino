#define POT_PIN 36
#define POT_AVG_MAX 30

int potVal = 0;

int potAvg = 5;
int potVals[POT_AVG_MAX];
int potValPtr = 0;

void setup() {
  for (int i = 0; i < POT_AVG_MAX; i++) {
    potVals[i] = 0;
  }

  Serial.begin(115200);
}

void loop(){
  potVals[potValPtr] = analogRead(POT_PIN);
  int tmp = 0;
  for (int i = 0; i < potAvg; i++) {
    tmp += potVals[i];
  }
  int tmpPotVal = tmp / potAvg;

  potValPtr++;
  if (!(potValPtr < potAvg)) {
    potValPtr = 0;
  }

  if (tmpPotVal != potVal) {
    potVal = tmpPotVal;
    Serial.print("potVal: ");
    Serial.println(potVal);
  }

  delay(100);
}
