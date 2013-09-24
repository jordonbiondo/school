package src;
import java.util.*;
import java.io.*;
import java.security.*;
import java.security.spec.*;

// look at key factory

/**
 * Verify RSA shtuff
 */
public class VerifyRSA {
    
    /**
     * Main
     */
    public static void main(String[] args) {
	if (args.length != 3) {
	    Uty.plnPanic("USAGE: verify-rsa <key file> <signature file> <text file>");
	}
	
	try {
	    FileInputStream keyStream = new FileInputStream(args[0]);
	    byte[] keyBytes = new byte[keyStream.available()];
	    keyStream.read(keyBytes);
	    keyStream.close();
	    FileInputStream sigStream = new FileInputStream(args[1]);
	    byte[] sigBytes = new byte[sigStream.available()];
	    sigStream.read(sigBytes);
	    sigStream.close();
	    FileInputStream textStream = new FileInputStream(args[1]);
	    byte[] textBytes = new byte[textStream.available()];
	    textStream.read(textBytes);
	    textStream.close();
	    
	    X509EncodedKeySpec pubKeyEncoded = new X509EncodedKeySpec(keyBytes);
	    KeyFactory keyFactory = KeyFactory.getInstance("RSA");
	    PublicKey pubKey = keyFactory.generatePublic(pubKeyEncoded);
	    Signature sig = Signature.getInstance("SHA1withRSA");
	    sig.initVerify(pubKey);
	    sig.update(textBytes);
	    Uty.pln((sig.verify(sigBytes)) ? "OK" : "FAIL");

	} catch (Exception e) {
	    Uty.plnPanic("Error during init\n\t" + e);
	}
	
    }
}
