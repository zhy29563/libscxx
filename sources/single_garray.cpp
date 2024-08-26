#include <glib.h>
#include <stdio.h>
#include <stdlib.h>

#include "Logger.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 测试函数
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// struct GArray
// {
//   gchar* data;   /*数据指针*/
//   guint len;     /*元素数量，不包含可能结尾零*/
// }

// g_print
GArray *arr = NULL;

void print_array(GArray *arr) {
  LOG_DEB("array addr   : = %p", arr);
  LOG_DEB("array length : = %d", arr->len);
  LOG_DEB("array pointor: = %p", arr->data);

  gint *data = reinterpret_cast<gint *>(arr->data);
  for (guint i = 0U; i < arr->len; i++) {
    LOG_DEB("index = %2d, value = %d", i, *(data + i));
  }
}

void TEST_new() {
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // 创建数组
  // GArray* g_array_new (gboolean zero_terminated, gboolean clear_, guint element_size)
  //  zero_terminated: 指定数组是否以零结尾。如果值为TRUE，则会在末尾处多出一个值为零的元素
  //  clear_         : 是否在分配内存时，将所有内容设置为零。如果值为TRUE，则执行零初始化
  //  element_size   : 元素的字节大小
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  arr = g_array_new(FALSE, FALSE, sizeof(gint));
  print_array(arr);
}

void TEST_free() {
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // 释放数组内存
  // gchar* g_array_free (GArray* array, gboolean free_segment)
  //  array       : 待释放数组地址
  //  free_segment: 是否释放数组中的实际存储的数据
  //  return value: 如果参数 free_segment 指定为FALSE，则返回数据的地址；否则返回NULL。元素数据的释放需要使用 g_free()
  //  函数。值为
  //                以 NUL 结束的 UTF-8 字符串。
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  gchar *ret_val = g_array_free(arr, TRUE);
  LOG_DEB("return value: %s", ret_val);
}

void TEST_element_size() {
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // 获取存储数组中单个元素所需的字节数
  // guint g_array_get_element_size ( GArray* array)
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  guint ele_bytes = g_array_get_element_size(arr);
  g_print("\telement bytes: = %d\n\n", ele_bytes);
}

void TEST_append() {
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // 尾插
  // #define g_array_append_val(a,v)	  g_array_append_vals (a, &(v), 1)
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  gint a = -1;
  g_array_append_val(arr, a);
  print_array(arr);
}

void TEST_appends() {
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // 尾插
  // GArray* g_array_append_vals ( GArray* array, gconstpointer data, guint len)
  //  array       : 需要尾插元素的数组指针
  //  data        : 待添加元素的地址
  //  len         : 待添加元素的数量
  //  return value: 添加元素后的数组指针, 实际上与参数 array 的结果相同
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  gint     count = 5;
  gpointer ptr   = g_malloc(sizeof(gint) * count);
  LOG_DEB("element addr : = %p", ptr);
  gint *elements_ptr = reinterpret_cast<gint *>(ptr);
  for (gint i = 0; i < count; i++) {
    *(elements_ptr++) = i;
  }

  g_array_append_vals(arr, ptr, count);
  print_array(arr);
  g_free(ptr);
}

void TEST_prepend() {
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // 尾插
  // #define g_array_prepend_val(a,v)	  g_array_prepend_vals (a, &(v), 1)
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  gint a = 9;
  g_array_prepend_val(arr, a);
  print_array(arr);
}

void TEST_prepends() {
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // 前插
  // GArray* g_array_prepend_vals ( GArray* array, gconstpointer data, guint len)
  //  array       : 需要前插元素的数组指针
  //  data        : 待添加元素的地址
  //  len         : 待添加元素的数量
  //  return value: 添加元素后的数组指针, 实际上与参数 array 的结果相同
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  gint     count = 5;
  gpointer ptr   = g_malloc(sizeof(gint) * count);
  LOG_DEB("element addr : = %p", ptr);
  gint *elements_ptr = reinterpret_cast<gint *>(ptr);
  for (gint i = 10; i < count + 10; i++) {
    *(elements_ptr++) = i;
  }

  g_array_prepend_vals(arr, ptr, count);
  print_array(arr);
  g_free(ptr);
}

void TEST_insert() {
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // 位插
  // #define g_array_insert_val(a,i,v) g_array_insert_vals (a, i, &(v), 1)
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  gint a = 19;
  g_array_insert_val(arr, 1, a);
  print_array(arr);
}

void TEST_inserts() {
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // 位插
  // GArray* g_array_insert_vals ( GArray* array, guint index_, gconstpointer data, guint len)
  //  array       : 需要前插元素的数组指针
  //  index_      : 插入数据在原始数组中的索引位置
  //                如果值大于当前数组的长度，则数组被扩展。如果在初始化时指定清空数组元素，则原数组末尾到指定索引之间的元素被赋值为零，否
  //                则值是不确定的。
  //                如果值小于当前数组的长度，则原数组中大于索引的元素将被后移
  //  data        : 待添加元素的地址。该值仅在参数 len 值为 0 时可为 NULL
  //  len         : 待添加元素的数量。如果 len 为零，函数不执行任何操作
  //  return value: 添加元素后的数组指针, 实际上与参数 array 的结果相同
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  gint     count  = 5;
  gint     offset = 20;
  gpointer ptr    = g_malloc(sizeof(gint) * count);
  g_print("element addr : = %p", ptr);
  gint *elements_ptr = reinterpret_cast<gint *>(ptr);
  for (gint i = offset; i < count + offset; i++) {
    *(elements_ptr++) = i;
  }

  g_array_insert_vals(arr, 2, ptr, count);
  print_array(arr);
  g_free(ptr);
}

void TEST_remove_index() {
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // 移除元素
  // GArray* g_array_remove_index ( GArray* array, guint index_)
  //  array       : 数组指针
  //  index_      : 待移除元素的索引位置
  //  return value: 添加元素后的数组指针, 实际上与参数 array 的结果相同
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  g_array_remove_index(arr, 0);
  g_array_remove_index(arr, 1);
  print_array(arr);
}

void TEST_remove_index_fast() {
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // 移除元素，用末尾元素填充预移除的元素
  // GArray* g_array_remove_index_fast ( GArray* array, guint index_)
  //  array       : 数组指针
  //  index_      : 待移除元素的索引位置
  //  return value: 添加元素后的数组指针, 实际上与参数 array 的结果相同
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  g_array_remove_index_fast(arr, 0);
  g_array_remove_index_fast(arr, 1);
  print_array(arr);
}

void TEST_remove_range() {
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // 移除元素
  // GArray* g_array_remove_range ( GArray* array, guint index_, guint length)
  //  array       : 数组指针
  //  index_      : 待移除元素的起始索引
  //  length      : 移除元素的长度
  //  return value: 添加元素后的数组指针, 实际上与参数 array 的结果相同
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  g_array_remove_range(arr, 3, 2);
  print_array(arr);
}

gint cmpint(gconstpointer a, gconstpointer b) {
  const gint *_a = static_cast<const gint *>(a);
  const gint *_b = static_cast<const gint *>(b);

  return (*_a - *_b);
}
void TEST_binary_search() {
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // 二分查找
  // gboolean g_array_binary_search ( GArray* array, gconstpointer target, GCompareFunc compare_func, guint*
  // out_match_index)
  //  array          : 数组指针
  //  target         : 待查找的目标
  //  compare_func   : 比较函数 gint (* GCompareFunc) (gconstpointer a, gconstpointer b)
  //  out_match_index: 查找到元素在数组中的索引
  //  return value   : 查找到指定元素，返回TRUE，否则返回FALSE
  //  如果元素被找到，函数返回TRUE，且位置索引由 out_match_index 参数传出。否则，函数返回FALSE，out_match_index
  //  的值是未定义的。 如果待查找元素在数组中出现多次，返回第一个被查找到的索引。
  //  该函数使用二分法进行查找，因此数组必须是有序的，否则结果不定
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  gint     target = 12;
  guint    out_match_index;
  gboolean ret = g_array_binary_search(arr, &target, cmpint, &out_match_index);
  g_print("\tret            : = %d\n", ret);
  g_print("\tout_match_index: = %u\n", out_match_index);
}

void TEST_binary_sort() {
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // 排序
  // void g_array_sort ( GArray* array, GCompareFunc compare_func)
  //  array          : 数组指针
  //  compare_func   : 比较函数，gint (* GCompareFunc) (gconstpointer a, gconstpointer b)
  //                   类似于qsort，如果第一个参数小于第二个参数，返回小于零的值；
  //                               如果第一个参数等于第二个参数，返回等于零的值；
  //                               如果第一个参数大于第二个参数，返回等大零的值；
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  g_array_sort(arr, cmpint);
  print_array(arr);
}

gint cmpdataint(gconstpointer a, gconstpointer b, gpointer user_data) {
  const gint *_a = static_cast<const gint *>(a);
  const gint *_b = static_cast<const gint *>(b);
  if (user_data == NULL) {
    return *_a - *_b;
  }

  GArray *data_ptr = (GArray *)user_data;
  gint    index_a  = -1;
  gint    index_b  = -1;
  for (int i = 0; i < data_ptr->len; i++) {
    gint ele = g_array_index(data_ptr, gint, i);
    if (*_a == ele) {
      index_a = i;
      break;
    }
  }

  for (int i = 0; i < data_ptr->len; i++) {
    if (*_b == g_array_index(data_ptr, gint, i)) {
      index_b = i;
      break;
    }
  }

  if (index_a == -1 || index_b == -1) {
    return *_a - *_b;
  }

  return index_a - index_b;
}
void TEST_binary_sort_data() {
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // 排序，以元素在指定数据中的位置进行排序
  // void g_array_sort_with_data ( GArray* array, GCompareDataFunc compare_func, gpointer user_data)
  //  array          : 数组指针
  //  compare_func   : 比较函数，gint (* GCompareDataFunc) ( gconstpointer a, gconstpointer b, gpointer user_data)
  //                   如果第一个参数在给给定数据中的索引小于第二个参数在给给定数据中的索引，返回小于零的值；
  //                   如果第一个参数在给给定数据中的索引等于第二个参数在给给定数据中的索引，返回等于零的值；
  //                   如果第一个参数在给给定数据中的索引大于第二个参数在给给定数据中的索引，返回等大零的值；
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  GArray *user_data = g_array_new(FALSE, TRUE, sizeof(gint));
  for (gint i = 30; i >= -30; i--) {
    g_array_append_val(user_data, i);
  }

  g_array_sort_with_data(arr, cmpdataint, user_data);
  g_array_free(user_data, TRUE);
  print_array(arr);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 主函数
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
  TEST_new();
  TEST_element_size();
  TEST_append();
  TEST_appends();
  TEST_prepend();
  TEST_prepends();
  TEST_prepends();
  TEST_insert();
  TEST_inserts();
  TEST_remove_index();
  TEST_remove_index_fast();
  TEST_remove_range();
  TEST_binary_search();
  TEST_binary_sort();
  TEST_binary_search();
  TEST_binary_sort_data();
  TEST_free();
  return 0;
}
