#include <dmlc/json.h>

#include <fstream>
#include <iostream>

int main() {
  std::ifstream is("bookstore.json");
  if (!is.good()) {
    return EXIT_FAILURE;
  }

  dmlc::JSONReader jr(&is);

  std::string key;
  std::string val_s;
  char        val_c;
  int         val_i;

  jr.BeginObject();
  {
    if (jr.NextObjectItem(&key)) {
      jr.ReadString(&val_s);
      std::cout << key << " : " << val_s << std::endl;
    }

    if (jr.NextObjectItem(&key)) {
      std::cout << key << " : " << std::endl;

      jr.BeginArray();
      while (jr.NextArrayItem()) {
        std::cout << "\t==================================" << std::endl;
        jr.BeginObject();
        if (jr.NextObjectItem(&key)) jr.ReadString(&val_s), std::cout << "\t" << key << " : " << val_s << std::endl;

        if (jr.NextObjectItem(&key))
          jr.ReadNumber<int>(&val_i), std::cout << "\t" << key << " : " << val_i << std::endl;

        if (jr.NextObjectItem(&key)) {
          std::cout << "\t" << key << " : " << std::endl;
          jr.BeginArray();
          while (jr.NextArrayItem()) jr.Read<std::string>(&val_s), std::cout << "\t\t" << val_s << std::endl;
        }

        if (jr.NextObjectItem(&key)) {
          std::cout << "\t" << key << " : " << std::endl;
          jr.BeginArray();
          while (jr.NextArrayItem()) {
            jr.BeginObject();
            if (jr.NextObjectItem(&key))
              jr.ReadString(&val_s), std::cout << "\t\t" << key << " : " << val_s << std::endl;

            if (jr.NextObjectItem(&key))
              jr.ReadNumber<int>(&val_i), std::cout << "\t\t" << key << " : " << val_i << std::endl;

            if (jr.NextObjectItem(&key)) jr.Read<char>(&val_c);
          }
        }
        if (!jr.NextObjectItem(&key)) continue;
      }
    }
  }

  is.close();

  return EXIT_SUCCESS;
}