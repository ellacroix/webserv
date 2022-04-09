FROM fabiocicerchia/nginx-lua

RUN apk update
RUN apk add vim 

RUN rm /var/log/nginx/*
RUN rm /etc/nginx/nginx.conf

RUN mkdir /data ; \
	mkdir /data/www \
	mkdir /data/www/public

RUN chmod +r /data

COPY /srcs/public /data/www/public/.
# COPY ./srcs/index.html /data/www/.

COPY ./srcs/nginx.conf /etc/nginx/.
COPY .vimrc /etc/.vimrc
