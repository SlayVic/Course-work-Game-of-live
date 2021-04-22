#include <stdio.h>
#include <string.h>

typedef struct
{ //структура для даты с назв dateSt
   int year, day, month;
} dateSt;

typedef struct
{ //структура для доходов, расходов, содержащая дату, с название market
   dateSt date;
   float outlay, income;
} market;

int dateCompare(dateSt date1, dateSt date2)
{ //функция для сравнения дат
   if (date1.day == date2.day && date1.month == date2.month && date1.year == date2.year)
   {
      return 1;
   }
   return 0;
}

int main()
{
   int ch;
   int quan; //переменная для кол-во наших введенных в последующем дней
   printf("1.проверить отчетность магазина по дням");
   printf("\n2.проверить похожий день");
   printf("\n3.Любая клавиша для выхода");

   printf("\n\nВведите свой вариант :");
   scanf("%d", &ch);

   dateSt date; //хранит данные о дате
   int a = 1;   //переменная для обозначение правды или лжы для if(1-правда, выполнения, 0 -наоборот)

   switch (ch)
   {
      case 1:
         printf("\nВведите кол-во дней ");
         scanf("%d", &quan);
         market arr[quan]; //для объявления массива базы данных, который хранит данные о ко-ве дней

         for (int i = 0; i < quan; i++)
         { //цикл для вывода всех наших дат, расходов и доходов
            printf("\n");
            for (;;)
            { //бесконечный цикл для даты, длится до испонения условия
               printf("Введи дату(дд мм гггг): ");
               scanf("%d %d %d", &arr[i].date.day, &arr[i].date.month, &arr[i].date.year); //вводим дату из структуры с учетом нашего массива
               if (arr[i].date.day > 0 && arr[i].date.day <= 31 && arr[i].date.month >= 1 && arr[i].date.month <= 12 && arr[i].date.year > 0)
               { //защита от дурака (а именно дней в году 31, месяцев 12 и год не может быть отрицвтелдьным)
                  break;
               }
               else
               { //если условие не исполнилось
                  printf("Нельзя такое вводить\n");
               }
            }
            if (arr[i].date.year < 1000)
            { //не учитываем года, позже 2000-х
               arr[i].date.year = arr[i].date.year + 2000;
            }
            for (;;)
            { //бесконечный цикл для расходов, длится до исполнения условия
               printf("Введи расход: ");
               scanf("%f", &arr[i].outlay); //ввод рахода из структуры с учетом нашего массива
               if (arr[i].outlay > 0)       //само условие
                  break;
               else
               {
                  printf("Нельзя такое вводить\n"); //если условие не исполнилось
               }
            }
            for (;;)
            { //бесконечный цикл для доходов, длится до исполнения условия
               printf("Введи доход: ");
               scanf("%f", &arr[i].income); //ввод дохода из структуры с учетом нашего массива
               if (arr[i].income > 0)       //само условие
                  break;
               else
               {
                  printf("Нельзя такое вводить\n"); //если условие не исполнилось
               }
            }
         }
         break;
      case 2:
         printf("\n");
         printf("Введите дату одного отчетного дня: ");
         scanf("%d %d %d", &date.day, &date.month, &date.year); //для отсчета одного дня
         for (int i = 0; i < quan; i++)
         { //цикл для вывода всех наших дат, расходов и доходов
            if (dateCompare(date, arr[i].date))
            {                                           // условие, если віполниламсьб наша собственная функция
               printf("Расход: %.3f\n", arr[i].outlay); //вывод
               printf("Доход: %.3f\n", arr[i].income);  //вывод
               a = 0;                                   //нашли дату и не выводить пользователю, что нет такой даты, которую он ввел, ведь мы нашли одинаковые числа
               break;
            }
         }
         if (a)
         { //не нашли одинаковых элементов
            printf("Не найдено\n");
         }
         break;

      case 3:
         getche();
         return 0;
      default:
         printf("Вы ввели не номер банковской карточки");
         break;
   }
}