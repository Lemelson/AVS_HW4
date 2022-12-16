#include <fstream>
#include <iostream>
#include <pthread.h>

// Текущая позиция в массиве
int line = 0;
pthread_mutex_t myMutex;
// Текущий поток
int cntThread = 0;
// Количество слов
const int n = 20;
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