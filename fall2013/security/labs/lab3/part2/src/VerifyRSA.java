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
	    if (args.length == 4 && args[4] == "-debug") Uty.debug = true;
	    else Uty.plnPanic("USAGE: verify-rsa <key file> <signature file> <text file>");
	}
	
	try {
	    byte[] keyBytes = readAll(args[0]);
	    byte[] sigBytes = readAll(args[1]);
	    byte[] textBytes = readAll(args[2]);
	    
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

    /**
     * Read all the bytes from a file
     */
    public static byte[] readAll(String filename) throws Exception{
	FileInputStream streamy = new FileInputStream(filename);
	Uty.dbg("byte size of " + filename + ": " + streamy.available());
	byte[] bytes = new byte[streamy.available()];
	streamy.read(bytes);
	streamy.close();
	return bytes;
    }
}
