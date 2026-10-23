# Running wssl examples as SSL services

This document describes how to build the `openssl` branch, run the
examples as systemd services (plain and SSL), and wire them to
Let's Encrypt certificates with automatic restart on renewal.

## 1. Ports

SSL ports are plain ports + 10.

| Example | Plain (IPv4/IPv6) | SSL (IPv4/IPv6) |
|---|---|---|
| echo | 5001 / 5101 | 5011 / 5111 |
| time | 5002 / 5102 | 5012 / 5112 |
| chat | 5003 / 5103 | 5013 / 5113 |

## 2. Prerequisites

Debian/Ubuntu:

```sh
sudo apt-get install -y gcc make libssl-dev openssl
```

Notes:

- No autoconf is used. If `pkg-config` exists it supplies OpenSSL
  flags, otherwise the Makefiles fall back to `-lssl -lcrypto`.
- `tools/*.sh` scripts are POSIX `sh` compatible (work with dash).
- In the link line libraries must stay after objects, otherwise the
  linker drops `-lssl -lcrypto` (`--as-needed`). See
  `examples/Makefile`.

## 3. Build

```sh
git clone https://github.com/chupcko/wssl.git
cd wssl
git checkout openssl
make
```

Result: `examples/echo.e`, `time.e`, `chat.e` (plain) and
`examples/echo_ssl.e`, `time_ssl.e`, `chat_ssl.e` (SSL),
plus `out/libwssl.a` and `out/wssl.h`.

## 4. Certificates

### 4a. Local testing (self signed)

```sh
tools/make_ssl_certificate /tmp/wssl-test   # server.crt + server.key
./examples/echo_ssl.e /tmp/wssl-test/server.crt /tmp/wssl-test/server.key
```

Browsers will warn about the self signed certificate; accept the
exception for testing.

### 4b. Public server (Let's Encrypt)

Issue once (nginx plugin shown; use `webroot` or `standalone` if
nginx is not on ports 80/443):

```sh
sudo certbot --nginx -d example.com
```

Certificates land in `/etc/letsencrypt/live/example.com/`.
Pass them to any `*_ssl` example:

```sh
./examples/echo_ssl.e \
  /etc/letsencrypt/live/example.com/fullchain.pem \
  /etc/letsencrypt/live/example.com/privkey.pem
```

The server sends the full chain (`SSL_CTX_use_certificate_chain_file`),
so strict clients verify without cached intermediates.

Let's Encrypt certificates last 90 days. Renewal (`certbot renew`,
usually on a systemd timer) does not restart wssl by itself, see
section 6.

## 5. systemd services

One unit per example. SSL example (`/etc/systemd/system/wssl-echo-ssl.service`):

```ini
[Unit]
Description=WSSL echo SSL service
After=network-online.target
Wants=network-online.target
[Service]
Type=simple
WorkingDirectory=/root/wssl
ExecStart=/root/wssl/examples/echo_ssl.e /etc/letsencrypt/live/example.com/fullchain.pem /etc/letsencrypt/live/example.com/privkey.pem
Restart=always
RestartSec=5
[Install]
WantedBy=multi-user.target
```

Plain example (`/etc/systemd/system/wssl-echo.service`): same file
with `ExecStart=/root/wssl/examples/echo.e`. Repeat for time and
chat with their binaries and ports from the table above.

Enable and start:

```sh
sudo systemctl daemon-reload
sudo systemctl enable --now wssl-echo wssl-time wssl-chat
sudo systemctl enable --now wssl-echo-ssl wssl-time-ssl wssl-chat-ssl
sudo systemctl is-active wssl-echo wssl-time wssl-chat wssl-echo-ssl wssl-time-ssl wssl-chat-ssl
```

## 6. Restart on certificate renewal

The server reads certificates at startup, so after `certbot renew`
the services must be restarted. certbot runs every executable in
`/etc/letsencrypt/renewal-hooks/deploy/` after a successful renewal.
Create `/etc/letsencrypt/renewal-hooks/deploy/wssl-ssl-examples`:

```sh
#!/bin/sh
systemctl restart wssl-echo-ssl wssl-time-ssl wssl-chat-ssl
```

```sh
sudo chmod +x /etc/letsencrypt/renewal-hooks/deploy/wssl-ssl-examples
```

Force a renewal once to prove the hook works:

```sh
sudo certbot renew --force-renewal
sudo systemctl status wssl-echo-ssl
```

## 7. Browser test pages

`html/` holds one page per example and address family. Pages ending
in `_ssl.html` connect via `wss://` (the JS uses the `serverScheme`
variable from `index.js`, default `ws://`). In the repository the
pages point at `127.0.0.1` / `[::1]` for local testing.

To serve them for a public host, copy the files to the web root and
rewrite the host names in the deployed copy (the repository stays
local-first):

```sh
mkdir -p /var/www/example.com/wssl
cp html/*.html html/*.js html/*.css /var/www/example.com/wssl/
rm /var/www/example.com/wssl/index.js.template
sed -i "s/127.0.0.1/example.com/g; s/\[::1\]/example.com/g" \
  /var/www/example.com/wssl/index.js
```

Then open e.g. `https://example.com/wssl/echo_ipv4_ssl.html`,
press Connect and Send. Plain pages need the plain services from
section 5, SSL pages need the SSL services.

## 8. Verification

Chain served correctly (expect return code 0):

```sh
openssl s_client -connect example.com:5011 -servername example.com \
  </dev/null 2>/dev/null | grep "Verify return"
```

Minimal WebSocket echo check (plain port 5001; wrap the socket in
`ssl` with the system trust store for SSL ports):

```sh
python3 - <<EOF
import base64, hashlib, os, socket
key = base64.b64encode(os.urandom(16)).decode()
s = socket.create_connection(('example.com', 5001), timeout=15)
s.sendall(('GET / HTTP/1.1\r\nHost: example.com\r\nUpgrade: websocket\r\n'
  'Connection: Upgrade\r\nSec-WebSocket-Key: %s\r\n'
  'Sec-WebSocket-Version: 13\r\n\r\n' % key).encode())
resp = b''
while b'\r\n\r\n' not in resp: resp += s.recv(4096)
print(resp.split(b'\r\n')[0].decode())
EOF
```

Expected: `HTTP/1.1 101 Web Socket Protocol Handshake`.
