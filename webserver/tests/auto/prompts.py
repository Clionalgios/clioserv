GREEN = "\033[92m"
RED = "\033[91m"
BLUE = "\033[94m"
RESET = "\033[0m"

def p_pass(name):
    print(f"[{GREEN}PASS{RESET}] - {name}")


def p_fail(name):
    print(f"[{RED}FAIL{RESET}] - {name}")


def p_info(msg):
    print(f"[{BLUE}INFO{RESET}] - {msg}")