//  ��������� ����� ���-241
/*  ������������ ������ ������ CO2, ����������� �������� ����� � ����� � ��������� ������ � ������� ������� process,
    ����� ��������� ������� ���� ��� ������ � �������������, �������� ��� �����, ���������� ������,
    ������ log_num �������� ��������� ���������� � ��� ���� � ������� ������� log_save,
    ����� ��������� ������� ����������� ������������� CO2 � ������� ������� print_co2,
    ����� ��������� ���������� ��� �����, �������� print_log, � � ����� ����� ��������������� �������� sort_log
    ��������, ���������� � ��� ����
*/

#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>

#define max_size 1000        // ������������ ������ ��������
#define green0 400.0         // ��������� �������� ��� �������� ������ CO2
#define red0 1000.0          // ��������� �������� ��� �������� ������ CO2
#define crit0 10000.0        // ����������� �������� ������ CO2
#define log_num0 10          // ��� ������ � ���-����

double process(char input[]);                                       // ����������� ������ ����� � ����� � ��������� ������
int log_save(const char* filename, double co2_level);               // ���������� �������� ������ CO2 � ���-����
int print_co2(double co2_level, double green, double red);          // ������� ����������� ������������� ������ CO2
int print_log(const char* filename);                                // ������� ���������� ���-�����
int sort_log(const char* filename);                                 // ��������� � ������� ������ �� ���-�����

// ���������� ���������� ���������� ��� ��������� ����������
double green = green0;
double red = red0;
double crit = crit0;
int log_num = log_num0;

int main()
{
    setlocale(LC_CTYPE, "RUS");

    puts("��������� ������������, ������ ��������� ������������� ��� ���������� � ������ ������ CO2 � �������,");
    puts("�� ��� ��� ���� �� ���� �� �������� ����������� ��������.\n");

    int choice;                                     // ���������� ��� �������� ������ ������������
    char input[max_size][max_size];                 // ������ ��� �������� ����� �����
    double co2_level[max_size];                     // ������ ��� �������� �������� ������ CO2
    int count = 0;                                  // ������� ��������� ��������
    const char* log_name = "co2_log.txt";           // ��� ��� �����

    do {
        printf("\n����:\n");
        printf("1. ������ �������� � �������� ������� � ������������\n");
        printf("2. �������� �������� ��������� ������� ������ (�������: %.1f)\n", green);
        printf("3. �������� �������� �������� �������� ������ (�������: %.1f)\n", red);
        printf("4. �������� �������� ������������ �������� (�������: %.1f)\n", crit);
        printf("5. �������� �������� ���� ������������� � ��� ���� ����� (�������: %d)\n", log_num);
        printf("6. ������� ���������� ���-�����\n");
        printf("7. ������� ��������������� ���������� ���-�����\n");
        printf("8. �����\n");
        printf("������� �������� ��������: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            // ������� ��� ����� ����� ������ ������
            FILE* log_file = fopen(log_name, "w");
            if (log_file == NULL)
            {
                printf("������ �������� ����� ��� �������.\n");
            }
            else
            {
                fclose(log_file);
            }
            count = 0;          // ����� �������� ��� ������ �����
            // ���� ����� �������� �� ���������� ������������ ������
            while (count < crit)
            {
                printf("������� �������� %d: ", count + 1);
                scanf("%s", input[count]);

                co2_level[count] = process(input[count]);       // ����������� ������ � �����

                if (co2_level[count] >= crit)
                {
                    printf("\n\t�������� ������ ��� ������ %.1f\n", crit);
                    printf("\t����������� �������!!!\n");
                    printf("\t������ �����������\n");
                    break;
                }

                // ������ � ���-���� ������ log_num ��������
                if ((count + 1) % log_num == 0)
                {
                    log_save(log_name, co2_level[count]);
                }
                count++;
            }

            // ����� ������������ ������������� ��� ������� ��������
            for (int i = 0; i < count; i++)
            {
                printf("\n%d ������������ ����� CO2: %.2f\n", i + 1, co2_level[i]);
                print_co2(co2_level[i], green, red);
            }

            break;
        }
        case 2:
        {
            printf("������� ����� �������� ��������� ������� ������: ");
            scanf("%lf", &green);
            break;
        }
        case 3:
        {
            printf("������� ����� �������� �������� �������� ������: ");
            scanf("%lf", &red);
            break;
        }
        case 4:
        {
            printf("������� ����� �������� ������������ ��������: ");
            scanf("%lf", &crit);
            break;
        }
        case 5:
        {
            printf("������� ����� �������� ���� ������������� � ��� ���� �����: ");
            scanf("%d", &log_num);
            break;
        }
        case 6:
        {
            printf("\n���������� ��� �����:\n");
            print_log(log_name);
            break;
        }
        case 7:
        {
            printf("\n��������������� ���������� ��� �����:\n");
            sort_log(log_name);
            break;
        }
        case 8:
        {
            break;      // ����� �� ����� while
        }
        default:
        {
            printf("�������� �����, ���������� ��� ���.\n");
            break;
        }
        }
    } while (choice != 8);

    printf("���������� ���������.\n");
    return 0;
}

// ������� ��� �������������� ������ ����� � ����� � ��������� ������
// ����: ������ input - ������, ���������� ����� � ��������� ������
// �����: double - ��������������� ����� � ��������� ������
double process(char input[])
{
    double co2_level = 0.0;
    int df = 0;                                     // ����, ����������� �� ������� ���������� �����
    double dp = 1.0;                                // �������� ��� ������� �����

    for (int i = 0; input[i] != '\0'; i++)          //������� �������� � ������ �� ������� ����� ������
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
            df = 1;             // ������������� ����, ���� ��������� ���������� �����
        }
    }

    if (df)
    {
        co2_level /= dp;
    }
    return co2_level;
}

// ������� ��� ������ �������� ������ CO2 � ���-����
// ����: filename - ��� �����, co2_level - �������� ������ CO2
// �����: 0 ��� ������, 1 ��� ������
int log_save(const char* filename, double co2_level)
{
    FILE* log_file = fopen(filename, "a");      // ��������� ���� � ������ ����������
    if (log_file == NULL)
    {
        perror("������ �������� ����� ��� ������");
        return 1;
    }
    fprintf(log_file, "%.2f\n", co2_level);
    fclose(log_file);
    return 0;
}

// ������� ��� ������ ������������ ������������� ������ CO2
// ����: co2_level - �������� ������ CO2, green - ����� ��������, red - ����� ��������
// �����: 0
int print_co2(double co2_level, double green, double red)
{
    printf("������� CO2: ");
    if (co2_level < green)
    {
        printf("�������\n");
        printf(" __________________ \n");
        printf("|                  |\n");
        printf("|                  |\n");
        printf("|__________________|\n");
    }
    else if (co2_level >= green && co2_level < red)
    {
        printf("������\n");
        printf(" __________________ \n");
        printf("|  o  o  o  o  o  o|\n");
        printf("| o  o  o  o  o  o |\n");
        printf("|o__o__o__o__o__o__|\n");
    }
    else
    {
        printf("�������\n");
        printf(" __________________ \n");
        printf("|/ / / / / / / / / |\n");
        printf("| / / / / / / / / /|\n");
        printf("|/_/_/_/_/_/_/_/_/_|\n");
    }
    return 0;
}

// ������� ��� ������ ����������� ���-�����
// ����: filename - ��� �����
// �����: 0 ��� ������, 1 ��� ������
int print_log(const char* filename)
{
    FILE* log_file = fopen(filename, "r");
    if (log_file == NULL)
    {
        perror("������ �������� ����� ��� ������");
        return 1;               // ����� �� ������� ��� ������
    }

    char log_line[max_size];
    while (fgets(log_line, sizeof(log_line), log_file) != NULL)
    {
        printf("%s", log_line);
    }

    fclose(log_file);
    return 0;
}

//  ���������� ��������� � ����� ��������������� ������
//  ����: filename - ��� �����
//  �����: 0 ��� ������, 1 ��� ������
int sort_log(const char* filename)
{
    FILE* log_file = fopen(filename, "r");
    if (log_file == NULL)
    {
        printf("������ �������� ����� ��� ������.\n");
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


