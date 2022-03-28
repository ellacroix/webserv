# webserv

## Logique
1. Parsing du fichier de config
2. Lancement du serveur
3. Reception des demandes de connection
4. Etablissement de la connectiom 
5. Reception des requetes HTTP
6. Parsing des requetes HTTP
7. Execution des CGI si necessaire
8. Parsing de l'output du CGI
9. Envoi des reponses 
10. Fermeture de la connection

## Architecture (Voir le dossier app.diagrams.net)
- Une classe Port, qui contient une std::map<int, Client*>
- Une classe Virtual_Server
- Une classe Client, qui contient ses parametres, un pointeur vers son Port, une instance Request, une instance Response
- Une classe Request, pour stocker la requete HTTP a recevoir
- Une classe Response, pour stocker la reponse HTTP a renvoyer

## Logique multi-thread
On a une pool de thread qui s'occuperont de toutes les taches de recv() a send().
Le main process gere juste les connections entrantes et l'ajout de taches pour la thread pool.

## Sujet
- Your server must never block and the client can be bounced properly if necessary.
- A request to your server should never hang forever.
- You must be able to serve a fully static website.
- You need at least GET, POST, and DELETE methods.
- Your server must be able to listen to multiple ports (see Configuration file).
- The first server for a host:port will be the default for this host:port (that means it will answer to all the requests that don’t belong to an other server). (Cas de plusieurs serveurs sur le meme port avec des noms differents).

## Fonctions autorisées
**[int poll(struct pollfd *fds, nfds_t nfds, int timeout)](https://man7.org/linux/man-pages/man2/poll.2.html)**
```
struct pollfd {
               int   fd;         /* file descriptor */
               short events;     /* requested events */
               short revents;    /* returned events */};
```

**[int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)](http://manpagesfr.free.fr/man/man2/select.2.html)**
``` 
exemple /experiment/select_experiment.c 
```

**htons, htonl, ntohs, ntohl**
Ensure the conversion of short and long numbers between Host and Network, regardless of their endian-ness

**[epoll](https://man7.org/linux/man-pages/man7/epoll.7.html)**



## Questions
- Concept de chunk et utilité dans le projet ?
- Doit on gerer l'access authentication ?

## Resources

### Server setup

[How to build a simple HTTP server in C](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa)

[Simple server with C++](https://ncona.com/2019/04/building-a-simple-server-with-cpp/)

[C++ Web Programming](https://www.tutorialspoint.com/cplusplus/cpp_web_programming.htm)

### HTTP 1.1 (standard to follow) :

[HTTP/1.1 (RFC 2616)](https://www.rfc-editor.org/rfc/rfc2616.html)
[Minimal version](http://www.cs.cmu.edu/~srini/15-441/F11/responses)

[HTTP/1.1 : Message Syntax and Routing (RFC 7230)](https://www.rfc-editor.org/rfc/rfc7230.html)

[HTTP/1.1 : Semantics and Content (RFC 7231)](https://www.rfc-editor.org/rfc/rfc7231.html)

[HTTP/1.1 : Conditional Requests (RFC 7232)](https://www.rfc-editor.org/rfc/rfc7232.html)

[HTTP/1.1 : Range Requests (RFC 7233)](https://www.rfc-editor.org/rfc/rfc7233.html)

[HTTP/1.1 : Caching (RFC 7234)](https://www.rfc-editor.org/rfc/rfc7234.html)

[HTTP/1.1 : Authentication (RFC 7235)](https://www.rfc-editor.org/rfc/rfc7235.html)

### Other HTTP (legacy / future) :

[HTTP/1.0 (RFC 1945)](https://www.rfc-editor.org/rfc/rfc1945.html)

[HTTP/2 (RFC 7240)](https://www.rfc-editor.org/rfc/rfc7540.html)

[HTTP/2 : Header Compression (RFC 7241)](https://www.rfc-editor.org/rfc/rfc7541.html)

[FTP (RFC 959)](https://www.rfc-editor.org/rfc/rfc959.html)

### HTTP Header Syntax

[HTTP Request Methods](https://en.wikipedia.org/wiki/Hypertext_Transfer_Protocol#Request_methods)

[HTTP Status Codes](https://en.wikipedia.org/wiki/List_of_HTTP_status_codes)

[HTTP Header Break Style](https://stackoverflow.com/questions/5757290/http-header-line-break-style)

### Select and non-blocking

[Select](https://www.lowtek.com/sockets/select.html)

[Non-blocking I/O](https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_72/rzab6/xnonblock.htm)

### CGI

[CGI : Getting Started](http://www.mnuwer.dbasedeveloper.co.uk/dlearn/web/session01.htm)

[CGI 1.1 Documentation](http://www.wijata.com/cgi/cgispec.html#4.0)

[nice overview](https://perso.liris.cnrs.fr/lionel.medini/enseignement/M1IF03/Tutoriels/Tutoriel_CGI_SSI.pdf)

[some details](https://www.developpez.net/forums/d151285/php/langage/php-js-quoi-sert-php-cgi-exe-repertoire-php/)


### web socket
- [nice article](https://www.bogotobogo.com/cplusplus/sockets_server_client.php)
- [Beej's Guide to Network Programming](http://beej.us/guide/bgnet/html/)
- ["build http server from scratch in C"](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa)
- [Les sockets en C](https://broux.developpez.com/articles/c/sockets/)
- [select()](http://manpages.ubuntu.com/manpages/xenial/fr/man2/select_tut.2.html)
- [select()example](https://man.developpez.com/man2/select_tut/)
- [select()in the details](https://notes.shichao.io/unp/ch6/#readset-writeset-and-exceptset-as-value-result-)
- [nice schema](https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_72/rzab6/xnonblock.htm)
- [select() vs threads](https://www.lowtek.com/sockets/select.html)
- [TCP 3-way handshake schema ; Blocking vs non-blocking socket](https://www.bogotobogo.com/cplusplus/sockets_server_client_2.php)
- [more socket theory to waste time](https://www.bogotobogo.com/cplusplus/sockets_server_client_3.php)
- [tuto Jacob Sorber](https://www.youtube.com/watch?v=bdIiTxtMaKA&list=PL9IEJIKnBJjH_zM5LnovnoaKlXML5qh17)

### http protocol
- How the web works http://www.garshol.priv.no/download/text/http-tut.html
- Request and Response format https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages

### http headers in details
- MIME list https://developer.mozilla.org/fr/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Common_types
- response status code https://fr.wikipedia.org/wiki/Liste_des_codes_HTTP
- auth theory https://developer.mozilla.org/en-US/docs/Web/HTTP/Authentication#Basic_authentication_scheme
- auth practice https://stackoverflow.com/questions/180947/base64-decode-snippet-in-c

### chunked encoding
- wiki https://fr.wikipedia.org/wiki/Chunked_transfer_encoding
- the idea https://www.geeksforgeeks.org/http-headers-transfer-encoding/

### utils
- https://superuser.com/questions/48505/how-to-find-virtual-memory-size-and-cache-size-of-a-linux-system
- https://blog.seboss666.info/2015/08/reprenez-le-controle-du-cache-memoire-du-noyau-linux/


## Discord
[Reception du header Content-Language](https://discord.com/channels/774300457157918772/785407573000060978/796296015522299935)

# Notes
- Les stream socket SOCK_STREAM, sont surs et integrent le protocole TCP
- Voir Type Mime pour gerer static site (Content Type)
- Pour l'optimisation multi-thread, faire une thread pool avec un std::dequeue [video](https://www.youtube.com/watch?v=FMNnusHqjpw&list=PL9IEJIKnBJjH_zM5LnovnoaKlXML5qh17&index=6&ab_channel=JacobSorber)
- Au vu des discussions Discord, le pipelining n'a pas a etre géré, notamment car il est abandonné en HTTP2.0 et par la plupart des navigateurs.
- Caching: plutot une notion pour les intermediaires, pas a gerer
- Charset = token, le token specifie quel charset a employer, voir cas de Missing Charset
- Transfer-coding: chunked, identity, gzip, compress, deflate 
- Chunked transfer Coding: OPTIONAL trailer
- If the Request has no Request-Line(an instant CRLF as first character), ignore the Request
- 

# Message
```
generic-message =   start-line
                    *(message-header CRLF)
                    CRLF
                    [ message-body ]

start-line      =   Request-Line | Status-Line
```
- RFC 2616 note: The HTTP protocol is a request/response protocol. A client sends a request to the server in the form of a request method, URI, and protocol version, followed by a MIME-like message containing request modifiers, client information, and possible body content over a connection with a server.

## Headers
```
    message-header = field-name ":" [ field-value ]
    field-name     = token
    field-value    = *( field-content | LWS )
    field-content  = <the OCTETs making up the field-value
                    and consisting of either *TEXT or combinations
                    of token, separators, and quoted-string>
```
- The order in which header fields with differing field names are received is not significant. However, it is "good practice" to send general-header fields first, followed by request-header or response-header fields, and ending with the entity-header fields
- Multiple message-header fields with the same field-name MAY be present in a message if and only if the entire field-value for that header field is defined as a comma-separated list [i.e., #(values)]. It MUST be possible to combine the multiple header fields into one "field-name: field-value" pair, without changing the semantics of the message, by appending each subsequent field-value to the first, each separated by a comma.
- General Header Fields: There are a few header fields which have general applicability for both request and response messages, but which do not apply to the entity being transferred. These header fields apply only to the message being transmitted.
```
general-header = Cache-Control | Connection | Date | Pragma | Trailer | Transfer-En| Upgrade | Via | Warning          
```

## Body
```
message-body = entity-body | <entity-body encoded as per Transfer-Encoding>
```
- Transfer-Encoding MUST be used to indicate any transfer-codings applied by an application to ensure safe and proper transfer of the message.

## Message Length
- The transfer-length of a message is the length of the message-body as it appears in the message; that is, after any transfer-codings have been applied.
- See more details [HERE](https://www.rfc-editor.org/rfc/rfc2616.html#:~:text=When%20a%20message%2Dbody%20is%20included%20with%20a%20message%2C%20the%0A%20%20%20transfer%2Dlength%20of%20that%20body%20is%20determined%20by%20one%20of%20the%20following%0A%20%20%20(in%20order%20of%20precedence)%3A)
- Messages MUST NOT include both a Content-Length header field and a non-identity transfer-coding. If the message does include a non-identity transfer-coding, the Content-Length MUST be ignored.

# Request
```
Request               = Request-Line            
                        *(( general-header      
                         | request-header       
                         | entity-header ) CRLF)
                        CRLF
                        [ message-body ]        
```
## Start-line
```
Request-Line   = Method SP Request-URI SP HTTP-Version CRLF
```
### Request-URI
```
Request-URI    = "*" | absoluteURI | abs_path | authority
```
- "*" optionnel pour nous ? Vu que cela ne s'applique pas pour GET/POST/DELETE ?
- absoluteURI semble s'appliquer aux proxys, mais a accepter tout de meme
- authority is only used by the CONNECT method
- Request-URI can be encoded as "% HEX HEX" and has to be decoded

### The Resource Identified by a Request
- Si on gere plusieurs host (server names ?) sur un meme port, on doit bien differencier les dossiers auxquels ils ont acces
- Le host peut etre defini dans l'absoluteURI ou le header host, un invalid host entraine une reponse 400
## Request Header Fields
```
request-header =    Accept | Accept-Charset | Accept-Encoding | Accept-Language 
                    | Authorization | Expect | From | Host | If-Match | If-Modified-Since 
                    | If-None-Match | If-Range | If-Unmodified-Since | Max-Forwards 
                    | Proxy-Authorization | Range | Referer | TE | User-Agent
```
- Expect request: "100-continue", Indique que le client attend une reponse 100 avant d'envoyer le body. [VOIR CAS POSSIBLES](https://www.rfc-editor.org/rfc/rfc2616.html#section-8.2.3)

## Body
- The presence of a message-body in a request is signaled by the inclusion of a Content-Length or Transfer-Encoding header field in the request's message-headers.
- A message-body MUST NOT be included in a request if the specification of the request method (section 5.1.1) does not allow sending an entity-body in requests.
- A server SHOULD read and forward a message-body on any request; if the request method does not include defined semantics for an entity-body, then the message-body SHOULD be ignored when handling the request.

# Response
```
Response      = Status-Line             
                *(( general-header      
                | response-header      
                | entity-header ) CRLF)
                CRLF
                [ message-body ]        
```
- If a request contains a message-body and a Content-Length is not given, the server SHOULD respond with 400 (bad request) if it cannot determine the length of the message, or with 411 (length required) if it wishes to insist on receiving a valid Content-Length.
## Status-line
```
Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
```
### Status codes to implement
1xx: Informational - Request received, continuing process
- ~~101 Switching Protocols~~

2xx: [Success - The action was successfully received, understood, and accepted](https://www.rfc-editor.org/rfc/rfc2616.html#section-10.2)
- [200 OK](https://www.rfc-editor.org/rfc/rfc2616.html#section-10.2.1)
- [201 Created](https://www.rfc-editor.org/rfc/rfc2616.html#section-10.2.2)
- ~~202 Accepted~~
- ~~203 Non-Authoritative Information~~
- [204 No Content](https://www.rfc-editor.org/rfc/rfc2616.html#section-10.2.5)
- ~~205 Reset Content~~
- [206 Partial Content](https://www.rfc-editor.org/rfc/rfc2616.html#section-10.2.7)

3xx: Redirection - Further action must be taken in order to complete the request
- [?? 300 Multiple Choices ??](https://www.rfc-editor.org/rfc/rfc2616.html#section-10.3.1)
- [301 Moved Permanently](https://www.rfc-editor.org/rfc/rfc2616.html#section-10.3.2)
- [?? 302 Found ??](https://www.rfc-editor.org/rfc/rfc2616.html#section-10.3.3)
- [?? 303 See Other ??](https://www.rfc-editor.org/rfc/rfc2616.html#section-10.3.4)
- [304 Not Modified](https://www.rfc-editor.org/rfc/rfc2616.html#section-10.3.5)
- ~~305 Use Proxy~~
- ~~306 (Unused)~~
- ~~307 Temporary Redirect~~

4xx: Client Error - The request contains bad syntax or cannot be fulfilled
- [400 Bad Request](https://www.rfc-editor.org/rfc/rfc2616.html#section-10.4.1)
- [?? 401 Unauthorized ??](https://www.rfc-editor.org/rfc/rfc2616.html#section-10.4.2)
- ~~402 Payment Required~~
- [403 Forbidden](https://www.rfc-editor.org/rfc/rfc2616.html#section-10.4.4)
- [404 Not Found](https://www.rfc-editor.org/rfc/rfc2616.html#section-10.4.5)
- [405 Method Not Allowed](https://www.rfc-editor.org/rfc/rfc2616.html#section-10.4.6)
- [406 Not Acceptable](https://www.rfc-editor.org/rfc/rfc2616.html#section-10.4.7)
- ~~407 Proxy Authentication Required~~
- [408 Request Timeout](https://www.rfc-editor.org/rfc/rfc2616.html#section-10.4.9)
- [?? 409 Conflict ??](https://www.rfc-editor.org/rfc/rfc2616.html#section-10.4.10)
- ~~410 Gone~~
- [?? 411 Length Required ??](https://www.rfc-editor.org/rfc/rfc2616.html#section-10.4.12)
- [?? 412 Precondition Failed ??](https://www.rfc-editor.org/rfc/rfc2616.html#section-10.4.13)
- [413 Request Entity Too Large](https://www.rfc-editor.org/rfc/rfc2616.html#section-10.4.14)
- [414 Request-URI Too Long](https://www.rfc-editor.org/rfc/rfc2616.html#section-10.4.15)
- [415 Unsupported Media Type](https://www.rfc-editor.org/rfc/rfc2616.html#section-10.4.16)
- [416 Requested Range Not Satisfiable](https://www.rfc-editor.org/rfc/rfc2616.html#section-10.4.17)
- [417 Expectation Failed](https://www.rfc-editor.org/rfc/rfc2616.html#section-10.4.18)

5xx: Server Error - The server failed to fulfill an apparently valid request
- [500 Internal Server Error](https://www.rfc-editor.org/rfc/rfc2616.html#section-10.5.1)
- [501 Not Implemented](https://www.rfc-editor.org/rfc/rfc2616.html#section-10.5.2)
- ~~502 Bad Gateway~~
- [503 Service Unavailable](https://www.rfc-editor.org/rfc/rfc2616.html#section-10.5.4): The existence of the 503 status code does not imply that a server must use it when becoming overloaded. Some servers may wish to simply refuse the connection.
- ~~504 Gateway Timeout~~
- [505 HTTP Version Not Supported](https://www.rfc-editor.org/rfc/rfc2616.html#section-10.5.6)

## Response Header Fields
```
response-header =   Accept-Ranges | Age | ETag | Location | Proxy-Authenticate 
                    | Retry-After | Server | Vary | WWW-Authenticate  
```

## Body
- All 1xx (informational), 204 (no content), and 304 (not modified) responses MUST NOT include a message-body. All other responses do include a message-body, although it MAY be of zero length.

# Entity

## Entity Header Fields
```
entity-header  = Allow | Content-Encoding | Content-Language | Content-Length | Content-Location 
                | Content-MD5 | Content-Range | Content-Type | Expires | Last-Modified 
```

## Entity Body
```
entity-body := Content-Encoding( Content-Type( data ) )
```
- Content-Type specifies the media type of the underlying data. Any HTTP/1.1 message containing an entity-body SHOULD include a Content-Type header field defining the media type of that body.
- Content-Encoding may be used to indicate any additional content codings applied to the data, usually for the purpose of data compression, that are a property of the requested resource. There is no default encoding.
- If and only if the media type is not given by a Content-Type field, the recipient MAY attempt to guess the media type via inspection of its content and/or the name extension(s) of the URI used to identify the resource. If the media type remains unknown, the recipient SHOULD treat it as type "application/octet-stream".
- The entity-length of a message is the length of the message-body before any transfer-codings have been applied.

# METHODS
## GET
The GET method means retrieve whatever information (in the form of an entity) is identified by the Request-URI.

If the Request-URI refers to a data-producing process, it is the produced data which shall be returned as the entity in the response and not the source text of the process, unless that text happens to be the output of the process.

## POST
The POST method is used to request that the origin server accept the entity enclosed in the request as a new subordinate of the resource identified by the Request-URI in the Request-Line.

The action performed by the POST method might not result in a resource that can be identified by a URI. In this case, either 200 (OK) or 204 (No Content) is the appropriate response status, depending on whether or not the response includes an entity that describes the result.
A quoi ressemble le body d'une POST Response ?

## DELETE
The DELETE method requests that the origin server delete the resource identified by the Request-URI.

# URI (Uniform Resource Identifier)
```
          userinfo       host      port
          ┌──┴───┐ ┌──────┴──────┐ ┌┴┐
  https://john.doe@www.example.com:123/forum/questions/?tag=networking&order=newest#top
  └─┬─┘   └───────────┬──────────────┘└───────┬───────┘ └───────────┬─────────────┘ └┬┘
  scheme          authority                  path                 query           fragment
```


http_URL = "http:" "//" host [ ":" port ] [ abs_path [ "?" query ]]# [CONFIGURING NGINX AS A WEB SERVER](https://docs.nginx.com/nginx/admin-guide/web-server/web-server/)

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


# Testing
- python library to make http requests https://requests.readthedocs.io/en/master/
- quickly and easily send requests https://www.postman.com/

# A faire
- Un signal (SIGINT, fonction), pour quitter proprement le serveur, fermer les connections, etc...
- Une fonction send qui s'assure que toute la data a bien ete envoyé avec la valeur de retour de send()