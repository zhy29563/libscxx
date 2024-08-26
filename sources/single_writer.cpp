#include <dmlc/json.h>

#include <fstream>
#include <iostream>

void write_functions() {
  dmlc::JSONWriter jw(&std::cerr);

  jw.WriteNoEscape("Write\tNoEscape");
  jw.Write('\n');

  jw.WriteString("W\rrite\tString\n");
  jw.Write('\n');

  jw.WriteNumber<int>(20);
  jw.Write('\t');

  jw.WriteNumber<long>(30);
  jw.Write('\t');

  jw.WriteNumber<float>(40.1);
  jw.Write('\t');

  jw.WriteNumber<double>(50.2);
  jw.Write('\n');

  int int_arr[]{1, 2, 3, 4, 5};

  jw.BeginArray(true);
  for (auto i : int_arr) {
    jw.WriteArrayItem<int>(i);
  }
  jw.EndArray();

  jw.BeginArray(false);
  for (auto i : int_arr) {
    jw.WriteArrayItem<int>(i);
  }
  jw.EndArray();

  jw.BeginArray(false);
  for (auto i : int_arr) {
    jw.WriteArrayItem<int>(i);
    jw.WriteArraySeperator();
  }
  jw.EndArray();

  jw.BeginObject(false);
  jw.WriteObjectKeyValue<int>("age", 10);
  jw.EndObject();

  jw.Write<int>(100);
}

void write_bookstore() {
  std::ofstream os("bookstore.json");
  if (!os.good()) return;

  dmlc::JSONWriter jw(&os);
  jw.BeginObject(true);
  {
    jw.WriteObjectKeyValue<std::string>("name", "bookstore");
    jw.WriteArraySeperator();

    jw.WriteString("books");
    jw.Write<char>(':');

    jw.BeginArray(true);
    {
      jw.BeginObject(true);
      {
        jw.WriteObjectKeyValue<std::string>("name", "book1");
        jw.WriteObjectKeyValue<int>("year", 2023);
        jw.WriteArraySeperator();

        jw.WriteNoEscape("authors"), jw.Write<char>(':');
        jw.BeginArray(false);
        jw.WriteArrayItem<std::string>("Joe");
        jw.WriteArrayItem<std::string>("Steven");
        jw.EndArray();

        jw.WriteArraySeperator();

        jw.WriteNoEscape("characters");
        jw.Write<char>(':');

        jw.BeginArray(true);
        {
          jw.BeginObject(true);
          jw.WriteObjectKeyValue<std::string>("name", "ch1");
          jw.WriteObjectKeyValue<int>("page", 1);
          jw.EndObject();

          jw.Write<char>(',');

          jw.BeginObject(true);
          jw.WriteObjectKeyValue<std::string>("name", "ch2");
          jw.WriteObjectKeyValue<int>("page", 10);
          jw.EndObject();
        }
        jw.EndArray();
      }
      jw.EndObject();

      jw.Write<char>(',');
      jw.BeginObject(true);
      {
        jw.WriteObjectKeyValue<std::string>("name", "book2");
        jw.WriteObjectKeyValue<int>("year", 2022);
        jw.WriteArraySeperator();

        jw.WriteNoEscape("authors");
        jw.Write<char>(':');

        jw.BeginArray(false);
        jw.WriteArrayItem<std::string>("Mike");
        jw.WriteArrayItem<std::string>("July");
        jw.EndArray();

        jw.WriteArraySeperator();

        jw.WriteNoEscape("characters");
        jw.Write<char>(':');

        jw.BeginArray(true);
        {
          jw.BeginObject(true);
          jw.WriteObjectKeyValue<std::string>("name", "ch1");
          jw.WriteObjectKeyValue<int>("page", 1);
          jw.EndObject();

          jw.Write<char>(',');

          jw.BeginObject(true);
          jw.WriteObjectKeyValue<std::string>("name", "ch2");
          jw.WriteObjectKeyValue<int>("page", 20);
          jw.EndObject();
        }
        jw.EndArray();
      }
      jw.EndObject();
    }
    jw.EndArray();
  }
  jw.EndObject();

  os.close();
}

int main() {
  write_functions();
  write_bookstore();
  return 0;
}