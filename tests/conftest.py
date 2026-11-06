import os
import socket
import subprocess
import time

import pytest

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PLAIN_PORT = 5001
SSL_PORT = 5011


def wait_for_port(host, port, timeout=15) :
  deadline = time.time() + timeout
  while time.time() < deadline :
    try :
      socket.create_connection((host, port), timeout=1).close()
      return
    except OSError :
      time.sleep(0.1)
    #$
  #$
  raise RuntimeError('server did not listen on %s:%d' % (host, port))
#$

def stop_server(proc) :
  proc.terminate()
  try :
    proc.wait(timeout=10)
  except subprocess.TimeoutExpired :
    proc.kill()
    proc.wait(timeout=10)
  #$
#$


@pytest.fixture(scope='module')
def echo_server() :
  proc = subprocess.Popen(
    [os.path.join(REPO_ROOT, 'examples', 'wssl_echo.e')],
    stdout=subprocess.DEVNULL,
    stderr=subprocess.DEVNULL,
  )
  try :
    wait_for_port('127.0.0.1', PLAIN_PORT)
    yield ('127.0.0.1', PLAIN_PORT)
  finally :
    stop_server(proc)
  #$
#$

@pytest.fixture(scope='session')
def kick_server() :
  proc = subprocess.Popen(
    [os.path.join(REPO_ROOT, 'tests', 'kick_server.e')],
    stdout=subprocess.DEVNULL,
    stderr=subprocess.DEVNULL,
  )
  try :
    wait_for_port('127.0.0.1', 5004)
    yield ('127.0.0.1', 5004)
  finally :
    stop_server(proc)
  #$
#$

@pytest.fixture(scope='module')
def echo_ssl_server(tmp_path_factory) :
  cert_dir = tmp_path_factory.mktemp('cert')
  subprocess.run(
    [os.path.join(REPO_ROOT, 'tools', 'make_ssl_certificate'), str(cert_dir)],
    check=True,
    stdout=subprocess.DEVNULL,
    stderr=subprocess.DEVNULL,
  )
  proc = subprocess.Popen(
    [
      os.path.join(REPO_ROOT, 'examples', 'wssl_echo.e'),
      str(cert_dir / 'server.crt'),
      str(cert_dir / 'server.key'),
    ],
    stdout=subprocess.DEVNULL,
    stderr=subprocess.DEVNULL,
  )
  try :
    wait_for_port('127.0.0.1', SSL_PORT)
    yield ('127.0.0.1', SSL_PORT)
  finally :
    stop_server(proc)
  #$
#$
