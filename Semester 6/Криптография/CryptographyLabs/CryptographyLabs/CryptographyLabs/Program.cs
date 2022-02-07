using System.Text;

const int w = 32;
const int r = 20;
const int Pw = unchecked((int)0xb7e15163);
const int Qw = unchecked((int)0x9e3779b9);

var input = "aclanguage_input";
var key = "abcdefhg";

var inputBytes = Encoding.UTF8.GetBytes(input);
byte[] keyBytes = Encoding.UTF8.GetBytes(key);

int[] S = ExpandKeys(keyBytes);
int[] encrypted = Encrypt(inputBytes, S);
byte[] decrypted = Decrypt(encrypted, S);

//string encrypted_text = Encoding.UTF8.GetString(encrypted);
string decrypted_text = Encoding.UTF8.GetString(decrypted);
Console.WriteLine(input);

Console.ReadKey();

int[] ExpandKeys(byte[] key)
{
    int c = Math.Max(key.Length, 1) / (w / 8);
    int[] L = new int[c];
    for (int x = 0; x < c; x++)
    {
        L[x] = key[x] & 0xff;
    }
    int[] S = new int[2 * r + 4];
    S[0] = Pw;

    for (int x = 1; x < S.Length; x++)
    {
        S[x] = S[x - 1] + Qw;
    }

    int v = 3 * Math.Max(c, 2 * r + 4);

    int A = 0, B = 0, i = 0, j = 0;

    for (int s = 1; s <= v; s++)
    {
        A = S[i] = ShiftLeft(S[i] + A + B, 3);
        B = L[j] = ShiftLeft(L[j] + A + B, A + B);
        i = (i + 1) % (2 * r + 4);
        j = (j + 1) % c;
    }

    return S;
}

int[] Encrypt(byte[] inputBytes, int[] S)
{
    var input = ByteArrayToIntArray(inputBytes);

    var A = input[0];
    var B = input[1];
    var C = input[2];
    var D = input[3];

    //к четным блокам прибавляются по модулю 2N первые два 32-битовых слова ключа.
    B += S[0];
    D += S[1];

    int shift = 5; // log2 (32)

    for (int i = 1; i <= r; i++)
    {
        var t = ShiftLeft(B * (2 * B + 1), shift);
        var u = ShiftLeft(D * (2 * D + 1), shift);

        A = ShiftLeft(A ^ t, u) + S[2 * i];
        C = ShiftLeft(C ^ u, t) + S[2 * i + 1];

        var temp = A;
        A = B;
        B = C;
        C = D;
        D = temp;
    }

    // к нечетным блокам прибавляются по модулю 2N последние два 32-битовых слова ключа.
    A += S[2 * r + 2];
    C += S[2 * r + 3];

    return new[] { A, B, C, D };
}

byte[] Decrypt(int[] input, int[] S)
{
    var A = input[0];
    var B = input[1];
    var C = input[2];
    var D = input[3];

    C -= S[2 * r + 3];
    A -= S[2 * r + 2];

    int shift = 5;

    for (int i = r; i >= 1; i--)
    {
        var temp = D;
        D = C;
        C = B;
        B = A;
        A = temp;

        var u = ShiftLeft(D * (2 * D + 1), shift);
        var t = ShiftLeft(B * (2 * B + 1), shift);
        C = ShiftRight(C - S[2 * i + 1], t) ^ u;
        A = ShiftRight(A - S[2 * i], u) ^ t;

    }
    D -= S[1];
    B -= S[0];

    var outputArr = IntArrayToByteArray(new[] { A, B, C, D });

    return outputArr;
}

static byte[] IntArrayToByteArray(int[] integerArray)
{
    byte[] result = new byte[integerArray.Length * 4];

    for (int i = 0; i < result.Length; i++)
    {
        result[i] = (byte)((integerArray[i / 4] >> i % 4 * 8) & 0xff);
    }

    return result;
}

static int[] ByteArrayToIntArray(byte[] bytes)
{
    int[] result = new int[bytes.Length / 4];

    for (int i = 0, counter = 0; i < result.Length; i++)
    {
        result[i] = (bytes[counter++] & 0xff) |
                ((bytes[counter++] & 0xff) << 8) |
                ((bytes[counter++] & 0xff) << 16) |
                ((bytes[counter++] & 0xff) << 24);
    }

    return result;
}

/*static int[] ByteArrayToIntArray(byte[] bytes, int length)
{
    var resultLength = bytes.Length / 4;

    var result = new int[resultLength];
    for (int i = 0; i < resultLength; i++)
    {
        result[i] = BitConverter.ToInt32(
            new[]
            {
                bytes[i],
                bytes[i + 1],
                bytes[i + 2],
                bytes[i + 3]
            });
        result[i] = bytes[i + 3];
        result[i] += bytes[i + 2] << 8;
        result[i] += bytes[i + 1] << 16;
        result[i] += bytes[i] << 24;
    }

    return result;
}

static byte[] IntArrayToByteArray(int[] intArray)
{
    var result = new List<byte>();
    for (int i = 0; i < intArray.Length; i++)
    {
        var current = BitConverter.GetBytes(intArray[i]);
        result.AddRange(current);
    }

    return result.ToArray();
}*/

static int ShiftLeft(int val, int pas)
{
    return (val << pas) | (val >> (32 - pas));
}

static int ShiftRight(int val, int pas)
{
    return (val >> pas) | (val << (32 - pas));
}
