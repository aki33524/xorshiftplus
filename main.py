from xorshift128plus import randgen
import numpy as np
from copy import deepcopy
from f2matrix import print_matrix, is_indepent, add_row, gaussian_elimination

s = [1145141919810, 893889464]
rand = randgen(s)

# for i in range(10):
# 	print(next(rand))

E = np.identity(64, dtype='int64')
O = np.matrix(np.zeros((64, 64)), dtype='int64')

def make_shift_matrix(x):
	# right shift
	# allowed to pass negative number

	# A = np.copy(O) #bug!
	A = deepcopy(O)

	i = x
	j = 0
	while i < 64 and j < 64:
		if 0<=i and 0<=j:
			A[i, j] = 1
		i += 1
		j += 1
	return A

L23 = make_shift_matrix(-23)
R18 = make_shift_matrix(18)
R5 = make_shift_matrix(5)


# s[0] = s0
# s1 ^= (s1 << 23) & ((1<<64) - 1)
# s[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5)
# yield (s[1] + s0) & ((1<<64) - 1)

# s1    = s[0]
# s0    = s[1]
# ns[0] = s[1]
# s1    = s[0] ^ (s[0] << 23)
#       = E*s[0] + L23*s[0]
#       = (E + L23)*s[0]
# ns[1] = s1 ^ s[1] ^ R18*s1 ^ R5*s[1]
#       = E*s1 + R18*s1 + E*s[1] + R5*s[1]
#       = (E + R18)*s1 + (E + R5)*s[1]
#       = (E + R18)*(E + L23)*s[0] + (E + R5)*s[1]

# ns[0] = O*s[0] + E*s[1]
# ns[1] = (E + R18)*(E + L23)*s[0] + (E + R5)*s[1]

# print_matrix(R5)
# print_matrix(R18)
# print_matrix(L23)

A = (E + R18)*(E + L23) %2
B = (E + R5) %2
T = np.block([
		[O, E],
		[A, B]
	])

def long_to_state(s):
	l = []
	
	for j in range(2):
		for i in range(64):
			l.append((s[j]>>(64-i-1)) & 1)

	return np.matrix(l, dtype='int64').T

def state_to_long(vec):
	x = 0
	for i in range(64):
		x += int(vec[i, 0]) * (1<<(64-1-i))

	y = 0
	for i in range(64):
		y += int(vec[i+64, 0]) * (1<<(64-1-i))

	return [x, y]

# ns = T * long_to_state(s) % 2
# print sum(state_to_long(ns)) & ((1<<64) - 1), next(rand)
# vs = long_to_state(s)

M = np.zeros((0, 128), dtype='int64')
A = np.zeros((0, 128), dtype='int64')
LSBS = np.zeros((128, 1), dtype='int64')

NT = T
for i in range(128):
	x = next(rand)
	y = sum(state_to_long(NT*long_to_state(s)%2)) & ((1<<64) - 1)
	assert x == y

	vec = (NT[63] + NT[127])%2
	if is_indepent(M, vec):
		M = add_row(M, vec)
		A = np.r_[A, vec]
		LSBS[i, 0] = x % 2

		# print_matrix(M)
	NT = NT * T % 2


# print_matrix(A)
# print_matrix(M)

LSBS2 = A * long_to_state(s) % 2
for i in range(128):
	assert LSBS[i, 0] == LSBS2[i, 0]

A = gaussian_elimination(A)
# print_matrix(A)

invA = np.hsplit(A, 2)[1]
print_matrix(invA)

print state_to_long(invA * LSBS % 2)
print state_to_long(long_to_state(s))

# LT = np.zeros((X, 128))
# for i in range(128):
# 	LT[i, i] = 1

# M = np.block([
# 		[LSBM, LT],
# 	])

# print_matrix(M)

# M = gaussian_elimination(M)
# print_matrix(M)

