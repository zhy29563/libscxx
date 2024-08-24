#include <readerwriterqueue/readerwriterqueue.h>

#include <iostream>

template <class T>
void PrintQueue(moodycamel::BlockingReaderWriterQueue<T>& queue, const char* description = nullptr) {
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
    moodycamel::BlockingReaderWriterQueue<int> q;  // max_capacity: 15
    PrintQueue(q, "default");

    moodycamel::BlockingReaderWriterQueue<int> q3(3);  // max_capacity: 3
    PrintQueue(q3, "3");

    moodycamel::BlockingReaderWriterQueue<int> q5(5);  // max_capacity: 7
    PrintQueue(q5, "5");
  }

  {
    moodycamel::BlockingReaderWriterQueue<int> q(5);
    for (int index = 0; index < 8; index++) {
      // q.enqueue(index);
      q.emplace(index);
    }
    PrintQueue(q, "enqueue/emplace");  // 0 1 2 3 4 5 6 7
  }

  {
    moodycamel::BlockingReaderWriterQueue<int> q(5);
    for (int index = 0; index < 8; index++) {
      // q.try_emplace(index);
      if (!q.try_enqueue(index)) {
        std::cout << "Insert element is failed, element: " << index << std::endl;
      }
    }
    PrintQueue(q, "try_enqueue/try_emplace");  // 0 1 2 3 4 5 6
  }

  {
    moodycamel::BlockingReaderWriterQueue<int> q(5);
    for (int index = 0; index < 8; index++) {
      q.emplace(index);
    }

    int element1;
    q.wait_dequeue(element1);  // 如果队列为空，函数会阻塞到队列非空

    int element2;
    if (q.wait_dequeue_timed(element2, 5)) {  // 如果队列为空，函数会阻塞到超时结束，然后返回false
      std::cout << "element2: " << element2 << std::endl;
    }

    PrintQueue(q, "wait_dequeue/wait_dequeue_timed");
  }

  return EXIT_SUCCESS;
}
