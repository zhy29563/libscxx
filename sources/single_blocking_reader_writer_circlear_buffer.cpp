#include <readerwriterqueue/readerwritercircularbuffer.h>

#include <iostream>

template <class T>
void PrintQueue(moodycamel::BlockingReaderWriterCircularBuffer<T>& queue, const char* description = nullptr) {
  std::cout << "------------------------------------------------ " << std::endl;
  std::cout << "description  : " << description << std::endl;
  std::cout << "max_capacity : " << queue.max_capacity() << std::endl;
  std::cout << "size_approx  : " << queue.size_approx() << std::endl;

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
    moodycamel::BlockingReaderWriterCircularBuffer<int> q3(3);  // max_capacity: 3
    PrintQueue(q3, "3");

    moodycamel::BlockingReaderWriterCircularBuffer<int> q5(5);  // max_capacity: 5
    PrintQueue(q5, "5");

    // 容量为指定位置
  }

  {
    moodycamel::BlockingReaderWriterCircularBuffer<int> q(5);
    for (int index = 0; index < 8; index++) {
      if (!q.try_enqueue(index)) {  // 如果插入元素时，队列已满，则丢弃
        std::cout << "Insert element is failed, element: " << index << std::endl;
      }
    }
    PrintQueue(q, "try_enqueue");  // 0 1 2 3 4
  }

  {
    moodycamel::BlockingReaderWriterCircularBuffer<int> q(5);
    for (int index = 0; index < 8; index++) {
      if (!q.wait_enqueue_timed(index, 5)) {  // 队列已满且在指定的超时时间内未获得存储空间，则插入失败，元素被丢弃
        std::cout << "Insert element is failed, element: " << index << std::endl;
      }
    }
    PrintQueue(q, "wait_enqueue_timed");  // 0 1 2 3 4
  }

  {
    moodycamel::BlockingReaderWriterCircularBuffer<int> q(5);
    for (int index = 0; index < 8; index++) {
      q.try_enqueue(index);
    }

    int element1;
    q.wait_dequeue(element1);
    std::cout << "wait_dequeue->element1: " << element1 << std::endl;

    int element2;
    if (q.wait_dequeue_timed(element2, std::chrono::milliseconds(1))) {
      std::cout << "element2: " << element2 << std::endl;
    }

    PrintQueue(q, "wait_dequeue/wait_dequeue_timed");  // 2 3 4
  }

  return EXIT_SUCCESS;
}
