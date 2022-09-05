FROM alpine:3.14

RUN apk add --no-cache git
RUN apk add --no-cache make
RUN apk add --no-cache tree
RUN apk add --no-cache g++
WORKDIR /TTT
COPY . .
RUN tree /TTT

RUN make

CMD [ "build/server" ]
