package src;

/**
 * Simple printing
 */
public class Uty {

    public static boolean debug = false;

    public static void pln(Object o) {System.out.println(o);}

    public static void dbg(Object o) {if (debug) System.out.println(o);}

    public static void plnPanic(Object o) {
	pln(o);
	System.exit(-1);
    }
}
