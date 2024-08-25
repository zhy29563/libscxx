#include <stdio.h>
#include <string.h>
#include <zmq.h>

int main(int argc, char const *argv[]) {
  void *counter = zmq_atomic_counter_new();

  // 获取当前值
  int val = zmq_atomic_counter_value(counter);
  printf("1. current value = %d\n", val);

  // 值增加
  val = zmq_atomic_counter_inc(counter);  // 返回增加前的值
  printf("2. current value = %d\n", val);

  val = zmq_atomic_counter_inc(counter);
  printf("3. current value = %d\n", val);

  val = zmq_atomic_counter_inc(counter);
  printf("4. current value = %d\n", val);

  val = zmq_atomic_counter_value(counter);
  printf("5. current value = %d\n", val);

  val = zmq_atomic_counter_dec(counter);  // 如果执行该函数之后，计数器的值不为零，返回1；否则返回零
  printf("6. current value = %d\n", val);

  val = zmq_atomic_counter_dec(counter);
  printf("7. current value = %d\n", val);

  val = zmq_atomic_counter_dec(counter);
  printf("8. current value = %d\n", val);

  val = zmq_atomic_counter_value(counter);
  printf("9. current value = %d\n", val);

  val = zmq_atomic_counter_dec(counter);
  printf("10. current value = %d\n", val);

  val = zmq_atomic_counter_value(counter);
  printf("11. current value = %d\n", val);

  zmq_atomic_counter_set(counter, 2);

  val = zmq_atomic_counter_value(counter);
  printf("12. current value = %d\n", val);

  zmq_atomic_counter_destroy(&counter);
  return 0;
}
