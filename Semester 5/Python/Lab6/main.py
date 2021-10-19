import datetime

from department_module import SalaryEmployee, HourlyEmployee

salary_employee = SalaryEmployee("Artiom",
                                 76743098,
                                 datetime.datetime(2000, 7, 27),
                                 "aciobanumain@gmail.com",
                                 ".Net Developer",
                                 9000)

salary = salary_employee.calculate_salary()

hourly_employee = HourlyEmployee("Alex",
                                 87293876,
                                 datetime.datetime(1990, 2, 3),
                                 "alex@gmail.com",
                                 "profesor",
                                 2000,
                                 20)

hourly_salary = hourly_employee.calculate_salary()

print(salary_employee.name)
salary_employee.name = "aaaaaaaaaaaaaaaaa"
print(salary_employee.name)
#del salary_employee

# del dep1.calculate_age

##print(dep1.__name)
