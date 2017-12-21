//
// Created by Александр on 18.12.17.
//

#ifndef CLIENT_COMMON_DIFFIEHELLMANWRAPPER_H
#define CLIENT_COMMON_DIFFIEHELLMANWRAPPER_H


#include <gmpxx.h>

class DiffieHellmanWrapper {
    mpz_class server_secret, client_exp, shared_secret, prime, group;
private:
    DiffieHellmanWrapper();
    std::string sha256(std::string line);
    int lower_bound;
    std::string getHash();
public:
    static DiffieHellmanWrapper* getInstance();
    ~DiffieHellmanWrapper();
    std::string getPrimeNumber();
    std::string getServersSecret();
    void setClientExp(mpz_class& server_exp);
    void calcSharedSecret();
    void setServerSecret(mpz_class from, mpz_class to);
    std::string checkStringToMpz(std::string toMpz);

};


#endif //CLIENT_COMMON_DIFFIEHELLMANWRAPPER_H