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
      Serial.print("Turning ");
      printRoomName(buffer[3]);
      if(buffer[5] == '0'){
        Serial.println(" Lights Off");
      } else if(buffer[5] == '1'){
        Serial.println(" Lights On");
      }
      else {
        Serial.println("Unkown Argument");
      }
    }
    else if(command == 'F'){
      Serial.print("Turning ");
      printRoomName(buffer[3]);
      if(buffer[5] == '0'){
        Serial.println(" Fan Off");
      } else if(buffer[5] == '1'){
        Serial.println(" Fan On");
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


void printRoomName(char roomChar){
  switch (roomChar){
    case 'L':
    Serial.print("Living Room");
    break;
    case 'K':
    Serial.print("Kitchen");
    break;
    case 'D':
    Serial.print("Den");
    break;
    case 'M':
    Serial.print("Master Bed");
    break;
    case 'G':
    Serial.print("Guest Bedroom");
    break;
    case 'H':
    Serial.print("Hallway");
    break;
    case 'B':
    Serial.print("Bathroom");
    break;
    case 'C':
    Serial.print("Carport"); // Because G for garage wsa already taken
    break;
  }    
}