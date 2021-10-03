itemList = []

def add(item):
    if item not in itemList:
        itemList.append(item)


def remove(itemToAdd):
    for item in itemList:
        if item == itemToAdd:
            itemList.remove(item)
            break


def print_items():
    for item in itemList:
        print(item)


print("Select one of the following options: ")
print("0 - exit")
print("1 - add an item")
print("2 - remove an item")
print("3 - print the list")

actionNumber = -1
while actionNumber != 0:
    print("\nenter an option:")
    actionNumber = int(input())
    if actionNumber > 3 or actionNumber < 0:
        print("Please select a correct option.")
    else:
        print(f"The selected option is: {actionNumber}")

    if actionNumber == 0:
        print("Exiting...")
        break
    if actionNumber == 1:
        print("Enter an item to add to the list: ")
        item = input()
        add(item)
    elif actionNumber == 2:
        print("Enter an item to remove from the list: ")
        item = input()
        remove(item)
    elif actionNumber == 3:
        print_items()

