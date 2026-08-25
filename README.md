<div align="center">

# 🌐 webserv

**An HTTP/1.1 compliant web server built from scratch in C++98**

[![42 School](https://img.shields.io/badge/42-School_Project-000000?style=for-the-badge&logo=42&logoColor=white)](https://42.fr)
[![Language](https://img.shields.io/badge/Language-C++98-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)](https://en.wikipedia.org/wiki/C%2B%2B)
[![HTTP](https://img.shields.io/badge/HTTP-1.1-FF6F61?style=for-the-badge)](https://datatracker.ietf.org/doc/html/rfc2616)

> *A fully functional web server inspired by NGINX — handling HTTP requests, serving static files, executing CGI scripts, and managing multiple virtual hosts, all with non-blocking I/O.*

</div>

---

## ✨ Features

| Feature | Description |
|---|---|
| 🔄 **Non-blocking I/O** | Single-threaded event loop using `poll()` for efficient multiplexing |
| 📄 **Static File Serving** | Serves HTML, CSS, JS, images with automatic MIME type detection |
| ⚙️ **CGI Support** | Executes Python scripts with full environment variable setup |
| 📤 **File Uploads** | Handles multipart/form-data with configurable upload paths |
| 🏠 **Virtual Hosting** | Multiple server blocks on the same port via `Host` header |
| 🔀 **HTTP Redirections** | Configurable 301/302 redirects |
| 📂 **Directory Listing** | Auto-generated HTML directory index (autoindex) |
| 🛡️ **Custom Error Pages** | Per-server configurable error pages (400–504) |
| 🔧 **NGINX-like Config** | Familiar configuration syntax with server and location blocks |

---

## 🏗️ Architecture

```
┌─────────────────────────────────────────────┐
│              ServerManager                   │
│         (poll() event loop)                  │
│                                              │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐   │
│  │ Server 1 │  │ Server 2 │  │ Server N │   │
│  │ :8080    │  │ :8443    │  │ :3000    │   │
│  │          │  │          │  │          │   │
│  │ Location │  │ Location │  │ Location │   │
│  │ /        │  │ /api     │  │ /static  │   │
│  │ /uploads │  │ /cgi-bin │  │ /docs    │   │
│  └──────────┘  └──────────┘  └──────────┘   │
│                                              │
│  ┌──────────────────────────────────────┐    │
│  │            Client Pool               │    │
│  │  READ → PARSE → PROCESS → RESPOND   │    │
│  └──────────────────────────────────────┘    │
└─────────────────────────────────────────────┘
```

### Core Components

| Class | Responsibility |
|---|---|
| `ServerManager` | Central event loop — accepts connections, dispatches I/O via `poll()` |
| `Server` | Virtual server bound to a host:port, manages locations and config |
| `Client` | Per-connection state machine (reading → parsing → processing → sending) |
| `HTTPRequest` | Parses method, URI, headers, body (supports chunked encoding) |
| `HTTPResponse` | Generates responses — static files, directory listings, errors, redirects |
| `CGI` | Fork/exec pipeline for script execution with timeout handling |
| `ConfigParser` | Tokenizer + recursive descent parser for NGINX-like config files |
| `Location` | URL routing with per-path configuration overrides |

---

## ⚙️ Configuration

The server uses an NGINX-inspired configuration format:

```nginx
server {
    listen 8080;
    host 0.0.0.0;
    server_name localhost;
    root ./www/default;
    index index.html;
    client_max_body_size 10M;

    error_page 404 /errors/404.html;
    error_page 500 /errors/500.html;

    location / {
        allowed_methods GET POST;
        autoindex off;
    }

    location /uploads {
        allowed_methods GET POST DELETE;
        upload_path ./www/uploads;
        client_max_body_size 50M;
    }

    location /cgi-bin {
        allowed_methods GET POST;
        cgi_path /usr/bin/python3;
        cgi_ext .py;
    }

    location /redirect {
        return 301 http://example.com;
    }
}
```

### Directives Reference

| Directive | Scope | Description |
|---|---|---|
| `listen` | server | Port to bind to |
| `host` | server | Host/IP address |
| `server_name` | server | Virtual server hostname |
| `root` | server/location | Document root directory |
| `index` | server/location | Default index file |
| `client_max_body_size` | server/location | Maximum request body size |
| `error_page` | server | Custom error page mapping |
| `allowed_methods` | location | Permitted HTTP methods |
| `autoindex` | location | Directory listing (on/off) |
| `upload_path` | location | Upload destination directory |
| `cgi_path` | location | Path to CGI interpreter |
| `cgi_ext` | location | CGI script file extension |
| `return` | location | HTTP redirect (code + URL) |

---

## 🏗️ Build & Run

### Prerequisites

- **Compiler**: `c++` with C++98 support
- **OS**: Linux / macOS

### Compile

```bash
git clone https://github.com/ouvled86/webserv.git
cd webserv
make
```

### Launch

```bash
# Start with default configuration
./webserv config/default.conf

# Access in browser
open http://localhost:8080
```

---

## 🌐 Supported HTTP Features

### Methods
- **GET** — Static file serving, directory listing, CGI execution
- **POST** — File uploads (multipart/form-data), CGI, URL-encoded data
- **DELETE** — File deletion with proper status codes

### Protocol
- HTTP/1.1 compliant (partial)
- Chunked transfer encoding
- Keep-alive connections
- Cookie passthrough to CGI
- Proper status codes and reason phrases

---

## 📁 Project Structure

```
webserv/
├── Makefile
├── config/
│   └── default.conf              # Server configuration file
├── includes/
│   ├── webserv.hpp               # Common includes & defines
│   ├── ServerManager.hpp         # Event loop manager
│   ├── Server.hpp                # Virtual server class
│   ├── Client.hpp                # Client connection handler
│   ├── HTTPRequest.hpp           # Request parser
│   ├── HTTPResponse.hpp          # Response generator
│   ├── CGI.hpp                   # CGI executor
│   ├── ConfigParser.hpp          # Configuration parser
│   ├── Location.hpp              # URL routing rules
│   └── StatusCode.hpp            # HTTP status code mapping
├── srcs/
│   ├── main.cpp                  # Entry point
│   ├── ServerManager.cpp         # poll() event loop
│   ├── Server.cpp                # Server socket setup
│   ├── Client.cpp                # Client state machine
│   ├── HTTPRequest.cpp           # Request parsing logic
│   ├── HTTPResponse.cpp          # Response generation
│   ├── CGI.cpp                   # Fork/exec CGI handling
│   ├── ConfigParser.cpp          # Config tokenizer & parser
│   ├── ConfigParser_validate.cpp # Config validation
│   ├── Location.cpp              # Location matching
│   └── StatusCode.cpp            # Status code registry
└── www/
    ├── cgi-bin/                   # CGI scripts (Python)
    │   ├── env.py                 # Environment variable dumper
    │   ├── post.py                # POST data handler
    │   └── upload.py              # File upload processor
    ├── default/                   # Default server root
    │   └── index.html
    ├── errors/                    # Custom error pages
    │   ├── 400.html ... 504.html
    │   └── styles.css
    ├── test/                      # Test pages
    │   ├── index.html
    │   ├── upload.html
    │   └── cookies.html
    └── uploads/                   # Upload destination
```

---

<div align="center">

Made with ☕ at **[1337 School](https://1337.ma)** (42 Network)

</div>
