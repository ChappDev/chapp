//
// Created by Александр on 18.12.17.
//

#ifndef CLIENT_COMMON_DIFFIEHELLMANWRAPPER_H
#define CLIENT_COMMON_DIFFIEHELLMANWRAPPER_H


#include <gmpxx.h>
#include "AesEncoder.h"
class AesEncoder;
class DiffieHellmanWrapper {
    mpz_class client_secret, server_exp, shared_secret, prime, group;
    friend class AesEncoder;
private:
    DiffieHellmanWrapper();
    std::string sha256(std::string line);
    int lower_bound;
    std::string getHash();
public:
    static DiffieHellmanWrapper* getInstance();
    ~DiffieHellmanWrapper();
    std::string getClientsSecret();
    void setServerExp(mpz_class& server_exp);
    void calcSharedSecret();
    void setPrimeNumber(mpz_class& prime);
    void setClientSecret(mpz_class from, mpz_class to);
    void checkStringToMpz(std::string toMpz);
    std::string getIV();

};


#endif //CLIENT_COMMON_DIFFIEHELLMANWRAPPER_H
