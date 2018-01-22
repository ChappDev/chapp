//
// Created by Александр on 22.01.18.
//

#include <openssl/sha.h>
#include "User.h"

void User::setName(std::string &_name) {

}

bool User::authenticate(std::string &password) {
  return hash == getHash(password);
}

std::string& User::getHash(std::string &password) {
  unsigned char hashedPass[SHA256_DIGEST_LENGTH];
  SHA256_CTX sha256;
  SHA256_Init(&sha256);
  SHA256_Update(&sha256, password.c_str(), password.length());
  SHA256_Final(hashedPass, &sha256);

  std::string output = "";
  for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    output += hashedPass[i];
  }
  return output;
}
void User::setHash(std::string &password) {
  hash = getHash(password);
}
bool User::empty() {
  return name == "";
}
void User::flush() {
  name = "";
  hash = "";
  is_used = false;
}