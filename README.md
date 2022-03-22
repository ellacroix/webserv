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
- Peut on avoir plusieurs requetes HTTP en attente d'un meme Client ?`
- Concept de chunk et utilité dans le projet ?
- IPV4/IPV6 ? Aucune mention dans le Discord, on reste sur du IPv4 pour le moment


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
- RFCs big picture, in french http://abcdrfc.online.fr/rfc-vf/pdf/rfc2616.pdf
- RFCs 7230-7235 overview, in french https://www.bortzmeyer.org/http-11-reecrit.html
- overview http://www.iprelax.fr/http/http_art.php
- another overview https://www.commentcamarche.net/contents/520-le-protocole-http
- Request and Response format https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages
- another overview https://www.coozook.com/static/book-samples/B212934F6A-sample.pdf

### http headers in details
- MIME list https://developer.mozilla.org/fr/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Common_types
- What HTTP response headers are required https://stackoverflow.com/questions/4726515/what-http-response-headers-are-required
- response status code https://fr.wikipedia.org/wiki/Liste_des_codes_HTTP
- auth theory https://developer.mozilla.org/en-US/docs/Web/HTTP/Authentication#Basic_authentication_scheme
- auth practice https://stackoverflow.com/questions/180947/base64-decode-snippet-in-c

### CGI programming
- nice overview https://perso.liris.cnrs.fr/lionel.medini/enseignement/M1IF03/Tutoriels/Tutoriel_CGI_SSI.pdf
- some details https://www.developpez.net/forums/d151285/php/langage/php-js-quoi-sert-php-cgi-exe-repertoire-php/

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
Request               = Request-Line              ; Section 5.1
                        *(( general-header        ; Section 4.5
                         | request-header         ; Section 5.3
                         | entity-header ) CRLF)  ; Section 7.1
                        CRLF
                        [ message-body ]          ; Section 4.3
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


## Body
- The presence of a message-body in a request is signaled by the inclusion of a Content-Length or Transfer-Encoding header field in the request's message-headers.
- A message-body MUST NOT be included in a request if the specification of the request method (section 5.1.1) does not allow sending an entity-body in requests.
- A server SHOULD read and forward a message-body on any request; if the request method does not include defined semantics for an entity-body, then the message-body SHOULD be ignored when handling the request.

# Response
- If a request contains a message-body and a Content-Length is not given, the server SHOULD respond with 400 (bad request) if it cannot determine the length of the message, or with 411 (length required) if it wishes to insist on receiving a valid Content-Length.
## Start-line
### Status codes to implement
1XX:
2XX:
3XX:
4XX:
- 400 (Bad Request)
- 405 (Method Not Allowed)
5XX:
- 501 (Not Implemented)
## Headers

## Body
- All 1xx (informational), 204 (no content), and 304 (not modified) responses MUST NOT include a message-body. All other responses do include a message-body, although it MAY be of zero length.

# METHODS
## GET
## POST
## DELETE

# Testing

- python library to make http requests https://requests.readthedocs.io/en/master/
- quickly and easily send requests https://www.postman.com/

# A faire
- Un signal (SIGINT, fonction), pour quitter proprement le serveur, fermer les connections, etc...
- Une fonction send qui s'assure que toute la data a bien ete envoyé avec la valeur de retour de send
- Proteger une erreur de select() en cas d' interruption par un signal, errno = EINTR comme [ici](http://www.beej.us/guide/bgnet/html/#:~:text=Why%20does%20select()%20keep%20falling%20out%20on%20a%20signal%3F)
