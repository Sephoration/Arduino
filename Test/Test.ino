void setup() {
  pinMode(PC13, OUTPUT);  // 设置板载LED引脚为输出
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("STM32 Test - Serial & LED");
  Serial.println("LED should blink, serial should output");
}

void loop() {
  digitalWrite(PC13, HIGH);   // 点亮LED（STM32是低电平点亮）
  Serial.println("LED ON");
  delay(500);
  
  digitalWrite(PC13, LOW);  // 熄灭LED
  Serial.println("LED OFF"); 
  delay(500);
}