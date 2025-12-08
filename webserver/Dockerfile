# Étape 1 : Compilation de l'application
FROM gcc:13.4.0-bookworm@sha256:0e862e47a2e81de1e25ec405ed85dc4d9d5c669bd60a392f654f86d4613d1940 AS build

WORKDIR /app

RUN --mount=type=bind,source=./src,target=/app/src \
    --mount=type=bind,source=./src/dependencies/mongoose,target=/app/mongoose \
    gcc /app/src/main.c /app/mongoose/mongoose.c -o server -I /app/mongoose -pthread -static

# Étape 2 : Création de l'image finale
FROM alpine:3.22.0@sha256:8a1f59ffb675680d47db6337b49d22281a139e9d709335b492be023728e11715 AS final

ARG port=80

WORKDIR /app

COPY --from=build /app/server /app/server
RUN apk add --no-cache libstdc++ \
    && groupadd -g 1000 app \
    && useradd -u 1000 -g app -s /bin/sh -d /app app \
    && chmod +x /app/server
EXPOSE ${port}

# Set the entrypoint to run the server
ENTRYPOINT ["/app/server"]
CMD ["--port", "${port}"]

LABEL server="Clio's Web Server"
LABEL maintainer="Loïc GLANOIS <loic.glanois@ynov.com>"
LABEL version="1.0"
LABEL description="A modest web server infrastructure for my personal needs."
# syntax=docker/dockerfile:experimental
# This Dockerfile builds a simple HTTP server using the Mongoose library.