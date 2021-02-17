
num_rows = 32
num_cols = 32

r = 0
c = 2
rx = 29
cx = 31

r_mask = (~rx & num_rows-1)
c_mask = (~cx & num_cols-1)

r_f = r & r_mask
c_f = c & c_mask


for row_index in range(num_rows):
	for col_index in range(num_cols-1, -1, -1):
		if ((row_index & r_mask) == r_f) and ((col_index & c_mask) == c_f):
			print("1 ", end='')
		else:
			print("0 ", end='')
	print()