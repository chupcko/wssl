import socket
import ssl
import struct
import warnings

import pytest

import helpers


def tls_connect(host, port, minimum=None, maximum=None, timeout=5) :
  context = ssl.SSLContext(ssl.PROTOCOL_TLS_CLIENT)
  context.check_hostname = False
  context.verify_mode = ssl.CERT_NONE
  if minimum is not None :
    context.minimum_version = minimum
  #$
  if maximum is not None :
    context.maximum_version = maximum
  #$
  raw = socket.create_connection((host, port), timeout=timeout)
  return context.wrap_socket(raw, server_hostname=host)
#$


def test_tls_versions(echo_ssl_server) :
  host, port = echo_ssl_server
  for version in (ssl.TLSVersion.TLSv1_2, ssl.TLSVersion.TLSv1_3) :
    sock = tls_connect(host, port, minimum=version, maximum=version)
    sock.close()
  #$
  with warnings.catch_warnings() :
    warnings.simplefilter('ignore', DeprecationWarning)
    old_maximum = ssl.TLSVersion.TLSv1_1
    with pytest.raises(Exception) :
      tls_connect(host, port, maximum=old_maximum)
    #$
  #$
#$


def test_tls13_negotiates_allowed_suite(echo_ssl_server) :
  host, port = echo_ssl_server
  sock = tls_connect(host, port,
                     minimum=ssl.TLSVersion.TLSv1_3,
                     maximum=ssl.TLSVersion.TLSv1_3)
  try :
    assert sock.cipher()[0] in ('TLS_AES_256_GCM_SHA384',
                                'TLS_AES_128_GCM_SHA256')
  finally :
    sock.close()
  #$
#$


def test_weak_cipher_rejected(echo_ssl_server) :
  host, port = echo_ssl_server
  context = ssl.SSLContext(ssl.PROTOCOL_TLS_CLIENT)
  context.check_hostname = False
  context.verify_mode = ssl.CERT_NONE
  context.set_ciphers('AES128-SHA')
  context.maximum_version = ssl.TLSVersion.TLSv1_2
  raw = socket.create_connection((host, port), timeout=5)
  with pytest.raises(Exception) :
    context.wrap_socket(raw, server_hostname=host)
  #$
  raw.close()
#$


def test_rst_storm_keeps_server_alive(echo_server) :
  import struct as struct_module
  host, port = echo_server
  for _ in range(20) :
    sock = socket.create_connection((host, port), timeout=5)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_LINGER,
                    struct_module.pack('ii', 1, 0))
    sock.sendall(
      b'GET / HTTP/1.1\r\nHost: x\r\nUpgrade: websocket\r\n'
      b'Connection: Upgrade\r\nSec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n'
      b'Sec-WebSocket-Version: 13\r\n\r\n'
    )
    sock.close()
  #$
  response, sock = helpers.ws_handshake(host, port)
  assert b'101' in helpers.status_line(response)
  sock.close()
#$


def test_ssl_handshake_and_echo(echo_ssl_server) :
  host, port = echo_ssl_server
  sock = tls_connect(host, port)
  try :
    sock.sendall(
      b'GET / HTTP/1.1\r\nHost: x\r\nUpgrade: websocket\r\n'
      b'Connection: Upgrade\r\nSec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n'
      b'Sec-WebSocket-Version: 13\r\n\r\n'
    )
    response = b''
    while b'\r\n\r\n' not in response :
      response += sock.recv(4096)
    #$
    assert b'101' in response.split(b'\r\n', 1)[0]
    helpers.send_frame(sock, 0x1, b'tls-hello')
    assert helpers.recv_frame(sock) == (0x1, b'tls-hello')
  finally :
    sock.close()
  #$
#$


def test_ssl_garbage_and_abort_keep_server_alive(echo_ssl_server) :
  host, port = echo_ssl_server
  raw = socket.create_connection((host, port), timeout=5)
  raw.sendall(b'GET / HTTP/1.0\r\n\r\nnot tls\r\n')
  raw.close()
  socket.create_connection((host, port), timeout=5).close()
  sock = tls_connect(host, port)
  sock.close()
#$


def test_ssl_huge_frame_disconnects_and_server_survives(echo_ssl_server) :
  host, port = echo_ssl_server
  sock = tls_connect(host, port)
  try :
    sock.sendall(
      b'GET / HTTP/1.1\r\nHost: x\r\nUpgrade: websocket\r\n'
      b'Connection: Upgrade\r\nSec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n'
      b'Sec-WebSocket-Version: 13\r\n\r\n'
    )
    response = b''
    while b'\r\n\r\n' not in response :
      response += sock.recv(4096)
    #$
    assert b'101' in response.split(b'\r\n', 1)[0]
    sock.sendall(b'\x81\xff' + struct.pack('!Q', 0xFFFFFFFFFFFFFFFF) + b'\x01\x02\x03\x04')
    sock.settimeout(2)
    assert sock.recv(100) == b''
  finally :
    sock.close()
  #$
  sock = tls_connect(host, port)
  sock.close()
#$
