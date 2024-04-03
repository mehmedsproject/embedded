  int butonPin = 8;
  int ledPin =  13;
boolean lastButton = LOW;
boolean currentButton = LOW;
boolean ledOn = false;

boolean arkiKapat(boolean son)
{
  boolean current = digitalRead(butonPin);
  if(son != current)
  { 
    delay(5);
    current = digitalRead(butonPin);
  }
  return current;  
}

void setup() {
  // put your setup code here, to run once:
  pinMode(butonPin,INPUT);
  pinMode(ledPin,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  currentButton = arkiKapat(lastButton);
  if (currentButton == HIGH && lastButton == LOW)
  {
    ledOn = !ledOn;
    }
    lastButton = currentButton;
      digitalWrite(ledPin,ledOn);
}
