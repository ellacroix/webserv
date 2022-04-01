FROM nginx:latest

RUN apt-get update
RUN apt-get install vim -y

RUN rm /var/log/nginx/*
RUN rm /etc/nginx/nginx.conf

COPY ./nginx.conf /etc/nginx/.
COPY ./index.html /usr/share/nginx/html/index.html
COPY ./.vimrc /etc/.vimrc
