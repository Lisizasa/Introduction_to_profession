#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main() {
    // Контрольная строка (коды символов для вашего ФИО)
    // Замените на вашу контрольную строку
    const char *control_string = "83117122100971081161151011189732691081051229711810111697";

    // Генератор случайных чисел
    srand(time(NULL));

    int total_digits = 10000;
    int middle = total_digits / 2;
    int digits_printed = 0;

    // Начало строки - выводим контрольную строку
    printf("%s", control_string);
    digits_printed += strlen(control_string);

    // Выводим случайные цифры до середины
    while (digits_printed < middle) {
        int random_digit = rand() % 10;
        printf("%d", random_digit);
        digits_printed++;
    }

    // Выводим контрольную строку в середине
    printf("%s", control_string);
    digits_printed += strlen(control_string);

    // Выводим случайные цифры до конца
    while (digits_printed < total_digits) {
        int random_digit = rand() % 10;
        printf("%d", random_digit);
        digits_printed++;
    }

    // Выводим контрольную строку в конце
    printf("%s", control_string);

    // Не выводим перевод строки, чтобы всё было в одну строку

    return 0;
}
