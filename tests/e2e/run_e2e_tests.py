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
    command_line: str = ""
    binary: str = ""


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
                    elif key == "Commandline":
                        test_spec.command_line = value
                    elif key == "Binary":
                        test_spec.binary= value
                    else:
                        raise ValueError(f"Unrecognized key: {key}")
                else:
                    reading_keys = False
            else:
                test_spec.content += line


    if not test_spec.command_line:
        test_spec.command_line = test_spec.filename
    if not test_spec.binary:
        test_spec.binary = Path(os.getcwd()) / Path(test_spec.filename).with_suffix(".out")
    return test_spec


def all_tests():
    test_dir = e2e_config.test_input_dir
    for f in os.listdir(test_dir):
        if f.endswith('.in'):
            yield parse_test(os.path.join(test_dir, f))


def main():
    executable = e2e_config.compiler_binary
    for test_spec in all_tests():
        file_name = test_spec.filename
        print(f"Running {test_spec.name}")
        with open(file_name, "w") as f:
            f.write(test_spec.content)

        command_line = [executable]
        split_cmd = test_spec.command_line.split()
        command_line.extend(test_spec.command_line.split())
        p = subprocess.run(command_line, capture_output=True, text=True)
        assert_on_process_returncode(p, 0)

        p = subprocess.run(test_spec.binary)
        assert_on_process_returncode(p, test_spec.returncode)

        os.remove(file_name)
        os.remove(test_spec.binary)


def assert_on_process_returncode(process_result, expected_returncode):
    assert process_result.returncode == expected_returncode, format_process_output(
        process_result.stdout, process_result.stderr, process_result.returncode)

def format_process_output(stdout, stderr, return_code):
    separator = "-" * 10
    return f"\nstdout: {stdout}\n{separator}\nstderr: {stderr}\n{separator}\ncompiler_return_code: {return_code}"


if __name__ == '__main__':
    main()
