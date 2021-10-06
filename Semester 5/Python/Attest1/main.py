from os.path import isfile


def read_dishes_from_file(file_name):
    dishes_from_file = dict()

    if not isfile(file_name):
        dishes_file = open(file_name, "r+")
        dishes_file.close()
    else:
        dishes_file = open(file_name, "r")

    file_lines = dishes_file.readlines()
    for line in file_lines:
        words = line.split()

        dish_name = words.pop(0)
        dish_ingredients = []

        for i in words:
            dish_ingredients.append(i)

        dishes_from_file[dish_name] = dish_ingredients

    dishes_file.close()

    return dishes_from_file


def write_dishes_to_file(file_name, dishes_to_write):
    dishes_file = open(file_name, "w+")

    lines = []

    for dish_name in dishes_to_write.keys():
        current_dish = dishes_to_write[dish_name]

        line = dish_name
        for i in current_dish:
            line += f" {i}"
        lines.append(line + "\n")

    dishes_file.writelines(lines)
    dishes_file.close()


def input_with_length(min_length, max_length, return_type):
    while 1:
        input_string = input()

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


dishes = read_dishes_from_file("items.txt")

print("Initial Dishes: ")
for dish in dishes.keys():
    ingredients = dishes[dish]
    print(f"{dish} : {ingredients}")

actionNumber = 0
while actionNumber != 3:
    print("\nChoose an option:")
    print("1 - Find an ingredient in a dish")
    print("2 - Add an ingredient")
    print("3 - Exit")

    actionNumber = input_with_length(1, 50, int)

    if actionNumber == 1:
        print("Enter the ingredient name: ")
        ingredientName = input_with_length(3, 15, str)

        foundDishes = dict()
        for dish in dishes.keys():
            ingredients = dishes[dish]

            for ingredient in ingredients:
                if ingredient.upper() == ingredientName.upper():
                    foundDishes[dish] = dishes[dish]

        foundDishAmount = len(foundDishes)
        if foundDishAmount > 0:
            dishText = "dishes" if foundDishAmount > 1 else "dish"

            print(f"The ingredient was found in {foundDishAmount} {dishText}:")
            for dishName in foundDishes.keys():
                print(f"{dishName}. Ingredients: {foundDishes[dishName]}")
        else:
            print("No dish with this ingredient was found")

    elif actionNumber == 2:
        print("Enter the dish to add the ingredient to:")
        dishName = input_with_length(3, 10, str)

        if dishName not in dishes.keys():
            dishes[dishName] = []

        print("Enter the ingredient to add to the entered dish:")
        ingredientName = input_with_length(3, 15, str)

        dishes[dishName].append(ingredientName)

    elif actionNumber == 3:
        print("Writing the dishes into the file...")

        write_dishes_to_file("items.txt", dishes)

        print("Exiting...")
        break
    else:
        print("Please select a correct option:")
