from copy import deepcopy
import numpy as np

def print_matrix(A):
	H, W = A.shape

	for i in range(H):
		l = []
		for j in range(W):
			l.append(int(A[i, j]))
		print("".join(map(str, l)))
	print "="*W

def gaussian_elimination(_A):
	H, W = _A.shape
	
	# A = deepcopy(_A)
	A = np.block([
			[_A, np.identity(H)]
		])

	# upper triangle
	for i in range(H):
		for j in range(i, H):
			if A[j, i] == 1:
				x = deepcopy(A[i])
				y = deepcopy(A[j])
				A[i] = y
				A[j] = x
				# A[i], A[j] = A[j], A[i]
				break

		for j in range(i+1, H):
			if A[j, i] == 0:
				continue
			A[j] = (A[j] - A[i]) % 2

	print_matrix(A)
	# lower triangle
	for i in range(H):
		print i
		assert A[H-i-1, H-i-1] == 1

		for j in range(H-i-1-1, -1, -1):
			if A[j, H-i-1] == 0:
				continue
			A[j] = (A[j] + A[H-i-1]) % 2

	return A

def gaussian_elimination2(_A, V):
	V = np.matrix(V, dtype="int64").T

	H, W = _A.shape
	
	# A = deepcopy(_A)
	A = np.block([
			[_A, V]
		])

	# upper triangle
	for i in range(H):
		for j in range(i, H):
			if A[j, i] == 1:
				x = deepcopy(A[i])
				y = deepcopy(A[j])
				A[i] = y
				A[j] = x
				# A[i], A[j] = A[j], A[i]
				break

		for j in range(i+1, H):
			if A[j, i] == 0:
				continue
			A[j] = (A[j] - A[i]) % 2

	# lower triangle
	for i in range(64):
		assert A[64-i-1, 64-i-1] == 1

		for j in range(64-i-1-1, -1, -1):
			if A[j, 64-i-1] == 0:
				continue
			A[j] = (A[j] + A[64-i-1]) % 2

	return A


def is_indepent(M, _l):
	# M is upper triangle!
	l = deepcopy(_l)
	o = np.zeros((1, 128), dtype='int64')
	H, W = M.shape

	ptr = 0
	for h in range(H):
		while ptr < 128 and M[h, ptr] == 0:
			if l[0, ptr] == 1:
				return True
			ptr += 1
		if ptr == 128:
			break
		if l[0, ptr] == 1:
			l[0] = (l[0] - M[h]) % 2
			if (l == o).all():
				return False
	return True

def add_row(M, _l):
	# M is upper triangle!
	l = deepcopy(_l)
	np.r_[M, l]
	A = np.zeros((0, 128), dtype='int64')
	H, W = M.shape

	ptr = 0
	inserted = False

	for r in M:
		while ptr < 128 and r[0, ptr] == 0:
			if l[0, ptr] == 1 and not inserted:
				A = np.r_[A, l]
				inserted = True
			ptr += 1
		if ptr == 128:
			break
		if not inserted and l[0, ptr] == 1:
			l[0] = (l[0] + r[0]) % 2
		A = np.r_[A, r]

	if not inserted:
		A = np.r_[A, l]
	return A



