//Include statements
#include <iostream>
#include <string>
#include <ctime>

//Namespace declaration
using namespace std;

//Functions

//Exponential number getter - pass in base and exponent
int exponent(int base, int exp)
{
	if (exp < 0)
	{
		return 0; //Return no value
	}
	else if (exp == 0) //If exponent is 0
	{
		return 1; //Return 1
	}
	else if (exp == 1)
	{
		return base; //Return the base
	}
	
	return base * exponent(base, exp - 1); //Return the number recursively
}

//Keygenerator = pass in 16 bit base, key storge array
void keygen(int d16, int keys[3])
{
	srand(time(NULL));

	int diff; //Difference between current and base
	int key_bin32[31]; //Key in binary
	key_bin32[30] = 1; //Set final digit = 1 to reach 32 bit value

	for (int i = 0; i < 3; i++)
	{
		cout << endl;

		for (int j = 0; j < 30; j++) //Generate random bin number of 64 bits
		{
			key_bin32[j] = rand() % 2; // Generate random bin value
		}

		cout << "Key in Binary: 0";
		for (int j = 30; j >= 0; j--)
		{
			cout << key_bin32[j];
		}
		cout << endl;

		//Convert to decimal
		for (int j = 0; j < 31; j++)
		{
			if (key_bin32[j] != 0) //If a 1 exists
			{
				keys[i] += 1 * exponent(2, j); //Calculate value there
			}
		}

		cout << "32 bit key before adjustment: " << keys[i] << endl;

		diff = keys[i] % d16;

		while (keys[i] % d16 != 0) //While not divisible by the base
		{
			if (keys[i] < 1073741824 + diff) //If number - diff will get an undesirable number
			{
				keys[i] += diff; //Add diff
			}
			else
			{
				keys[i] -=diff; //Subtract diff
			}
		}

		cout << "Key " << i + 1 << ": " << keys[i] << endl;
	}

	cout << endl;
}

//Encrypter - pass in string to encrypt, and base
string encrypt(string pt, int b16)
{
	string cipText; //Ciphertext storage
	string basekey = to_string(b16); //Base key in string format
	char convert = 0; //Character to convert
	int stringSize = pt.size(); //Get size of string
	int ascii = 0; //ASCII value storage
	int digit = 0; //Digit used to determine what to multiply ascii value by

	for (int i = 0; i < stringSize; i++) //Encrypt the string
	{
		convert = pt[i]; //Get character
		ascii = (int)convert; //Convert ascii value to integer

		digit = basekey[i % 5] - 48; //Get digit

		ascii += ((15 * digit) % 127); //encrypt character based on current value + key value * 15, in a circular motion
		cipText += ascii; //Add encrypted character to the cipher text string
	}

	return cipText; //Return the encrypted string
}

//Decrypter - pass in string to decrypt, and base
string decrypt(string ct, int b16, int key)
{
	string plainText; //Ciphertext storage
	string basekey = to_string(b16); //Base key in string format
	char convert = 0; //Character to convert
	int stringSize = ct.size(); //Get size of string
	int ascii = 0; //ASCII value storage
	int digit = 0; //Digit used to determine what to multiply ascii value by

	if (key % b16 == 0) //If key is correct
	{
		for (int i = 0; i < stringSize; i++) //Decrypt the string
		{
			convert = ct[i]; //Get character
			ascii = (int)convert; //Convert ascii value to integer

			digit = basekey[i % 5] - 48; //Get digit

			ascii -= ((15 * digit) % 127); //Decrypt character based on key value * 15, in a circular motion
			plainText += ascii; //Add Decrypted character to the cipher text string
		}

		return plainText; //Return the decrypted string
	}
	else //If key is incorrect
	{
		cout << "Incorrect decryption key." << endl;
	}
}

int main()
{
	//Random number generation seed
	srand(time(NULL));

	int base16 = 0; //16 bit base
	int keys[3] = { 0, 0, 0 }; //Key database
	int diff; //Difference for 16 bit number to divisible by 7
	string plainText = "This string of text is to be encrypted and decrypted."; //plaintext storage
	string cipherText = ""; //Ciphertext storage
	string decrypted[3] = { "", "", "" }; //Decrypted text storage

	int base16_bin[16]; //Binary 16 bit key
	base16_bin[15] = 1; //Initialize last value to 1 to get into 16 bit range

	//Generate key
	for (int i = 0; i < 15; i++)
	{
		base16_bin[i] = rand() % 2; // Generate random bin value
	}

	//Convert to decimal
	for (int i = 0; i < 16; i++)
	{
		if (base16_bin[i] != 0) //If a 1 exists
		{
			base16 += 1 * exponent(2, i); //Calculate value there
		}
	}

	cout << "Base in Binary: ";
	for (int i = 15; i >= 0; i--)
	{
		cout << base16_bin[i];
	}
	cout << endl;
	cout << "Base before adjustment: " << base16 << endl;

	//Make divisible by 7
	while (base16 % 7 != 0)
	{
		diff = base16 % 7; //Get diff

		if (base16 < 32775) //If current base number - 7 would give you a number below 32768
		{
			base16 += diff; //Add difference
		}
		else //If current base number - 7 would not give you a number below 32768
		{
			base16 -=diff; //Subtract difference
		}
	}

	cout << "Base: " << base16 << endl;

	keygen(base16, keys); //Generate keys

	cout << "Plaintext: \"" << plainText << "\"" << endl;

	cipherText = encrypt(plainText, base16); //Encrypt

	cout << "Encrypted text: " << cipherText << endl << endl;

	for (int i = 0; i < 3; i++)
	{
		cout << "Decrypting using key #" << i + 1 << " (" << keys[i] << ")" << endl;

		decrypted[i] = decrypt(cipherText, base16, keys[i]); //Decrypt

		cout << "Decrypted text: \"" << decrypted[i] << "\"" << endl;

		if (decrypted[i] == plainText)
		{
			cout << "Encryption and decryption processed successfully." << endl;
		}
		else
		{
			cout << "An error has occurred." << endl;
		}

		cout << endl;
	}

	system("pause");
	return 0;
}