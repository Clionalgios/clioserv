import socket

def test_server_banner_as_header(host="127.0.0.1", port=8181):

    name = "server banner returned"

    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((host, port))

        request = b"GET / HTTP/1.1\r\nHost: localhost\r\n\r\n"
        s.send(request)

        response = s.recv(4096).decode()
        s.close()

        if "Server:" in response:
            return True
        else:
            return False

    except Exception as e:
        return False
