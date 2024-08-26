#include <glib.h>
#include <stdio.h>
#include <stdlib.h>

/* GVariant 格式字符串
 * 1. 变量参数转换
 *    转换依赖于格式字符串。格式字符串在一个 GVariant 值与一个或多个 C 值之间执行双向映射
 *    从 C 值转换到 GVariant 值使用函数 g_variant_new()
 *    从 GVariant 值转换到 C 值使用函数 g_variant_get()
 * 2. 语法
 *    此处详尽地列出了所有可能的格式字符串。有以下可能的形式：
 *    - 任何类型的字符串
 *    - 以@为前缀的类型字符串
 *    - &s, &o, &g, ^as, ^a&s, ^ao, ^a&o, ^ay, ^&ay, ^aay or ^a&ay
 *    - 以m为前缀的任何格式字符串
 *    - 一系列零个或多个格式字符串，连接并括在括号中
 *    - 一个左大括号，后跟两个格式字符串，然后是一个右大括号（前提是第一个格式字符串对应于可用作字典键类型的有效类型）
 * 3. 符号
 *    - b, y, n, q, i, u, x, t, h, d
 *      构建或销毁 boolean, byte, numberic 类型
 *    - s, o, g
 *      构建或销毁 string 类型
 *    - v
 *      构建或销毁 variant 类型
 *    - a
 *      构建或销毁 arrays 类型
 *    - m
 *      构建或销毁 maybe 类型
 *    - ()
 *      构建或销毁 tuples 类型
 *    - {}
 *      构建或销毁 dictionary 类型
 *    - @
 *      用作 GVariant 类型字符串的前缀（不是格式字符串的前缀，因此是有效的格式字符串，但 @^as 不是）。
 *      表示应该使用指向 GVariant 的指针来代替普通的 C 类型或类型。
 *      对于 g_variant_new() 这意味着您必须传递一个非 NULL (GVariant *);如果它是一个浮动引用，则所有权将被获取，就像使用
 * g_variant_ref_sink() 一样。 对于 g_variant_get() 这意味着您必须传递一个指向 (GVariant *) 的指针，以便通过引用返回值或
 * NULL 以忽略该值。
 *    - *, ?, r
 *      完全等价于@*,@?和r。仅为了完整性而提供，以便所有 GVariant 类型的字符串也可以用作格式字符串。
 *    - &
 *      用作 GVariant 类型字符串的前缀（不是格式字符串的前缀，因此 &s 是有效的格式字符串，但 &s"
 * 不是）。表示应该使用指向序列化数据的 C 指针来代替普通的 C 类型。
 *    - ^
 *      用作某些特定类型的格式字符串的前缀
 */

void TEST_NumericTypes() {
  // 数字类型的变量参数转换以最明显的方式工作。
  // 在遇到这些字符之一时，g_variant_new() 将等效的 C 类型作为参数。
  // g_variant_get() 采用指向等效 C 类型的指针（或 NULL 以忽略该值）。
  // Character    Equivalent C type
  // b            gboolean
  // y            guchar
  // n            gint16
  // q            guint16
  // i            gint32
  // u            guint32
  // x            gint64
  // t            guint64
  // h            gint32
  // d            gdouble
  // 注意
  //    在 C 中，变量参数列表中的小整数类型会酌情提升为 int 或 unsigned int，并相应地回读。
  //    int 在当前支持 GLib 的每个平台上都是 32 位的。这意味着您可以使用带有 g_variant_new()
  //    和格式字符 b、y、n、q、i、u 和 h 的 int 类型的 C 表达式。具体来说，您可以将整数文字与这些字符一起使用。
  //    使用 x 和 t 字符时，您必须确保您提供的值是 64 位。这意味着您应该使用强制转换或使用 G_GINT64_CONSTANT 或
  //    G_GUINT64_CONSTANT 宏 使用 g_variant_get()
  //    时不会发生类型提升，因为它使用指针操作。指针必须始终指向大小完全正确的内存区域。

  GVariant *value1;
  GVariant *value2;
  GVariant *value3;
  GVariant *value4;

  value1 = g_variant_new("y", 200);
  value2 = g_variant_new("b", TRUE);
  value3 = g_variant_new("d", 37.5);
  value4 = g_variant_new("x", G_GINT64_CONSTANT(998877665544332211));

  {
    gdouble  floating;
    gboolean truth;
    gint64   bignum;

    g_variant_get(value1, "y", NULL); /* ignore the value. */
    g_variant_get(value2, "b", &truth);
    g_variant_get(value3, "d", &floating);
    g_variant_get(value4, "x", &bignum);
  }
}

void TEST_string() {
  // 与标准以 nul 结尾的 C 字符串进行字符串转换。
  // 在格式字符串中遇到 s、o 或 g 时，g_variant_new() 将获取 (const gchar *) 并对其进行复制。
  // NULL 不是一个有效的字符串；使用可能的类型来编码。
  // 如果使用 o 或 g 字符，则必须注意确保传递的字符串分别是有效的 D-Bus 对象路径或 D-Bus 类型签名。

  // 在遇到 s、o 或 g 时，g_variant_get() 获取指向 (gchar *) 的指针（即：(gchar **)）并将其设置为新分配的字符串副本。
  // 使用 g_free() 释放这个副本是合适的。也可以传递 NULL 来指示字符串的值应该被忽略（在这种情况下不复制）。

  GVariant *value1;
  GVariant *value2;
  GVariant *value3;

  value1 = g_variant_new("s", "hello world!");
  value2 = g_variant_new("o", "/must/be/a/valid/path");
  value3 = g_variant_new("g", "iias");

#if 0
  g_variant_new ("s", NULL);      /* not valid: NULL is not a string. */
#endif

  {
    gchar *result;

    g_variant_get(value1, "s", &result);
    g_print("It was '%s'\n", result);
    g_free(result);
  }
}

int main() {
  GVariant *v = g_variant_new("u", 40);
  return 0;
}
