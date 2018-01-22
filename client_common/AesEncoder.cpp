//
// Created by Александр on 19.12.17.
//

#include <iostream>

#include <openssl/aes.h>
#include "AesEncoder.h"
#include <openssl/aes.h>
std::string& AesEncoder::encrypt(DiffieHellmanWrapper* wrapper, std::string message) {
    gmp_randclass randseed(gmp_randinit_default);
    unsigned char aes_input[message.size()];
    unsigned char aes_key[256];
    // by bytes
    memset(aes_input, 0, message.size()/8);
    memset(aes_key, 0, 32);
    //assign
    strcpy((char*) aes_input, message.c_str());
    strcpy((char*) aes_key, wrapper->getHash().c_str());

    /* init vector */
    unsigned char iv[256];
    std::string ivStr = wrapper->getIV();
    int i =0;
    std::for_each(ivStr.begin(),ivStr.end(),[&iv,&i](char ivC){
        iv[i++] = (unsigned char) ivC;
    });

    // buffers
    auto enc_out = new unsigned char();

    AES_KEY enc_key;
    AES_set_encrypt_key(aes_key, 256, &enc_key);
    AES_cbc_encrypt(aes_input, enc_out, message.size(), &enc_key, iv, AES_ENCRYPT);
    std::string encrypted_msg;
    for(int i=0;i<strlen((char*)enc_out);i++){
        encrypted_msg.push_back(enc_out[i]);
    }
    //std::cout << wrapper->getHash().c_str();
    return encrypted_msg;
}
std::string& AesEncoder::decrypt(DiffieHellmanWrapper* wrapper, std::string message){
    gmp_randclass randseed(gmp_randinit_default);
    unsigned char aes_input[message.size()];
    unsigned char aes_key[256];
    // by bytes
    memset(aes_input, 0, message.size()/8);
    memset(aes_key, 0, 32);
    //assign
    strcpy((char*) aes_input, message.c_str());
    strcpy((char*) aes_key, wrapper->getHash().c_str());

    /* init vector */
    unsigned char iv[256];
    std::string ivStr = wrapper->getIV();
    int i =0;
    std::for_each(ivStr.begin(),ivStr.end(),[&iv,&i](char ivC){
      iv[i++] = (unsigned char) ivC;
    });

    // buffers
    auto enc_out = new unsigned char();

    AES_KEY enc_key;
    AES_set_decrypt_key(aes_key, 256, &enc_key);
    AES_cbc_encrypt(aes_input, enc_out, message.size(), &enc_key, iv, AES_DECRYPT);
    std::string encrypted_msg;
    for(int i=0;i<strlen((char*)enc_out);i++){
        encrypted_msg.push_back(enc_out[i]);
    }
    return encrypted_msg;
}