def FunctionWithoutParamerers():
    print("\nThis is a function without parameters")

def FunctionWith2Paramerers(a, b):
    print(f"\nThis is a function with 2 parameters. Their values: {a} and {b}")

def FunctionWithDefaultParameterValue(parameter = False):
    print(f"\nThis is a function with default parameter value: {parameter}")

def FunctionWithReturnValue():
    return 5


FunctionWithoutParamerers()
FunctionWith2Paramerers(1, 2)

FunctionWithDefaultParameterValue()
FunctionWithDefaultParameterValue(True)

print(f"\nReturn value of a funtion: {FunctionWithReturnValue()}")

incrementValue = lambda x: x+1
print(f"\nLambda function: {incrementValue(5)}")

import SomeModeule
SomeModeule.FunctionFromModule()

print("\nEnter n: ")
n = int(input())
if n > 0:
    print("n > 0")
elif n < 0:
    print("n < 0")
else:
    print("n = 0")

print("\nset:")
numberSet = {1, 2, 4, -1, 0}
for number in numberSet:
    print(str(number))

print("\nlist:")
numberList = [1, 2, 3, -9]
for number in numberList:
    print(str(number))

print("\ndictionary keys:")
facultyDictionary = {
    "Maths": 50,
    "Economics": 20,
    "Physics": 30,
    "Psychology": 23
}
for faculty in facultyDictionary:
    print(faculty)

print("\ndictionary values:")
for faculty in facultyDictionary.values():
    print(faculty)

print("\nWhile counter:")
i = 0
while i < 10:
    print(f"i = {i}")
    i += 1