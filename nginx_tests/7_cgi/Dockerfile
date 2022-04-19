FROM fabiocicerchia/nginx-lua

RUN apk update
RUN apk add vim 
RUN apk add php-cgi

RUN rm /var/log/nginx/*
RUN rm /etc/nginx/nginx.conf

RUN mkdir /data ; \
	mkdir /data/www 

RUN chmod +r /data

COPY /srcs/public /data/www/.
COPY ./srcs/index.html /data/www/.
COPY ./srcs/init.sh /
RUN chmod 777 /init.sh

COPY ./srcs/nginx.conf /etc/nginx/.
