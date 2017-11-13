import struct, os
from copy import deepcopy

def randgen(_s = []):
	s = deepcopy(_s)

	if s == []:
		for i in range(2):
			s.append( struct.unpack("<Q",os.urandom(8))[0] )

	assert len(s) == 2

	while True:
		s1 = s[0]
		s0 = s[1]
		s[0] = s0
		s1 ^= (s1 << 23) & ((1<<64) - 1)
		s[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5)

		yield (s[1] + s0) & ((1<<64) - 1)