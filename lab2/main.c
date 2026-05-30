#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <math.h>


// Определение типа данных с фиксированной точкой
typedef struct {
    unsigned long long integer;    // Целая часть
    unsigned long long fraction;   // Дробная часть (19 знаков после запятой)
} FixedPoint;

FixedPoint multiplyFixedPoint(FixedPoint a, FixedPoint b);

// Константа для работы с дробной частью
#define FRACTION_SCALE 10000000000000000000ULL

// Создание числа из целых значений
FixedPoint createFixedPoint(unsigned long long intPart, unsigned long long fracPart) {
    FixedPoint num;
    num.integer = intPart;
    num.fraction = fracPart;
    return num;
}

// Функция возведения в степень
FixedPoint power(FixedPoint base, int exp) {
    FixedPoint result = createFixedPoint(1, 0);

    for (int i = 0; i < exp; i++) {
        result = multiplyFixedPoint(result, base);
    }

    return result;
}

// Функция умножения двух FixedPoint чисел
FixedPoint multiplyFixedPoint(FixedPoint a, FixedPoint b) {
    FixedPoint result = createFixedPoint(0, 0);

    // (a_int + a_frac) * (b_int + b_frac) =
    // a_int*b_int + a_int*b_frac + a_frac*b_int + a_frac*b_frac

    // Целая * целая
    result.integer = a.integer * b.integer;
    result.fraction = 0;

    // Целая * дробная
    unsigned long long temp1 = a.integer * b.fraction;
    result.integer += temp1 / FRACTION_SCALE;
    result.fraction += temp1 % FRACTION_SCALE;

    // Дробная * целая
    unsigned long long temp2 = a.fraction * b.integer;
    result.integer += temp2 / FRACTION_SCALE;
    result.fraction += temp2 % FRACTION_SCALE;

    // Дробная * дробная (используем __uint128_t для избежания переполнения)
    __uint128_t temp3 = (__uint128_t)a.fraction * b.fraction;
    unsigned long long frac_part = (unsigned long long)(temp3 / FRACTION_SCALE);
    result.fraction += frac_part;

    // Нормализация (перенос из дробной в целую)
    unsigned long long carry = result.fraction / FRACTION_SCALE;
    result.integer += carry;
    result.fraction %= FRACTION_SCALE;

    return result;
}

// сложение
FixedPoint addFixedPoint(FixedPoint a, FixedPoint b) {
    FixedPoint result = createFixedPoint(0, 0);

    // Сложение дробных частей
    result.fraction = a.fraction + b.fraction;

    // Перенос из дробной части в целую
    unsigned long long carry = result.fraction / FRACTION_SCALE;
    result.fraction %= FRACTION_SCALE;

    // Сложение целых частей
    result.integer = a.integer + b.integer + carry;

    return result;
}

// Вычитание
FixedPoint subtractFixedPoint(FixedPoint a, FixedPoint b) {
    FixedPoint result = createFixedPoint(0, 0);

    // Вычитание дробных частей
    if (a.fraction >= b.fraction) {
        result.fraction = a.fraction - b.fraction;
        result.integer = a.integer - b.integer;
    } else {
        // Занимаем 1 из целой части
        result.fraction = FRACTION_SCALE + a.fraction - b.fraction;
        result.integer = a.integer - b.integer - 1;
    }

    return result;
}

// Деление
FixedPoint divideFixedPoint(FixedPoint a, FixedPoint b) {
    FixedPoint result = createFixedPoint(0, 0);

    // Проверка деления на ноль
    if (b.integer == 0 && b.fraction == 0) {
        printf("Ошибка: деление на ноль!\n");
        return createFixedPoint(0, 0);
    }

    long double a_double = (long double)a.integer + (long double)a.fraction / FRACTION_SCALE;
    long double b_double = (long double)b.integer + (long double)b.fraction / FRACTION_SCALE;

    long double quotient = a_double / b_double;

    // Извлекаем целую часть
    result.integer = (unsigned long long)quotient;

    // Извлекаем дробную часть
    long double frac_part = quotient - result.integer;
    result.fraction = (unsigned long long)(frac_part * FRACTION_SCALE + 0.5);

    // Нормализация на случай ошибок округления
    if (result.fraction >= FRACTION_SCALE) {
        result.integer++;
        result.fraction -= FRACTION_SCALE;
    }

    return result;
}


// Вывод числа в читаемом формате
void printFixedPoint(FixedPoint num) {
    printf("%llu", num.integer);
    printf(".%019llu", num.fraction);
}

// Вывод таблицы степеней
void printPowersOfTwo() {
    printf("|Показатель степени|Значение числа|\n\n");
    for(int i = 0; i <= 24; i++) {
        FixedPoint result = power(createFixedPoint(2, 0), i);
        printf("%d  ", i);
        printFixedPoint(result);
        printf("\n");
    }

    // Особые случаи
    printf("32 %llu\n", (unsigned long long)pow(2, 32));
    printf("48 %llu\n", (unsigned long long)pow(2, 48));
    printf("64 %llu\n", (unsigned long long)pow(2, 64));
}

// Замер времени выполнения
void measureTime(void (*func)()) {
    clock_t start = clock();
    func();
    clock_t end = clock();

    double time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\nВремя выполнения: %f секунд\n", time);
}

int main() {
    setlocale(LC_ALL, "Rus");
    measureTime(printPowersOfTwo);
    return 0;
}
