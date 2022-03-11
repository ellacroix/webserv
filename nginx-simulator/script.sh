#!/bin/bash

docker run --name docker-nginx -p 8000:80 -d nginx
telnet localhost 8000

docker stop docker-nginx
docker rm docker-nginx