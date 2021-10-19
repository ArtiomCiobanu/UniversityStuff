import re
from os.path import isfile


def read_from_file(file_name):
    file_data = dict()

    if not isfile(file_name):
        dishes_file = open(file_name, "w+")
        dishes_file.close()

    dishes_file = open(file_name, "r")

    file_lines = dishes_file.readlines()
    for line in file_lines:
        words = line.strip().split()

        full_name = words.pop(0) + " " + words.pop(0)

        kid_amount = int(words.pop(0))

        file_data[full_name] = kid_amount

    dishes_file.close()

    return file_data


def write_to_file(file_name, data_to_write):
    dishes_file = open(file_name, "w+")

    lines = []

    for full_name in data_to_write.keys():
        kid_amount = data_to_write[full_name]

        lines.append(f"{full_name} {kid_amount} \n")

    dishes_file.writelines(lines)
    dishes_file.close()


def input_with_length(min_length, max_length, return_type):
    while 1:
        input_string = input().strip()

        try:
            input_value = return_type(input_string)
        except ValueError:
            print(f"Please enter the value of type: {return_type}.")
            continue

        input_length = len(input_string)
        if min_length <= input_length <= max_length:
            return input_value
        else:
            print(f"Please enter a value with length between {min_length} and {max_length} characters: ")


def input_only_characters(min_length, max_length):
    pattern = re.compile("^[a-zA-z]*$")

    while 1:
        input_string = input_with_length(min_length, max_length, str)

        if pattern.match(input_string):
            return input_string
        else:
            print("Please enter only character string:")


data = read_from_file("items.txt")


def create_record_from_keyboard():
    print("Enter the first name: ")
    first_name = input_only_characters(1, 50)

    print("Enter the last name: ")
    last_name = input_only_characters(1, 50)

    print("Enter the amount of kids: ")
    kid_amount = input_with_length(1, 1, int)

    full_name = f"{first_name} {last_name}"
    data[full_name] = kid_amount


def print_records():
    for full_name in data.keys():
        kid_amount = data[full_name]
        print(f"{full_name} : {kid_amount}")


def count_children():
    amount = 0
    for employee_name in data.keys():
        amount += data[employee_name]

    return amount


print("Initial Data: ")
print_records()

actionNumber = 0
while actionNumber != 4:
    print("\nChoose an option:")
    print("1 - Print all records")
    print("2 - Add a record")
    print("3 - Count all children")
    print("4 - Exit")

    actionNumber = input_with_length(1, 50, int)

    if actionNumber == 1:
        print("\nAll the records: ")
        print_records()

    elif actionNumber == 2:
        create_record_from_keyboard()

    elif actionNumber == 3:
        childrenAmount = count_children()
        print(f"Children amount: {childrenAmount}")

    elif actionNumber == 4:
        print("Writing data into the file...")

        write_to_file("items.txt", data)

        print("Exiting...")
        break
else:
    print("Please select a correct option:")
