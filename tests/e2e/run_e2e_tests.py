import os
import subprocess
from pathlib import Path
from dataclasses import dataclass

import e2e_config

@dataclass
class TestSpec:
	name: str = ""
	returncode: int = 0
	filename: str = ""
	content: str = ""

def parse_test(filename) -> TestSpec:
	test_spec = TestSpec()
	with open(filename) as f:
		reading_keys = True
		for line in f.readlines():
			if reading_keys:
				if ':' in line:
					key, value = line.split(':')
					key = key.strip()
					value = value.strip()
					if key == "Returncode":
						test_spec.returncode = int(value)
					elif key == "Name":
						test_spec.name = value
					elif key == "File":
						test_spec.filename = value
					else:
						raise ValueError(f"Unrecognized key: {key}")
				else:
					reading_keys = False
			else:
				test_spec.content += line

	return test_spec


def all_tests():
	cwd = os.path.dirname(os.path.realpath(__file__))
	for f in os.listdir(cwd):
		if f.endswith('.in'):
			yield parse_test(os.path.join(cwd, f))


def main():
	executable = e2e_config.compiler_binary
	for test_spec in all_tests():
		file_name = test_spec.filename
		with open(file_name, "w") as f:
			f.write(test_spec.content)
		p = subprocess.run([executable, file_name], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		compiler_returncode = p.returncode
		print(p.stdout)
		print(p.stderr)
		print(compiler_returncode)
		assert compiler_returncode == 0
		compiled_file_name = Path(file_name).with_suffix(".out")
		print(compiled_file_name)
		actual_returncode = subprocess.run(compiled_file_name).returncode
		expected_returncode = test_spec.returncode
		assert actual_returncode == expected_returncode
		os.remove(file_name)
		os.remove(compiled_file_name)


if __name__ == '__main__':
	main()