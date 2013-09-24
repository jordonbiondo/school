package src;
public class Uty {

    public static void pln(Object o) {System.out.println(o);}

    public static void plnPanic(Object o) {
	pln(o);
	System.exit(-1);
    }
}
