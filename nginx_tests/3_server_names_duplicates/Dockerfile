#FROM nginx:latest
FROM fabiocicerchia/nginx-lua

#RUN apt-get update
#RUN apt-get install vim -y

RUN apk update
RUN apk add vim 

RUN rm /var/log/nginx/*
RUN rm /etc/nginx/nginx.conf

RUN mkdir /data ; \
	mkdir /data/images ; \
	mkdir /data/www
RUN chmod +r /data

RUN mkdir /data2 ; \
	mkdir /data2/images ; \
	mkdir /data2/www
RUN chmod +r /data2

COPY ./index.html /data/www/.
COPY ./image.png /data/images/.
COPY ./image.jpg /data/images/.
COPY ./favicon.ico /data/www/.

COPY ./index2.html /data2/www/index.html
COPY ./image.png /data2/images/.
COPY ./image.jpg /data2/images/.
COPY ./favicon.ico /data2/www/.

COPY ./nginx.conf /etc/nginx/.
COPY ./index.html /usr/share/nginx/html/index.html
COPY ./.vimrc /etc/.vimrc
