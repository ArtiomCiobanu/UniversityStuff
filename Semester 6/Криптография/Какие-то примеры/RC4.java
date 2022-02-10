import java.math.BigInteger;
import java.util.Scanner;

public class RC4 {
	static int[]  S =  {1, 2, 3, 4, 5, 6, 7, 8};
	public static void main(String args[])
	{
		S = new int[]{1, 2, 3, 4, 5, 6, 7, 8};
		Scanner s = new Scanner(System.in);
		String key = inputKey(s);
		String message = inputMessage(s);
		s.close();
		
		int[] keyASCII = charToASCII(key.toCharArray());
		
		showASCIIKey(keyASCII);
		showFullInitalData(keyASCII, S);
		
		S = swapping(S, keyASCII);
		
		int t = tWord(S);
		
		showBinarKey(keyASCII[t], t, keyASCII);
		
		String[] r = encrypt(message.split(""), keyASCII[t]);
		
		ShowEncryptingOfNORMandK(r, message.split(""), keyASCII[t], t);
		
		String[] m = descript(r, keyASCII[t]);
		
		ShowDescriptingOfNORMandK(r, m, keyASCII[t], t);
		
		showTheResultOfDescripting(m, key);
	}
	
	private static String inputKey(Scanner s)
	{
		String key;
		
		do {
			System.out.print("Input the key (8 numbers, without space): ");
			key = s.nextLine();
		}while(key.length()!=8);
		
		return key;
	}
	
	private static String inputMessage(Scanner s)
	{
		String message;
		
		do {
			System.out.print("Input the message (8 symbols, without space): ");
			message = s.nextLine();
		}while(message.length()!=8);

		return message;
	}
	
	private static int[] charToASCII(char... args)
	{
		int i = 0;
		int ASCII[] = new int[args.length];
		
		for(char arg : args)
		{
			ASCII[i++] = (int) arg;
		}
		return ASCII;
	}
	
	private static void showASCIIKey(int[] keyASCII)
	{
		System.out.println("\nThe key in ASCII: ");
		
		for(int i: keyASCII)
		{
			System.out.print(i + "\t");
		}
		
		System.out.print("\n\n");
	}
	
	private static void showFullInitalData(int[] keyASCII, int[] S)
	{
		System.out.print("P: \t");
		for(int i=0;i<8;i++)
		{
			System.out.print(i + "\t");
		}
		
		System.out.print("\nS: \t");
		for(int i: S)
		{
			System.out.print(i + "\t");
		}
		
		System.out.print("\nK: \t");
		for(int i: keyASCII)
		{
			System.out.print(i + "\t");
		}
		
		System.out.print("\n\n");
	}
	
	private static int[] swapping(int[] S, int[] keyASCII)
	{	
		for(int i=0, j = 0; i<8 ; i++)
		{
			showSwapping(i, j, S, keyASCII);
			
			j = (j + S[j] + keyASCII[i]) % 8;
			swap(S, i, j);
			swap(keyASCII, i, j);

			showFullInitalData(keyASCII, S);
		}
		
		return S;
	}
	
	private static void showSwapping(int i, int j, int[] S, int[] keyASCII)
	{
		int newJ = (j + S[j] + keyASCII[i]) % 8;
		
		System.out.println("\n" + (i+1) + ". j = (j + S[j] + K[i]) mod(%) 8 => " + j + " + " + S[j] + " + " 
	+ keyASCII[i] + " mod(%) 8" + " = " 
	+ (j + S[j] + keyASCII[i]) % 8);
		System.out.println("   S - Swap( " + S[i] +", " + S[newJ] + " )");
		System.out.println("   K - Swap( " + keyASCII[i] +", " + keyASCII[newJ] + " )");
	}
	
	private static void swap(int[] T, int i, int j) 
	{
		int tmp = T[i];
		T[i] = T[j];
		T[j] = tmp;
	}
	
	private static int tWord(int[] S)
	{
		int i = 0, j = 0;
		i = (i+1) % 8; 
		j = (j+S[i]) % 8;
		int t = (S[i]+S[j]) % 8;
		return t;
	}
	
	private static void showPseudoGeneration(int a, int b, int S[], int i, int j)
	{
		System.out.println("Generation of pseudo range: ");
		System.out.println("i = (i+1) mod(%) 8 => " + "( " + a + " + 1 ) mod(%) 8 = " +(a+1) % 8);
		System.out.println("j = (j+S[i]) mod(%) 8 => " + "( " + b + " + " + S[i] + " ) mod(%) 8 = " +(b+S[i]) % 8);
		System.out.println("t = (S[i]+S[j]) % 8 => " + "( " + S[i] + " + " + S[j] + " ) mod(%) 8 = " +(S[i]+S[j]) % 8);
		System.out.print("\n\n");
	}
	
	private static void showBinarKey(int binarTKey, int t, int[] keyASCII)
	{
		System.out.println("K[t] = K[" + t +"] = ("+ keyASCII[t] + ") = " +  IntToBinarString(binarTKey) + "\n\n");
	}
	
	private static String[] encrypt(String[] message, int binarTKey)
	{
		String[] r = new String[8];
		String sctbs;
		int ictbs;
		
		for(int i=0;i<8; i++)
		{
			sctbs = CharToBinarString(message[i]);
			ictbs = BinarStringToInt(sctbs);
			r[i] = BinaryStringToSChar(IntToBinarString (ictbs ^ binarTKey));
		}
		
		System.out.println("\n");
		return r;
	}
		
	private static void showEncryptingOfMessage(char messageChar, int i)
	{
		int m = (int) messageChar;
		System.out.println(i + ". m[" + (i-1) + "] = " + messageChar + " -> " + ((int) messageChar)+ " = " + 
				IntToBinarString(m));
	}
	
	private static void ShowEncryptingOfNORMandK(String[] r, String[] message, int binarTkey, int t)
	{
		for(int i=0;i<r.length;i++)
		{
			System.out.println(r[i]);
			int m = (int) message[i].charAt(0);
			String itbs = IntToBinarString (m ^ binarTkey);
			
			System.out.println((i+1) + ". r["+i+"] = m["+i+"](="+message[i]+") XOR(^) K["+t+"] =>"
					+ IntToBinarString(m) + " XOR(^) " + IntToBinarString(binarTkey) +" = " 
					+ itbs + " => " + BinarStringToInt(itbs) + " => " + BinaryStringToSChar(itbs));
		}
		
		System.out.println("\n");
	}
	
	private static String[] descript(String[] messageR, int binarTKey)
	{
		String[] m = new String[8];
		String sctbs;
		int ictbs;
		
		for(int i=0;i<8; i++)
		{
			showDesciptingOfMessage(messageR[i].charAt(0), i+1);
			sctbs = CharToBinarString(messageR[i]);
			ictbs = BinarStringToInt(sctbs);
			System.out.println(sctbs);
			//m[i] = BinaryStringToSChar(IntToBinarString (ictbs ^ binarTKey));
		}
		
		System.out.println("\n");
		return m;
	}
	
	private static void ShowDescriptingOfNORMandK(String[] r, String[] message, int binarTkey, int t)
	{
		String binarKey = IntToBinarString(binarTkey);
		
		for(int i=0;i<message.length;i++)
		{
			int rMessage = (int) r[i].charAt(0);
			String itbs = IntToBinarString (rMessage ^ binarTkey);
			
			System.out.println((i+1) + ". m["+i+"] = r["+i+"](="+r[i]+") XOR(^) K["+t+"] =>"
					+ IntToBinarString(rMessage) + " XOR(^) " + binarKey +" = " 
					+ itbs + " => " + BinarStringToInt(itbs) + " => " + BinaryStringToSChar(itbs));
		}
		
		System.out.println("\n");

	}
	
	private static void showDesciptingOfMessage(char messageChar, int i)
	{
		int r = (int) messageChar;
		System.out.println(i + ". r[" + (i-1) + "] = " + messageChar + " -> " + ((int) messageChar)+ " = " + 
				IntToBinarString(r));
	}
	
	private static String CharToBinarString(String first) {
		String binarString = new BigInteger(first.getBytes()).toString(2);
		return binarString;
	}
	
	private static int BinarStringToInt(String first) {
		return Integer.parseInt(first, 2);
	}
	
	private static String IntToBinarString(int first) {
		String binarString = Integer.toBinaryString(first);
		return binarString;
	}

	private static String BinaryStringToSChar(String first) {
		String text = new String(new BigInteger(first, 2).toByteArray());
		return text;
	}

	private static void showTheResultOfDescripting(String[] m, String key)
	{
		System.out.println("Your encrypted word is: "+concatArrayString(m));
		System.out.println("The key is: " + key);
	}
	
	private static String concatArrayString(String[] m)
	{
		String str = "";
		for(String i : m)
		{
			str += i;
		}
		
		return str;
	}
}
