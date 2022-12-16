# ИДЗ4 
# Бескоровайный Артём Вариант №32 БПИ216

## Задание:
Пляшущие человечки. На тайном собрании глав преступного мира города Лондона председатель собрания профессор Мориарти постановил: отныне
вся переписка между преступниками должна вестись тайнописью. В качестве
стандарта были выбраны «пляшущие человечки», шифр, в котором каждой
букве латинского алфавита соответствует хитроумный значок. Реализовать
многопоточное приложение, шифрующее исходный текст (в качестве ключа используется кодовая таблица, устанавливающая однозначное сoответствие между каждой буквой и каким-нибудь числом). Каждый поток шифрует свои кусочки текста. При решении использовать парадигму портфеля задач.

## Входные данные:
Входные данные: n

Количество слов в массиве.

## Вывод:

Зашифрованные слова выглядят так:

ab -> bc 

Где ab - изначальное, а bc - зашифрованное!

## Модель параллельных вычислений.
Тут используется портфель задач. Значит, каждый человек будет отправляться кодировать своё слово, беря его из портфеля задач. Портфель - это наши слова, которые мы должны закадировать.


## Отчет га 8 баллов:
- 4 балла (выполнено):

1) Приведено условие задачи.
2) Описана модель параллельных вычислений, используемая при разработке многопоточной программы.
3) Описаны входные данные программы, включающие вариативные
диапазоны, возможные при многократных запусках.
4) Реализовано консольное приложение, решающее поставленную задачу с использованием одного варианта синхропримитивов
5) Ввод данных в приложение реализован с консоли.
6) Результаты работы приведены в отчете.

- 5 баллов (выполнено)
1) В программу добавлены комментарии, поясняющие выполняемые
действия и описание используемых переменных.
2) В отчете должен быть приведен сценарий, описывающий одновре-
менное поведение представленных в условии задания сущностей в
терминах предметной области. То есть, описано поведение объектов
разрабатываемой программы как взаимодействующих субъектов, а
не то, как это будет реализовано в программе.

- 6 - 8 баллов (выполнено)
1) Был реализован метод, который запускается при создании потока
2) Были реализованы ввод из строки (рандом) и чтение из файла.
По умолчанию запускается ввод с консоли.

## Код на Сpp
Код идёт сразу с чтением из файла и рандомной генерацией. 
Стоит отметить, что рандомная генерация хорошо выполняет задачу, так как мы не просто кодируем слова, а еще и заменяем их на непонятные символы, но под умполчанию работает ввод из консоли.

```Cpp
#include <fstream>
#include <iostream>
#include <pthread.h>

// Текущая позиция в массиве
int line = 0;
pthread_mutex_t myMutex;
// Текущий поток
int cntThread = 0;
// Количество слов
const int n = 10;
// Массив слов
std::string words[n];

// Главная функция кодировки
void *mutexFunc(void *param)
{
  pthread_mutex_lock(&myMutex);
  std::string word = words[line];
  word[0] += 1;
  word[1] += 1;
  std::cout << words[line] << " -> " << word << "\n";
  words[line] = word;
  line++;                         // go to next row if bear was not found!
  cntThread++;                       // set num of current thread
  pthread_mutex_unlock(&myMutex);
}


int main(int argc, char** argv)
{
  int letters;
  // Рандомный ввод.
  if (argc == 2) {
    char* arg = argv[1];
    int seed = atoi(arg);
    srand(seed);
    letters = rand() % 32;
    std::string s = "ab";
    for (int i = 0; i < n; ++i) {
      words[i] = s;
      s[0] += letters;
      s[1] += letters;
    }
    // Считываем из файла
  } else if (argc == 3) {
    std::ifstream ifstream(argv[1]);
    ifstream >> letters;
  } else {
    // Дефолтный ввод из консоли.
    std::cout << "Enter num of letters:" << '\n';
    std::cin >> letters;
    std::string s = "ab";
    // Итак, у нас дефолтное слово ab, и мы создаем списко разных слов
    for (int i = 0; i < n; ++i) {
      words[i] = s;
      s[0] += 1;
      s[1] += 1;
    }
  }

  // Список изначальных слов
  std::cout << "Your words: \n";
  for (int i = 0; i < n; ++i) {
    std::cout << words[i] << '\n';
  }
  // Потоки людей, которые кодируют
  pthread_t peoples[n];
  // Создание мьютекса
  pthread_mutex_init(&myMutex,0);
  // Начинаем кодировать каждое слово, запуская по очереди людей (берем задачи из портфеля задач)
  for (int i = 0; i < n; ++i) {
    pthread_create(&peoples[i], 0, mutexFunc, (void *) cntThread);
    std::cout << "People num -  " << i << " was started coding!\n";
  }
  // Портфель задач, где каждый человек получает нужную ему кодировку
  for (int i = 0; i < n; ++i) {
    // Из портфеля задач запускается каждый поток.
    pthread_join(peoples[i], 0);
    std::cout << "People num - " << i << " has finished!!!\n";
  }
  // Мьютекс заканчивается.
  pthread_mutex_destroy(&myMutex);
  return 0;
}
```

Получается, что мы берем наши задачи из портфеля задач и раздаем их на кодирование. 

## Выходные данные:
Вот что мы получили!
```
Enter num of letters:
10
Your words:
ab
bc
cd
de
ef
fg
gh
hi
ij
jk
People num -  ab -> bc
0 was started coding!
People num -  1 was started coding!
bc -> cd
People num -  2 was started coding!
cd -> de
People num -  3 was started coding!
de -> ef
People num -  4 was started coding!
ef -> fg
People num -  5 was started coding!
fg -> gh
People num -  6 was started coding!
gh -> hi
People num -  7 was started coding!
hi -> ij
People num -  8 was started coding!
ij -> jk
People num -  9 was started coding!
jk -> kl
People num - 0 has finished!!!
People num - 1 has finished!!!
People num - 2 has finished!!!
People num - 3 has finished!!!
People num - 4 has finished!!!
People num - 5 has finished!!!
People num - 6 has finished!!!
People num - 7 has finished!!!
People num - 8 has finished!!!
People num - 9 has finished!!!
```

Тут видно, что каждый поток при запуске берет свою задачу, которую выполняет в портфеле. Также я попытался замедлить выполнение, чтобы посмотреть на параллельность действий, что успешно у меня получилось.

## Рандомный ввод:

```
Your words:
ab
КЕ
H
)u
 #
Q│
Ъд
Л_
tJ
'Ё
☻,
Й8
╟╤
ы/
°K
■☻
Eр
°Є
E║
Иu
People num -  ab -> bc
0 was started coding!
People num -  1 was started coding!
КЕ -> ЛЖ
People num -  2 was started coding!
H        -> I

People num -  3 was started coding!
)u -> *v
People num -  4 was started coding!
 # ->  $
People num -  5 was started coding!
Q│ -> R┤
People num -  6 was started coding!
Ъд -> Ые
People num -  7 was started coding!
Л_ -> М`
People num -  8 was started coding!
tJ -> uK
People num -  9 was started coding!
'Ё -> (ё
People num -  10 was started coding!
☻, -> ♥-
People num -  11 was started coding!
Й8 -> К9
People num -  12 was started coding!
╟╤ -> ╚╥
People num -  13 was started coding!
ы/ -> ь0
People num -  14 was started coding!
°K -> ∙L
People num -  15 was started coding!
■☻ ->  ♥
People num -  16 was started coding!
Eр -> Fс
People num -  17 was started coding!
°Є -> ∙є
People num -  18 was started coding!
E║ -> F╗
People num -  19 was started coding!
Иu -> Йv
People num - 0 has finished!!!
People num - 1 has finished!!!
People num - 2 has finished!!!
People num - 3 has finished!!!
People num - 4 has finished!!!
People num - 5 has finished!!!
People num - 6 has finished!!!
People num - 7 has finished!!!
People num - 8 has finished!!!
People num - 9 has finished!!!
People num - 10 has finished!!!
People num - 11 has finished!!!
People num - 12 has finished!!!
People num - 13 has finished!!!
People num - 14 has finished!!!
People num - 15 has finished!!!
People num - 16 has finished!!!
People num - 17 has finished!!!
People num - 18 has finished!!!
People num - 19 has finished!!!
```

Вот результат рандомной шифровки. Получилось достаточно интересно.