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

## Architecture
- Une classe Server, qui contient ses parametres, un std::vector<Client*>, 
- Une classe Client, qui contient ses parametres, un pointeur vers son Server, une instance Request, une instance Response
- Une classe Request, pour stocker la requete HTTP a recevoir
- Une classe Response, pour stocker la reponse HTTP a renvoyer

## Logique multi-serveurs
Soit un select() sur les fd_set de tous les serveurs, puis une boucle pour que chaque serveur traite les evenements.
Soit un pthread par serveur, qui gere chacun leurs fd_set et select(), avec un mutex pour accept() et un mutex pour l'ecriture de fichiers.

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
- Dans le cas de multiples serveurs, est ce qu'on fait tourner les serveurs sur un seul thread dans une boucle, ou on lance un thread par serveur ?
- Mettre le config file comme une struct globale ?
- Concept de chunk et utilité dans le projet ?
- IPV4/IPV6 ? Aucune mention dans le Discord, on reste sur du IPv4 pour le moment
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

# Message
```
generic-message = start-line
                    *(message-header CRLF)
                    CRLF
                    [ message-body ]
start-line      = Request-Line | Status-Line
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

# Testing
- python library to make http requests https://requests.readthedocs.io/en/master/
- quickly and easily send requests https://www.postman.com/

# A faire
- Un signal (SIGINT, fonction), pour quitter proprement le serveur, fermer les connections, etc...
- Une fonction send qui s'assure que toute la data a bien ete envoyé avec la valeur de retour de send
- Proteger une erreur de select() en cas d' interruption par un signal, errno = EINTR comme [ici](http://www.beej.us/guide/bgnet/html/#:~:text=Why%20does%20select()%20keep%20falling%20out%20on%20a%20signal%3F)


http_URL = "http:" "//" host [ ":" port ] [ abs_path [ "?" query ]]