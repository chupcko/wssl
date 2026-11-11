FROM ubuntu:resolute

RUN apt-get update

RUN apt-get install -y --no-install-recommends \
    build-essential                            \
    dpkg-dev                                   \
    fakeroot                                   \
    libssl-dev                                 \
    openssl                                    \
    pkg-config                                 \
    python3                                    \
    python3-pip

RUN rm -r -f /var/lib/apt/lists/*

RUN pip install --no-cache-dir --break-system-packages pytest
