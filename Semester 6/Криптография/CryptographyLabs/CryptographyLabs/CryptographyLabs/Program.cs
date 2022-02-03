
const int w = 32;
const int r = 20;
const int Pw = unchecked((int)0xb7e15163);
const int Qw = unchecked((int)0x9e3779b9);

String given_text;
String[] input_text_val;
BufferedWriter output_to_text_file = null;

try
{
    FileReader input_file = new FileReader("input.txt");
    FileWriter output_file = new FileWriter("output.txt", false);
    BufferedReader bf = new BufferedReader(input_file);

    given_text = bf.readLine();
    input_text_val = given_text.split(":");
    byte[] myWord = input_text_val[1].getBytes();

    given_text = bf.readLine();
    String[] input_key_val = given_text.split(":");
    byte[] key = input_key_val[1].getBytes();

    int[] S = new AlgorithmRC6().KeySchedule(key);
    byte[] encrypt = new AlgorithmRC6().encryption(myWord, S);
    byte[] decrypt = new AlgorithmRC6().decryption(encrypt, S);

    String encrypted_text = new String(encrypt, StandardCharsets.UTF_8);
    String decrypted_text = new String(decrypt, StandardCharsets.UTF_8);
    output_to_text_file = new BufferedWriter(output_file);
    output_to_text_file.write("ciphertext: " + encrypted_text +
            "\nplaintext: " + decrypted_text);

}
catch (Exception e)
{
    Console.WriteLine(e.Message);
}

int[] ExpandKeys(byte[] key)
{
    int c = Math.Max(key.Length, 1) / (w / 8);
    int[] L = new int[c];
    for (int i = 0; i < c; i++)
    {
        L[i] = key[i] & 0xff;
    }

    int[] keys = new int[2 * r + 4];
    keys[0] = Pw; // первый элемент S = b7e15163

    for (int i = 1; i < keys.Length; i++)
    {
        keys[i] = keys[i - 1] + Qw;
    }

    int A = 0, B = 0, ii = 0, j = 0;

    int v = 3 * Math.Max(c, 2 * r + 4);

    for (int s = 0; s < v; s++)
    {
        A = keys[ii] = shiftLeft((keys[ii] + A + B), 3);
        B = L[j] = shiftLeft(L[j] + A + B, A + B);
        ii = (ii + 1) % (2 * r + 4);
        j = (j + 1) % c;
    }
    return keys;
}

byte[] encryption(byte[] myWord, int[] S)
{

    int temp, t, u;
    int[] temp_data = new int[myWord.length / 4];

    temp_data = convertByteToInt(myWord, temp_data.length);

    int A, B, C, D;
    A = temp_data[0];
    B = temp_data[1];
    C = temp_data[2];
    D = temp_data[3];

    //к четным блокам прибавляются по модулю 2N первые два 32-битовых слова ключа.
    B = B + S[0];
    D = D + S[1];

    int shift = 5; // log2 (32)

    byte[] outputArr;
    for (int i = 1; i <= r; i++)
    {
        // операция преобразования T(X)=(X*(2*X+1)) mod 2N и
        // циклический сдиг влево на 5 бит.
        t = shiftLeft(B * (2 * B + 1), shift);
        u = shiftLeft(D * (2 * D + 1), shift);

        //XOR и циклический сдвиг влево на количество бит,
        // хранимое после преобразования в четных блоках.
        // Заключительная операция в цикле - сложение по модулю 2N
        // с (2*i)-м и (2*i+1)-м 32-битовыми словами ключа.
        A = shiftLeft(A ^ t, u) + S[2 * i];
        C = shiftLeft(C ^ u, t) + S[2 * i + 1];

        temp = A;
        A = B;
        B = C;
        C = D;
        D = temp;
    }

    // к нечетным блокам прибавляются по модулю 2N последние два 32-битовых слова ключа.
    A += S[2 * r + 2];
    C += S[2 * r + 3];

    temp_data[0] = A;
    temp_data[1] = B;
    temp_data[2] = C;
    temp_data[3] = D;

    outputArr = convertIntToByte(temp_data, myWord.Length);

    return outputArr;
}

byte[] Decryption(byte[] myWord, int[] S)
{
    int temp, t, u;
    int A, B, C, D;

    int[] temp_data_decryption = new int[myWord.Length / 4];

    temp_data_decryption = convertByteToInt(myWord, temp_data_decryption.Length);

    A = temp_data_decryption[0];
    B = temp_data_decryption[1];
    C = temp_data_decryption[2];
    D = temp_data_decryption[3];

    C -= S[2 * r + 3];
    A -= S[2 * r + 2];

    int shift = 5;

    for (int i = r; i >= 1; i--)
    {
        temp = D;
        D = C;
        C = B;
        B = A;
        A = temp;

        u = shiftLeft(D * (2 * D + 1), shift);
        t = shiftLeft(B * (2 * B + 1), shift);
        C = shiftRight(C - S[2 * i + 1], t) ^ u;
        A = shiftRight(A - S[2 * i], u) ^ t;

    }
    D -= S[1];
    B -= S[0];

    temp_data_decryption[0] = A;
    temp_data_decryption[1] = B;
    temp_data_decryption[2] = C;
    temp_data_decryption[3] = D;

    var outputArr = convertIntToByte(temp_data_decryption, myWord.Length);

    return outputArr;
}

static byte[] convertIntToByte(int[] integerArray, int length)
{
    byte[] int_to_byte = new byte[length];

    for (int i = 0; i < length; i++)
    {
        int_to_byte[i] = (byte)((integerArray[i / 4] >> (i % 4) * 8) & 0xff);
    }

    return int_to_byte;
}

static int[] convertByteToInt(byte[] arr, int length)
{
    int[] byte_to_int = new int[length];

    for (int i = 0, counter = 0; i < byte_to_int.Length; i++)
    {
        byte_to_int[i] = ((arr[counter++] & 0xff)) |
                ((arr[counter++] & 0xff) << 8) |
                ((arr[counter++] & 0xff) << 16) |
                ((arr[counter++] & 0xff) << 24);
    }

    return byte_to_int;
}

static int shiftLeft(int val, int pas)
{
    return (val << pas) | (val >> (32 - pas));
}

static int shiftRight(int val, int pas)
{
    return (val >> pas) | (val << (32 - pas));
}
