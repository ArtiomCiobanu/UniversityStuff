# string = 'String!'
# print(f'Hello! here is a string variable: {string}. The type: {type(string)}. The length: {len(string)}')
#
# print(f'\nThe previous text in uppercase: {string.upper()}')
#
# print(f'\nLets take a character: {string[5]}')
#
# print(f'\nLets take a character from the end: {string[-2]}')
#
# print(f'\nLets take a substring: {string[4:]}')
# print(f'\nLets take anoter substring: {string[:2:-2]}')
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


def print_hi(name):
    # Use a breakpoint in the code line below to debug your script.
    print(f'Hi, {name}')  # Press Ctrl+F8 to toggle the breakpoint.


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    print_hi('PyCharm')

txt = "More results from text..."
substr = txt[4:12]
print(substr)
print(substr.strip())
