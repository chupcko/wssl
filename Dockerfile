FROM ubuntu:resolute

RUN apt-get update                             \
    &&                                         \
    apt-get install -y --no-install-recommends \
    build-essential                            \
    libssl-dev                                 \
    pkg-config                                 \
    python3                                    \
    python3-pip                                \
    openssl                                    \
    &&                                         \
    rm -rf /var/lib/apt/lists/*                \

RUN pip install --no-cache-dir --break-system-packages pytest

WORKDIR /wssl
COPY . .
RUN make

CMD ["make", "test"]
