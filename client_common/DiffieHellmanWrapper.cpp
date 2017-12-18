//
// Created by Александр on 18.12.17.
//

#include <iostream>
#include "DiffieHellmanWrapper.h"
#include <openssl/sha.h>
DiffieHellmanWrapper& DiffieHellmanWrapper::getInstance() {
    static DiffieHellmanWrapper* instance;
    if(instance == nullptr){
        instance = new DiffieHellmanWrapper();
    }
    return *instance;
}
DiffieHellmanWrapper::~DiffieHellmanWrapper() = default;
/**
 * constructor
 * sets group const and clients(servers secret)
 */
DiffieHellmanWrapper::DiffieHellmanWrapper() {
    group = 7;
    mpz_class lower_base = 13;
    mpz_class min_value,max_value;
    mpz_pow_ui(min_value.get_mpz_t(),lower_base.get_mpz_t(),50);
    mpz_pow_ui(max_value.get_mpz_t(),lower_base.get_mpz_t(),50);
    setClientSecret(min_value,max_value);
}
/**
 * get random number, seeds by time and adding
 * @param from
 * @param to
 */
void DiffieHellmanWrapper::setClientSecret(mpz_class from, mpz_class to) {
    gmp_randclass randseed(gmp_randinit_default);
    randseed.seed(time(NULL));
    client_secret = randseed.get_z_range(from);
    to = randseed.get_z_range(to);
    __gmpz_add(client_secret.get_mpz_t(),client_secret.get_mpz_t(),to.get_mpz_t());
    if(!__gmpz_cmp_si(client_secret.get_mpz_t(),10000000L)){
        abort();
    }
    std::cout << client_secret.get_mpz_t() << "\n";
}
void DiffieHellmanWrapper::setServerExp(mpz_class server_exp) {
    server_exp = server_exp;
}
void DiffieHellmanWrapper::setPrimeNumber(mpz_class prime) {
    prime = prime;
}
/**
 * calcs pow by module of second step of DH algorithm
*/
void DiffieHellmanWrapper::calcSharedSecret() {
    mpz_powm(shared_secret.get_mpz_t(),server_exp.get_mpz_t(),client_secret.get_mpz_t(),prime.get_mpz_t());
    std::cout << shared_secret.get_mpz_t() << "\n";
}
std::string DiffieHellmanWrapper::sha256(std::string line) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, line.c_str(), line.length());
    SHA256_Final(hash, &sha256);

    std::string output = "";
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        output += hash[i];
    }
    return output;
}
/**
 * gets hashed shared secret of size 256 bits, it can be aes key.
 * @return
 */
std::string DiffieHellmanWrapper::getHash() {
    return sha256(shared_secret.get_str(10));
}