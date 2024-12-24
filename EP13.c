//  Просветов Вадим бИЦ-241
/*  Программадля оценки уровня CO2, преобразует введённые буквы и цифры с плавающей точкой с помощью функции process,
    далее программа выводит меню для работы с пользователем, учитывая его ответ, продолжает работу,
    каждое log_num значение программа записывает в лог файл с помощью функции log_save,
    далее программа выводит графическое представление CO2 с помощью функции print_co2,
    далее выводится содержимое лог файла, функцией print_log, а в самом конце отсортированные функцией sort_log
    значения, записанные в лог файл
*/

#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>

#define max_size 1000        // Максимальный размер массивов
#define green0 400.0         // Пороговое значение для зеленого уровня CO2
#define red0 1000.0          // Пороговое значение для красного уровня CO2
#define crit0 10000.0        // Критическое значение уровня CO2
#define log_num0 10          // Шаг записи в лог-файл

double process(char input[]);                                       // Преобразует строку ввода в число с плавающей точкой
int log_save(const char* filename, double co2_level);               // Записывает значение уровня CO2 в лог-файл
int print_co2(double co2_level, double green, double red);          // Выводит графическое представление уровня CO2
int print_log(const char* filename);                                // Выводит содержимое лог-файла
int sort_log(const char* filename);                                 // Сортирует и выводит данные из лог-файла

// Объявление глобальных переменных для настройки параметров
double green = green0;
double red = red0;
double crit = crit0;
int log_num = log_num0;

int main()
{
    setlocale(LC_CTYPE, "RUS");

    puts("Уважаемый пользователь, данная программа предназначена для считывания и оценки уровня CO2 в воздухе,");
    puts("до тех пор пока на ввод не поступит критическое значение.\n");

    int choice;                                     // Переменная для хранения выбора пользователя
    char input[max_size][max_size];                 // Массив для хранения строк ввода
    double co2_level[max_size];                     // Массив для хранения значений уровня CO2
    int count = 0;                                  // Счетчик введенных значений
    const char* log_name = "co2_log.txt";           // Имя лог файла

    do {
        printf("\nМеню:\n");
        printf("1. Ввести значения и получить таблицы с результатами\n");
        printf("2. Изменить значение максимума зелёного уровня (текущее: %.1f)\n", green);
        printf("3. Изменить значение минимума красного уровня (текущее: %.1f)\n", red);
        printf("4. Изменить значение критического значения (текущее: %.1f)\n", crit);
        printf("5. Изменить значение шага записываемого в лог файл числа (текущее: %d)\n", log_num);
        printf("6. Вывести содержимое лог-файла\n");
        printf("7. Вывести отсортированное содержимое лог-файла\n");
        printf("8. Выход\n");
        printf("Введите желаемое значение: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            // Очистка лог файла перед вводом данных
            FILE* log_file = fopen(log_name, "w");
            if (log_file == NULL)
            {
                printf("Ошибка открытия файла для очистки.\n");
            }
            else
            {
                fclose(log_file);
            }
            count = 0;          // Сброс счётчика для нового ввода
            // Цикл ввода значений до достижения критического уровня
            while (count < crit)
            {
                printf("Введите значение %d: ", count + 1);
                scanf("%s", input[count]);

                co2_level[count] = process(input[count]);       // Преобразует строку в число

                if (co2_level[count] >= crit)
                {
                    printf("\n\tЗНАЧЕНИЕ БОЛЬШЕ ИЛИ РАВНОЕ %.1f\n", crit);
                    printf("\tКРИТИЧЕСКИЙ УРОВЕНЬ!!!\n");
                    printf("\tЗАПИСЬ ОСТАНОВЛЕНА\n");
                    break;
                }

                // Запись в лог-файл каждые log_num значений
                if ((count + 1) % log_num == 0)
                {
                    log_save(log_name, co2_level[count]);
                }
                count++;
            }

            // Вывод графического представления для каждого значения
            for (int i = 0; i < count; i++)
            {
                printf("\n%d Составленное число CO2: %.2f\n", i + 1, co2_level[i]);
                print_co2(co2_level[i], green, red);
            }

            break;
        }
        case 2:
        {
            printf("Введите новое значение максимума зелёного уровня: ");
            scanf("%lf", &green);
            break;
        }
        case 3:
        {
            printf("Введите новое значение минимума красного уровня: ");
            scanf("%lf", &red);
            break;
        }
        case 4:
        {
            printf("Введите новое значение критического значения: ");
            scanf("%lf", &crit);
            break;
        }
        case 5:
        {
            printf("Введите новое значение шага записываемого в лог файл числа: ");
            scanf("%d", &log_num);
            break;
        }
        case 6:
        {
            printf("\nСодержимое лог файла:\n");
            print_log(log_name);
            break;
        }
        case 7:
        {
            printf("\nОтсортированное содержимое лог файла:\n");
            sort_log(log_name);
            break;
        }
        case 8:
        {
            break;      // Выход из цикла while
        }
        default:
        {
            printf("Неверный выбор, попробуйте еще раз.\n");
            break;
        }
        }
    } while (choice != 8);

    printf("Завершение программы.\n");
    return 0;
}

// Функция для преобразования строки ввода в число с плавающей точкой
// Вход: строка input - строка, содержащая число с плавающей точкой
// Выход: double - преобразованное число с плавающей точкой
double process(char input[])
{
    double co2_level = 0.0;
    int df = 0;                                     // Флаг, указывающий на наличие десятичной точки
    double dp = 1.0;                                // Делитель для дробной части

    for (int i = 0; input[i] != '\0'; i++)          //Перебор символов в строке до символа конца строки
    {
        if (input[i] >= '0' && input[i] <= '9')
        {
            co2_level = co2_level * 10 + (input[i] - '0');
            if (df)
            {
                dp *= 10.0;
            }
        }
        else if (input[i] == '.' || input[i] == ',')
        {
            df = 1;             // Устанавливаем флаг, если встретили десятичную точку
        }
    }

    if (df)
    {
        co2_level /= dp;
    }
    return co2_level;
}

// Функция для записи значения уровня CO2 в лог-файл
// Вход: filename - имя файла, co2_level - значение уровня CO2
// Выход: 0 при успехе, 1 при ошибке
int log_save(const char* filename, double co2_level)
{
    FILE* log_file = fopen(filename, "a");      // Открываем файл в режиме добавления
    if (log_file == NULL)
    {
        perror("Ошибка открытия файла для записи");
        return 1;
    }
    fprintf(log_file, "%.2f\n", co2_level);
    fclose(log_file);
    return 0;
}

// Функция для вывода графического представления уровня CO2
// Вход: co2_level - значение уровня CO2, green - порог зеленого, red - порог красного
// Выход: 0
int print_co2(double co2_level, double green, double red)
{
    printf("Уровень CO2: ");
    if (co2_level < green)
    {
        printf("Зеленый\n");
        printf(" __________________ \n");
        printf("|                  |\n");
        printf("|                  |\n");
        printf("|__________________|\n");
    }
    else if (co2_level >= green && co2_level < red)
    {
        printf("Желтый\n");
        printf(" __________________ \n");
        printf("|  o  o  o  o  o  o|\n");
        printf("| o  o  o  o  o  o |\n");
        printf("|o__o__o__o__o__o__|\n");
    }
    else
    {
        printf("Красный\n");
        printf(" __________________ \n");
        printf("|/ / / / / / / / / |\n");
        printf("| / / / / / / / / /|\n");
        printf("|/_/_/_/_/_/_/_/_/_|\n");
    }
    return 0;
}

// Функция для вывода содержимого лог-файла
// Вход: filename - имя файла
// Выход: 0 при успехе, 1 при ошибке
int print_log(const char* filename)
{
    FILE* log_file = fopen(filename, "r");
    if (log_file == NULL)
    {
        perror("Ошибка открытия файла для чтения");
        return 1;               // Выход из функции при ошибке
    }

    char log_line[max_size];
    while (fgets(log_line, sizeof(log_line), log_file) != NULL)
    {
        printf("%s", log_line);
    }

    fclose(log_file);
    return 0;
}

//  Сортировка пузырьком и вывод отсортированных данных
//  Вход: filename - имя файла
//  Выход: 0 при успехе, 1 при ошибке
int sort_log(const char* filename)
{
    FILE* log_file = fopen(filename, "r");
    if (log_file == NULL)
    {
        printf("Ошибка открытия файла для чтения.\n");
        return 1;
    }

    double values[max_size];
    int count = 0;
    double val;

    while (fscanf(log_file, "%lf", &val) == 1 && count < max_size)
    {
        values[count++] = val;
    }
    fclose(log_file);

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (values[j] > values[j + 1])
            {
                double temp = values[j];
                values[j] = values[j + 1];
                values[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < count; i++)
    {
        printf("%.2f\n", values[i]);
    }
    printf("\n");
    return 0;
}


