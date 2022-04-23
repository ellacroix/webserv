#!/bin/bash
docker build . -t server
docker run -it -p 8000:8000 --rm --name server_container server
