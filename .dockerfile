FROM alpine:latest

RUN apk add --no-cache gcc musl-dev

WORKDIR /app

COPY dis.c /app

RUN gcc -Wall -Wextra -pedantic --std=c17 -o dis dis.c

CMD ["./dis"]
