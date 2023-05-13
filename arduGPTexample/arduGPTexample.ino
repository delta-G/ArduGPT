#define BUFFER_SIZE 10

void setup() {
  Serial.begin(115200);
}



boolean receiving = false;
char c;
char buffer[BUFFER_SIZE];
int index;

void loop() {
  
  if(Serial.available()){
    char c = Serial.read();

    if (c == '<'){
      receiving = true;
      index = 0;
      buffer[index] = 0;
    }
    if(receiving){
      buffer[index] = c;
      buffer[++index] = 0;
      if (index >= BUFFER_SIZE){
        index--;  // crude, but we will fix before we build anything that could overflow
      }
      if (c == '>'){
        receiving = false;
        processBuffer(buffer);
      }
    }

  }

}

void processBuffer(char* buffer){
  int len = strlen(buffer);
  char command = 0;
  if((buffer[0] == '<') && (buffer[len-1] == '>')){
    command = buffer[1];
    if(command == 'L'){
      if(buffer[3] == '0'){
        Serial.println("Turning Lights Off");
      } else if(buffer[3] == '1'){
        Serial.println("Turning Lights On");
      }
      else {
        Serial.println("Unkown Argument");
      }
    }
    else if(command == 'F'){
      if(buffer[3] == '0'){
        Serial.println("Turning Fan Off");
      } else if(buffer[3] == '1'){
        Serial.println("Turning Fan On");
      }
      else {
        Serial.println("Unkown Argument");
      }
    }
    else if(command == 'A'){
      int temp = strtol(buffer+3, NULL, 10);
      Serial.print("Setting HVAC to ");
      Serial.println(temp);
    }
    else {
      Serial.println("Unknown Command");
    }
  }
  else {
    Serial.println("Bad Command Format");
  }
  


}
