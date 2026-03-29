#include <Arduino.h>
#include <string.h>

#define MAX_FILES 10         
#define NAME_LEN 12         
#define CONTENT_LEN 32      
#define PATH_LEN 16         
#define DMESG_LINES 6
#define DMESG_LEN 40

typedef struct {
  char name[NAME_LEN];
  char content[CONTENT_LEN];
  char parentDir[PATH_LEN];
  int isDirectory;
  int active;
} RAMFile;

typedef struct {
  unsigned long timestamp;
  char message[DMESG_LEN];
} DmesgEntry;

RAMFile fs[MAX_FILES];
char currentPath[PATH_LEN] = "/";
char inputBuffer[32] = "";
int inputLen = 0;
DmesgEntry dmesg[DMESG_LINES];
int dmesgIndex = 0;

int freeMemory() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void(* resetFunc) (void) = 0;

void addDmesg(const char* msg) {
  if (dmesgIndex >= DMESG_LINES) dmesgIndex = 0;
  dmesg[dmesgIndex].timestamp = millis() / 1000;
  strncpy(dmesg[dmesgIndex].message, msg, DMESG_LEN - 1);
  dmesg[dmesgIndex].message[DMESG_LEN - 1] = '\0';
  dmesgIndex++;
}

void initFS() {
  const char* dirs[] = {"home", "dev"};
  int d, i;
  
  for (d = 0; d < 2; d++) {
    for (i = 0; i < MAX_FILES; i++) {
      if (!fs[i].active) {
        strcpy(fs[i].name, dirs[d]);
        strcpy(fs[i].parentDir, "/");
        fs[i].isDirectory = 1;
        fs[i].active = 1;
        break;
      }
    }
  }
  
  char devPath[PATH_LEN] = "/dev/";
  const char* pins[] = {"pin2", "pin3", "pin4"};
  for (d = 0; d < 3; d++) {
    for (i = 0; i < MAX_FILES; i++) {
      if (!fs[i].active) {
        strcpy(fs[i].name, pins[d]);
        strcpy(fs[i].parentDir, devPath);
        fs[i].isDirectory = 0;
        fs[i].content[0] = '\0';
        fs[i].active = 1;
        break;
      }
    }
  }
  
  addDmesg("Kernel initialized");
  addDmesg("Filesystem mounted");
  addDmesg("Ready for commands");
}

void setup() {
  Serial.begin(115200);
  initFS(); 
  delay(1000);
  Serial.println(F("\n--- KernelUNO v1.0 ---"));
  Serial.println(F("Type 'help' for commands"));
  printPrompt();
}

void printPrompt() {
  Serial.print(F("root@arduino:"));
  Serial.print(currentPath);
  Serial.print(F("# "));
}

void loop() {
  if (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\r' || c == '\n') {
      if (inputLen > 0) {
        inputBuffer[inputLen] = '\0';
        Serial.println();
        executeCommand(inputBuffer);
        inputLen = 0;
        memset(inputBuffer, 0, 32);
        printPrompt();
      }
    } 
    else if (c == 8 || c == 127) {
      if (inputLen > 0) {
        inputLen--;
        inputBuffer[inputLen] = '\0';
        Serial.print(F("\b \b")); 
      }
    } 
    else if (inputLen < 31) {
      Serial.print(c);
      inputBuffer[inputLen] = c;
      inputLen++;
    }
  }
}

int indexOf(const char* str, const char* substr) {
  int i, j, slen = strlen(str), sublen = strlen(substr);
  for (i = 0; i <= slen - sublen; i++) {
    int match = 1;
    for (j = 0; j < sublen; j++) {
      if (str[i + j] != substr[j]) {
        match = 0;
        break;
      }
    }
    if (match) return i;
  }
  return -1;
}

int atoi_safe(const char* str) {
  int num = 0;
  while (*str >= '0' && *str <= '9') {
    num = num * 10 + (*str - '0');
    str++;
  }
  return num;
}

void toLowercase(char* str) {
  int i;
  for (i = 0; str[i] != '\0'; i++) {
    if (str[i] >= 'A' && str[i] <= 'Z') {
      str[i] = str[i] - 'A' + 'a';
    }
  }
}

int safeConcatPath(char* dest, const char* add) {
  int destLen = strlen(dest);
  int addLen = strlen(add);
  
  if (destLen + addLen + 1 >= PATH_LEN) {
    return 0;
  }
  
  strcat(dest, add);
  strcat(dest, "/");
  return 1;
}

void executeCommand(char* line) {
  char cmd[32] = "";
  char args[32] = "";
  int space1 = -1;
  int i, sp, pin, count;
  char buf[40];
  
  strcpy(cmd, line);
  
  for (i = 0; cmd[i] != '\0'; i++) {
    if (cmd[i] == ' ') {
      space1 = i;
      strcpy(args, cmd + i + 1);
      cmd[i] = '\0';
      break;
    }
  }
  
  toLowercase(cmd);
  
  if (strcmp(cmd, "pinmode") == 0) {
    sp = indexOf(args, " ");
    if (sp == -1) { Serial.println(F("Usage: pinmode [pin] [in/out]")); return; }
    pin = atoi_safe(args);
    char mode[8] = "";
    strcpy(mode, args + sp + 1);
    toLowercase(mode);
    if (strcmp(mode, "out") == 0) { 
      pinMode(pin, OUTPUT); 
      sprintf(buf, "Pin %d set to OUTPUT", pin);
      addDmesg(buf);
      Serial.println(F("Pin set to OUTPUT")); 
    }
    else if (strcmp(mode, "in") == 0) { 
      pinMode(pin, INPUT_PULLUP); 
      sprintf(buf, "Pin %d set to INPUT", pin);
      addDmesg(buf);
      Serial.println(F("Pin set to INPUT_PULLUP")); 
    }
  }
  else if (strcmp(cmd, "write") == 0) {
    sp = indexOf(args, " ");
    if (sp == -1) { Serial.println(F("Usage: write [pin] [high/low]")); return; }
    pin = atoi_safe(args);
    char val[8] = "";
    strcpy(val, args + sp + 1);
    toLowercase(val);
    digitalWrite(pin, (strcmp(val, "high") == 0 ? HIGH : LOW));
    sprintf(buf, "Pin %d wrote %s", pin, strcmp(val, "high") == 0 ? "HIGH" : "LOW");
    addDmesg(buf);
    Serial.println(F("Write OK."));
  }
  else if (strcmp(cmd, "read") == 0) {
    pin = atoi_safe(args);
    int value = digitalRead(pin);
    Serial.print(F("Pin ")); Serial.print(pin);
    Serial.print(F(" value: ")); Serial.println(value);
    sprintf(buf, "Pin %d read: %d", pin, value);
    addDmesg(buf);
  }
  else if (strcmp(cmd, "gpio") == 0) {
    sp = indexOf(args, " ");
    if (sp == -1) { 
      Serial.println(F("Usage: gpio [pin] [on/off] OR gpio vixa [count]"));
      return; 
    }
    char pinStr[8] = "";
    strncpy(pinStr, args, sp);
    pinStr[sp] = '\0';
    char action[8] = "";
    strcpy(action, args + sp + 1);
    toLowercase(action);
    
    if (strcmp(pinStr, "vixa") == 0) {
      count = atoi_safe(action);
      if (count <= 0) count = 10;
      addDmesg("LED disco mode activated");
      Serial.println(F("LED DISCO MODE!"));
      
      int cycle, p;
      for (cycle = 0; cycle < count; cycle++) {
        for (p = 2; p <= 13; p++) {
          pinMode(p, OUTPUT);
          digitalWrite(p, HIGH);
          delay(50);
          digitalWrite(p, LOW);
        }
      }
      Serial.println(F("Disco finished!"));
      addDmesg("Disco complete");
    } else {
      pin = atoi_safe(pinStr);
      if (strcmp(action, "on") == 0) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
        sprintf(buf, "GPIO %d ON", pin);
        addDmesg(buf);
        Serial.print(F("GPIO ")); Serial.print(pin); Serial.println(F(" ON"));
      }
      else if (strcmp(action, "off") == 0) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
        sprintf(buf, "GPIO %d OFF", pin);
        addDmesg(buf);
        Serial.print(F("GPIO ")); Serial.print(pin); Serial.println(F(" OFF"));
      }
      else if (strcmp(action, "toggle") == 0) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, !digitalRead(pin));
        sprintf(buf, "GPIO %d toggled", pin);
        addDmesg(buf);
        Serial.print(F("GPIO ")); Serial.print(pin); Serial.println(F(" toggled"));
      }
    }
  }
  else if (strcmp(cmd, "ls") == 0) {
    int empty = 1, j;
    for (j = 0; j < MAX_FILES; j++) {
      if (fs[j].active && strcmp(fs[j].parentDir, currentPath) == 0) {
        Serial.print(fs[j].name);
        if (fs[j].isDirectory) Serial.print(F("/"));
        Serial.print(F("  "));
        empty = 0;
      }
    }
    if (empty) Serial.print(F("(empty)"));
    Serial.println();
  }
  else if (strcmp(cmd, "mkdir") == 0 || strcmp(cmd, "touch") == 0) {
    int foundSlot = -1, j;
    for (j = 0; j < MAX_FILES; j++) { 
      if (!fs[j].active) { foundSlot = j; break; } 
    }
    if (foundSlot == -1) {
      Serial.println(F("No space."));
      return;
    }
    
    strcpy(fs[foundSlot].name, args);
    strcpy(fs[foundSlot].parentDir, currentPath);
    fs[foundSlot].isDirectory = (strcmp(cmd, "mkdir") == 0);
    fs[foundSlot].content[0] = '\0';
    fs[foundSlot].active = 1;
    Serial.println(F("OK."));
  }
  else if (strcmp(cmd, "cd") == 0) {
    if (strcmp(args, "..") == 0 || strcmp(args, "/") == 0) {
      strcpy(currentPath, "/");
    }
    else {
      int j, found = 0;
      for (j = 0; j < MAX_FILES; j++) {
        if (fs[j].active && fs[j].isDirectory && strcmp(args, fs[j].name) == 0 && strcmp(fs[j].parentDir, currentPath) == 0) {
          if (!safeConcatPath(currentPath, fs[j].name)) {
            strcpy(currentPath, "/");
            Serial.println(F("Path too long."));
            return;
          }
          found = 1;
          break;
        }
      }
      if (!found) Serial.println(F("No dir."));
    }
  }
  else if (strcmp(cmd, "pwd") == 0) {
    Serial.println(currentPath);
  }
  else if (strcmp(cmd, "echo") == 0) {
    int arrow = indexOf(args, " > ");
    if (arrow != -1) {
      char text[40] = "";
      strncpy(text, args, arrow);
      text[arrow] = '\0';
      char filename[12] = "";
      strcpy(filename, args + arrow + 3);
      
      int j, found = 0;
      for (j = 0; j < MAX_FILES; j++) {
        if (fs[j].active && !fs[j].isDirectory && strcmp(filename, fs[j].name) == 0 && strcmp(fs[j].parentDir, currentPath) == 0) {
          strcpy(fs[j].content, text);
          Serial.println(F("Saved."));
          found = 1;
          break;
        }
      }
      if (!found) Serial.println(F("File not found."));
    }
    else {
      Serial.println(args);
    }
  }
  else if (strcmp(cmd, "cat") == 0) {
    int j, found = 0;
    for (j = 0; j < MAX_FILES; j++) {
      if (fs[j].active && !fs[j].isDirectory && strcmp(args, fs[j].name) == 0 && strcmp(fs[j].parentDir, currentPath) == 0) {
        Serial.println(fs[j].content);
        found = 1;
        break;
      }
    }
    if (!found) Serial.println(F("File not found."));
  }
  else if (strcmp(cmd, "info") == 0) {
    int j, found = 0;
    for (j = 0; j < MAX_FILES; j++) {
      if (fs[j].active && strcmp(args, fs[j].name) == 0 && strcmp(fs[j].parentDir, currentPath) == 0) {
        Serial.print(F("Name: ")); Serial.println(fs[j].name);
        Serial.print(F("Type: ")); Serial.println(fs[j].isDirectory ? F("Directory") : F("File"));
        Serial.print(F("Size: ")); Serial.print(strlen(fs[j].content)); Serial.println(F(" bytes"));
        found = 1;
        break;
      }
    }
    if (!found) Serial.println(F("Not found."));
  }
  else if (strcmp(cmd, "rm") == 0) {
    int j, found = 0;
    for (j = 0; j < MAX_FILES; j++) {
      if (fs[j].active && strcmp(args, fs[j].name) == 0 && strcmp(fs[j].parentDir, currentPath) == 0) {
        if (fs[j].isDirectory) {
          // Rekursywnie usuń wszystko wewnątrz katalogu
          char dirPath[PATH_LEN];
          strcpy(dirPath, currentPath);
          strcat(dirPath, args);
          strcat(dirPath, "/");
          
          int k;
          for (k = 0; k < MAX_FILES; k++) {
            if (fs[k].active && strncmp(fs[k].parentDir, dirPath, strlen(dirPath)) == 0) {
              fs[k].active = 0;
            }
          }
        }
        fs[j].active = 0;
        Serial.println(F("Removed."));
        found = 1;
        break;
      }
    }
    if (!found) Serial.println(F("Not found."));
  }
  else if (strcmp(cmd, "dmesg") == 0) {
    Serial.println(F("=== KERNEL MESSAGES ==="));
    int j;
    for (j = 0; j < DMESG_LINES; j++) {
      if (dmesg[j].message[0] != '\0') {
        Serial.print(F("["));
        Serial.print(dmesg[j].timestamp);
        Serial.print(F("] "));
        Serial.println(dmesg[j].message);
      }
    }
  }
  else if (strcmp(cmd, "uptime") == 0) {
    unsigned long s = millis()/1000;
    unsigned long h = s / 3600;
    unsigned long m = (s % 3600) / 60;
    unsigned long sec = s % 60;
    Serial.print(F("up "));
    Serial.print(h); Serial.print(F("h "));
    Serial.print(m); Serial.print(F("m "));
    Serial.print(sec); Serial.println(F("s"));
    addDmesg("uptime command");
  }
  else if (strcmp(cmd, "df") == 0 || strcmp(cmd, "free") == 0) {
    Serial.print(F("Free RAM: "));
    Serial.print(freeMemory());
    Serial.println(F(" bytes"));
  }
  else if (strcmp(cmd, "whoami") == 0) {
    Serial.println(F("root"));
  }
  else if (strcmp(cmd, "uname") == 0) {
    Serial.println(F("KernelUNO v1.0"));
    Serial.print(F("Kernel: Arduino "));
    Serial.println(F("AVR"));
    Serial.print(F("Hardware: "));
    Serial.println(F("Arduino UNO"));
    Serial.print(F("RAM: "));
    Serial.print(freeMemory());
    Serial.println(F(" bytes free"));
  }
  else if (strcmp(cmd, "reboot") == 0) {
    Serial.println(F("Rebooting..."));
    addDmesg("System reboot");
    delay(500);
    resetFunc();
  }
  else if (strcmp(cmd, "clear") == 0) {
    int j;
    for(j = 0; j < 30; j++) Serial.println();
  }
  else if (strcmp(cmd, "help") == 0) {
    Serial.println(F("Commands: ls, cd, pwd, mkdir, touch, cat, echo, rm, info, pinmode, write, read, gpio, uptime, uname, dmesg, df, free, whoami, clear, reboot, help"));
    Serial.println(F("GPIO: gpio [pin] on/off/toggle OR gpio vixa [count]"));
  }
  else {
    Serial.println(F("Unknown command."));
  }
}
