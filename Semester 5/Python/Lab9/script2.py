import calendar

myCalendar = calendar.Calendar()
myCalendar.setfirstweekday(calendar.SUNDAY)

print(myCalendar.firstweekday)

selectedYear = 2003
getLeapText = lambda year: "leap" if calendar.isleap(year) else "not leap"
print(f"The year {selectedYear} is {getLeapText(selectedYear)}")

