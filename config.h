#define BUF_SIZE 4096

#define SERVER_ADDR "127.0.0.1"

#define CMD_STOP "stop"

#define CROSS_DOMAIN_POLICY \
    "<cross-domain-policy>\n" \
      "<site-control permitted-cross-domain-policies=\"master-only\"/>\n" \
      "<allow-access-from domain=\"wiavia.com\" to-ports=\"3001\" />\n" \
      "<allow-access-from domain=\"*.wiavia.com\" to-ports=\"3001\" />\n" \
      "<allow-access-from domain=\"wiavia.com\" to-ports=\"3002\" />\n" \
      "<allow-access-from domain=\"*.wiavia.com\" to-ports=\"3002\" />\n" \
      "<allow-access-from domain=\"portcamapp.com\" to-ports=\"3001\" />\n" \
      "<allow-access-from domain=\"*.portcamapp.com\" to-ports=\"3001\" />\n" \
      "<allow-access-from domain=\"portcamapp.com\" to-ports=\"3002\" />\n" \
      "<allow-access-from domain=\"*.portcamapp.com\" to-ports=\"3002\" />\n" \
      "<allow-access-from domain=\"localhost\" to-ports=\"3001\" />\n" \
      "<allow-access-from domain=\"127.0.0.1\" to-ports=\"3001\" />\n" \
    "</cross-domain-policy>\n"

#define CONFIG_SSL_PEM "ssl.pem.fullpath"
#define CONFIG_SSL_TMP_DH "ssl.tmp_dh.fullpath"
#define CONFIG_HTTP_PORT "server.http_port"
#define CONFIG_CONTROL_PORT "server.control_port"

#define CONFIG_DB_HOST "db.host"
#define CONFIG_DB_USERNAME "db.username"
#define CONFIG_DB_PASSWORD "db.password"
#define CONFIG_DB_NAME "db.name"
