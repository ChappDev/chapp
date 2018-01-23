//
// Created by Александр on 18.12.17.
//

#include <iostream>
#include "DiffieHellmanWrapper.h"
#include <openssl/sha.h>

DiffieHellmanWrapper* DiffieHellmanWrapper::getInstance() {
    static DiffieHellmanWrapper* instance;
    if(instance == nullptr){
        instance = new DiffieHellmanWrapper();
    }
    return instance;
}
DiffieHellmanWrapper::~DiffieHellmanWrapper() = default;
/**
 * constructor
 * sets group const and clients(servers secret)
 */
DiffieHellmanWrapper::DiffieHellmanWrapper() {
    group = 7;
    //from RSA standart (2048 bit prime number)
    prime = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784406918290641249515082189298559149176184502808489120072844992687392807287776735971418347270261896375014971824691165077613379859095700097330459748808428401797429100642458691817195118746121515172654632282216869987549182422433637259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133844143603833904414952634432190114657544454178424020924616515723350778707749817125772467962926386356373289912154831438167899885040445364023527381951378636564391212010397122822120720357";
    mpz_class lower_base = 13;
    mpz_class min_value,max_value;
    mpz_pow_ui(min_value.get_mpz_t(),lower_base.get_mpz_t(),50);
    mpz_pow_ui(max_value.get_mpz_t(),lower_base.get_mpz_t(),50);
    setServerSecret(min_value,max_value);
}
std::string DiffieHellmanWrapper::getServersSecret() {
    mpz_class clientsByGroup;
    mpz_powm(clientsByGroup.get_mpz_t(),group.get_mpz_t(),server_secret.get_mpz_t(),prime.get_mpz_t());
    return clientsByGroup.get_str(10);
}
/**
 * get random number, seeds by time and adding
 * @param from
 * @param to
 */
void DiffieHellmanWrapper::setServerSecret(mpz_class from, mpz_class to) {
    gmp_randclass randseed(gmp_randinit_default);
    randseed.seed(time(NULL));
    server_secret = randseed.get_z_range(from);
    to = randseed.get_z_range(to);
    __gmpz_add(server_secret.get_mpz_t(),server_secret.get_mpz_t(),to.get_mpz_t());
    if(!__gmpz_cmp_si(server_secret.get_mpz_t(),10000000L)){
        abort();
    }
    std::cout << server_secret.get_mpz_t() << "\n";
}
void DiffieHellmanWrapper::setClientExp(mpz_class &server_exp){
    client_exp = server_exp;
}
/**
 * calcs pow by module of second step of DH algorithm
*/
void DiffieHellmanWrapper::calcSharedSecret() {
    mpz_powm(shared_secret.get_mpz_t(),client_exp.get_mpz_t(),server_secret.get_mpz_t(),prime.get_mpz_t());
    std::cout << "\n" <<  shared_secret.get_mpz_t() << "\n";
    std::cout << "\n" <<  getHash() << "\n";
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
std::string DiffieHellmanWrapper::checkStringToMpz(std::string toMpz) {
    std::string sanitazedToMpz;
    for(int i =0;i < toMpz.size();i++) {
        if(isdigit(toMpz[i])){
           sanitazedToMpz.push_back(toMpz[i]);
        }
    }
    return sanitazedToMpz;
}
std::string DiffieHellmanWrapper::getPrimeNumber() {
    return prime.get_str(10);
}