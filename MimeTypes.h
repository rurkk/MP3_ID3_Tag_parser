#include <cstring>
#include <string>

const uint16_t kNumberOfMimeTypes = 347;

class MimeTypes {
 public:
  static const char* getExtension(const char* type, int skip = 0);

 private:
  struct entry {
    const char* fileExtension;
    const char* mimeType;
  };
  static MimeTypes::entry types[kNumberOfMimeTypes];
};
