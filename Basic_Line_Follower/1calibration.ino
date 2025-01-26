void calibration() {
  Serial.println("in cal");
  for (int i = 0; i < numSensor; i++) {
    //setting starting value of max, min array before calibration
    maximum[i] = 0;
    minimum[i] = 1023;
  }
  digitalWrite(led, 1);
  for (int i = 0; i < 5000; i++) {
    for (int i = 0; i < numSensor; i++) {
      Mux_Addr(i);        
      rawSensor[i] = analogRd(0);
      maximum[i] = max(maximum[i], rawSensor[i]);
      minimum[i] = min(minimum[i], rawSensor[i]);
    }
    drive(80, -80);
  }
  for (int i = 0; i < 5000; i++) {
    for (int i = 0; i < numSensor; i++) {
      Mux_Addr(i);
      rawSensor[i] = analogRd(0);
      maximum[i] = max(maximum[i], rawSensor[i]);
      minimum[i] = min(minimum[i], rawSensor[i]);
    }
    drive(-80, 80);
  }
  drive(0,0);

  for (int i = 0; i < numSensor; i++) {
    mid[i] = (maximum[i] + minimum[i]) / 2;
  }
  digitalWrite(led, 0);
  printCalibration();
  saveCalibration();
}

void printCalibration() {
  //==========Simply print calibrated values==========
  for (int i = 0; i < numSensor; i++) {
    Serial.print(minimum[i]);
    Serial.print(" ");
  }
  Serial.println();
  for (int i = 0; i < numSensor; i++) {
    Serial.print(maximum[i]);
    Serial.print(" ");
  }
  Serial.println();
  Serial.println();
}

void saveCalibration() {
  for (int i = 0; i < numSensor; i++) {
    EEPROM.update(51 + i, minimum[i]);
  }
  for (int i = 0; i < numSensor; i++) {
    EEPROM.update(71 + i, maximum[i] / 4);
  }
}

void readCalibration() {
  for (int i = 0; i < numSensor; i++) {
    minimum[i] = EEPROM.read(51 + i);  //technoxian value start from 31 to 38
  }
  for (int i = 0; i < numSensor; i++) {
    maximum[i] = EEPROM.read(71 + i) * 4;  //technoxian value start from 39 to 46
  }
  printCalibration();
}
