#!/bin/bash

#clean
rm -f foo.txt foobad.txt my_priv.key my_pub.der my_pub.key my_sig.txt
#Create file

echo "$@"	>> foo.txt
echo "$@" "bad" >> foobad.txt

#create private key
openssl genrsa -out my_priv.key 

#generate public key
openssl rsa -pubout -in my_priv.key -out my_pub.key 

#generate DER key from pub
openssl rsa -outform DER -pubin -in my_pub.key -out my_pub.der

# create signature from foo.txt with private key
cat foo.txt | openssl dgst -sha1 -sign my_priv.key -out my_sig.txt

#test verify
cat foo.txt | openssl dgst -sha1 -verify my_pub.key -signature my_sig.txt 
echo ""
# verify
echo "signature created for \"""$@""\""
echo -n verifying "$@""... "
./verify-rsa my_pub.der my_sig.txt foo.txt 
echo -n verifying "$@""bad... "
./verify-rsa my_pub.der my_sig.txt foobad.txt 
