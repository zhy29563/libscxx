#include <readerwriterqueue/readerwriterqueue.h>

#include <iostream>

template <class T>
void PrintQueue(moodycamel::ReaderWriterQueue<T>& queue, const char* description = nullptr) {
  std::cout << "------------------------------------------------ " << std::endl;
  std::cout << "description  : " << description << std::endl;
  std::cout << "max_capacity : " << queue.max_capacity() << std::endl;
  std::cout << "size_approx  : " << queue.size_approx() << std::endl;
  std::cout << "peek address : " << queue.peek() << std::endl;
  T    element;
  bool succeeded;
  while (queue.size_approx() > 0) {
    succeeded = queue.try_dequeue(element);
    if (succeeded) {
      std::cout << element << " ";
    }
  }
  std::cout << std::endl;
}

int main() {
  {
    // 默认构造
    moodycamel::ReaderWriterQueue<int> q;  // max_capacity: 15
    PrintQueue(q, "default");

    // 有参构造，查看最大容量
    moodycamel::ReaderWriterQueue<int> q3(3);  // max_capacity: 3
    PrintQueue(q3, "3");

    // 有参构造，查看最大容量
    moodycamel::ReaderWriterQueue<int> q5(5);  // max_capacity: 7
    PrintQueue(q5, "5");
  }

  {
    moodycamel::ReaderWriterQueue<int> q(5);
    for (int index = 0; index < 10; index++) {
      // q.try_emplace(index);
      if (!q.try_enqueue(index)) {
        std::cout << "Insert element is failed, element: " << index << std::endl;
      }
    }
    PrintQueue(q, "try_enqueue/try_emplace");  // 0 1 2 3 4 5 6
  }

  {
    moodycamel::ReaderWriterQueue<int> q(3);
    PrintQueue(q, "init");

    for (int index = 0; index < 5; index++) {
      // q.enqueue(index);
      q.emplace(index);
    }
    PrintQueue(q, "enqueue/emplace");  // 0 1 2 3 4 5 6 7
  }

  return EXIT_SUCCESS;
}
