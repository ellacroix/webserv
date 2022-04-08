FROM fabiocicerchia/nginx-lua

RUN apk update
RUN apk add vim 

RUN rm /var/log/nginx/*
RUN rm /etc/nginx/nginx.conf

RUN mkdir /data ; \
	mkdir /data/images ; \
	mkdir /data/www ; \
	mkdir /data/www/fld1 ; \
	mkdir /data/www/fld2 ; \
	mkdir /data/www/fld3 

RUN chmod +r /data

COPY ./index.html /data/www/.
COPY ./image.png /data/images/.
COPY ./image.jpg /data/images/.
COPY ./favicon.ico /data/www/.
#COPY ./custom_error.html /data/www/.

COPY ./nginx.conf /etc/nginx/.
COPY ./index.html /usr/share/nginx/html/index.html
COPY ./.vimrc /etc/.vimrc
