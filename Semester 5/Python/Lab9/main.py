import calendar
import time
from datetime import date, datetime

now = datetime.now()
print(f"Here's the current date and time: {now}\n")

year = now.year
month = now.month
day = now.day
print(f"Current year: {year}")
print(f"Current month: {month}")
print(f"Current day: {day}")

someDate = date(year, 12, 31)
print(f"\nNew year's date: {someDate}")

if now == someDate:
    print("Happy new Year!")
else:
    print("Days until new Year:", (someDate - now.date()).days)

print(f"\nHere's the calendar for {year}")

cal = calendar.TextCalendar(calendar.SUNDAY)
year = cal.formatyear(2021)
print(year)

print(time.time())
print(time.strftime("%B %d, %Y"))
print(time.asctime())

dateOfBirthday = time.strptime(input("Enter your birthday: "), "%d.%m.%Y")
day = date.today() - date.fromtimestamp(time.mktime(dateOfBirthday))
age = day / 365.25
print("Your age:", age.days)

time.sleep(1)
