using System.Text;

var input = "aclanguage_input";
Console.WriteLine($"Input string: {input}");

var inputBytes = Encoding.UTF8.GetBytes(input);

Console.WriteLine("Encrypting:");
var encrypted = Encrypt(inputBytes);

Console.WriteLine("\nResult:");
string decrypted_text = Encoding.UTF8.GetString(encrypted);
Console.WriteLine(decrypted_text);

Console.ReadKey();

byte[] Encrypt(byte[] inputBytes)
{
    uint h0 = 0x67452301;
    uint h1 = 0xEFCDAB89;
    uint h2 = 0x98BADCFE;
    uint h3 = 0x10325476;
    uint h4 = 0xC3D2E1F0;


}

