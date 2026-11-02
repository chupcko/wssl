import helpers


def test_callback_disconnect_closes_connection(kick_server) :
  host, port = kick_server
  _, sock = helpers.ws_handshake(host, port)
  helpers.send_frame(sock, 0x1, b'kick')
  sock.settimeout(2)
  assert sock.recv(100) == b''
  sock.close()
#$


def test_callback_error_closes_connection(kick_server) :
  host, port = kick_server
  _, sock = helpers.ws_handshake(host, port)
  helpers.send_frame(sock, 0x1, b'fail')
  sock.settimeout(2)
  assert sock.recv(100) == b''
  sock.close()
#$


def test_callback_ok_keeps_connection(kick_server) :
  host, port = kick_server
  _, sock = helpers.ws_handshake(host, port)
  helpers.send_frame(sock, 0x1, b'hello')
  assert helpers.recv_frame(sock) == (0x1, b'hello')
  sock.close()
#$
