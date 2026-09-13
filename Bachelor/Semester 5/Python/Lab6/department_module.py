from datetime import date


def print_salary(func):
    def wrapper(*args, **kwargs):
        result = func(*args, **kwargs)
        print(result)
        return result

    return wrapper


class Department:
    def __init__(self, name):
        self.__name = name
        self.__employee_list = []

    def get_employee_list(self):
        return self.__employee_list

    def add_employee(self, employee):
        self.__employee_list.append(employee)

    def remove_employee(self, employee):
        self.__employee_list.remove(employee)


class Employee:
    def __init__(self, name, phone, birthday, email, position):
        self._name = name
        self._phone = phone
        self._birthday = birthday
        self._email = email
        self._position = position

    def calculate_age(self):
        current_date = date.today()

        difference = current_date - self._birthday

        return difference.year

    def calculate_salary(self):
        pass

    @property
    def phone(self):
        return self._phone

    @phone.setter
    def phone(self, phone):
        self._phone = phone

    @phone.deleter
    def phone(self):
        del self._phone

    # name property
    def getname(self):
        return self._name

    def setname(self, name):
        self._name = name

    def delname(self):
        del self._name

    name = property(getname, setname, delname)


class HourlyEmployee(Employee):
    def __init__(self, name, phone, birthday, email, position, hour_amount, hourly_pay):
        super().__init__(name, phone, birthday, email, position)

        self.__hour_amount = hour_amount
        self.__hourly_pay = hourly_pay

    @print_salary
    def calculate_salary(self):
        return self.__hourly_pay * self.__hour_amount


class SalaryEmployee(Employee):
    def __init__(self, name, phone, birthday, email, position, salary):
        super().__init__(name, phone, birthday, email, position)

        self.__salary = salary

    @print_salary
    def calculate_salary(self):
        return self.__salary