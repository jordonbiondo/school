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
	    Uty.pln("Key Size: " + keyStream.available());
	    ArrayList<Byte> keyByteList = new ArrayList<Byte>();
	    while(keyStream.available() > 0) {
		keyByteList.add(new Byte((byte)keyStream.read()));
	    }
	    byte[] keyBytes = new byte[keyByteList.size()];
	    int i = 0;
	    for (Byte b : keyByteList) {
		keyBytes[i] = b.byteValue();
		i++;
	    }
	    keyStream.close();
	    
	    FileInputStream sigStream = new FileInputStream(args[1]);
	    Uty.pln("Sig Size: " + sigStream.available());
	    byte[] sigBytes = new byte[sigStream.available()];
	    sigStream.read(sigBytes);
	    sigStream.close();
	    
	    FileInputStream textStream = new FileInputStream(args[1]);
	    Uty.pln("TXT Size: " + textStream.available());
	    ArrayList<Byte> textByteList = new ArrayList<Byte>();
	    while(textStream.available() > 0) {
		textByteList.add(new Byte((byte)textStream.read()));
	    }
	    byte[] textBytes = new byte[textByteList.size()];
	    i = 0;
	    for (Byte b : textByteList) {
		textBytes[i] = b.byteValue();
		i++;
	    }

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
