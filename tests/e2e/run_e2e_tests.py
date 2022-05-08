import os

def parse_test(filename):
	test_spec = {'content': ''}
	with open(filename) as f:
		reading_keys = True
		for line in f.readlines():
			if reading_keys:
				if ':' in line:
					key, value = line.split(':')
					test_spec[key.strip()] = value.strip()
				else:
					reading_keys = False
			else:
				test_spec['content'] += line

	if 'Returncode' in test_spec:
		test_spec['Returncode'] = int(test_spec['Returncode'])
	return test_spec


def all_tests():
	cwd = os.path.dirname(os.path.realpath(__file__))
	for f in os.listdir(cwd):
		if f.endswith('.in'):
			yield parse_test(os.path.join(cwd, f))


def main():
	for test_spec in all_tests():
		print(test_spec)


if __name__ == '__main__':
	main()