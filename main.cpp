#include "ID3Parser.h"

int main(int argc, char** argv) {
  for (int i = 1; i < argc; i++){
    Tag my_tag(argv[1]);
    for (Frame* argument : my_tag.arguments) {
      std::cout << *argument;
    }
  }
  return 0;
}