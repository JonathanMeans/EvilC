import os
import subprocess
import sys
from pathlib import Path

def parse_test(filename):
	test_spec = {'Content': ''}
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
				test_spec['Content'] += line

	if 'Returncode' in test_spec:
		test_spec['Returncode'] = int(test_spec['Returncode'])
	return test_spec


def all_tests():
	cwd = os.path.dirname(os.path.realpath(__file__))
	for f in os.listdir(cwd):
		if f.endswith('.in'):
			yield parse_test(os.path.join(cwd, f))


def main():
	executable = sys.argv[1]
	for test_spec in all_tests():
		file_name = test_spec["File"]
		with open(file_name, "w") as f:
			f.write(test_spec["Content"])
		compiler_returncode = subprocess.run([executable, file_name]).returncode
		assert compiler_returncode == 0
		compiled_file_name = Path(file_name).with_suffix(".out")
		actual_returncode = subprocess.run(compiled_file_name).returncode
		expected_returncode = test_spec["Returncode"]
		assert actual_returncode == expected_returncode
		os.remove(file_name)


if __name__ == '__main__':
	main()