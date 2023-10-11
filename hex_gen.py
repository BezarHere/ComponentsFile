import string
b, e = ord('0'), ord('f')
for i in range(b, e+1):
	c = chr(i)
	if c in string.digits or c.lower() in 'abcdef':
		print('\n')
	else:
		print('0\n')
