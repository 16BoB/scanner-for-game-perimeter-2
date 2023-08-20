#include "LCD_1602_RUS.h" // подключаем библеотеку для работы с LCD дисплеем на русском
#include "kernel.h" // подключаем библеотеку для выполнения функций асинхронно

Kernel kernel; // Объявляем глобально, чтобы использовать другие файлы

LCD_1602_RUS lcd(0x27, 16, 2); // передаем порт i2c и параметры дисплея

int perc = 100; // задаем 100% для отрисовки графика загрузки
int status_perc = 100; // задаем 100% для отрисовки числового процента загрузки
int delay_diod_on, delay_diod_off; 
int task_one; 
int task_two; 

void setup()
{
  task_one = kernel.setInterval(500000UL, blinkingDiode); // присваиваем первую асинхроную задачу мигание светодиодом (UL - задержка в милисекундах, метод) 
  task_two = kernel.setInterval(11250000UL, writeOnDisplay); // присваиваем вторую асинхроную задачу вывод нгадписей на дисплей
  pinMode(13, OUTPUT); // инициализируем 13 пин на выход
  pinMode(12, OUTPUT); // инициализируем 12 пин на выход
  lcd.init(); // инизиализируем LCD дисплей
  lcd.backlight(); // включаем подсветку 
  lcd.setCursor(2, 0); // ставим курсор (номер символа, номер строки)
  lcd.print(L"Запуск"); // пишем на экране (если пишем на русском то обязательно ставим L перед текстом)
  lcd.setCursor(1, 1);
  lcd.print(L"Сканирования");
  delay(1000);
}

void loop() {
  kernel.runNext(); // запускаем задачи по событию
}

// метод для отрисовки графика
void fillBar0(byte start_pos, byte row, byte bar_length, byte fill_percent) {
  byte infill = round((float)bar_length * fill_percent / 100);
  lcd.setCursor(start_pos, row);
  if (infill == 0) lcd.write(16);
  else lcd.write(255);
  for (int n = 1; n < bar_length - 1; n++) {
    if (n < infill) lcd.write(255);
    if (n >= infill) lcd.write(16);
  }
  if (infill == bar_length) lcd.write(255);
  else lcd.write(16);
}

// метод вывода надписей на экран + метод отрисовки графика
void writeOnDisplay() {
  lcd.setCursor(0, 0);
  lcd.print(L"Сканирую");
  lcd.setCursor(10, 0);
  lcd.print(status_perc);
  if (status_perc < 100) {
    lcd.setCursor(12, 0);
    lcd.write(16);
    if (status_perc < 10) {
      lcd.setCursor(11, 0);
      lcd.write(16);
    }
  }
  //fillBar0 принимает аргументы (столбец, строка, длина полосы, значение в % (0 - 100) )
  fillBar0(0, 1, 16, perc);
  delay(500);
  perc = perc - 6;
  status_perc = status_perc - 6;
  if (perc == -2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(L"Сканирование");
    lcd.setCursor(0, 1);
    lcd.print(L"Завершено");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(L"Темп: 100%");
    lcd.setCursor(11, 0);
    lcd.print(L"Ко: 3");
    lcd.setCursor(0, 1);
    lcd.print(L"Лов: 13");
    lcd.setCursor(8, 1);
    lcd.print(L"Адд: 22");
    delay(20000);
    lcd.clear();
  }
}

// метод для мигания светодиодами
void blinkingDiode() {
  delay_diod_on = random(0, 200);
  delay_diod_off = random(0, 200);
  digitalWrite(13, HIGH); // зажигаем светодиод по 13 пину
  delay(delay_diod_on); // ждем секунду
  digitalWrite(13, LOW); // выключаем светодиод
  delay(delay_diod_off); // ждем секунду
  digitalWrite(12, HIGH);   
  delay(delay_diod_on);          
  digitalWrite(12, LOW);   
  delay(delay_diod_off); 
}