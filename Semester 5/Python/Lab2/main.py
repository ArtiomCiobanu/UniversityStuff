#string = 'String!'
# print(f'Hello! here is a string variable: {string}. The type: {type(string)}. The length: {len(string)}')

# print(f'\nThe previous text in uppercase: {string.upper()}')

# print(f'\nLets take a character: {string[5]}')

# print(f'\nLets take a character from the end: {string[-2]}')

# print(f'\nLets take a substring: {string[4:]}')
#print(f'\nLets take anoter substring: {string[:2:-2]}') #Evetything before
#
# number = 5
# print(f'\nIt is a number: {number}. Its type is {type(number)}')
#
# number = string
# print(f'\nLet us change the type of the number variable: {number}')
#
# floatNumber = 5.5
# print(f'\nFloating-point number: {floatNumber}')
#
# character = 'a'
# print(f'\nHere is the \'a\' character: {character}')


# def print_hi(name):
    # Use a breakpoint in the code line below to debug your script.
    # print(f'Hi, {name}')  # Press Ctrl+F8 to toggle the breakpoint.


# Press the green button in the gutter to run the script.
# if __name__ == '__main__':
#     print_hi('PyCharm')

txt = "More results from text..."
substr = txt[4:6] #Substring made of characters from 4th to 6th
print(substr)
print(substr.strip())

print("    asdiuahsd asdasd   ".strip()) #Truncates the string

txt = "More results from text..."
print(txt.split())


# stri = "abcdefgh12345678"
# print(stri[5:]) #From 5 character till the end
# print(stri[5:-2]) #From 5 to 2 from the end
# print(stri[-5:])#From 5 from the end to the end
# print(stri[:10:3])#Skips 3 in substring from begin to 10 character
# print(stri[:10])#First 10 characters

names = " Cathy,Erika"

print(len(names[1:3]))