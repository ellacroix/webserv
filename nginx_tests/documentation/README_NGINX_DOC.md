# [CONFIGURING NGINX AS A WEB SERVER](https://docs.nginx.com/nginx/admin-guide/web-server/web-server/)

## Setting up a virtual servers

The NGINX Plus configuration file must include at least one **server** directive to define a virtual server. When NGINX Plus processes a request, it first selects the virtual server that will serve the request.
A virtual server is defined by a **server** directive in the **http** context, for example:

```
http {
	server {
		# Server configuration
	}
}
```

The server configuration block usually includes a **listen** directive to specify the IP address and port on which the server listens for requests.
The example below shows configuration of a server that listens on IP address 127.0.0.1 and port 8080:

```
server {
	listen 127.0.0.1:8080;
	# Additional server configuration
}
```

If a port is omitted, the standard port is used. Likewise, if an address is omitted, the server listens on all addresses (0.0.0.0 or :::). If the **listen** directive is not included at all, the “standard” port is 80/tcp.
If there are several servers that match the IP address and port of the request, NGINX Plus tests the request’s *Host* header field against the **server\_name** directives in the **server** blocks. The parameter to **server\_name** IS a full (exact) name. This example illustrates an exact name.

```
server {
	listen	80;
	server_name example.org www.example.org;
	#...
}
```

If several names match the *Host* header, NGINX Plus selects one by searching for names in the following order and using the first match it finds.
- Exact name
- *More options unrelated to project*

If the *Host* header field does not match a **server\_name**, NGINX Plus routes the request to the default server for the port on which the request arrived. The default server is the first one listed in the nginx.conf file.

## Configuring Locations

NGINX Plus can send traffic to different proxies or serve different files based on the request URIs. These blocks are defined using the **location** directive placed within a **server** directive.

NGINX Plus tests request URIs against the parameters of all **location** directives and applies the directives defined in the matching **location**. Inside each **location** block, it is usually possible (with a few exceptions) to place even more **location** directives to further refine the processing for specific groups of requests.

The **location** directive takes a prefix parameter. The following sample **location** with a pathname parameter matches request URIs that begin with */some/path/*, such as */some/path/document.html*. (It does not match */my-site/some/path* because */some/path* does not occur at the start of that URI.)

The exact logic for selecting a location to process a request is given below:
- Test the URI against all prefix strings.
- *The = (equals sign) modifier defines an exact match of the URI and a prefix string. If the exact match is found, the search stops.*
- Store the longest matching prefix string.

*A typical use case for the = modifier is requests for / (forward slash). If requests for / are frequent, specifying = / as the parameter to the location directive speeds up processing, because the search for matches stops after the first comparison.*

A location context can contain directives that define how to resolve a request.

```
    location /images/ {
        root /data;
    }
```

The **root** directive specifies the file system path in which to search for the static files to serve. The request URI associated with the location is appended to the path to obtain the full name of the static file to serve. In the example above, in response to a request for */images/example.png*, NGINX Plus delivers the file */data/images/example.png*.

## Returning Specific Status Codes

Some website URIs require immediate return of a response with a specific **redirect** code, for example when a page has been moved temporarily or permanently. The easiest way to do this is to use the **return** directive. For example:

```
location /wrong/url {
    return 404;
}
```

The first parameter of **return** is a response code. The optional second parameter can be the URL of a redirect (for codes 301, 302, 303, and 307) or the text to return in the response body. For example:

```
location /permanently/moved/url {
    return 301 http://www.example.com/moved/here;
}
```

The return directive can be included in both the **location** and **server** contexts.

## Handling Errors

With the **error\_page** directive, you can configure NGINX Plus to return a custom page along with an error code, substitute a different error code in the response, or redirect the browser to a different URI. In the following example, the **error\_page** directive specifies the page (/404.html) to return with the 404 error code.

```
error_page 404 /404.html;
```

Note that this directive does not mean that the error is returned immediately (the **return** directive does that), but simply specifies how to treat errors when they occur. The error code can occur during processing by NGINX Plus (for example, the 404 results when NGINX Plus can’t find the file requested by the client).

# [SERVING STATIC CONTENT](https://docs.nginx.com/nginx/admin-guide/web-server/serving-static-content/)

Configure NGINX and NGINX Plus to serve static content, with type-specific root directories, checks for file existence, and performance optimizations.

## Root Directory and Index Files

The **[root](https://nginx.org/en/docs/http/ngx_http_core_module.html#root)** directive specifies the root directory that will be used to search for a file. To obtain the path of a requested file, NGINX appends the request URI to the path specified by the **root** directive. The directive can be placed on any level within the **http {}**, **server {}**, or **location {}** contexts. In the example below, the root directive is defined for a virtual server. It applies to all **location {}** blocks where the root directive is not included to explicitly redefine the root:

```
server {
    root /www/data;

    location / {
    }

    location /images/ {
    }

    location ~ \.(mp3|mp4) {
        root /www/media;
    }
}
```

If a request ends with a slash, NGINX treats it as a request for a directory and tries to find an index file in the directory. The **[index](https://nginx.org/en/docs/http/ngx_http_index_module.html#index)** directive defines the index file’s name (the default value is index.html). To continue with the example, if the request URI is /images/some/path/, NGINX delivers the file /www/data/images/some/path/index.html if it exists. If it does not, NGINX returns HTTP code 404 (Not Found) by default. To configure NGINX to return an automatically generated directory listing instead, include the on parameter to the **[autoindex](https://nginx.org/en/docs/http/ngx_http_autoindex_module.html#autoindex)** directive: 

```
location /images/ {
    autoindex on;
}
```

You can list more than one filename in the **index** directive. NGINX searches for files in the specified order and returns the first one it finds.

```
location / {
    index index.htm index.html;
}
```

To return the index file, NGINX checks for its existence and then makes an internal redirect to the URI obtained by appending the name of the index file to the base URI. The internal redirect results in a new search of a location and can end up in another location as in the following example:

```
location / {
    root /data;
    index index.html index.php;
}

location ~ \.php {
    fastcgi_pass localhost:8000;
    #...
}
```

Here, if the URI in a request is /path/, and /data/path/index.html does not exist but /data/path/index.php does, the internal redirect to /path/index.php is mapped to the second location. As a result, the request is proxied.

# [HOW NGINX PROCESSES A REQUEST](http://nginx.org/en/docs/http/request_processing.html)

## Name-based virtual servers

nginx first decides which server should process the request. Let’s start with a simple configuration where all three virtual servers listen on port \*:80 : 

```


    server {
        listen      80;
        server_name example.org www.example.org;
        ...
    }

    server {
        listen      80;
        server_name example.net www.example.net;
        ...
    }

    server {
        listen      80;
        server_name example.com www.example.com;
        ...
    }
```

In this configuration nginx tests only the request’s header field “Host” to determine which server the request should be routed to. If its value does not match any **server\_name**, or the request does not contain this header field at all, then nginx will route the request to the default server for this port (the first defined for this port). In the configuration above, the default server is the first one — which is nginx’s standard default behaviour.

## Mixed name-based and IP-based virtual servers

Let’s look at a more complex configuration where some virtual servers listen on different addresses: 

```
server {
    listen      192.168.1.1:80;
    server_name example.org www.example.org;
    ...
}

server {
    listen      192.168.1.1:80;
    server_name example.net www.example.net;
    ...
}

server {
    listen      192.168.1.2:80;
    server_name example.com www.example.com;
    ...
}
```

In this configuration, nginx first tests the IP address and port of the request against the **[listen](http://nginx.org/en/docs/http/ngx_http_core_module.html#listen)** directives of the **[server](http://nginx.org/en/docs/http/ngx_http_core_module.html#server)** blocks. It then tests the “Host” header field of the request against the **[server_name](http://nginx.org/en/docs/http/ngx_http_core_module.html#server_name)** entries of the **[server](http://nginx.org/en/docs/http/ngx_http_core_module.html#server)** blocks that matched the IP address and port. If the server name is not found, the request will be processed by the default server. For example, a request for www.example.com received on the 192.168.1.1:80 port will be handled by the default server of the 192.168.1.1:80 port, i.e., by the first server, since there is no www.example.com defined for this port. 

## A simple PHP site configuration

**(THIS SECTION IS NOT FULLY RELEVANT REGARDING THE SUBJECT, AS WE DON'T NEED TO HANDLE REGULAR EXPRESSIONS BUT NEED TO BE ABLE TO HANDLE REQUEST WITH A CGI BASED ON FILE EXTENSION. I THINK WE WILL THEN MAKE UNIQUE RECOGNIZABLE REGULAR EXPRESSION PATTERN - " \*.php " FOR EXAMPLE - AS A POSSIBLE PARAMETER FOR A location DIRECTIVE.)**

Now let’s look at how nginx chooses a **location** to process a request for a typical, simple PHP site: 

```
server {
    listen      80;
    server_name example.org www.example.org;
    root        /data/www;

    location / {
        index   index.html index.php;
    }

    location ~ \.php$ {
        fastcgi_pass  localhost:9000;
        fastcgi_param SCRIPT_FILENAME
                      $document_root$fastcgi_script_name;
        include       fastcgi_params;
    }
}
```

nginx first searches for the most specific prefix location given by literal strings regardless of the listed order. In the configuration above the only prefix location is “/” and since it matches any request it will be used as a last resort. Then nginx checks locations given by regular expression in the order listed in the configuration file. The first matching expression stops the search and nginx will use this location. If no regular expression matches a request, then nginx uses the most specific prefix location found earlier. 

Note that locations of all types test only a URI part of request line without arguments. This is done because arguments in the query string may be given in several ways, for example: 

```
/index.php?user=john&page=1
/index.php?page=1&user=john
```

Besides, anyone may request anything in the query string: 

```
/index.php?page=1&something+else&user=john
```
Now let’s look at how requests would be processed in the configuration above: 
- A request “/index.php” is matched by the prefix location “/” first and then by the regular expression “\.(php)$”. Therefore, it is handled by the latter **location** and the request is passed to a FastCGI server listening on localhost:9000. The **fastcgi\_param** directive sets the FastCGI parameter SCRIPT\_FILENAME to “/data/www/index.php”, and the FastCGI server executes the file. The variable $document\_root is equal to the value of the **root** directive and the variable $fastcgi\_script\_name is equal to the request URI, i.e. “/index.php”. 
- A request “/about.html” is matched by the prefix location “/” only, therefore, it is handled in this **location**. Using the directive “**root /data/www**” the request is mapped to the file /data/www/about.html, and the file is sent to the client. 
- Handling a request “/” is more complex. It is matched by the prefix location “/” only, therefore, it is handled by this location. Then the index directive tests for the existence of index files according to its parameters and the “root /data/www” directive. If the file /data/www/index.html does not exist, and the file /data/www/index.php exists, then the directive does an internal redirect to “/index.php”, and nginx searches the locations again as if the request had been sent by a client. As we saw before, the redirected request will eventually be handled by the FastCGI server. 

## SUBJECT CONFIG FILE RULES AND NGINX EQUIVALENT

In the configuration file, you should be able to:
- Choose the port and host of each ’server’.
	- ==> **[listen](http://nginx.org/en/docs/http/ngx_http_core_module.html#listen)**
- Setup the server\_names or not.
	- ==> **[server\_name](http://nginx.org/en/docs/http/ngx_http_core_module.html#server_name)**
- The first server for a host:port will be the default for this host:port (that means it will answer to all the requests that don’t belong to an other server).
	- ==> **[default NGINX behaviour](http://nginx.org/en/docs/http/request_processing.html)**
- Setup default error pages.
	- ==> **[error\_page](http://nginx.org/en/docs/http/ngx_http_core_module.html#error_page)**
- Limit client body size.
	- ==> **[client\_max\_body\_size](http://nginx.org/en/docs/http/ngx_http_core_module.html#client_max_body_size)**
- Setup routes with one or multiple of the following rules/configuration (routes won't be using regexp):
	- ==> **[location](http://nginx.org/en/docs/http/ngx_http_core_module.html#location)**
	- Define a list of accepted HTTP methods for the route.
		- ==> **[limit_except](http://nginx.org/en/docs/http/ngx_http_core_module.html#limit_except)**
	- Define a HTTP redirection.
		- ==> **[return](https://nginx.org/en/docs/http/ngx_http_rewrite_module.html#return)**
	- Define a directory or a file from where the file should be searched (for example, if url /kapouet is rooted to /tmp/www, url /kapouet/pouic/toto/pouet is /tmp/www/pouic/toto/pouet).
		- ==> **[root](http://nginx.org/en/docs/http/ngx_http_core_module.html#root)**
	- Turn on or off directory listing.
		- ==> **[autoindex](http://nginx.org/en/docs/http/ngx_http_autoindex_module.html#autoindex)**
	- Set a default file to answer if the request is a directory.
		- ==> **[index](https://nginx.org/en/docs/http/ngx_http_index_module.html#index)**
	- Execute CGI based on certain file extension (for example .php).
	- Make the route able to accept uploaded files and configure where they should be saved.
		- Do you wonder what a CGI is?
		- Because you won’t call the CGI directly, use the full path as PATH_INFO.
		- Just remember that, for chunked request, your server needs to unchunk it and the CGI will expect EOF as end of the body.
		- Same things for the output of the CGI. If no content_length is returned from the CGI, EOF will mark the end of the returned data.
		- Your program should call the CGI with the file requested as first argument.
		- The CGI should be run in the correct directory for relative path file access.
		- Your server should work with one CGI (php-CGI, Python, and so forth).

