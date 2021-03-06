from xorshift128plus import randgen
import numpy as np
from copy import deepcopy
from f2matrix import print_matrix, is_indepent, add_row, gaussian_elimination, gaussian_elimination2

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

A = (E + R18)*(E + L23) %2
B = (E + R5) %2
T = np.block([
		[O, E],
		[A, B]
	])

A = np.zeros((0, 128), dtype='int64')
V = []

M = np.zeros((0, 128), dtype='int64')

N = 32
Os = [17962250324576395206L, 5096684253032747143L, 857995788474698698L, 18235998401885193202L, 11101998926922657622L, 7067090697806163522L, 6705222410198942547L, 12445007163585810098L, 7280833661466656335L, 14299028489771582333L, 8685817599813258765L, 5256391907479113302L, 6147287965408744887L, 3987783209764114603L, 16669477412017422081L, 14045287478250320278L, 10180930710903077314L, 9668209631338609176L, 13055740679462599968L, 16945238704748735991L, 13077936350034283582L, 209155122405628693L, 7758155243950709250L, 12419371017270472947L, 17418073839329418522L, 15777905655834813376L, 15154586186832974378L, 3189428568312239083L, 17764210997104018348L, 10925946846457534699L, 7077529873849787913L, 8449319548619750588L]

NTs = []
NT = T
for i in range(N):
	vec = (NT[63] + NT[127])%2
	if is_indepent(M, vec):
		M = add_row(M, vec)
		
		A = np.r_[A, vec]
		V.append(Os[i]%2)

	NTs.append(NT)
	NT = NT * T % 2

for i in range(N):
	x = Os[i]
	for j in range(N):
		if x % 2 == 0:
			break
		vec = (NTs[i][63-j-1] + NTs[i][127-j-1]) % 2
		if is_indepent(M, vec):
			M = add_row(M, vec)

			A = np.r_[A, vec]
			V.append((x>>1)%2)

		x /= 2

invT = gaussian_elimination(T)


A = (A*invT) % 2
# fuckin hardcord!
xbits = [58, 60, 61, 62]

for v in xbits:
	vec = np.zeros((1, 128), dtype='int64')
	vec[0, v] = 1
	A = np.r_[A, vec]

oV = deepcopy(V)
oA = deepcopy(A)

print " ".join(map(str, xbits))
for x in range(1<<4):
	l = [(x>>(3-i))&1 for i in range(4)]
	print sum([b*(1<<(63-k)) for b, k in zip(l, xbits)])

	V = deepcopy(oV)
	A = deepcopy(oA)

	for b, v in zip(l, xbits):
		V.append(b)
	
	E, C, v = np.split(gaussian_elimination2(A, V), [64, 128], axis=1)

	ms1 = []
	ms2 = []
	for r in C:
		x = 0
		for i in range(32):
			x += int(r[0, i]) * (1<<(31-i))
		ms1.append(x)

		x = 0
		for i in range(32):
			x += int(r[0, 32+i] * (1<<(31-i)))
		ms2.append(x)

	# print "lsb"
	print " ".join([str(int(x)) for x in v])

print " ".join(map(str, ms1))
print " ".join(map(str, ms2))
