import socket
from prompts import p_pass, p_info
from test_runner import run_tests

# ------------------------
# MAIN
# ------------------------

def main():

    p_info("Starting server tests")
    p_pass("Test prompt success")

    run_tests()



    print()


if __name__ == "__main__":
    main()