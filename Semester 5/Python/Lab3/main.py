# Creating a list
facultyList = ['Maths', 'Physics', 'Biology', 'Psychology', 'Economics']
facultyList[4] = 'Sociology'

print(f'The entire faculty list: {facultyList}')
print(f'The first item: {facultyList[0]}')
print(f'The third item: {facultyList[2]}')

facultyTuple = ('Maths', 'Physics', 'Biology', 'Psychology', 'Economics')
# facultyTuple[4] = 'aaa' #We can't change any item in the tuple
print(f'\nThe entire tuple: {facultyTuple}')
print(f'Tuple\'s type: {type(facultyTuple)}')
print(f'The first element in tuple: {facultyTuple[0]}')
print(f'The last element in tuple: {facultyTuple[-1]}')

facultySet = {'Maths', 'Physics', 'Biology', 'Psychology'}
facultySet.add('Economics')
facultySet.add('Economics')  # This is a duplicate, and it's not added
# facultySet[0] = 'aaa' #We can't change any item in the set
print(f'\nThe entire set: {facultyTuple}')
print(f'Set\'s type: {type(facultyTuple)}')
print(f'The first element in set: {facultyTuple[0]}')
print(f'The last element in set: {facultyTuple[-1]}')
print('Let\'s print all the items of this set:')
for item in facultySet:
    print(item)

facultyDictionary = {
    "Maths": 50,
    "Economics": 20,
    "Physics": 30,
    "Psychology": 23
}

print(f'\nThe whole dictionary: {facultyDictionary}')
print(f'Maths value: {facultyDictionary["Maths"]}')
print(f'Physics value: {facultyDictionary["Physics"]}')

