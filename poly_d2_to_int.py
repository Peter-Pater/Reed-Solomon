result = ['0'] * 17
inputValue = input()
inputValue = inputValue.split(" + ")
for i, value in enumerate(inputValue):
	if "x^" in value:
		inputValue[i] = inputValue[i][2:]
		inputValue[i] = int(inputValue[i])
	elif value == 'x':
		inputValue[i] = 1
	elif value == '1':
		inputValue[i] = 0
	result[len(result) - inputValue[i] - 1] = '1'
print(int(''.join(result), 2))
