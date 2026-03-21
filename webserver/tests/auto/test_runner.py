import os
import importlib
from prompts import p_pass, p_fail

def run_tests():
    TEST_DIR = "tests"

    tests = []

    for file in os.listdir(TEST_DIR):

        if file.startswith("test_") and file.endswith(".py"):

            module_name = file[:-3]

            module = importlib.import_module(f"{TEST_DIR}.{module_name}")

            for attr in dir(module):

                if attr.startswith("test_"):

                    tests.append(getattr(module, attr))


    for test in tests:

        try:

            if test():
                p_pass(test.__name__)
            else:
                p_fail(test.__name__)

        except Exception:
            p_fail(test.__name__)
